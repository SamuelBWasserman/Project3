/*THIS IS THE CODE FOR THE SERVER*/
//Imports
#include "mergesort.c"
#include "sorter_server.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>


// Stores the first line of the CSV. (Represents the column headers)
char *first_line;
// Create the locks for the threads
// pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **(argv)){
    // Check for correct number or args
    if(argc != 3) {
    printf("Error: Invalid Number of Arguments\n");
    return 0;
    }

    // Check for correct flag
    if(strcmp(argv[1], "-p") != 0){
        printf("Error: Incorrect flag/parameter\n");
        return 0;
    }

    // Check for correct port number
    int portNum;
    if((portNum = atoi(argv[2])) == 0){
        printf("Error: Value for argument 3(Port Number) is incorrect\n");
        return 0;
    }

    // Set socket params
    int setupSocket, clientSocket;
    char message[65535]; // Message set to max number of TCP bytes
    struct addrinfo hints, *results;
    socklen_t addr_size;

    // Create the socket w/ TCP connection
    setupSocket = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    // Configure Server Settingss
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Get address information
    int s;
    if((s = getaddrinfo(NULL, argv[2], &hints, &results)) != 0){
        printf("getaddrinfo: %s\n", gai_strerror(s));
        return 0;
    }

    // Bind the address struct to the socket
    bind(setupSocket, results->ai_addr, results->ai_addrlen);

    // Listen to the socket and set the maximum number of connections(which we set to 255 because that is the max number of threads)
    if(listen(setupSocket, 255) == 0){
        printf("Server has been initialized. Listening...\n");
    } else {
        printf("Error: Something went wrong with initializing the listener.\n");
        return 0;
    }

    // Create resulting address
    struct sockaddr_in *result_addr = (struct sockaddr_in *)results->ai_addr;
    printf("Listening on FD %d and port %d\n", setupSocket, ntohs(result_addr->sin_port));

    // Create client TID array
    pthread_t *client_tids = (pthread_t *)malloc(sizeof(pthread_t));
    int max_tid = 0;
    
    // Create new client sockets and spawn new threads
    printf("Waiting for connections...\n");
    while((clientSocket = accept(setupSocket, NULL, NULL)) != -1){ // TODO: Needs to handle signal kill
        // Print the IP address of the client
        struct sockaddr_in addr;
        socklen_t addr_size = sizeof(struct sockaddr_in);
        int res = getpeername(clientSocket, (struct sockaddr *)&addr, &addr_size);
        printf("Connection received from %d\n", inet_ntoa(addr.sin_addr));
        
        // Put clientSocket into struct to pass to thread
        client_args *clientArgs = (client_args *)malloc(sizeof(client_args));
        clientArgs->client_sock = clientSocket;

        // Spawn a new thread
        pthread_t client_threadid;
        pthread_create(&client_threadid, NULL, handle_connection, (void *)clientArgs);

        // Add TID to TID array
        client_tids[max_tid] = client_threadid;
        client_tids = (pthread_t *)realloc(client_tids, sizeof(client_tids) + sizeof(pthread_t));
        max_tid++; 
    }
    
    // Wait for all child threads to complete
    int i;
    for(i=0;i<max_tid;i++){
        pthread_join((pthread_t)client_tids[i], NULL);
    }
    
    return 0;
}

