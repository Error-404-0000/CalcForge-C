#include "List.h"
#include "Parser.h"
#include "Tokenizer.h"
#include "Strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
static Token* EvaluateFunction(Token* token);
char* doubleToString(double* valuePtr);
static char IsFunction(Token** token);
static Token* EvaluateGroup(Token* token);
static double HandleOperations(int IndexOfNextOperation, ArrayInfo* token, Token* operation);
static char isGroup(Token* token);
static double Eval(double left, TokenOperation operation, double right);
static char isOperator(Token** token);
static char isMul_Dev_Power(Token** token);
static ArrayInfo* GetParams(char* strings);
int stringToDouble(const char* str, double* outValue) {
	if (str == NULL || outValue == NULL) return 0;

	char* endPtr;
	errno = 0;  // Reset errno
	double value = strtod(str, &endPtr);

	if (errno != 0 || *endPtr != '\0') {
		return 0;  // Error: invalid number or leftover characters
	}

	*outValue = value;
	return 1;
}
double Evaluate(ArrayInfo* Tokens) {
	int FuncTokens = IndexOf(&IsFunction, 0, Tokens);
	double result;

	while (FuncTokens != -1) {
		Token* tokenResult = EvaluateFunction(*(Token**)GetValueByIndex(FuncTokens, Tokens));
		if (tokenResult == -1) {
			printf("invalid function call.\n");
			return 0;
		}
		replace(Tokens, tokenResult, FuncTokens);
		FuncTokens = IndexOf(&IsFunction, 0, Tokens);
	}
	int indexofGroup = IndexOf(&isGroup, 0, Tokens);
	while (indexofGroup !=-1) {
		Token* tokenResult = EvaluateGroup(*(Token**)GetValueByIndex(indexofGroup, Tokens));
		replace(Tokens,  tokenResult, indexofGroup);
		indexofGroup=IndexOf(&isGroup, 0, Tokens);
	}
	int IndexOfNextOperation = IndexOf(&isMul_Dev_Power, 0, Tokens);
    while (IndexOfNextOperation != -1 && Tokens->CurrentIndex > 2)
	{
		Token* operation = *(Token**)GetValueByIndex(IndexOfNextOperation, Tokens);

		if (IndexOfNextOperation - 1 >= 0 && IndexOfNextOperation + 1 < Tokens->CurrentIndex) {
			result = HandleOperations(IndexOfNextOperation, Tokens, operation);
			ArrayInfo* newArray = RemoveRange(Tokens, IndexOfNextOperation - 1, IndexOfNextOperation);//removing only the 2 values for example 2*2 ;only the 2* will be removed
			Tokens = newArray;
			Token* newSovToken = calloc(1, sizeof(Token));
			
			newSovToken->Value = doubleToString(&result);
			newSovToken->TokenType = TokenType_Number;
			newSovToken->TokenTree = TokenTree_Single;
			replace(Tokens, newSovToken, IndexOfNextOperation - 1);
		}
		else {
			printf("invalid token : operation missing left or right side value\n");
			return 0;
		}
		IndexOfNextOperation = IndexOf(&isMul_Dev_Power, 0, Tokens);
	}
	IndexOfNextOperation = IndexOf(&isOperator, 0, Tokens);
	while (IndexOfNextOperation != -1 && Tokens->CurrentIndex > 2)
	{
		Token* operation = *(Token**)GetValueByIndex(IndexOfNextOperation, Tokens);

		if (IndexOfNextOperation - 1 >= 0 && IndexOfNextOperation + 1 < Tokens->CurrentIndex) {
			result = HandleOperations(IndexOfNextOperation, Tokens, operation);
			ArrayInfo* newArray = RemoveRange(Tokens, IndexOfNextOperation - 1, IndexOfNextOperation);//removing only the 2 values for example 2*2 ;only the 2* will be removed
			
			Tokens = newArray;
			Token* newSovToken = calloc(1, sizeof(Token));
			
			newSovToken->Value = doubleToString(&result);
			newSovToken->TokenType = TokenType_Number;
			newSovToken->TokenTree = TokenTree_Single;
			replace(Tokens, newSovToken, IndexOfNextOperation - 1);
		}
		else {
			printf("invalid token : operation missing left or right side value\n");
			return 0;
		}
		 IndexOfNextOperation = IndexOf(&isOperator, 0, Tokens);

	}
	if (Tokens->CurrentIndex >= 2) {
		printf("invalid tokens / wrong token index ");
		return 0;
	}
	Token* tok = *(Token**)GetValueByIndex(0, Tokens);
	double finalresult;
	 stringToDouble((char*)tok->Value,&finalresult);
	 return finalresult;

}
static char IsFunction(Token** token) {
	if (token == NULL || *token == NULL)
		return 0;
	return (*token)->TokenType == TokenType_Function;
}
static Token* EvaluateFunction(Token* token) {
	if (token == NULL || token->TokenType != TokenType_Function)
		return -1;

	FuncationToken* functoken = (FuncationToken*)token->Value;
	ArrayInfo* functionsParms = GetParams(functoken->raw_token);
	if (!functionsParms || functionsParms->CurrentIndex == 0) {
		printf("Invalid parameters for function.\n");
		return -1;
	}

	__new__array(resultArray, -1, sizeof(double));
	for (size_t i = 0; i < functionsParms->CurrentIndex; i++) {
		char* expr = *((char**)functionsParms->Array + i);
		ArrayInfo* groupTokens = Parse(Tokenize(expr));
		double result = Evaluate(groupTokens);
		__add__array__copy(resultArray, &result);
		FreeArray(groupTokens);
	}

	double finalResult = 0.0;
	TokenOperation op = token->TokenOperation;
	double* args = (double*)__name__of__array(resultArray)->Array;
	int argc = __name__of__array(resultArray)->CurrentIndex;

	switch (op) {
	case TokenOperation_Pow:
		if (argc >= 2) finalResult = pow(args[0], args[1]);
		break;
	case TokenOperation_Abs:
		if (argc >= 1) finalResult = fabs(args[0]);
		break;
	case TokenOperation_Sin:
		if (argc >= 1) finalResult = sin(args[0]);
		break;
	case TokenOperation_Cos:
		if (argc >= 1) finalResult = cos(args[0]);
		break;
	case TokenOperation_Tan:
		if (argc >= 1) finalResult = tan(args[0]);
		break;
	case TokenOperation_Sqrt:
		if (argc >= 1) finalResult = sqrt(args[0]);
		break;
	case TokenOperation_Log:
		if (argc >= 1) finalResult = log(args[0]);
		break;
	case TokenOperation_Log10:
		if (argc >= 1) finalResult = log10(args[0]);
		break;
	case TokenOperation_Exp:
		if (argc >= 1) finalResult = exp(args[0]);
		break;
	case TokenOperation_Floor:
		if (argc >= 1) finalResult = floor(args[0]);
		break;
	case TokenOperation_Ceiling:
		if (argc >= 1) finalResult = ceil(args[0]);
		break;
	case TokenOperation_Round:
		if (argc >= 1) finalResult = round(args[0]);
		break;
	case TokenOperation_Truncate:
		if (argc >= 1) finalResult = (int)args[0]; // cast to int truncates
		break;
	case TokenOperation_Max:
		if (argc >= 2) finalResult = args[0] > args[1] ? args[0] : args[1];
		break;
	case TokenOperation_Min:
		if (argc >= 2) finalResult = args[0] < args[1] ? args[0] : args[1];
		break;
	case TokenOperation_Add:
		for (int i = 0; i < argc; i++) finalResult += args[i];
		break;
	case TokenOperation_Subtract:
		if (argc >= 1) {
			finalResult = args[0];
			for (int i = 1; i < argc; i++) finalResult -= args[i];
		}
		break;
	case TokenOperation_Multiply:
		finalResult = 1;
		for (int i = 0; i < argc; i++) finalResult *= args[i];
		break;
	case TokenOperation_Divide:
		if (argc >= 1) {
			finalResult = args[0];
			for (int i = 1; i < argc; i++) {
				if (args[i] == 0) {
					printf("Divide by zero in function.\n");
					finalResult = 0;
					break;
				}
				finalResult /= args[i];
			}
		}
		break;
	case TokenOperation_Print:
		for (int i = 0; i < argc; i++) {
			printf("Print arg[%d] = %f\n", i, args[i]);
		}
		finalResult = args[argc - 1]; 
		return -1;
		break;
	default:
		printf("unknown function: %s\n", functoken->FuncName);
		FreeArray(__name__of__array(resultArray));
		return -1;
	}

	Token* resultToken = calloc(1, sizeof(Token));
	resultToken->TokenType = TokenType_Number;
	resultToken->TokenTree = TokenTree_Single;
	resultToken->TokenOperation = TokenOperation_None;
	resultToken->Value = doubleToString(&finalResult);

	FreeArray(__name__of__array(resultArray));
	return resultToken;
}

