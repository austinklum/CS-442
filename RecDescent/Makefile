SHELL=/bin/bash

#==========================
# include lex and yacc libraries
# enable debugging and use gnu11 standard
#==========================
LOADLIBES = -ll -ly
YFLAGS = -d
CFLAGS = -g  -std=gnu11

#==========================
# Make rule syntax
# target: dependencies
# 	rule to build target from dependencies
#
# - no rule necessary if make already knows (i.e. .o from .c)
# - sometimes no dependencies (e.g. clean target)

#==========================
# Symbol Table Project
SymTab.o: SymTab.c SymTab.h
SymTabDriver.o: SymTabDriver.c SymTab.h
SymTabDriver: SymTabDriver.o SymTab.o

symtest: SymTabDriver
		make -s symtest-0
		make -s symtest-1

symtest-0: SymTabDriver
		echo "Test 0"
		./SymTabDriver SymData-0.in > out
		cat out
		if diff out SymData-0.out; then \
			echo -e "\n\nSUCCESS\n\n" ;\
		else \
			echo -e "\n\nFAILED\n\n" ;\
		fi
		rm out

symtest-1: SymTabDriver
		echo "Test 1"
		./SymTabDriver SymData-1.in > out
		cat out
		if diff out SymData-1.out; then \
			echo -e "\n\nSUCCESS\n\n" ;\
		else \
			echo -e "\n\nFAILED\n\n" ;\
		fi
		rm out

#===========================
# IOMngr Project
IOMngr.o: IOMngr.c IOMngr.h
IOMngrDriver.o: IOMngrDriver.c IOMngr.h
IOMngrDriver: IOMngrDriver.o IOMngr.o

iotest:	IOMngrDriver
		make -s iotest-0
		make -s iotest-1
		make -s iotest-2

iotest-0: IOMngrDriver
		echo "Test 0"
		./IOMngrDriver IOMngrSource > out
		cat out
		if diff out IOMngrSource-0.out; then \
			echo -e "\n\nSUCCESS\n\n" ;\
		else \
			echo -e "\n\nFAILED\n\n" ;\
		fi
		rm out

iotest-1: IOMngrDriver
		echo "Test 1"
		./IOMngrDriver -m IOMngrSource > out
		cat out
		if diff out IOMngrSource-1.out; then \
			echo -e "\n\nSUCCESS\n\n" ;\
		else \
			echo -e "\n\nFAILED\n\n" ;\
		fi
		rm out

iotest-2: IOMngrDriver
		echo "Test 2"
		./IOMngrDriver -u IOMngrSource > out
		cat out
		if diff out IOMngrSource-2.out; then \
			echo -e "\n\nSUCCESS\n\n" ;\
		else \
			echo -e "\n\nFAILED\n\n" ;\
		fi
		rm out

#===========================
# Scanner Project
ScannerDriver.o: ScannerDriver.c Scanner.h SymTab.h ScanTokens.h
ScannerDriver: 	ScannerDriver.o Scanner.o SymTab.o IOMngr.o

scantest: 	ScannerDriver
	./ScannerDriver

scanextest: 	ScanExample
	./ScanExample


#===========================
# Recursive Descent Project
RDSupport.o:	RDSupport.c RDSupport.h
RecDescent.o: 	RecDescent.c Scanner.h RDTokens.h
RecDescent: 	RecDescent.o RDGrammar.o RDScanner.o RDSupport.o IOMngr.o

rdtest: 	rdtest1 rdtest2 rdtest3 rdtest4 rdtest5

rdtest1: 	RecDescent
	./RecDescent -l RDSrc-1.src
rdtest2: 	RecDescent
	./RecDescent -l RDSrc-2.src
rdtest3: 	RecDescent
	./RecDescent -l RDSrc-3.src
rdtest4: 	RecDescent
	./RecDescent -l RDSrc-4.src
rdtest5: 	RecDescent
	./RecDescent -l RDSrc-5.src



# Other
clean:
	rm -f *.o SymTabDriver IOMngrDriver ScannerDriver RecDescent Parse Y
