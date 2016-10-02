//Yuchen Lin; yul761; 11138672
//Xingeng Wang; xiw031

#include "list.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
 {
	LIST* test = malloc(sizeof(test));
	LIST* test_2 = malloc(sizeof(test_2));
	void *return_test = NULL;
	int int_test = 0; 
	char data_1 = 'a'; 
	//*********Test list create*******************
   test = ListCreate();   
   printf("****************************************************************************\n");
   //*********Test list count*******************
   int_test = ListCount(test);
   printf("number of element in list now is %d\n",ListCount(test));
   /*
   return_test = ListFirst(test);
   return_test = ListLast(test);
   return_test = ListNext(test);
   return_test = ListPrev(test);
   return_test = ListCurr(test);
*/
   //********************************************************************
   //*************Test list add********************
	printf("************************ListAdd*******************\n");		
	printf("Add %c \n", data_1);	
	int_test = ListAdd(test, data_1);

	printf("number of element in list now is %d\n",ListCount(test));


	char data_2 = 'b';
	int_test = ListAdd(test,data_2);
	printf("Add %c \n", data_2);	
	printf("number of element in list now is %d\n",ListCount(test));

	char data_3 = 'c';
	int_test = ListAdd(test,data_3);
	printf("Add %c \n", data_3);	
	printf("number of element in list now is %d\n",ListCount(test));

	char data_4 = 'd';
	int_test = ListAdd(test,data_4);
	printf("Add %c \n", data_4);	
	printf("number of element in list now is %d\n",ListCount(test));


   //**********************************************************
   //****************Test list insert*********************************
	printf("************************ListInsert*******************\n");	
	int_test = ListInsert(test, data_1);
	printf("Insert %c \n", data_1);	
	printf("number of element in list now is %d\n",ListCount(test));
	int_test = ListInsert(test, data_2);
	printf("Insert %c \n", data_2);	
	printf("number of element in list now is %d\n",ListCount(test));
	int_test = ListInsert(test, data_3);
	printf("Insert %c \n", data_3);	
	printf("number of element in list now is %d\n",ListCount(test));
	int_test = ListInsert(test, data_4);
	printf("Insert %c \n", data_4);	
	printf("number of element in list now is %d\n",ListCount(test));

//**********************************************************************
//***************test list append**********************************	
printf("************************ListAppend*******************\n");	
	int_test = ListAppend(test, data_1);
	printf("Append %c \n", data_1);	
	printf("number of element in list now is %d\n",ListCount(test));
	int_test = ListAppend(test, data_2);
	printf("Append %c \n", data_2);	
	printf("number of element in list now is %d\n",ListCount(test));
	int_test = ListAppend(test, data_3);
	printf("Append %c \n", data_3);	
	printf("number of element in list now is %d\n",ListCount(test));
	int_test = ListAppend(test, data_4);
	printf("Append %c \n", data_4);	
	printf("number of element in list now is %d\n",ListCount(test));
	
	
//******************test list prepend***************************
printf("************************ListPrepend*******************\n");	
	int_test = ListPrepend(test, data_1);
	printf("Prepend %c \n", data_1);	
	printf("number of element in list now is %d\n",ListCount(test));
	int_test = ListPrepend(test, data_2);
	printf("Prepend %c \n", data_1);	
	printf("number of element in list now is %d\n",ListCount(test));
	int_test = ListPrepend(test, data_3);
	printf("Prepend %c \n", data_1);	
	printf("number of element in list now is %d\n",ListCount(test));
	int_test = ListPrepend(test, data_4);
	printf("Prepend %c \n", data_1);	
	printf("number of element in list now is %d\n",ListCount(test));
	
/*	
	return_test = ListRemove(test);
	ListConcat(test,test_2);
	ListFree(test);
	return_test = ListTrim(test);
	return_test = ListSearch(test,int_test,data);   
*/

   return 0;
 }
