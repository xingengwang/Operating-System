//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672


#include "list.h"
#include <stdlib.h>
/*
 * return cur and take out, make next current
 */ 
 
 void *ListRemove(LIST* list)
 {
	NODE *takeout_item = list->cur;
	NODE *temp = list->cur->next;		
	list->cur->prev = takeout_item->prev;
	
	list->cur = temp;
	 
	 if(takeout_item == list->head)
	{
		list->head = temp;
	}
	list->count--;
	 return takeout_item->data;
 }
 
 /*
 * add list 2 to end of list 1
 */ 
 
 void ListConcat(LIST* list1, LIST* list2)
 {
	LIST *temp = list2;
	list2 = NULL;
	void *buffer;
	ListLast(list1);
	ListFirst(temp);
	while(!(ListCount(temp)==0))
	{
		list1->tail->next = temp->head;
		temp->head->prev = list1->tail;
		list1->cur = list1->tail->next;
		list1->tail = list1->cur;
		list1->count++;
		ListRemove(temp);
	}
 }
 
 
 