#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parserDef.h" 
#include "lexerDef.h"
#include "SymbolTableDef.h"

struct MultipleNodeType
{
  char *type;
  struct MultipleNodeType *next;
};

int no_of_children = 0; 

int size(struct node* node) 
{  
   int i;
   no_of_children = no_of_children + node ->n_children;
   for(i=0;i<node ->n_children;i++)
    { 
     size(node->children[i]);
    }
   return no_of_children;
}

void append(struct MultipleNodeType** head_ref, char *new_data)
{
    struct MultipleNodeType* new_node = (struct MultipleNodeType*) malloc(sizeof(struct MultipleNodeType));
    struct MultipleNodeType *last = *head_ref; 
    new_node->type  = new_data;
    new_node->next = NULL;
    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }  
    while (last->next != NULL)
        last = last->next;
     last->next = new_node;
    return;    
}

int check_multiple_as_types(struct MultipleNodeType *a, struct MultipleNodeType *b)
{
  if (a == NULL && b == NULL)    
  return 1;  
  if (a == NULL && b != NULL) 
    return 0;  
  if (a != NULL && b == NULL) 
    return 0;  
  if (strcmp(a->type,b->type)!=0)
  return 0;  
  return check_multiple_as_types(a->next, b->next);   
}

void create_list(struct MultipleNodeType** head_ref, struct node* node)
{
 int i;
 for(i=0;i<node ->n_children;i++)
  { 
    int i1;
    for(i1=0;i1<TABLE_SIZE;i1++)
     { if(TABLE[i1])
        { if(TABLE[i1]->var)
          {  
            if(strcmp(TABLE[i1]->var->name, node->children[i] ->node_value ) ==0)    
              { 
                append(head_ref,TABLE[i1] ->var -> type);  
              }
           }
      }  }    
  }
 for(i=0;i<node ->n_children;i++)
    { 
     create_list(head_ref,node->children[i]);
    }
}

void create_list2(struct MultipleNodeType** head_ref, struct node* node)
{
  int i;
  for(i=0;i<node ->n_children;i++)
  {   
    if(strcmp(node->children[i] ->node_name, "INTR") ==0)  
            append(head_ref,"INTEGER");
    if(strcmp(node->children[i] ->node_name, "DEC") ==0)  
            append(head_ref,"DECIMAL");      
  }
  for(i=0;i<node ->n_children;i++)
    { 
     create_list2(head_ref,node->children[i]);
    }
}

int search_matching_index_in_table(struct node* node)
{
  int i;
  for(i=0;i<TABLE_SIZE;i++)
  {  
  if(TABLE[i])
  { 
    if(TABLE[i]->var)
    {
      if(strcmp(TABLE[i]->var->name, node->node_value ) ==0)
      {
        return i;
      }
    }
  }
  }
}

