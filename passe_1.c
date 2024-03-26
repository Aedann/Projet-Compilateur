
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "miniccutils.h"
#include "passe_1.h"

extern int trace_level;
bool global_env;
node_type last_type = TYPE_VOID;

expverif exp_tab[21];

void analyse_passe_1(node_t root) {

    if(root == NULL) return;
    switch(root->nature) {
        case NODE_PROGRAM:
            global_env = true;
            push_global_context();
            break;

        case NODE_BLOCK:
            global_env = false;
            push_context();
            break;

        case NODE_IDENT:
            if(get_decl_node(root->ident)) { // réutilisation
                root->decl_node = get_decl_node(root->ident);
                root->type = (root->decl_node)->type;
            }
            else { // déclaration
                int eadr;
                root->offset = eadr;
                root->global_decl = global_env;
            }
            break;

        case NODE_STRINGVAL:
            root->offset = add_string(root->str);
            break;

        case NODE_FUNC:
            global_env = false;
            reset_env_current_offset();
            break;

        case NODE_TYPE:
            last_type = root->type;
            break;

        case NODE_PLUS: // Integer exprs
        case NODE_MINUS:
        case NODE_MUL:
        case NODE_DIV:
        case NODE_MOD:
        case NODE_UMINUS:
        case NODE_BAND:
        case NODE_BOR:
        case NODE_BXOR:
        case NODE_SLL:
        case NODE_SRL:
        case NODE_SRA:
        case NODE_BNOT:
        case NODE_LT :
        case NODE_GT :
        case NODE_LE :
        case NODE_GE :
        case NODE_EQ :
        case NODE_NE :
        case NODE_AND:
        case NODE_OR :
        case NODE_NOT:
            
            break;

        default:
            break;
    }
    
    for(int n = 0; n < (root->nops); n++) {
        analyse_passe_1(root->opr[n]);
    }

    switch(root->nature) {

        case NODE_BLOCK:
            pop_context();
            break;

        case NODE_PROGRAM:
        case NODE_FUNC:
            root->offset = get_env_current_offset();
            break;

        default:
            break;
    }
}

void verif_expr(node_t expr) {
    for(int i = 0; i < 21; i++) {
        if((exp_tab[i]).nexpr == expr->nature) {
            expr->type = exp_tab[i].outtype;
            if(expr->nops == exp_tab[i].nops) {
                printf("Error on number of operands on line %d\n", expr->lineno);
                exit(1);
            }
            if(expr->opr[0]->nature != exp_tab[i].ltype) {
                printf("Error on left operand's type on line %d\n", expr->lineno);
                exit(1);
            }
            if(expr->opr[1]->nature != exp_tab[i].rtype) {
                printf("Error on right operand's type on line %d\n", expr->lineno);
                exit(1);
            }
        }
    }
}

void init_exprverif(){

    exp_tab[0] = (expverif){.nexpr = NODE_PLUS, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[1] = (expverif){.nexpr = NODE_MINUS, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[2] = (expverif){.nexpr = NODE_MUL, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[3] = (expverif){.nexpr = NODE_DIV, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[4] = (expverif){.nexpr = NODE_MOD, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[5] = (expverif){.nexpr = NODE_UMINUS, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[6] = (expverif){.nexpr = NODE_LT, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_BOOL};
    exp_tab[7] = (expverif){.nexpr = NODE_GT, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_BOOL};
    exp_tab[8] = (expverif){.nexpr = NODE_LE, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_BOOL};
    exp_tab[9] = (expverif){.nexpr = NODE_GE, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_BOOL};
    exp_tab[10] = (expverif){.nexpr = NODE_EQ, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_BOOL};
    exp_tab[11] = (expverif){.nexpr = NODE_NE, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_BOOL};
    exp_tab[12] = (expverif){.nexpr = NODE_AND, .nops = 2, .ltype = TYPE_BOOL, .rtype = TYPE_BOOL, .outtype = TYPE_BOOL};
    exp_tab[13] = (expverif){.nexpr = NODE_OR, .nops = 2, .ltype = TYPE_BOOL, .rtype = TYPE_BOOL, .outtype = TYPE_BOOL};
    exp_tab[14] = (expverif){.nexpr = NODE_BAND, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[15] = (expverif){.nexpr = NODE_BOR, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[16] = (expverif){.nexpr = NODE_BXOR, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[17] = (expverif){.nexpr = NODE_SLL, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[18] = (expverif){.nexpr = NODE_SRL, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[19] = (expverif){.nexpr = NODE_SRA, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[20] = (expverif){.nexpr = NODE_NOT, .nops = 1, .ltype = TYPE_BOOL, .rtype = TYPE_NONE, .outtype = TYPE_BOOL};
    exp_tab[21] = (expverif){.nexpr = NODE_BNOT, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
}