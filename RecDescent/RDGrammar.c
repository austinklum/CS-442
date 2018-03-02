#include <stdbool.h>

#include "IOMngr.h"
#include "RDGrammar.h"
#include "RDTokens.h"

bool Prog()
// <Prog>     :==  Ident { <StmtSeq> }
{
  ENTER;

  if (!Match(IDENT_TOK)) return LEAVE_FAIL;
  if (!Match(LBRACE_TOK)) return LEAVE_FAIL;
  if (!StmtSeq()) return LEAVE_FAIL;
  if (!Match(RBRACE_TOK)) return LEAVE_FAIL;

  return LEAVE_SUCC;
}

bool StmtSeq()
// <StmtSeq>  :==  <Stmt> ; <StmtSeq>
// <StmtSeq>  :==
{
  ENTER;

  switch (CurrentToken()) {
  case INT_TOK:
  case CHR_TOK:
  case IDENT_TOK:
    break;
  case RBRACE_TOK:
    break;
  default:
    ParseError("No option in switch");
    return LEAVE_FAIL;
    break;
  }

  return LEAVE_SUCC;
}

bool Stmt()
// <Stmt>    :== <Decl>
// <Stmt>    :== <Assign>
{
  ENTER;

  return LEAVE_SUCC;
}

bool Decl()
// <Decl>    :== <Type> <IDLst>              INT CHR
{
	ENTER;
	if(!Type()) return LEAVE_FAIL;
	if(!IDLst()) return LEAVE_FAIL;
	return LEAVE_SUCC;
}

bool Type()
// <Type>    :== INT
// <Type>    :== CHR
{
	ENTER;
	switch (CurrentToken()) {
		case INT_TOK:
			if(!Match(INT_TOK)) return LEAVE_FAIL;
			break;
		case CHR_TOK:
			if(!Match(CHR_TOK)) return LEAVE_FAIL;
			break;
		default:
			ParseError("Unknown Type");
			return LEAVE_FAIL;
	}
	return LEAVE_SUCC;
}

bool IDLst()
// <IDLst>   :== Ident <MLst>
{
	ENTER;
	if(!Match(IDENT_TOK)) return LEAVE_FAIL;
	if(!MLst()) return LEAVE_FAIL;
	return LEAVE_SUCC;
}

bool MLst()
// <MLst>    :== , <IDLst>
// <MLst>    :==
{
	ENTER;
	switch(CurrentToken()){
		case COMMA_TOK:
			if(!Match(COMMA_TOK)) return LEAVE_FAIL;
			if(!IDLst()) return LEAVE_FAIL;
			break;
		default:
			ParseError("Unknown MLst");
			return LEAVE_FAIL;
	}

	return LEAVE_SUCC;
	//TODO I'm not quite sure about the blank RHS

}

bool Assign()
// <Assign>  :==  <Ident> := <Expr>
{
	//TODO not sure about := being a single token or 2 tokens.
	ENTER;
	if(!Match(IDENT_TOK)) return LEAVE_FAIL;
	if(!Match(':=')) return LEAVE_FAIL;
	if(!Expr()) return LEAVE_FAIL;
	return LEAVE_SUCC;
}

bool Expr()
// <Expr>    :==  <Term> <MExpr>
{
	ENTER;
	if(!Term()) return LEAVE_FAIL;
	if(!MExpr()) return LEAVE_FAIL;
	return LEAVE_SUCC;
}

bool MExpr()
// <MExpr>   :==  <AddOp> <Term> <MExpr>
// <MExpr>   :==
{
	ENTER;
	if(!AddOp()) return LEAVE_FAIL;
	if(!Term()) return LEAVE_FAIL;
	if(!MExpr()) return LEAVE_FAIL;
	return LEAVE_SUCC;
}

bool Term()
//<Term>     :==  <Factor> <MTerm>
{
	ENTER;
	if(!Factor()) return LEAVE_FAIL;
	if(!MTerm()) return LEAVE_FAIL;
	return LEAVE_SUCC;
}

bool MTerm()
//<MTerm>    :==  <MultOp> <Factor> <MTerm>
//<MTerm>    :==
{
	ENTER;
	if(!MultOp()) return LEAVE_FAIL;
	if(!Factor()) return LEAVE_FAIL;
	if(!MTerm()) return LEAVE_FAIL;
	return LEAVE_SUCC;
}

bool Factor()
// <Factor>  :==  ( <Expr> )
// <Factor>  :==  - <Factor>
// <Factor>  :==  <IntLit>
// <Factor>  :==  <FloatLit>
// <Factor>  :==  <Ident>
{
}

bool AddOp()
// <AddOp>    :==  -
// <AddOp>    :==  +
{
}

bool MultOp()
// <MultOp>   :==  *
// <MultOp>   :==  /
{
}