static Token* EvaluateGroup(Token* token) {
	if (token->TokenTree != TokenTree_Group)
	{
		return token;
	}
	ArrayInfo* GroupTokens = Parse(Tokenize((char*)token->Value));
	double result;
	int IndexOfFunc = IndexOf(&IsFunction, 0, GroupTokens);
	while (IndexOfFunc != -1) {
		Token* tokenResult = EvaluateFunction(*(Token**)GetValueByIndex(IndexOfFunc, GroupTokens));
		replace(GroupTokens, tokenResult, IndexOfFunc);
		IndexOfFunc = IndexOf(&IsFunction, 0, GroupTokens);
	}
	 int indexofGroup = IndexOf(&isGroup, 0, GroupTokens);
	while (indexofGroup != -1) {
		Token* tokenResult = EvaluateGroup(*(Token**)GetValueByIndex(indexofGroup, GroupTokens));
		replace(GroupTokens, tokenResult, indexofGroup);
		  indexofGroup = IndexOf(&isGroup, 0, GroupTokens);
	}
	int IndexOfNextOperation = IndexOf(&isMul_Dev_Power, 0, GroupTokens);
	while (IndexOfNextOperation !=-1 && GroupTokens->CurrentIndex>2)
	{
		Token* operation = *(Token**)GetValueByIndex(IndexOfNextOperation, GroupTokens);

		if (IndexOfNextOperation - 1 >= 0 && IndexOfNextOperation + 1 < GroupTokens->CurrentIndex) {
			 result = HandleOperations(IndexOfNextOperation, GroupTokens, operation);
			ArrayInfo* newArray =RemoveRange(GroupTokens, IndexOfNextOperation - 1, IndexOfNextOperation);//removing only the 2 values for example 2*2 ;only the 2* will be removed
			GroupTokens = newArray;
			Token* newSovToken = calloc(1, sizeof(Token));
			newSovToken->Value = doubleToString(&result);
			newSovToken->TokenType = TokenType_Number;
			newSovToken->TokenTree = TokenTree_Single;
			replace(GroupTokens, newSovToken, IndexOfNextOperation - 1);
		}
		else {
			printf("invalid token : operation missing left or right side value.\n");
			return 0;
		}	 IndexOfNextOperation = IndexOf(&isMul_Dev_Power, 0, GroupTokens);

	}
	IndexOfNextOperation = IndexOf(&isOperator, 0, GroupTokens);
	while (IndexOfNextOperation != -1 && GroupTokens->CurrentIndex > 2)
	{
		Token* operation = *(Token**)GetValueByIndex(IndexOfNextOperation, GroupTokens);

		if (IndexOfNextOperation - 1 >= 0 && IndexOfNextOperation + 1 < GroupTokens->CurrentIndex) {
			 result = HandleOperations(IndexOfNextOperation, GroupTokens, operation);
			ArrayInfo* newArray = RemoveRange(GroupTokens, IndexOfNextOperation - 1, IndexOfNextOperation);//removing only the 2 values for example 2*2 ;only the 2* will be removed
			FreeArray(GroupTokens);
			GroupTokens = newArray;
			Token* newSovToken = calloc(1, sizeof(Token));
			
			newSovToken->Value = doubleToString(&result);
			newSovToken->TokenType = TokenType_Number;
			newSovToken->TokenTree = TokenTree_Single;
			replace(GroupTokens, newSovToken, IndexOfNextOperation - 1);
		}
		else {
			printf("invalid token : operation missing left or right side value.\n");
			return 0;
		}
		IndexOfNextOperation = IndexOf(&isOperator, 0, GroupTokens);

	}
	if (GroupTokens->CurrentIndex >= 2) {
		printf("invalid tokens / wrong token index \n");
		return 0;
	}
	Token* tok = *(Token**)GetValueByIndex(0, GroupTokens);
	GroupTokens->Array = 0;
	FreeArray(GroupTokens);

	return tok;

}

