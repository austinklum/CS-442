/* Semantics.c
 Support and semantic action routines.

 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "YSemantics.h"

// Shared Data

struct SymTab * IdentifierTable;
enum AttrKinds { VOID_KIND = -1, INT_KIND, STRING_KIND, STRUCT_KIND };

char * BaseTypeNames[2] = { "int", "chr"};
char * TypeNames[2] = { "", "func"};

// corresponds to enum Operators
char * Ops[] = { "add", "sub", "mul", "div"};
//char * BoolOps[] = { "and", "or", "not" };

// corresponds to negation of enum Comparisons
// enum Comparisons { LtCmp, LteCmp, GtCmp, GteCmp, EqCmp, NeqCmp };
char * Branches[] = { "bge", "bg", "ble", "bl", "bne", "beq"};
// comparison set instructions, in order with enum Comparisons
//char * CmpSetReg[] = { "slt", "sle", "sgt", "sge", "seq", "sne" };


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
}

char *
StringForType(struct TypeDesc * desc) {
  switch (desc->declType) {
    case PrimType: {
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
    case PrimType: {
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

    } break;
    case FuncType: {
      // nothing to do here
    } break;
  }
}

void processFunctions(struct SymEntry * entry, int cnt, void * textCode) {
  struct Attr * attr = GetAttr(entry);
  switch (attr->typeDesc->declType) {
    case PrimType: {
      // nothing to do here
    } break;
    case FuncType: {
      if (!attr->typeDesc->funcDesc->funcCode) {
        PostMessageAndExit(GetCurrentColumn(),"function never implemented");
      }

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
  AppendSeq(textCode,GenInstr(NULL,"jal",mainAttr->reference,NULL,NULL));
  AppendSeq(textCode,GenInstr(NULL,"li","$v0","10",NULL));
  AppendSeq(textCode,GenInstr(NULL,"syscall",NULL,NULL,NULL));

  // run SymTab with InvokeOnEntries putting globals in data seg
  // run SymTab with InvokeOnEntries putting functions in code seq

  // combine and write
  struct InstrSeq * moduleCode = AppendSeq(textCode,dataCode);
  WriteSeq(moduleCode);

  // free code
  FreeSeq(moduleCode);
  CloseCodeGen();
  fprintf(stderr,"Finish \n");
}

void
ProcDecls(struct IdList * idList, enum BaseTypes baseType) {

/*	struct IdList item = malloc(sizeof(struct IdList));
	struct IdList needToFree = item;
	item->next = idList;*/
	struct IdList item = idList;
  // walk IdList items
	while((item = item->next)) {
		struct Attr itemAttr = ((struct Attr)GetAttr(item->entry));
	 // switch for prim or func type
		switch(itemAttr->typeDesc->declType) {
		  // names on IdList are only specified as PrimType or FuncType
			case PrimType:
				itemAttr->typeDesc->primDesc = baseType;
				itemAttr->reference = strdup(strcat("_", GetName(item->entry)));
				break;
			case FuncType:
			// set type desc
			// attr reference string is id name with prepended "_"
				itemAttr->typeDesc->funcDesc = baseType;
				///malloc of proper size
				///Append _ strcat entryname
				itemAttr->reference = strdup(strcat("_", GetName(item->entry)));
				break;
		}
		// for Sem1 everthing is in global scope, otherwise check scope
	}
	/*free(needToFree);*/
}

struct IdList *
AppendIdList(struct IdList * item, struct IdList * list) {
	item->next = list->next;
	list->next = item;
	return item;
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
	attr->typeDesc = type;
	SetAttr(entry,-1,attr);

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
	struct InstrSeq * funcLabel = GenInstr(GenLabel(), "","","");
  // include function body code
	instrs = AppendSeq(funcLabel,instrs);
  // function exit code, i.e. jump return
	attr->typeDesc->funcDesc->funcCode = instrs;
}
