#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parserDef.h"  
#include "lexerDef.h"
#include "SymbolTableDef.h"

FILE* f;
int data_segment_count = 0;
int code_segment_count = 0;
char data_segment[100][256];
char code_segment[200][256];
char str[100];
char code[100];	
int count = 0;
int hash;

void write_data_segment()
{
	int i=0;
	for(i=0;i<data_segment_count;i++)
	{
		fprintf(f,"%s\n",data_segment[i]);
	
	}
}
void write_code_segment()
{
	int i=0;
	for(i=0;i<code_segment_count;i++)
	{
		fprintf(f,"%s\n",code_segment[i]);
	}
}
void generate(struct node* node)
{
	int i;	
	
	if(strcmp(node->node_name,"STR") == 0)
	{
		if(strcmp(node->parent->node_name,"PRINT") == 0)
	        {
			
			data_segment_count++;
			code_segment_count++;
			sprintf(str,"str%d db '%s',0ah,0dh,'$'",data_segment_count,node->node_value);
			sprintf(code,"mov ah,09h \n lea dx,str%d \n int 21h\n jmp x2 \n",data_segment_count); 
			strcpy(data_segment[data_segment_count-1],str);
			strcpy(code_segment[code_segment_count-1],code);
		}
	}
	else if(strcmp(node->node_name,"PRINT") == 0)
	{
		if(strcmp(node->children[0]->node_name,"STR") == 0)
			generate(node->children[0]);
		else
		{
			int hash;
			hash = hash_func(node->children[0]->node_value);
			if(strcmp(TABLE[hash]->var->type,"BOOLEAN"))
			{
				// Printing of strings remaining
				code_segment_count++;
				sprintf(code,"lea dx,%s\nmov ah,09h\nint 21h",node->children[0]->node_value);
				strcpy(code_segment[code_segment_count-1],code);

				code_segment_count++;
				sprintf(code,"lea dx,newline\nmov ah,09h\nint 21h");
				strcpy(code_segment[code_segment_count-1],code);
			}
		} 
	}
	
	else if(strcmp(node->node_name,"INTEGER") == 0 || strcmp(node->node_name,"BOOLEAN") == 0)
	{

		if(strcmp(node->children[0]->node_name,"INTR") == 0)
		{
			data_segment_count++;
			sprintf(str,"%s db %s dup(0)",node->children[1]->node_value,node->children[0]->node_value);
			strcpy(data_segment[data_segment_count-1],str);
			
		}
		else
		for(i=0;i<node->n_children;i++)
		{
			data_segment_count++;
			sprintf(str,"%s db ?",node->children[i]->node_value);
			strcpy(data_segment[data_segment_count-1],str);
			
		}
	}
	else if(strcmp(node->node_name,"STRING") == 0)
	{
		for(i=0;i<node->n_children;i++)
		{
			data_segment_count++;
			sprintf(str,"%s db 250 dup('$') ",node->children[i]->node_value);
			strcpy(data_segment[data_segment_count-1],str);
		}
	}
	else if(strcmp(node->node_name,"ASSIGN") == 0)
	{
		if(strcmp(node->children[0]->node_name,"MULTIPLE"))
		{	
			if(strcmp(node->children[1]->node_name,"TRUE") == 0)
			{
				code_segment_count++;
				sprintf(code,"lea si,%s \n mov cl,1 \n mov [si],cl \n",node->children[0]->node_value);
				strcpy(code_segment[code_segment_count-1],code);
			}
			else if(strcmp(node->children[1]->node_name,"FALSE") == 0)
			{
				code_segment_count++;
				sprintf(code,"lea si,%s \n mov cl,0 \n mov [si],cl \n",node->children[0]->node_name);
				strcpy(code_segment[code_segment_count-1],code);
			}
			else if(strcmp(node->children[1]->node_name,"STR") == 0)
			{
				data_segment_count++;
				sprintf(str,"temp db '%s'",node->children[1]->node_value);
				strcpy(data_segment[data_segment_count-1],str);
				code_segment_count++;
				int a = strlen(node->children[1]->node_value);
				count++;
				sprintf(code,"mov cx,%d \n lea si,temp \n lea di,%s \n label%d: mov al,[si] \n mov [di],al \n inc si \n inc di \n dec cx \n jnz label%d \n",a,node->children[0]->node_value,count,count);
				strcpy(code_segment[code_segment_count-1],code);
			}
			else if(strcmp(node->children[1]->node_name,"AS") == 0)
			{
				struct node* temp;
				temp = node->children[1];

				if(strcmp(temp->children[0]->node_name,"REPEAT") == 0)
				{
					temp = temp->children[0];
					code_segment_count++;
					count++;
					sprintf(code,"mov cx,%s \nlea si,%s \nlabel%d: \n",temp->children[1]->node_value,node->children[0]->node_value,count);
					strcpy(code_segment[code_segment_count-1],code);
					count++;
					
					sprintf(code,"mov bx,%s \nlea di,%s \nlabel%d: mov al,[di] \nmov[si],al \ninc si\ninc di\n dec bx\njnz label%d\n dec cx\n jnz label%d\n",temp->children[1]->node_value,temp->children[0]->node_value,count,count,count-1);
					code_segment_count++;
					strcpy(code_segment[code_segment_count-1],code);
				}
				else if(strcmp(temp->children[0]->node_name,"LEXICOGRAPHIC") == 0)
				{
					temp = temp->children[0];
					code_segment_count++;
					count++;
					sprintf(code,"lea si,%s \nlea di,%s \ndec si\ndec di\nlabel%d: inc si\ninc di\nmov al,[si]\nmov [di],al\njz label%d\njnz label%d\n",temp->children[1]->node_value,temp->children[0]->node_value,count,count,count+1);
					strcpy(code_segment[code_segment_count-1],code);

					count++;
					code_segment_count++;
					sprintf(code,"lea si,%s\nmov al,1\nmov [si],al\njmp label%d\n",node->children[0]->node_value,count+1);
					strcpy(code_segment[code_segment_count-1],code);

					code_segment_count++;
					sprintf(code,"lea dx,TRUE\nmov ah,09h\nint 21h\nlea dx,newline\nmov ah,09h\nint 21h");
					strcpy(code_segment[code_segment_count-1],code);
			
					code_segment_count++;
					sprintf(code,"label%d: \nlea si,%s\nmov al,0\n mov[si],al\n",count,node->children[0]->node_value);
					strcpy(code_segment[code_segment_count-1],code);

					code_segment_count++;
					sprintf(code,"lea dx,FALSE\nmov ah,09h\nint 21h\nlea dx,newline\nmov ah,09h\nint 21h\n\n label%d: \n",count+1);
					strcpy(code_segment[code_segment_count-1],code);
			
				}
				else		// For array slicing
				{
					code_segment_count++;
					sprintf(code,"lea di,%s",node->children[0]->node_value);
					strcpy(code_segment[code_segment_count-1],code);

					struct node* temp;
					temp = node->children[0]->children[0];
					int diff = atoi(temp->children[0]->node_value) - atoi(temp->children[1]->node_value);
					if(diff < 0)
						diff*= (-1);				
					diff+= 1;
					code_segment_count++;
					sprintf(code,"mov cx,%d",diff);
					strcpy(code_segment[code_segment_count-1],code);

					temp = node->children[1];
					code_segment_count++;
					sprintf(code,"lea si,%s\nadd si,%d\n",temp->children[0]->node_value,(diff-1));
					strcpy(code_segment[code_segment_count-1],code);


					code_segment_count++;
					sprintf(code,"x%d:\nmov al,[si]\nmov [di],al\ninc di\ndec si\ndec cx\njnz x%d\n",code_segment_count,code_segment_count);
					strcpy(code_segment[code_segment_count-1],code);						
				}
				
			}
			else
			 if(strcmp(node->children[1]->node_name,"INTR") == 0)			
			{
				code_segment_count++;
				sprintf(code,"lea si,%s \n mov cl,%s \n mov [si],cl \n",node->children[0]->node_value,node->children[1]->node_value);
				strcpy(code_segment[code_segment_count-1],code);
			}
				
			

		}
		else
		{
			sprintf(code,";Multiple Assignment blocks start here \n");
			code_segment_count++;
			strcpy(code_segment[code_segment_count-1],code);
			struct node* temp1;
			struct node* temp2;
			struct node* temp3;
			temp1 = node->children[0];
			temp2 = node->children[1];
			int check = 1;

			while(temp1)
			{
				code_segment_count++;
				if(temp1->n_children == 1)
				{					
					for(i=0;i<temp2->n_children;i++)
					{
						temp1 = temp1->children[0];
						sprintf(code,"lea si,%s \n mov cl,%s \n mov [si],cl \n",temp1->node_value,temp2->children[i]->node_value);
						strcpy(code_segment[code_segment_count-1],code);
						code_segment_count++;		
					}
					if(check)
					{
						temp2 = temp2->children[1];
						check = 0;
					}
					else
						temp2 = temp2->children[0];
						
				}
				else
				{
					for(i=0;i<temp1->n_children;i++)
					{
						sprintf(code,"lea si,%s \n mov cl,%s \n mov [si],cl \n",temp1->children[i]->node_value,temp2->node_value);
						strcpy(code_segment[code_segment_count-1],code);
						temp2 = temp2->children[0];
					}
					temp1 = temp1->children[0];
				}
			}
			
			sprintf(code,";Multiple Assignment blocks ends here \n");
			code_segment_count++;
			strcpy(code_segment[code_segment_count-1],code);

			
		} 
	}
	else if(strcmp(node->node_name,"IF") == 0)
	{
		int a;
		struct node* temp1;
		struct node* temp2;
		temp1 = node->children[0];

		if(strcmp(temp1->node_name,"EQ") == 0)
		{
			temp2 = temp1->children[1];
			temp1 = temp1->children[0];	
			if(strcmp(temp2->node_name,"TRUE") == 0)
			{
				
				code_segment_count++;
				count++;
				sprintf(code,"lea si,%s \n mov al,[si]\n cmp al,1 \n jz x%d \n jnz x%d\n",temp1->node_value,count,count+1);
				strcpy(code_segment[code_segment_count-1],code);
			}
		}

		temp1 = node->children[1];
		sprintf(code,"x%d: \n",count);
		count++;
		a = count;
		code_segment_count++;
		strcpy(code_segment[code_segment_count-1],code);
		generate(temp1);

		
		temp1 = node->children[2];
		if(temp1 != NULL)
		{
		
		sprintf(code,"x%d: \n",count);
		code_segment_count++;
		strcpy(code_segment[code_segment_count-1],code);
		
			generate(temp1);
		
		}
		else
		{
			sprintf(code,"x%d: \n",a);	
			code_segment_count++;
			strcpy(code_segment[code_segment_count-1],code);
		}
		
		
	}
	else if(strcmp(node->node_name,"WHILE") == 0)
	{
		struct node* temp;
		struct node* temp1;
		struct node* temp2;
		temp = node->children[0];

		if(strcmp(temp->node_name,"LT") == 0)
		{
			temp1 = temp->children[0];
			temp2 = temp->children[1];
			
			code_segment_count++;
			sprintf(code,"lea si,%s\nmov cx,%s",temp1->node_value,temp2->node_value);
			strcpy(code_segment[code_segment_count-1],code);
		}

		temp = node->children[1];

		if(strcmp(temp->node_name,"<smts_&_func_defs>") == 0)
		{
			int a = code_segment_count;
			if(strcmp(temp->children[0]->node_name,"ASSIGN") == 0)
			{
				if(temp->children[0]->children[0]->n_children > 0)
				{
					code_segment_count++;
					sprintf(code,"lea di,%s \n\n x%d: ",temp->children[0]->children[0]->node_value,a);
					strcpy(code_segment[code_segment_count-1],code);

					code_segment_count++;
					sprintf(code,"mov al,[si] \nmov [di],al \ninc %s\ninc di\ndec cx\njnz x%d\n",temp->children[0]->children[1]->node_value,a);
					strcpy(code_segment[code_segment_count-1],code);
				}
			}
			if(strcmp(temp->children[0]->node_name,"PRINT") == 0)
			{
				data_segment_count++;
				sprintf(str,"dat1 db 10 dup ('$')");
				strcpy(data_segment[data_segment_count-1],str);

				code_segment_count++;					
				sprintf(code,"lea si,%s\nlea di,dat1\n",temp->children[0]->children[0]->node_value);
				strcpy(code_segment[code_segment_count-1],code);

				code_segment_count++;
				sprintf(code,"x%d: mov al,[si]\nadd al,30h\nmov dat1,al\nlea dx,dat1\nmov ah,09h\nint 21h\nlea dx,newline\nmov ah,09h\nint 21h\ninc si\ndec cx\njnz x%d",code_segment_count,code_segment_count);
				strcpy(code_segment[code_segment_count-1],code);
			}
		}


	}
	else if(node ==  NULL)
	{
		return;
	}
	else
	for(i=0;i<node->n_children;i++)
	{
		generate(node->children[i]);
	}
}

void generate_code()
{
	
	f = fopen("Code.asm","w");
	fprintf(f,".model tiny\n");
	fprintf(f,".data\n\n");
	fprintf(f,"newline db 0dh,0ah,'$'\n");
	fprintf(f,"TRUE db 'TRUE$'\n");
	fprintf(f,"FALSE db 'FALSE$'\n");
	generate(parse_tree);

	write_data_segment();
	
	fprintf(f,"\n.code\n");
	fprintf(f,".startup\n\n");

	write_code_segment();

	fprintf(f,"\n.exit\n");
	fprintf(f,"end\n");
}

void CodeGeneration_helper()
{
	generate_code();
	printf("***********************************************\n");
	printf("ASM CODE GENERATED SUCCESSFULLY....!!!! \n");
}
