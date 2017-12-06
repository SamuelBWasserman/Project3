/*
	Implemented by Akshay Mehta and Rohit Thakre
	Group: Tingerz
	November 29th, 2017
	Professor Francisco
*/
#ifndef SORTER_THREAD_H_
#define SORTER_THREAD_H_

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

/* function prototypes */
void outputErrorMessage(char *error);
void trim(char *token);
char *tokenizer(char **input);
char* attachName(const char* directoryName, const char* name);
char* attachSorted();
void outputData();
int switchVariable(const char* columnType);
int checkCSV(char *filename);
int checkRepeat(char *name);
void* traverseDirectory(void* arg);
void* inputData(void* arg);

typedef struct node {
	char *color;
	char *director_name;
	char *num_critic_for_reviews;
	char *duration;
	char *director_facebook_likes;
	char *actor_3_facebook_likes;
	char *actor_2_name;
	char *actor_1_facebook_likes;
	char *gross;
	char *genres;
	char *actor_1_name;
	char *movie_title;
	char *num_voted_users;
	char *cast_total_facebook_likes;
	char *actor_3_name;
	char *facenumber_in_poster;
	char *plot_keywords;
	char *movie_imdb_link;
	char *num_user_for_reviews;
	char *language;
	char *country;
	char *content_rating;
	char *budget;
	char *title_year;
	char *actor_2_facebook_likes;
	char *imdb_score;
	char *aspect_ratio;
	char *movie_facebook_likes;

	struct node *next;
} node;

#endif