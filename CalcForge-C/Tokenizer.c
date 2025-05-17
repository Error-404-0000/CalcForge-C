#include "List.h"
#include "Strings.h"
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define ISHOLDINGFUNCTIONNAME(byte) (byte&1)
#define ISHOLDINGANUMBER(byte) (byte&0b10)>>1
//hold a value meaning if it is 1 that means we hit a "dot"
#define DOTHIT(byte) (byte&0b100)>>1
static union State {
	struct {
		unsigned char IsCurrentlyHoldingFunctionName : 1;
		unsigned char IsCurrentlyHoldingNumber : 1;
		unsigned char HitDot : 1;
	};
	unsigned char allFlags;

};

ArrayInfo* Tokenize(char* strings)
{
	int length = LengthOf(strings);
	union State mata = {0};
	//const char Ender = '\222';
	//const char Starter = '\221';
	__new__array(tokens, -1, sizeof(void*));
	__new__array(currentNumber, -1, 1);
	__new__array(currentFuncName, -1, 1);
	for (int i = 0; i < length; i++)
	{
		if (IsLetter(*(strings + i))) {
			if (mata.IsCurrentlyHoldingFunctionName==TRUE)
			{
				__add__array__copy(currentFuncName, (strings + i));
			}
			else {
				mata.IsCurrentlyHoldingFunctionName = TRUE;
				__add__array__copy(currentFuncName, strings + i);
			}			continue;

			
		}
		else if (mata.IsCurrentlyHoldingFunctionName == TRUE) {
			mata.IsCurrentlyHoldingFunctionName=FALSE;

			void* allocForToken = calloc(__name__of__array(currentFuncName)->CurrentIndex+1,1);
			for (int c = 0; c < __name__of__array(currentFuncName)->CurrentIndex; c++)
			{
				//writing to the new allo mem
				*((char*)allocForToken + c) = *(__name__of__array(currentFuncName)->Array + c);
			}				
			*((char*)allocForToken + __name__of__array(currentFuncName)->CurrentIndex) =
				'\0';

			__array__remove_range(currentFuncName, 0, __name__of__array(currentFuncName)->CurrentIndex);
			__add__array(tokens, allocForToken);

		}
		else if (IsNumber(*(strings + i)) == TRUE) {
			if (mata.IsCurrentlyHoldingNumber == TRUE) {
				__add__array__copy(currentNumber, (strings + i));
			}
			else {
				mata.IsCurrentlyHoldingNumber=TRUE;
				__add__array__copy(currentNumber, strings + i);
			}
			continue;
		}
		//checking this so you dont check something outside our mem reg
		else if (mata.IsCurrentlyHoldingNumber == TRUE && i+1<length &&*(strings + i)=='.'&&
			mata.HitDot==FALSE)
		{
			//if we get "." , we are checking if the next value after the dot is a valid number
			if (IsNumber(*(strings + i + 1))==TRUE) {
				__add__array__copy(currentNumber, (strings + i));
				mata.HitDot= TRUE;
				continue;
			}
		}
		else if (mata.IsCurrentlyHoldingNumber== TRUE) {
			mata.IsCurrentlyHoldingNumber=FALSE ;
			
			void* allocForToken = calloc(__name__of__array(currentNumber)->CurrentIndex+1,1);

			for (int c = 0; c < __name__of__array(currentNumber)->CurrentIndex; c++)
			{
				//writing to the new allo mem
				*((char*)allocForToken + c) = *(__name__of__array(currentNumber)->Array + c);
			}
			*((char*)allocForToken + __name__of__array(currentNumber)->CurrentIndex) = '\0';
			__array__remove_range(currentNumber, 0, __name__of__array(currentNumber)->CurrentIndex);
			__add__array(tokens, allocForToken);
			
		}
		if (*(strings + i) != ' ' ) {
			void* allocChar = malloc(2);//1 char
			if (allocChar == 0) {
				return 0;
			}
			*(char*)allocChar = *(strings + i);
			*((char*)allocChar + 1) = '\0';
			__add__array(tokens, allocChar);

		}

	}
	//for last token
	if (mata.IsCurrentlyHoldingFunctionName == TRUE) {
		mata.IsCurrentlyHoldingFunctionName = FALSE;
		void* allocForToken = calloc(__name__of__array(currentFuncName)->CurrentIndex+1,1);

		for (int i = 0; i < __name__of__array(currentFuncName)->CurrentIndex; i++)
		{
			*((char*)allocForToken + i) = *(__name__of__array(currentFuncName)->Array + i);
		}
		*((char*)allocForToken + __name__of__array(currentFuncName)->CurrentIndex) = '\0';

		__add__array(tokens, allocForToken);

	}
	if (mata.IsCurrentlyHoldingNumber == TRUE) {
		mata.IsCurrentlyHoldingNumber=FALSE;

		void* allocForToken = calloc(__array__currentNumber->CurrentIndex+1,1);
		for (int i = 0; i < __name__of__array(currentNumber)->CurrentIndex; i++)
		{
			*((char*)allocForToken + i) = *(__array__currentNumber->Array + i);
		}		
		*((char*)allocForToken + __name__of__array(currentNumber)->CurrentIndex) = '\0';

		__add__array(tokens, allocForToken);
	}
	FreeArray(__name__of__array(currentNumber));
	FreeArray(__name__of__array(currentFuncName));
	return __name__of__array(tokens);
}

