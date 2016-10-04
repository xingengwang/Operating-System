//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672


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
	char data_2 = 'b';	
	char data_3 = 'c';
	char data_4 = 'd';
	char data_5 = 'e';
	char data_6 = 'f';	
	char data_7 = 'g';
	char data_8 = 'h';
//*********Test list create*******************
printf("*****************Test ListCreate**************************************\n");
	test = ListCreate();
	test_2 = ListCreate();
	if(!test)
	{
		printf("test create failed. \n");
		return 0;
	}
	if(!test_2)
	{
		printf("test_2 create failed. \n");
		return 0;
	}
  //*********Test list count************************************************
  printf("*****************Test ListCount**************************************\n");
   int_test = ListCount(test);
   if(int_test != 0)
   {
		printf("test count should be 0. but it is %d \n", int_test);
		return 0;
   }
 //***************************Test List add*************************************************************
 printf("*****************Test ListAdd**************************************\n");
//***************************For list 'test'*************************************	
	int_test = ListAdd(test, &data_1);
				if(ListCount(test) != 1)
			   {
					printf("test count should be 1. but it is %d \n", ListCount(test));
					return 0;
			   }
			   if(int_test == -1)
			   {
				printf("List add %c failed \n", data_1);
				return 0;
			   }
	int_test = ListAdd(test, &data_2);
				if(ListCount(test) != 2)
			   {
					printf("test count should be 2. but it is %d \n", ListCount(test));
					return 0;
			   }
			   if(int_test == -1)
			   {
					printf("List add %c failed \n", data_2);
					return 0;
			   }
	int_test = ListAdd(test, &data_3);
				if(ListCount(test) != 3)
			   {
					printf("test count should be 3. but it is %d \n", ListCount(test));
					return 0;
			   }
			   if(int_test == -1)
			   {
					printf("List add %c failed \n", data_3);
					return 0;
			   }
	int_test = ListAdd(test, &data_4);
				if(ListCount(test) != 4)
			   {
					printf("test count should be 4. but it is %d \n", ListCount(test));
					return 0;
			   }
			   if(int_test == -1)
			   {
					printf("List add %c failed \n", data_4);
					return 0;
			   }
	
//****************************For list 'test_2'******************************	
	int_test = ListAdd(test_2, &data_5);
				if(ListCount(test_2) != 1)
			   {
					printf("test_2 count should be 4. but it is %d \n", ListCount(test_2));
					return 0;
			   }
			   if(int_test == -1)
			   {
					printf("List add %c failed \n", data_5);
					return 0;
			   }
	int_test = ListAdd(test_2, &data_6);
				if(ListCount(test_2) != 2)
			   {
					printf("test_2 count should be 3. but it is %d \n", ListCount(test_2));
					return 0;
			   }
			   if(int_test == -1)
			   {
					printf("List add %c failed \n", data_6);
					return 0;
			   }
	int_test = ListAdd(test_2, &data_7);
				if(ListCount(test_2) != 3)
			   {
					printf("test_2 count should be 2. but it is %d \n", ListCount(test_2));
					return 0;
			   }
			   if(int_test == -1)
			   {
					printf("List add %c failed \n", data_7);
					return 0;
			   }
	int_test = ListAdd(test_2, &data_8);
				if(ListCount(test_2) != 4)
			   {
					printf("test_2 count should be 1. but it is %d \n", ListCount(test_2));
					return 0;
			   }
			   if(int_test == -1)
			   {
					printf("List add %c failed \n", data_8);
					return 0;
			   }
			   
