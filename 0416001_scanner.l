%{
#include <stdio.h>
#include <string.h>
#include "node-type.h"
#include "y.tab.h"
/*
#define   ARRAY      1
#define   DO      2
#define   ELSE      3
#define   END      4

#define   FUNCTION      5
#define   IF      6
#define   NOT      7
#define   OF       8
#define   PROCEDURE      9
#define   PROGRAM      10
#define   THEN      11
#define   VAR      12
#define   WHILE      13
#define   IDENTIFIER      14

#define   ASSIGNMENT      15
#define   COLON      16
#define   COMMA      17
#define   DIGSEQ      18
#define   DOT      19
#define   DOTDOT      20
#define   EQUAL      21
#define   GE      22
#define   GT      23
#define   LBRAC      24
#define   LE      25
#define   LPAREN      26
#define   LT      27
#define   MINUS      28
#define   PLUS      29
#define   RBRAC      30
#define   REALNUMBER      31
#define   RPAREN      32
#define   SEMICOLON      33
#define   SLASH      34
#define   STAR      35

#define   notEQUAL    36
#define   STRING  37
#define   real 38
#define   INTEGER 39
#define   begin 40
#define   string_v 41
*/
int lineCount = 1;
extern YYSTYPE yylval;
struct node* newTokenNode(int tokenType , int lineCount);

%}

