/* Semantics.c
 Support and semantic action routines.

 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "YSemantics.h"

// Shared Data

struct SymTab * IdentifierTable;
struct SymTab * StringTable;
struct InstrSeq * cnstAssignments;
enum AttrKinds { VOID_KIND = -1, INT_KIND, STRING_KIND, STRUCT_KIND };

char * BaseTypeNames[2] = { "int", "chr"};
char * TypeNames[2] = { "", "func"};

// corresponds to enum Operators
char * Ops[] = { "add", "sub", "mul", "div"};
char * BoolOps[] = { "and", "or", "not" };

// corresponds to negation of enum Comparisons
// enum Comparisons { LtCmp, LteCmp, GtCmp, GteCmp, EqCmp, NeqCmp };
char * Branches[] = { "bge", "bgt", "ble", "blt", "bne", "beq"};
// comparison set instructions, in order with enum Comparisons
char * CmpSetReg[] = { "slt", "sle", "sgt", "sge", "seq", "sne" };


// Supporting Routines
void
PostMessageAndExit(int col, char * message) {
  PostMessage(col,1,message);
  CloseSource();
  exit(0);
}

void
InitSemantics() {
  IdentifierTable = CreateSymTab(100,"global",NULL);
  StringTable = CreateSymTab(100,"strings",NULL);
  cnstAssignments = GenInstr("_cnsts",NULL,NULL,NULL,NULL);
}

char *
StringForType(struct TypeDesc * desc) {
  switch (desc->declType) {
    case PrimType:
    case ArrayType: {
      return strdup(BaseTypeNames[desc->primDesc]);
    } break;
    case FuncType: {
      return strdup(BaseTypeNames[desc->funcDesc->returnType]);
    } break;
  }
}

struct TypeDesc *
MakePrimDesc(enum BaseTypes type) {
  struct TypeDesc * desc = malloc(sizeof(struct TypeDesc));
  desc->declType = PrimType;
  desc->primDesc = type;
  return desc;
}

struct TypeDesc *
MakeFuncDesc(enum BaseTypes returnType) {
  struct TypeDesc * desc = malloc(sizeof(struct TypeDesc));
  desc->declType = FuncType;
  desc->funcDesc = malloc(sizeof(struct FuncDesc));
  desc->funcDesc->returnType = returnType;
  desc->funcDesc->funcCode = NULL;
  return desc;
}

void displayEntry(struct SymEntry * entry, int cnt, void * ignore) {
  char * scope = GetScopePath(GetTable(entry));
  printf("%3d %-20s %-15s",cnt,scope,GetName(entry));
  free(scope);
  int attrKind = GetAttrKind(entry);
  struct Attr * attr = GetAttr(entry);
  switch (attrKind) {
    case VOID_KIND: {
    } break;
    case INT_KIND: {
    } break;
    case STRING_KIND: {
    } break;
    case STRUCT_KIND: {
      if (attr) {
        printf("%-10s",attr->reference);
        printf("     ");
        struct TypeDesc * desc = attr->typeDesc;
        char * typeStr = StringForType(desc);
        printf("%-10s ",typeStr);
        free(typeStr);
      }
      else {
        printf("empty");
      }
    } break;
  }
  printf("\n");
}

void
ListIdentifierTable() {
  printf("\nIdentifier Symbol Table\n");
  printf("Num Scope                Name           Ref       Kind Type\n");
  InvokeOnEntries(IdentifierTable,true,displayEntry,0,NULL);
}

void
FreeTypeDesc(struct TypeDesc * desc) {
  switch (desc->declType) {
    case PrimType:
    case ArrayType: {
    } break;
    case FuncType: {
      if (desc->funcDesc->funcCode) FreeSeq(desc->funcDesc->funcCode);
      free(desc->funcDesc);
    } break;
  }
  free(desc);
}

// provided to the SymTab for destroying table contents
void
FreeEntryAttr(struct SymEntry * entry) {
  int attrKind = GetAttrKind(entry);
  struct Attr * attr = GetAttr(entry);
  switch (attrKind) {
    case VOID_KIND: {
    } break;
    case INT_KIND: {
    } break;
    case STRING_KIND: {
    } break;
    case STRUCT_KIND: {
      if (attr) {
        if (attr->typeDesc)  FreeTypeDesc(attr->typeDesc);
        if (attr->reference) free(attr->reference);
        free(attr);
      }
    } break;
  }
}

void processGlobalIdentifier(struct SymEntry * entry, int cnt, void * dataCode) {
  struct Attr * attr = GetAttr(entry);
  switch (attr->typeDesc->declType) {
    case PrimType: {
       AppendSeq(dataCode,GenInstr(attr->reference,".word", "0",NULL,NULL));
    } break;
    case FuncType: {
      // nothing to do here
    } break;
    case ArrayType: {
      AppendSeq(dataCode,GenInstr(attr->reference,".space",Imm(attr->arrayLen*4),NULL,NULL));
    }
  }
}

void processGlobalStrings(struct SymEntry * entry, int cnt, void * dataCode) {
  if(cnt == 0){
    AppendSeq(dataCode,GenInstr("__iobuf",".space","4",NULL,NULL));
  }
  struct Attr * attr = GetAttr(entry);
  AppendSeq(dataCode,GenInstr(attr->reference,".asciiz",strdup(GetName(entry)),NULL,NULL));
}

void processFunctions(struct SymEntry * entry, int cnt, void * textCode) {
  struct Attr * attr = GetAttr(entry);
  switch (attr->typeDesc->declType) {
    case PrimType:
    case ArrayType: {
      // nothing to do here
    } break;
    case FuncType: {
      if (!attr->typeDesc->funcDesc->funcCode) {
        PostMessageAndExit(GetCurrentColumn(),"function never implemented");
      }
      AppendSeq(textCode,attr->typeDesc->funcDesc->funcCode);
    } break;
  }
}

// Semantics Actions

void
Finish() {
  printf("Finish\n");
  ListIdentifierTable();

  struct InstrSeq * textCode = GenInstr(NULL,".text",NULL,NULL,NULL);
  struct InstrSeq * dataCode = GenInstr(NULL,".data",NULL,NULL,NULL);

  // form module preamble
  struct SymEntry * mainEntry = LookupName(IdentifierTable,"main");
  if (!mainEntry) {
    PostMessageAndExit(GetCurrentColumn(),"no main function for module");
  }
  // should make sure main takes no arguments
  struct Attr * mainAttr = GetAttr(mainEntry);
  // need to keep spim happy
  AppendSeq(textCode,GenInstr(NULL,".globl","__start",NULL,NULL));
  AppendSeq(textCode,GenInstr("__start",NULL,NULL,NULL,NULL));
  AppendSeq(textCode,cnstAssignments);
  AppendSeq(textCode,GenInstr(NULL,"jal",mainAttr->reference,NULL,NULL));
  AppendSeq(textCode,GenInstr(NULL,"li","$v0","10",NULL));
  AppendSeq(textCode,GenInstr(NULL,"syscall",NULL,NULL,NULL));

  // run SymTab with InvokeOnEntries putting globals in data seg
  InvokeOnEntries(IdentifierTable,true,processGlobalIdentifier,0,dataCode);
  InvokeOnEntries(StringTable,true,processGlobalStrings,0,dataCode);
  // // run SymTab with InvokeOnEntries putting functions in code seq
  InvokeOnEntries(IdentifierTable,true,processFunctions,0,textCode);
  // combine and write

  struct InstrSeq * moduleCode = AppendSeq(textCode,dataCode);
  WriteSeq(moduleCode);

  // free code
  FreeSeq(moduleCode);
  CloseCodeGen();
  fprintf(stderr,"Finish \n");
}
void ProcArray(char * id, int index) {
  struct SymEntry * entry = EnterName(IdentifierTable, id);
		//If there is an attr then the entry must exist already. ERROR.
	if(GetAttr(entry))
		return;

	//New entry. init attribute of entry
	struct Attr * attr = malloc(sizeof(struct Attr));
  attr->typeDesc = malloc(sizeof(struct TypeDesc));
	attr->typeDesc->declType = ArrayType;
  attr->arrayLen = index;
  attr->typeDesc->primDesc = IntBaseType;
  attr->cnst = 0;
  ///malloc of proper size
  char * lbl = malloc(strlen(id) + 2);
  ///Append _ strcat entryname
  strcpy(lbl,"_");
  strcat(lbl,id);

  attr->reference = lbl;
	SetAttr(entry,STRUCT_KIND,attr);
  struct Attr * itemAttr = GetAttr(entry);
}
void
ProcDecls(struct IdList * idList, enum BaseTypes baseType,int cnst) {
 int cnt = 0;
	struct IdList * item = malloc(sizeof(struct IdList));
	struct IdList * needToFree = item;
	item->next = idList;
	//struct IdList * item = idList;
  // walk IdList items
	while((item = item->next)) {
		struct Attr * itemAttr = GetAttr(item->entry);
    itemAttr->cnst = cnst;
	 // switch for prim or func type
		switch(itemAttr->typeDesc->declType) {
		  // names on IdList are only specified as PrimType or FuncType
			case PrimType:
				itemAttr->typeDesc->primDesc = baseType;
				break;
			case FuncType:
			// set type desc
      itemAttr->typeDesc->funcDesc = malloc(sizeof(struct FuncDesc));
			// attr reference string is id name with prepended "_"
				itemAttr->typeDesc->funcDesc->returnType = baseType;
				break;
		}
    ///malloc of proper size
    char * lbl = malloc(strlen(GetName(item->entry)) + 2);
    ///Append _ strcat entryname
    strcpy(lbl,"_");
    strcat(lbl,GetName(item->entry));

    itemAttr->reference = lbl;
		// for Sem1 everthing is in global scope, otherwise check scope
	}
	free(needToFree);
}

struct IdList *
AppendIdList(struct IdList * item, struct IdList * list) {
	item->next = list->next;
	list->next = item;
	return list;
}

struct IdList *
ProcName(char * id, enum DeclTypes type) {
  // get entry for id, error if it exists
  // enter id in symtab
  // create IdList node for entry
  // create and partially init type descriptor, completed in ProcDecls
  // create, init and set attr struct

	struct SymEntry * entry = EnterName(IdentifierTable, id);
		//If there is an attr then the entry must exist already. ERROR.
	if(GetAttr(entry))
		return NULL;

	//New entry. init attribute of entry
	struct Attr * attr = malloc(sizeof(struct Attr));
  attr->typeDesc = malloc(sizeof(struct TypeDesc));
	attr->typeDesc->declType = type;
	SetAttr(entry,STRUCT_KIND,attr);

	struct IdList * idList = malloc(sizeof(struct IdList));
	idList->entry = entry;

	return idList;
}

void
ProcFunc(char * id, struct InstrSeq * instrs) {
  // lookup name
	struct SymEntry * entry = LookupName(IdentifierTable, id);
  // get attr
	struct Attr * attr = GetAttr(entry);
  // gen instr for function entry
  // include function body code
  struct InstrSeq * body = AppendSeq(GenInstr(attr->reference, NULL,NULL,NULL,NULL),instrs);
  AppendSeq(body,GenInstr(NULL, "jr","$ra",NULL,NULL));
	attr->typeDesc->funcDesc->funcCode = body;
}

struct ExprResult * ProcGet(){
  struct ExprResult * expr = malloc(sizeof(struct ExprResult));
  expr->reg = AvailTmpReg();
  expr->type = IntBaseType;
  expr->instrs = GenInstr(NULL,"li","$v0","5",NULL);
  AppendSeq(expr->instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(expr->instrs,GenInstr(NULL,"move",TmpRegName(expr->reg),"$v0",NULL));
  return expr;
}

/**
Used to create literal ExprResult
*/
struct ExprResult * ProcLiteral(char * literal, enum BaseTypes type) {
  struct ExprResult * expr = malloc(sizeof(struct ExprResult));
  expr->reg = AvailTmpReg();
  expr->type = type;
  int litVal = 0;
  switch(type){
    case IntBaseType:
        expr->instrs = GenInstr(NULL,"li",TmpRegName(expr->reg),literal,NULL);
      break;
    case ChrBaseType:
      litVal = literal[1];
      if(strlen(literal) > 3){
        switch(literal[2]){
          case 'n':
            litVal = 10;
            break;
        }
      }
      expr->instrs = GenInstr(NULL,"li",TmpRegName(expr->reg),Imm(litVal),NULL);
      break;
  }
  return expr;
}

