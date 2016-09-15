//a1-ptc
//Yuchen Lin 
 
 
#include<stdio.h>
#include <string.h>

typedef struct NODE{
	void *data;
	struct NODE *prev;
	struct NODE *next;
}NODE;


typedef struct LIST{
	NODE *head;
	NODE *tail;
	NODE *cur;
	int count;

	
}LIST;

LIST *ListCreate()
{
	//return new LIST(); //!!!don't know how to represent fail to create!!!
	return NULL;
}

/*
 * return the number of items in list
 */
int ListCount(LIST list)
{
	return 0;
}

/*
 * return a pointer to first item and make 1st cur
 */
 
 void *ListFirst(LIST list)
 {
	 return NULL;//return list.head.data;
 }
 
/*
 *	return a pointer to last and make last cur
 */
 
 void *ListLast(LIST list)
 {
	 return NULL; //return list.head.tail;
 }
 
/*
 * move cur to next,return null if at the end
 */ 
 void *ListNext(LIST list)
 {
	 return NULL;
 }
 
/*
 * move one forward, return null if at start
 */
 
 void *ListPrev(LIST list)
 {
	 return NULL;
 }
 
/*
 * return current item
 */
 void *ListCurr(LIST list) //!!!!!!!!!!!!!!!!!!!!!how to return a void pointer
 {
	 return NULL;
 }
 
/*
 * add new after current item, and make new current item;
 * 			return 0 on success; -1 on failure
 */ 
 
 int ListAdd(LIST list, char new_item)
 {
	 return 0;
 }
 
/*
 * add new before current, make new current;
 * 			return 0 if success; -1 if failure
 */ 
 
 int ListInsert(LIST list, char new_item)
 {
	 return 0;
 }
 
/*
 * add to end, make new current;
 * 			return 0 if success; -1 if failure
 */
 
 int ListAppend(LIST list, char new_item)
 {
	 return 0;
 }
 
/*
 * add to front, make new current;
 * 			return 0 if success; -1 if failure
 */
 
 int ListPrepend(LIST list, char new_item)
 {
	 return 0;
 }
 
/*
 * return cur and take out, make next current
 */ 
 
 void *ListRemove(LIST list)
 {
	 return NULL;
 }
 
/*
 * add list 2 to end of list 1
 */ 
 
 void ListConcat(LIST list1, LIST list2)
 {
	 
 }
 
/*
 * delete list
 */
 
 void ListFree(LIST list)//!!
 {
 }
 
 
/*
 * return the last and take out , make new last item current
 */ 
 
 void *ListTrim(LIST list)
 {
	 return NULL;
 }
 
/*
 * search through the list
 */
 
 void *ListSearch(LIST list, int comparator, char comparisonArg)
 {
	 return NULL;
 }
 
 int main()
 {
   return 0;
 }
 
 
