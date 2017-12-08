#ifndef __NODE_H__
#define __NODE_H__

struct node;

#define VALUE_INVALID   0
#define VALUE_I_VALID   1
#define VALUE_R_VALID   2

#define NODE_TOKEN          1
#define NODE_OP             2

//varible or direct value ex: 13, 13.34, "apple"
#define NODE_INT            3
#define NODE_REAL           4
#define NODE_STRING_v       5

#define NODE_PROGRAM        6
#define NODE_DECL           7
#define NODE_SUB_DECLS      8

//type declarations
#define NODE_TYPE_INT       9
#define NODE_TYPE_REAL      10
#define NODE_TYPE_STRING    11
#define NODE_TYPE_ARRAY     12
#define NODE_TYPE_FUNCTION  13
#define NODE_TYPE_PROCEDURE 14


#define NODE_ASSIGN_STMT    15
#define NODE_SYM_REF        16

#define NODE_VARIABLE       17
#define NODE_lambda         18

//subprogram_declarations
#define NODE_FUN_HEAD       19
#define NODE_PRO_HEAD       20

#define NODE_LIST           50
#define NODE_OPT_VAR        51

//statement
#define NODE_IF             52
#define NODE_WHILE          53
#define NODE_BEGIN          54
#define NODE_END            55
#define NODE_FUN_PRO_REF    56

#define NODE_VAR_OR_PROC    99
#define NODE_ERROR          100
#define NODE_EMPTY          101


//operation node 
#define OP_ADD  1
#define OP_SUB  2
#define OP_MUL  3
#define OP_DIV  4
#define OP_GT   5
#define OP_LT   6
#define OP_EQ   7
#define OP_GE   8
#define OP_LE   9
#define OP_NE   10
#define OP_NOT  11
#define OP_LBRAC 12
#define OP_RBRAC 13


  
#include "symtab.h"
struct node {
    int nodeType;
    struct node *parent;
    struct node *child;
    struct node *lsibling;
    struct node *rsibling;

    /* Attribute for NODE_TOKEN */
    int tokenType;


    /* Values for general use */
    int iValue;
    double rValue;
    char valueValid;
    char *string;
    
    /* Indicates which OP */
    char op;
    
    /*attribute and symbol table referred entry */
    struct array_descriptor * array;
    struct function_attribute * function;
    struct procedure_attribute * procedure;
    enum StdType valueType;
    struct SymTableEntry *entry;

};

struct node * newNode(int type);
void deleteNode(struct node * node);
void addChild(struct node * node, struct node * child);
void printTree(struct node * node, int ident);

#endif

