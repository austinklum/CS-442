Letter	[A-Za-z]
Digit   [0-9]

%{
  #include "IOMngr.h"
  #define YY_INPUT(buf,result,max_size) \
  { int c = GetSourceChar(); \
		result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
    }

    bool shouldLog = true;

    #define RETURN(val) \
    { if (shouldLog) fprintf(stderr,"Token#=%d, Length=%zu, Text=\"%s\"\n",val,yyleng,yytext); \
    return(val); \
    }

    int commentLevel = 0;
    int lineComment = 0;
    #define IN_COMMENT (lineComment || commentLevel > 0)

    #include "YSemantics.h"
    #include "y.tab.h"

%}

%%
decl                            { if (!IN_COMMENT) RETURN(DECL_TOK); }
impl                            { if (!IN_COMMENT) RETURN(IMPL_TOK); }
[ \t]                           { }
[\n]                            { lineComment = 0; }
 /** Keywords */
int                 							{ if (!IN_COMMENT) RETURN(INT_TOK); }
chr                 							{ if (!IN_COMMENT) RETURN(CHR_TOK); }
get                               { if (!IN_COMMENT) RETURN(GET_TOK); }
put                               { if (!IN_COMMENT) RETURN(PUT_TOK); }
while                             { if (!IN_COMMENT) RETURN(WHILE_TOK); }
for                               { if (!IN_COMMENT) RETURN(FOR_TOK); }
if                                { if (!IN_COMMENT) RETURN(IF_TOK); }
else                              { if (!IN_COMMENT) RETURN(ELSE_TOK); }
return                            { if (!IN_COMMENT) RETURN(RET_TOK); }
cnst                              { if (!IN_COMMENT) RETURN(CNST_TOK); }
\+\+                              { if (!IN_COMMENT) RETURN(INCR_TOK); }
\-\-                              { if (!IN_COMMENT) RETURN(DECR_TOK); }
\&\&                              { if (!IN_COMMENT) RETURN(AND_TOK); }
\|\|                              { if (!IN_COMMENT) RETURN(OR_TOK); }
\!                                { if (!IN_COMMENT) RETURN(NOT_TOK); }
\[.*\]                            { if (!IN_COMMENT) RETURN(INDEX_TOK); }
 /** Basic Structure */
 \".*\"                             { if (!IN_COMMENT) RETURN(STRLIT_TOK); }
{Letter}({Letter}|{Digit})*       { if (!IN_COMMENT) RETURN(IDENT_TOK);  }
\'\\.\'                      { if (!IN_COMMENT) RETURN(CHRLIT_TOK); }
\'.\'                    { if (!IN_COMMENT) RETURN(CHRLIT_TOK); }
-?{Digit}+     									{ if (!IN_COMMENT) RETURN(INTLIT_TOK); }
\=  										        { if (!IN_COMMENT) RETURN('='); }
\;                              { if (!IN_COMMENT) RETURN(';'); }
\(        											{ if (!IN_COMMENT) RETURN('('); }
\)								  				{ if (!IN_COMMENT) RETURN(')'); }
\-													{ if (!IN_COMMENT) RETURN('-'); }
\+													{ if (!IN_COMMENT) RETURN('+'); }
\*													{ if (!IN_COMMENT) RETURN('*'); }
\/													{ if (!IN_COMMENT) RETURN('/'); }
\{													{ if (!IN_COMMENT) RETURN('{'); }
\}													{ if (!IN_COMMENT) RETURN('}'); }
\,													{ if (!IN_COMMENT) RETURN(','); }
\:                          { if (!IN_COMMENT) RETURN(':'); }
 /** Branches */
\<                          { if (!IN_COMMENT) RETURN(BGE_TOK); }
\>                          { if (!IN_COMMENT) RETURN(BLE_TOK); }
\<\=                        { if (!IN_COMMENT) RETURN(BGT_TOK); }
\>\=                        { if (!IN_COMMENT) RETURN(BLT_TOK); }
\=\=                        { if (!IN_COMMENT) RETURN(BNE_TOK); }
\#\#                        { if (!IN_COMMENT) RETURN(BEQ_TOK); }
 /** Comments */
\/\*											  { commentLevel++; }
\*\/											  { commentLevel == 0 ? commentLevel = 0 : commentLevel--; }
\/\/                        { lineComment = 1;}
.                           { }
%%
