Letter	[A-Za-z]
Digit   [0-9]

%{
#include "y.tab.h"
#include "IOMngr.h"
#define YY_INPUT(buf,result,max_size) \
  { int c = GetSourceChar(); \
    result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
  }

bool shouldLog = false;

#define RETURN(val) \
  { if (shouldLog) fprintf(stderr,"Token#=%d, Length=%zu, Text=\"%s\"\n",val,(unsigned long)yyleng,yytext); \
    return(val); \
  }

int commentLevel = 0;
int lineComment = 0;
#define IN_COMMENT (lineComment || commentLevel > 0)

%}

%%

  /* Combine comment nesting from Scanner.l with RDScanner.l regular
     expressions except for real literal. Use RETURN(token-symbolic-name)
     where token-symbolic-name appears in yacc file.
   */

\;                              { if (!IN_COMMENT) RETURN(';'); }
[\n]                            { lineComment = 0; }
[ \t]                           { }
int                 							{ if (!IN_COMMENT) RETURN(INT_TOK); }
chr                 							{ if (!IN_COMMENT) RETURN(CHR_TOK); }
":="											{ if (!IN_COMMENT) RETURN(ASSIGN_TOK); }
"("        											{ if (!IN_COMMENT) RETURN('('); }
")"								  					{ if (!IN_COMMENT) RETURN(')'); }
"-"													{ if (!IN_COMMENT) RETURN('-'); }
"+"													{ if (!IN_COMMENT) RETURN('+'); }
"*"													{ if (!IN_COMMENT) RETURN('*'); }
"/"													{ if (!IN_COMMENT) RETURN('/'); }
"{"													{ if (!IN_COMMENT) RETURN('{'); }
"}"													{ if (!IN_COMMENT) RETURN('}'); }
","													{ if (!IN_COMMENT) RETURN(','); }
-?{Digit}+     									{ if (!IN_COMMENT) RETURN(INTLIT_TOK); }
{Letter}({Letter}|{Digit})*     				{ if (!IN_COMMENT) RETURN(IDENT_TOK); }
\/\*														{ commentLevel++; }
\*\/														{ commentLevel == 0 ? commentLevel = 0 : commentLevel--; }
\/\/														{ lineComment = 1;}
.                               { }
%%
