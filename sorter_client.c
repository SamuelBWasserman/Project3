#include "sorter_client.h"

#define BUFFER_SIZE 2048 // large enough buffer size

int switchValue;
char* columnType;
char *outputDirectory;
char *hostName;
char *portNumber;
pthread_t *tids;
int threadCount = 0;
int threadSize = 0;
int sockfd = 0;
// int total = 0;
// pthread_mutex_t listMutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char** argv) {
    if(argc != 7 && argc != 9 && argc != 11) { //Make sure input is valid.
    	outputErrorMessage("must enter 7, 9 or 11 arguments");
    }
    if((strcmp(argv[1], "-c") != 0) || (strcmp(argv[3], "-h") != 0) || (strcmp(argv[5], "-p") != 0)) {
    	outputErrorMessage("invalid argument input");
    }

    char* directoryName = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    columnType = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    outputDirectory = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    hostName = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    portNumber = (char*)malloc(sizeof(char)*BUFFER_SIZE);

    if (argc == 7) {
    	strcpy(columnType, argv[2]);
    	strcpy(hostName, argv[4]);
    	strcpy(portNumber, argv[6]);
    	strcpy(directoryName, ".");
    	strcpy(outputDirectory, ".");
    }
    else if (argc == 9) {
    	strcpy(columnType, argv[2]);
    	strcpy(hostName, argv[4]);
    	strcpy(portNumber, argv[6]);
    	strcpy(directoryName, argv[8]);
    	strcpy(outputDirectory, ".");
    }
    else if (argc == 11) {
    	strcpy(columnType, argv[2]);
    	strcpy(hostName, argv[4]);
    	strcpy(portNumber, argv[6]);
    	strcpy(directoryName, argv[8]);
    	strcpy(outputDirectory, argv[10]);
    }

    switchValue = switchVariable(columnType);

    // fprintf(stdout, "Initial PID: %d\n", getpid());
    // fprintf(stdout, "TIDS of all child threads: ");
    // fflush(stdout);

    fprintf(stderr, "Column Type  : [%s]\n", columnType);
    fprintf(stderr, "Column Number: [%d]\n", switchValue);
    fprintf(stderr, "Host Name    : [%s]\n", hostName);
    fprintf(stderr, "Port Number  : [%s]\n", portNumber);
    fprintf(stderr, "Directory    : [%s]\n", directoryName);
    fprintf(stderr, "Output       : [%s]\n\n", outputDirectory);

    /* Create a socket first */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    	outputErrorMessage("could not create socket");
    }

    int s;
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((s = getaddrinfo(hostName, portNumber, &hints, &result)) != 0) {
    	outputErrorMessage("error with making sock");
    }

    /* Attempt a connection */
    if(connect(sockfd, result->ai_addr, result->ai_addrlen) == -1) {
    	outputErrorMessage("connect failed");
    }

    // printf("Connected to ip: %s : %d\n",inet_ntoa(serv_addr.sin_addr),ntohs(serv_addr.sin_port));

    threadSize = threadSize + 1;
    tids = (pthread_t*)malloc(sizeof(pthread_t) * threadSize);
    pthread_create(&tids[threadCount], NULL, traverseDirectory, directoryName);
    threadCount = threadCount + 1;

    int x;
    for (x = 0; x < threadSize; x++) {
    	pthread_join(tids[x], NULL);
    }

    int sentconf;
    int bytesReceived = 0;
    char recvBuff[1024];
    memset(recvBuff, '0', sizeof(recvBuff));
    char mesg[8];
    char *dump = "DUMP-";
    int columnNumber = switchVariable(columnType);
    int dataType = 0;
    char columnNumberString[1];
    char dataTypeString[1];
    char *dash = "-";
    // itoa(columnNumber,columnNumberString,10);
    // itoa(dataType,dataTypeString,10);
    snprintf(columnNumberString, sizeof(char), "%d", columnNumber);
    snprintf(dataTypeString, sizeof(char), "%d", dataType);
    strcpy(mesg, dump);
    strcat(mesg,columnNumberString);
    strcat(mesg,dash);
    strcat(mesg,dataTypeString);
    sentconf = send(sockfd, mesg, strlen(mesg), 0);

   	 /* Create file where data will be stored */
    FILE *fp;
    char* outputName = (char*)malloc(strlen(attachSorted()) + 1);
    strcpy(outputName, attachSorted());
    char* fName = (char*)malloc(strlen(outputDirectory)+strlen(outputName)+ 3);
    strcpy(fName, attachName(outputDirectory, outputName));
    read(sockfd, fName, 256);
	//strcat(fname,"AK");
    printf("File Name: %s\n", fName);
    printf("Receiving file...");
    fp = fopen(fName, "ab"); 
    if(NULL == fp)
    {
    	outputErrorMessage("error opening file");
    }
    long double sz=1;
    /* Receive data in chunks of 256 bytes */
    while((bytesReceived = read(sockfd, recvBuff, 1024)) > 0)
    { 
    	sz++;
    	gotoxy(0,4);
    	printf("Received: %Lf Mb",(sz/1024));
    	fflush(stdout);
        // recvBuff[n] = 0;
    	fwrite(recvBuff, 1, bytesReceived, fp);
        // printf("%s \n", recvBuff);
    }

    if(bytesReceived < 0)
    {
    	printf("\n Read Error \n");
    }
    printf("\nFile OK....Completed\n");

    close(sockfd);

    // pFirstNode = SortList(pFirstNode, switchValue, threadSize);
    // outputData();
    // fprintf(stdout, "\nTotal number of threads: %d\n", threadSize);

    return 0;
}

