#ifndef __SYMTAB_H___
#define __SYMTAB_H___

enum StdType {TypeInt, TypeReal , TypeString , TypeArray};

extern int current_SymbolTable;
struct array_descriptor{
    enum  StdType type;
    int array_begin;
    int array_end;
    struct array_descriptor * array_child;
};

struct SymTableEntry {
    char name[100];
    int scope;
    int links;
    enum StdType type;
    struct array_descriptor * array;
};

struct SymTable {
    int size;
    struct SymTableEntry entries[100];
};

extern struct SymTable SymbolTable;
//extern struct SymTable SymbolTable[200];
#include "node-type.h"
void semanticCheck(struct node* node);

#endif

