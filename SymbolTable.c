#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parserDef.h"  
#include "lexerDef.h"
#include "SymbolTableDef.h"

char str1[50] = "<more_func_defs>";

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

void createHashTable(struct node* node,int curr_scope)
{
  int i,j;
  if(node == NULL)
    return;
  else
  {
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

  else if(strcmp(node->node_name,"<func_sig>") == 0)
  {
          curr_scope++;
          hash = hash_func(node->children[0]->node_value);
          struct funcNode* f = (struct funcNode*) malloc(sizeof(struct funcNode));
          f->scope = curr_scope;
          strcpy(f->dec_line,node->children[0]->node_lineno);
          strcpy(f->name,node->children[0]->node_value);
          f->return_list = NULL;
          f->parameter_list = NULL;
          
          TABLE[hash] =(struct HashTableNode*) malloc(sizeof(struct HashTableNode));
          TABLE[hash]->func = f;
          TABLE[hash]->var = NULL;
          TABLE[hash]->next = NULL;
          TABLE[hash]->func->ret = 1;
          TABLE[hash]->func->param = 1;
      for(i=1;i<node->n_children;i++)
      {
       // printf("Hello %s\n",node->children[i]->node_name);
        
        if(strcmp(node->children[i]->node_name,"<remaining_list>") == 0)
        {
            if(TABLE[hash]->func->parameter_list == NULL)
            {
              TABLE[hash]->func->parameter_list = (struct par_list*)malloc(sizeof(struct par_list));
              strcpy(TABLE[hash]->func->parameter_list->type,node->children[i]->children[0]->node_name);
              strcpy(TABLE[hash]->func->parameter_list->name,node->children[i]->children[1]->node_value);
              TABLE[hash]->func->parameter_list->next = NULL;
            }
        }
      }

      for(i=0;i<node->n_children;i++)
      {
          createHashTable(node->children[i],curr_scope);
      }
      


  }
  

   else if(strcmp(node->node_name,"IDEN") == 0)
    {
      hash = hash_func(node->node_value);
     

      if(TABLE[hash])
      {
          if(TABLE[hash]->var)
         {
            if(TABLE[hash]->var->referenced_line)
            {
                struct ref_line* temp;
                temp = TABLE[hash]->var->referenced_line;
                while(true)
                {
                  if(temp->next != NULL)
                  {
                    temp = temp->next;
                  }
                  else
                  {
                    break;
                  }
                }

                temp->next = (struct ref_line*) malloc(sizeof(struct ref_line));
                strcpy(temp->next->data,node->node_lineno);
                temp->next->next = NULL;
            }
            else
            {

              if(!TABLE[hash]->func)
              {


              TABLE[hash]->var->referenced_line = (struct ref_line*) malloc(sizeof(struct ref_line));
              strcpy(TABLE[hash]->var->referenced_line->data,node->node_lineno);
              TABLE[hash]->var->referenced_line->next = NULL;
              }
            }
         }
         
      }
      else
      {
          struct varNode* v = (struct varNode*) malloc(sizeof(struct varNode));
          v->scope = curr_scope;
          strcpy(v->dec_line,node->node_lineno);
          strcpy(v->name,node->node_value);

	  struct node* temp = node->parent;
	  printf("%s\n",temp->node_name);
		if(strcmp(temp->node_name,"AS") == 0)
		{
			for(i=0;i<temp->n_children;i++)
			{
				if(temp->children[i] == node)
				{
					if(temp->children[i-1])
					{
						if(strcmp(temp->children[i]->node_name,"IDEN") == 0)
						{
						          strcpy(v->type,"COLLECTION");

						}
					}
				}
			}
		}
		else
		{
			strcpy(v->type,node->node_name);
		}

          TABLE[hash] =(struct HashTableNode*) malloc(sizeof(struct HashTableNode));
          TABLE[hash]->func = NULL;
          TABLE[hash]->var = v;
          TABLE[hash]->next = NULL;
      }

      if(node->children[0])
      {
        createHashTable(node->children[0],curr_scope);
      }

    }
    else if(strcmp(node->node_name,"INTEGER") == 0 || strcmp(node->node_name,"BOOLEAN") == 0 || strcmp(node->node_name,"COLLECTION") == 0 || strcmp(node->node_name,"STRING") == 0 || strcmp(node->node_name,"CHARACTER") == 0 || strcmp(node->node_name,"DECIMAL") == 0)
    {

      for(i=0;i<node->n_children;i++)
      { 
        if(strcmp(node->children[i]->node_name,"IDEN") == 0)
        {
               hash = hash_func(node->children[i]->node_value);
             //  printf("%d\n",hash);
          
        
          struct varNode* v = (struct varNode*) malloc(sizeof(struct varNode));
          v->scope = curr_scope;
          strcpy(v->dec_line,node->children[i]->node_lineno);
          strcpy(v->name,node->children[i]->node_value);
          strcpy(v->type,node->node_name); 
         
            if(TABLE[hash])
            {
              struct HashTableNode* temp;
              temp = TABLE[hash];
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



void SymbolTable_helper()
{
  createHashTable(parse_tree,0);
    int i;

    printf("**********************************Symbol Table*****************************\n");

    
    for(i=0;i<TABLE_SIZE;i++)
     {
        if(TABLE[i])
        {
         if(TABLE[i]->var)
          {
	           if(strcmp(TABLE[i]->var->name,str1))
             {
                printf("%s %s %s ",TABLE[i]->var->name,TABLE[i]->var->dec_line,TABLE[i]->var->type);
                struct ref_line* temp = TABLE[i]->var->referenced_line;
                while(temp)
                {
                  printf("%s ",temp->data);
                  temp = temp->next;
                }
                printf("\n");
             }
            if(TABLE[i]->next)
            {
              printf("%s %s\n",TABLE[i]->next->var->name,TABLE[i]->next->var->dec_line);
            }
          }
          if(TABLE[i]->func)
	 {
	    if(strcmp(TABLE[i]->func->name,str1))
          if(TABLE[i]->func->parameter_list)
          {
            printf("%s %s %s %s\n",TABLE[i]->func->name,TABLE[i]->func->dec_line,TABLE[i]->func->parameter_list->type,TABLE[i]->func->parameter_list->name);
          }
	  else
	  {
		printf("%s %s\n",TABLE[i]->func->name,TABLE[i]->func->dec_line);
	  }

          }
        }
     }

}
