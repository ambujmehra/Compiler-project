#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parserDef.h"  
#include "lexerDef.h"

int parse_table[NUM_NON_TERM][NUM_TERM];
char word[15];
char word_line[5];
int marker = 0;
char *focus;
struct StackNode* root = NULL;
int focus_no,word_no,rule_number;
char dummy[256];
char res[20][256];
struct grammar rules[NUM_TOTAL_RULES];
struct nonterminals nonter[NUM_NON_TERM];
struct terminals ter[NUM_TERM];
struct tokens token[MAX]; 

 int find_terminal_exixts(char *to_find)
{
  int i;
  for(i=0;i<NUM_TERM;i++)
  {
    if(strcmp(ter[i].t_name,to_find) ==0)
      return 0;
  }
  return 1;
}

 int find_nt_no(char *focus)
{
  int i; int flag = -1;
  for(i=0;i<NUM_NON_TERM;i++){  
    if(strcmp(nonter[i].nt_name,focus)==0)
    {
          flag = 1;
          return nonter[i].nt_no;
         
      }

  }
  if(flag ==-1)
    return -1;

}

int find_t_no(char* word){
  int i; int flag = -1;
  for(i=0;i<NUM_TERM;i++){  
    if(strcmp(ter[i].t_name,word)==0)
    { 
          flag = 1;
          return ter[i].t_no;
         
      }

  }
  if(flag ==-1)
    return -1;
}

struct StackNode* newNode(char data[])
{ 
  struct StackNode* new;
	new = (struct StackNode*) malloc(sizeof(struct StackNode));
	if(new == NULL)
	{
		printf("ERROR\n");
	}
    strcpy(new->data,data);
    new->next = NULL;
    return new;
}
 
int isEmpty(struct StackNode *root)
 { 
    return !root;
 }
 
struct StackNode* push(struct StackNode* root, char *data)
{
    struct StackNode* stackNode = newNode(data);
    stackNode->next = root;
    root = stackNode;
	 // printf("%s pushed to stack\n", data);
	  return root;
}
 
struct StackNode *pop(struct StackNode* root)
{
    if (isEmpty(root))
        return (char)0;
    struct StackNode* temp = root;
    root = root->next;
    char *popped = temp->data;
	  //printf("%s popped from stack\n", popped);
	return root;
}
 
char *peek(struct StackNode* root)
{
    if (isEmpty(root))
        return (char)0;
    return root->data;
}

 char *choppy( char *s )
{
    char *n = malloc( strlen( s ? s : "\n" ) );
    if( s )
        strcpy( n, s );
    n[strlen(n)-1]='\0';
    return n;
}

char *choppy_one( char *s )
{
    char *n = malloc( strlen( s ? s : "\0" ) );
    if( s )
        strcpy( n, s );
    n[strlen(n)-1]='\0';
    return n;
}

struct node* alloc_node(char *payload)
{
   struct node *new_node;
   int i;
   new_node = malloc(sizeof (struct node));
   if (new_node == NULL) 
	 return NULL;
   strcpy(new_node->node_name,payload);
   for (i = 0; i < 20; ++i)
     {
       new_node->children[i] = NULL;
     }
   return new_node; 
 }

void parse(struct node* current)
{		
      if ((find_terminal_exixts(focus) ==0) || (strcmp(focus,"EOF") == 0))
        {
			    if(strcmp(current->node_name,"ENDPROG") == 0)
			    { 
				    root = pop(root);
				    focus = peek(root);
						if( (strcmp(focus,"EOF") == 0))
				       {
					       printf("PARSING DONE SUCCESSFULLY!!!\n");
					       return;
				       }
			    }
			    else if (strcmp(focus,word) == 0)
            {  
               strcpy(current->node_lineno,word_line); 
               root = pop(root);
               focus = peek(root);
               marker ++;
               strncpy(word,token[marker].token_name, sizeof(token[marker].token_name));
               current->n_children = 0;
               strcpy(current->node_value, token[marker-1].token_value);
               strncpy(word_line,token[marker-1].line_no, sizeof(token[marker].line_no));
               //current->n_children = 0;
            }
          else
            { 
             printf("PARSER ERROR AT LINE %s : LOOKING FOR %s TOKEN VALUE AT TOP OF STACK\n", token[marker].line_no,token[marker].token_value);
             return;
             }
        }
      else 
        {
          focus_no = find_nt_no(focus);
          if(focus_no == -1)
          printf("NON TERMINAL %s NOT VALID\n",focus );
          word_no = find_t_no(word);
          if(word_no == -1)
          printf("TERMINAL ILLEGAL: %s\n", word);
          rule_number = parse_table[focus_no][word_no];
          if(rule_number != -1)
            {
			       char *rule = rules[rule_number].rule_rhs;
             strcpy(dummy, rule );
             char *  p    = strtok (dummy, " ");
             int n_spaces = 0;
             int i6;
            while (p){
			                n_spaces++;
                      //if (res == NULL)
                      //return;
			                strcpy(res[n_spaces-1],p);
                      p = strtok(NULL, " ");
                    }

            strcpy(res[n_spaces -1],choppy(res[n_spaces-1]));  
			      root = pop(root);
		        int j; int a =0;
			      for(j = 0; j<n_spaces;j++)
              { 
               if(strcmp(res[j],"EPSILON") != 0)
                {  root = push(root,res[n_spaces-j-1]);/**/
                   current->children[j] = alloc_node(res[j]);
					      }
                else 
                {
                  a++;
                }
				      }
			      focus = peek(root);	
            current ->n_children = n_spaces -a;
            for(j = 0;j<n_spaces;j++)
            {
					   if(current->children[j])
					   {
							parse(current->children[j]);
              current ->children[j]-> parent = current;
					  } 
            }     
          }
          else
            { 
					    printf("%s\n",current->node_name);
					    printf("%s\n",peek(root));
					    printf("PARSING ERROR: PARSE TABLE GIVES -1\n");
              return; }
             }


}