char* type_of_simple_expression(struct node* node)
{  
  char* type_of_exp = malloc(sizeof(char) * 20);
  char *def_return = malloc(sizeof(char) * 20);
  strcpy(def_return,"NOVALUE");
  if(strcmp(node->children[0]->node_name,"IDEN") == 0)
 { char operand1_type[20];
   int index = search_matching_index_in_table(node->children[0]);
   strcpy(operand1_type,TABLE[index]->var->type);
   //printf("%s\n",operand1_type );
  if(strcmp(operand1_type,"INTEGER") ==0)
  { 
    if(strcmp(node->children[1]->children[0]->node_name,"INTR")!=0)
      { if(strcmp(node->children[1]->children[0]->node_name,"IDEN")==0)
        {  int ind = search_matching_index_in_table(node->children[1] ->children[0]);
           if(strcmp(TABLE[ind]->var->type,"INTEGER")!=0)
            {printf("TYPE CHECKING ERROR (At Line %s): Cannot '%s' different types. \n",node->children[0]->node_lineno ,node->children[1]->node_name);
              return def_return;}
           else
           {
            if(strcmp(node->children[1]->node_name,"PLUS")!=0 && strcmp(node->children[1]->node_name,"MINUS")!=0 && strcmp(node->children[1]->node_name,"MULT")!=0 && strcmp(node->children[1]->node_name,"DIV")!=0)
               {printf("SEMANTIC ERROR(At line %s): Only '+','-','*','/' operations are supported for integers.\n",node->children[0]->node_lineno);
                return def_return;}
           }
        }
        else
          {printf("TYPE CHECKING ERROR (At Line %s): Cannot '%s' different types. \n",node->children[0]->node_lineno ,node->children[1]->node_name);
           return def_return;}
      }
     else
      { if(strcmp(node->children[1]->node_name,"PLUS")!=0 && strcmp(node->children[1]->node_name,"MINUS")!=0 && strcmp(node->children[1]->node_name,"MULT")!=0 && strcmp(node->children[1]->node_name,"DIV")!=0)
         {printf("SEMANTIC ERROR(At line %s): Only '+','-','*','/' operations are supported for integers.\n",node->children[0]->node_lineno);
         return def_return;}
        else
         { strcpy(type_of_exp,"INTEGER");
           return type_of_exp;
         }
      }
  }
  if(strcmp(operand1_type,"DECIMAL") ==0)
  {
    if(strcmp(node->children[1]->children[0]->node_name,"INTR")!=0 && strcmp(node->children[1]->children[0]->node_name,"DEC")!=0 )
      { if(strcmp(node->children[1]->children[0]->node_name,"IDEN")==0)
        {  int ind = search_matching_index_in_table(node->children[1] ->children[0]);
           if(strcmp(TABLE[ind]->var->type,"INTEGER")!=0 && strcmp(TABLE[ind]->var->type,"DECIMAL")!=0)
            {printf("TYPE CHECKING ERROR (At Line %s): Cannot '%s' different types. \n",node->children[0]->node_lineno ,node->children[1]->node_name);
             return def_return;}
           else
           {
            if(strcmp(node->children[1]->node_name,"PLUS")!=0 && strcmp(node->children[1]->node_name,"MINUS")!=0 && strcmp(node->children[1]->node_name,"MULT")!=0 && strcmp(node->children[1]->node_name,"DIV")!=0)
               {printf("SEMANTIC ERROR(At line %s): Only '+','-','*','/' operations are supported for decimals.\n",node->children[0]->node_lineno);
                return def_return;}
           }
        }
        else
          {printf("TYPE CHECKING ERROR (At Line %s): Cannot '%s' different types. \n",node->children[0]->node_lineno ,node->children[1]->node_name);
        return def_return;}
      }
     else
      { if(strcmp(node->children[1]->node_name,"PLUS")!=0 && strcmp(node->children[1]->node_name,"MINUS")!=0 && strcmp(node->children[1]->node_name,"MULT")!=0 && strcmp(node->children[1]->node_name,"DIV")!=0)
         {printf("SEMANTIC ERROR(At line %s): Only '+','-','*','/' operations are supported for decimals.\n",node->children[0]->node_lineno);
         return def_return;}
        else
         { strcpy(type_of_exp,"DECIMAL");
           return type_of_exp;
         }
      } 
  }

 }
 if(strcmp(node->children[0]->node_name,"INTR") == 0)
 {
       //do this fn if time
 }

 if(strcmp(node->children[0]->node_name,"REPEAT") ==0)
  {
    if(strcmp(node->children[0]->children[0]->node_name,"IDEN") == 0)
    {
      int index = search_matching_index_in_table(node->children[0]->children[0]);
      if(strcmp(TABLE[index]->var->type,"STRING")!=0 || strcmp(node->children[0]->children[1]->node_name,"INTR") != 0)
        {printf("SEMANTIC ERROR(At line %s): Either the variable near Repeat '#' is not of type 'String' or the value specified is not a integer.\n",node->children[0]->node_lineno );
          return def_return;
        }
      else
      {
        strcpy(type_of_exp,"STRING");
        return type_of_exp;
      }       
    }
  } 

 if(strcmp(node->children[0]->node_name,"LEXICOGRAPHIC") ==0)
  {
    if(strcmp(node->children[0]->children[0]->node_name,"IDEN") == 0 && strcmp(node->children[0]->children[1]->node_name,"IDEN") == 0)
    {
      int index1 = search_matching_index_in_table(node->children[0]->children[0]);
      int index2 = search_matching_index_in_table(node->children[0]->children[1]);
      if(strcmp(TABLE[index1]->var->type,"STRING")==0 && strcmp(TABLE[index2]->var->type,"STRING")==0)
      {
        strcpy(type_of_exp,"BOOLEAN");
        return type_of_exp;
      }
      else
        {printf("SEMANTIC ERROR (At line %s): One or both variables near LEXICOGRAPHIC '?' is not of type'String'.\n",node->children[0]->node_lineno );
          return def_return;
        }
    }
   else
   {
     printf("SEMANTIC ERROR (At line %s): LEXICOGRAPHIC comparison '?' can be done on predefined strings only.\n",node->children[0]->node_lineno );
     return def_return;
   }
  }  
}

int main_flag =0;

int check_main_present_or_not(struct node* node)
{
  if(strcmp(node -> node_name,"MAIN" )==0)
  { main_flag ++;
    int i;
    for(i=0;i<node->n_children;i++)
    check_main_present_or_not(node->children[i]); 

  }
 else
  {
    int i;
    for(i=0;i<node->n_children;i++)
    check_main_present_or_not(node->children[i]); 
  }
 return main_flag; 
}

char call_child[20];
int call_flag =0;
char call_def[20];

char *traverse(struct node* node)
{  
  strcpy(call_def,"NOVALUE");
  if(strcmp(node->node_name,"<functionCall>")==0)
    {
      strcpy(call_child, node->children[0]->node_value);
      call_flag =1;
    }
  
    int i;
    for(i=0;i<node->n_children;i++)
      traverse(node->children[i]);
  if(call_flag ==1)
  {  //printf("%s\n", call_child);
    return call_child;
  }
  else
    return call_def;
} 
 
int return_flag=0;

int search_for_return(struct node* node)
{
  if(strcmp(node->node_name,"RETURN")==0)
  {
    return_flag =1;
  }
  int i;
  for(i=0;i<node->n_children;i++)
    search_for_return(node->children[i]);
  if(return_flag == 1)
    return 1;
  else
    return 0;
} 