struct ExprResult * ProcIdExprResult (char * id) {
	struct SymEntry * entry = LookupName(IdentifierTable, id);
	struct Attr * attr = GetAttr(entry);
  struct ExprResult * expr = malloc(sizeof(struct ExprResult));
  expr->reg = AvailTmpReg();
  expr->type = IntBaseType;
  expr->instrs = GenInstr(NULL,"lw",TmpRegName(expr->reg),attr->reference,NULL);
  return expr;
}

struct ExprResult * ProcOp(struct ExprResult * termExpr, char * op , struct ExprResult * factorExpr) {
  struct ExprResult * expr = malloc(sizeof(struct ExprResult));
  expr->type = IntBaseType;
  expr->reg = AvailTmpReg();
  //concatenate all the code and generate a new instruction
  expr->instrs = AppendSeq(termExpr->instrs,factorExpr->instrs);
  AppendSeq(expr->instrs,GenInstr(NULL,Ops[GetOpFromChar(op)],TmpRegName(expr->reg),TmpRegName(termExpr->reg),TmpRegName(factorExpr->reg)));
  //Clean up
  FreeExprResult(termExpr);
  FreeExprResult(factorExpr);
  return expr;
}

struct InstrSeq * ProcPut(struct ExprResult * expr) {
  char * sysOp = malloc(3);
  switch (expr->type) {
    case IntBaseType:
      sysOp[0] = '1';
      sysOp[1] = '\0';
      break;
    case ChrBaseType:
      sysOp[0] = '1';
      sysOp[1] = '1';
      sysOp[2] = '\0';
      break;
  }
  AppendSeq(expr->instrs,GenInstr(NULL,"li","$v0",sysOp,NULL));
  AppendSeq(expr->instrs,GenInstr(NULL,"move","$a0",TmpRegName(expr->reg),NULL));
  AppendSeq(expr->instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  ReleaseTmpReg(expr->reg);
  //  free(expr);
  //  free(sysOp);
  return expr->instrs;
}
/**
Process Assignment Satement
*/
struct InstrSeq * ProcAssignStmt(char * id,struct ExprResult * expr) {
  struct SymEntry * entry = LookupName(IdentifierTable, id);
	struct Attr * attr = GetAttr(entry);
  if(attr->cnst){
     printf("Attempt to change cnst ignored!\n");
     return NULL;
  }
  struct InstrSeq * instrs = AppendSeq(expr->instrs,GenInstr(NULL,"sw",TmpRegName(expr->reg),attr->reference,NULL));
  free(id);
  FreeExprResult(expr);
  return instrs;
}

struct InstrSeq * ProcAssignStmtCallFunc(char * id,struct InstrSeq * instrs) {
  struct SymEntry * entry = LookupName(IdentifierTable, id);
	struct Attr * attr = GetAttr(entry);
  if(attr->cnst){
     printf("Attempt to change cnst ignored!\n");
     return NULL;
  }
  AppendSeq(instrs,GenInstr(NULL,"sw","$v0",attr->reference,NULL));
  free(id);

