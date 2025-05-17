

#define TRUE 1
#define FALSE 0
#define BOOLEAN(NAME) char NAME;
#define BOOL char
#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#pragma once

static void WriteObject(void* where_to, unsigned int size, void* from);


/// <summary>
/// Creates and initializes a new ArrayInfo structure for managing dynamic arrays.
/// </summary>
struct ArrayInfo* __new_arrayinfo(unsigned int size, unsigned int _object_size)
{
	struct ArrayInfo* arrayinfo = malloc(sizeof(struct ArrayInfo));
	if (size == -1) {
	
		 arrayinfo->Array = calloc(20,_object_size);
		 arrayinfo->FIXEDSIZE = FALSE;
		 arrayinfo->object_size = _object_size;
		 arrayinfo->CurrentIndex = 0;
		 arrayinfo->Capacity = 20;
	  
	}
	else {
		size = size < 1 ? 1 : size;
		arrayinfo->Array = calloc(size , _object_size);
		arrayinfo->object_size = _object_size;
		arrayinfo->FIXEDSIZE = TRUE;

		arrayinfo->CurrentIndex = 0;
		arrayinfo->Capacity = size;

	}
	return arrayinfo;
}

/// <summary>
/// Adds a new object to an array by copying it, with support for dynamic resizing if the array is not fixed-size.
/// </summary>
/// <param name="arrayinfo">A pointer to an ArrayInfo structure that contains metadata about the array, such as its size, capacity, and whether it is fixed-size.</param>
/// <param name="newObject">A pointer to the object to be added to the array.</param>
/// <returns>A BOOL value indicating success (TRUE) or failure (FALSE). Returns FALSE if the array is fixed-size and full, or if memory allocation fails during resizing.</returns>
BOOL AddByCopy(struct ArrayInfo* arrayinfo, void* newObject) {
	if(arrayinfo->FIXEDSIZE ==TRUE && arrayinfo->CurrentIndex>=arrayinfo->Capacity)
	{
		return FALSE;
	}
	else if (arrayinfo->CurrentIndex < arrayinfo->Capacity) {
		WriteObject(GET_ELEMENT_PTR(arrayinfo->Array, arrayinfo->object_size, arrayinfo->CurrentIndex++),arrayinfo->object_size,newObject);
		return TRUE;
	}
	else if (arrayinfo->FIXEDSIZE == FALSE && arrayinfo->CurrentIndex >= arrayinfo->Capacity) {
		void* newRealloc = (char*)realloc(arrayinfo->Array, arrayinfo->Capacity * 2 * (arrayinfo->object_size));
		if (newRealloc != 0) 
		{
			arrayinfo->Array = newRealloc;
			arrayinfo->Capacity = arrayinfo->Capacity * 2;
			return AddByCopy(arrayinfo, newObject);
		}
		return FALSE;
	
	}
	return FALSE;
}
/// <summary>
/// Adds a new object to an array by reference, resizing the array if necessary and allowed.
/// </summary>
/// <param name="arrayinfo">A pointer to an ArrayInfo structure containing metadata about the array, such as its size, capacity, and whether it is fixed-size.</param>
/// <param name="newObject">A pointer to the object to be added to the array.</param>
/// <returns>TRUE if the object was successfully added to the array; FALSE otherwise.</returns>
BOOL AddByReference(struct ArrayInfo* arrayinfo, void* newObject) {
	if (arrayinfo->FIXEDSIZE == TRUE && arrayinfo->CurrentIndex >= arrayinfo->Capacity)
	{
		return FALSE;
	}
	else if (arrayinfo->CurrentIndex < arrayinfo->Capacity) {
		*(intptr_t*)GET_ELEMENT_PTR(arrayinfo->Array, arrayinfo->object_size, arrayinfo->CurrentIndex++) = newObject;

		return TRUE;
	}
	else if (arrayinfo->FIXEDSIZE == FALSE && arrayinfo->CurrentIndex >= arrayinfo->Capacity) {
		void* newRealloc = (char*)realloc(arrayinfo->Array, arrayinfo->Capacity * 2 * (arrayinfo->object_size));
	   if(newRealloc!=0){
		   arrayinfo->Array = newRealloc;
		   arrayinfo->Capacity = arrayinfo->Capacity * 2;
		   return AddByReference(arrayinfo, newObject);
	   }
	   return FALSE;
	}
	return FALSE;
}
static void MoveValue(void* address, void* value, int size) {
	for (size_t i = 0; i < size; i++)
	{
		*((char*)address + i) = (&value + i);
	}
}
/// <summary>
/// Finds the first element in an array that satisfies a given predicate.
/// </summary>
/// <param name="pred">A function pointer to a predicate that takes two arguments: an element of the array and additional arguments, and returns a char indicating whether the condition is met.</param>
/// <param name="otherargs">Additional arguments to be passed to the predicate function.</param>
/// <param name="arrayinfo">A pointer to an ArrayInfo structure containing metadata about the array, such as the array itself, its current size, and the size of each object.</param>
/// <returns>A pointer to the first element in the array that satisfies the predicate, or 0 if no such element is found.</returns>
void* First(char (pred)(void*,void*), void* otherargs, struct  ArrayInfo* arrayinfo) {
	for (int i = 0; i < arrayinfo->CurrentIndex; i++)
	{
		if (pred(((char*)arrayinfo->Array + i * arrayinfo->object_size), otherargs) == TRUE) {
			return arrayinfo->Array + i * arrayinfo->object_size;
		}
	}
	return 0;
}
signed  int IndexOf(char (pred)(void*, void*), void* otherargs, struct  ArrayInfo* arrayinfo) {
	for (int i = 0; i < arrayinfo->CurrentIndex; i++)
	{
		if (pred(((char*)arrayinfo->Array + i * arrayinfo->object_size), otherargs) == TRUE) {
			return i;
		}
	}
	return -1;
}
void replaceCopy(struct  ArrayInfo* arrayinfo,void* value,int index) {
	if (index > arrayinfo->CurrentIndex)
		return 0;
	WriteObject(GET_ELEMENT_PTR(arrayinfo->Array, arrayinfo->object_size, index), arrayinfo->object_size, value);
	return 1;
}
void replace(struct  ArrayInfo* arrayinfo, void* value, int index) {
	if (index > arrayinfo->CurrentIndex)
		return 0;
	*(intptr_t*)GET_ELEMENT_PTR(arrayinfo->Array, arrayinfo->object_size, index) = value;
	return 1;
}
void* GetValueByIndex(int index,ArrayInfo* array) {
	return ((char*)array->Array + (index * array->object_size));
}
/// <summary>
/// Filters elements of an array based on a predicate function.
/// </summary>
/// <param name="pred">A function pointer that takes two arguments: an element of the array and additional arguments, and returns a boolean-like value (TRUE or FALSE) indicating whether the element satisfies the condition.</param>
/// <param name="arrayinfo">A pointer to an ArrayInfo structure containing metadata about the array to be filtered.</param>
/// <param name="otherargs">A pointer to additional arguments that are passed to the predicate function.</param>
/// <param name="read">A pointer to an integer that will store the number of elements that satisfy the predicate.</param>
/// <returns>A pointer to a new ArrayInfo structure containing the filtered elements.</returns>
struct ArrayInfo* Where(char (pred)(void*, void*), struct ArrayInfo* arrayinfo, void* otherargs, int* read) {
	*read = 0;
	__new__array(tempArray, -1, arrayinfo->object_size);
	for (int i = 0; i < arrayinfo->CurrentIndex; i++)
	{
		if (pred(((char*)arrayinfo->Array + i * arrayinfo->object_size), otherargs) == TRUE) {
			__add__array__copy(tempArray, arrayinfo->Array + i * arrayinfo->object_size);
			*read += 1;
		}
	}
	Resize(__name__of__array(tempArray), *read);
	return __name__of__array(tempArray);
}
/// <summary>
/// Removes an element from an array at the specified index and returns the updated array.
/// </summary>
/// <param name="Array">A pointer to the ArrayInfo structure representing the array.</param>
/// <param name="index">The index of the element to remove.</param>
/// <returns>A pointer to the updated ArrayInfo structure after the element has been removed.</returns>
struct ArrayInfo* RemoveAt(struct ArrayInfo* Array, int index) {
	 
