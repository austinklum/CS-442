/* Semantics.h
 The action and supporting routines for performing semantics processing.
 */

#include "SymTab.h"
#include "IOMngr.h"
#include "Scanner.h"
#include "YCodeGen.h"

/* Declaration of semantic record data types used in grammar.y %union */
struct IdList {
  struct SymEntry * entry;
  struct IdList * next;
};

enum BaseTypes { IntBaseType, ChrBaseType, StrBaseType };
enum Ops { Add, Sub, Mult, Div };
enum BranchType { Bge, Bgt, Ble, Blt, Bne, Beq};
struct FuncDesc {
  enum BaseTypes returnType;
  struct InstrSeq * funcCode;
};


enum DeclTypes { PrimType, FuncType, ArrayType};
struct TypeDesc {
  enum DeclTypes declType;
  union {
    enum BaseTypes primDesc;
    struct FuncDesc * funcDesc;
  };
};

struct ExprResult {
  int reg;
  struct InstrSeq * instrs;
  enum BaseTypes type;
};

struct CondResult {
  char * label;
  struct InstrSeq * instrs;
};

// Symbol Table Structures
struct Attr {
  struct TypeDesc * typeDesc;
  char * reference;
  int cnst;
  int arrayLen;
};

// Supporting Routines

void InitSemantics();
void ListIdentifierTable();

struct TypeDesc * MakePrimDesc(enum BaseTypes type);
struct TypeDesc * MakeFuncDesc(enum BaseTypes returnType);

// Semantics Actions
void                    Finish();

void                    ProcDecls(struct IdList * idList, enum BaseTypes baseType, int cnst);
struct IdList *         AppendIdList(struct IdList * item, struct IdList * list);
struct IdList *         ProcName(char * id, enum DeclTypes type);
void                    ProcFunc(char * id, struct InstrSeq * instrs);

struct ExprResult * ProcOp(struct ExprResult * term, char * op , struct ExprResult * factor);
struct ExprResult * ProcIdExprResult (char * id);
struct ExprResult * ProcLiteral(char * literal, enum BaseTypes type);
struct ExprResult * ProcGet();
struct InstrSeq * ProcPut(struct ExprResult * expr);
struct InstrSeq * ProcPutId(char * id);
struct InstrSeq * ProcPutChar(char * literal);
struct InstrSeq * ProcAssignStmt(char * id,struct ExprResult * expr);
struct InstrSeq * ProcFuncStmts(struct InstrSeq * stmt, struct InstrSeq * funcStmts);
/**
Part 3
*/
struct CondResult * ProcCond(struct ExprResult * lhs,enum BranchType branch, struct ExprResult * rhs);
struct InstrSeq * ProcIfStmt(struct CondResult * cond, struct InstrSeq * ifBody, struct InstrSeq * elseBody);
struct InstrSeq * ProcWhileStmt(struct CondResult * cond, struct InstrSeq * whileBody);
/**
Part 4
*/
struct InstrSeq * ProcPutStr(char * str);
char * ProcStrLiteral(char * str);
void processGlobalStrings(struct SymEntry * entry, int cnt, void * dataCode);
/**
Part 5
*/
struct InstrSeq * ProcIncrDecr(char * id, int incrDecrNum);
struct InstrSeq * ProcForLoop(struct InstrSeq * assignment, struct CondResult * cond, struct InstrSeq * incrDecr, struct InstrSeq * body);
struct InstrSeq * ProcFuncCall(char * id);
struct InstrSeq * ProcReturnStmt(struct ExprResult * expr);
struct InstrSeq * ProcAssignStmtCallFunc(char * id,struct InstrSeq * instrs);
void ProcAssignStmtCnst(struct IdList * idList,struct ExprResult * expr);
//struct CondResult * ProcBool(struct CondResult * lhs, enum BoolOp, struct CondResult * rhs);
int ProcIndex(char * index);
struct ExprResult * ProcArrayIndexing(char * id, int index);
void ProcArray(char * id, int index);
/**Helpers*/
void FreeExprResult(struct ExprResult * expr);
enum Ops GetOpFromChar(char * op);
enum BranchType GetBranchType(char * branch);
char * BranchToString(enum BranchType branch);