void* traverseDirectory(void* arg) {
	// fprintf(stderr, "%lu, ", (unsigned long)pthread_self());
	char* directoryName = (char*)malloc(strlen((char*) arg) + 1);
	strcpy(directoryName, (char*) arg);
	// printf("Directory Name: [%s]\n", directoryName);
	DIR *dirStream;
	struct dirent *dir;
	dirStream = opendir(directoryName);

	if(dirStream == NULL) {
		outputErrorMessage("incorrect path");
	}

	while ((dir = readdir(dirStream)) != NULL) {
		char* newPath = (char*)malloc(strlen(directoryName)+strlen(dir->d_name)+ 3);
		strcpy(newPath, attachName(directoryName, dir->d_name));
		if (((strcmp(".", dir->d_name)) == 0) || ((strcmp("..", dir->d_name)) == 0)) {
			continue;
		}
		if (dir->d_type == 4) {
			threadSize = threadSize + 1;
			tids = (pthread_t*)realloc(tids, sizeof(pthread_t) * threadSize);
			pthread_create(&tids[threadCount], NULL, traverseDirectory, newPath);
			threadCount = threadCount + 1;
			fprintf(stderr, "A thread was created!\n");
		}
		else if (strstr(dir->d_name, ".csv") && dir->d_type == 8) {
			if (checkRepeat(dir->d_name) == 1) {
				if (checkCSV(newPath) == 1) {
					printf("CSV Found: [%s]\n", newPath);
					threadSize = threadSize + 1;
					tids = (pthread_t*)realloc(tids, sizeof(pthread_t) * threadSize);
					pthread_create(&tids[threadCount], NULL, sendRequest, newPath);
					threadCount = threadCount + 1;
					// fprintf(stderr, "A thread was created!\n");
				}
				else {
					outputErrorMessage("invalid csv file");
				}
			}
		}
	}

	closedir(dirStream);
	pthread_exit(0);
}

void outputErrorMessage(char *error) { // Standard output error function
	fprintf(stderr, "ERROR: %s\n", error);
	exit(0);
}