//***************************************test list concat****************************
printf("*****************Test ListConcat**************************************\n");

	ListConcat(test,test_2);
				if(ListCount(test) != 8)
				{
					printf("After concat test and test_2, ListCount(test) should be 8, but it is %d \n", ListCount(test));
					return 0;
				}
	ListFirst(test);
				if(test->cur != test->head)
				{
					printf("ListFirst fail to move to start. \n");
					return 0;
				}
	ListNext(test);
				if(*(char*)test->cur->data != data_2)
				{
					printf("ListNext failed. Current item should be %c, but it is %c \n", data_2, *(char*)test->cur->data);
					return 0;
				}	
	ListNext(test);
				if(*(char*)test->cur->data != data_3)
				{
					printf("ListNext failed. Current item should be %c, but it is %c \n", data_3, *(char*)test->cur->data);
					return 0;
				}	
	ListNext(test);
				if(*(char*)test->cur->data != data_4)
				{
					printf("ListNext failed. Current item should be %c, but it is %c \n", data_4, *(char*)test->cur->data);
					return 0;
				}	
	
	
	ListNext(test);
				if(*(char*)test->cur->data != data_5)
				{
					printf("ListNext failed. Current item should be %c, but it is %c \n", data_5, *(char*)test->cur->data);
					return 0;
				}	
	ListNext(test);
				if(*(char*)test->cur->data != data_6)
				{
					printf("ListNext failed. Current item should be %c, but it is %c \n", data_6, *(char*)test->cur->data);
					return 0;
				}
	ListNext(test);
				if(*(char*)test->cur->data != data_7)
				{
					printf("ListNext failed. Current item should be %c, but it is %c \n", data_7, *(char*)test->cur->data);
					return 0;
				}	
	ListNext(test);
				if(*(char*)test->cur->data != data_8)
				{
					printf("ListNext failed. Current item should be %c, but it is %c \n", data_8, *(char*)test->cur->data);
					return 0;
				}
	//After reach the end, ListNext should return NULL
				if(ListNext(test) != NULL)
				{
					printf("ListNext should return NULL, but it is %s \n", ListNext(test));
					return 0;
				}
	
	//check if test_2 is gone
				if(test_2->head != NULL)
				{
					printf("test_2 should be gone, but it is not \n");
					return 0;
				}
	
	//******************test ListFirst & ListNext & ListLast & ListPrev & ListCurr*****************************
	printf("************test ListFirst*****************************\n");
	printf("************test ListNext******************************\n");
	printf("************test ListLast******************************\n");
	printf("************test ListPrev *****************************\n");
	printf("************test ListCurr *****************************\n");
	printf("Correct list is (from head to tail) %c,%c,%c,%c,%c,%c,%c,%c \n", data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8);
	ListFirst(test);
	printf("1st item is %c \n", *(char*)ListCurr(test));
	ListNext(test);
	printf("2nd item is %c \n", *(char*)ListCurr(test));
	ListNext(test);
	printf("3rd item is %c \n", *(char*)ListCurr(test));
	ListNext(test);
	printf("4th item is %c \n", *(char*)ListCurr(test));
	ListNext(test);
	printf("5th item is %c \n", *(char*)ListCurr(test));
	ListNext(test);
	printf("6th item is %c \n", *(char*)ListCurr(test));
	ListNext(test);
	printf("7th item is %c \n", *(char*)ListCurr(test));
	ListNext(test);
	printf("8th item is %c \n", *(char*)ListCurr(test));	
	ListPrev(test);
	printf("7th item is %c \n", *(char*)ListCurr(test));
	ListPrev(test);
	printf("6th item is %c \n", *(char*)ListCurr(test));
	ListPrev(test);
	printf("5th item is %c \n", *(char*)ListCurr(test));
	ListPrev(test);
	printf("4th item is %c \n", *(char*)ListCurr(test));
	ListPrev(test);
	printf("3rd item is %c \n", *(char*)ListCurr(test));
	ListPrev(test);
	printf("2nd item is %c \n", *(char*)ListCurr(test));
	ListPrev(test);
	printf("1st item is %c \n", *(char*)ListCurr(test));
	
	ListFirst(test);
	if(*(char*)test->cur->data != data_1)
	{
		printf("ListFirst should move cur to the head, which is %c, but it is %c \n", data_1, *(char*)test->cur->data);
		return 0;
	}
	ListLast(test);
	if(*(char*)test->cur->data != data_8)
	{
		printf("ListLast should move cur to the head, which is %c, but it is %c \n", data_8, *(char*)test->cur->data);
		return 0;
	}
//*******************test list search***************************
printf("*****************Test ListSearch**************************************\n");
	char data = 'z';
	//search from start
	ListFirst(test);
	//search for data_3(c), which exist in the list
	return_test = ListSearch(test, test->comparator, &data_3);
				if(*(char*)return_test != data_3)
				{
					printf("ListSearch should get c but it return %s \n", return_test);
					return 0;
				}
				if(test->comparator != 1)
				{
					printf("test->comparator should be 1 but it is %d \n", test->comparator);
					return 0;
				}
	//search for data(z), which is not in the list
	return_test = ListSearch(test, test->comparator, &data);
				if(return_test != NULL)
				{
					printf("ListSearch should return NULL since z is not in the list, but it return %s \n", return_test);
					return 0;
				}
				if(test->comparator != 0)
				{
					printf("test->comparator should be 0 when search sth not in the list, but it is %d \n", test->comparator);
					return 0;
				}
	//search start from the tail. Then will only get 'match' result when search for the tail item
	//tail is h
	ListLast(test);
	return_test = ListSearch(test, test->comparator, &data_8);
				if(*(char*)return_test != data_8)
				{
					printf("ListSearch should get %c but it return %s \n", data_8, return_test);
					return 0;
				}
				if(test->comparator != 1)
				{
					printf("test->comparator should be 1 but it is %d \n", test->comparator);
					return 0;
				}
	return_test = ListSearch(test, test->comparator, &data_3);
				if(return_test != NULL)
				{
					printf("ListSearch should return NULL since %c is not in the list, but it return %s \n", data_3, return_test);
					return 0;
				}
				if(test->comparator != 0)
				{
					printf("test->comparator should be 0 when search sth not in the list, but it is %d \n", test->comparator);
					return 0;
				}
	
	
