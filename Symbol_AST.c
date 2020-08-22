#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parserDef.h"  
#include "lexerDef.h"
#define NUM_TOTAL_RULES 130
#define NUM_NON_TERM 59
#define NUM_TERM 68
#define MAX 500
int parse_table[NUM_NON_TERM][NUM_TERM];
char word[15];
char word_line[5];
int marker = 0;
char *focus;
struct StackNode* root = NULL;
int focus_no,word_no,rule_number;
char dummy[256];
char res[20][256];

struct node* parse_tree_root;
struct node* parse_tree;
struct node* ast;
struct grammar rules[NUM_TOTAL_RULES];
struct nonterminals nonter[NUM_NON_TERM];
struct terminals ter[NUM_TERM];
struct tokens token[MAX];  

char* ast_terminals[44]=
{
  "PROG",
  "GLOBAL",
  "SUBPROG",
  "INTEGER",
  "STRING", 
  "DECIMAL",
  "CHARACTER",
  "BOOLEAN",
  "ASSIGN",
  "MULTIPLE",
  "UNDERSCORE",
  "IF",
  "ELSEIF",
  "ELSE",
  "WHILE",
  "NAME_EQU",
  "STRUCT_EQU",
  "REPEAT",
  "LEXICOGRAPHIC",
  "LT",
  "LTE",
  "GT",
  "GTE",
  "NEQ",
  "EQ",
  "PLUS",
  "MINUS",
  "MULT",
  "DIV",
  "AND",
  "OR",
  "NOT",
  "SCAN",
  "PRINT",
  "RETURN",
  "COLLECTION",
  "STR",
  "INTR",
  "DEC",
  "TRUE",
  "FALSE",
  "CHAR",
  "IDEN",
  "MAIN"
};


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

void load_data(char* argv)
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
  tf= fopen(argv,"r");
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


void merge_node(struct node* current)
{

  
  int i,j;
  if(current == NULL)
  {
    return;
  }
  else
  {
   // printf("%s %s %d %s\n",current->node_name,current->node_value,current->n_children,current->node_lineno);
    /*if(strcmp(current->node_name,"SEMICOLON") == 0)
    {
      printf("%d\n",current->n_children);
      printf("%s\n",current->children[2]->node_name);
    } 
    */    
    if(current->n_children == 1)
    {
      if(strcmp(current->node_name,"<return_list>") == 0 || strcmp(current->node_name,"<remaining_list>") == 0)
      {
        return;
      }
      struct node* temp1;
      struct node* temp2;
      if(current->parent != NULL)
      {
        int n = current->parent->n_children;
        temp1 = current->parent;
        temp2 = current;
        for(i=0;i<n;i++)
        {
          if(current->parent->children[i] == current)
          {
          //printf("Single degree node found at %s\n",current->node_name);

            current->parent->children[i] = current->children[0];
            (current->children[0])->parent = current->parent;
          //printf("Hello Link created between %s %s\n",temp1->node_name,temp1->children[i]); 
          //printf("\n");
          //printf("\n");
          
            merge_node(temp1->children[i]);
          //  free(current);
          }
        }
      }
    }
    else if(current->n_children == 0)
    {
      int flag = 0;
      for(i=0;i<44;i++)
      {
        if(strcmp(current->node_name,ast_terminals[i]) == 0)
        {
          flag = 1;
          break;
        }
      }
      
      if(!flag)
      {
        int n = current->parent->n_children;
        struct node* temp = current->parent;
        for(i=0;i<n;i++)
        {
          if(temp->children[i] == current)
          {
              //printf("Hello  %s\n",temp->node_name);
              for(j=i;j<n-1;j++)
              {
                temp->children[j] = temp->children[j+1];
              }
              temp->children[n-1] = NULL;
              temp->n_children--;
              break;
          }
        }
        merge_node(temp);
        free(current);
        
      }
      return;
    }
    else
    {
      
      for(i=0;i<current->n_children;i++)
      {
        if(current->children[i])
          merge_node(current->children[i]);
      }
    }
    
  }
}