  return instrs;
}

void ProcAssignStmtCnst(struct IdList * idList,struct ExprResult * expr) {
	struct Attr * attr = GetAttr(idList->entry);
  AppendSeq(cnstAssignments,expr->instrs);
  AppendSeq(cnstAssignments,GenInstr(NULL,"sw",TmpRegName(expr->reg),attr->reference,NULL));
  FreeExprResult(expr);
}
/**
 Part 3
*/

struct CondResult * ProcCond(struct ExprResult * lhs,enum BranchType branch, struct ExprResult * rhs){
  struct CondResult * cond = malloc(sizeof(struct CondResult));
  cond->label = GenLabel();
  cond->instrs = AppendSeq(AppendSeq(lhs->instrs,rhs->instrs),
    GenInstr(NULL,BranchToString(branch),TmpRegName(lhs->reg),TmpRegName(rhs->reg),cond->label));
  FreeExprResult(lhs);
  FreeExprResult(rhs);
  return cond;
}

struct InstrSeq * ProcIfStmt(struct CondResult * cond, struct InstrSeq * ifBody, struct InstrSeq * elseBody){

  struct InstrSeq * instrs = malloc(sizeof(struct InstrSeq));
  if(!elseBody){
    instrs = AppendSeq(cond->instrs,ifBody);
    AppendSeq(instrs,GenInstr(cond->label,NULL,NULL,NULL,NULL));
  } else {
    char * lbl = GenLabel();
    instrs = AppendSeq(cond->instrs,ifBody);
    AppendSeq(instrs,GenInstr(NULL,"b",lbl,NULL,NULL));
    AppendSeq(instrs,GenInstr(cond->label,NULL,NULL,NULL,NULL));
    AppendSeq(instrs,elseBody);
    AppendSeq(instrs,GenInstr(lbl,NULL,NULL,NULL,NULL));
  }
  return instrs;
}

struct InstrSeq * ProcWhileStmt(struct CondResult * cond, struct InstrSeq * whileBody){
  char * lbl = GenLabel();
  struct InstrSeq * instrs = malloc(sizeof(struct InstrSeq));
  instrs = AppendSeq(GenInstr(lbl,NULL,NULL,NULL,NULL),cond->instrs);
  AppendSeq(instrs,whileBody);
  AppendSeq(instrs,GenInstr(NULL,"b",lbl,NULL,NULL));
  AppendSeq(instrs,GenInstr(cond->label,NULL,NULL,NULL,NULL));
  return instrs;
}

/** Part 4 */
struct InstrSeq * ProcPutStr(char * str) {
  struct SymEntry * entry = LookupName(StringTable,str);
  if(!entry){
    printf("NULL entry in PutStr\n");
    return NULL;
  }
  struct Attr * attr = GetAttr(entry);
  if(!attr){
    printf("NULL attr in PutStr!\n");
    return NULL;
  }
  struct InstrSeq * instrs = GenInstr(NULL,"li","$v0","4",NULL);
  AppendSeq(instrs,GenInstr(NULL,"la","$a0",attr->reference,NULL));
  AppendSeq(instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  return instrs;
}

char * ProcStrLiteral(char * strLit) {
  char * str = strdup(strLit);
  struct SymEntry * entry = EnterName(StringTable, str);
		//If there is an attr then the entry must exist already. ERROR.
	if(GetAttr(entry))
		return str;

	//New entry. init attribute of entry
	struct Attr * attr = malloc(sizeof(struct Attr));
  attr->typeDesc = malloc(sizeof(struct TypeDesc));
	attr->typeDesc->declType = PrimType;
  attr->typeDesc->primDesc = StrBaseType;
  attr->reference = GenLabel();
	SetAttr(entry,STRING_KIND,attr);
  return str;
}
/** Part 5 */
struct InstrSeq * ProcIncrDecr(char * id, int incrDecrNum){
  int reg = AvailTmpReg();
  struct SymEntry * entry = LookupName(IdentifierTable, id);
  struct Attr * attr = GetAttr(entry);
  if(attr->cnst){
     printf("Attempt to change cnst ignored!\n");
     return NULL;
  }
  struct InstrSeq * instrs = AppendSeq(GenInstr(NULL,"lw",TmpRegName(reg),attr->reference,NULL),GenInstr(NULL,"addi",TmpRegName(reg),TmpRegName(reg),Imm(incrDecrNum)));
  AppendSeq(instrs,GenInstr(NULL,"sw",TmpRegName(reg),attr->reference,NULL));

