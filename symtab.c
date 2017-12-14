#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SymTable SymbolTable;
int check =1;
int scope_check = 0;
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
        enum StdType arrayType = StdtypeCheck(array_node);//array_node->nodeType;
        int capacity = end - start;
        struct array_descriptor * array_desc_root = newArrayDes(arrayType);
        array_desc_root->array_begin = start;
        array_desc_root->array_end = end;
        array_desc_root->capacity = capacity;

        //create array descriptor link list
        while(array_node->nodeType == NODE_TYPE_ARRAY)
        {
            array_node = array_node->child;
            int start = array_node->iValue;
            array_node = array_node->rsibling;
            int end = array_node->iValue;
            array_node = array_node->rsibling;                
            enum StdType arrayType = StdtypeCheck(array_node);

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
    int past = 1024;
    for(int i =  SymbolTable.size-1 ; i >= 0 ; i--){
        struct SymTableEntry * it = &SymbolTable.entries[i];
        if(it->level > past)
            return 0;
        if(strcmp(s, it->name) == 0 && it->level == SymbolTable.current_level){
                //printf("%d , %d\n" , it->level , SymbolTable.current_level);
                return it;
        }
        past = it->level;        
    }
    return 0;
}
struct SymTableEntry* findSymbol_fun_pro_var(char *s) {
    for(int i = 0 ; i < SymbolTable.size  ; i++){
        struct SymTableEntry * it = &SymbolTable.entries[i];
        if(strcmp(s, it->name) == 0 && it->level == 0){
                //printf("%d , %d\n" , it->level , SymbolTable.current_level);
                return it;
        }
    }
    return 0;
}
/*search the symbol table for the variable within the global scope*/
struct SymTableEntry* findSymbol_in_global(char *s) {
    int past = 1024;
    for(int i =  SymbolTable.size-1 ; i >= 0 ; i--){
        struct SymTableEntry * it = &SymbolTable.entries[i];
        if(it->level > past)
            break;
        if(strcmp(s, it->name) == 0 && it->level <= 1){
                //printf("%d , %d\n" , it->level , SymbolTable.current_level);
                return it;
        }
        past = it->level;        
    }
    for(int i = 0 ; i < SymbolTable.size  ; i++){
        struct SymTableEntry * it = &SymbolTable.entries[i];
        if(strcmp(s, it->name) == 0 && it->level == 0){
                //printf("%d , %d\n" , it->level , SymbolTable.current_level);
                return it;
        }
    }
    return 0;
}
//////////////////// enter variable into a symbol table ///////////////
struct SymTableEntry* addVariable(char *s, enum StdType type , struct array_descriptor * array_desc) {

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
    struct node * typeList = parameter->child;// "identifier_list" type                
    struct node * idList = typeList->child;// "identifier" "identifier"
    struct node * index = idList;
    
    typeList = typeList->rsibling;// type 
    struct param_list * param_root = newParam(idList->string , StdtypeCheck(typeList)); 
    //printf("param type %s is %d\n", idList->string , param_root->type);               
    
    
    //typeList = typeList->rsibling;
    if(param_root->type == TypeArray){
        struct array_descriptor * array_desc_root = initArray(typeList->child);
        param_root->array = array_desc_root;
    }
    
