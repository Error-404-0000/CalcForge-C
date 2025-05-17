#include  "Parser.h"
#include "Strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 6011)
#pragma once
const TokenType GetTokenType(const char Value);
const TokenType GetTokenType(const char Value);
int FindOpenedParenthesisEnd(char** strings, int count);
void memcopy(void* dest, const void* src, size_t n);
ArrayInfo* Parse(const ArrayInfo* Rawtokens) {
    __new__array(Tokens, -1, sizeof(void*));
    for (int i = 0; i < Rawtokens->CurrentIndex; i++)
    {
        char* currentStringToken = *(char**)(Rawtokens->Array + (i* Rawtokens->object_size));
        int currentTokenStringLength = LengthOf(currentStringToken);
        if (AreNumbers(currentStringToken, currentTokenStringLength)) {
            Token* token = calloc(1, sizeof(Token));
            token->TokenOperation = TokenOperation_None;
            token->TokenTree = TokenTree_Single;
            token->TokenType = TokenType_Number;
            token->Value = calloc(currentTokenStringLength+1, 1);
            memcopy(token->Value, currentStringToken, currentTokenStringLength);
            *((char*)token->Value + currentTokenStringLength ) = '\0';
            __add__array(Tokens, token);
        }
        else if (AreLetters(currentStringToken, currentTokenStringLength)) {
            Token* token = calloc(1, sizeof(Token));
            if (i + 2 >= Rawtokens->CurrentIndex )
            {
                printf("unclosed parenthesis at index %i", i);
                return 0;

            }
            int IndexofGroupParms = FindOpenedParenthesisEnd((char**)(Rawtokens->Array  + ((i+2) * Rawtokens->object_size)), Rawtokens->CurrentIndex - i);
            if (IndexofGroupParms == -1) {
                printf("unclosed parenthesis at index %i", i);
                return 0 ;
            }
            token->TokenType = TokenType_Function;
            token->TokenOperation = GetTokenOperationFromString(currentStringToken);
          
            if (token->TokenOperation == TokenOperation_None) {
                printf("unknown function named : %s.\n", currentStringToken);
                return 0;
            }            
            token->TokenTree = TokenTree_Single;
            FuncationToken* FuncToken = (FuncationToken*)calloc(1, sizeof(FuncationToken) );
            if (!FuncToken) {
                printf("failed to alloc mem of token");
                return 0;
            }
            char* FunctionName = calloc(1, currentTokenStringLength + 1);
            FuncToken->FuncName = FunctionName;
            memcopy(FuncToken->FuncName, currentStringToken, currentTokenStringLength);

            ArrayInfo* joinedArray = Join(GET_ELEMENT_PTR(Rawtokens->Array, Rawtokens->object_size, i + 2), IndexofGroupParms);
            FuncToken->raw_token = calloc(1, joinedArray->CurrentIndex + 1);
            memcopy(FuncToken->raw_token, joinedArray->Array, joinedArray->CurrentIndex);
            token->Value = FuncToken;
            FreeArray(joinedArray);
            i += IndexofGroupParms + 2;//skipping the full group
            __add__array(Tokens, token);
            continue;
        }
        else {
            Token* token = calloc(1, sizeof(Token));
            if (token) {
                token->TokenType = GetTokenType(*currentStringToken);
                if (token->TokenType == TokenType_ParenthesisOpen) {
                    if (i + 2 >= Rawtokens->CurrentIndex )
                    {
                        printf("unclosed parenthesis at index %i", i);
                        return 0;

                    }
                    int IndexofGroupParms = FindOpenedParenthesisEnd((char**)(Rawtokens->Array + ((i + 1) * Rawtokens->object_size)), Rawtokens->CurrentIndex - i);
                    if (IndexofGroupParms == -1) {
                        printf("unclosed parenthesis at index %i", i);
                        return 0;
                    }
                    token->TokenType = TokenOperation_Expression;
                    token->TokenOperation = TokenOperation_None;
                    token->TokenTree = TokenTree_Group;

                    ArrayInfo* GroupStrings = Join(GET_ELEMENT_PTR(Rawtokens->Array, Rawtokens->object_size, i + 1), IndexofGroupParms);
                    token->Value = calloc(1, GroupStrings->CurrentIndex + 1);
                    memcopy(token->Value, GroupStrings->Array, GroupStrings->CurrentIndex);
                    FreeArray(GroupStrings);
                    i += IndexofGroupParms + 1;//skipping the full group
                    __add__array(Tokens, token);
                    continue;
                }
                else  if (token->TokenType == TokenType_ParenthesisClose) {
                    printf("closed parenthesis at index %i without opened parenthesis\n", i);
                    return 0;
                }
                else {
                   
                    token->TokenOperation = GetTokenOperationFromString(currentStringToken);
                    if (token->TokenOperation == TokenType_None) {
                        printf("unknown function token at index %i : %s\n", i, currentStringToken);
                        return 0;
                    }
                    token->TokenTree = TokenTree_Single;
                    token->Value = calloc(currentTokenStringLength + 1, 1);
                    memcopy(token->Value, currentStringToken, currentTokenStringLength);
                    *((char*)token->Value + currentTokenStringLength) = '\0';
                    __add__array(Tokens, token);
                }
            }
            else {
                return 0;

            }
            
        }

    }
    return __array__Tokens;
}
int FindOpenedParenthesisEnd(char** strings,unsigned int count) {
    int OpnedParenthesis = 1;
    for (int i = 0; i < count; i++)
    {
        if (OpnedParenthesis == 0)
            return i - 1;
        else if (**(strings + i) == '(')
            OpnedParenthesis++;
        else if (**(strings + i) == ')')
            OpnedParenthesis--;
    }
    return -1;
}
const TokenType GetTokenType(const char Value) {
     switch (Value) {
     case '+':
     case '-':
     case '*':
     case '/':
     case '^':
     case '<':
     case '>':
     case '&':
     case '%':
        return TokenType_Operation;
        case '(':
            return TokenType_ParenthesisOpen;
        case ')':
            return TokenType_ParenthesisClose;
        default: TokenType_None;
     }
}

