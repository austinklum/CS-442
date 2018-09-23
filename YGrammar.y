%{
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include "IOMngr.h"
  #include "Scanner.h"
  #include "YSemantics.h"

  void yyerror(char *s);
  %}

/* Union structure of data that can be attached to non-terminals   */
%union     {
// real-type union-field-name
  int Integer;
  char * Text;
  struct IdList * IdList;
  enum BaseTypes BaseType;
  struct InstrSeq * InstrSeq;
  struct ExprResult * ExprResult;
  struct CondResult * CondResult;
  enum BranchType BranchType;
}

/* Type declaration for data attached to non-terminals. Allows     */
/* $# notation to be correctly type casted for function arguments. */
//    <union-field-name> non-terminal
%type <Text> Id
%type <IdList> DeclList
%type <IdList> DeclItem
%type <InstrSeq> DeclImpls
%type <BaseType> Type
%type <InstrSeq> FuncBody
%type <InstrSeq> FuncStmts
%type <InstrSeq> Stmt
%type <InstrSeq> AssignStmt
%type <Text> AddOp
%type <Text> MultOp
%type <ExprResult> Factor
%type <ExprResult> Term
%type <ExprResult> Expr
%type <Text> INTLIT_TOK
%type <Text> CHRLIT_TOK
%type <Text> INT_TOK
%type <CondResult> Cond
%type <BranchType> Branch
%type <InstrSeq> IfStmt
%type <InstrSeq> Else
%type <InstrSeq> WhileStmt
%type <Text> StrLit
%type <InstrSeq> IncrDecr
%type <InstrSeq> ForStmt
%type <InstrSeq> CallFunc
%type <InstrSeq> ReturnStmt
%type <Integer> Index

/* List of token name and corresponding numbers */
/* y.tab.h will be generated from these */
%token IDENT_TOK  	1
%token DECL_TOK   	2
%token IMPL_TOK   	3
%token CHR_TOK      4
%token INT_TOK      5
%token INTLIT_TOK   6
%token CHRLIT_TOK   7
%token GET_TOK      8
%token PUT_TOK      9
%token BGE_TOK      10
%token BGT_TOK      11
%token BLE_TOK      12
%token BLT_TOK      13
%token BNE_TOK      14
%token BEQ_TOK      15
%token WHILE_TOK    16
%token IF_TOK       17
%token ELSE_TOK     18
%token STRLIT_TOK   19
%token INCR_TOK     20
%token DECR_TOK     21
%token FOR_TOK      22
%token RET_TOK      23
%token CNST_TOK     24
%token AND_TOK      25
%token OR_TOK       26
%token NOT_TOK      27
%token INDEX_TOK    28
// can't go past 32 without conflicting with single char tokens
// could use larger token numbers


%%

Module        : DeclImpls                                       { Finish(); };

DeclImpls     : Decl DeclImpls                                  { };
DeclImpls     : Impl DeclImpls                                  { };
DeclImpls     :                                                 { };

Decl          : CNST_TOK DeclItem ':' Type '=' Expr ';'         { ProcDecls($2,$4,1); ProcAssignStmtCnst($2,$6); };
Decl          : DECL_TOK Id Index ':' Type ';'                  { ProcArray($2,$3); };
Decl          : DECL_TOK DeclList ':' Type  ';'                 { ProcDecls($2,$4,0); };
DeclList      : DeclItem ',' DeclList                           { $$ = AppendIdList($1,$3); };
DeclList      : DeclItem                                        { $$ = $1; };

DeclItem      : Id                                              { $$ = ProcName($1,PrimType); };
DeclItem      : Id FuncArgTypes                                 { $$ = ProcName($1,FuncType); };

Id            : IDENT_TOK                                       { $$ = strdup(yytext); };
FuncArgTypes  : '(' ')'                                         {  };

Type          : INT_TOK                                         { $$ = IntBaseType; };
Type          : CHR_TOK                                         { $$ = ChrBaseType; };

Impl          : IMPL_TOK Id FuncArgNames FuncBody ';'           { ProcFunc($2,$4); };
FuncArgNames  : '(' ')'                                         {  };
FuncBody      : '{' FuncStmts '}'                               { $$ = $2; };

FuncStmts     : Stmt ';' FuncStmts                              { $$ = ProcFuncStmts($1,$3); };
FuncStmts     :                                                 { $$ = NULL; };

Stmt          : AssignStmt                                      { $$ = $1; };

