/*THIS IS THE CODE FOR THE SERVER*/
//Imports
#include "mergesort.c"
#include "sorter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <errno.h>
#define PORT 8080


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
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    // Create the socket w/ TCP connection
    setupSocket = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    // Configure Server Settings
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNum);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // TODO: Change from local host to some other IP address
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    // Bind the address struct to the socket
    bind(setupSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    // Listen to the socket and set the maximum number of connections(which we set to 255 because that is the max number of threads)
    if(listen(setupSocket, 255) == 0){
        printf("Server has been initialized. Listening...\n");
    } else {
        printf("Error: Something went wrong with initializing the listener.\n");
        return 0;
    }

    // Create a new socket for the incoming connection
    addr_size = sizeof serverStorage;

    // Create client thread tracker
    pthread_t client_threadid;
    
    // Create new client sockets and spawn new threads
    printf("Waiting for connections...\n");
    while((clientSocket = accept(setupSocket, (struct sockaddr *) &serverStorage, &addr_size)) != -1){ // TODO: Needs to handle signal kill
        // TODO: Check request type
        // Print the IP address of the client
        printf("Connection received from %d\n", clientSocket);

        // Get data from the client
        char buffer[65535];
        int len;
        if((len = recv(clientSocket,buffer,sizeof(buffer) - 1)) < 0){
            printf("%s\n", strerror(errno));
            printf("Error: Could not read from socket\n");
        }
        buffer[len] = '\0';

        // Get the request string
        char request[5];
        strncpy(request, buffer, 4);
        request[4] = '\0';

        // Check to see the request type
        if(strcmp(request, "SORT") == 0){
            printf("Sorting...\n");
            // Spawn new thread to sort the file
            pthread_t tid;
            // TODO: call some function to sort and do whatever. And send whatever args
            pthread_create(&tid, NULL, process_csv, (void*)args);
            pthread_join(tid_dir, NULL);
            
        } else if(strcmp(request, "DUMP") == 0){
            printf("Sorting then dumping...\n");
            // Spawn new thread to sort the datastructure and dump it to client
        }

    }


    return 0;
}

void * handle_sort_request(void *args){


}