	if (Array->CurrentIndex <= index)
	{
		return Array;
	}
	__new__array(temparray, Array->FIXEDSIZE == TRUE? Array->Capacity:-1, Array->object_size);
	for (int i = 0; i < Array->CurrentIndex; i++)
	{
		if (i != index) {
			__add__array__copy(temparray, GET_ELEMENT_PTR(Array->Array, Array->object_size, i));
		}
		else {

		}
	}
	FreeArray(Array);
	return __name__of__array(temparray);

}
struct ArrayInfo* RemoveRange(struct ArrayInfo* Array, int start,int end) {

	if (Array->CurrentIndex < start || end > Array->CurrentIndex || start>end )
	{
		return Array;
	}
	__new__array(temparray, Array->FIXEDSIZE == TRUE ? Array->Capacity : -1, Array->object_size);
	for (int i = 0; i < Array->CurrentIndex; i++)
	{
		if (!(i >= start && i<=end)) {
			__add__array__copy(temparray, GET_ELEMENT_PTR(Array->Array, Array->object_size, i));
		}
	
	}
	FreeArray(Array);
	return __name__of__array(temparray);

}
void Foreach(struct ArrayInfo* Array, void* ptr(void*))
{
	for (size_t i = 0; i < Array->CurrentIndex; i++)
	{
		ptr(GET_ELEMENT_PTR(Array->Array, Array->object_size, i));
	}
}
static int Resize(struct ArrayInfo* arrayinfo,int newSize) {
	if (arrayinfo->CurrentIndex >= newSize) {
		return FALSE;
	}		
	void* newPtr = realloc(arrayinfo->Array, newSize * arrayinfo->object_size);
	if (newPtr != 0)
	{
		arrayinfo->Array = newPtr;
			arrayinfo->Capacity = newSize;
		return 1;
	}
	return FALSE;
}
































/// <summary>
/// Frees the memory allocated for an array and its associated metadata.
/// </summary>
/// <param name="arrayinfo">A pointer to a struct containing the array and its metadata. The struct and the array it points to will be freed.</param>
void FreeArray(struct ArrayInfo* arrayinfo) {
	
	if (arrayinfo->Array < 0 || arrayinfo->FIXEDSIZE<0)
		return;
	if(arrayinfo->Array!=0)
		free(arrayinfo->Array);
	free(arrayinfo);
}

static  void WriteObject(void* where_to, unsigned int size, void* from) {
	for (unsigned int  i = 0; i < size; i++)
	{
		*(char*)((char*)where_to + i) = *((char*)from + i);
	}
}