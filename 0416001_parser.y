%{
 #include <stdio.h>
 #include <stdlib.h>
 #include "node-type.h"
 #include "symtab.h"
    extern struct SymTable SymbolTable;
    struct node *  ASTRoot;
    struct node * newOpNode(int op);
    /* Called by yyparse on error.  */
     void
     yyerror (char const *s)
     {
        extern char *yytext;
        extern int lineCount;
        fprintf (stderr, "%s: at line %d symbol'%s'\n", s,lineCount,yytext);
     }
%}

%union { 
    struct node * node_t ;
}

%token <node_t> ARRAY DO ELSE END FUNCTION IF NOT OF PROCEDURE  
%token <node_t> PROGRAM THEN VAR WHILE 
%token <node_t> IDENTIFIER 
%token <node_t> ASSIGNMENT COLON 
%token <node_t> COMMA 
%token <node_t> DIGSEQ 
%token <node_t> DOT DOTDOT EQUAL GE GT LBRAC LE LPAREN LT 
%token <node_t> MINUS PLUS RBRAC 
%token <node_t> REALNUMBER  
%token <node_t> RPAREN SEMICOLON SLASH  STAR  
%token <node_t> notEQUAL 
%token <node_t> STRING  
%token <node_t> real INTEGER  begin string_v

%type <node_t> prog identifier_list standard_type optional_var 
%type <node_t> procedure_statement relop term factor
%type <node_t> subprogram_declaration subprogram_declarations 
%type <node_t> subprogram_head parameter_list expression
%type <node_t> expression_list   arguments   declarations type
%type <node_t> variable tail addop mulop compound_statement optional_statements 
%type <node_t> statement statement_list  simple_expression 
%start prog
%%


prog : PROGRAM IDENTIFIER LPAREN identifier_list RPAREN SEMICOLON
     declarations
     subprogram_declarations
     compound_statement
     DOT {  //create symbol table
            $$ = newNode(NODE_PROGRAM);
            addChild($$ , $2);
            deleteNode($4);
            deleteNode($1); 
            deleteNode($3); 
            deleteNode($5); 
            deleteNode($6); 
            addChild($$ , $7); 
            addChild($$ , $8);
            addChild($$ , $9);
            deleteNode($10); 
            ASTRoot = $$;};

identifier_list : IDENTIFIER {
                        $$ = newNode(NODE_LIST);
                        addChild($$ , $1);};
        | identifier_list COMMA IDENTIFIER {  
                            $$ = $1;
                            addChild($$ , $3);
                            deleteNode($2); };

declarations : declarations VAR identifier_list COLON type SEMICOLON {
                        $$ = $1;
                        addChild($$ , $3);
                        addChild($$ , $5);
                        deleteNode($2); 
                        deleteNode($4); 
                        deleteNode($6);}; 
                | lambda {
                        $$ = newNode(NODE_DECL); //handle declaration problem ex: array int real string_v
                    };

type : standard_type {
            $$ = $1;
        };
        | ARRAY LBRAC DIGSEQ DOTDOT DIGSEQ RBRAC OF type {
            $$ = newNode(NODE_TYPE_ARRAY);
            addChild($$ , $3);
            addChild($$ , $5);
            addChild($$ , $8);
            deleteNode($1);
            deleteNode($2);
            deleteNode($4);
            deleteNode($6);
            deleteNode($7);
        };

standard_type : INTEGER {
                $$ = $1;
                $$->nodeType = NODE_TYPE_INT;};
        | real   {
            $$ = $1;
            $$->nodeType = NODE_TYPE_REAL;};
        | STRING {
            $$ = $1;
            $$->nodeType = NODE_TYPE_STRING; };


subprogram_declarations :
	subprogram_declarations subprogram_declaration SEMICOLON {
                                    $$ = $1;
                                    addChild($$ , $2);
                                    deleteNode($3);
                                    };
	| lambda {
            $$ = newNode(NODE_SUB_DECLS);
        };