A [aA]
B [bB]
C [cC]
D [dD]
E [eE]
F [fF]
G [gG]
H [hH]
I [iI]
J [jJ]
K [kK]
L [lL]
M [mM]
N [nN]
O [oO]
P [pP]
Q [qQ]
R [rR]
S [sS]
T [tT]
U [uU]
V [vV]
W [wW]
X [xX]
Y [yY]
Z [zZ]
NQUOTE [^']

%%


{A}{R}{R}{A}{Y}     {
            yylval.node_t = newTokenNode(ARRAY,lineCount); 
            return(ARRAY);
}
{D}{O}       {
          yylval.node_t = newTokenNode(DO,lineCount); 
          return(DO);
}
{E}{L}{S}{E}    {
          yylval.node_t = newTokenNode(ELSE,lineCount); 
          return(ELSE);
}
{E}{N}{D}      {
          yylval.node_t = newTokenNode(END,lineCount); 
          return(END);
}
{F}{U}{N}{C}{T}{I}{O}{N}     {
                    yylval.node_t = newTokenNode(FUNCTION,lineCount); 
                    return(FUNCTION);
}
{I}{F}     {
        yylval.node_t = newTokenNode(IF,lineCount); 
        return(IF);
}
{N}{O}{T}        {
            yylval.node_t = newTokenNode(NOT,lineCount); 
            return(NOT);
}
{O}{F}        {
          yylval.node_t = newTokenNode(OF,lineCount);  
          return(OF); 
}
{P}{R}{O}{C}{E}{D}{U}{R}{E}  {
            yylval.node_t = newTokenNode(PROCEDURE,lineCount); 
            return(PROCEDURE);
}
{P}{R}{O}{G}{R}{A}{M}        {
            yylval.node_t = newTokenNode(PROGRAM,lineCount); 
            return(PROGRAM);
}
{T}{H}{E}{N}       {
            yylval.node_t = newTokenNode(THEN,lineCount); 
            return(THEN);
}
{V}{A}{R}          {
            yylval.node_t = newTokenNode(VAR,lineCount); 
            return(VAR);
}
{W}{H}{I}{L}{E}    {
            yylval.node_t = newTokenNode(WHILE,lineCount); 
            return(WHILE);
}
{R}{E}{A}{L}         {
            yylval.node_t = newTokenNode(real,lineCount); 
            return(real);
}
{I}{N}{T}{E}{G}{E}{R}        {
                               yylval.node_t = newTokenNode(INTEGER,lineCount); 
                               return(INTEGER);
}
{B}{E}{G}{I}{N}    {
            yylval.node_t = newTokenNode(begin,lineCount);
            return(begin);
}
{S}{T}{R}{I}{N}{G}           {
                          yylval.node_t = newTokenNode(STRING,lineCount);
                          return(STRING);
}
[a-zA-Z]([a-zA-Z0-9]|"_")*   { 
                          yylval.node_t = newTokenNode(IDENTIFIER,lineCount); 
                          yylval.node_t->string = (char*)malloc(yyleng+1); 
                          strcpy(yylval.node_t->string , yytext);
                          return(IDENTIFIER);
}

":="              {
            yylval.node_t = newTokenNode(ASSIGNMENT,lineCount); 
            return(ASSIGNMENT);
}
":"                  {
        yylval.node_t = newTokenNode(COLON,lineCount);  
        return(COLON);
}
","               {
        yylval.node_t = newTokenNode(COMMA,lineCount);  
        return(COMMA);
}

[0-9]+  |
"-"[0-9]+                 {
      yylval.node_t = newTokenNode(DIGSEQ,lineCount);     
      yylval.node_t->valueValid = VALUE_I_VALID;
      yylval.node_t->iValue = atoi(yytext); 
      return(DIGSEQ);
}

"."                   {
        yylval.node_t = newTokenNode(DOT,lineCount); 
        return(DOT);
}

".."             {
        yylval.node_t = newTokenNode(DOTDOT,lineCount);  
        return(DOTDOT);
}

"="              {
        yylval.node_t = newTokenNode(EQUAL,lineCount);  
        yylval.node_t->op = OP_EQ;
        return(EQUAL);
}

"!="             {
      yylval.node_t = newTokenNode(notEQUAL,lineCount);
      yylval.node_t->op = OP_NE; 
      return(notEQUAL);
}

">="             {
      yylval.node_t = newTokenNode(GE,lineCount);
      yylval.node_t->op = OP_GE; 
      return(GE);
}

">"              {
      yylval.node_t = newTokenNode(GT,lineCount);
      yylval.node_t->op = OP_GT; 
      return(GT);
}

"["             {
      yylval.node_t = newTokenNode(LBRAC,lineCount);
      return(LBRAC);
}

"<="           {
      yylval.node_t = newTokenNode(LE,lineCount); 
      yylval.node_t->op = OP_LE;
      return(LE);
}

"("            {
      yylval.node_t = newTokenNode(LPAREN,lineCount); 
      return(LPAREN);
}

"<"            {
      yylval.node_t = newTokenNode(LT,lineCount);  
      yylval.node_t->op = OP_LT;  
      return(LT);
}

"-"            {
      yylval.node_t = newTokenNode(MINUS,lineCount);
      yylval.node_t->op = OP_SUB; 
      return(MINUS);
}

"+"            {
      yylval.node_t = newTokenNode(PLUS,lineCount); 
      yylval.node_t->op = OP_ADD;
      return(PLUS);
}

"]"            {
      yylval.node_t = newTokenNode(RBRAC,lineCount);
      return(RBRAC);
}

[0-9]+"."[0-9]+  |
"-"[0-9]+"."[0-9]+            {
                  yylval.node_t = newTokenNode(REALNUMBER,lineCount);
                  yylval.node_t->rValue = atof(yytext); 
                  yylval.node_t->valueValid = VALUE_R_VALID;
                  return(REALNUMBER);
}

")"                {
          yylval.node_t = newTokenNode(RPAREN,lineCount); 
          return(RPAREN);
}

";"                {
          yylval.node_t = newTokenNode(SEMICOLON,lineCount); 
          return(SEMICOLON);
}

"/"                {
          yylval.node_t = newTokenNode(SLASH,lineCount);
          yylval.node_t->op = OP_DIV; 
          return(SLASH);
}

"*"                {
          yylval.node_t = newTokenNode(STAR,lineCount); 
          yylval.node_t->op = OP_MUL;
          return(STAR);
}


"-"[0-9]*"."[0-9]+"E"["+"|"-"][0-9]+ | 
[0-9]*"."[0-9]+"E"["+"|"-"][0-9]+   {
            yylval.node_t = newTokenNode(REALNUMBER,lineCount); 
            yylval.node_t->rValue = atof(yytext);
            yylval.node_t->valueValid = VALUE_R_VALID;
            return(REALNUMBER);
}

"(*"   |
"{"    { register int c; 
     while ((c = input()))
     {
      if (c == '}')
       break;
      else if (c == '*')
      {
       if ((c = input()) == ')')
        break;
       else
        unput (c);
      }
      else if (c == '\n')
       lineCount++;
      else if (c == 0)
        fprintf (stderr, "Unexpected EOF inside comment at line %d\n",lineCount);
     }
    }

[ \t\f]    ;

\r         ;
\n    lineCount++;

.    {fprintf(stderr,"Lexical analyzer error at line %d : %s\n",lineCount,yytext);}

\"[^\t\f|^\r|^\n|^\"]*\"  {
            yylval.node_t = newTokenNode(string_v ,lineCount) ; 
            yylval.node_t->string = (char *)malloc(yyleng+1);
            strcpy(yylval.node_t->string , yytext);
            return(string_v);
}
 \'[^t\f|^\r|^\n|^\"]*\' {
            yylval.node_t = newTokenNode(string_v,lineCount) ; 
            yylval.node_t->string = (char *)malloc(yyleng+1);
            strcpy(yylval.node_t->string , yytext);
            return(string_v);
}


"//".* {}


%%

struct node* newTokenNode(int tokenType , int lineCount) {
    struct node *node = newNode(NODE_TOKEN, lineCount);
    node->tokenType = tokenType;
    node->lineCount = lineCount;
    return node;
}

/*int main(int argc, char** argv) {
  int i, tok;
  yyin = fopen("test.txt", "r");
  for (i = 0; 1==1; i ++) {
    tok = yylex();
    fprintf(stderr, "%3d-th token(type:%d) on line %d : %s\n", i, tok,lineCount, yytext);
  }
  return(0);
} */

yywrap() {
  //fclose(yyin);
  //exit(1);
}
