#program: lexer.o parser.o main.o ast.o SymbolTable.o SemanticAnalysis.o
#	gcc -o compiler lexer.o parser.o ast.o SymbolTable.o SemanticAnalysis.o main.o
program: lexer.o parser.o main.o ast.o SymbolTable.o SemanticAnalysis.o CodeGeneration.o
	gcc -o compiler lexer.o parser.o ast.o SymbolTable.o CodeGeneration.o SemanticAnalysis.o main.o
main.o:
	gcc -c  main.c -g
lexer.o: 
	gcc -c lexer.c -g
parser.o:
	gcc -c parser.c -g
ast.o:
	gcc -c ast.c -g
SymbolTable.o:
	gcc -c SymbolTable.c -g
SemanticAnalysis.o:
	gcc -c SemanticAnalysis.c -g
CodeGeneration.o:
	gcc -c CodeGeneration.c -g

rm:
	rm *.o
	rm *~