void IS_VARIABLE_DECLARED_BEFORE_USE()
{ 
  int i;
  for(i=0;i<TABLE_SIZE;i++)
  {  
    if(TABLE[i])
    {
      if(TABLE[i]->var)
      {
        if(strcmp(TABLE[i]->var->type,"IDEN") ==0)
        {
        int i1;
        int flag =0;
        for(i1=0;i1<TABLE_SIZE;i1++)
        {  
        if(TABLE[i1])
        {
        if(TABLE[i1]->func)
        {
        if(TABLE[i1]->func->parameter_list)
        {  
          if(strcmp(TABLE[i]->var->name,TABLE[i1]->func->parameter_list->name) ==0)
          { //printf("%s\n",TABLE[i1]->func->parameter_list->type );
            flag =1;
            break;
          }
        }
        }  
        }
        }
        if(flag == 0)
        printf("SEMANTIC ERROR(At Line %s): Variable '%s' not declared before use.\n",TABLE[i]->var->dec_line, TABLE[i]->var->name);
        }
      }
    }
  }
}

void IS_LHS_AND_RHS_OF_ASSIGN_ARE_SAME_TYPE(struct node* node)
{
  if(strcmp(node->node_name,"ASSIGN" )==0 )//&& node->children[0]->n_children == 0 && strcmp(node->children[1]->node_name,"AS") !=0)
    { if(node->children[0]->n_children == 0 && strcmp(node->children[1]->node_name,"AS") !=0)
    { int i;
      for(i=0;i<TABLE_SIZE;i++)
      { if(TABLE[i])
        { if(TABLE[i]->var)
         {  
          if(strcmp(TABLE[i]->var->name, node->children[0] ->node_value ) ==0)     
           { if(strcmp(TABLE[i]->var->type, "BOOLEAN")==0)
            { if((strcmp(node->children[1]->node_value,"TRUE")==0) || (strcmp(node->children[1] ->node_value,"FALSE")==0))
             {}
             else
             { printf("TYPE CHECKING ERROR (At Line %s): 'assign %s' has different types of values, expected Boolean\n",node->children[1]->node_lineno, node->children[0]->node_value );
               break;
             } 
            }
            if(strcmp(TABLE[i]->var->type, "INTEGER")==0)
            { if(strcmp(node->children[1] ->node_name, "INTR")==0) 
            { //printf("SEMANTIC ERROR (At Line %s):'assign %s' has different types of values, expected Integer.\n",node->children[1]->node_lineno,node->children[0]->node_value  );
              //ok
            }
            else if(strcmp(node->children[1] ->node_name, "IDEN")==0) 
            {
              //printf("here2\n");
              int i1;
              for(i1=0;i1<TABLE_SIZE;i1++)
              {  
              if(TABLE[i1])
              {
               if(TABLE[i1]->func)
                {
                   if(TABLE[i1]->func->parameter_list)
                    {  
                   if(strcmp(node->children[1]->node_value,TABLE[i1]->func->parameter_list->name) ==0)
                     {if (strcmp(TABLE[i1]->func->parameter_list->type,"INTEGER") ==0)
                       {
                         //ok
                       }
                       else
                         printf("TYPE CHECKING ERROR (At Line %s):'assign %s' has different types of values, expected Integer.\n",node->children[1]->node_lineno,node->children[0]->node_value  );
                      }
                    }
                }
              }
              }    
             

            }
            else
              printf("TYPE CHECKING ERROR (At Line %s):'assign %s' has different types of values, expected Integer.\n",node->children[1]->node_lineno,node->children[0]->node_value  );
            }
            if(strcmp(TABLE[i]->var->type, "DECIMAL")==0)
            {       
            if(strcmp(node->children[1] ->node_name, "DEC")==0 || strcmp(node->children[1] ->node_name, "INTR")==0) 
            {// printf("SEMANTIC ERROR (At Line %s):'assign %s' has different types of values, expected Decimal or Integer.\n",node->children[1]->node_lineno , node->children[0]->node_value );
              //break;
            } 
            else if(strcmp(node->children[1] ->node_name, "IDEN")==0) 
            {
              //printf("here2\n");
              int i1;
              for(i1=0;i1<TABLE_SIZE;i1++)
              {  
              if(TABLE[i1])
              {
               if(TABLE[i1]->func)
                {
                   if(TABLE[i1]->func->parameter_list)
                    {  
                   if(strcmp(node->children[1]->node_value,TABLE[i1]->func->parameter_list->name) ==0)
                     {if (strcmp(TABLE[i1]->func->parameter_list->type,"DECIMAL") ==0)
                       {
                         //ok
                       }
                       else
                         printf("TYPE CHECKING ERROR (At Line %s):'assign %s' has different types of values, expected Decimal.\n",node->children[1]->node_lineno,node->children[0]->node_value  );
                      }
                    }
                }
              }
              }  
            }
            else
                 printf("TYPE CHECKING ERROR (At Line %s):'assign %s' has different types of values, expected Decimal or Integer.\n",node->children[1]->node_lineno , node->children[0]->node_value );   
            } //decimal ends
            if(strcmp(TABLE[i]->var->type, "STRING")==0)
            { if(strcmp(node->children[1] ->node_name, "STR")==0) 
            { //printf("SEMANTIC ERROR (At Line %s):'assign %s' has different types of values, expected String.\n",node->children[1]->node_lineno,node->children[0]->node_value  );
              //break;
            }
            else if(strcmp(node->children[1] ->node_name, "IDEN")==0) 
            {
              //printf("here2\n");
              int i1;
              for(i1=0;i1<TABLE_SIZE;i1++)
              {  
              if(TABLE[i1])
              {
               if(TABLE[i1]->func)
                {
                   if(TABLE[i1]->func->parameter_list)
                    {  
                   if(strcmp(node->children[1]->node_value,TABLE[i1]->func->parameter_list->name) ==0)
                     {if (strcmp(TABLE[i1]->func->parameter_list->type,"STRING") ==0)
                       {
                         //ok
                       }
                       else
                         printf("TYPE CHECKING ERROR (At Line %s):'assign %s' has different types of values, expected String.\n",node->children[1]->node_lineno,node->children[0]->node_value  );
                      }
                    }
                }
              }
              }  
            }
            else
              printf("TYPE CHECKING ERROR (At Line %s):'assign %s' has different types of values, expected String.\n",node->children[1]->node_lineno,node->children[0]->node_value  );
            }
           
          break;
          }
        }
       }
      } 
    }
  }

  else
  {
  int i;
  for(i=0;i<node->n_children;i++)
  IS_LHS_AND_RHS_OF_ASSIGN_ARE_SAME_TYPE(node->children[i]);
  }
}