subprogram_declaration : subprogram_head
                             declarations
                            compound_statement {
                $$ = newNode(NODE_LIST);
                addChild($$ , $1);
                addChild($$ , $2);
                addChild($$ , $3);};

subprogram_head : FUNCTION IDENTIFIER arguments COLON standard_type SEMICOLON { //create symbol table
                    $$ = newNode(NODE_FUN_HEAD);
                    deleteNode($1);
                    addChild($$ , $2);
                    addChild($$ , $3);
                    addChild($$ , $5);
                    deleteNode($4);
                    deleteNode($6);};
	| PROCEDURE IDENTIFIER arguments SEMICOLON { //create symbol table
                    $$ =  newNode(NODE_PRO_HEAD);
                    deleteNode($1);
                    addChild($$ , $2);
                    addChild($$ , $3);
                    deleteNode($4);};

arguments : LPAREN parameter_list RPAREN {
                    $$ = $2;
                    deleteNode($1);
                    deleteNode($3);};
	| lambda {
            $$ = newNode(NODE_lambda);};


parameter_list : optional_var identifier_list COLON type {
                        $$ = $1;
                        addChild($$ , $2);
                        addChild($$ , $4);
                        deleteNode($3);};
	| optional_var identifier_list COLON type SEMICOLON parameter_list {
            $$ = $6;
            addChild($$ , $2);
            addChild($$ , $4);
            //addChild($$ , $6);
            deleteNode($3);
            deleteNode($5);};

optional_var   : VAR {
                        $$ = newNode(NODE_DECL);
                        deleteNode($1);
                     };
        | lambda {$$ = newNode(NODE_DECL);};


compound_statement : begin
		       optional_statements
		       END {    // create symbol table
                        $$ = newNode(NODE_BEGIN);
                        addChild($$ , $2);
                        struct node * temp = newNode(NODE_END);
                        addChild($$ , temp);
                        deleteNode($1);
                        deleteNode($3);
                    };


optional_statements : statement_list {
                            $$ = $1;};


statement_list : statement {
                    $$ = newNode(NODE_LIST);
                    addChild($$ , $1);};
	| statement_list SEMICOLON statement {
                    $$ = $1;
                    addChild($$ , $3);
                    deleteNode($2);};

statement : variable ASSIGNMENT expression {
                $$ = newNode(NODE_ASSIGN_STMT);
                addChild($$ , $1);
                addChild($$ , $3);
                $1->nodeType = NODE_SYM_REF; //check declaration
                deleteNode($2);};
	| procedure_statement {
                $$ = $1;
                          };
	| compound_statement {
                $$ = $1;
                         };
	| IF expression THEN statement ELSE statement { 
                        //semantic if here
                        $$ = newNode(NODE_IF);
                        addChild($$ , $2);
                        //addChild($$ , $3);
                        addChild($$ , $4);
                        //addChild($$ , $5);
                        addChild($$ , $6);
                        deleteNode($1);
                        //deleteNode($3);
                        //deleteNode($5);
                        };
	| WHILE expression DO statement { 
            $$ = newNode(NODE_WHILE);
            addChild($$ , $2);
            addChild($$ , $4);
            deleteNode($1);
            deleteNode($3);};
	| lambda {$$ = newNode(NODE_LIST);};


variable : IDENTIFIER tail {
                $$ = newNode(NODE_SYM_REF);
                $$->string =  $1->string;
                addChild($$ , $2);};

tail     : LBRAC expression RBRAC tail { 
                    $$ = $4;
                    addChild($$ , newOpNode(OP_LBRAC));
                    addChild($$ , $2);
                    addChild($$ , newOpNode(OP_RBRAC));
                    deleteNode($1);
                    deleteNode($3);
                    };

	| lambda {$$ = newNode(NODE_LIST);};


procedure_statement : IDENTIFIER {
                            $$ = newNode(NODE_SYM_REF);//without parameter
                            $$->string = $1->string;
                            deleteNode($1);
                                 };
	| IDENTIFIER LPAREN expression_list RPAREN {
                $$ = newNode(NODE_SYM_REF);
                $$->string = $1->string;
                deleteNode($1);
                addChild($$ , $3);
                deleteNode($2);
                deleteNode($4);};

