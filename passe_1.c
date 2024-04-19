
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "miniccutils.h"
#include "passe_1.h"
#include "common.h"

extern int trace_level;
bool global_env;
bool under_decl;
bool func_prototype;
node_type last_type = TYPE_VOID;

expverif exp_tab[22];

void verif_expr(node_t expr) {
    if(expr->nature == NODE_AFFECT) {
        if((expr->nops) != 2) {
            printf("Error line %d: Expected 2 operands around '=' sign\n", expr->lineno);   
            exit(0);
        }
        else if(expr->opr[0]->type != expr->opr[1]->type) {
            printf("Error line %d: Different types around '=' sign : left is %s, right is %s\n", expr->lineno,node_type2string(expr->opr[0]->type),node_type2string(expr->opr[1]->type));   
            exit(0); //Atention dans le poly c'est 2 regles différentes (p.25) regles 1.12 et 1.13 
        }  
        expr->type = expr->opr[0]->type; 
        return;
    }
    for(int i = 0; i < 22; i++) {
        if((exp_tab[i]).nexpr == expr->nature) {
            expr->type = exp_tab[i].outtype;
            printf_level(4,"i = %d, expr = %s\n",i,node_nature2string(expr->nature));
            if(expr->nops != exp_tab[i].nops) {
                printf("Error line %d: expected %d operands around \"%s\", got %d\n", expr->lineno, expr->nops, node_nature2symb(exp_tab[i].nexpr), exp_tab[i].nops);
                exit(1);
            }
            if(expr->opr[0]->type != exp_tab[i].ltype) {
                if(exp_tab[i].nops == 1) {
                    printf("Error line %d: expected %s as operand of \"%s\", got %s\n", expr->lineno, node_type2string(exp_tab[i].ltype), node_nature2symb(exp_tab[i].nexpr), node_type2string(expr->opr[0]->type));
                }
                else {
                    printf("Error line %d: expected %s as left operand of \"%s\", got %s\n", expr->lineno, node_type2string(exp_tab[i].ltype), node_nature2symb(exp_tab[i].nexpr), node_type2string(expr->opr[0]->type));
                }
                exit(1);
            }
            if(exp_tab[i].nops > 1 && expr->opr[1]->type != exp_tab[i].rtype) {
                // if(expr->opr[1]->type == TYPE_NONE) printf("Right = NONE\n");
                printf("Error line %d: expected %s as right operand of \"%s\", got %s\n", expr->lineno, node_type2string(exp_tab[i].rtype), node_nature2symb(exp_tab[i].nexpr), node_type2string(expr->opr[1]->type));
                exit(1);
            }
        }
    }
}