void IS_NUMBER_AND_TYPES_WHILE_ASSIGNING_MULTIPLE_SAME(struct node *node)
{
  if(strcmp(node->node_name,"ASSIGN" )==0 )
  {
    if(strcmp(node->children[0] ->node_name, "MULTIPLE") == 0 && strcmp(node->children[1] ->node_name, "AS") == 0)
    {int multiple_children = size(node->children[0]); 
     //printf("%d\n",multiple_children );
     no_of_children =0;
     int as_children = size(node->children[1]);
     no_of_children =0;
     //printf("%d\n",as_children );
     if(multiple_children != as_children)
      printf("SEMANTIC ERROR (At Line %s): number of variables in 'multiple' and values assigned are different.\n", node->children[0]->node_lineno );
     else
     {
       struct MultipleNodeType* head = NULL;
       create_list(&head,node->children[0]);
       //printList(head);
       struct MultipleNodeType *head2 = NULL;
       create_list2(&head2,node->children[1]);
       //printList(head2);
       int check = check_multiple_as_types(head, head2);
       if(check !=1)
        printf("TYPE CHECKING ERROR (At Line %s): types of corresponding variables in 'multiple' and values assigned are different.\n", node->node_lineno );
      } 

    }
  }
  else
  {
  int i;
  for(i=0;i<node->n_children;i++)
  IS_NUMBER_AND_TYPES_WHILE_ASSIGNING_MULTIPLE_SAME(node->children[i]);
  }  
}

void RULES_WHILE_CALLING_A_FUNCTION_AND_ASSIGNING(struct node* node)
{ 
  if(strcmp(node->node_name,"ASSIGN" )==0 )
  {
  if(strcmp(node->children[0] ->node_name, "MULTIPLE") == 0 && strcmp(node->children[1] ->node_name, "<functionCall>") == 0)
    {
      int multiple_children = size(node->children[0]); 
      //printf("%d\n",multiple_children );
      no_of_children =0;
      int functionCall_children = size(node->children[1]);
      //printf("%d\n",functionCall_children );
      no_of_children =0;
      int no_of_ret=0;
      int no_of_para =0;
      int i;
      int flag =0;
      for(i=0;i<TABLE_SIZE;i++)
      { 
        if(TABLE[i])
        { 
          if(TABLE[i]->func)
           {
            if(strcmp(TABLE[i]->func->name, node->children[1]->children[0] ->node_value ) ==0)  
              { 
                flag =1;
                no_of_ret = TABLE[i]->func->ret ;
                no_of_para = TABLE[i]->func->param;
                break;
              }
           }
        }
      }
      if(flag ==0)
        printf("TYPE CHECKING ERROR (At Line %s): Expected a function name after 'call' keyword.\n",node->children[0]->node_lineno );  
      if(multiple_children != no_of_ret)
        printf("SEMANTIC ERROR (At Line %s):Number of values returned by function and specified in 'assign' do not match.\n",node->children[0]->node_lineno );
      if((functionCall_children -1) != no_of_para)
        printf("SEMANTIC ERROR (At Line %s):Number of parameters in function definition and specified in 'assign' do not match.\n",node->children[0]->node_lineno );
    }
   } 
    else
    {
     int i;
     for(i=0;i<node->n_children;i++)
     RULES_WHILE_CALLING_A_FUNCTION_AND_ASSIGNING(node->children[i]); 
    }
}