Stmt          : PUT_TOK '(' Factor ')'                          { $$ = ProcPut($3); };
Stmt          : PUT_TOK '(' StrLit ')'                          { $$ = ProcPutStr($3); };

AssignStmt    : Id '=' CallFunc                                 { $$ = ProcAssignStmtCallFunc($1,$3); };
AssignStmt    : Id '=' Expr                                     { $$ = ProcAssignStmt($1,$3); };
Stmt          : IncrDecr                                        { $$ = $1; };
Expr          : Expr AddOp Term                                 { $$ = ProcOp($1,$2,$3); };
Expr          : Term                                            { $$ = $1; };
Term          : Term MultOp Factor                              { $$ = ProcOp($1,$2,$3); };
Term          : Factor                                          { $$ = $1; };
IncrDecr      : Id INCR_TOK                                     { $$ = ProcIncrDecr($1,1); };
IncrDecr      : Id DECR_TOK                                     { $$ = ProcIncrDecr($1,-1); };
Factor        : GET_TOK '(' INT_TOK ')'                         { $$ = ProcGet(); };
Factor        : '(' Expr ')'                                    { $$ = $2; };
Factor        : '-' Factor                                      { /*$$ = ProcNegateFactor($2);*/ };
Factor        : CHRLIT_TOK                                      { $$ = ProcLiteral(yytext, ChrBaseType); };
Factor        : INTLIT_TOK                                      { $$ = ProcLiteral(yytext, IntBaseType); };
StrLit        : STRLIT_TOK                                      { $$ = ProcStrLiteral(yytext); };
Factor        : Id                                              { $$ = ProcIdExprResult($1); };
AddOp         : '+'                                             { $$ = strdup(yytext); };
AddOp         : '-'                                             { $$ = strdup(yytext); };
MultOp        : '*'                                             { $$ = strdup(yytext); };
MultOp        : '/'                                             { $$ = strdup(yytext); };

/* Part 3 */
Stmt         : IfStmt                                          { $$ = $1; };
Stmt         : WhileStmt                                       { $$ = $1; };
WhileStmt    : WHILE_TOK '(' Cond ')' FuncBody                 { $$ = ProcWhileStmt($3,$5); };
IfStmt       : IF_TOK '(' Cond ')' FuncBody Else               { $$ = ProcIfStmt($3,$5,$6); };
Cond         : Expr Branch Expr                                { $$ = ProcCond($1,$2,$3); };
Else         : ELSE_TOK FuncBody                               { $$ = $2; };
Else         :                                                 { $$ = NULL; };
Branch       : BGE_TOK                                         { $$ = Bge; };
Branch       : BGT_TOK                                         { $$ = Bgt; };
Branch       : BLE_TOK                                         { $$ = Ble; };
Branch       : BLT_TOK                                         { $$ = Blt; };
Branch       : BNE_TOK                                         { $$ = Bne; };
Branch       : BEQ_TOK                                         { $$ = Beq; };
 /* Part 5 */

 /* For Loops */
Stmt         : ForStmt                                          { $$ = $1; };
ForStmt      : FOR_TOK '(' AssignStmt ';' Cond ';' IncrDecr ')' FuncBody { $$ = ProcForLoop($3,$5,$7,$9); };

 /*Parameterless Functions*/
Stmt         : CallFunc                                        { $$ = $1; };
Stmt         : ReturnStmt                                      { $$ = $1; };
CallFunc     : Id '(' ')'                                      { $$ = ProcFuncCall($1); };
ReturnStmt   : RET_TOK Expr                                    { $$ = ProcReturnStmt($2); };

 /* Boolean */
 /*
 Bool: Bool BoolOp Cond
 Bool: Cond BoolOp Cond                                         { $$ = ProcBool($1,$2,$3); };
 Bool: NOT_TOK Cond                                             { $$ = ProcNotCond($1,$2); };
 BoolOp: AND_TOK                                                { $$ = and; };
 BoolOp: OR_TOK                                                 { $$ = or;  };
 */

/* Arrays */
Factor      : Id Index                                         { $$ = ProcArrayIndexing($1,$2); };
Factor      : Id '[' Expr ']'                                  { $$ = ProcArrayIndexing($1,$3); };
Index       : INDEX_TOK                                        { $$ = ProcIndex(strdup(yytext)); };
%%

void
yyerror( char *s)     {
  char msg[MAXLINE];
  sprintf(msg,"ERROR \"%s\" token: \"%s\"",s,yytext);
  PostMessage(GetCurrentColumn(),strlen(yytext),msg);
}
