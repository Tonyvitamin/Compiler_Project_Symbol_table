#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SymTable SymbolTable;

//new array descriptor
struct array_descriptor * newArrayDes(enum StdType type){
    struct array_descriptor * array_descriptor = (struct array_descriptor*)malloc(sizeof(struct array_descriptor));
    array_descriptor->type = type;
    array_descriptor->array_begin = 0;
    array_descriptor->array_end = 0;
    array_descriptor->next_array = array_descriptor;
    return array_descriptor;
}
//new function attribute
struct function_attribute * newFunctionAttr(enum StdType type){
    struct function_attribute * function = (struct function_attribute *)malloc(sizeof(struct function_attribute));
    
}
//extend array descriptor list 
void add_ArrayDescri_list(struct  array_descriptor * root , struct array_descriptor * child){
    struct array_descriptor * point = root;
    while(point->next_array != point)
        point = point->next_array;
    point->next_array = child;
    return ;
}

/*search the symbol table for the pattern within the same scope*/
struct SymTableEntry* findSymbol_in_scope(char *s) {
    for(int i = 0 ; i < SymbolTable.size ; i++){
        struct SymTableEntry * it = &SymbolTable.entries[i];
        if(strcmp(s, it->name) == 0 && it->level == SymbolTable.current_level) 
            return it;
    }
    return 0;
}
/*search the symbol table for the variable within the global scope*/
struct SymTableEntry* findSymbol_in_global(char *s) {
    for(int i = SymbolTable.size ; i >= 0 ; i--){
        struct SymTableEntry * it = &SymbolTable.entries[i];
        if(strcmp(s, it->name) == 0 && it->level <= SymbolTable.current_level) 
            return it;
    }
    return 0;
}
//enter variable into a symbol table
struct SymTableEntry* addVariable(char *s, enum StdType type , struct array_descriptor * array_desc) {
    if(findSymbol_in_scope(s) != 0) { 
        printf("Error: duplicate declaration of variable %s\n", s);
        exit(0);
    }

    int index = SymbolTable.size;
    SymbolTable.size++;
    
    strcpy(SymbolTable.entries[index].name, s);
    SymbolTable.entries[index].type = type;
    SymbolTable.entries[index].level = SymbolTable.current_level;
    
    if(type == TypeArray)
        SymbolTable.entries[index].array = array_desc;
    
    printf("Enter variable %s in symbol table\n" , s);
    return &SymbolTable.entries[index];
}

//enter function into a symbol table
struct SymTableEntry* addFunction(char *s, enum StdType type , struct function_attribute * function) {
    if(findSymbol_in_global(s) != 0) { 
        printf("Error: duplicate declaration of function %s\n", s);
        exit(0);
    }

    int index = SymbolTable.size;
    SymbolTable.size++;
    
    strcpy(SymbolTable.entries[index].name, s);
    SymbolTable.entries[index].type = type;
    SymbolTable.entries[index].level = SymbolTable.current_level;
    SymbolTable.entries[index].function = function;
    
    printf("Enter function %s in symbol table\n" , s);
    return &SymbolTable.entries[index];
}

//enter procedure into a symbol table
struct SymTableEntry* addProcedure(char *s, enum StdType type , struct procedure_attribute * procedure) {
    if(findSymbol_in_global(s) != 0) { 
        printf("Error: duplicate declaration of procedure %s\n", s);
        exit(0);
    }

    int index = SymbolTable.size;
    SymbolTable.size++;
    
    strcpy(SymbolTable.entries[index].name, s);
    SymbolTable.entries[index].type = type;
    SymbolTable.entries[index].level = SymbolTable.current_level;
    SymbolTable.entries[index].procedure = procedure;
    
    printf("Enter %s in symbol table\n" , s);
    return &SymbolTable.entries[index];
}
//find and return nth child in an node
struct node* nthChild(int n, struct node *node) {
    struct node *child = node->child;
    for(int i=1; i<n; i++) {
        child = child->rsibling;
    }
    return child;
}

