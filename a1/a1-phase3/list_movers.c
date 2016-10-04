//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672


#include "list.h"

#include <stdlib.h>
/*
 * return a pointer to first item and make 1st cur
 */
 
 void *ListFirst(LIST* list)
 {
	 list->cur = list->head; 
	 return list->head->data;
 }
 
 /*
 *	return a pointer to last and make last cur
 */
 
 void *ListLast(LIST* list)
 {
	  list->cur = list->tail;
	  return list->tail->data; 
 }
 
 /*
 * move cur to next,return null if at the end
 */ 
 void *ListNext(LIST* list)
 {
	 if(list->cur == list->tail)
	{
		return NULL;
	}	
	
	if(list->cur->next == list->tail)
	{
		list->cur = list->tail;
		return list->cur->data;
	}
	
		NODE *temp = list->cur->next;
		list->cur = temp;
		list->cur->next = temp->next;
		
		return list->cur->data;
	
 }
 
 /*
 * move one forward, return null if at start
 */
 
 void *ListPrev(LIST* list)
 {
	if(list->cur == list->head)
	{
		return NULL;
	}
	
	if(list->cur->prev == list->head)
	{
		list->cur = list->head;
		return list->cur->data;
	}
	
	NODE *temp = list->cur->prev;
	list->cur = temp;
	list->cur->prev = temp->prev;
	 return list->cur->data;
 }
 
 
 