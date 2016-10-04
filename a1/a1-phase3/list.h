//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672



#include <stdio.h>
#include <string.h>

typedef struct NODE{
	void *data;
	struct NODE *prev;
	struct NODE *next;
}NODE;

typedef struct NODE NODE;

typedef struct LIST{
	NODE *head;
	NODE *tail;
	NODE *cur;
	int count;
	int comparator;
}LIST;

LIST *list_element;
int count_list;
NODE *node_element;
int count_NODE;



LIST *ListCreate();
int ListCount(LIST* list);
void *ListFirst(LIST* list);
void *ListLast(LIST* list);
void *ListNext(LIST* list);
void *ListPrev(LIST* list);
void *ListCurr(LIST* list);
int ListAdd(LIST* list, void *new_item);
int ListInsert(LIST* list, void *new_item);
int ListAppend(LIST* list, void *new_item);
int ListPrepend(LIST* list, void *new_item);
void *ListRemove(LIST* list);
void ListConcat(LIST* list1,LIST* list2);
void ListFree(LIST* list);
void *ListTrim(LIST* list);
 void *ListSearch(LIST* list, int comparator, void *comparisonArg);
