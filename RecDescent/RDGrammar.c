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
  	  //TODO I have two perfectly valid RHS. I can't use a switch on a token as these are functions.
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
		case CHR_TOK:
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
			if(!IDLst()) return LEAVE_FAIL;
			break;
		default:
			ParseError("Unknown MLst.");
			return LEAVE_FAIL;
	}

	return LEAVE_SUCC;
	//TODO I'm not quite sure about the blank RHS

}

bool Assign()
// <Assign>  :==  <Ident> := <Expr>
{
	//not sure about := being a single token or 2 tokens.
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
	switch (CurrentToken()){
		case MINUS_TOK:
		case PLUS_TOK:
			if(!Term()) return LEAVE_FAIL;
			if(!MExpr()) return LEAVE_FAIL;
			break;
		default:
			if(!Match(SEMI_TOK) || !Match(RPAREN_TOK)) return LEAVE_FAIL;
	}
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
	ENTER;
	switch(CurrentToken()){
		case LPAREN_TOK:
			if(!Expr()) return LEAVE_FAIL;
			if(!Match(RPAREN_TOK)) return LEAVE_FAIL;
			break;
		case MINUS_TOK:
			if(!Factor()) return LEAVE_FAIL;
			break;
		case INTLIT_TOK:
		case FLOATLIT_TOK:
		case IDENT_TOK:
			break;
		default:
			ParseError("Unknown Factor.");
			return LEAVE_FAIL;
	}
	return LEAVE_SUCC;
}

bool AddOp()
// <AddOp>    :==  -
// <AddOp>    :==  +
{
	ENTER;
	switch(CurrentToken()){
		case MINUS_TOK:
		case PLUS_TOK:
			break;
		default:
			ParseError("Unknown AddOp.");
			return LEAVE_FAIL;
	}
	return LEAVE_SUCC;
}

bool MultOp()
// <MultOp>   :==  *
// <MultOp>   :==  /
{
	ENTER;
	switch(CurrentToken()){
		case TIMES_TOK:
		case DIV_TOK:
			break;
		default:
			ParseError("Unknown MultOp.");
			return LEAVE_FAIL;
	}
	return LEAVE_SUCC;
}