void analyse_passe_1(node_t root) {

    if(root == NULL) return;

    // Opérations en OUVERTURE / AVANT considération des enfants
    switch(root->nature) {
        case NODE_PROGRAM:
            global_env = true;
            under_decl = false;
            func_prototype = false;
            push_global_context();
            break;

        case NODE_BLOCK:
            global_env = false;
            func_prototype = false;
            push_context();
            break;
            
        case NODE_DECL:
            under_decl = true;
            break;

        case NODE_IDENT:
            if(get_decl_node(root->ident)) { // réutilisation
                if(under_decl) { // redéclaration : erreur !
                    printf("Error line %d: identifier %s alreadly declared\n", root->lineno, root->ident);
                    exit(0);
                }
                else if(global_env) { // il ne peut que y avoir des initialisations par littéral en environnement global
                    printf("Error line %d: global declaration using non-constant value is not allowed\n", root->lineno);
                    exit(0);
                }
                root->decl_node = get_decl_node(root->ident);
                root->type = root->decl_node->type;
            }
            else {
                if(under_decl) { // déclaration
                    int eadr = env_add_element(root->ident,root);
                    printf_level(4,"Decl de %s, eadr %d, ligne %d\n",root->ident,eadr,root->lineno);
                    root->offset = eadr;
                    root->global_decl = global_env;
                    if(last_type == TYPE_VOID) {
                        printf("Error line %d: cannot declare void-type variable\n", root->lineno);
                        exit(0);
                    }
                    root->type = last_type;
                    under_decl = false; // une seule déclaration par decl (au singulier)
                }
                else {
                    if(func_prototype) {
                        if(strcmp(root->ident,"main") != 0) {
                        printf("Error line %d: declared function with label other than 'main'\n", root->lineno);
                        exit(0);
                        }
                    }
                    else {
                        printf("Error line %d: unknown identifier '%s'\n",root->lineno,root->ident);
                        exit(0);
                    }
                }
            }
            printf_level(4,"%s type is %s\n",root->ident, node_type2string(root->type));
            break;

        case NODE_STRINGVAL:
            root->offset = add_string(root->str);
            break;

        case NODE_FUNC:
            global_env = false;
            func_prototype = true;
            reset_env_current_offset();
            root->type = root->opr[0]->type;
            break;

        case NODE_TYPE:
            if(func_prototype && root->type != TYPE_VOID) {
                printf("Error line %d: declared function is of non-void return type\n", root->lineno);
                exit(0);
            }
            last_type = root->type;
            break;

        case NODE_INTVAL:
            root->type = TYPE_INT;
            break;

        case NODE_BOOLVAL:
            root->type = TYPE_BOOL;
            break;

        default:
            break;
    }

    // Considération des enfants
    
    for(int n = 0; n < (root->nops); n++) {
        if(root->opr[n] != NULL) analyse_passe_1(root->opr[n]);
    }

    // Opérations en FERMETURE / APRES considération des enfants

    switch(root->nature) {
        case NODE_BLOCK:
            pop_context();
            break;

        case NODE_PROGRAM:
            pop_context();
            break;
        case NODE_FUNC:
            root->offset = get_env_current_offset();
            break;

        case NODE_DECL:
            under_decl = false;
            if(root->opr[1] != NULL) {
                if(root->opr[0]->type != root->opr[1]->type) {
                    printf("Error line %d: mismatched types in declaration - got %s and %s\n", root->lineno, node_type2string(root->opr[0]->type), node_type2string(root->opr[1]->type));
                    exit(0);
                }
            }
            break;

        case NODE_PLUS:
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
        case NODE_AFFECT:
            verif_expr(root); // le typage des identifiants ou des expressions enfants doît être fait au préalable
            break;

        //ICI On a pas testé si les conditions des if,while... sont bien booléennes, donc voici ma modeste correction : 
        case NODE_WHILE:
            if(root->opr[0]->type != TYPE_BOOL) {
                printf("Error line %d: condition is not boolean in the WHILE\n", root->lineno);
                exit(0);
            }
            break;
        case NODE_IF:
            if(root->opr[0]->type != TYPE_BOOL) {
                printf("Error line %d: condition is not boolean in the IF\n", root->lineno);
                exit(0);
            }
            break;
        case NODE_DOWHILE:
            if(root->opr[1]->type != TYPE_BOOL) {
                printf("Error line %d: condition is not boolean in the DO_WHILE\n", root->lineno);
                exit(0);
            }
            break;
        case NODE_FOR:
            if(root->opr[1]->type != TYPE_BOOL) {
                printf("Error line %d: condition is not boolean in the FOR\n", root->lineno);
                exit(0);
            }
            break;
        default:
            break;
    }
}

void init_exprverif() {

    exp_tab[0] = (expverif){.nexpr = NODE_PLUS, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[1] = (expverif){.nexpr = NODE_MINUS, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[2] = (expverif){.nexpr = NODE_MUL, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[3] = (expverif){.nexpr = NODE_DIV, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[4] = (expverif){.nexpr = NODE_MOD, .nops = 2, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
    exp_tab[5] = (expverif){.nexpr = NODE_UMINUS, .nops = 1, .ltype = TYPE_INT, .rtype = TYPE_INT, .outtype = TYPE_INT};
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
    exp_tab[20] = (expverif){.nexpr = NODE_BNOT, .nops = 1, .ltype = TYPE_INT, .rtype = TYPE_NONE, .outtype = TYPE_INT};
    exp_tab[21] = (expverif){.nexpr = NODE_NOT, .nops = 1, .ltype = TYPE_BOOL, .rtype = TYPE_NONE, .outtype = TYPE_BOOL};
}