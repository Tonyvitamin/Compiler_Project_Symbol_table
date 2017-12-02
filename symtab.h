#ifndef __SYMTAB_H___
#define __SYMTAB_H___

enum StdType {TypeInt, TypeReal , TypeString};

struct SymTableEntry {
    char name[100];
    int scope;
    int links;
    enum StdType type;
};

struct SymTable {
    int size;
    struct SymTableEntry entries[100];
};

extern struct SymTable SymbolTable;

#include "node-type.h"
void semanticCheck(struct node* node);

#endif

