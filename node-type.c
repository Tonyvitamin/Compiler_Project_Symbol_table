#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "node-type.h"

struct node* newNode(int type) {
    struct node *node = (struct node*)malloc(sizeof(struct node));
    node->nodeType = type;
    node->valueValid = VALUE_INVALID;
    node->string = NULL;
    node->parent = NULL;
    node->child = NULL;
    node->lsibling = node;
    node->rsibling = node;
    return node;
}

void addChild(struct node *node, struct node *child) {
    child->parent = node;

    if(node->child == NULL) {
        node->child = child;
    }
    else {
        child->lsibling = node->child->lsibling;
        child->rsibling = node->child;
        node->child->lsibling->rsibling = child;
        node->child->lsibling = child;
    }
}

void deleteNode(struct node *node) {
    if(node->string != NULL)
        free(node->string);
    free(node);
}

void printTree(struct node *node, int ident) {
    static char blank[1024];
    for(int i=0; i<ident; i++)
        blank[i] = ' ';
    blank[ident] = 0;

    switch(node->nodeType) {
        case NODE_TOKEN:
        printf("%sToken %s\n", blank, node->string);
        break;
        case NODE_OP:
            switch(node->op) {
                case OP_ADD:
                    printf("%s+\n", blank);
                    break;
                case OP_SUB:
                    printf("%s-\n", blank);
                    break;
                case OP_MUL:
                    printf("%s*\n", blank);
                    break;
                case OP_DIV:
                    printf("%s/\n", blank);
                    break;
                case OP_GT:
                    printf("%s>\n", blank);
                    break;
                case OP_LT:
                    printf("%s<\n", blank);
                    break;
                case OP_EQ:
                    printf("%s=\n", blank);
                    break;
                case OP_GE:
                    printf("%s>=\n", blank);
                    break;
                case OP_LE:
                    printf("%s<=\n", blank);
                    break;
                case OP_NE:
                    printf("%s!=\n", blank);
                    break;
            }
            ident++;
            break;
        case NODE_INT:
            printf("%s%d\n", blank, node->iValue);
            break;
        case NODE_REAL:
            printf("%s%f\n", blank, node->rValue);
            break;
        case NODE_VAR_OR_PROC:
            printf("%s%s\n", blank, node->string);
            break;
        case NODE_STRING_v:
            printf("%s%s\n", blank, node->string);
            break;
        case NODE_LIST:
            break;
        case NODE_lambda:
            break;
        case NODE_IF:
            printf("%sIF\n" , blank);
            ident++;
            break;
        case NODE_PROGRAM:
            printf("%sPROGRAM\n", blank);
            ident++;
            break;
        case NODE_DECL:
            printf("%sDECLS\n" , blank);
            ident++;
            break;
        case NODE_SUB_DECLS:
            printf("%sSUB_DECLS\n" , blank);
            ident++;
            break;
        case NODE_TYPE_INT:
            printf("%sTYPE_INT\n", blank);
            break;
        case NODE_TYPE_REAL:
            printf("%sTYPE_REAL\n", blank);
            break;
        case NODE_TYPE_STRING:
            printf("%sTYPE_STRING\n", blank);
            break;
        case NODE_TYPE_ARRAY:
            printf("%sTYPE_ARRAY\n", blank);
            break;
        case NODE_ASSIGN_STMT:
            printf("%sASSIGN_STMT\n", blank);
            ident++;
            break;
        case NODE_FUN_HEAD:
            printf("%sFUNCTION\n" , blank);
            ident++;
            break;
        case NODE_PRO_HEAD:
            printf("%sPROCEDURE\n" , blank);
            ident++;
            break;
        case NODE_SYM_REF: //check declaration
            printf("%sSYM_REF %s\n", blank, node->string);
            break;
    }

    struct node *child = node->child;
    if(child != NULL) {
        do {
            printTree(child, ident);
            child = child->rsibling;
        } while(child != node->child);
    }
}

