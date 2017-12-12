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

//int  check = 1;

enum StdType {TypeInt, TypeReal , TypeString , TypeArray , TypeFunction , TypeProcedure , TypeNULL};

extern int current_level;

struct param_list{
    char name[30];
    enum StdType type;
    struct array_descriptor * array;
    struct param_list * next_param;
};

struct array_descriptor{
    enum  StdType type;
    int array_begin;
    int array_end;
    int capacity;
    int iValue;
    double rValue;
    char * string;
    struct array_descriptor * next_array;
};

struct function_attribute{
    enum StdType type;
    struct param_list * param;
};
struct procedure_attribute{
    struct param_list * param;
};

struct SymTableEntry {
    char name[100];
    int iValue;
    char * string;
    double rValue;
    int int_array[100][100][100][100][100];
    double real_array[100][100][100][100][100];
    //string type to be added
    int level;
    enum StdType type;
    struct procedure_attribute * procedure;
    struct function_attribute * function ;
    struct array_descriptor * array;
};

struct SymTable {
    int size;
    int current_level;
    int capacity;
    struct SymTableEntry entries[1024];
};

extern struct SymTable SymbolTable;
#include "node-type.h"
void semanticCheck(struct node* node);
void printf_symbol_table();
#endif

