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
	node->next = NULL;
	node->prev = NULL;
	
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
	list->cur = NULL;
	list->head = NULL;
	list->tail = NULL;
	list->comparator = 2;
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
 
 int ListAdd(LIST* list, void *new_item)
 {
	NODE *newNode = NodeCreate(new_item);
	
	if((new_item == NULL) || (list == NULL))
	{
		return -1;
	}
	
	if(list->count == 0)
	{
		list->cur = newNode;
		list->head = newNode;
		list->tail = newNode;
		list->count ++;
		return 0;
	}
	
	if(list->cur == list->tail)
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
		
	newNode->next = list->cur->next;
	newNode->prev = list->cur;
	list->cur->next = newNode;
	list->cur = newNode;
	list->count++;
	/*
		printf("cur element: %c \n", *(char*)list->cur->data);
		printf("head element: %c \n", *(char*)list->head->data);
		printf("tail element: %c \n", *(char*)list->tail->data);
		*/
	return 0;
	
	
 }
 
 /*
 * add new before current, make new current;
 * 			return 0 if success; -1 if failure
 */ 
 
 int ListInsert(LIST* list, void *new_item)
 {
	NODE *newNode = NodeCreate(new_item);
	
	if((new_item == NULL) || (list == NULL))
	{
		return -1;
	}
	
	if(list->count == 0)
	{
		list->cur = newNode;
		list->head = newNode;
		list->tail = newNode;
		list->count ++;
		return 0;
	}
	
	if(list->head == list->cur)
	{
		list->head = newNode;
	}
		
	if(list->cur->prev == NULL)
	{
		list->head = newNode;
	}
	else
	{
		list->cur->prev->next = newNode;
	}

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
 
 int ListAppend(LIST* list, void *new_item)
 {
	NODE *newNode = NodeCreate(new_item);
	
	if((new_item == NULL) || (list == NULL))
	{
		return -1;
	}
	
	list->cur = list->tail;
	
	 return ListAdd(list, new_item);
 }
 
 
 /*
 * add to front, make new current;
 * 			return 0 if success; -1 if failure
 */
 
 int ListPrepend(LIST* list, void *new_item)
 {
	 NODE *newNode = NodeCreate(new_item);
	
	if((new_item == NULL) || (list == NULL))
	{
		return -1;
	}
	list->cur = list->head;
	
	return ListInsert(list, new_item);
 }
 
 

/*
 * delete list
 */
 
 void ListFree(LIST* list)
 {
 	printf("Go to procedure ListFree!\n");
 }
 
 /*
 * return the last and take out , make new last item current
 */ 
 
 void *ListTrim(LIST* list)
 {
	void *takeout_item;
	 if(list->count == 0)
	{
		printf("No item in the list. \n");
	}
	
	if(list->count == 1)
	{
		takeout_item = list->tail->data;
		list->tail= NULL;
		return takeout_item;
	}
	 takeout_item = list->tail->data;
	 list->cur = list->tail->prev;
	 list->tail = list->cur;  
	 
	 list->tail->prev = list->cur->prev;
	 
	 list->count--;
	 return takeout_item;
 }
 
 
 /*
 * search through the list
 */
 
 void *ListSearch(LIST* list, int comparator, void *comparisonArg)
 {
 	 if(list->cur == list->tail)
	{
		if(list->cur->data == comparisonArg)
		{
			list->comparator = 1;
			return list->cur->data;
		}
		else
		{
			list->comparator=0;
			return NULL;
		}
	}
	else if(list->cur->data == comparisonArg)
	{
		list->comparator = 1;
		return list->cur->data;
	}
	
	ListNext(list);
	
	 return ListSearch(list,list->comparator,comparisonArg);
 }



/*
 * return current item
 */
 void *ListCurr(LIST* list) 
 {
	 	 
	 if(list->cur == NULL)
	 {
		return NULL;
	 }
	 return list->cur->data;
 }

 
