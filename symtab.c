#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SymTable SymbolTable;

//destroy useless symbol table entry 
void popSymbolTable(){
    for(int i = 0 ; i < SymbolTable.size ; i++)
    {
        if(SymbolTable.entries[i].level == SymbolTable.current_level){
            if(SymbolTable.entries[i].type!=TypeFunction && SymbolTable.entries[i].type!=TypeProcedure){
                //strcpy(SymbolTable.entries[i].name , NULL);
                printf("Destroy symbol table entry name is %s type is %d \n " , SymbolTable.entries[i].name , SymbolTable.entries[i].type);
                bzero(SymbolTable.entries[i].name ,sizeof(SymbolTable.entries[i].name));
                SymbolTable.entries[i].level = -1;
                SymbolTable.entries[i].array = NULL;
                SymbolTable.entries[i].function = NULL;
                SymbolTable.entries[i].procedure = NULL;
                SymbolTable.entries[i].type = TypeNULL;
            }
        }
    }
}
// new parameter element
struct param_list * newParam(char * s , enum StdType type){
    struct param_list * param_list = (struct param_list *)malloc(sizeof(struct param_list));
    param_list->type = type;
    strcpy(param_list->name , s);
    param_list->array = NULL;
    param_list->next_param = param_list;
    return param_list;
}
//enter parameter into parameter list
void add_param(struct param_list * root , struct param_list * child){
    struct param_list * point = root;
    while(point->next_param != point)
        point = point->next_param;
    point->next_param = child;
}
//check node type and return corresponding entry type
enum  StdType StdtypeCheck(struct node * node ){
    //printf("node type is %d\n " , node->nodeType);
    if (node->nodeType==NODE_TYPE_ARRAY)
        return TypeArray;
    else if (node->nodeType==NODE_TYPE_INT)
        return TypeInt;
    else if (node->nodeType==NODE_TYPE_REAL)
        return TypeReal;
    else if (node->nodeType==NODE_TYPE_STRING)
        return TypeString;
    else if (node->nodeType==NODE_TYPE_FUNCTION)
        return TypeFunction;
    else if(node->nodeType==NODE_TYPE_PROCEDURE)
        return TypeProcedure;
    else return TypeNULL;
}
//new array descriptor
struct array_descriptor * newArrayDes(enum StdType type){
    struct array_descriptor * array_descriptor = (struct array_descriptor*)malloc(sizeof(struct array_descriptor));
    array_descriptor->type = type;
    array_descriptor->array_begin = 0;
    array_descriptor->array_end = 0;
    array_descriptor->capacity = 0;
    array_descriptor->next_array = array_descriptor;
    return array_descriptor;
}
//new function attribute
struct function_attribute * newFunctionAttr(enum StdType type){
    struct function_attribute * function = (struct function_attribute *)malloc(sizeof(struct function_attribute));
    function->type = type;
    function->param = NULL;
    return function;
}
//new procedure attribute 
struct procedure_attribute * newProcedureAttr(){
    struct procedure_attribute * procedure = (struct procedure_attribute *)malloc(sizeof(struct procedure_attribute));
    procedure->param = NULL;    
    return procedure;
}
//extend array descriptor list 
void add_ArrayDescri_list(struct  array_descriptor * root , struct array_descriptor * child){
    struct array_descriptor * point = root;
    while(point->next_array != point)
        point = point->next_array;
    point->next_array = child;
    return ;
}
struct array_descriptor *  initArray(struct node * array_node ){

        /*initiate the array-root*/
        int start = array_node->iValue;
        array_node = array_node->rsibling;
        int end = array_node->iValue;
        array_node = array_node->rsibling;                
        enum StdType arrayType = array_node->nodeType;
        int capacity = end - start;
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
            array_desc->capacity = end - start;
            add_ArrayDescri_list(array_desc_root , array_desc);
        }
        return array_desc_root;
}
/*search the symbol table for the pattern within the same scope*/
struct SymTableEntry* findSymbol_in_scope(char *s) {
    for(int i = 0 ; i < SymbolTable.size ; i++){
        struct SymTableEntry * it = &SymbolTable.entries[i];
        if(strcmp(s, it->name) == 0 && it->level == SymbolTable.current_level) 
            {
                printf("%d , %d\n" , it->level , SymbolTable.current_level);
                return it;
            }
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
//////////////////// enter variable into a symbol table ///////////////
struct SymTableEntry* addVariable(char *s, enum StdType type , struct array_descriptor * array_desc) {
    if(findSymbol_in_scope(s) != 0) { 
        printf("Error: duplicate declaration of variable %s\n", s);
        return 0;
        //exit(0);
    }

    int index = SymbolTable.size;
    SymbolTable.size++;
    
    strcpy(SymbolTable.entries[index].name, s);
    SymbolTable.entries[index].type = type;
    SymbolTable.entries[index].level = SymbolTable.current_level;
    
    if(type == TypeArray)
        SymbolTable.entries[index].array = array_desc;
    
    printf("Enter variable %s in symbol table at level %d\n" , s , SymbolTable.entries[index].level);
    return &SymbolTable.entries[index];
}
//////////////////// enter function into a symbol table ///////////////
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
    
    printf("Enter function %s in symbol table at level %d\n" , s , SymbolTable.entries[index].level);
    return &SymbolTable.entries[index];
}
//////////////////// enter procedure into a symbol table //////////////
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
    
