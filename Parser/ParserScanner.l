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

int                             { if (!IN_COMMENT) RETURN(INT_TOK); }
\;                              { if (!IN_COMMENT) RETURN(';'); }

[\n]                            { lineComment = 0; }
[ \t]                           { }
.                               { }
%%