void create_ast(struct node* current)
{
  int i,j;
  if(current->n_children == 0)
  {
    return;
  }
  else
  {
  // printf("%s\n",current->node_name);
  }
  if(current ==  parse_tree)
  {
      struct node* temp;
      temp = current->children[0];
      temp->n_children = current->n_children-1;
      for(i=0;i<temp->n_children;i++)
      {
          temp->children[i] = current->children[i+1];
          current->children[i+1]->parent = temp;
          create_ast(temp->children[i]);
      }
      free(current);
      parse_tree = temp;
  }
  else
  {
  /* if(strcmp(current->node_name,"<func_defs>") == 0 )
    {

      
      struct node* temp;
      temp = current->children[0];
      if(temp->n_children == 0)
      {
          current->parent->children[0] = temp;
          temp->parent = current->parent;
          temp->n_children = current->n_children-1;
         for(i=0;i<temp->n_children;i++)
         {
            
            temp->children[i] = current->children[i+1];
            current->children[i+1]->parent = temp;
            create_ast(temp->children[i]);
          }
        //free(current);
        }
      }
    
  */
    
      //else
      //{
      /*  printf("WTF\n");
        struct node* temp1;
        temp1 = current->children[1]->children[0];
        printf("%s\n",temp1->node_name);
        temp1->parent = current->parent;
        current->parent->children[0] = temp1;
        temp1->n_children = current->n_children;
        current->children[1]->n_children --;
        current->children[1]->children[0] = current->children[1]->children[1];
        current->children[1]->children[1] = NULL;
        
        for(i=0;i<temp1->n_children;i++)
        {
          printf("%s\n",temp1->node_name);
          printf("%s\n",current->children[i]->node_name);
          temp1->children[i] = current->children[i];
          printf("%s\n",temp1->children[i]->node_name);
          current->children[i]->parent = temp1;
          create_ast(temp1->children[i]);
        }
        free(current);
        
      
      }
        
    }


    
      

  /*  if(strcmp(current->node_name,"ELSE") == 0)
    {

      printf("djhcjskdc %d\n",current->n_children);
    }
  */
    if(strcmp(current->node_name,"<IO_statements>") == 0 || strcmp(current->node_name,"<Else>") == 0 || strcmp(current->node_name,"<ExpB>") == 0 || strcmp(current->node_name,"<Return_statement>") == 0 || strcmp(current->node_name,"<func_defs>") == 0 || strcmp(current->node_name,"<func_body>") == 0 || strcmp(current->node_name,"<Collection_declaration>") == 0 || strcmp(current->node_name,"<Declaration>") == 0 || strcmp(current->node_name,"<Lvalue>") == 0 || strcmp(current->node_name,"<B>") == 0 || strcmp(current->node_name,"<var>") == 0 || strcmp(current->node_name,"<Iterative>") == 0 || strcmp(current->node_name,"<Assignment>") == 0)
      {

        if(strcmp(current->node_name,"<func_defs>") == 0)
        {
        //  printf(" Entered\n");
        }
    
        struct node* temp1;
        struct node* temp2;
        temp1 = current->parent;
        temp2 = current->children[0];


        
        if(temp2->n_children == 0)
      {

         
      //  printf("%s\n",current->node_name);
        for(i=0;i<temp1->n_children;i++)
        {
          if(temp1->children[i] == current)
          {

           if(strcmp(current->node_name,"<func_body>") == 0)
          {
             // printf("%s\n",current->parent->node_name);
            //  printf("Body Entered %d\n",i);
            //  printf("%s\n",temp2->node_name);
          }

            current->parent->children[i] = temp2;
            temp2->n_children = current->n_children-1;
            
            for(j = 0;j<temp2->n_children;j++)
            {
              temp2->children[j] = current->children[j+1];
              current->children[j+1]->parent = temp2;
              create_ast(temp2->children[j]);
            }
            
              break;
            }
          }
          create_ast(current->parent);
          //free(current);
              
      }
      else
      {

        for(i=0;i<temp1->n_children;i++)
        {
          if(temp1->children[i] == current)
          {          
            
            current->parent->children[i] = temp2;
            int a = current->n_children-1+temp2->n_children;
            
            for(j=temp2->n_children;j<a;j++)
            {
              temp2->children[j] = current->children[j-temp2->n_children+i+1];
              current->children[j-temp2->n_children+i+1]->parent = temp2;

            }

            temp2->n_children = a;
          //  printf("%s\n",temp2->node_name);
           // printf("%s\n",temp2->children[0]->node_name);
            create_ast(temp2);

            for(j=0;j<a;j++)
            {
            //    printf("%s\n",temp2->children[j]->node_name);     
                create_ast(temp2->children[j]);
            }
        //    printf("jhsdcbhjsdbcshdj   %s\n",temp2->node_name);
         //   printf("%s\n",temp2->node_name);
            
        //    free(current);
              break;
            }
          }
         // create_ast(current->parent);
      
      }

    }
   /* if(strcmp(current->node_name,"<func_sig>") == 0)
    {
      struct node* temp1;
      struct node* temp2;
      temp1 = current->children[0];
      temp2 = current->parent;
      

      for(i=0;i<temp2->n_children;i++)
      {
        if(temp2->children[i] == current)
        {
          printf("%d\n",i);
          printf("%s\n",temp1->node_name);
          temp2->children[i] = temp1;
          current->children[i]->parent == temp2;
          printf("%s\n",temp2->children[i]->node_name);
          printf("%d\n",current->n_children);
          temp1->n_children = current->n_children - 1;
          printf("%s\n",current->children[0]->node_name);
          printf("%s\n",current->children[1]->node_name);
          printf("%s\n",current->children[2]->node_name);
          printf("%s\n",current->children[3]->node_name);
          printf("%d\n",temp1->n_children);
          for(j=0;j<temp1->n_children;j++)
          {
            temp1->children[j] = current->children[j+1];
            current->children[j+1]->parent = temp1;
            printf("%s\n",temp1->children[j]->node_name);
            create_ast(temp1->children[j]);
          }
          break;
        }
      }
     //   free(current);
      }
    */
    
  
    if(strcmp(current->node_name,"<relep>") == 0)
    {
      struct node* temp;
      temp = current->children[1];
      current->parent->children[0] = temp;
      temp->parent = current->parent;
      temp->n_children = current->n_children-1;
      
      temp->children[0] = current->children[0];
      temp->children[1] = current->children[2];
      
      create_ast(temp->children[0]);
      create_ast(temp->children[1]);
      free(current);
    }
    if(strcmp(current->node_name,"<Expression>") == 0)
    {
      for(i=0;i<current->n_children;i++)
      {
        create_ast(current->children[i]);
      }
    }
    if(strcmp(current->node_name,"<Conditional>") == 0)
    {
      struct node* temp;
      temp = current->children[0];
      //printf("%s\n",temp->node_name);
    //  printf("%d\n",current->n_children);
      //printf("%s\n",current->children[0]->node_name);
      //printf("%s\n",current->children[1]->node_name);
      //printf("%s\n",current->children[2]->node_name);
      
      temp->parent = current->parent;
      current->parent->children[current->parent->n_children-1] = temp;
      temp->n_children = current->n_children - 1;
      for(i=0;i<temp->n_children;i++)
      {
        temp->children[i] = current->children[i+1];
        current->children[i+1]->parent = temp;
        create_ast(temp->children[i]);
      }
      
      free(current);
      
    }
    if(strcmp(current->node_name,"AS") == 0)
    {
        struct node* temp1;
        if(strcmp(current->children[1]->node_name,"LEXICOGRAPHIC") == 0 || strcmp(current->children[1]->node_name,"REPEAT") == 0)
        {
          
          temp1 = current->children[0];
          current->children[1]->children[1] = current->children[1]->children[0];
          current->children[1]->children[0] = temp1;
          temp1->parent = current->children[1];
          current->children[1]->n_children++;
          current->n_children--;
          current->children[0] = current->children[1];
          current->children[1] = NULL;
          create_ast(current->children[0]);
          
        }
      /*  if(strcmp(current->children[0]->node_name,"NAME_EQU") == 0)
        {
          temp1 = current->children[0];
          current->children[0]->
        }*/


      
    } 
    if(strcmp(current->node_name,"<Mul_Left_Rec>") == 0|| strcmp(current->node_name,"<Expression>") == 0)
    {
      strcpy(current->node_name,"AS");
      for(i=0;i<current->n_children;i++)
      {
          create_ast(current->children[i]);
      }
      //create_ast(current);
    }
    if( strcmp(current->node_name,"<Global_Block>") == 0   || strcmp(current->node_name,"<type>") == 0)
    {


      struct node* temp;
      temp = current->children[0];

        /*   if(strcmp(current->node_name,"<func_sig>") == 0)
          {
           printf("Hello ");
           printf("%s\n",temp->node_name);
            for(i=0;i<current->n_children;i++)
          {
          
          }
        }
      */

      current->parent->children[0] = temp;
      temp->parent = current->parent;
      temp->n_children = current->n_children-1;

      for(i=0;i<temp->n_children;i++)
      {
          temp->children[i] = current->children[i+1];
          current->children[i+1]->parent = temp;
          create_ast(temp->children[i]);
      }
      //printf("%d\n",temp->n_children);
     // free(current);
    }
    if(strcmp(current->node_name,"<smts_&_func_defs>") == 0 || strcmp(current->node_name,"<more_func_defs>") == 0)
    { 
    
      /*
      struct node* temp1;
      temp1 = current->parent;
      printf("%d\n",temp1->n_children);
      temp1->n_children++;
      */
      
      
      /*temp1->children[temp1->n_children-2] = current->children[0];
      temp1->children[temp1->n_children-1] = current->children[1];
      
      create_ast(temp1->children[temp1->n_children-2]);
      create_ast(temp1->children[temp1->n_children-1]);
      
      current->children[0]->parent = temp1;
      current->children[1]->parent = temp1;
      
      */
      for(i=0;i<current->n_children;i++)
      {
          create_ast(current->children[i]);
      }
      
    //  create_ast(temp1);
      
      

      
      //free(current);
    }
    
    
    if(strcmp(current->node_name,"INTEGER") == 0 || strcmp(current->node_name,"BOOLEAN") == 0 || strcmp(current->node_name,"MULTIPLE") == 0)
    { 
      for(i=0;i<current->n_children;i++)
      {
          //printf("****  %s\n",current->children[i]->node_name);
          create_ast(current->children[i]);
      }
    }
    
    /*if(strcmp(current->node_name,"<Declaration>") == 0)
    {
      
      struct node* temp;
      temp = current->children[0];
      //printf("Hello   %s\n",temp->node_name);
      
      current->parent->children[0] = temp;
      temp->parent = current->parent;
      temp->n_children = current->n_children-1;
      for(i=0;i<temp->n_children;i++)
      {
          temp->children[i] = current->children[i+1];
          current->children[i+1]->parent = temp;
        //  printf("%s\n",temp->children[i]->node_name);
          create_ast(temp->children[i]);
      }
      //printf("%s\n",temp->node_name);
      create_ast(temp);
      free(current);
    }
    */
    if(strcmp(current->node_name,"<variables>") == 0)
    {
     // printf("Hello Entered\n");
   //  printf("%d\n",current->n_children);
      if(strcmp(current->node_name,"MULTIPLE") == 0)
      {
        //printf("Hello\n");
      }
      struct node* temp;
      temp = current->children[0];
      current->parent->children[0] = temp;
      temp->parent = current->parent;
      temp->n_children = current->n_children-1;
      for(i=0;i<temp->n_children;i++)
      {
          temp->children[i] = current->children[i+1];
          current->children[i+1]->parent = temp;
          create_ast(temp->children[i]);
      }
   //   create_ast(temp);
      free(current);
    }
    if(strcmp(current->node_name,"<integerLiteralMore>") == 0)
    {
    //printf("%d\n",current->n_children);
    
      struct node* temp;
      temp = current->children[0];
      if(strcmp(current->parent->node_name,"AS") == 0)
        current->parent->children[1] = temp;
      else
        current->parent->children[0] = temp;
      temp->parent = current->parent;
      temp->n_children = current->n_children-1;
      for(i=0;i<temp->n_children;i++)
      {
          temp->children[i] = current->children[i+1];
          current->children[i+1]->parent = temp;
          create_ast(temp->children[i]);
      }
      //create_ast(temp);
      free(current);
    
    }
    if(strcmp(current->node_name,"IDEN") == 0 || strcmp(current->node_name,"INTR") == 0)
    {
    //  if(strcmp(current->parent->node_name,"MULTIPLE") == 0)
      //  printf("Hello\n");
      struct node* temp;
    //  printf("Hello Entered\n");
      if(current->n_children == 0 || strcmp(current->children[0]->node_name,"UNDERSCORE") == 0)
      {
        return;
      }
      else
      {
        if(current->parent == parse_tree)
        {
          //for(i=0;i<current->n_children;i++)
          //{
           // create_ast(current->parent);
          //}
        }
        else
        {
        //  printf("Hello1312312\n");
          temp = current->children[0];
          current->parent->children[current->parent->n_children] = temp;
          current->n_children--;
          current->parent->n_children++;
          current->children[0] = NULL;
          create_ast(current->parent);
          
        }
      }
    }
  
  }
}

