Letter	[A-Za-z]
Digit   [0-9]

%{
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

    #include "YSemantics.h"
    #include "y.tab.h"

%}

%%
decl                            { if (!IN_COMMENT) RETURN(DECL_TOK); }
impl                            { if (!IN_COMMENT) RETURN(IMPL_TOK); }
{Letter}({Letter}|{Digit})*			{ if (!IN_COMMENT) RETURN(IDENT_TOK); }
\{                              { if (!IN_COMMENT) RETURN('{'); }
[ \t]                           { }
[\n]                            { lineComment = 0; }
.                               { }
%%
