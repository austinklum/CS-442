#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "SymTab.h"
/*  Author:   Austin Klum
    Created:  02/01.18
    Resources: https://www.tutorialspoint/c_standard_library/c_function_strcmp.htm
               https://www.tutorialspoint.com/c_standard_library/c_function_strcat.htm
               https://www.systutorials.com/docs/linux/man/3-strdup/
*/

char * GetScopePathHelper(struct SymTab *aTable, char * path);
/* The symbol table entry structure proper.
*/
struct SymEntry {
  struct SymTab * table;
  char *name;
  int attrKind;
  void *attributes;
  struct SymEntry *next;
};

/* The symbol table structure proper. The hash table array Contents
   is dynamically allocated according to size
*/
struct SymTab {
  struct SymTab * parent;
  char *scopeName;
  int size;
  struct SymEntry **contents;
};

struct SymTab *
CreateSymTab(int size, char * scopeName, struct SymTab * parentTable) {
	struct SymTab *tab = malloc(sizeof(struct SymTab));
	tab->contents = malloc(size * sizeof(struct SymEntry *));
	int i;

	for (i = 0; i < size; i++){
		tab->contents[i] = NULL;
	}

	tab->size = size;
	tab->scopeName = scopeName ? strdup(scopeName) : strdup("..\0");
	tab->parent = parentTable;

	return tab;
}

struct SymTab *
DestroySymTab(struct SymTab *aTable) {
	int i;
  if(!aTable){
      return NULL;
  }
	//Delete all the entries in the content hash.
	for (i = 0; i < aTable->size; i++) {
			struct SymEntry *tmp = aTable->contents[i];

			// Traverse linked list and free up memory.
			while (tmp) {
				struct SymEntry *next = tmp->next;
				free(tmp->name);
				free(tmp);
				tmp = next;
			}
	}
    free(aTable->contents);

		struct SymTab * parentTable = aTable->parent;
		// Free up any memory on the actual table itself.

    free(aTable->scopeName);
		free(aTable);

		return parentTable;
	}


int HashName(int size, const char *name) {
    int hash = 0;
    // Loop through and add up all the ascii values
    while(*name)
    	hash += *name++;

    //Integer overflow
    if (hash < 0)
        hash = hash++ * -1;

    return hash % size;
}


struct SymEntry * SearchATable(struct SymTab *aTable, const char * name){
	// Start at the correct index in the table
	struct SymEntry *tmp = aTable->contents[HashName(aTable->size,name)];

	// Loop through the linked list until we're at the end OR we found the name
	while(tmp && strcmp(tmp->name,name)) {
		tmp = tmp->next;
	}

	return tmp;
}

struct SymEntry *
LookupName(struct SymTab *aTable, const char * name) {
	// Loop through all parent tables
	struct SymTab *curTable = aTable;
	struct SymEntry *tmp = NULL;


	//While not some entry and there is a table, Search that table.
	while(!tmp && curTable){
	// Search all tables
		tmp = SearchATable(curTable,name);
		curTable = curTable->parent;
	}

	return tmp;
}

struct SymEntry *
EnterName(struct SymTab *aTable, const char *name) {

	struct SymEntry *tmp = SearchATable(aTable, name);
	// Check if it's already in the table.
	if (tmp) {
		return tmp;
	}

	// Create a new struct entry
	struct SymEntry *entry = malloc(sizeof(struct SymEntry));
	entry->table = aTable;
	entry->name = strdup(name);
	entry->attrKind = -1;
	entry->attributes = NULL;

	// Add it to the head of the list in the proper place in hashing table
	int hash = HashName(aTable->size,name);
	entry->next = aTable->contents[hash];
	aTable->contents[hash] = entry;

	return entry;
}

void
SetAttr(struct SymEntry *anEntry, int kind, void *attributes) {
  if(anEntry) {
  	anEntry->attrKind = kind;
  	anEntry->attributes = attributes;
  }
}

int
GetAttrKind(struct SymEntry *anEntry) {
	return anEntry ? anEntry->attrKind : -1;
}

void *
GetAttr(struct SymEntry *anEntry) {
	return anEntry ? anEntry->attributes : NULL;
}

const char *
GetName(struct SymEntry *anEntry) {
	return anEntry ? anEntry->name : NULL;
}

struct SymTab *
GetTable(struct SymEntry *anEntry) {
	return anEntry ? anEntry->table : NULL;
}

const char *
GetScopeName(struct SymTab *aTable) {
	return aTable ? aTable->scopeName : NULL;
}

char *
GetScopePath(struct SymTab *aTable) {
  char *path = strdup("");
  return GetScopePathHelper(aTable, path);
}

char * GetScopePathHelper(struct SymTab *aTable, char * path){
  //Recurse all the way down linked list
  if (!aTable->parent) {
      path = realloc(path,strlen(aTable->scopeName) + 1);
      strcat(path,aTable->scopeName);
      return path;
  }
  GetScopePathHelper(aTable->parent,path);

  //On the way back up make memory and add the table names
  path = realloc(path,strlen(path) + strlen(aTable->scopeName) + 2);
  strcat(path,">");
  strcat(path,aTable->scopeName);
  return path;
}

struct SymTab *
GetParentTable(struct SymTab *aTable) {
	return aTable->parent;
}

void
InvokeOnEntries(struct SymTab *aTable, bool includeParentTable, entryWorkFunc workFunc, int startCnt, void * withArgs) {
	struct SymTab * curTable = aTable;
	int i;
  int counter = startCnt;
  bool loopOnParent = true;
	//Loop for all the tables, shuold run at least once
	while(curTable && loopOnParent){
		//Loop for all the content
		for(i = 0; i < curTable->size; i++){
			struct SymEntry * tmp = curTable->contents[i];
			//Loop for all the linked lists
					while (tmp) {
						workFunc(tmp,counter,withArgs);
            counter++;
						tmp = tmp->next;
					  }
		}
		curTable = curTable->parent;
    loopOnParent = includeParentTable;
	}
}

struct Stats *
Statistics(struct SymTab *aTable) {
	struct Stats * stat = malloc(sizeof(struct Stats));
	stat->minLen = INT_MAX;
	stat->maxLen = 0;
	stat->entryCnt = 0;
	int linksSeen = 0;
	int i;
	for(i = 0; i < aTable->size; i++){
		struct SymEntry * tmp = aTable->contents[i];
		//Loop for all the linked lists
				while (tmp) {
					linksSeen++;
					tmp = tmp->next;
				}

				stat->entryCnt += linksSeen;

				if (linksSeen > stat->maxLen) {
					stat->maxLen = linksSeen;
				}

				if (linksSeen < stat->minLen) {
					stat->minLen = linksSeen;
				}

				//Reset Seen links
				linksSeen = 0;
	}

	stat->avgLen = stat->entryCnt / aTable->size;

	return stat;
}
