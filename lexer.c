#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"

#define LETTER 0
#define DIGIT 1
#define MOD 10007
#define NUM_TOKENS 66
#define buff_size 45


// Global Variables
int Char_Class,lexlen;
char lexeme[100];
char tokens[66][100];
int hash_table[66];
char next_char;
int state;
int line_number = 1;
char str[40];
char c;
FILE* output;

Token t;

char getChar(FILE* f)
{
	char c;
	fscanf(f,"%c",&c);
	if(c == EOF)
		return '1';
	return c;
}

void print_Token(Token t){fprintf(output,"%s	%s	%d\n",t.token,t.lexeme,t.line_number);}

Token generate_new_token(char temp[],int line_number1,char lexeme[])
{
	Token t;
	if(c == '\n')	line_number --;
	line_number1 = line_number;
	t.line_number = line_number1;
	strcpy(t.lexeme,lexeme);
	strcpy(t.token,temp);
	if(c == '\n')	line_number ++;
	return t;
}
int isNumeric(char c)
{
	if(c >= '0' && c<='9'){ return 1;}
	else	{ return 0;}
}
int check_char(char c)
{
	if(c == '\0' || c == ' ' || c == '\n' || c == '[' || c == ']' || c == '(' || c == ')' || c == 10 || c == 13)
	{	return 1;}
	else if(c == '\t' || c == ',' ||c == ';' || c == '+' || c == '-' || c == '/' || c == '*' || c == ':')
	{ return 1;}
	else{ return 0;}
}

