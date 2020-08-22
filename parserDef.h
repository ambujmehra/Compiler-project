# ifndef PARSERDEFH_
#define PARSERDEFH_

#define NUM_TOTAL_RULES 130
#define NUM_NON_TERM 59
#define NUM_TERM 68
#define MAX 500
 

struct grammar {
  int rule_no;           //rule number in grammar
  char rule[105];        // the actual rule   
  char *rule_lhs;        // lhs of the rule.
  char *rule_rhs;        // rhs of rule  
};
struct nonterminals {
   int nt_no;
   char nt_name[35];
};
struct terminals {
   int t_no;
   char t_name[35];
};
struct StackNode
{
    char data[50];
    struct StackNode* next;
};
struct node
{   struct node *parent;
    int n_children;
   char node_name[30];
   char node_lineno[5];
   char node_value[40];
   struct node *children[20];
}; 
struct tokens{
    char token_full[100];
    char token_name[20];
    char token_value[20];
    char line_no[5];
};


struct node* parse_tree_root;
struct node* parse_tree;
struct node* ast;

# endif