    printf("Enter procedure %s in symbol table at level %d\n" , s , SymbolTable.entries[index].level);
    return &SymbolTable.entries[index];
}
/////////////////// find and return nth child in an node ////////////////
struct node* nthChild(int n, struct node *node) {
    struct node *child = node->child;
    for(int i=1; i<n; i++) {
        child = child->rsibling;
    }
    return child;
}
////////////////// initiate argument(s) of function procedure //////////////////
struct param_list * initParam(struct node * parameter ){
    struct node * typeList = parameter->child;//node decls                
    struct node * idList = typeList->child;//node list
    struct node * index = idList;
    struct param_list * param_root = newParam(idList->string , StdtypeCheck(typeList));                
    typeList = typeList->rsibling;
    if(param_root->type == TypeArray){
        struct array_descriptor * array_desc_root = initArray(typeList->child);
        param_root->array = array_desc_root;
    }
    
    idList = idList->rsibling;
    while(typeList != parameter->child){//to be continued
        enum StdType type = StdtypeCheck(typeList);
        while(idList != index){
            struct param_list * nextParam = newParam(idList->string , type);//to be added
            idList = idList->rsibling;
            if(nextParam->type == TypeArray){
                struct array_descriptor * array_desc_root = initArray(typeList->child);
                nextParam->array = array_desc_root;
            }
            add_param(param_root , nextParam);                        
        }
        typeList = typeList->rsibling;
        if(typeList == parameter->child)
            break;
        idList = typeList->child;
        index = idList;
        typeList = typeList->rsibling;
    }
    return param_root;
}
void semanticCheck(struct node *node) {
    printf("%d\n" , node->nodeType);
    switch(node->nodeType) {
        /*implement scope increase*/

        /************Function declaration*************/
        case NODE_FUN_HEAD: { 
            struct node * function_name = nthChild(1 , node);
            struct node * parameter = nthChild(2 , node);
            struct node * typeNode = nthChild(3 , node);
            
            /***********check function redeclaration*************/
            struct SymTableEntry * entry ;
            if((entry=findSymbol_in_global(function_name->string))!=0 && entry->type == TypeFunction)
                printf("Error: duplicate declaration of function %s\n" , function_name->string);
                
            struct function_attribute * function_attribute_root = newFunctionAttr(StdtypeCheck(typeNode));
            

            /***********check whether it has parameter**********/
            if(parameter->nodeType != NODE_lambda)
                function_attribute_root->param = initParam(parameter);

            /*******enter function entry into symbol table ************/
            //entry functin_name , functiontype , function_attribtue 
            addFunction(function_name->string , TypeFunction , function_attribute_root);
            SymbolTable.current_level++;
            printf("New scope created \n");                        
            break;
        }
        /************Procedure declaration***********/
        case NODE_PRO_HEAD: { 
            struct node * procedure_name = nthChild(1 , node);
            struct node * parameter = nthChild(2 , node);

            /* check procedure of redeclared problem*/
            struct SymTableEntry * entry ;
            if((entry=findSymbol_in_global(procedure_name->string))!=0 && entry->type == TypeProcedure)
                printf("redeclared procedure %s\n" , procedure_name->string);

            struct procedure_attribute * procedure_attribute_root = newProcedureAttr();

            //////////////  check whether it has parameter //////////////////////
            if(parameter->nodeType != NODE_lambda)
                procedure_attribute_root->param = initParam(parameter);
            
            enum StdType valueType = TypeProcedure;

            addProcedure(procedure_name->string , valueType , procedure_attribute_root);
            SymbolTable.current_level++;
            printf("New scope created \n");            
            break;
        }
        /************Close Scope********************/
        case NODE_END:{
            popSymbolTable();
            SymbolTable.current_level--;
            printf("Scope deleted \n");                        
            return;
        }
        /*************** Variable declaration ***********/
        case NODE_DECL: {
        ///////////// empty declaration /////////////          
            if(node->child == NULL)
                    return;
        ////////////declaration ////////////////    
            else {
                struct node * check_declaration = node->child;
                int check_type = 2;
                int check_node = 1;
                do{
                        struct node *typeNode = nthChild(check_type, node);//node type
                        enum StdType valueType = StdtypeCheck(typeNode);
                        /********* Array variable declaration implement here******************/
                        if(valueType == TypeArray){
                            struct node * array_node = typeNode->child;
                            struct array_descriptor * array_desc_root = initArray(array_node);
                            struct node *idList = nthChild(check_node, node);
                            struct node *idNode = idList->child;
                            do {
                                addVariable(idNode->string, valueType , array_desc_root);
                                idNode = idNode->rsibling;
                            } while(idNode != idList->child);                          
                        }
                        /*********int , string , real variable declaration implement here***********/
                        else {
                            struct node *idList = nthChild(check_node, node);
                            struct node *idNode = idList->child;
                            do {
                                addVariable(idNode->string, valueType , 0);
                                idNode = idNode->rsibling;
                            } while(idNode != idList->child);
                        }
                        check_declaration = typeNode->rsibling;
                        check_node+=2;
                        check_type+=2;
                }while(check_declaration != node->child);
                return ;
            }
        }

        /* This case is simplified, actually you should check
           the symbol is a variable or a function with no parameter */
        //case NODE_VAR_OR_PROC: 
        /********function or procedure  arguments should also be checked here*********/ 
        case NODE_SYM_REF: { 
            struct SymTableEntry *entry = findSymbol_in_global(node->string);
            printf("identifier is %s\n" , node->string);
            if(entry == 0) {
                printf("current_level is %d \n" , SymbolTable.current_level);
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
        case NODE_OP:{
                    struct node * child1 = nthChild(1 , node);
                    struct node * child2 = nthChild(2 , node);
                    if(child1 != child2){//binary op 
                        semanticCheck(child1);
                        semanticCheck(child2);
                    }
                    else 
                        semanticCheck(child1);//unary op
                    
                    /////////////////left child type is TypeArray////////////
                    if(child1->valueType == TypeArray){
                        struct array_descriptor * array_element = child1->array;
                        if(child1->child->child == NULL)
                            printf("type errors in arithmetic expression\n");
                        else{
                            struct node * idx = child1->child->child;// "["
                            idx = idx->rsibling;// "digseq"
                            while(idx!=child1->child->child){
                                if(idx->valueType != TypeInt){
                                    printf("type in [] is not integer in this arithmetic expression\n");
                                    exit(0);
                                }
                                if(idx->iValue >array_element->array_end || idx->iValue < array_element->array_begin){
                                    printf("Point  out of range  \n");
                                    exit(0);
                                }
                                idx = idx->rsibling;//"]"
                                idx = idx->rsibling;//"[" or the first "["
                                if(idx == child1->child->child)
                                    break;
                                idx = idx->rsibling;//"digseq"
                                if(array_element == array_element->next_array){
                                    printf("Mismatch of array type\n");
                                    exit(0);
                                    break;
                                }
                                array_element = array_element->next_array;
                            }
                            if(array_element->type == TypeArray){
                                child1->array = array_element;
                            }
                            else {
                                child1->valueType = array_element->type;
                                child1->array = NULL;
                            }
                        }

                    }


                    if(child1->valueType == child2->valueType)
                        node->valueType = child1->valueType;
                    else {
                        printf("type errors in arithmetic expression\n");
                    }
                    return ;
        }

        /*************** If statement check is implemented here ********************/
        case NODE_IF:{
            struct node * expr = nthChild(1 , node);
            struct node * statement1 = nthChild(2 , node);
            struct node * statement2 = nthChild(3 , node);
            semanticCheck(expr);
            semanticCheck(statement1);
            semanticCheck(statement2);
            if(expr->valueType != TypeInt){
                printf("expression in If is not a right type\n");
                exit(0);
            }
            return;
        }
        /*************** while statement check is implemented here *****************/
        case NODE_WHILE:{
            struct node * expr = nthChild(1 , node);
            struct node * statement = nthChild(2 , node);
            semanticCheck(expr);
            semanticCheck(statement);
            if(expr->valueType != TypeInt){
                printf("expression in while is not a right type\n");
                exit(0);
            }
            return;
        }
        /*************** digseq facto check is implemented here*********/
        case NODE_INT:{
            node->valueType = TypeInt;
            return;
        }
        case NODE_REAL:{
            node->valueType = TypeReal;
            return;
        }
        case NODE_STRING_v:{
            node->valueType = TypeString;
            return;
        } 
        case NODE_ASSIGN_STMT: {
            struct node *child1 = nthChild(1, node);
            struct node *child2 = nthChild(2, node);
            semanticCheck(child1);
            semanticCheck(child2);

            //////////procedure use in the assignment ///////////
            if(child1->valueType == TypeProcedure || child2->valueType == TypeProcedure)
                printf("Miss use of procedure\n");
  
            if(child1->valueType==TypeFunction )//&& child1->function->type == child2->valueType)
            {
                printf("Function placed in LHS , maybe it should be placed in RHS\n");
                return;
            }


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

