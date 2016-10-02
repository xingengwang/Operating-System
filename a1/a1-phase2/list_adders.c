//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672

#include "list.h"
#include <stdlib.h>
#include <stdio.h>

NODE *NodeCreate(void *data)
{
	if(count_NODE == 0) //first node
	{
		node_element= (NODE *)malloc(sizeof(NODE)*1024);
	}
	
	NODE *node = &node_element[count_NODE];
	node->data = data;
	node->next = malloc(sizeof(NODE));
	node->prev = malloc(sizeof(NODE));
	
	count_NODE ++;
	return node;
}




LIST *ListCreate()
{
	if(count_list == 0)
	{
		list_element = (LIST *)malloc(sizeof(LIST)*1024);
	}
	LIST *list = &list_element[count_list];
	list->count = 0;
	list->cur = malloc(sizeof(NODE));
	list->head = malloc(sizeof(NODE));
	list->tail = malloc(sizeof(NODE));
	
	count_list ++;
	return list;
}

/*
 * return the number of items in list
 */
int ListCount(LIST* list)
{
	return list->count;
}

/*
 * add new after current item, and make new current item;
 * 			return 0 on success; -1 on failure
 */ 
 
 int ListAdd(LIST* list, char new_item)
 {
	NODE *newNode = NodeCreate(&new_item);
	
	if((&new_item == NULL) || (list == NULL))
	{
		return -1;
	}
	
	if(list->count == 0)
	{
		list->cur = newNode;
		list->head = newNode;
		list->tail = newNode;
		printf("current element: %c \n", *(char*)list->cur->data );
		list->count ++;
		return 0;
	}
	
	if(list->tail == list->cur)
	{
		list->tail = newNode;
		
	
	}

	if(list->cur->next == NULL)
	{
		list->tail = newNode;
	}
	else
	{
		list->cur->next->prev = newNode;	
	}
		
	printf("current element: %c \n", *(char*)list->cur->data );
	newNode->next = list->cur->next;
	newNode->prev = list->cur;
	list->cur->next = newNode;
	list->cur = newNode;
	list->count++;
	return 0;
	
	
 }
 
 /*
 * add new before current, make new current;
 * 			return 0 if success; -1 if failure
 */ 
 
 int ListInsert(LIST* list, char new_item)
 {
	NODE *newNode = NodeCreate(&new_item);
	
	if((&new_item == NULL) || (list == NULL))
	{
		return -1;
	}
	
	if(list->count == 0)
	{
		list->cur = newNode;
		list->head = newNode;
		list->tail = newNode;
		list->count ++;
		printf("current element: %c \n", *(char*)list->cur->data );
		return 0;
	}
	
	if(list->head == list->cur)
	{
		list->head = newNode;
		newNode=list->cur;
		list->count++;
	
	}
		
	printf("current element: %c \n", *(char*)list->cur->data );
	newNode->prev = list->cur->prev;
	newNode->next = list->cur;
	list->cur->prev = newNode;
	list->cur = newNode;
	list->count++;
	return 0; 
	
 }
 
/*
 * add to end, make new current;
 * 			return 0 if success; -1 if failure
 */
 
 int ListAppend(LIST* list, char new_item)
 {
	NODE *newNode = NodeCreate(&new_item);
	
	if((&new_item == NULL) || (list == NULL))
	{
		return -1;
	}
	
	if(list->count == 0)
	{
		list->cur = newNode;
		list->head = newNode;
		list->tail = newNode;
		printf("current element: %c \n", *(char*)list->cur->data );
		list->count ++;
		return 0;
	}
	
	printf("current element: %c \n", *(char*)list->cur->data );
	list->tail = newNode;
	list->cur = newNode;
	list->count++;

	
	 return 0;
 }
 
 
 /*
 * add to front, make new current;
 * 			return 0 if success; -1 if failure
 */
 
 int ListPrepend(LIST* list, char new_item)
 {
	 NODE *newNode = NodeCreate(&new_item);
	
	if((&new_item == NULL) || (list == NULL))
	{
		return -1;
	}
	
	if(list->count == 0)
	{
		list->cur = newNode;
		list->head = newNode;
		list->tail = newNode;
		printf("current element: %c \n", *(char*)list->cur->data );
		list->count ++;
		return 0;
	}
	printf("current element: %c \n", *(char*)list->cur->data );
	list->head = newNode;
	newNode = list->cur;
	list->count++;
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

 
