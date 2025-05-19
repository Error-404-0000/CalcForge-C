#include "List.h"
#pragma once

char AreLetters(char* Char, int leng);
ArrayInfo* Parse(ArrayInfo* Rawtokens);
typedef enum TokenType {
    TokenType_None,
    TokenType_Function,
    TokenType_Number,
    TokenType_Operation,
    TokenOperation_Expression,
    TokenType_ParenthesisOpen,
    TokenType_ParenthesisClose

}TokenType;


typedef enum TokenTree
{
    TokenTree_Single,
    TokenTree_Group
} TokenTree;
typedef enum TokenOperation {
    TokenOperation_None,
    

    TokenOperation_AddOperation,
    TokenOperation_SubtractOperation,
    TokenOperation_MultiplyOperation,
    TokenOperation_DivideOperation,
    TokenOperation_PowerOperation,
    TokenOperation_ShiftLeftOperation,
    TokenOperation_ShiftRightOperation,
    TokenOperation_OROperation,
    TokenOperation_ANDOperation,
    TokenOperation_Reminder,
    TokenOperation_Pow,
    TokenOperation_Abs,
    TokenOperation_Sin,
    TokenOperation_Cos,
    TokenOperation_Tan,
    TokenOperation_Sqrt,
    TokenOperation_Log,
    TokenOperation_Print,
    TokenOperation_Log10,
    TokenOperation_Exp,
    TokenOperation_Floor,
    TokenOperation_Ceiling,
    TokenOperation_Round,
    TokenOperation_Truncate,
    TokenOperation_Max,
    TokenOperation_Min,
    TokenOperation_Add,
    TokenOperation_Subtract,
    TokenOperation_Multiply,
    TokenOperation_Divide
}TokenOperation;

typedef struct {
    TokenType TokenType;
    TokenOperation TokenOperation;
    TokenTree TokenTree;
    void* Value;
} Token;
typedef struct {
    char* FuncName;
    char* raw_token;
}FuncationToken;

 char* TokenTypeToString(TokenType type);
 char* TokenTreeToString(TokenTree tree);
void memcopy(void* dest, const void* src,int size);
const char* TokenOperationToString(TokenOperation op);