// Converts a double pointer to a string and returns the string's address
void* doubleToString(double* valuePtr) {
	if (valuePtr == NULL) return NULL;

	char* buffer = malloc(32);  // Allocate memory for the string
	if (buffer == NULL) return NULL;  // Check malloc success

	snprintf(buffer, 32, "%f", *valuePtr);  // Format the double value
	return buffer;  // Return the string's address
}

static double HandleOperations(int IndexOfNextOperation, ArrayInfo* token,Token* operation) {

		Token* left_token = *(Token**)GetValueByIndex(IndexOfNextOperation - 1,token);
		if (left_token->TokenType != TokenType_Number) {
			printf("invalid token %s before %c", left_token->Value, operation->Value);
			return 0.0;
		}
		Token* right_token = *(Token**)GetValueByIndex( IndexOfNextOperation + 1, token);

		if (right_token ==NULL||right_token->TokenType != TokenType_Number) {
			printf("invalid token %s after %s");
			return 0;
		}
		char* success;
		double leftvalue = strtod(left_token->Value, &success);
		if (success == '\0') {
			printf("invalid token given before %c : %s", operation->Value, left_token->Value);
			return 0;

		}
		double rightvalue = strtod(right_token->Value, &success);
		if (success == '\0') {
			printf("invalid token given after %c : %s", operation->Value, left_token->Value);
			return 0;
		}
		
		double result = Eval(leftvalue, operation->TokenOperation, rightvalue);
		free(left_token->Value);
		free(right_token->Value);
		free(right_token);
		free(left_token);
		free(operation->Value);
		free(operation);
		return result;

}
static double Eval(double left, TokenOperation operation, double right) {
	switch (operation) {
	case TokenOperation_AddOperation:
		return left + right;

	case TokenOperation_SubtractOperation:
		return left - right;

	case TokenOperation_MultiplyOperation:
		return left * right;

	case TokenOperation_DivideOperation:
		if (right == 0) {
			printf("Error: can't divide by zero.\n");
			return 0;
		}
		return left / right;

	case TokenOperation_PowerOperation:
		return pow(left, right);

	case TokenOperation_ANDOperation:
		return (double)((int)left & (int)right);

	case TokenOperation_OROperation:
		return (double)((int)left | (int)right);

	case TokenOperation_ShiftLeftOperation:
		return (double)((int)left << (int)right); 

	case TokenOperation_ShiftRightOperation:
		return (double)((int)left >> (int)right); 

	default:
		printf("Error: Unknown operation.\n");
		return 0;
	}
}
static ArrayInfo* GetParams(char* strings) {
	int OpenPres = 0;
	char hit1 = 0;
	__new__array(params, -1, sizeof(void*));
	__new__array(current_temp_value, -1, 1);
	int stringLength = LengthOf(strings);
	for (size_t i = 0; i < stringLength; i++)
	{
		if ((*(strings + i) == ',' && OpenPres == 0) ) {

			hit1 = 1;
			char* single_param = calloc(__name__of__array(current_temp_value)->CurrentIndex + 1, 1);
			memcopy(single_param, __name__of__array(current_temp_value)->Array, __name__of__array(current_temp_value)->CurrentIndex);
			__add__array(params, single_param);
			__array__remove_range(current_temp_value, 0, __name__of__array(current_temp_value)->CurrentIndex);
			continue;
		}
		else if (*(strings + i) == '(')
		{
			OpenPres++;
		}
		else if (*(strings + i) == ')')
		{
			OpenPres--;
		}
		
		__add__array__copy(current_temp_value, (strings + i));


	}
	if (__name__of__array(current_temp_value)->CurrentIndex>0) {
		
		char* single_param = calloc(__name__of__array(current_temp_value)->CurrentIndex + 1, 1);
		memcopy(single_param, __name__of__array(current_temp_value)->Array, __name__of__array(current_temp_value)->CurrentIndex);
		__add__array(params, single_param);
		__array__remove_range(current_temp_value, 0, __name__of__array(current_temp_value)->CurrentIndex);
	}
	FreeArray(__name__of__array(current_temp_value));
	return __name__of__array(params);
}
static char isGroup(Token** token) {
	if (*token == NULL)
		return 0;
	return (*token)->TokenTree == TokenTree_Group;

}
static char isOperator(Token** token) {
	return (*token)->TokenType == TokenType_Operation ;
}
static char isMul_Dev_Power(Token** token){
	if (*token == NULL)
		return 0;
	if ((*token)->TokenType != TokenType_Operation)
		return 0;
	if ((*token)->TokenOperation == TokenOperation_MultiplyOperation ||
		(*token)->TokenOperation == TokenOperation_DivideOperation
		|| (*token)->TokenOperation == TokenOperation_PowerOperation) {
		return 1;
	}
	return 0;

}