/*
	Implemented by Akshay Mehta and Rohit Thakre
	Group: Tingerz
	November 29th, 2017
	Professor Francisco
*/
#include "mergesort.h"

/* sort a list using array of pointers to list       */
/* aList[i] == NULL or ptr to list with 2^i nodes    */

node * SortList(node *pList, int switchValue, int NUMLISTS) {
node * aList[NUMLISTS];         /* array of lists */
	node * pNode;
	node * pNext;
	int i;
if(pList == NULL)           /* check for empty list */
	return NULL;
for(i = 0; i < NUMLISTS; i++)   /* init array */
	aList[i] = NULL;
pNode = pList;              /* merge nodes into array */
	while(pNode != NULL){
		pNext = pNode->next;
		pNode->next = NULL;
		for(i = 0; (i < NUMLISTS) && (aList[i] != NULL); i++){
			pNode = MergeLists(aList[i], pNode, switchValue);
			aList[i] = NULL;
		}
if(i == NUMLISTS)   /* don't go beyond end of array */
		i--;
		aList[i] = pNode;
		pNode = pNext;
	}
pNode = NULL;           /* merge array into one list */
	for(i = 0; i < NUMLISTS; i++)
		pNode = MergeLists(aList[i], pNode, switchValue);
	return pNode;
}

/* merge two already sorted lists                    */
/* compare uses pSrc2 < pSrc1 to follow the STL rule */
/*   of only using < and not <=                      */
node * MergeLists(node *pSrc1, node *pSrc2, int switchValue)
{
node *pDst = NULL;          /* destination head ptr */
node **ppDst = &pDst;       /* ptr to head or prev->next */
	if(pSrc1 == NULL)
		return pSrc2;
	if(pSrc2 == NULL)
		return pSrc1;
	while(1) {
		if (switchValue == 0) {
			if(strcmp(pSrc2->color, pSrc1->color) < 0) {  /* if src2 < src1 */
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 1) {
			if(strcmp(pSrc2->director_name, pSrc1->director_name) < 0) {  /* if src2 < src1 */
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 2) {
			int aInt = atoi(pSrc2->num_critic_for_reviews);
			int bInt = atoi(pSrc1->num_critic_for_reviews);
			if(aInt < bInt) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 3) {
			int aInt = atoi(pSrc2->duration);
			int bInt = atoi(pSrc1->duration);
			if(aInt < bInt) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 4) {
			int aInt = atoi(pSrc2->director_facebook_likes);
			int bInt = atoi(pSrc1->director_facebook_likes);
			if(aInt < bInt) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 5) {
			int aInt = atoi(pSrc2->actor_3_facebook_likes);
			int bInt = atoi(pSrc1->actor_3_facebook_likes);
			if(aInt < bInt) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 6) {
			if(strcmp(pSrc2->actor_2_name, pSrc1->actor_2_name) < 0) {  /* if src2 < src1 */
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 7) {
			int aInt = atoi(pSrc2->actor_1_facebook_likes);
			int bInt = atoi(pSrc1->actor_1_facebook_likes);
			if(aInt < bInt) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 8) {
			long aLong = atol(pSrc2->gross);
			long bLong = atol(pSrc1->gross);
			if(aLong < bLong) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 9) {
			if(strcmp(pSrc2->genres, pSrc1->genres) < 0) {  /* if src2 < src1 */
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 10) {
			if(strcmp(pSrc2->actor_1_name, pSrc1->actor_1_name) < 0) {  /* if src2 < src1 */
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 11) {
			if(strcmp(pSrc2->movie_title, pSrc1->movie_title) < 0) {  /* if src2 < src1 */
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 12) {
			int aInt = atoi(pSrc2->num_voted_users);
			int bInt = atoi(pSrc1->num_voted_users);
			if(aInt < bInt) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 13) {
			int aInt = atoi(pSrc2->cast_total_facebook_likes);
			int bInt = atoi(pSrc1->cast_total_facebook_likes);
			if(aInt < bInt) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 14) {
			if(strcmp(pSrc2->actor_3_name, pSrc1->actor_3_name) < 0) {  /* if src2 < src1 */
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 15) {
			int aInt = atoi(pSrc2->facenumber_in_poster);
			int bInt = atoi(pSrc1->facenumber_in_poster);
			if(aInt < bInt) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 16) {
			if(strcmp(pSrc2->plot_keywords, pSrc1->plot_keywords) < 0) {  /* if src2 < src1 */
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 17) {
			if(strcmp(pSrc2->movie_imdb_link, pSrc1->movie_imdb_link) < 0) {  /* if src2 < src1 */
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 18) {
			int aInt = atoi(pSrc2->num_user_for_reviews);
			int bInt = atoi(pSrc1->num_user_for_reviews);
			if(aInt < bInt) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 19) {
			if(strcmp(pSrc2->language, pSrc1->language) < 0) {  /* if src2 < src1 */
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 20) {
			if(strcmp(pSrc2->country, pSrc1->country) < 0) {  /* if src2 < src1 */
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 21) {
			if(strcmp(pSrc2->content_rating, pSrc1->content_rating) < 0) {  /* if src2 < src1 */
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 22) {
			long aLong = atol(pSrc2->budget);
			long bLong = atol(pSrc1->budget);
			if(aLong < bLong) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 23) {
			int aInt = atoi(pSrc2->title_year);
			int bInt = atoi(pSrc1->title_year);
			if(aInt < bInt) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 24) {
			int aInt = atoi(pSrc2->actor_2_facebook_likes);
			int bInt = atoi(pSrc1->actor_2_facebook_likes);
			if(aInt < bInt) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 25) {
			double aDouble = atof(pSrc2->imdb_score);
			double bDouble = atof(pSrc1->imdb_score);
			if(aDouble < bDouble) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 26) {
			double aDouble = atof(pSrc2->aspect_ratio);
			double bDouble = atof(pSrc1->aspect_ratio);
			if(aDouble < bDouble) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
		if (switchValue == 27) {
			int aInt = atoi(pSrc2->movie_facebook_likes);
			int bInt = atoi(pSrc1->movie_facebook_likes);
			if(aInt < bInt) {
				*ppDst = pSrc2;
				pSrc2 = *(ppDst = &(pSrc2->next));
				if(pSrc2 == NULL) {
					*ppDst = pSrc1;
					break;
				}
			} 
			else {                        /* src1 <= src2 */
				*ppDst = pSrc1;
				pSrc1 = *(ppDst = &(pSrc1->next));
				if(pSrc1 == NULL) {
					*ppDst = pSrc2;
					break;
				}
			}
		}
	}

	return pDst;
}