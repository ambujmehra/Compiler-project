#ifndef _SYMBOLTABLEDEF_H_
#define _SYMBOLTABLEDEF_H_

#define TABLE_SIZE 200
int hash;

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
  int ret;
  struct par_list* parameter_list;
  int param;

};

struct HashTableNode
{
  struct funcNode* func;
  struct varNode* var;
  struct HashTableNode* next;
};


struct HashTableNode* TABLE[200];

#endif
