/*THIS IS THE CODE FOR THE SERVER*/
//Imports
#include "mergesort.c"
#include "sorter.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

// Create global variables to store CSV data
data_row **big_db;
int big_lc = 0;

// Create the locks for the threads
pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

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
        printf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return 0;
    }

    // Bind the address struct to the socket
    bind(setupSocket, result->ai_addr, result->ai_ddrlen);

    // Listen to the socket and set the maximum number of connections(which we set to 255 because that is the max number of threads)
    if(listen(setupSocket, 255) == 0){
        printf("Server has been initialized. Listening...\n");
    } else {
        printf("Error: Something went wrong with initializing the listener.\n");
        return 0;
    }

    // Create resulting address
    struct sockaddr_in *result_addr = (struct sockaddr_in *)result->ai_addr;
    printf("Listening on FD %d and port %d\n", setupSocket, ntohs(result_addr->sin_port));

    // Create client TID array
    pthread_t *client_tids = (pthread_t *)malloc(sizeof(pthread_t));
    int max_tid = 0;
    
    // Create new client sockets and spawn new threads
    printf("Waiting for connections...\n");
    while((clientSocket = accept(setupSocket, NULL, NULL)) != -1){ // TODO: Needs to handle signal kill
        // Print the IP address of the client
        printf("Connection received from %d\n", clientSocket);

        // Spawn a new thread
        pthread_t client_threadid;
        pthread_create(&client_threadid, NULL, handle_connection, (void *)clientSocket);

        // Add TID to TID array
        client_tids[max_tid] = client_threadid;
        client_tids = (pthread_t *)realloc(sizeof(client_tids) + sizeof(pthread_t));
        max_tid++;
    }

    return 0;
}

void *handle_connection(void *arg){
    // Create an integer to hold the client IP value
    int client_sock = (int)arg;

    // Get data from the client
    char buffer[65535];
    int len;
    if((len = read(client_sock, buffer, sizeof(buffer) - 1)) < 0){ // Rewrite to use sendfile instead!
        printf("%s\n", strerror(errno));
        printf("Error: Could not read from socket\n");
    }
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
        strok(buffer, "-");
        strok(buffer, "-");
        strcpy(column, buffer);
        int column_to_sort = atoi(column);
        strok(buffer, "-");
        char data[3];
        strcpy(data, buffer);
        int data_flag = atoi(data);
        
        // Sort the big DB
        sort(big_db, column_to_sort, data_flag,0,big_lc-1);

        // TODO: Call print_to_csv and use sendfile to send it back
    } else { // This is Sort and a file descriptor is provided
        printf("Adding to Mega DB...\n");
        // TODO: Add to megaDB
        FILE *csv_file = (FILE*) fdopen(client_sock, "r");
        // TODO: call process_csv() with some args of some sort
        // TODO: return a response possibly
    }

    pthread_exit(1);
}