expression_list : expression {
                    $$ = newNode(NODE_LIST);
                    addChild($$ , $1);
                             };
	| expression_list COMMA expression {
            $$ = $1;
            addChild($$ , $3);
            deleteNode($2);};

expression : simple_expression {
                        $$ = $1;
                               };
	| simple_expression relop simple_expression {
                $$ = $2;
                addChild($$ , $1);
                addChild($$ , $3);};


simple_expression : term {
                        $$ = $1;
                         };
	| simple_expression addop term {
            $$ = $2;
            addChild($$ , $1);
            addChild($$ , $3);};


term : factor {
            $$ = $1;
              };
	| term mulop factor {
            $$ = $2;
            addChild($$ , $1);
            addChild($$ , $3);};


factor : IDENTIFIER tail { //call declared variable , function , procedure 
            $$ = newNode(NODE_SYM_REF);
            $$->string = $1->string;
            addChild($$ , $2);
                         };
	| IDENTIFIER LPAREN expression_list RPAREN {
            $$ = newNode(NODE_SYM_REF);
            $$->string = $1->string;
            deleteNode($1);
            addChild($$ , $3);
            deleteNode($2);
            deleteNode($4);};
	| DIGSEQ {
        $$ = $1;
        $$->nodeType = NODE_INT;
             };
    | addop DIGSEQ {
            $$ = $1;
            addChild($$ , $2);         
                   };
    | REALNUMBER {
        $$ = $1;
        $$->nodeType = NODE_REAL;
                 };
    | addop REALNUMBER {
            $$ = $1;
            addChild($$ , $2);
                       };
	| LPAREN expression RPAREN {
                $$ = $2;
                deleteNode($1);
                deleteNode($3);};
	| NOT factor {
                    $$ = $2;
                    addChild($$ , $1);};
    | string_v {
            $$ = $1;
            $$->nodeType = NODE_STRING_v;};

addop : PLUS {
                $$ = newOpNode(OP_ADD);
                deleteNode($1);};
        | MINUS {
                $$ = newOpNode(OP_SUB);
                deleteNode($1);};


mulop : STAR {
                $$ = newOpNode(OP_MUL);
                deleteNode($1);};
        | SLASH {
                $$ = newOpNode(OP_DIV);
                deleteNode($1);};

relop : LT {
            $$ = newOpNode(OP_LT);
            deleteNode($1);};
	| GT {
            $$ = newOpNode(OP_GT);
            deleteNode($1);};
	| EQUAL {
            $$ = newOpNode(OP_EQ);
            deleteNode($1);};
	| LE {
            $$ = newOpNode(OP_LE);
            deleteNode($1);};
	| GE {
            $$ = newOpNode(OP_GE);
            deleteNode($1);};
	| notEQUAL {
            $$ = newOpNode(OP_NE);
            deleteNode($1);};

lambda : ;

%%

struct node * newOpNode(int op) {
            struct node * node = newNode(NODE_OP);
            node->op = op;

            return node;
    }

int main(int argc, char** argv) {
    int res;
    
    fprintf(stderr, "open file.\n");
    if(argc>1 && freopen(argv[1],"r",stdin)==NULL){
        exit(1);
    }
    
    fprintf(stderr, "call yyparse\n");
    res = yyparse();
    fprintf(stderr, "after call yyparse, res = %d.\n", res);
    
    if (res==0)
        fprintf(stderr, "SUCCESS\n NO syntax error !\n");
    else
        fprintf(stderr, "ERROR\n");
    
    printf("-----------------------------------------------\n");
    printTree(ASTRoot, 0);

    SymbolTable.size = 0;
    SymbolTable.current_level = 0;
    printf("New scope created\n");
    semanticCheck(ASTRoot);
    printf("********************************\n"
           "*      No semantic error!      *\n"
          "********************************\n");
    
    return 0;
}