void RULES_WHILE_ASSIGNING_ARRAYS(struct node* node)
{
  if(strcmp(node->node_name,"ASSIGN" )==0 )
  {
    if((node->children[0]->n_children != 0) && strcmp(node->children[1]->node_name,"AS") !=0)
    { int i;
      for(i=0;i<TABLE_SIZE;i++)
      { if(TABLE[i])
        {if(TABLE[i]->var)
          {
          if(strcmp(TABLE[i]->var->name, node->children[0] ->node_value ) ==0)
            { //printf("%s----%s\n",TABLE[i]->var->name,TABLE[i]->var->type );
              int index = search_matching_index_in_table(node->children[1]);
              if(strcmp(TABLE[i]->var->type,TABLE[index]->var->type) !=0)
                printf("TYPE CHECKING ERROR(At Line No %s): %s array '%s' cannot be assigned a %s value. \n",node->children[0]->node_lineno,TABLE[i]->var->type,node->children[0] ->node_value,TABLE[index]->var->type );
              int index2 = search_matching_index_in_table(node->children[1]);
                if(strcmp(TABLE[index2]->var->type,"INTEGER") !=0)
                  printf("SEMANTIC ERROR (At Line No %s): Array '%s' should have a integer index.\n",node->children[0]->node_lineno , node->children[0] ->node_value); 
            }
          }
        }
      }
    }
  }
  else
  {
   int i;
   for(i=0;i<node->n_children;i++)
   RULES_WHILE_ASSIGNING_ARRAYS(node->children[i]); 
  }
}

void RULES_WHILE_ARRAY_SLICING(struct node* node)
{
  if(strcmp(node->node_name,"ASSIGN" )==0 )
  { 
    if((node->children[0]->n_children != 0) && strcmp(node->children[1]->node_name,"AS") ==0 && strcmp(node->children[0]->node_name,"MULTIPLE") !=0 )
    { 
      int index1 = search_matching_index_in_table(node->children[0]);
      //printf("%s\n",TABLE[index1]->var->name );
      int index2 = search_matching_index_in_table(node->children[1]->children[0]);
      //printf("%s\n",TABLE[index2]->var->name );
      if(strcmp(TABLE[index1]->var->type, TABLE[index2]->var->type)!=0)
        printf("TYPE CHECKING ERROR(At Line No %s): Array slicing not possible due to different types of '%s' and '%s'.\n",node->children[0]->node_lineno ,TABLE[index1]->var->name, TABLE[index2]->var->name); 
      else
      {
        node = node->children[0] -> children[0];
        int range1 = atoi(node->children[0]->node_value) - atoi(node->children[1]->node_value);
        if(range1 < 0)
          range1 = -1* range1;
        //printf("%d\n",range1 );
        node = node->parent->parent->parent;
        node = node->children[1]->children[1];
        int range2 = atoi(node->children[0]->node_value) - atoi(node->children[1]->node_value);
        if(range2 < 0)
          range2 = -1* range2;
        //printf("%d\n",range2 );
        if(range1 != range2)
          printf("SEMANTIC ERROR(At Line No %s): Ranges for slicing of '%s' and '%s'do not match.\n",node->children[0]->node_lineno ,TABLE[index1]->var->name, TABLE[index2]->var->name); 
        node = node->parent->parent;
      }
    }
  }
  else
  {
   int i;
   for(i=0;i<node->n_children;i++)
   RULES_WHILE_ARRAY_SLICING(node->children[i]); 
  }
}

void IS_EXPRESSION_TYPE_AND_VARIABLE_TYPE_SAME(struct node* node)
{
  if(strcmp(node->node_name,"ASSIGN" )==0 )
  { 
    if((node->children[0]->n_children == 0) && strcmp(node->children[1]->node_name,"AS") ==0 && strcmp(node->children[1]->children[0] ->node_name,"NAME_EQU")!=0  && strcmp(node->children[1]->children[0] ->node_name,"STRUCT_EQU")!=0)
    {  //printf("hello---\n");
      //printf("%s-- \n",node->children[0]->node_value );
      int index = search_matching_index_in_table(node->children[0]);
      char type_lhs[20];
      strcpy(type_lhs,TABLE[index]->var->type );
      //printf("lhs %s\n",type_lhs );
      char type_rhs[20];
      strcpy(type_rhs,type_of_simple_expression(node->children[1]));
      //printf("rhs %s\n",type_rhs );   
      if(strcmp(type_lhs,type_rhs) != 0)
      {
        if(strcmp(type_lhs,"DECIMAL")==0 && strcmp(type_rhs,"INTEGER") ==0)
        {
          //ok
        }
        else
          printf("SEMANTIC ERROR (At Line %s): Trying to 'assign' expression of a different type to a variable.\n", node->children[0]->node_lineno);
      }
    }
  }
  else
  {
   int i;
   for(i=0;i<node->n_children;i++)
   IS_EXPRESSION_TYPE_AND_VARIABLE_TYPE_SAME(node->children[i]); 
  }
}