void print_tree(struct node* node)
{

	if(node == NULL)
		return;
	else
	{
    
		printf("%s %d\n",node->node_name,node->n_children);
		int i;
		for(i=0;i<node->n_children;i++)
		{
    //  if(strcmp(node->children[i]->node_name,"IDEN"))
			print_tree(node->children[i]);
		}
	}
	
}

void load_data()
{
	FILE *fr,*ntfp,*tfp,*tf;
  int i = 1;
  int i1=0; int i2=0;  int i4 =0; int i5=0;
	char line[105],line1[35],line2[20],line3[100];
	int x,y;
  const char s[2] = "-";
  //READ GRAMMAR
	fr = fopen("grammar.txt","r");
	while(fgets(line,105,fr) != NULL)
	{   char *temp;
      char *temp1;
      char *s_name;
   		rules[i].rule_no = i;
      int size2 = strlen(line) -1;
		  strncpy(rules[i].rule, line,size2);
      //
      rules[i].rule_lhs = strtok(rules[i].rule, s);
      temp = strtok(NULL,"\n");
      temp1 = strtok(temp,"\t,\b");
      rules[i].rule_rhs = strtok(NULL,"\n");
      //
		  i++;
	}
	fclose(fr);
	//READ NON TERMINALS
  ntfp = fopen("nonterminals.txt","r");
	while(fgets(line1,35,ntfp) != NULL)
	{
		nonter[i1].nt_no = i1;
    int size3 = strlen(line1)-1;
		strncpy(nonter[i1].nt_name, line1,size3);
    //
		i1++;
	}
    fclose(ntfp);
  //READ TERMINALS
  tfp = fopen("terminals","r");
	while(fgets(line2,20,tfp) != NULL)
	{
		ter[i2].t_no = i2;
		int size1 = strlen(line2) -1;
		strncpy(ter[i2].t_name, line2, size1);
    //
		i2++;
	}
    fclose(tfp);
  //READ TOKENS  
  tf= fopen("Token_Stream","r");
  while(fgets(line3,100,tf) != NULL)
  {  
    int size4 = strlen(line3) -1;
    strncpy(token[i4].token_full,line3,size4);
    // 
    i4++;
  }
  fclose(tf);
  for(i5 =0;i5<i4;i5++)
  {  
    char *temp,*temp1,*temp2, *temp3;
    temp = strtok(token[i5].token_full,"\t");
    strncpy(token[i5].token_name,temp,strlen(temp));
    //
    temp1 = strtok(NULL,"\t");
    strncpy(token[i5].token_value,temp1,strlen(temp1)); 
    //
    temp2= strtok(NULL,"\t");
    strncpy(token[i5].line_no,temp2,strlen(temp2));
    //
  }
 strncpy(word,token[marker].token_name, sizeof(token[marker].token_name)); 
 strncpy(word_line,token[marker].line_no, sizeof(token[marker].line_no));
}
int read_parse_table(){
  FILE *pt;
  pt = fopen("parse_table", "r");
  int i,j;
  if (pt == NULL)
    {
        printf("Error Reading File parse_table\n");
        return 0;
    }
    for (i = 0; i < NUM_NON_TERM; i++)
    {
       for(j=0;j<NUM_TERM;j++)
       { fscanf(pt, "%d,", &parse_table[i][j] );
       }
    }     
  fclose(pt);
  return 0;
}


void parser_helper(){
      
    read_parse_table();
    load_data();
    root = push(root, "EOF");
    root = push(root, "<Program>");
    focus = peek(root);
    parse_tree_root = alloc_node("<Program>");
    parse_tree = parse_tree_root;
    parse(parse_tree_root);
 
    
	print_tree(parse_tree);
  }

