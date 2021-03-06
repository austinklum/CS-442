%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IOMngr.h"

extern int yylex(); /* The next token function. */
extern char *yytext; /* The matched token text. */
extern int yyleng; /* The token text length. */

void yyerror(char *s);

#define YYSTYPE long /* 64 bit so can hold pointer and int */

%}

// These token definitions populate y.tab.h
// Single character tokens are their own token numbers (e.g. scanner returns
// the value ';' for the semicolon token)
%token INT_TOK 1
%token CHR_TOK 2
%token ASSIGN_TOK 3
%token INTLIT_TOK 4
%token IDENT_TOK 5

%%
Prog    : IDENT_TOK '{' StmtSeq '}'                                               ;
StmtSeq : Stmt ';' StmtSeq                                                        ;
StmtSeq :                                                                         ;

Assign  : LHS ASSIGN_TOK Expr                    { printf("%s =\n",(char *)$1); } ;
LHS     : IDENT_TOK                               { $$ = (long) strdup(yytext); } ;

Stmt    : Decl																	  ;
Stmt	: Assign																  ;
Decl	: Type IDLst															  ;
Type	: INT_TOK																  ;
Type	: CHR_TOK																  ;
IDLst   : IDENT_TOK MLst														  ;
MLst    : ',' IDLst;
MLst    : ;
Expr    : Term MExpr;
MExpr   : AddOp Term MExpr;
MExpr   : ;
Term    : Factor MTerm;
MTerm   : MultOp Factor MTerm;
MTerm   : ;
Factor  :  '(' Expr ')';
Factor  :  '-' Factor;
Factor  : INTLIT_TOK;
Factor  : IDENT_TOK;
AddOp   : '-';
AddOp   : '+';
MultOp  : '*';
MultOp  : '/';
.					         	
%%

void
yyerror(char *s) {
 // Called when parser encounters error. Write newline to stdout in case partial
 // postfix expression has been generated. Format error message and post to
 // current line. IOMngr will then echo error line along with error message.
 char msg[256];
 printf("\n");
 snprintf(msg,255,"error: \"%s\" yytext: \"%s\"",s,yytext);
 PostMessage(GetCurrentColumn(),yyleng,msg);
}
