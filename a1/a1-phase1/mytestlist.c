//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672

#include "list.h"
#include <stdlib.h>
#include <stdio.h>

int main()
 {
	LIST* test = malloc(sizeof(test));
	LIST* test_2 = malloc(sizeof(test_2));
	void *return_test = NULL;
	int int_test = 0; 
	char data;  
   test = ListCreate();
   int_test = ListCount(test);
   return_test = ListFirst(test);
   return_test = ListLast(test);
   return_test = ListNext(test);
   return_test = ListPrev(test);
   return_test = ListCurr(test);
	int_test = ListAdd(test, data);
	int_test = ListInsert(test, data);
	int_test = ListAppend(test, data);
	int_test = ListPrepend(test, data);
	return_test = ListRemove(test);
	ListConcat(test,test_2);
	ListFree(test);
	return_test = ListTrim(test);
	return_test = ListSearch(test,int_test,data);   
   
   return 0;
 }