void* sendRequest(void *arg) {
	char* fileName = (char*)malloc(strlen((char*) arg) + 1);
	strcpy(fileName, (char*) arg);

	ssize_t len;
	int sent_bytes = 0;
	int offset;
	int remain_data;

	FILE *csv = fopen(fileName, "r");
	struct stat file_stat;
	fstat(fileno(csv), &file_stat);
	offset = 0;
    remain_data = file_stat.st_size;

    fseek(csv, 0, SEEK_END);
    char file_size[256];
    sprintf(file_size, "%d", file_stat.st_size);
    // Sending file size 
    len = send(sockfd, file_size, sizeof(file_size),0);

    while (((sent_bytes = sendfile(sockfd, fileno(csv), &offset, BUFSIZ)) > 0) && (remain_data > 0))
    {
        remain_data -= sent_bytes;
    }

	// if (fstat(fd, &file_stat) < 0)
	// {
	// 	outputErrorMessage("error fstat");
	// }

	// fprintf(stdout, "File Size: \n%lld bytes\n", file_stat.st_size);

	// sock_len = sizeof(struct sockaddr_in);
 //        /* Accepting incoming peers */
	// // peer_socket = accept(server_socket, (struct sockaddr *)&peer_addr, &sock_len);
	// // if (peer_socket == -1)
	// // {
	// // 	fprintf(stderr, "Error on accept --> %s", strerror(errno));

	// // 	exit(EXIT_FAILURE);
	// // }
	// // fprintf(stdout, "Accept peer --> %s\n", inet_ntoa(peer_addr.sin_addr));

	// sprintf(file_size, "%lld", file_stat.st_size);

 //        /* Sending file size */
	// len = send(sockfd, file_size, sizeof(file_size), 0);
	// if (len < 0)
	// {
	// 	outputErrorMessage("error on sending greetings");
	// }

	// fprintf(stdout, "Server sent %d bytes for the size\n", len);

	// offset = 0;
	// remain_data = file_stat.st_size;
 //        /* Sending file data */
	// while (((sent_bytes = sendfile(sockfd, fd, &offset, BUFSIZ)) > 0) && (remain_data > 0))
	// {
	// 	fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
	// 	remain_data -= sent_bytes;
	// 	fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
	// }

	pthread_exit(0);
}

// void* sendRequest(void* arg) {
// 	char* fileName = (char*)malloc(strlen((char*) arg) + 1);
// 	strcpy(fileName, (char*) arg);

// 	FILE * file_to_send;
// 	int ch;
// 	char toSEND[1];
// 	char remoteFILE[4096];
// 	int count1=1,count2=1, percent;

// 	file_to_send = fopen (fileName,"r");
// 	if(!file_to_send) {
// 		close(sockfd);
// 		outputErrorMessage("error opening file");
// 	} else {
// 		long fileSIZE;
// 		fseek (file_to_send, 0, SEEK_END); fileSIZE =ftell (file_to_send);
// 		rewind(file_to_send);

// 		sprintf(remoteFILE,"FBEGIN:%s:%ld\r\n", fileName, fileSIZE);
// 		send(sockfd, remoteFILE, sizeof(remoteFILE), 0);

// 		percent = fileSIZE / 100;
// 		while((ch=getc(file_to_send))!=EOF){
// 			toSEND[0] = ch;
// 			send(sockfd, toSEND, 1, 0);
// 			if( count1 == count2 ) {
// 				printf("33[0;0H");
// 				printf( "\33[2J");
// 				printf("Filename: %s\n", fileName);
// 				printf("Filesize: %ld Kb\n", fileSIZE / 1024);
// 				printf("Percent : %d%% ( %d Kb)\n",count1 / percent ,count1 / 1024);
// 				count1+=percent;
// 			}
// 			count2++;

// 		}
// 	}
// 	fclose(file_to_send);
// 	close(sockfd);
// 	pthread_exit(0);
// }

// void outputData() { // output data to output csv file.
// 	node *pData = pFirstNode;
// 	mkdir(outputDirectory, 0700);

// 	char* outputName = (char*)malloc(strlen(attachSorted()) + 1);
// 	strcpy(outputName, attachSorted());

// 	char* newPath = (char*)malloc(strlen(outputDirectory)+strlen(outputName)+ 3);
// 	strcpy(newPath, attachName(outputDirectory, outputName));

// 	FILE *fileO = fopen(newPath, "w");

// 	if (fileO != NULL) {
// 		fprintf(fileO, "%s", firstLine);

// 		while(pData != NULL) {
// 			fprintf(fileO, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n"
// 				, pData->color, pData->director_name, pData->num_critic_for_reviews, pData->duration
// 				, pData->director_facebook_likes, pData->actor_3_facebook_likes, pData->actor_2_name
// 				, pData->actor_1_facebook_likes, pData->gross, pData->genres, pData->actor_1_name
// 				, pData->movie_title, pData->num_voted_users, pData->cast_total_facebook_likes
// 				, pData->actor_3_name, pData->facenumber_in_poster, pData->plot_keywords, pData->movie_imdb_link
// 				, pData->num_user_for_reviews, pData->language, pData->country, pData->content_rating, pData->budget
// 				, pData->title_year, pData->actor_2_facebook_likes, pData->imdb_score, pData->aspect_ratio
// 				, pData->movie_facebook_likes);

// 			pData = pData->next;
// 		}
// 	}
// 	else {
// 		outputErrorMessage("file open error");
// 	}

