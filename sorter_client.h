#ifndef SORTER_CLIENT_H_
#define SORTER_CLIENT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h> 
#include <sys/mman.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/sendfile.h>
#include <errno.h>

/* function prototypes */
void outputErrorMessage(char *error);
char* attachName(const char* directoryName, const char* name);
char* attachSorted();
void outputData();
int switchVariable(const char* columnType);
int checkCSV(char *filename);
int checkRepeat(char *name);
void* traverseDirectory(void* arg);
void sendRequest(char*);
void gotoxy(int x, int y);
int determine_data_type(int);

#endif
