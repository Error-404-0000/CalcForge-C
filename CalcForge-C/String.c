#include "List.h"
#define BOOL char
#define TRUE 1
#define FALSE 0
int LengthOf(char* strings) {
	int count = 0;
	while (*(strings + count) != 0) {
		count++;
	}
	return count;
}
ArrayInfo* Join(char** strings, int count) {
	__new__array(joinedstrings, -1, 1);
	for (int i = 0; i < count; i++)
	{
		char* string = *((char**)strings+i);
		int _count = 0;
		while (*(string+ _count) != 0) {
			__add__array__copy(joinedstrings, (string + _count));
			_count++;
		}
	}
	char EndOfString = '\0';
	__add__array__copy(joinedstrings, &EndOfString);
	return __name__of__array(joinedstrings);
}
BOOL IsLetter(char  Char) {
	return (Char >= 'A' && Char <= 'Z') || (Char >= 'a' && Char <= 'z');
}
BOOL AreLetters(char*  Char,int leng) {
	for (int i = 0; i < leng; i++)
	{
		if (!(IsLetter(*(Char + i)))) {
			return FALSE;
		}
	}
	return TRUE;
}
BOOL IsNumber(char Char) {
	return (Char >= '0' && Char <= '9');
}
BOOL AreNumbers(char* Char,int Length) {
	for (int i = 0; i < Length; i++)
	{
		if((*(Char + i) == '.' && i + 1 < Length &&
			IsNumber(*(Char + i + 1))))
		{ }
		else if (!(IsNumber(*(Char + i))) ) {
			return FALSE;
		}
	}
	return TRUE;
}