void IS_EXPRESSION_INSIDE_IF_BOOLEAN(struct node* node)
{
  if(strcmp(node -> node_name,"IF" )==0)
  { 
    if(strcmp(node ->children[0]->node_name,"LTE")!=0 && strcmp(node ->children[0]->node_name,"LT")!=0 && strcmp(node ->children[0]->node_name,"GTE")!=0 && strcmp(node ->children[0]->node_name,"GT")!=0 && strcmp(node ->children[0]->node_name,"EQ")!=0 && strcmp(node ->children[0]->node_name,"NEQ")!=0 && strcmp(node ->children[0]->node_name,"AND")!=0 && strcmp(node ->children[0]->node_name,"OR")!=0 && strcmp(node ->children[0]->node_name,"NOT")!=0 )
      {  //printf("here2\n");
        printf("SEMANTIC ERROR(At Line %s): EXPRESSION INSIDE IF STATEMENT IS NOT OF THE TYPE BOOLEAN. \n", node->children[0]->node_lineno );
       }
    int i;
    for(i=0;i<node->n_children;i++)
    IS_EXPRESSION_INSIDE_IF_BOOLEAN(node->children[i]); 
  }
  else
  {
    int i;
    for(i=0;i<node->n_children;i++)
    IS_EXPRESSION_INSIDE_IF_BOOLEAN(node->children[i]); 
  }   
}

void CHECKING_TYPES_OF_IF_EXPRESSION(struct node* node)
{
 if(strcmp(node -> node_name,"IF" )==0)
  { 
    if(node-> children[0] -> n_children ==2)
    {
     int index_lhs = search_matching_index_in_table(node->children[0] ->children[0]);
     //printf("%s\n",TABLE[index_lhs]->var->name );
     char type_lhs[20];
     strcpy(type_lhs,TABLE[index_lhs]->var->type );
     //char type_rhs[20];
     //printf("%s\n",type_lhs );
     if(strcmp(type_lhs,"BOOLEAN") ==0)
      {
       if(strcmp(node->children[0]->children[1]->node_value,"TRUE") ==0 ||(strcmp(node->children[0]->children[1] ->node_value,"FALSE")==0))
       // strcpy(type_rhs,"BOOLEAN");
       {
        //ok
       }
       else if( strcmp(TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type, "BOOLEAN" )==0)  
        //strcpy(type_rhs,"BOOLEAN");
       {
        //ok
       }
        else
          {
           printf("TYPE CHECKING ERROR (At Line %s): IF HAS DIFFERENT TYPES OF VALUES, LHS is Boolean\n",node->children[0]->node_lineno );
         }
      }
      if(strcmp(type_lhs,"INTEGER") ==0)
      {
        if(strcmp(node->children[0]->children[1]->node_name,"INTR") ==0 )
       // strcpy(type_rhs,"BOOLEAN");
       {
        //ok
       }
       else if(TABLE[search_matching_index_in_table(node->children[0] ->children[1])] )//strcmp(TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type, "INTEGER" )==0)  
        //strcpy(type_rhs,"BOOLEAN");
       { //printf("%s\n",TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type );
        if(strcmp(TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type, "INTEGER" )==0)
        {
           //ok
        }
        else
          printf("TYPE CHECKING ERROR (At Line %s): IF HAS DIFFERENT TYPES OF VALUES, LHS is Integer\n",node->children[0]->node_lineno );
       }
        else
          {
           printf("TYPE CHECKING ERROR (At Line %s): IF HAS DIFFERENT TYPES OF VALUES, LHS is Integer\n",node->children[0]->node_lineno );
         }
      }
      if(strcmp(type_lhs,"DECIMAL") ==0)
      {
        if(strcmp(node->children[0]->children[1]->node_name,"INTR") ==0 || strcmp(node->children[0]->children[1]->node_name,"DEC") ==0)
       // strcpy(type_rhs,"BOOLEAN");
       {
        //ok
       }
       else if(TABLE[search_matching_index_in_table(node->children[0] ->children[1])] )//strcmp(TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type, "INTEGER" )==0)  
        //strcpy(type_rhs,"BOOLEAN");
       { //printf("%s\n",TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type );
        if(strcmp(TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type, "INTEGER" )==0 || strcmp(TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type, "DECIMAL" )==0)
        {
           //ok
        }
        else
          printf("TYPE CHECKING ERROR (At Line %s): IF HAS DIFFERENT TYPES OF VALUES, LHS is Decimal.\n",node->children[0]->node_lineno );
       }
        else
          {
           printf("TYPE CHECKING ERROR (At Line %s): IF HAS DIFFERENT TYPES OF VALUES, LHS is Decimal.\n",node->children[0]->node_lineno );
         }
      }
    }
    int i;
    for(i=0;i<node->n_children;i++)
    CHECKING_TYPES_OF_IF_EXPRESSION(node->children[i]);
  }
  else
  {
    int i;
    for(i=0;i<node->n_children;i++)
    CHECKING_TYPES_OF_IF_EXPRESSION(node->children[i]);
  }
}