void lexical_analysis(FILE* input_file)
{
	char buffer[47];
	int count = 0,i = 0,j=0;
	
	state = 0;
	i = 0;
	line_number = 1;
	while(1)
	{
		if(feof(input_file))
			{
				buffer[count] = '\0';
				break;
			}
		else
		{
			count = 0;
			while(count < buff_size)
			{
				c = getChar(input_file);
				buffer[count++] = c;
			}
			buffer[count] = '\0';
		}
		
			for(j=0;j<buff_size;j++)
			{			
				c = buffer[j];
				//printf("%c %d\n",c,(int)c);

			if(c == '\n'){line_number++;}
	
				switch(state)
				{
				
				
				/// For indentifier.
				case 188:
					//printf("Hello Here\n");
					//printf("%d\n",(int)c);

					if(strlen(lexeme) > 30)
					{
						printf("Lexical Error. Token Name too long.\n");
						break;
					}

					if((c>= 'a' && c<= 'z')||(c>='A' && c<='Z')||(c>='0' && c<='9'))
					{state = 188;lexeme[i++] = c;}
					
					else if(check_char(c))
					{
						lexeme[i] = '\0';
						char temp[] = "IDEN";
						t = generate_new_token(temp,line_number,lexeme);
						print_Token(t);
						i = 0;state = 0;
						j--;
					}
					
					else
					{

						lexeme[i] = '\0';
						char temp[] = "IDEN";
						t = generate_new_token(temp,line_number,lexeme);
						print_Token(t);	i = 0;	j--;	state = 0;
					}
					break;
				case 0:
				if(c == ' '||c == '\n'|| c== '\t' || c == 13){continue;}
				else if(isNumeric(c)) { state = 197; lexeme[i++] = c;}
				else if(c == 'P'){	state = 1;	lexeme[i++]=c;}
				else if(c == 'E'){	state = 5;	lexeme[i++]=c;}
				else if(c == 'G'){	state = 12;	lexeme[i++]=c;}
				else if(c == ']'){	state = 22;	lexeme[i++]=c;}
				else if(c == '['){ 	state = 21;	lexeme[i++]=c;}
				else if(c == ','){ 	state = 24;	lexeme[i++]=c;}
				else if(c == ';'){	state = 25;	lexeme[i++]=c;}
				else if(c == '('){	state = 19;	lexeme[i++]=c;}
				else if(c == ')'){	state = 20;	lexeme[i++]=c;}
				else if(c == '%'){	state = 18;	lexeme[i++]=c;}
				else if(c == ':'){	state = 23;	lexeme[i++]=c;}
				else if(c == 'I'){	state = 26;	lexeme[i++]=c;}
				else if(c == 'D'){	state = 33;	lexeme[i++]=c;}
				else if(c == 'S'){	state = 40;	lexeme[i++]=c;}
				else if(c == 'B'){	state = 46;	lexeme[i++]=c;}
				else if(c == 'i'){	state = 53;	lexeme[i++]=c;}
				else if(c == 'e'){	state = 55;	lexeme[i++]=c;}
				else if(c == 'b'){	state = 59;	lexeme[i++]=c;}
				else if(c == 'p'){	state = 68;	lexeme[i++]=c;}
				else if(c == 's'){	state = 73;	lexeme[i++]=c;}
				else if(c == 'C'){	state = 77;	lexeme[i++]=c;}
				else if(c == 'r'){	state = 101;lexeme[i++]=c;}
				else if(c == 'T'){	state = 107;lexeme[i++]=c;}
				else if(c == 'F'){	state = 111;lexeme[i++]=c;}
				else if(c == 'w'){	state = 116;lexeme[i++]=c;}
				else if(c == 'L'){	state = 121;lexeme[i++]=c;}
				else if(c == 'a'){	state = 130;lexeme[i++]=c;}
				else if(c == 'm'){	state = 136;lexeme[i++]=c;}
				else if(c == 'N'){	state = 144;lexeme[i++]=c;}
				else if(c == '#'){	state = 161;lexeme[i++]=c;}
				else if(c == '~'){	state = 162;lexeme[i++]=c;}
				else if(c == '$'){	state = 163;lexeme[i++]=c;}
				else if(c == '?'){	state = 164;lexeme[i++]=c;}
				else if(c == '<'){	state = 165;lexeme[i++]=c;}
				else if(c == '>'){	state = 167;lexeme[i++]=c;}
				else if(c == '='){	state = 169;lexeme[i++]=c;}
				else if(c == '&'){	state = 171;lexeme[i++]=c;}
				else if(c == '|'){	state = 172;lexeme[i++]=c;}
				else if(c == '!'){	state = 173;lexeme[i++]=c;}
				else if(c == '+'){	state = 174;lexeme[i++]=c;}
				else if(c == '-'){	state = 175;lexeme[i++]=c;}
				else if(c == '/'){	state = 177;lexeme[i++]=c;}
				else if(c == '*'){	state = 176;lexeme[i++]=c;}
				else if(c == '_'){	state = 178;lexeme[i++]=c;}
				else if(c == '@'){	state = 179;}
				else if(c == '"'){	state = 181;lexeme[i++]=c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 1: 
				if(c == 'r')
				{	state = 2;lexeme[i++]=c;}
				else{state = 188;	lexeme[i++] = c;}
				break;
				case 2:
				if(c == 'o')
				{	state = 3;lexeme[i++]=c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 3:
				if(c == 'g')
				{	state = 4;lexeme[i++]=c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 4:			/// Reduce Prog keyword
				if(check_char(c))
				{
					printf("%c\n",c);
					lexeme[i]='\0';	
					state = 0;	strcpy(str,"PROG");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
					if(c !='\n') j--;
				}
				else
				{	state = 188;lexeme[i++] = c;}
				break;
				
				case 5:
				if(c == 'n'){	state = 6;lexeme[i++] = c;}
				else{
				state = 188;lexeme[i++] = c;}
				break;
				
				case 6:
				if(c == 'd'){	state = 7;	lexeme[i++] = c;}
				else
				{	
				state = 188;lexeme[i++] = c;}
				break;
				
				case 7:
				if(c == 'P'){	state = 8;lexeme[i++] = c;}
				else if(c == 'i')
				{
					state = 195;lexeme[i++] = c;}
				else if(c == 'S')
				{
					state = 40;lexeme[i++] = c;}
				else if(c == 'G')
				{
					state = 12;lexeme[i++] = c;}
				else if(c == 'L')
				{
					state = 121;lexeme[i++] =c;}
				else if(check_char(c))
				{
					lexeme[i]='\0';
					state = 0;	strcpy(str,"END");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);i = 0;	
				}
				else
				{	state = 188;lexeme[i++] = c;}
				break;
				
				case 8:
				if(c == 'r'){	state = 9;lexeme[i++] = c;}
				else
				{	state = 188;lexeme[i++] = c;}
				break;
				
				case 9:
				if(c == 'o'){	state = 10;lexeme[i++] = c;}
				else
				{	state = 188;lexeme[i++] = c;}
				break;
				
				case 10:
				if(c == 'g'){	state = 11;lexeme[i++] = c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 11:
				if(check_char(c))
				{
					lexeme[i]='\0';
					state = 0;strcpy(str,"ENDPROG");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);i = 0;
				}
				else
				{	state = 188;lexeme[i++] = c;}
				break;
				
				case 12:
				if(c == 'l'){	state = 13;lexeme[i++] = c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 13:
				if(c == 'o'){	state = 14;lexeme[i++] = c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 14:
				if(c == 'b'){	state = 15;lexeme[i++] = c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 15:
				if(c == 'a'){	state = 16;lexeme[i++] = c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 16:
				if(c == 'l'){	state = 17;lexeme[i++] = c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 17:		/// Reduce Func_Start
				if(check_char(c))
				{
					lexeme[i] = '\0';	state = 0;
					if(strcmp(lexeme,"EndGlobal") == 0)
					{
						strcpy(str,"ENDGLOBAL");
						t = generate_new_token(str,line_number,lexeme);
						print_Token(t);	i = 0;
						if(c != '\n') j--;}
					else
					{
						strcpy(str,"GLOBAL");
						t = generate_new_token(str,line_number,lexeme);
						print_Token(t);	i = 0;
						if(c != '\n') j--;}
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 18:		/// Reduce Func_Start
				
				lexeme[i] = '\0';	state = 0;
				strcpy(str,"FUNC_START");
				t = generate_new_token(str,line_number,lexeme); 
				print_Token(t); i = 0;	j--;
				break;
				
				case 19:			// Reduce Open Paranthesis
				
				lexeme[i] = '\0';	state = 0;
				strcpy(str,"O_PAR");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);	i = 0;
				if(c != '\n') j--;
				break;
				
				case 20:			// Reduce Closed Paranthesis
				if(check_char(c))
				{
					lexeme[i] = '\0';	state = 0;
					strcpy(str,"C_PAR");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;
					if(c != '\n') j--;
				}
				else
				{
					lexeme[i] = '\0';	state = 0;
					strcpy(str,"C_PAR");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
				}
				break;
				
				case 24:			// Reduce Closed Paranthesis
				lexeme[i] = '\0';	state = 0;
				strcpy(str,"COMMA");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(c != '\n') j--;
				break;
				
				case 23:			// Reduce Colon
				lexeme[i] = '\0';	state = 0;
				strcpy(str,"COLON");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(c != '\n') j--;
				break;
				
				case 40:			
				if(c == 't'){	state = 41;lexeme[i++] = c;}
				else if(c == 'u'){	state = 189;lexeme[i++] = c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 189:
				
				if(c == 'b'){	state = 190;lexeme[i++] = c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
			
				case 190:
				
				lexeme[i++] = c;
				if(c == 'P'){	state = 191;}
				else{	state = 188;}
				break;
				
				case 191:
				
				lexeme[i++] = c;
				if(c == 'r'){	state = 192;}
				else{	state = 188;}
				break;
				
				case 192:
				lexeme[i++] = c;
				if(c == 'o'){	state = 193;}
				else{	state = 188;}
				break;
				
				case 193:
				lexeme[i++] = c;
				if(c == 'g'){	state = 194;}
				else{	state = 188;}
				break;
				
				case 194:			// Reduce SubProg
				if(check_char(c))
				{
					state = 0;
					lexeme[i] = '\0';	
					if(strcmp(lexeme,"EndSubProg") == 0)
					{
						strcpy(str,"ENDSUBPROG");
						t = generate_new_token(str,line_number,lexeme);
						print_Token(t);
						i = 0;
					}
					else
					{
						strcpy(str,"SUBPROG");
						t = generate_new_token(str,line_number,lexeme);
						print_Token(t);
						i = 0;
					}
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 21:		// Reduce LSB
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"LSB");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);	i = 0;
				j--;
				//if(!check_char(c)){j--;}
				break;
				
				case 22:			// Reduce RSB
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"RSB");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(c != '\n') {j--;}
				break;
				
				case 25:			// Reduce SEMICOLON
				if(check_char(c))
				{
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"SEMICOLON");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!(c == '\n')) j--;
				}
				break;
				
				case 26:
				if(c == 'n'){	state = 27;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 27:
				if(c == 't'){	state = 28;lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 28:
				if(c == 'e'){	state = 29;lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 29:
				
				if(c == 'g'){	state = 30;lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 30:
				if(c == 'e'){	state = 31;lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 31:
				if(c == 'r'){	state = 32;lexeme[i++]= c;	}
				else{	state = 188;}
				break;
				
				case 32:			// Reduce Integer
				if(check_char(c)){state = 0;
				lexeme[i] = '\0';
				strcpy(str,"INTEGER");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t); i = 0;j--;}
				else{state = 188; lexeme[i++] = c;}
				break;
				
				case 33:
				if(c == 'e'){	state = 34;lexeme[i++]= c;}
				else if(check_char(c))
				{
					state = 0;
					lexeme[i] = '\0';
					strcpy(str,"IDEN");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
					if(c!= '\n') {j--;}
				}
				else
				{
					state = 188;lexeme[i++] = c;
				}
				break;
				
				case 34:
				
				if(c == 'c'){	state = 35;	lexeme[i++]= c;}
				else{	state = 188; lexeme[i++] = c;}
				break;
				
				case 35:
				if(c == 'i'){	state = 36;	lexeme[i++]= c;}
				else{	state = 188; lexeme[i++] = c;}
				break;
				
				case 36:
				
				if(c == 'm'){	state = 37;	lexeme[i++]= c;}
				else{	state = 188; lexeme[i++] = c;}
				break;
				case 37:
				
				if(c == 'a'){	state = 38;	lexeme[i++]= c;}
				else{	state = 188; lexeme[i++] = c;}
				break;
				
				case 38:
				if(c == 'l'){	state = 39;	lexeme[i++]= c;}
				else{	state = 188; lexeme[i++] = c;}
				break;
				
				case 39:			// Reduce Decimal
				if(check_char(c)){state = 0;
				lexeme[i] = '\0';
				strcpy(str,"DECIMAL");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;j--;}
				else{	state = 188;lexeme[i++] = c;}
				break;
			
				case 41:
				if(c == 'r'){	state = 42;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 42:
				if(c == 'i'){	state = 43;	lexeme[i++]= c;}
				else if(c == 'u'){state = 154;	lexeme[i++] = c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 43:
				if(c == 'n'){	state = 44;	lexeme[i++]= c;}
				else{	state = 188; lexeme[i++] = c;}
				break;
				
				case 44:
				if(c == 'g'){	state = 45;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 45:			// Reduce String
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"STRING");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
				}
				else{	lexeme[i++] = c;	state = 188;}
				break;
				
				case 46:
				if(c == 'o'){	state = 47;	lexeme[i++]= c;}
				else if(c == 'e'){	state = 60;	lexeme[i++] = c;}
				else if(check_char(c)){
				state = 0;	lexeme[i] = '\0';
				strcpy(str,"IDEN");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);	i = 0;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 47:
				
				if(c == 'o'){	state = 48;	lexeme[i++]= c;	}
				else{	state = 188; lexeme[i++] = c;}
				break;
				
				case 48:
				if(c == 'l'){	state = 49;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}	
				break;
				
				case 49:
				if(c == 'e'){	state = 50;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] =c;}
				break;
		
				case 50:
				if(c == 'a'){	state = 51;	lexeme[i++]= c;}
				else{	state = 188; lexeme[i++] = c;}
				break;
				
				case 51:
				if(c == 'n'){state = 52;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				
				case 52:			// Reduce Boolean
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"BOOLEAN");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 53:
				if(c == 'f'){state = 54;lexeme[i++]= c;}
				else if(c == 'n'){	state = 87;	lexeme[i++] = c;}
				else if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"IDEN");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;	j--;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 54:			// Reduce IF
				if(check_char(c))
				{
					state = 0;
					lexeme[i] = '\0';
					strcpy(str,"IF");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;	j--;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 87:
				if(c == 'c'){state = 88;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 88:
				if(c == 'l'){state = 89;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 89:
				if(c == 'u'){state = 90;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 90:
				if(c == 'd'){state = 91;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 91:
				if(c == 'e'){state = 92;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 92:			
				if(c == 's'){state = 93;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 93:			// Reduce includes
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"INCLUDES");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
					if(c !='\n') j--;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 55:
				if(c == 'l'){state = 56;lexeme[i++]= c;}
				else if(c == 'n'){state = 64;lexeme[i++] = c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 56:
				if(c == 's'){state = 57;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 57:
				if(c == 'e'){state = 58;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 58:
				if(c == 'i'){state = 66;lexeme[i++]= c;}
				else if(check_char(c))
				{
					state = 0;
					lexeme[i] = '\0';
					strcpy(str,"ELSE");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 66:
				if(c == 'f'){state = 67;lexeme[i++]= c;}
				if(c == 'n'){state = 94;lexeme[i++] = c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 67:			
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"ELSEIF");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;

				case 94:
				if(c == 'c'){state = 95;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 95:
				if(c == 'l'){state = 96;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 96:
				if(c == 'u'){state = 97;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 97:
				if(c == 'd'){state = 98;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 98:
				if(c == 'e'){state = 99;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 99:
				if(c == 's'){state = 100;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 100:
				
				if(check_char(c))
				{
					state = 0;
					lexeme[i] = '\0';
					strcpy(str,"ENDINCLUDES");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 64:
				if(c == 'd')
				{state = 65;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 65:
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"END");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
				}
				else if(c == 'i'){lexeme[i++] = c;state = 66;}
				else if(c == 'L'){	lexeme[i++] = c;	state = 121;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 59:
				if(c == 'e'){	state = 60;	lexeme[i++]= c;}
				else if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"IDEN");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;	j--;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 60:
				if(c == 'g'){	state = 61;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 61:
				if(c == 'i'){state = 62;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;;
				
				case 62:
				if(c == 'n'){	state = 63;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 63:			///Reduce begin
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"BEGIN");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 68:
				if(c == 'r')
				{state = 69;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 69:
				if(c == 'i'){state = 70;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 70:
				if(c == 'n'){state = 71;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 71:
				if(c == 't'){state = 72;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 72:			///Reduce print
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"PRINT");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;	j--;	
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 73:
				if(c == 'c'){state = 74;lexeme[i++]= c;}
				
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 74:
				if(c == 'a'){state = 75;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 75:
				if(c == 'n'){state = 76;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
			
				case 76:			///Reduce SCAN
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"SCAN");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 77:
				if(c == 'o'){state = 78;lexeme[i++]= c;}
				else if(c == 'a'){state = 125;lexeme[i++] = c;}
				else if(c == 'h'){state = 199;lexeme[i++] = c;}
				else if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"IDEN");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;	j--;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 78:
				if(c == 'l'){state = 79;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 79:
				if(c == 'l'){state = 80;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 80:
				if(c == 'e'){state = 81;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 81:
				if(c == 'c'){state = 82;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 82:
				if(c == 't'){state = 83;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 83:
				if(c == 'i'){state = 84;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 84:
				if(c == 'o'){state = 85;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 85:
				if(c == 'n'){state = 86;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 86:			///Reduce Collection
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"COLLECTION");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 101:
				if(c == 'e'){state = 102;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 102:
				if(c == 't'){state = 103;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 103:
				if(c == 'u'){state = 104;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 104:
				if(c == 'r'){state = 105;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 105:
				if(c == 'n'){state = 106;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 106:			///Reduce RETURN
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"RETURN");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 107:
				if(c == 'R'){state = 108;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 108:
				if(c == 'U'){state = 109;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 109:
				if(c == 'E'){	state = 110;lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 110:			///Reduce TRUE
				if(check_char(c))
				{
					state = 0; lexeme[i] = '\0';
					strcpy(str,"TRUE");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;	j--;	
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 111:
				if(c == 'A'){state = 112;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 112:
				if(c == 'L'){state = 113;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 113:
				if(c == 'S'){state = 114;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 114:
				if(c == 'E'){state = 115;lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
					
				case 115:			///Reduce FALSE
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"FALSE");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t); i = 0;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 116:
				if(c == 'h'){state = 117;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 117:
				if(c == 'i'){state = 118;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 118:
				if(c == 'l'){state = 119;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 119:
				if(c == 'e'){state = 120;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 120:			///WHILE
				if(check_char(c))
				{
					state = 0;
					lexeme[i] = '\0';
					strcpy(str,"WHILE");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;
					j--;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 121:
				if(c == 'o'){state = 122;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 122:
				if(c == 'o'){state = 123;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 123:
				if(c == 'p'){state = 124;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 124:			///Reduce Loop
				if(check_char(c))
				{
					state = 0;
					lexeme[i] = '\0';
					if(strcmp(lexeme,"EndLoop") == 0)
					{strcpy(str,"ENDLOOP");}
					else
					{strcpy(str,"LOOP");}
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 125:
				if(c == 'l'){state = 126;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 126:
				if(c == 'l'){state = 127;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 127:			///Reduce CALL
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"CALL");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;	
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
		
				case 130:
				if(c == 's'){state = 131;lexeme[i++]= c;}
				else if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"IDEN");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;	j--;
				}
				
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 131:
				if(c == 's'){state = 132;lexeme[i++]= c;}
				else if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"AS");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 132:
				if(c == 'i'){state = 133;	lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 133:
				if(c == 'g'){state = 134;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 134:
				if(c == 'n'){state = 135;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 135:			///Reduce assign
				if(check_char(c))
				{
					state = 0;lexeme[i] = '\0';
					strcpy(str,"ASSIGN");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);i = 0;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 136:
				if(c == 'u'){	state = 137;	lexeme[i++]= c;}
				else if(c == 'a'){	state = 300;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 137:
				if(c == 'l'){	state = 138;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 138:
				if(c == 't')
				{state = 139;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 139:
				if(c == 'i'){state = 140;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 140:
				if(c == 'p'){state = 141;lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 141:
				if(c == 'l'){state = 142;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 142:
				if(c == 'e'){	state = 143;	lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 143:			///Reduce multiple
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"MULTIPLE");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;	
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 300:
				if(c == 'i'){	state = 301;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 301:
				if(c == 'n')
				{state = 302;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				
				
				
				case 302:			///Reduce main
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"MAIN");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;	
					if(check_char(c)) j--;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				
				case 144:
				if(c == 'a'){state = 145;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 145:
				if(c == 'm'){	state = 146;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 146:
				if(c == 'e'){state = 147;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 147:
				if(c == '_'){state = 148;	lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 148:
				if(c == 'E'){state = 149;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 149:
				
				if(c == 'q'){ state = 150;lexeme[i++]= c;}
				else{ state = 188;lexeme[i++] = c;}
				break;
				
				case 150:
				if(c == 'u'){state = 151;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 151:			///Reduce Name_Equ
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"NAME_EQU");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;j--;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
		
				case 154:
				if(c == 'c'){state = 155;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 155:
				if(c == 't'){state = 156;lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 156:
				if(c == '_'){state = 157;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 157:
				if(c == 'E'){state = 158;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 158:
				if(c == 'q'){	state = 159;lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 159:
				if(c == 'u'){	state = 160;	lexeme[i++]= c;}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 160:			///Reduce Struct_Equ
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"STRUCT_EQU");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;	j--;
				}
				else{state = 188;lexeme[i++] = c;}
				
				break;
				
				case 161:			///Reduce #
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"REPEAT");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){ j--;}
				break;
				
				case 162:			///Reduce ~
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"CONCAT");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){ j--;}
				break;
				
				case 163:			///Reduce $
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"LENGTH");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){ j--;}
				break;
				
				case 164:			///Reduce ?
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"LEXICOGRAPHIC");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){ j--;}
				break;
				
				case 165:			///Reduce <
				if(c == '='){	state = 166;	lexeme[i++] = c;}		
				else if(c == '>'){	state = 170;	lexeme[i++] = c;}
				else
				{
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"LT");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){ j--;}}
				break;
				
				case 166:			///Reduce <=
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"LTE");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){ j--;}
				break;
				
				case 167:			///Reduce >
				
				if(c == '='){	state = 168;	lexeme[i++] = c;}
				else
				{
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"GT");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){ j--;}}
				break;
				
				case 168:			///Reduce >=
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"GTE");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){ j--;}
				break;
				
				case 169:			///Reduce =
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"EQ");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){ j--;}
				break;
				
				case 170:			///Reduce <>
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"NEQ");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){ j--;}
				break;
				
				case 171:			///Reduce &
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"AND");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){ j--;}
				break;
				
				case 172:			///Reduce |
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"OR");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){ j--;}
				break;
				
				case 173:			///Reduce !
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"NOT");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c))	{j--;}
				break;
				
				case 174:			///Reduce +
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"PLUS");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c))	{j--;}
				break;
				
				case 175:			///Reduce -
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"MINUS");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)) j--;
				break;
				
				case 176:			///Reduce *
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"MULT");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c))	j--;
				break;
				
				case 177:			///Reduce /
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"DIV");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){	j--;}
				break;
				
				case 178:			///Reduce _
				state = 0;
				lexeme[i] = '\0';
				strcpy(str,"UNDERSCORE");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;
				if(!check_char(c)){j--;}
				break;
				
				case 179:			///Reduce @
			//printf("Hello\n");
				/*state = 0;
				lexeme[i] = '\0';
				strcpy(str,"COMM_START");
				t = generate_new_token(str,line_number,lexeme);
				print_Token(t);
				i = 0;	
				if(!check_char(c)){j--;}
				
				*/if(c ==  92 ){state = 0; i = 0;}
				break;

				case 181:			
				if ((c >= 'a' && c<='z') || (c>='A' && c<='Z'))
				{
					lexeme[i] = '\0';
					strcpy(str,"DOUBLE_QUOTE");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;
					lexeme[i++] = c;	state = 182;
				}
				break;
				
				case 182:			
				if(c == '"')
				{
					lexeme[i] = '\0';
					strcpy(str,"STR");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;	
					
					strcpy(str,"DOUBLE_QUOTE");
					lexeme[i++] = c;
					lexeme[i] = '\0';
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;
					state = 0;
				}
				else{	lexeme[i++] = c;state = 182;}
				break;
				
				case 183:
				if(!check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"CL_QUOTE");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i = 0;	j--;
				}
				break;
				
				case 195:
				if(c == 'f'){	state = 196;lexeme[i++]= c;}
				else{	state = 188; lexeme[i++] = c;}
				break;
				
				case 196:
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"ENDIF");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
				}
				else{	state = 188; lexeme[i++] = c;}
				break;
				
				case 197:		// Reduce INTR
				if(isNumeric(c)){lexeme[i++] = c;}
				else if(c == '.'){lexeme[i++] = c; state = 198;}
				else if(check_char(c)){
					state = 0; lexeme[i] = '\0';
					strcpy(str,"INTR");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i=0;
					if(c !='\n') { j--;}
				}
				else{
				lexeme[i++] = c;
				lexeme[i] = 0;
				printf("Lexical Analysis error at %d. Unrecognized Token %s\n",line_number,lexeme);
				i = 0;
				}
				break;
				
				case 198:			// Reduce DEC
				if(isNumeric(c)){lexeme[i++] = c;}
				else if(check_char(c)){
					state = 0; lexeme[i] = '\0';
					strcpy(str,"DEC");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);
					i=0;
					if(c !='\n') { j--;}
				}
				else{
				lexeme[i++] = c;
				lexeme[i] = '\0';
				printf("Lexical Analysis error at %d\n. Unrecognized Token %s",line_number,lexeme);
				i = 0;
				}
				break;
				
				case 199:
				if(c == 'a'){state = 200;	lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 200:
				if(c == 'r'){state = 201;	lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 201:
				if(c == 'a'){state = 202;	lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 202:
				if(c == 'c'){state = 203;	lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 203:
				if(c == 't'){state = 204;	lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 204:
				if(c == 'e'){state = 205;	lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 205:
				if(c == 'r'){state = 206;	lexeme[i++]= c;	}
				else{	state = 188;lexeme[i++] = c;}
				break;
				
				case 206:			// Reduce Character
				if(check_char(c))
				{
					state = 0;	lexeme[i] = '\0';
					strcpy(str,"CHARACTER");
					t = generate_new_token(str,line_number,lexeme);
					print_Token(t);	i = 0;
					if(c !='\n') j--;
				}
				else{	state = 188;lexeme[i++] = c;}
				break;
			
			}
			
		}


	}

	if(state == 179)
		{
			printf("Lexical Error. Unterminated Comments\n");
		}	

}

void lexer_helper()
{
	
		FILE* input_file;
		input_file = fopen("Test_Case.txt","r");
		if(input_file){
			output = fopen("Token_Stream","w");
			lexical_analysis(input_file);
			fprintf(output,"EOF	eof	0\n");
			
			fclose(output);	}
		else
			{printf("File is corrupted\n");}
}