struct ref_line
{
  char data[20];
  struct ref_line* next;
};
struct ret_list
{
  char data[20];
  struct ret_list* next;
};
struct par_list
{
  char type[20];
  char name[20];
  struct par_list* next;
};
struct varNode
{
  int scope;
  char dec_line[20];
  char type[20];
  char name[30];
  struct ref_line* referenced_line;
};

struct funcNode
{
  int scope;
  char dec_line[20];
  char name[20];
  struct list* return_list;
  struct par_list* parameter_list;
  
};

struct HashTableNode
{
  struct funcNode* func;
  struct varNode* var;
  struct HashTableNode* next;
};


struct HashTableNode* TABLE[200];
char temp[50];


int hash_func(char *str)
{
    int i,hash=7;
  for (i=0; i < strlen(str); i++) 
  {
    hash = hash*31+str[i];
    hash%= 200;
  }

    return hash%200;
}
struct funcNode* f;
int curr_scope,i;
/*void createHashTable(struct node* ast,int curr_scope)
{
  
  if(ast == NULL)
  {
    return;
  }
  else
  {
    printf("%s\n",ast->node_name);
    if(ast == parse_tree)
    {
      
      if(strcmp(ast->children[0]->node_name,"GLOBAL") == 0)
      {
        curr_scope = 0;
      }
      else
      {
        
        for(i=0;i<ast->n_children;i++)
        {
          curr_scope = i;
          int hash = hash_func(ast->children[i]->node_name);
          
          struct funcNode* f = (struct funcNode*) malloc(sizeof(struct funcNode));
          f->scope = curr_scope;
          strcpy(f->dec_line,ast->children[i]->node_lineno);
          strcpy(f->name,ast->children[i]->node_name);
          f->return_list = NULL;
          f->parameter_list = NULL;
          
          TABLE[hash] =(struct HashTableNode*) malloc(sizeof(struct HashTableNode));
          TABLE[hash]->func = f;
          TABLE[hash]->var = NULL;
          TABLE[hash]->next = NULL;
          
          createHashTable(ast->children[i],curr_scope);
          return;
        }
      }
    }
    if(strcmp(ast->node_name,"INTEGER") == 0 || strcmp(ast->node_name,"BOOLEAN") == 0 || strcmp(ast->node_name,"COLLECTION") == 0 || strcmp(ast->node_name,"STRING") == 0 || strcmp(ast->node_name,"CHARACTER") == 0)
    {
      //printf("Hello\n");
      //printf("%s\n",ast->node_name);
      for(i=0;i<ast->n_children;i++)
      {
        int hash = hash_func(ast->children[i]->node_value);
        printf("%d\n",hash);
        printf("%s\n",ast->children[i]->node_value);
        
        struct varNode* v = (struct varNode*) malloc(sizeof(struct varNode));
        v->scope = curr_scope;
        strcpy(v->dec_line,ast->children[i]->node_lineno);
        strcpy(v->name,ast->children[i]->node_value);
        strcpy(v->type,ast->node_name);
        if(v->referenced_line == NULL)
        {
          v->referenced_line = (struct ref_line*) malloc(sizeof(struct ref_line));
          strcpy(v->referenced_line->data,ast->children[i]->node_lineno);
          v->referenced_line->next = NULL;
        }
        else
        {
          struct ref_line* r = (struct ref_line*) malloc(sizeof(struct ref_line));
          strcpy(r->data,ast->children[i]->node_lineno);
          r->next = v->referenced_line;
        }
          TABLE[hash] =(struct HashTableNode*) malloc(sizeof(struct HashTableNode));
          TABLE[hash]->func = NULL;
          TABLE[hash]->var = v;
          TABLE[hash]->next = NULL;
        
      }
    }
    /*else
    {
      /*if(strcmp(ast->node_name,"<smts_&_func_defs>") == 0)
      {
        printf("Hello Here\n");
       // printf("%d\n",ast->n_children);
       // printf("%s\n",ast->children[0]->node_name);
        printf("%d\n",ast->children[1]->n_children);
        printf("%s\n",ast->children[0]->children[0]->node_name);
        printf("%s\n",ast->children[0]->children[0]->node_name);
    
      }
      
    }
    
    if(ast->n_children > 0)
    {
      for(i=0;i<ast->n_children;i++)
      {
        createHashTable(ast->children[i],curr_scope);
      }
    }
  }
}
*/
int hash;

