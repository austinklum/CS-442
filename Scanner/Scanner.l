Letter	[A-Za-z]
Digit   [0-9]

%{
#include "ScanTokens.h"
#include "IOMngr.h"
#define YY_INPUT(buf,result,max_size) \
	{ int c = GetSourceChar(); \
		result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
	}

bool shouldLog = false;

#define RETURN(val) \
  { if (shouldLog) fprintf(stderr,"Token#=%d, Length=%zu, Text=\"%s\"\n",val,yyleng,yytext); \
    return(val); \
  }

%}

%%
\@INIT                          { if (!IN_COMMENT) RETURN(INIT_TOK); }
\@DUMP                          { if (!IN_COMMENT) RETURN(DUMP_TOK); }
{Letter}({Letter}|{Digit})*     { if (!IN_COMMENT) RETURN(IDENT_TOK); }
[ \t]                           { }
[\n]                            { }
.                               { }
-?{Digit}+     					{ if (!IN_COMMENT) RETURN(INT_TOK); }
-?{Digit}*\.{Digit}+  			{ if (!IN_COMMENT) RETURN(FLOAT_TOK); }
%%

%{
	int comments, code, whiteSpace;	
%}

%x COMMENT
%%
^[ \t]*"/*" { BEGIN COMMENT; /*enter comment eating state* / }
^[ \t]*"/*".*"*/"[ \t]*\n { comments++; /*self-contained comment*/ }

<COMMENT>"*/"[ \t]*\n 		{ BEGIN 0; comments++;}
<COMMENT>"*/"				{ BEGIN 0; }
<COMMENT>\n 				{ comments++; }
<COMMENT>.\n				{ comments++; }

^[ \t]*\n					{ whiteSpace++; }

.+"/*".*"*/".*\n			{ code++; }
.*"/*".*"*/".+\n			{ code++; }
.+"/*".*\n					{ code++; BEING COMMENT; }
.\n							{ code++; }

.							; /* ignore everything else*/
%%