  free(id);
  ReleaseTmpReg(reg);

  return instrs;
}

struct InstrSeq * ProcForLoop(struct InstrSeq * assignment, struct CondResult * cond, struct InstrSeq * incrDecr, struct InstrSeq * body) {
  char * lbl = GenLabel();
  struct InstrSeq * instrs = AppendSeq(assignment,GenInstr(lbl,NULL,NULL,NULL,NULL));
  AppendSeq(instrs,cond->instrs);
  AppendSeq(instrs,body);
  AppendSeq(instrs,incrDecr);
  AppendSeq(instrs,GenInstr(NULL,"b",lbl,NULL,NULL));
  AppendSeq(instrs,GenInstr(cond->label,NULL,NULL,NULL,NULL));
  return instrs;
}

struct InstrSeq * ProcFuncCall(char * id){
  struct SymEntry * entry = LookupName(IdentifierTable, id);
  struct Attr * attr = GetAttr(entry);

  struct InstrSeq * instrs = AppendSeq(CalleeProlog(),GenInstr(NULL,"jal",attr->reference,NULL,NULL));
  AppendSeq(instrs,CalleeEpilog());

  return instrs;
}

struct InstrSeq * ProcReturnStmt(struct ExprResult * expr){
  struct InstrSeq * instrs = AppendSeq(expr->instrs,GenInstr(NULL,"move","$v0",TmpRegName(expr->reg),NULL));
  AppendSeq(instrs,GenInstr(NULL,"jr","$ra",NULL,NULL));
}

struct InstrSeq * ProcFuncStmts(struct InstrSeq * stmt, struct InstrSeq * funcStmts ){
  return AppendSeq(stmt,funcStmts);
}
/** Booleans */
/*
struct CondResult * ProcBool(struct CondResult * lhs, enum BoolOp, struct CondResult * rhs){

}
*/

int ProcIndex(char * index) {
  int len = strlen(index);
  char * str = malloc(len);
  index[len-1] = '\0';
  strncpy(str,index+1,len);
  int i = atoi(str);
  free(str);
  free(index);
  return i;
}

struct ExprResult * ProcArrayIndexing(char * id, int index){
  struct SymEntry * entry = LookupName(IdentifierTable, id);
  struct Attr * attr = GetAttr(entry);
  struct ExprResult * expr = malloc(sizeof(struct ExprResult));
  expr->reg = AvailTmpReg();
  expr->type = IntBaseType;
  int reg = AvailTmpReg();
  expr->instrs = GenInstr(NULL,"la",TmpRegName(expr->reg),attr->reference,NULL);
  AppendSeq(expr->instrs,GenInstr(NULL,"li",TmpRegName(reg),Imm(index),NULL));
  AppendSeq(expr->instrs,GenInstr(NULL,"sll",TmpRegName(reg),TmpRegName(reg),"2"));
  AppendSeq(expr->instrs,GenInstr(NULL,"add",TmpRegName(expr->reg),TmpRegName(expr->reg),TmpRegName(reg)));
  AppendSeq(expr->instrs,GenInstr(NULL,"lw",TmpRegName(expr->reg),RegOff(0,TmpRegName(expr->reg)),NULL));
  ReleaseTmpReg(reg);
  return expr;
}

/**
*
* Helper functions
*branch
*/

void FreeExprResult(struct ExprResult * expr) {
  ReleaseTmpReg(expr->reg);
  //FreeSeq(expr->instrs);
  free(expr);
}

enum Ops GetOpFromChar(char * op){
  switch (op[0]) {
    case '+':
      return Add;
    case '-':
      return Sub;
    case '*':
     return Mult;
    case '/':
      return Div;
    default:
      printf("No op found in switch!\n");
      return Add;
  }
}
// I might want to do strdup's on all of these.
char * BranchToString(enum BranchType branch) {
    switch (branch) {
      case Bge:
        return "bge";
      case Bgt:
        return "bgt";
      case Ble:
       return "ble";
      case Blt:
        return "blt";
      case Bne:
        return "bne";
      case Beq:
        return "beq";
      default:
        printf("No Branch found in switch!\n");
        return "bge";
    }
}
