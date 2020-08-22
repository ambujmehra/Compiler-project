# ifndef LEXERDEFH_
# define LEXERDEFH_

typedef struct
{
	char lexeme[100];
	int line_number;
	char token[20];
}Token;

#endif
