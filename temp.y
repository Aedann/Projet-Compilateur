%%
program : 
    listdeclnonnull maindecl
    {
        $$ = NULL;
    }
    | maindecl
    {
        $$ = NULL;
    }
    ;
listdecl : 
    listdeclnonnull
    {
        $$ = NULL;
    }
    |
    {
        $$ = NULL;
    }
    ;
listdeclnonnull : 
    vardecl
    {
        $$ = NULL;
    }
    | listdeclnonnull vardecl
    {
        $$ = NULL;
    }
    ;
vardecl : 
    type listtypedecl TOK_SEMICOL
    {
        $$ = NULL;
    }
    ;
type : 
    TOK_INT
    {
        $$ = NULL;
    }
    | TOK_BOOL
    {
        $$ = NULL;
    }
    | TOK_VOID
    {
        $$ = NULL;
    }
    ;
listtypedecl : 
    decl
    {
        $$ = NULL;
    }
    | listtypedecl TOK_COMMA decl
    {
        $$ = NULL;
    }
    ;
decl : 
    ident
    {
        $$ = NULL;
    }
    | ident TOK_AFFECT expr
    {
        $$ = NULL;
    }
    ;
maindecl : 
    type ident TOK_LPAR TOK_RPAR block
    {
        $$ = NULL;
    }
    ;
listinst : 
    listinstnonnull
    {
        $$ = NULL;
    }
    |
    {
        $$ = NULL;
    }
    ;
listinstnonnull : 
    inst
    {
        $$ = NULL;
    }
    | listinstnonnull inst
    {
        $$ = NULL;
    }
    ;
inst : 
    expr TOK_SEMICOL
    {
        $$ = NULL;
    }
    | TOK_IF TOK_LPAR expr TOK_RPAR inst TOK_ELSE inst
    {
        $$ = NULL;
    }
    | TOK_IF TOK_LPAR expr TOK_RPAR inst %prec TOK_THEN
    {
        $$ = NULL;
    }
    | TOK_WHILE TOK_LPAR expr TOK_RPAR inst
    {
        $$ = NULL;
    }
    | TOK_FOR TOK_LPAR expr TOK_SEMICOL expr TOK_SEMICOL expr TOK_RPAR inst
    {
        $$ = NULL;
    }
    | TOK_DO inst TOK_WHILE TOK_LPAR expr TOK_RPAR TOK_SEMICOL
    {
        $$ = NULL;
    }
    | block
    {
        $$ = NULL;
    }
    | TOK_SEMICOL
    {
        $$ = NULL;
    }
    | TOK_PRINT TOK_LPAR listparamprint TOK_RPAR TOK_SEMICOL
    {
        $$ = NULL;
    }
    ;
block : 
    TOK_LACC listdecl listinst TOK_RACC
    {
        $$ = NULL;
    }
    ;
expr : 
    expr TOK_MUL expr
    {
        $$ = NULL;
    }
    | expr TOK_DIV expr
    {
        $$ = NULL;
    }
    | expr TOK_PLUS expr
    {
        $$ = NULL;
    }
    | expr TOK_MINUS expr
    {
        $$ = NULL;
    }
    | expr TOK_MOD expr
    {
        $$ = NULL;
    }
    | expr TOK_LT expr
    {
        $$ = NULL;
    }
    | expr TOK_GT expr
    {
        $$ = NULL;
    }
    | TOK_MINUS expr %prec TOK_UMINUS
    {
        $$ = NULL;
    }
    | expr TOK_GE expr
    {
        $$ = NULL;
    }
    | expr TOK_LE expr
    {
        $$ = NULL;
    }
    | expr TOK_EQ expr
    {
        $$ = NULL;
    }
    | expr TOK_NE expr
    {
        $$ = NULL;
    }
    | expr TOK_AND expr
    {
        $$ = NULL;
    }
    | expr TOK_OR expr
    {
        $$ = NULL;
    }
    | expr TOK_BAND expr
    {
        $$ = NULL;
    }

    | expr TOK_BOR expr
    {
        $$ = NULL;
    }
    | expr TOK_BXOR expr
    {
        $$ = NULL;
    }
    | expr TOK_SRL expr
    {
        $$ = NULL;
    }
    | expr TOK_SRA expr
    {
        $$ = NULL;
    }
    | expr TOK_SLL expr
    {
        $$ = NULL;
    }
    | TOK_NOT expr
    {
        $$ = NULL;
    }
    | TOK_BNOT expr
    {
        $$ = NULL;
    }
    | TOK_LPAR expr TOK_RPAR
    {
        $$ = NULL;
    }
    | ident TOK_AFFECT expr
    {
        $$ = NULL;
    }
    | TOK_INTVAL
    {
        $$ = NULL;
    }
    | TOK_TRUE
    {
        $$ = NULL;
    }
    | TOK_FALSE
    {
        $$ = NULL;
    }
    | ident
    {
        $$ = NULL;
    }
    ;
listparamprint : 
    listparamprint TOK_COMMA paramprint
    {
        $$ = NULL;
    }
    | paramprint
    {
        $$ = NULL;
    }
    ;
paramprint : 
    ident
    {
        $$ = NULL;
    }
    | TOK_STRING
    {
        $$ = NULL;
    }
    ;
ident : 
    TOK_IDENT
    {
        $$ = NULL;
    }
    ;


%%