void IS_EXPRESSION_INSIDE_WHILE_BOOLEAN(struct node* node)
{
  if(strcmp(node -> node_name,"WHILE" )==0)
  { 
    if(strcmp(node ->children[0]->node_name,"LTE")!=0 && strcmp(node ->children[0]->node_name,"LT")!=0 && strcmp(node ->children[0]->node_name,"GTE")!=0 && strcmp(node ->children[0]->node_name,"GT")!=0 && strcmp(node ->children[0]->node_name,"EQ")!=0 && strcmp(node ->children[0]->node_name,"NEQ")!=0 && strcmp(node ->children[0]->node_name,"AND")!=0 && strcmp(node ->children[0]->node_name,"OR")!=0 && strcmp(node ->children[0]->node_name,"NOT")!=0 )
      {  //printf("here2\n");
        printf("SEMANTIC ERROR(At Line %s): EXPRESSION INSIDE WHILE STATEMENT IS NOT OF THE TYPE BOOLEAN. \n", node->children[0]->node_lineno );
       }
    int i;
    for(i=0;i<node->n_children;i++)
    IS_EXPRESSION_INSIDE_WHILE_BOOLEAN(node->children[i]); 
  }
  else
  {
    int i;
    for(i=0;i<node->n_children;i++)
    IS_EXPRESSION_INSIDE_WHILE_BOOLEAN(node->children[i]); 
  }  
}

void CHECKING_TYPES_OF_WHILE_EXPRESSION(struct node* node)
{
  if(strcmp(node -> node_name,"WHILE" )==0)
  { 
    if(node-> children[0] -> n_children ==2)
    {
     int index_lhs = search_matching_index_in_table(node->children[0] ->children[0]);
     //printf("%s\n",TABLE[index_lhs]->var->name );
     char type_lhs[20];
     strcpy(type_lhs,TABLE[index_lhs]->var->type );
     //char type_rhs[20];
     //printf("%s\n",type_lhs );
     if(strcmp(type_lhs,"BOOLEAN") ==0)
      {
       if(strcmp(node->children[0]->children[1]->node_value,"TRUE") ==0 ||(strcmp(node->children[0]->children[1] ->node_value,"FALSE")==0))
       // strcpy(type_rhs,"BOOLEAN");
       {
        //ok
       }
       else if( strcmp(TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type, "BOOLEAN" )==0)  
        //strcpy(type_rhs,"BOOLEAN");
       {
        //ok
       }
        else
          {
           printf("TYPE CHECKING ERROR (At Line %s): WHILE HAS DIFFERENT TYPES OF VALUES, LHS is Boolean\n",node->children[0]->node_lineno );
         }
      }
      if(strcmp(type_lhs,"INTEGER") ==0)
      {  
        if(strcmp(node->children[0]->children[1]->node_name,"INTR") ==0 )
       // strcpy(type_rhs,"BOOLEAN");
       {
        //ok
       }
       else if(TABLE[search_matching_index_in_table(node->children[0] ->children[1])] )//strcmp(TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type, "INTEGER" )==0)  
        //strcpy(type_rhs,"BOOLEAN");
       { //printf("%s\n",TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type );
        if(strcmp(TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type, "INTEGER" )==0)
        {
           //ok
        }
        else
          printf("TYPE CHECKING ERROR (At Line %s): WHILE HAS DIFFERENT TYPES OF VALUES, LHS is Integer\n",node->children[0]->node_lineno );
       }
        else
          {
           printf("TYPE CHECKING ERROR (At Line %s): WHILE HAS DIFFERENT TYPES OF VALUES, LHS is Integer\n",node->children[0]->node_lineno );
         }
      }
      if(strcmp(type_lhs,"DECIMAL") ==0)
      {
        if(strcmp(node->children[0]->children[1]->node_name,"INTR") ==0 || strcmp(node->children[0]->children[1]->node_name,"DEC") ==0)
       // strcpy(type_rhs,"BOOLEAN");
       {
        //ok
       }
       else if(TABLE[search_matching_index_in_table(node->children[0] ->children[1])] )//strcmp(TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type, "INTEGER" )==0)  
        //strcpy(type_rhs,"BOOLEAN");
       { //printf("%s\n",TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type );
        if(strcmp(TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type, "INTEGER" )==0 || strcmp(TABLE[search_matching_index_in_table(node->children[0] ->children[1])]->var->type, "DECIMAL" )==0)
        {
           //ok
        }
        else
          printf("TYPE CHECKING ERROR (At Line %s): WHILE HAS DIFFERENT TYPES OF VALUES, LHS is Decimal.\n",node->children[0]->node_lineno );
       }
        else
          {
           printf("TYPE CHECKING ERROR (At Line %s): WHILE HAS DIFFERENT TYPES OF VALUES, LHS is Decimal.\n",node->children[0]->node_lineno );
         }
      }
    }
    int i;
    for(i=0;i<node->n_children;i++)
    CHECKING_TYPES_OF_WHILE_EXPRESSION(node->children[i]);
  }
  else
  {
    int i;
    for(i=0;i<node->n_children;i++)
    CHECKING_TYPES_OF_WHILE_EXPRESSION(node->children[i]);
  }
}

void IS_PROGRAM_HAVING_A_MAIN_FUNCTION(struct node* node)
{ 
  int flag = check_main_present_or_not(node);
  //printf(" main present %d\n",flag );
  if(flag ==0)
    printf("SEMANTIC ERROR: Cannot Run: The program should have a main function.\n");
  if (flag > 1)
    printf("SEMANTIC ERROR: More than one definition of 'main' function.\n");
}