void *handle_connection(void *arg){
    // Create an integer to hold the client IP value
    client_args *c_args = arg;
    int client_sock = c_args -> client_sock;
    // Create global variables to store CSV data
    data_row **big_db;
    int big_lc = 0;
    // Get data from the client
    char buffer[65535];
    int len;
    // TODO Optimization: Let's spawn a thread to handle this while loop.
    while((len = read(client_sock, buffer, sizeof(buffer) - 1)) >= 0){
        buffer[len] = '\0';

        // Get the request string
        char request[5];
        strncpy(request, buffer, 4);
        request[4] = '\0';

        // Check to see the request type
        if(strcmp(request, "DUMP") == 0){
            printf("Sorting then dumping...\n");
            // Get the remaining 2 args
            char column[3];
            strtok(buffer, "-");
            strtok(buffer, "-");
            strcpy(column, buffer);
            int column_to_sort = atoi(column);
            strtok(buffer, "-");
            char data[3];
            strcpy(data, buffer);
            int data_flag = atoi(data);
            char buf[PATH_MAX];
            // Sort the big DB
            sort(big_db, column_to_sort, data_flag,0,big_lc-1);
        
            char file_name[200];
            // Creates a relative path /sorter_files/All-Files-Sorted-COLUMN.csv
            strcpy(file_name,"/sorter_files/All-Files-Sorted");
            strcpy(file_name,"-");
            strcpy(file_name, column);
            strcpy(file_name, ".csv");
            char *file_path = realpath(file_name, buf);
            
            int sent_bytes = 0;
            int offset;
            int remain_data;
            ssize_t len;
            
            // Creates a file from big_db
            print_to_csv(big_db, big_lc, file_path, first_line);
            FILE *csv = fopen(file_path, "r");
            
            struct stat file_stat;
	        fstat(fileno(csv), &file_stat);
	        offset = 0;
            remain_data = file_stat.st_size;
            
            // Get file size
            fseek(csv, 0, SEEK_END);
            char file_size[256];
            sprintf(file_size, "%d", file_stat.st_size);
             // Sending file size 
            len = send(client_sock, file_size, sizeof(file_size),0);
            
           
            /* Sending file data */
            while (((sent_bytes = sendfile(client_sock, fileno(csv), &offset, BUFSIZ)) > 0) && (remain_data > 0))
            {
                remain_data -= sent_bytes;
            }

        
        } else { // This is Sort and a file descriptor is provided
            printf("Adding to Mega DB...\n");
            // Receive the file size, needed to receive the file
            int file_size;
            recv(client_sock, buffer, BUFSIZ, 0);
            file_size = atoi(buffer);
            int remaining_data = 0;
            char buf[PATH_MAX];
            char file_name[200]; // Arbitrary length
        
            // Creates a relative path /sorter_files/tid#.csv
            strcpy(file_name,"/sorter_files/");
            int a = (int)pthread_self();
            char string_buffer[20];
            snprintf(string_buffer, 6, "%s", a);
            strcpy(file_name, string_buffer);
            strcpy(file_name, ".csv");
        
            char *file_path = realpath(file_name, buf);
            FILE *csv_file = (FILE*) fopen(file_path, "w");
            if (csv_file == NULL)
            {
                fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
            }
            ssize_t len;
            remaining_data = file_size;
            while (((len = recv(client_sock, buffer, BUFSIZ, 0)) > 0) && (remaining_data > 0))
            {
                fwrite(buffer, sizeof(char), len, csv_file);
                remaining_data -= len;
            }
        
            process_csv(csv_file, big_db, big_lc);
            // Remove file that was created, after data is stored in memory
            fclose(csv_file);
            remove(file_path);
        }
    }
    close(client_sock);
    pthread_exit(0);
}


