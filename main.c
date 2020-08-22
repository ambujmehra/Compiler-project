
#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"
#include "parserDef.h"
#include "connect.h"

int main()
{
	lexer_helper();	
	parser_helper();
	ast_helper();
	SymbolTable_helper();
	semantic_analysis_helper();
	CodeGeneration_helper();
	return 0;
}

