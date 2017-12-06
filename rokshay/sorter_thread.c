/*
	Implemented by Akshay Mehta and Rohit Thakre
	Group: Tingerz
	November 29th, 2017
	Professor Francisco
*/
#include "sorter_thread.h"
#include "mergesort.h"

#define BUFFER_SIZE 2048 // large enough buffer size

node *pFirstNode = NULL; // Pointer to first struct pointer
node *pLastNode = NULL; // Pointer to last struct pointer
char *firstLine; // First line read from File (read from one method, used in another)
int switchValue;
char *columnType;
char *outputDirectory;
pthread_t *tids;
int threadCount = 0;
int threadSize = 0;
int total = 0;
pthread_mutex_t listMutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char** argv) {
    if(argc != 3 && argc != 5 && argc != 7) { //Make sure input is valid.
    	outputErrorMessage("must enter 3, 5 or 7 arguments");
    }

    char* directoryName = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    columnType = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    outputDirectory = (char*)malloc(sizeof(char)*BUFFER_SIZE);

    if (argc == 3) {
    	if (strcmp(argv[1], "-c") == 0) {
    		strcpy(columnType, argv[2]);
    		strcpy(directoryName, ".");
    		strcpy(outputDirectory, ".");
    	}
    	else {
    		outputErrorMessage("invalid argument input - no \"-c\" found");
    	}
    }
    else if (argc == 5) {
    	if (strcmp(argv[1], "-c") == 0) {
    		if (strcmp(argv[3], "-d") == 0) {
    			strcpy(columnType, argv[2]);
    			strcpy(directoryName, argv[4]);
    			strcpy(outputDirectory, ".");
    		}
    		else {
    			outputErrorMessage("invalid argument input - no \"-d\" found");
    		}
    	}
    	else if (strcmp(argv[1], "-d") == 0) {
    		if (strcmp(argv[3], "-c") == 0) {
    			strcpy(columnType, argv[4]);
    			strcpy(directoryName, argv[2]);
    			strcpy(outputDirectory, ".");
    		}
    		else {
    			outputErrorMessage("invalid argument input - no \"-c\" found");
    		}
    	}
    	else {
    		outputErrorMessage("invalid argument input - no \"-c\" or \"-d\" found");
    	}
    }
    else if (argc == 7) {
    	if (strcmp(argv[1], "-c") == 0) {
    		if (strcmp(argv[3], "-d") == 0) {
    			if (strcmp(argv[5], "-o") == 0) {
    				strcpy(columnType, argv[2]);
    				strcpy(directoryName, argv[4]);
    				strcpy(outputDirectory, argv[6]);
    			}
    			else {
    				outputErrorMessage("invalid argument input - no \"-o\" found");
    			}
    		}
    		else if (strcmp(argv[3], "-o") == 0) {
    			if (strcmp(argv[5], "-d") == 0) {
    				strcpy(columnType, argv[2]);
    				strcpy(directoryName, argv[6]);
    				strcpy(outputDirectory, argv[4]);
    			}
    			else {
    				outputErrorMessage("invalid argument input - no \"-d\" found");
    			}
    		}
    		else {
    			outputErrorMessage("invalid argument input - no \"-d\" or \"-o\" found");
    		}
    	}
    	else if (strcmp(argv[1], "-d") == 0) {
    		if (strcmp(argv[3], "-c") == 0) {
    			if (strcmp(argv[5], "-o") == 0) {
    				strcpy(columnType, argv[4]);
    				strcpy(directoryName, argv[2]);
    				strcpy(outputDirectory, argv[6]);
    			}
    			else {
    				outputErrorMessage("invalid argument input - no \"-o\" found");
    			}
    		}
    		else if (strcmp(argv[3], "-o") == 0) {
    			if (strcmp(argv[5], "-c") == 0) {
    				strcpy(columnType, argv[6]);
    				strcpy(directoryName, argv[2]);
    				strcpy(outputDirectory, argv[4]);
    			}
    			else {
    				outputErrorMessage("invalid argument input - no \"-c\" found");
    			}
    		}
    		else {
    			outputErrorMessage("invalid argument input - no \"-c\" or \"-o\" found");
    		}
    	}
    	else if (strcmp(argv[1], "-o") == 0) {
    		if (strcmp(argv[3], "-d") == 0) {
    			if (strcmp(argv[5], "-c") == 0) {
    				strcpy(columnType, argv[6]);
    				strcpy(directoryName, argv[4]);
    				strcpy(outputDirectory, argv[2]);
    			}
    			else {
    				outputErrorMessage("invalid argument input - no \"-c\" found");
    			}
    		}
    		else if (strcmp(argv[3], "-c") == 0) {
    			if (strcmp(argv[5], "-d") == 0) {
    				strcpy(columnType, argv[4]);
    				strcpy(directoryName, argv[6]);
    				strcpy(outputDirectory, argv[2]);
    			}
    			else {
    				outputErrorMessage("invalid argument input - no \"-d\" found");
    			}
    		}
    		else {
    			outputErrorMessage("invalid argument input - no \"-c\" or \"-d\" found");
    		}
    	}
    	else {
    		outputErrorMessage("invalid argument input - no \"-c\" or \"-d\" or \"-o\" found");
    	}
    }

    switchValue = switchVariable(columnType);

    fprintf(stdout, "Initial PID: %d\n", getpid());
    fprintf(stdout, "TIDS of all child threads: ");
    fflush(stdout);

    threadSize = threadSize + 1;
    tids = (pthread_t*)malloc(sizeof(pthread_t) * threadSize);
    pthread_create(&tids[threadCount], NULL, traverseDirectory, directoryName);
    threadCount = threadCount + 1;

    int x;
    for (x = 0; x < threadSize; x++) {
    	pthread_join(tids[x], NULL);
    }

    pFirstNode = SortList(pFirstNode, switchValue, threadSize);
    outputData();
    fprintf(stdout, "\nTotal number of threads: %d\n", threadSize);

    return 0;
}

