//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672

#include "list.h"
#include <stdlib.h>

LIST *ListCreate()
{
	printf("Go to procedure ListCreate!\n");	
	return NULL;
}

/*
 * return the number of items in list
 */
int ListCount(LIST* list)
{
	printf("Go to procedure ListCount!\n");	
	return 0;
}

/*
 * add new after current item, and make new current item;
 * 			return 0 on success; -1 on failure
 */ 
 
 int ListAdd(LIST* list, char new_item)
 {
	 printf("Go to procedure ListAdd!\n");	 
	 return 0;
 }
 
 /*
 * add new before current, make new current;
 * 			return 0 if success; -1 if failure
 */ 
 
 int ListInsert(LIST* list, char new_item)
 {
	 printf("Go to procedure ListInsert!\n");	 
	 return 0;
 }
 
/*
 * add to end, make new current;
 * 			return 0 if success; -1 if failure
 */
 
 int ListAppend(LIST* list, char new_item)
 {
	 printf("Go to procedure ListAppend! \n");	 
	 return 0;
 }
 
 
 /*
 * add to front, make new current;
 * 			return 0 if success; -1 if failure
 */
 
 int ListPrepend(LIST* list, char new_item)
 {
	 printf("Go to procedure ListPrepend!\n");	 
	 return 0;
 }
 
 

/*
 * delete list
 */
 
 void ListFree(LIST* list)//!!
 {
 	printf("Go to procedure ListFree!\n");
 }
 
 /*
 * return the last and take out , make new last item current
 */ 
 
 void *ListTrim(LIST* list)
 {
	 printf("Go to procedure ListTrim!\n");	 
	 return NULL;
 }
 
 
 /*
 * search through the list
 */
 
 void *ListSearch(LIST* list, int comparator, char comparisonArg)
 {
 	 printf("Go to procedure ListSearch!\n");	 
	 return NULL;
 }



/*
 * return current item
 */
 void *ListCurr(LIST* list) 
 {
	 printf("Go to procedure ListCurr!\n");		 
	 return NULL;
 }

 