void IS_FUNCTION_CALLED_RECURSIVELY(struct node* node)
{
  if (strcmp(node->node_name,"<func_sig>") ==0)
  {
    char func_name[20];
    strcpy(func_name, node->children[0]->node_value);
    //printf("%s\n",func_name );
    char str[20];
    strcpy(str,traverse(node));
    //printf("%s\n",str );
    if(strcmp(func_name,str) ==0)
      printf("SEMANTIC ERROR (In function at line %s): Function '%s' cannot be called recursively.\n", node->children[0]->node_lineno, func_name);

  }
  else
  {
   int i;
   for(i=0;i<node->n_children;i++)
   IS_FUNCTION_CALLED_RECURSIVELY(node->children[i]);
  } 
}

void RULES_WHILE_USING_NAME_AND_STRUCT_EQUIVALENCE(struct node* node)
{
  if(strcmp(node->node_name,"ASSIGN" )==0 )
  { 
    if( (node->children[0]->n_children == 0) && strcmp(node->children[1]->node_name,"AS") ==0 && (strcmp(node->children[1]->children[0] ->node_name,"NAME_EQU")==0  || strcmp(node->children[1]->children[0] ->node_name,"STRUCT_EQU")==0))
    {
      //printf("here\n");
      int index = search_matching_index_in_table(node->children[0]);
      if(strcmp(TABLE[index]->var->type,"BOOLEAN")!=0)
        printf("SEMANTIC ERROR(At line %s): Expected 'boolean' variable for assignment.\n", node->children[0]->node_lineno);
      int index1 = search_matching_index_in_table(node->children[1]->children[1]);
      int index2 = search_matching_index_in_table(node->children[1]->children[2]);
      if(strcmp(TABLE[index1]->var->type,"COLLECTION")!=0 || strcmp(TABLE[index2]->var->type,"COLLECTION")!=0)
        printf("SEMANTIC ERROR(At line %s): Only collections can be passed to 'Name_Equ' and 'Struct_Equ'.\n", node->children[0]->node_lineno);  

     } 
  }
  else
  {
   int i;
   for(i=0;i<node->n_children;i++)
   RULES_WHILE_USING_NAME_AND_STRUCT_EQUIVALENCE(node->children[i]); 
  }
}


void HANDLING_NON_RETURNING_FUNCTIONS(struct node* node)
{  
  if(strcmp(node->node_name,"SUBPROG") ==0 && (node->parent->parent-> n_children ==1))
  { //printf("%s\n",node->parent->parent->node_name );
    //printf("here\n");
    int return_value = search_for_return(node);
    if(return_value ==1)
      printf("SEMANTIC ERROR (At function %s): Function does not return anything, 'return' statement invalid.\n", node->parent->parent->node_name);  
  }
  else
  {
    int i;
   for(i=0;i<node->n_children;i++)
   HANDLING_NON_RETURNING_FUNCTIONS(node->children[i]); 
  }
}

void semantic_analysis_helper()
{ 
  printf("****************TYPE CHECKING & SEMANTIC ANALYZER****************\n");
  printf("****     CHECKING RULE 1     ****\n");
  IS_VARIABLE_DECLARED_BEFORE_USE();
  printf("****     CHECKING RULE 2     ****\n");
  IS_LHS_AND_RHS_OF_ASSIGN_ARE_SAME_TYPE(parse_tree);
  printf("****     CHECKING RULE 3,4   ****\n");
  IS_NUMBER_AND_TYPES_WHILE_ASSIGNING_MULTIPLE_SAME(parse_tree);
  printf("****     CHECKING RULE 5,6,7 ****\n");
  RULES_WHILE_CALLING_A_FUNCTION_AND_ASSIGNING(parse_tree);
  printf("****     CHECKING RULE 8,9   ****\n");
  RULES_WHILE_ASSIGNING_ARRAYS(parse_tree);
  printf("****     CHECKING RULE 10,11 ****\n");
  RULES_WHILE_ARRAY_SLICING(parse_tree);
  printf("****     CHECKING RULE 12    ****\n");
  IS_EXPRESSION_TYPE_AND_VARIABLE_TYPE_SAME(parse_tree);
  printf("****     CHECKING RULE 13    ****\n");
  IS_EXPRESSION_INSIDE_IF_BOOLEAN(parse_tree);
  printf("****     CHECKING RULE 14    ****\n");
  CHECKING_TYPES_OF_IF_EXPRESSION(parse_tree);
  printf("****     CHECKING RULE 15    ****\n");
  IS_EXPRESSION_INSIDE_WHILE_BOOLEAN(parse_tree);
  printf("****     CHECKING RULE 16    ****\n");
  CHECKING_TYPES_OF_WHILE_EXPRESSION(parse_tree);
  printf("****     CHECKING RULE 17    ****\n");
  IS_PROGRAM_HAVING_A_MAIN_FUNCTION(parse_tree);
  printf("****     CHECKING RULE 18    ****\n");
  IS_FUNCTION_CALLED_RECURSIVELY(parse_tree);
  printf("****     CHECKING RULE 19,20 ****\n");
  RULES_WHILE_USING_NAME_AND_STRUCT_EQUIVALENCE(parse_tree);
  printf("****     CHECKING RULE 21    ****\n");
  HANDLING_NON_RETURNING_FUNCTIONS(parse_tree);
}