//**********************test list remove*************************
printf("*****************Test ListRemove**************************************\n");
	return_test = ListFirst(test);
	if(test->cur != test->head)
	{
		printf("ListFirst fail to move cur to start \n");
		return 0;
	}
	return_test = ListRemove(test);
	if(*(char*)return_test != 'a')
	{
		printf("ListRemove should remove a, but it removes %c \n", *(char*)return_test);
		return 0;
	}
	if((*(char*)test->head->data != 'b') && (*(char*)test->head->data != 'b'))
	{
		printf("After remove, should make next item cur, and head should be b, but it is %c \n", *(char*)test->head->data);
		return 0;
	}
	return_test = ListRemove(test);
	if(*(char*)return_test != 'b')
	{
		printf("ListRemove should remove b, but it removes %c \n", *(char*)return_test);
		return 0;
	}
	if((*(char*)test->head->data != 'c') && (*(char*)test->head->data != 'c'))
	{
		printf("After remove, should make next item cur, and head should be c, but it is %c \n", *(char*)test->head->data);
		return 0;
	}
	return_test = ListRemove(test);
	if(*(char*)return_test != 'c')
	{
		printf("ListRemove should remove c, but it removes %c \n", *(char*)return_test);
		return 0;
	}
	if((*(char*)test->head->data != 'd') && (*(char*)test->head->data != 'd'))
	{
		printf("After remove, should make next item cur, and head should be d, but it is %c \n", *(char*)test->head->data);
		return 0;
	}

  
   //********************************************************************
   
   //**********************************************************
   //****************Test list insert*********************************
   printf("****************Test ListInsert***********************\n");
   test_2 = ListCreate();
   int_test = ListInsert(test_2, &data_1);
   int_test = ListInsert(test_2, &data_2);
   int_test = ListInsert(test_2, &data_3);
   int_test = ListInsert(test_2, &data_4);
   
	printf("Correct order (from head to tail) should be %c,%c,%c,%c \n", data_4, data_3, data_2, data_1);
	ListFirst(test_2);
	printf("1st item is %c \n", *(char*)ListCurr(test_2));
	ListNext(test_2);
	printf("2nd item is %c \n", *(char*)ListCurr(test_2));
	ListNext(test_2);
	printf("3rd item is %c \n", *(char*)ListCurr(test_2));
	ListNext(test_2);
	printf("4th item is %c \n", *(char*)ListCurr(test_2));
	
	
	
//**********************************************************************

	printf("********************Test Listappend*****************\n");
	int_test = ListAppend(test_2, &data_5);
	if(*(char*)ListCurr(test_2) != data_5)
	{
		printf("ListAppend should add %c to the end, but the tail is actually %c \n", data_5, *(char*)test_2->cur->data);
		return 0;
	}
	
	printf("********************Test ListPrepend*****************\n");
	int_test = ListPrepend(test_2, &data_6);
	if(*(char*)ListCurr(test_2) != data_6)
	{
		printf("ListAppend should add %c to the start, but the head is actually %c \n", data_6, *(char*)test_2->cur->data);
		return 0;
	}

	//******************************************************************
	
	printf("*******************test list trim********************\n");
	//test_2 now should be data_6, data_4, data_3, data_2, data_1,data_5 
	return_test = ListTrim(test_2);
	if( *(char*) return_test != data_5)
	{
		printf("ListTrim should return the tail which is %c, but it return %c \n", data_5, *(char*)test_2->tail->data);
		return 0;
	}
	if(*(char*) test_2->tail->data != data_1)
	{
		printf("ListTrim fail to manage the pointer after called \n");
		printf("tail should be %c, but it is %c \n",data_1, *(char*)test_2->tail->data);
		return 0;
	}
	
	//test_2 now should be data_6, data_4, data_3, data_2, data_1 
	return_test = ListTrim(test_2);
	if( *(char*) return_test != data_1)
	{
		printf("ListTrim should return the tail which is %c, but it return %c \n", data_1, *(char*)test_2->tail->data);
		return 0;
	}
	if(*(char*) test_2->tail->data != data_2)
	{
		printf("ListTrim fail to manage the pointer after called \n");
		printf("tail should be %c, but it is %c \n",data_2, *(char*)test_2->tail->data);
		return 0;
	}

	printf("Pass The Test!!! \n");
   return 0;
 }