void* traverseDirectory(void* arg) {
	fprintf(stderr, "%lu, ", (unsigned long)pthread_self());
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
			// fprintf(stderr, "A thread was created!\n");
		}
		else if (strstr(dir->d_name, ".csv") && dir->d_type == 8) {
			if (checkRepeat(dir->d_name) == 1) {
				if (checkCSV(newPath) == 1) {
					threadSize = threadSize + 1;
					tids = (pthread_t*)realloc(tids, sizeof(pthread_t) * threadSize);
					pthread_create(&tids[threadCount], NULL, inputData, newPath);
					threadCount = threadCount + 1;
					// fprintf(stderr, "A thread was created!\n");
						// inputData(newPath);
						// MergeSort(&pFirstNode, switchVariable(columnType));
						// char *newName = (char*) malloc(strlen(dir->d_name) + strlen(columnType) + 13);
						// strcpy(newName, attachSorted(dir->d_name, columnType));
						// outputData(outputDirectory, newName);
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

void trim(char *token) { // Gets rid of white spaces before or after token.
	int index, i;

	index = 0;
	while(token[index] == ' ' || token[index] == '\t' || token[index] == '\n') {
		index++;
	}

	i = 0;
	while(token[i + index] != '\0') {
		token[i] = token[i + index];
		i++;
	}
	token[i] = '\0';

	i = 0;
	index = -1;
	while(token[i] != '\0') {
		if(token[i] != ' ' && token[i] != '\t' && token[i] != '\n') {
			index = i;
		}

		i++;
	}

	token[index + 1] = '\0';
}

char *tokenizer(char **input) { // Custom tokenizer that tokenizes by commas and quotes.
	char *token = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	char *pointer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	strcpy(pointer, *input);
	int length = 0;

	if(**input == '\0') {
		token[0] = '\0';
		*input = *input + 1;
		*input = NULL;
		return token;
	}
	if(**input == ' ') {
		while(**input == ' ') {
			*input = *input + 1;
			length = length + 1;
		}
	}
	if(**input == '\"') {
		*input = *input + 1;
		length = length + 1;
		while(**input != '\"') {
			*input = *input + 1;
			length = length + 1;
		}
		*input = *input + 1;
		length = length + 1;
		strncpy(token, pointer, length);
		token[length] = '\0';
		if(**input == '\0') {
			*input = NULL;
		}
		else if(**input == ',') {
			*input = *input + 1;
		}
		else if(**input == ' ') {
			while(**input == ' ') {
				*input = *input + 1;
			}

			if(**input == '\0') {
				*input = NULL;
			}
			else if(**input == ',') {
				*input = *input + 1;
			}
		}
		return token;
	}
	else if(**input == ',') {
		token[0] = '\0';
		*input = *input + 1;
		return token;
	}
	else if(**input != '\0') {
		while(**input != ',' && **input != '\0') {
			*input = *input + 1;
			length = length + 1;
		}
		if(**input == ',') {
			strncpy(token, pointer, length);
			token[length] = '\0';
			*input = *input + 1;
			return token;
		}
		else if(**input == '\0') {
			strncpy(token, pointer, length);
			token[length] = '\0';
			*input = *input + 1;
			*input = NULL;
			return token;
		}
	}

	return NULL;
}

void* inputData(void* arg) {
	fprintf(stderr, "%lu, ", (unsigned long)pthread_self());
	char* fileName = (char*)malloc(strlen((char*) arg) + 1);
	strcpy(fileName, (char*) arg);
	// printf("Path Found: [%s]\n", fileName);
	FILE *fileI = fopen(fileName, "r");

	char *line = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	fgets(line, BUFFER_SIZE, fileI);

	if (firstLine == NULL) {
		firstLine = (char*)malloc(sizeof(char)*BUFFER_SIZE);
		strcpy(firstLine, line);
	}

	char *token = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	
	while(fgets(line, BUFFER_SIZE, fileI) != NULL) {
		pthread_mutex_lock(&listMutex);
		total = total + 1;
		pthread_mutex_unlock(&listMutex);
		char *tempLine = (char*)malloc(sizeof(char)*BUFFER_SIZE);
		strcpy(tempLine, line);

		pthread_mutex_lock(&listMutex);
		if(pFirstNode == NULL) {
			node *pNewStruct = (node *)malloc(sizeof(node));
			pNewStruct->next = NULL;

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->color = malloc(strlen(token) + 1);
			strcpy(pNewStruct->color, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->director_name = malloc(strlen(token) + 1);
			strcpy(pNewStruct->director_name, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->num_critic_for_reviews = malloc(strlen(token) + 1);
			strcpy(pNewStruct->num_critic_for_reviews, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->duration = malloc(strlen(token) + 1);
			strcpy(pNewStruct->duration, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->director_facebook_likes = malloc(strlen(token) + 1);
			strcpy(pNewStruct->director_facebook_likes, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->actor_3_facebook_likes = malloc(strlen(token) + 1);
			strcpy(pNewStruct->actor_3_facebook_likes, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->actor_2_name = malloc(strlen(token) + 1);
			strcpy(pNewStruct->actor_2_name, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->actor_1_facebook_likes = malloc(strlen(token) + 1);
			strcpy(pNewStruct->actor_1_facebook_likes, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->gross = malloc(strlen(token) + 1);
			strcpy(pNewStruct->gross, token);
			
			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->genres = malloc(strlen(token) + 1);
			strcpy(pNewStruct->genres, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->actor_1_name = malloc(strlen(token) + 1);
			strcpy(pNewStruct->actor_1_name, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->movie_title = malloc(strlen(token) + 1);
			strcpy(pNewStruct->movie_title, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->num_voted_users = malloc(strlen(token) + 1);
			strcpy(pNewStruct->num_voted_users, token);
			
			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->cast_total_facebook_likes = malloc(strlen(token) + 1);
			strcpy(pNewStruct->cast_total_facebook_likes, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->actor_3_name = malloc(strlen(token) + 1);
			strcpy(pNewStruct->actor_3_name, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->facenumber_in_poster = malloc(strlen(token) + 1);
			strcpy(pNewStruct->facenumber_in_poster, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->plot_keywords = malloc(strlen(token) + 1);
			strcpy(pNewStruct->plot_keywords, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->movie_imdb_link = malloc(strlen(token) + 1);
			strcpy(pNewStruct->movie_imdb_link, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->num_user_for_reviews = malloc(strlen(token) + 1);
			strcpy(pNewStruct->num_user_for_reviews, token);
			
			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->language = malloc(strlen(token) + 1);
			strcpy(pNewStruct->language, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->country = malloc(strlen(token) + 1);
			strcpy(pNewStruct->country, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->content_rating = malloc(strlen(token) + 1);
			strcpy(pNewStruct->content_rating, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->budget = malloc(strlen(token) + 1);
			strcpy(pNewStruct->budget, token);
			
			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->title_year = malloc(strlen(token) + 1);
			strcpy(pNewStruct->title_year, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->actor_2_facebook_likes = malloc(strlen(token) + 1);
			strcpy(pNewStruct->actor_2_facebook_likes, token);
			
			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->imdb_score = malloc(strlen(token) + 1);
			strcpy(pNewStruct->imdb_score, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->aspect_ratio = malloc(strlen(token) + 1);
			strcpy(pNewStruct->aspect_ratio, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->movie_facebook_likes = malloc(strlen(token) + 1);
			strcpy(pNewStruct->movie_facebook_likes, token);

			// pthread_mutex_lock(&listMutex);
			pFirstNode = pLastNode = pNewStruct;
			// pthread_mutex_unlock(&listMutex);
		}
		else {
			node *pNewStruct = (node *)malloc(sizeof(node));

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->color = malloc(strlen(token) + 1);
			strcpy(pNewStruct->color, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->director_name = malloc(strlen(token) + 1);
			strcpy(pNewStruct->director_name, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->num_critic_for_reviews = malloc(strlen(token) + 1);
			strcpy(pNewStruct->num_critic_for_reviews, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->duration = malloc(strlen(token) + 1);
			strcpy(pNewStruct->duration, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->director_facebook_likes = malloc(strlen(token) + 1);
			strcpy(pNewStruct->director_facebook_likes, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->actor_3_facebook_likes = malloc(strlen(token) + 1);
			strcpy(pNewStruct->actor_3_facebook_likes, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->actor_2_name = malloc(strlen(token) + 1);
			strcpy(pNewStruct->actor_2_name, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->actor_1_facebook_likes = malloc(strlen(token) + 1);
			strcpy(pNewStruct->actor_1_facebook_likes, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->gross = malloc(strlen(token) + 1);
			strcpy(pNewStruct->gross, token);
			
			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->genres = malloc(strlen(token) + 1);
			strcpy(pNewStruct->genres, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->actor_1_name = malloc(strlen(token) + 1);
			strcpy(pNewStruct->actor_1_name, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->movie_title = malloc(strlen(token) + 1);
			strcpy(pNewStruct->movie_title, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->num_voted_users = malloc(strlen(token) + 1);
			strcpy(pNewStruct->num_voted_users, token);
			
			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->cast_total_facebook_likes = malloc(strlen(token) + 1);
			strcpy(pNewStruct->cast_total_facebook_likes, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->actor_3_name = malloc(strlen(token) + 1);
			strcpy(pNewStruct->actor_3_name, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->facenumber_in_poster = malloc(strlen(token) + 1);
			strcpy(pNewStruct->facenumber_in_poster, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->plot_keywords = malloc(strlen(token) + 1);
			strcpy(pNewStruct->plot_keywords, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->movie_imdb_link = malloc(strlen(token) + 1);
			strcpy(pNewStruct->movie_imdb_link, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->num_user_for_reviews = malloc(strlen(token) + 1);
			strcpy(pNewStruct->num_user_for_reviews, token);
			
			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->language = malloc(strlen(token) + 1);
			strcpy(pNewStruct->language, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->country = malloc(strlen(token) + 1);
			strcpy(pNewStruct->country, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->content_rating = malloc(strlen(token) + 1);
			strcpy(pNewStruct->content_rating, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->budget = malloc(strlen(token) + 1);
			strcpy(pNewStruct->budget, token);
			
			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->title_year = malloc(strlen(token) + 1);
			strcpy(pNewStruct->title_year, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->actor_2_facebook_likes = malloc(strlen(token) + 1);
			strcpy(pNewStruct->actor_2_facebook_likes, token);
			
			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->imdb_score = malloc(strlen(token) + 1);
			strcpy(pNewStruct->imdb_score, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->aspect_ratio = malloc(strlen(token) + 1);
			strcpy(pNewStruct->aspect_ratio, token);

			token = tokenizer(&tempLine);
			trim(token);
			pNewStruct->movie_facebook_likes = malloc(strlen(token) + 1);
			strcpy(pNewStruct->movie_facebook_likes, token);

			if(pFirstNode == pLastNode) {
				pFirstNode->next = pNewStruct;
				pLastNode = pNewStruct;
				pNewStruct->next = NULL;
			}
			else{
				pLastNode->next = pNewStruct;
				pNewStruct->next = NULL;
				pLastNode = pNewStruct;
			}
		}
		pthread_mutex_unlock(&listMutex);
	}

	fclose(fileI);
	pthread_exit(0);
}

void outputData() { // output data to output csv file.
	node *pData = pFirstNode;
	mkdir(outputDirectory, 0700);

	char* outputName = (char*)malloc(strlen(attachSorted()) + 1);
	strcpy(outputName, attachSorted());

	char* newPath = (char*)malloc(strlen(outputDirectory)+strlen(outputName)+ 3);
	strcpy(newPath, attachName(outputDirectory, outputName));

	FILE *fileO = fopen(newPath, "w");

	if (fileO != NULL) {
		fprintf(fileO, "%s", firstLine);

		while(pData != NULL) {
			fprintf(fileO, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n"
				, pData->color, pData->director_name, pData->num_critic_for_reviews, pData->duration
				, pData->director_facebook_likes, pData->actor_3_facebook_likes, pData->actor_2_name
				, pData->actor_1_facebook_likes, pData->gross, pData->genres, pData->actor_1_name
				, pData->movie_title, pData->num_voted_users, pData->cast_total_facebook_likes
				, pData->actor_3_name, pData->facenumber_in_poster, pData->plot_keywords, pData->movie_imdb_link
				, pData->num_user_for_reviews, pData->language, pData->country, pData->content_rating, pData->budget
				, pData->title_year, pData->actor_2_facebook_likes, pData->imdb_score, pData->aspect_ratio
				, pData->movie_facebook_likes);

			pData = pData->next;
		}
	}
	else {
		outputErrorMessage("file open error");
	}

	fclose(fileO);
}

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
	if (strstr(name, "-sorted")) {
		return 0;
	}
	else {
		return 1;
	}
}