void semanticCheck(struct node *node) {
    printf("%d\n" , node->nodeType);
    switch(node->nodeType) {
        /*implement scope increase*/
        case NODE_BEGIN: { //printf("here\n");
            SymbolTable.current_level++;
            printf("%d %d begin works\n" , node->nodeType , NODE_FUN_HEAD);
        }
    
        case NODE_FUN_HEAD: { printf("here\n");printf("%d %d function works\n" , node->nodeType , NODE_FUN_HEAD);
            SymbolTable.current_level++;
            struct node * function_name = nthChild(1 , node);
            struct node * typeNode = nthChild(3 , node);
            enum StdType valueType;
            if(typeNode->nodeType == NODE_TYPE_INT)
                valueType = TypeInt;
            else if(typeNode->nodeType == NODE_TYPE_REAL)
                valueType = TypeReal;
            else if(typeNode->nodeType == NODE_TYPE_STRING)
                valueType = TypeString;
            addVariable(function_name->string , valueType , 0);
            SymbolTable.current_level--;
        }
        case NODE_PRO_HEAD: { //printf("here\n");
            SymbolTable.current_level++;
            SymbolTable.current_level--;
        }
        case NODE_END:{//printf("here\n");
            SymbolTable.current_level--;
            return;
        }
        case NODE_DECL: { //printf("here\n");//var declarations and should perform scope check
            /* We only implement integer and real type here,
               you should implement array type by yourself */

            struct node *typeNode = nthChild(2, node);//node type
            enum StdType valueType;

            if(typeNode->nodeType == NODE_TYPE_INT)
                valueType = TypeInt;
            else if(typeNode->nodeType == NODE_TYPE_REAL)
                valueType = TypeReal;
            else if(typeNode->nodeType == NODE_TYPE_STRING)
                valueType = TypeString;
            else if(typeNode->nodeType == NODE_TYPE_ARRAY){//implement array type here
                valueType = TypeArray;
                struct node * array_node = typeNode->child;

                int start = array_node->iValue;
                array_node = array_node->rsibling;
                int end = array_node->iValue;
                array_node = array_node->rsibling;                
                enum StdType arrayType = array_node->nodeType;
                int capacity = end -start;
                struct array_descriptor * array_desc_root = newArrayDes(arrayType);
                array_desc_root->array_begin = start;
                array_desc_root->array_end = end;
                array_desc_root->capacity = capacity;

                //create array descriptor link list
                while(array_node->nodeType == TypeArray)
                {
                    array_node = array_node->child;
                    
                    int start = array_node->iValue;
                    array_node = array_node->rsibling;
                    int end = array_node->iValue;
                    array_node = array_node->rsibling;                
                    enum StdType arrayType = array_node->nodeType;
    
                    struct array_descriptor * array_desc = newArrayDes(arrayType);
                    array_desc->array_begin = start;
                    array_desc->array_end = end;
                    add_ArrayDescri_list(array_desc_root , array_desc);
                }
                struct node *idList = nthChild(1, node);
                struct node *idNode = idList->child;
                do {
                    addVariable(idNode->string, valueType , array_desc_root);
                    idNode = idNode->rsibling;
                } while(idNode != idList->child);
                return;
            }

            //int real string handle
            struct node *idList = nthChild(1, node);
            struct node *idNode = idList->child;
            do {
                addVariable(idNode->string, valueType , 0);
                idNode = idNode->rsibling;
            } while(idNode != idList->child);

            return;
        }

        /* This case is simplified, actually you should check
           the symbol is a variable or a function with no parameter */
        //case NODE_VAR_OR_PROC: 
        case NODE_SYM_REF: {//printf("where\n");
        //printf("here\n");
            struct SymTableEntry *entry = findSymbol_in_global(node->string);
            //printf("where\n");
            if(entry == 0) {
                printf("Error: undeclared variable %s\n", node->string);
                exit(0);
            }

            node->entry = entry;
            node->valueType = entry->type;
            if(entry->type == TypeProcedure) {
                node->procedure = entry->procedure;
            }
            else if (entry->type == TypeFunction) {
                node->function = entry->function;
            }
            else if(entry->type == TypeArray) {
                node->array = entry->array;
            }
            return;
        }


        /* Only implemented binary op here, you should implement unary op */
        case NODE_OP:{//printf("here\n");
                    struct node * child1 = nthChild(1 , node);
                    struct node * child2 = nthChild(2 , node);
                    if(child1 != child2){//binary op 
                        semanticCheck(child1);
                        semanticCheck(child2);
                    }
                    else 
                        semanticCheck(child1);//unary op

                    if(child1->valueType == child2->valueType)
                        node->valueType = child1->valueType;
                    else {
                        printf("type errors in arithmetic expression\n");
                    }
        }


        case NODE_INT:{//printf("here\n");
            node->valueType = TypeInt;
            return;
        }
        case NODE_REAL:{//printf("here\n");
            node->valueType = TypeReal;
            return;
        }
        case NODE_STRING_v:{//printf("here\n");
            node->valueType = TypeString;
            return;
        }
        /* You should check the LHS of assign stmt is assignable
           You should also report error if LHS is a function with no parameter 
           (function is not implemented in this sample, you should implement it) */ 
        case NODE_ASSIGN_STMT: {//printf("here\n");
            struct node *child1 = nthChild(1, node);
            struct node *child2 = nthChild(2, node);
            semanticCheck(child1);
            semanticCheck(child2);




            /* We only implement the checking for integer and real types
               you should implement the checking for array type by yourself */
            if(child1->valueType != child2->valueType) {
                if(node->nodeType == NODE_OP)
                    printf("Error: type mismatch for operator\n");
                else
                    printf("Error: type mismatch for assignment\n");
                exit(0);
            }

            node->valueType = child1->valueType;

            return;
        }

    }
    

    /* Default action for other nodes not listed in the switch-case */
    struct node *child = node->child;


    if(child != NULL) {
        do {
            semanticCheck(child);
            child = child->rsibling;
        } while(child != node->child);
    }
}