// 	fclose(fileO);
// }

char* attachName(const char* directoryName, const char* name) { 
	char* result;
	result = (char*)malloc(strlen(directoryName)+strlen(name)+ 3);
	if(!result) {
		outputErrorMessage("space not allocated");
	}

	strcpy(result, directoryName);
	strcat(result,"/");
	strcat(result, name);
	return result;
}

char* attachSorted() {
	char* name = (char*)malloc(9);
	strcpy(name, "AllFiles");

	char* output_filename = (char*) malloc(strlen(name) + strlen(columnType) + 13);
	char* s1 = "-sorted-";
	char* s2 = ".csv";
	strcpy(output_filename, name);
	strcat(output_filename, s1);
	strcat(output_filename, columnType);
	strcat(output_filename, s2);

	return output_filename;
}

int switchVariable(const char* columnType) {
	if(strcmp(columnType, "color") == 0) {
		return 0;
	}
	else if(strcmp(columnType, "director_name") == 0) {
		return 1;
	}
	else if(strcmp(columnType, "num_critic_for_reviews") == 0) {
		return 2;
	}
	else if(strcmp(columnType, "duration") == 0) {
		return 3;
	}
	else if(strcmp(columnType, "director_facebook_likes") == 0) {
		return 4;
	}
	else if(strcmp(columnType, "actor_3_facebook_likes") == 0) {
		return 5;
	}
	else if(strcmp(columnType, "actor_2_name") == 0) {
		return 6;
	}
	else if(strcmp(columnType, "actor_1_facebook_likes") == 0) {
		return 7;
	}
	else if(strcmp(columnType, "gross") == 0) {
		return 8;
	}
	else if(strcmp(columnType, "genres") == 0) {
		return 9;
	}
	else if(strcmp(columnType, "actor_1_name") == 0) {
		return 10;
	}
	else if(strcmp(columnType, "movie_title") == 0) {
		return 11;
	}
	else if(strcmp(columnType, "num_voted_users") == 0) {
		return 12;
	}
	else if(strcmp(columnType, "cast_total_facebook_likes") == 0) {
		return 13;
	}
	else if(strcmp(columnType, "actor_3_name") == 0) {
		return 14;
	}
	else if(strcmp(columnType, "facenumber_in_poster") == 0) {
		return 15;
	}
	else if(strcmp(columnType, "plot_keywords") == 0) {
		return 16;
	}
	else if(strcmp(columnType, "movie_imdb_link") == 0) {
		return 17;
	}
	else if(strcmp(columnType, "num_user_for_reviews") == 0) {
		return 18;
	}
	else if(strcmp(columnType, "language") == 0) {
		return 19;
	}
	else if(strcmp(columnType, "country") == 0) {
		return 20;
	}
	else if(strcmp(columnType, "content_rating") == 0) {
		return 21;
	}
	else if(strcmp(columnType, "budget") == 0) {
		return 22;
	}
	else if(strcmp(columnType, "title_year") == 0) {
		return 23;
	}
	else if(strcmp(columnType, "actor_2_facebook_likes") == 0) {
		return 24;
	}
	else if(strcmp(columnType, "imdb_score") == 0) {
		return 25;
	}
	else if(strcmp(columnType, "aspect_ratio") == 0) {
		return 26;
	}
	else if(strcmp(columnType, "movie_facebook_likes") == 0) {
		return 27;
	}
	else {
		outputErrorMessage("invalid column name");
		return 0;
	}
}

int checkCSV(char *filename) {
	FILE *fileCheck = fopen(filename, "r");
	char *firstLineCheck = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	char *line = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	if (fileCheck != NULL) {
		fgets(line, BUFFER_SIZE, fileCheck);
		strcpy(firstLineCheck, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes");
		strcat(firstLineCheck, "\r\n");
		if (strcmp(line, firstLineCheck) == 0) {
			fclose(fileCheck);
			return 1;
		}
		else {
			fclose(fileCheck);
			return 0;
		}
	}
	else {
		outputErrorMessage("file open error");
	}

	return 0;
}

int checkRepeat(char *name) {
	if (strstr(name, "-sorted") || strstr(name, "file_buffer.csv")) {
		return 0;
	}
	else {
		return 1;
	}
}

void gotoxy(int x, int y)
{
	printf("%c[%d;%df",0x1B,y,x);
}
