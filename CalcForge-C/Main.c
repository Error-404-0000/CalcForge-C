#include <stdio.h>
#include <stdlib.h>
#include <time.h> // For measuring time
#include "List.h"
#include "Tokenizer.h"
#include "Strings.h"
#include "Parser.h"
#include "Evaluator.h"

void PrintTokens(void* string);
void PrintTokenBlock(Token** token);

char* GetTree(TokenTree tokenTree) {
	switch (tokenTree) {
	case TokenTree_Single: return "Single";
	case TokenTree_Group: return "Group";
	default: return "Unknown";
	}
}

void PrintTokenBlock(Token** token_) {
	if (!token_) return;
	Token* token = (*(Token**)token_);
	printf(" [  Type: %s\n", TokenTypeToString(token->TokenType));
	printf("    Operation: %s\n", TokenOperationToString(token->TokenOperation));
	printf("    Tree: %s\n", GetTree(token->TokenTree));
	if (token->TokenTree == TokenTree_Group) {
		printf("    Value: %s ] \n\n", token->Value);
	}
	else if (token->TokenType == TokenType_Function) {
		FuncationToken* body = (FuncationToken*)token->Value;
		printf("    Function-name : %s \n    values : %s ] \n\n", body->FuncName, body->raw_token);
	}
	else if (token->Value)
		printf("    Value: %s ] \n\n", token->Value);
}

void PrintTokens(void* string) {
	printf("[ %s ]\n", *(char**)string);
}

int main() {
<<<<<<< HEAD
	const char* expr =
		"add(pow(2, 5), subtract(100, divide(50, 2))) + "
		"multiply(add(1, 2), subtract(10, 3)) + "
		"sqrt(add(500, 250)) +((((((2 + 0) + 3) * ((4 * 1) + (0 + 5))) + 2) / (((6 + 6) * 1) * (1 + 0 + 1))) + ((100 / 5) - ((3 + 2) + 0))) + (((7 * 8) + (9 - 4 + 0)) * (((2 + 3 + 0) * 1) + 2)) - (((10 + 0 + 5) * 1) * ((6 - 0 - 1))) + ((((50 + 50) / 2) * (((3 + 2) - 0) - (1 + 1)))) + 0 + 0 * 1234 ";
=======
	const char* expr = "Sqrt(102)";
>>>>>>> ....Add project files.


	clock_t start = clock();

	const ArrayInfo* tokens = Tokenize((char*)expr);
	ArrayInfo* ParsedTokens = Parse(tokens);
	double result = Evaluate(ParsedTokens);

	clock_t end = clock();
	double elapsed_secs = (double)(end - start) / CLOCKS_PER_SEC;

	printf("\nExpression: %s\n", expr);
	printf("Result: %f\n", result);
	printf("Time taken: %.6f seconds\n", elapsed_secs);

	return 0;
}