/* This function takes in a csv_file, and appends the rows of data to a data structure */
/* in the heap for later sorting */
void process_csv(FILE *csv_file, data_row **big_db, int big_lc){
  printf("%d, ", pthread_self());
  /* Processes the CSV file */
  // cast the arguments passed from pthread_create
  // thread_args *t_args = args;
  
  
  // Define path variables
  char curr_dir[_POSIX_PATH_MAX] = {0};
  char *path = NULL;
  if(csv_file==NULL){
     printf("NULL FILE exiting\n");
     exit(1);
  }
  // printf("Processing CSV\n");
  // printf("%s\n",file_path);
  char delims[] = ",";
  // data_row **db = (data_row**)malloc(sizeof(data_row)); // 1 data row
  big_db[big_lc] = (data_row*)malloc(sizeof(data_row)); // 1 data row
  char line[600]; // one line from the file
  int line_counter = -1; // count what line we're on to keep track of the struct array
  int word_counter = 0; // keep track of what word were on for assignment in the struct
  int type_flag = 0; // 0:STRING, 1:INT, 2:FLOAT

  while(fgets(line, 600, csv_file) != NULL){
  	//pthread_mutex_lock(&MUTEX);
    int i;
    if(line_counter < 0){
      line_counter++;
      if(first_line == NULL){
        first_line = (char *) malloc(sizeof(char)*strlen(line)+1);
        strcpy(first_line,line);
      }
      if(!(is_csv_correct(line))){
        printf("Incorrect CSV");
        fflush(stdout);
        return;
      }
      //pthread_mutex_unlock(&MUTEX);
      continue;
    }
    
    //IF first char is ',' in the line
    if(line[0] == ','){
      char null_val[15];
	    char templine[600];
      sprintf(null_val, "NULL_VALUE%d", line_counter);
	    strcpy(templine,null_val);
	    templine[strlen(null_val)] = '\0';
	    strcat(templine, line);
	    strcpy(line, templine);
    }

    //IF ",," exists in the line
    for(i = 0;i<strlen(line);i++){
	    if(line[i] == ',' && line[i+1] == ','){
	      char null_val[15];
	      char templine[600]; // Buffer to put the modified line in
	      strncpy(templine,line,i+1); //TODO: Change this to i+1 if it copies incorrent # of chars
	      templine[i+1] = '\0';
	      sprintf(null_val, "NULL_VALUE%d", line_counter);
	      strcat(templine,null_val);
	      strcat(templine,line+i+1);
	      strcpy(line, templine);
	    }
    }

    //IF the line ends with a ','
    if(line[strlen(line) -1] == ','){
      char null_val[15];
      sprintf(null_val, "NULL_VALUE%d", line_counter);
      strcat(line,null_val);
    }

    // Tokenize until end of line
    char *word;
    word = strtok(line, delims);
    
    while(word){
      //IF string has commas in the middle somwhere
	    if(word[0] == '\"'){
        char buffer[100];
        strcpy(buffer, word);
	      word = strtok(NULL, ",");
	      while(strpbrk(word, "\"") == NULL){
	        strcat(buffer, ",\0");
	        strcat(buffer, word);
	        word = strtok(NULL, ",");
	      }
	      strcat(buffer, ",\0");
        strcat(buffer, word);
	      strcat(buffer, "\0");
        big_db[big_lc]->col[word_counter] = (char *)malloc((strlen(buffer)+1)*sizeof(char));
	      strcpy(big_db[big_lc]->col[word_counter], buffer);
	      word_counter++;
	      word = strtok(NULL,",");
	      continue;
      }

      // Allocate enough space for the string to be placed in the array
      big_db[big_lc]->col[word_counter] = (char *)malloc((strlen(word)+1)*sizeof(char));
      // Copy the string into the array and add trailing string ender
      strcpy(big_db[big_lc]->col[word_counter], word);
      // Move to the next token
      word_counter++;
      word = strtok(NULL, delims);
    }
    word_counter = 0;
    line_counter++;
    big_lc++;
    big_db = (data_row**)realloc(big_db, (sizeof(data_row)*(big_lc+1)));
    big_db[big_lc] = (data_row*)malloc(sizeof(data_row));
    // pthread_mutex_unlock(&MUTEX);
  }
   pthread_exit(0);
}

void print_to_csv(data_row **db, int line_counter, char *file_path_name, char *first_line) {
  char buffer[200];
  
  FILE *f;
  f = fopen(file_path_name, "w");
  int i, j;
  for (i = -1; i < line_counter; i++) {
    //Print first line to csv
    if(i == -1){
    	fprintf(f, first_line);
    	continue;
    }
    for (j = 0; j < 28; j++) {
    
      if(strstr(db[i]->col[j],"NULL") != NULL){
	    fprintf(f,",");
	    if(j == 27){
	        fprintf(f,"\n");
	    }
	    continue;
      }
      
      if(j != 27){
      	char tmp[200];
        strcpy(tmp,db[i]->col[j]);
        strcat(tmp,",\0");
     	fprintf(f,tmp);
     	continue;
	  }
      fprintf(f,db[i]->col[j]);
    }
   }
}