TokenOperation GetTokenOperationFromString(const char* str) {
    if (strcmp(str, "+") == 0) return TokenOperation_AddOperation;
    if (strcmp(str, "-") == 0) return TokenOperation_SubtractOperation;
    if (strcmp(str, "*") == 0) return TokenOperation_MultiplyOperation;
    if (strcmp(str, "/") == 0) return TokenOperation_DivideOperation;
    if (strcmp(str, "^") == 0) return TokenOperation_PowerOperation;
    if (strcmp(str, "<") == 0) return TokenOperation_ShiftLeftOperation;
    if (strcmp(str, ">") == 0) return TokenOperation_ShiftRightOperation;
    if (strcmp(str, "|") == 0) return TokenOperation_OROperation;
    if (strcmp(str, "&") == 0) return TokenOperation_ANDOperation;
    if (strcmp(str, "%") == 0) return TokenOperation_Reminder;

    // Functions
    if (_stricmp(str, "pow") == 0) return TokenOperation_Pow;
    if (_stricmp(str, "abs") == 0) return TokenOperation_Abs;
    if (_stricmp(str, "sin") == 0) return TokenOperation_Sin;
    if (_stricmp(str, "cos") == 0) return TokenOperation_Cos;
    if (_stricmp(str, "tan") == 0) return TokenOperation_Tan;
    if (_stricmp(str, "sqrt") == 0) return TokenOperation_Sqrt;
    if (_stricmp(str, "log") == 0) return TokenOperation_Log;
    if (_stricmp(str, "log10") == 0) return TokenOperation_Log10;
    if (_stricmp(str, "exp") == 0) return TokenOperation_Exp;
    if (_stricmp(str, "floor") == 0) return TokenOperation_Floor;
    if (_stricmp(str, "ceiling") == 0) return TokenOperation_Ceiling;
    if (_stricmp(str, "round") == 0) return TokenOperation_Round;
    if (_stricmp(str, "truncate") == 0) return TokenOperation_Truncate;
    if (_stricmp(str, "max") == 0) return TokenOperation_Max;
    if (_stricmp(str, "min") == 0) return TokenOperation_Min;
    if (_stricmp(str, "add") == 0) return TokenOperation_Add;
    if (_stricmp(str, "subtract") == 0) return TokenOperation_Subtract;
    if (_stricmp(str, "multiply") == 0) return TokenOperation_Multiply;
    if (_stricmp(str, "divide") == 0) return TokenOperation_Divide;
        
    return TokenOperation_None;
}

const char* TokenTypeToString(TokenType type) {
    static const char* names[] = {
        "TokenType_None",
        "TokenType_Function",
        "TokenType_Number",
        "TokenType_Operation",
        "TokenOperation_Expression",
        "TokenType_ParenthesisOpen",
        "TokenType_ParenthesisClose"
    };
    return (type >= 0 && type <= TokenType_ParenthesisClose) ? names[type] : "Unknown";
}

const char* TokenOperationToString(TokenOperation op) {
    static const char* names[] = {
        "TokenOperation_None",
        "TokenOperation_AddOperation",
        "TokenOperation_SubtractOperation",
        "TokenOperation_MultiplyOperation",
        "TokenOperation_DivideOperation",
        "TokenOperation_PowerOperation",
        "TokenOperation_ShiftLeftOperation",
        "TokenOperation_ShiftRightOperation",
        "TokenOperation_OROperation",
        "TokenOperation_ANDOperation",
        "TokenOperation_Reminder",
        "TokenOperation_Pow",
        "TokenOperation_Abs",
        "TokenOperation_Sin",
        "TokenOperation_Cos",
        "TokenOperation_Tan",
        "TokenOperation_Sqrt",
        "TokenOperation_Log",
        "TokenOperation_Print",
        "TokenOperation_Log10",
        "TokenOperation_Exp",
        "TokenOperation_Floor",
        "TokenOperation_Ceiling",
        "TokenOperation_Round",
        "TokenOperation_Truncate",
        "TokenOperation_Max",
        "TokenOperation_Min",
        "TokenOperation_Add",
        "TokenOperation_Subtract",
        "TokenOperation_Multiply",
        "TokenOperation_Divide"
    };
    return (op >= 0 && op <= TokenOperation_Divide) ? names[op] : "Unknown";
}
const char* TokenTreeToString(TokenTree tree) {
    static const char* names[] = {
        "TokenTree_Single",
        "TokenTree_Group"
    };
    return (tree >= 0 && tree <= TokenTree_Group) ? names[tree] : "Unknown";
}

void memcopy(void* dest, const void* src, int n) {
    for (int i = 0; i < n; i++)
    {
        *((char*)dest + i) = *((char*)src + i);
    }
}