void createHashTable(struct node* node,int curr_scope)
{
  int i;
  if(node == NULL)
    return;
  else
  {
  //  printf("%s\n",node->node_name);
    if(node == parse_tree)
    {
      
      if(strcmp(node->children[0]->node_name,"GLOBAL") == 0)
      {
        curr_scope = 0;
        for(i=0;i<node->n_children;i++)
        {
          createHashTable(node->children[i],0);
        }
      }
      else
      {
        
        for(i=0;i<node->n_children;i++)
        {
          
          int hash = hash_func(node->children[i]->node_name);
          
          struct funcNode* f = (struct funcNode*) malloc(sizeof(struct funcNode));
          f->scope = curr_scope;
          strcpy(f->dec_line,node->children[i]->node_lineno);
          strcpy(f->name,node->children[i]->node_name);
          f->return_list = NULL;
          f->parameter_list = NULL;
          
          TABLE[hash] =(struct HashTableNode*) malloc(sizeof(struct HashTableNode));
          TABLE[hash]->func = f;
          TABLE[hash]->var = NULL;
          TABLE[hash]->next = NULL;
          
          createHashTable(node->children[i],i+1);
          return;
        }
      } 
    } 
   // if(strcmp(node->node_name,"IDEN") == 0)
    //  return;


  else if(strcmp(node->node_name,"MAIN") == 0)
  {
      hash = hash_func(node->node_name);

      if(TABLE[hash])
      {

      }
      else
      {
          struct funcNode* f = (struct funcNode*) malloc(sizeof(struct funcNode));
          f->scope = curr_scope;
          strcpy(f->dec_line,node->node_lineno);
          strcpy(f->name,node->node_name);
          f->return_list = NULL;
          f->parameter_list = NULL;
          
          TABLE[hash] =(struct HashTableNode*) malloc(sizeof(struct HashTableNode));
          TABLE[hash]->func = f;
          TABLE[hash]->var = NULL;
          TABLE[hash]->next = NULL;
      }

      for(i=0;i<node->n_children;i++)
        createHashTable(node->children[i],curr_scope);
     

  }

   else if(strcmp(node->node_name,"IDEN") == 0)
    {
      hash = hash_func(node->node_value);
     

      if(TABLE[hash])
      {
         //  printf("%s\n",node->node_value);
      }
      else
      {
          struct varNode* v = (struct varNode*) malloc(sizeof(struct varNode));
          v->scope = curr_scope;
          //printf("Hello  %s %s\n",node->children[i]->node_value,node->children[i]->node_lineno);
          strcpy(v->dec_line,node->node_lineno);
          strcpy(v->name,node->node_value);
          strcpy(v->type,node->node_name);

          TABLE[hash] =(struct HashTableNode*) malloc(sizeof(struct HashTableNode));
          TABLE[hash]->func = NULL;
          TABLE[hash]->var = v;
          TABLE[hash]->next = NULL;
      }

    }
    else if(strcmp(node->node_name,"INTEGER") == 0 || strcmp(node->node_name,"BOOLEAN") == 0 || strcmp(node->node_name,"COLLECTION") == 0 || strcmp(node->node_name,"STRING") == 0 || strcmp(node->node_name,"CHARACTER") == 0)
    {
      if(strcmp(node->node_name,"STRING") == 0)
      {
        //printf("Hello\n");
        //printf("%s\n",node->children[0]->node_name);
        //printf("%s\n",node->children[0]->node_value);
      }

      for(i=0;i<node->n_children;i++)
      { 
        if(strcmp(node->children[i]->node_name,"IDEN") == 0)
        {
               hash = hash_func(node->children[i]->node_value);
             //  printf("%d\n",hash);
          
        
          struct varNode* v = (struct varNode*) malloc(sizeof(struct varNode));
          v->scope = curr_scope;
          //printf("Hello  %s %s\n",node->children[i]->node_value,node->children[i]->node_lineno);
          strcpy(v->dec_line,node->children[i]->node_lineno);
          strcpy(v->name,node->children[i]->node_value);
          strcpy(v->type,node->node_name);
          if(v)
          {
            //printf("udcshjcsd  %s %s\n",v->name,v->dec_line);
          } 
          
              
        
            if(TABLE[hash])
            {
           //   printf("Hello\n");
            //  printf("%s\n",TABLE[hash]->var->name);
              //printf("%d\n",hash);
              struct HashTableNode* temp;
              temp = TABLE[hash];
             // printf("Hello   %s\n",temp->var->name);
              while(temp->next)
              {
                  temp = temp->next;
              }
              struct HashTableNode* a = (struct HashTableNode*) malloc(sizeof(struct HashTableNode));
              a->func = NULL;
              a->var = v;
              a->next = NULL;
              temp->next = a;

              
           //   temp = TABLE[hash];
              while(temp)
              {
              //  printf("%d\n",hash);
              //  printf("Hello   %s\n",temp->var->name);
              //  printf("Hello %s\n",temp->var->dec_line);
                temp = temp->next;
              }

            }
          
           else
           {
              TABLE[hash] =(struct HashTableNode*) malloc(sizeof(struct HashTableNode));
              TABLE[hash]->func = NULL;
              TABLE[hash]->var = v;
            //  printf("%d\n",hash);
             // printf("dsvdsvcsd  %s %s\n",TABLE[hash]->var->name,TABLE[hash]->var->dec_line);
              TABLE[hash]->next = NULL;  
            }
        }
        else
        {
            createHashTable(node->children[i],curr_scope);
        }
      }
    }
    else
    {
      for(i=0;i<node->n_children;i++)
      {
//        printf("%d\n",i);
        if(node->children[i] != NULL)
          createHashTable(node->children[i],curr_scope);
      }
      
    }
  }
}