    idList = idList->rsibling;
    while(typeList != parameter->child){//to be continued
        //enum StdType type = StdtypeCheck(typeList);
        while(idList != index){
            struct param_list * nextParam = newParam(idList->string ,StdtypeCheck(typeList));//to be added
            idList = idList->rsibling; //"identifier" "identifier"
            if(nextParam->type == TypeArray){
                struct array_descriptor * array_desc_root = initArray(typeList->child);
                nextParam->array = array_desc_root;
            }
            //printf("param type %s is %d\n",nextParam->name , nextParam->type);
            add_param(param_root , nextParam);                        
        }
        typeList = typeList->rsibling;
        //printf("typelist node %s type is %d\n", typeList->child->string , typeList->nodeType);
        if(typeList == parameter->child){
            //printf("shit \n");
            break;
        }
        idList = typeList->child;
        index = idList;
        typeList = typeList->rsibling;
        struct param_list * nextParam = newParam(idList->string , StdtypeCheck(typeList));//to be added
        idList = idList->rsibling; //"identifier" "identifier"
        if(nextParam->type == TypeArray){
            struct array_descriptor * array_desc_root = initArray(typeList->child);
            nextParam->array = array_desc_root;
        }
        //printf("param type %s is %d\n",nextParam->name , nextParam->type);
        add_param(param_root , nextParam);   
        //typeList = typeList->rsibling;
    }
    /*struct param_list * temp = param_root;
    while(temp != temp->next_param){
        printf("%s type %d\n" , temp->name , temp->type);
        temp = temp->next_param;
    }
    printf("%s type %d\n" , temp->name , temp->type);*/
    return param_root;
}
///////////////// print out symbol table ///////////////
void printf_symbol_table(){
    printf("Name    Type    scope\n");
    for(int i = 0 ; i < SymbolTable.size ; i++){
        printf("%s    ",SymbolTable.entries[i].name);
        if(SymbolTable.entries[i].type == TypeArray)
            printf("%s    " , "ARRAY");
        else if(SymbolTable.entries[i].type == TypeInt)
        printf("%s    " , "INT");
        else if(SymbolTable.entries[i].type == TypeReal)
        printf("%s    " , "REAL");
        else if(SymbolTable.entries[i].type == TypeString)
        printf("%s    " , "STRING");
        else if(SymbolTable.entries[i].type == TypeFunction)
        printf("%s    " , "FUNCTION");
        else if(SymbolTable.entries[i].type == TypeProcedure)
        printf("%s    " , "PROCEDURE");

        printf("%d\n" , SymbolTable.entries[i].level);
    }
}
void semanticCheck(struct node *node) {
    //printf("current node type is %d\n" , node->nodeType);
    if(node->lsibling->nodeType == NODE_SUB_DECLS)
        scope_check=1;
    switch(node->nodeType) {
        /*implement scope increase*/

        case NODE_BEGIN:{
            //if(node->rsibling->final == 1 )
              //  break;
            if(node->rsibling->nodeType !=NODE_FUN_HEAD  && node->rsibling->nodeType != NODE_PRO_HEAD){
                printf("begin at line %d\n" , node->lineCount);
                SymbolTable.current_level++;
            }
            break;
        }
        /************Function declaration*************/
        case NODE_FUN_HEAD: { 
            struct node * function_name = nthChild(1 , node);
            struct node * parameter = nthChild(2 , node);
            struct node * typeNode = nthChild(3 , node);
            
            /***********check function redeclaration*************/
            struct SymTableEntry * entry = findSymbol_fun_pro_var(function_name->string);
            if(entry!=0 ){//&& entry->type == TypeFunction){
                printf("[Error ] redeclared function %s at line %d\n" , function_name->string , node->lineCount);
                check = 0;
                //return;
                SymbolTable.current_level++;                
                return;
            }
            else {    
                struct function_attribute * function_attribute_root = newFunctionAttr(StdtypeCheck(typeNode));
                

                /***********check whether it has parameter**********/
                if(parameter->nodeType != NODE_lambda)
                    function_attribute_root->param = initParam(parameter);

                /*******enter function entry into symbol table ************/
                //entry functin_name , functiontype , function_attribtue 
                addFunction(function_name->string , TypeFunction , function_attribute_root);
                SymbolTable.current_level++;
                addVariable(function_name->string , StdtypeCheck(typeNode) , 0);
                printf("New scope created at line %d\n" , node->lineCount);                        
            }
            break;
        }
        /************Procedure declaration***********/
        case NODE_PRO_HEAD: { 
            struct node * procedure_name = nthChild(1 , node);
            struct node * parameter = nthChild(2 , node);

            /* check procedure of redeclared problem*/
            struct SymTableEntry * entry ;
            if( (entry=findSymbol_fun_pro_var(procedure_name->string)) != 0 ){//&& entry->type == TypeProcedure){
                printf("[Error ] redeclared procedure %s at line %d\n" , procedure_name->string , node->lineCount);
                check = 0;
                SymbolTable.current_level++;                
                return;
            }
            else {
                struct procedure_attribute * procedure_attribute_root = newProcedureAttr();

                //////////////  check whether it has parameter //////////////////////
                if(parameter->nodeType != NODE_lambda)
                    procedure_attribute_root->param = initParam(parameter);
                
                addProcedure(procedure_name->string , TypeProcedure , procedure_attribute_root);
                SymbolTable.current_level++;
                printf("New scope created at line %d\n" , node->lineCount);
            }            
            break;
        }
        /************Close Scope********************/
        case NODE_END:{
            //popSymbolTable();
            SymbolTable.current_level--;
            printf("Scope deleted at line %d\n" , node->lineCount);                        
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
                                struct SymTableEntry * temp =  findSymbol_in_scope(idNode->string);
                                if(temp == 0){
                                    addVariable(idNode->string, valueType , array_desc_root);
                                }
                                else {
                                    printf("[Error ] Redclared Variable \"%s\" at line %d\n", idNode->string , idNode->lineCount);
                                    check = 0;
                                    return;
                                }
                                idNode = idNode->rsibling;
                            } while(idNode != idList->child);                          
                        }
                        /*********int , string , real variable declaration implement here***********/
                        else {
                            struct node *idList = nthChild(check_node, node);
                            struct node *idNode = idList->child;
                            do {
                                struct SymTableEntry * temp =  findSymbol_in_scope(idNode->string);
                                if(temp == 0){
                                    addVariable(idNode->string, valueType , 0);
                                }
                                else {
                                    printf("[Error ] Redclared Variable \"%s\" at line %d\n", idNode->string , idNode->lineCount);
                                    check = 0;
                                    return;
                                }                                
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
            struct SymTableEntry *entry;
            if(scope_check==0)
                entry = findSymbol_in_global(node->string);
            else 
                entry = findSymbol_fun_pro_var(node->string);

            if(entry == 0) {
                printf("[Error ] undeclared variable %s and at line %d\n", node->string , node->lineCount);
                check = 0;
                return;
                //exit(0);
            }
            //printf("identifier is %s type is %d\n" , node->string , entry->type);
            node->entry = entry;
            node->valueType = entry->type;
            ////////////////// symbol table entry is procedure /////////////////////////////
            if(entry->type == TypeProcedure) {
                node->procedure = entry->procedure;
                //procedure -> identifier
                if(node->child == NULL){
                    if(entry->procedure->param == NULL)
                        return;
                    else {
                        printf("[Error ] wrong number of argument at line %d \n" , node->lineCount);
                        check = 0;
                        return;
                    }
                }
                else if(node->child->child == NULL){
                    if(entry->procedure->param == NULL){
                        return;
                    }
                    else {
                        printf("[Error ] mismatch of procedure %s at line %d \n" , node->string , node->lineCount);
                        check = 0;
                        return;
                    }
                }
                /// procedure call ex: a[] , a()
                else if(node->child->child == NULL||node->child->child->nodeType == TOKEN_LBRAC){
                    printf("[Error ] mismatch of procedure %s at line %d\n" , node->string , node->lineCount);
                    check = 0;
                    return;
                }
                else {
                    struct node * argument = node->child->child; // first argument
                    if(entry->procedure->param == NULL){
                        printf("[Error ] wrong number of argument\n");
                        check = 0;
                        return;
                    }
                    int node_argument_num = 0;
                    struct node * argument_check = argument;
                    do{
                        node_argument_num++;
                        printf("expr type is %d at line %d\n" , argument_check->nodeType , argument_check->lineCount);
                        argument_check = argument_check->rsibling;
                    }while (argument_check != argument);

                    struct param_list * first_param = entry->procedure->param;
                    int argumentNum = 1;
                    while(first_param != first_param->next_param){
                        argumentNum++;
                        first_param = first_param->next_param;
                        //printf("argument %d\n" , argumentNum);
                    }
                    printf("argument is %d\n" , argumentNum);
                    if(node_argument_num!=argumentNum){
                        printf("[Error ] wrong number of argument at line %d\n" , node->lineCount);
                        check = 0;
                        return;
                    }
                    struct node * expr = node->child->child;
                    for(int idx = 0 ; idx < argumentNum ; idx++){
                        semanticCheck(expr);
                        struct param_list * checkParam =entry->procedure->param;
                        for(int j = idx+1 ; j < argumentNum ; j++){
                            //printf("%s type is %d\n", checkParam->name , checkParam->type);
                            checkParam = checkParam->next_param;   
                        }
                        if(expr->valueType != TypeArray && checkParam->type != TypeArray){
                            if(expr->valueType != checkParam->type){
                                printf("[Error ] wrong type of argument  node value type is %d and entry is %d at line %d\n" ,expr->valueType, checkParam->type, node->lineCount);
                                check = 0;
                                return;
                            }
                        }
                        if(expr->valueType == checkParam->type){
                            if(expr->valueType == TypeArray){
                                struct array_descriptor * expr_array = expr->array;
                                struct array_descriptor * entry_array = checkParam->array;
                                while(entry_array->next_array != entry_array){
                                    if(expr_array->array_begin != entry_array->array_begin || expr_array->array_end != entry_array->array_end){
                                        printf("[Error ] out range at line %d\n" , node->lineCount);
                                        check = 0;
                                        return;
                                    }
                                    if(expr_array->type != entry_array->type){
                                        printf("[Error ] array type mismatch at line %d\n" , node->lineCount);
                                        check = 0;
                                        return;
                                    }
                                    expr_array = expr_array->next_array;
                                    entry_array = entry_array->next_array;
                                }
                            }
                        }
                        //to be continued 
                        expr = expr->rsibling;
                    }
                }
            }
            else if (entry->type == TypeFunction) {
                node->function = entry->function;
                node->valueType = entry->function->type;
                //procedure -> identifier
                if(node->child == NULL){
                    if(entry->function->param == NULL)
                        return;
                    else {
                        printf("[Error ] wrong number of argument at line %d\n" , node->lineCount);
                        check = 0;
                        return;
                    }
                }
                else if(node->child->child == NULL){
                    //node->iValue = entry->iValue;
                    //check =0;
                    return;
                }
                else if(node->child->child->nodeType == TOKEN_LBRAC){
                    printf("[Error ] mismatch of function %s at line %d\n" , node->string , node->lineCount);
                    check = 0;
                    return;
                }

                else {
                    struct node * argument = node->child->child; // first argument
                    if(entry->function->param == NULL){
                        printf("[Error ] wrong number of argument at line %d\n" , node->lineCount);
                        check = 0;
                        return;
                    }
                    int node_argument_num = 0;
                    struct node * argument_check = argument;
                    do{
                        node_argument_num++;
                        printf("expr type is %d at line %d\n" , argument_check->nodeType , argument_check->lineCount);
                        argument_check = argument_check->rsibling;
                    }while (argument_check != argument);

                    struct param_list * first_param = entry->function->param;
                    int argumentNum = 1;
                    while(first_param != first_param->next_param){
                        argumentNum++;
                        first_param = first_param->next_param;
                    }
                    printf("%d %d\n" , node_argument_num , argumentNum);
                    if(node_argument_num!=argumentNum){
                        printf("[Error ] wrong number of argument at line %d\n" , node->lineCount);
                        check = 0;
                        return;
                    }
                    struct node * expr = node->child->child;
                    for(int idx = 0 ; idx < argumentNum ; idx++){
                        semanticCheck(expr);
                        struct param_list * checkParam =entry->function->param;
                        for(int j = idx+1 ; j < argumentNum ; j++){
                            checkParam = checkParam->next_param;
                        }
                        if(expr->valueType != TypeArray && checkParam->type != TypeArray){
                            if(expr->valueType != checkParam->type){
                                printf("[Error ] wrong type of argument at line %d\n" , node->lineCount);
                                check = 0;
                                return;
                            }
                        }
                        if(expr->valueType == checkParam->type){
                            if(expr->valueType == TypeArray){
                                struct array_descriptor * expr_array = expr->array;
                                struct array_descriptor * entry_array = checkParam->array;
                                while(entry_array->next_array != entry_array){
                                    if(expr_array->array_begin != entry_array->array_begin || expr_array->array_end != entry_array->array_end){
                                        printf("[Error ] out range at line %d\n" , node->lineCount);
                                        check = 0;
                                        return;
                                    }
                                    if(expr_array->type != entry_array->type){
                                        printf("[Error ] array type mismatch at line %d\n" , node->lineCount);
                                        check = 0;
                                        return;
                                    }
                                    expr_array = expr_array->next_array;
                                    entry_array = entry_array->next_array;
                                }
                            }
                        }
                        //to be continued 
                        expr = expr->rsibling;
                    }
                }
            }
            /*************** symbol talbe entry is array **********************/
            else if(entry->type == TypeArray) {
                node->array = entry->array;
                if(node->child == NULL){
                    printf("warning: meanless use of array %s at line %d\n" , node->string , node->lineCount);
                    return;
                }
                else if(node->child->child == NULL || node->child->child->nodeType == TOKEN_LBRAC){
                    struct node * idx = node->child;
                    if(idx->child == NULL)
                        return;
                    else {
                        ////////// count the dimenson in the symbol table and its standard type //////////
                        struct array_descriptor * array_argument = node->array;
                        int argumentNum = 1;
                        while(array_argument!=array_argument->next_array){
                            argumentNum++;
                            array_argument = array_argument->next_array;
                        }
                        enum StdType type = array_argument->type;

                        //////check for dimension//////////
                        struct node * tmp = idx->child;
                        int check_dimesion = 0;

                        do{
                            tmp = tmp->rsibling;//"num"
                            tmp = tmp->rsibling;//"]"
                            tmp = tmp->rsibling;//"[ or null"
                            check_dimesion++;
                        }while(tmp != idx->child);

                        if(check_dimesion != argumentNum){
                            printf("[Error ] wrong array dimesion at line %d\n" , node->lineCount);
                            check = 0;
                            return;
                        }

                        idx = idx->child; //"["
                        //printf("%d\n",idx->nodeType);
                        semanticCheck(idx->rsibling);
                        if(idx->rsibling->valueType != TypeInt){
                            printf("[Error ] the argument is not integer at line %d\n" , node->lineCount);
                            check = 0;
                            return;
                        }
                        int num =0;
                        idx = idx->rsibling; //"num"
                        //printf("%d\n",idx->iValue);
                        while(idx != node->child->child){
                            array_argument = node->array;
                            for(int i = 1 ; i < argumentNum ; i++){
                                array_argument = array_argument->next_array;
                            }
                            if(idx->iValue < array_argument->array_begin || idx->iValue > array_argument->array_end){
                                printf("[Error ] idx is %d out of range from %d to %d at line %d\n" , idx->iValue , array_argument->array_begin , array_argument->array_end , node->lineCount);
                                check = 0;
                                //return;
                            }
                            idx = idx->rsibling; //"]"
                            if(idx->rsibling == node->child->child){// no more [ num ]
                                //printf("no more [\n");
                                if(argumentNum == 1){
                                    //printf("happens %d\n" , type);
                                    node->valueType = type;
                                    return;
                                    //printf("shit type is %d\n" , type);
                                }
                                break;
                            }
                            if(argumentNum == 0){ // too many 
                                printf("[Error ] wrong dimension at line %d\n" , node->lineCount);
                                return;
                            }
                            num++;
                            argumentNum--;
                            idx = idx->rsibling; //"["                            
                            semanticCheck(idx->rsibling);
                            if(idx->rsibling->valueType != TypeInt){
                                printf("[Error ] the argument is not integer at line %d\n" , node->lineCount);
                                return;
                            }
                            idx = idx->rsibling;//"num"
                            
                        }
                        node->array = array_argument;   
                    }
                }
                else 
                    return;

            }
            /*************** symbol table entry is variable *****************/ 
            else {
                if(node->child!=NULL&&node->child->nodeType==TOKEN_LBRAC){
                    printf("[Error ] wrong use of variabe %s at line %d\n" , node->string , node->lineCount);
                    check = 0;
                    return;
                }
                else {
                    if(entry->type == TypeInt){
                        node->iValue = entry->iValue;
                    }
                    else if(entry->type == TypeReal){
                        node->rValue = entry->rValue;
                    }
                    //else if(entry->type == TypeString){
                        
                    //}
                }
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
                        if(child1->valueType == child2->valueType && child1->valueType != TypeProcedure && child2->valueType != TypeProcedure){
                            node->valueType = child1->valueType;
                            switch(node->op){
                                case OP_ADD: {
                                        if(child1->valueType == TypeInt){
                                            node->iValue = child1->iValue + child2->iValue;
                                            //node->entry->iValue = 
                                            return;
                                        }
                                        else if(child1->valueType == TypeReal){
                                            node->rValue = child1->rValue + child2->rValue; 
                                            return;
                                        }
                                        else if(child1->valueType == TypeString){
                                            printf("[Error ] wrong type  at line %d\n" , node->lineCount);
                                            check = 0;
                                            return;
                                        }
                                        else {//type array
    
                                        }
                                }
                                case OP_SUB: {
                                    if(child1->valueType == TypeInt){
                                        node->iValue = child1->iValue - child2->iValue;
                                        return;
                                    }
                                    else if(child1->valueType == TypeReal){
                                        node->rValue = child1->rValue - child2->rValue; 
                                        return;
                                    }
                                    else if(child1->valueType == TypeString){
                                        printf("Error: wrong type at line %d\n" , node->lineCount);
                                        return;
                                    }
                                    else {//type array
    
                                    }
                                }
                                case OP_MUL:{
                                    if(child1->valueType == TypeInt){
                                        node->iValue = child1->iValue * child2->iValue;
                                        return;
                                    }
                                    else if(child1->valueType == TypeReal){
                                        node->rValue = child1->rValue * child2->rValue; 
                                        return;
                                    }
                                    else if(child1->valueType == TypeString){
                                        printf("[Error ] wrong type at line %d\n" , node->lineCount);
                                        return;
                                    }
                                    else {//type array
    
                                    }
                                }
                                case OP_DIV:{
                                    if(child1->valueType == TypeInt){
                                        node->iValue = child1->iValue / child2->iValue;
                                        return;
                                    }
                                    else if(child1->valueType == TypeReal){
                                        node->rValue = child1->rValue / child2->rValue; 
                                        return;
                                    }
                                    else if(child1->valueType == TypeString){
                                        printf("[Error ] wrong type at line %d\n" , node->lineCount);
                                        check = 0;
                                        return;
                                    }
                                    else {//type array
                                        printf("[Error ]wrong type in arithmetic operation at line %d\n" , node->lineCount);
                                        check = 0;
                                        return; 
                                    }
                                }
                                case OP_LT: {
                                    if(child1->valueType == TypeInt){
                                        node->iValue = child1->iValue < child2->iValue?1:0;
                                        return;
                                    }
                                    else if(child1->valueType == TypeReal){
                                        node->iValue = child1->rValue < child2->rValue?1:0; 
                                        node->valueType = TypeInt;
                                        return;
                                    }
                                    else if(child1->valueType == TypeString){
                                        printf("[Error ] wrong type at line %d\n" , node->lineCount);
                                        check = 0;
                                        return;
                                    }
                                    else {//type array
    
                                    }
                                }
                                case OP_GT:{
                                    if(child1->valueType == TypeInt){
                                        node->iValue = child1->iValue > child2->iValue?1:0;
                                        return;
                                    }
                                    else if(child1->valueType == TypeReal){
                                        node->iValue = child1->rValue > child2->rValue?1:0;
                                        node->valueType = TypeInt; 
                                        return;
                                    }
                                    else if(child1->valueType == TypeString){
                                        printf("[Error ]wrong type at line %d\n" , node->lineCount);
                                        check = 0;
                                        return;
                                    }
                                    else {//type array
    
                                    }
                                }
                                case OP_EQ:{
                                    if(child1->valueType == TypeInt){
                                        node->iValue = child1->iValue == child2->iValue?1:0;
                                        return;
                                    }
                                    else if(child1->valueType == TypeReal){
                                        node->iValue = child1->rValue == child2->rValue?1:0;
                                        node->valueType = TypeInt; 
                                        return;
                                    }
                                    else if(child1->valueType == TypeString){
                                        printf("[Error ] wrong type at line %d\n" , node->lineCount);
                                        check = 0;
                                        return;
                                    }
                                    else {//type array
    
                                    }
                                }
                                case OP_NE:{
                                    if(child1->valueType == TypeInt){
                                        node->iValue = (child1->iValue != child2->iValue )?1:0;
                                        return;
                                    }
                                    else if(child1->valueType == TypeReal){
                                        node->iValue = (child1->rValue != child2->rValue)?1:0; 
                                        node->valueType = TypeInt;
                                        return;
                                    }
                                    else if(child1->valueType == TypeString){
                                        printf("[Error ] wrong type at line %d\n" , node->lineCount);
                                        check = 0;
                                        return;
                                    }
                                    else {//type array
    
                                    }
                                }
                                case OP_GE:{
                                    if(child1->valueType == TypeInt){
                                        node->iValue = (child1->iValue >= child2->iValue)?1:0 ;
                                        return;
                                    }
                                    else if(child1->valueType == TypeReal){
                                        node->iValue = (child1->rValue >= child2->rValue)?1:0 ;
                                        node->valueType = TypeInt; 
                                        return;
                                    }
                                    else if(child1->valueType == TypeString){
                                        printf("[Error ] wrong type at line %d\n" , node->lineCount);
                                        check = 0;
                                        return;
                                    }
                                    else {//type array
    
                                    }
                                }
                                case OP_LE:{
                                    if(child1->valueType == TypeInt){
                                        node->iValue = (child1->iValue <= child2->iValue)?1:0;
                                        return;
                                    }
                                    else if(child1->valueType == TypeReal){
                                        node->iValue = (child1->rValue <= child2->rValue)?1:0;
                                        node->valueType = TypeInt; 
                                        return;
                                    }
                                    else if(child1->valueType == TypeString){
                                        printf("[Error ] wrong type at line %d\n" , node->lineCount);
                                        check = 0;
                                        return;
                                    }
                                    else {//type array
    
                                    }
                                }
                                /*NOT factor
                                case OP_NOT:{
    
                                }*/
                            }
                        }
                        else {
                            printf("[Error] type errors in arithmetic expression at line %d\n" , node->lineCount);
                            check = 0;
                            return;
                        }
                    }
                    else {                        
                        semanticCheck(child1);//unary op
                        node->valueType = child1->valueType;
                        switch(node->op){
                            case OP_ADD: {
                                    if(child1->valueType == TypeInt){
                                        node->iValue = child1->iValue;
                                        return;
                                    }
                                    else if(child1->valueType == TypeReal){
                                        node->rValue = child1->rValue; 
                                        return;
                                    }
                                    else if(child1->valueType == TypeString){
                                        printf("[Error ] wrong type at line %d\n" , node->lineCount);
                                        check =0;
                                        return;
                                    }
                                    else {//type array

                                    }
                            }
                        case OP_SUB: {
                                if(child1->valueType == TypeInt){
                                    node->iValue = child1->iValue;
                                    return;
                                }
                                else if(child1->valueType == TypeReal){
                                    node->rValue = child1->rValue; 
                                    return;
                                }
                                else if(child1->valueType == TypeString){
                                    printf("[Error ] wrong type at line %d\n" , node->lineCount);
                                    check =0;
                                    return;
                                }
                                else {//type array

                                }
                        }
                        }
                    }
                    return ;
        }

        /*************** If statement check is implemented here ********************/
        case NODE_IF:{
            //printf("fuck \n");
            struct node * expr = nthChild(1 , node);
            struct node * statement1 = nthChild(2 , node);
            struct node * statement2 = nthChild(3 , node);
            semanticCheck(expr);            
            if(expr->valueType != TypeInt){
                printf("[Error ] expression in If is not a right type at line %d\n" , node->lineCount);
                //exit(0);
                check = 0;
                return;
            }
            if(expr->iValue != 0)
                semanticCheck(statement1);
            else 
                semanticCheck(statement2);
                printf("fuck\n");
                
            SymbolTable.current_level++;
            return;
        }
        /*************** while statement check is implemented here *****************/
        case NODE_WHILE:{
            struct node * expr = nthChild(1 , node);
            struct node * statement = nthChild(2 , node);
            semanticCheck(expr);
            semanticCheck(statement);
            if(expr->valueType != TypeInt){
                printf("[Error ] expression in while is not a right type at line %d\n" , node->lineCount);
                check = 0;
                return;
                //exit(0);
            }
            SymbolTable.current_level++;
            return;
        }
        /*************** digseq factor check is implemented here*********/
        case NODE_INT:{
            node->valueType = TypeInt;
            return;
        }
        /*************** realnumber factor check is implemented here*********/
        case NODE_REAL:{
            node->valueType = TypeReal;
            return;
        }
        /*************** string factor check is implemented here********/
        case NODE_STRING_v:{
            node->valueType = TypeString;
            return;
        } 
        case NODE_ASSIGN_STMT: {
            struct node *child1 = nthChild(1, node);

            struct SymTableEntry * temp_check ;//= findSymbol_in_global(child1->string);
            if(scope_check==0){
                temp_check = findSymbol_in_global(child1->string);
                //printf("here\n");
            }
            else
                temp_check = findSymbol_fun_pro_var(child1->string);

            if(temp_check==0){
                printf("[Error ] undeclared variable %s at line %d\n" , child1->string , child1->lineCount);
                check = 0;
                return;
            }

            if(temp_check->type == TypeFunction){
                printf("[Error ] Function placed in LHS , maybe it should be placed in RHS at line %d\n" , node->lineCount);
                //exit(0);
                check = 0;
                return;
            }
            struct node *child2 = nthChild(2, node);
            semanticCheck(child1);
            semanticCheck(child2);
            
            //////////procedure use in the assignment ///////////
            if(child1->valueType == TypeProcedure || child2->valueType == TypeProcedure){
                printf("[Error ] Miss use of procedure at line %d\n" , node->lineCount);
                //exit(0);
                check = 0;
                return;
            }
  
            /* We only implement the checking for integer and real types
               you should implement the checking for array type by yourself */
            if(child1->valueType != child2->valueType) {
                printf("[Error ] type mismatch for assignment at line %d\n" , child1->lineCount);
                check = 0;
                return ;
                //exit(0);
            }
            node->valueType = child1->valueType;
            /**********assign value into left element****************/
            if(child1->valueType == TypeInt){
                //printf("fuckleo here\n");
                child1->entry->iValue = child2->iValue;
                //printf("Variable %s's value is %d \n", child1->entry->name , child1->entry->iValue);
            }
            else if(child1->valueType == TypeReal){
                child1->entry->rValue = child2->rValue;
            }
            else if(child1->valueType == TypeString){
                strcpy(child1->entry->string , child2->string);
            }
            //////////////// array assignment ////////
            /*else {

            }*/
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

