/*
	Implemented by Akshay Mehta and Rohit Thakre
	Group: Tingerz
	November 29th, 2017
	Professor Francisco
*/
#ifndef MERGESORT_H_
#define MERGESORT_H_

#include <stdio.h>
#include <stdlib.h>
#include "sorter_thread.h"
 
/* function prototypes */
node * SortList(node *pList, int switchValue, int NUMLISTS);
node * MergeLists(node *pSrc1, node *pSrc2, int switchValue);

#endif // MERGESORT_H_