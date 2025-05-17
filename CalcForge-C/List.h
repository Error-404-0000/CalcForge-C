
#pragma once
typedef __declspec(align(64)) struct  ArrayInfo {
	unsigned int object_size;
	unsigned int Capacity;
	unsigned int CurrentIndex;
	char FIXEDSIZE;
	char* Array;
} ArrayInfo;

#pragma region macos

#define __new__array(name,fixedSize,objectsize) \
	     ArrayInfo* __array__##name = __new_arrayinfo(fixedSize,objectsize);
#define __add__array(name,newobject)\
		  AddByReference(__name__of__array(name),newobject)
#define __add__array__copy(name,newobject)\
		  AddByCopy(__name__of__array(name),newobject)
#define __name__of__array(name)__array__##name

#define __array__filter__first__(funcptr,otherargs,arrayName)\
		First(funcptr,otherargs,__name__of__array(arrayName))
#define __array_filter_where__(funcptr,arrayName,otherargs,read)\
		Where(funcptr,__name__of__array(arrayName),otherargs,read)
#define GET_ELEMENT_PTR(ptr,size, index) ((char*)ptr + (index)*(size))
#define __array__remove_at(name,index)\
		__name__of__array(name) = RemoveAt(__name__of__array(name),index)
#define __array__remove_range(name,start,end)\
		__name__of__array(name) = RemoveRange(__name__of__array(name),start,end)
void* GetValueByIndex(int index, ArrayInfo* array);
#pragma endregion
struct ArrayInfo* RemoveRange( ArrayInfo* Array, int start, int end);
struct ArrayInfo* RemoveAt( ArrayInfo* Array, int index);
struct ArrayInfo* __new_arrayinfo(unsigned int size, unsigned int _object_size);
char AddByReference( ArrayInfo* arrayinfo, void* newObject);
char AddByCopy( ArrayInfo* arrayinfo, void* newObject);
void Foreach( ArrayInfo* Array, void* ptr());
void FreeArray( ArrayInfo* arrayinfo);
void* First(char (pred)(void*, void*), void* otherargs,  ArrayInfo* arrayinfo);
struct ArrayInfo* Where(char (pred)(void*, void*),  ArrayInfo* arrayinfo, void* otherargs, int* read);
int IndexOf(char (pred)(void*, void*), void* otherargs, struct  ArrayInfo* arrayinfo);
void replace(struct  ArrayInfo* arrayinfo, void* value, int index);
void replaceCopy(struct  ArrayInfo* arrayinfo, void* value, int index);

