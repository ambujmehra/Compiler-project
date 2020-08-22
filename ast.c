
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parserDef.h" 
#include "lexerDef.h"
#include "astDef.h"



void merge_node(struct node* current)
{

  
  int i,j;
  if(current == NULL)
  {
    return;
  }
  else
  {  
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
            current->parent->children[i] = current->children[0];
            (current->children[0])->parent = current->parent;
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
            create_ast(temp2);

            for(j=0;j<a;j++)
            {
            //    printf("%s\n",temp2->children[j]->node_name);     
                create_ast(temp2->children[j]);
            }
        //    free(current);
              break;
            }
          }
         // create_ast(current->parent);
      
      }

    }
  
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
      current->parent->children[0] = temp;
      temp->parent = current->parent;
      temp->n_children = current->n_children-1;

      for(i=0;i<temp->n_children;i++)
      {
          temp->children[i] = current->children[i+1];
          current->children[i+1]->parent = temp;
          create_ast(temp->children[i]);
      }
    }
    if(strcmp(current->node_name,"<smts_&_func_defs>") == 0 || strcmp(current->node_name,"<more_func_defs>") == 0)
    { 
      for(i=0;i<current->n_children;i++)
      {
          create_ast(current->children[i]);
      }
      
    //  create_ast(temp1);
      
      

      
      //free(current);
    }
    
    
    if(strcmp(current->node_name,"INTEGER") == 0 || strcmp(current->node_name,"BOOLEAN") == 0 || strcmp(current->node_name,"MULTIPLE") == 0 || strcmp(current->node_name,"DECIMAL") == 0 || strcmp(current->node_name,"STRING") == 0 )
    { 
      for(i=0;i<current->n_children;i++)
      {
          //printf("****  %s\n",current->children[i]->node_name);
          create_ast(current->children[i]);
      }

      
    }
    if(strcmp(current->node_name,"<variables>") == 0)
    {
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
    if(strcmp(current->node_name,"IDEN") == 0 || strcmp(current->node_name,"INTR") == 0 || strcmp(current->node_name,"DEC") == 0)
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
void ast_helper(){
   

    merge_node(parse_tree);
    create_ast(parse_tree);
    printf("*******ABSTRACT SYNTAX TREE*********\n");
    print_tree(parse_tree);
}