void lookup(Token str)
{
  hash = hash_func(str.lexeme);
  if(TABLE[hash])
  {
      struct HashTableNode* temp;
      temp = TABLE[hash];

      if(temp->func)
      {

      }
      else{
        if(temp->var)
        {
          while(strcmp(temp->var->name,str.lexeme) != 0)
          {
            if(temp == NULL)
            {
              break;
            }
            temp = temp->next;

          }

          if(temp)
          {
            if(temp->var->referenced_line == NULL)
            {
              temp->var->referenced_line = (struct ref_line*) malloc(sizeof(struct ref_line));
              sprintf(temp->var->referenced_line->data,"%d",str.line_number);
              temp->var->referenced_line->next = NULL;
            }
            else
            {
              struct ref_line* new;
              new = (struct ref_line*) malloc(sizeof(struct ref_line));
              sprintf(new->data,"%d",str.line_number);
              new->next = temp->var->referenced_line;
              temp->var->referenced_line = new;
            }
            printf("%s %s %d\n",temp->var->name,temp->var->dec_line,temp->var->scope);
          }
          else
          {

          }  

        }
      }
  }
  else
  {
    printf("Identifier Not declared\n");
  }
}

int main(int argc,char* argv[]){
	
	if(argc < 1)
	{
		printf("File name not given in the arguments\n");
	}
	else
	{
		read_parse_table();
    load_data(argv[1]);
    root = push(root, "EOF");
    root = push(root, "<Program>");
    focus = peek(root);
	  parse_tree_root = alloc_node("<Program>");
	  parse_tree = parse_tree_root;
	  parse(parse_tree_root);
   
    merge_node(parse_tree);
    create_ast(parse_tree);
     print_tree(parse_tree);
    ast = parse_tree;
  //  print_tree(parse_tree);
   createHashTable(ast,0);

   
    for(i=0;i<200;i++)
     {
        if(TABLE[i])
        {
       //   printf("%d\n",i);
          if(TABLE[i]->var)
          {
            printf("%s %s\n",TABLE[i]->var->name,TABLE[i]->var->dec_line);
            if(TABLE[i]->next)
            {
              printf("%s %s\n",TABLE[i]->next->var->name,TABLE[i]->next->var->dec_line);
            }
           // printf("%d %s\n",i,TABLE[i]->var->dec_line);
          }
          if(TABLE[i]->func)
          {
            printf("%s %s\n",TABLE[i]->func->name,TABLE[i]->func->dec_line);
          //  printf("%d %s\n",i,TABLE[i]->func->name); 
          }
        }
     }
  
	 
  }
  
	return 0;
}

