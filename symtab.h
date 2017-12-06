#ifndef __SYMTAB_H___
#define __SYMTAB_H___


/*
Every Symbol table's first entry is the name(function , procedure , ) for the scope or it will be an local compound statement

*/

#define type_variable 0
#define type_function 1
#define type_procedure 2
#define type_program 3


#define scope_domain 0
#define global_domain 1
#define while_domain 2

enum StdType {TypeInt, TypeReal , TypeString , TypeArray , TypeFunction , TypeProcedure};

extern int current_level;

struct array_descriptor{
    enum  StdType type;
    int array_begin;
    int array_end;
    struct array_descriptor * array_child;
};
struct function_attribute{
    enum StdType type;
    int arguments 
};


struct SymTableEntry {
    char name[100];
    char kind[20];
    int level;

    struct function_attribute * function;
    enum StdType type;
    struct array_descriptor * array;
};

struct SymTable {
    int size;
    int current_level;
    int capacity;
    struct SymTableEntry entries[1024];
};

extern struct SymTable SymbolTable;
//extern struct SymTable SymbolTable[200];
#include "node-type.h"
void semanticCheck(struct node* node);

#endif

