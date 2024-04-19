
#include <stdio.h>

#include "utils/miniccutils.h"
#include "defs.h"
#include "passe_2.h"
#include "common.h"

int expr_depth;
bool in_print;
int temporary_count;

int32_t allocate() {
    if(reg_available()) {
        allocate_reg();
    }
    else {
        push_temporary(get_current_reg());
        temporary_count++;
    }
    return get_current_reg();
}

int32_t deallocate() {
    if(temporary_count > 0) {
        pop_temporary(get_restore_reg());
        return get_restore_reg();
    }
    else {
        release_reg();
        return get_current_reg();
    }
}
 
int32_t gen_code_expr(node_t root) {
        expr_depth++;

        switch(root->nature) {

            case NODE_PLUS:
                addu_inst_create(get_current_reg()-1, gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_MINUS:
                subu_inst_create(get_current_reg()-1, gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_MUL:
                mult_inst_create(get_current_reg(), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_DIV:
                div_inst_create(get_current_reg(), gen_code_expr(root->opr[1]));
                release_reg();
                mflo_inst_create(get_current_reg());
                break;

            case NODE_MOD:
                div_inst_create(get_current_reg()-1, gen_code_expr(root->opr[1]));
                release_reg();
                mfhi_inst_create(get_current_reg());
                break;

            case NODE_UMINUS:
                subu_inst_create(get_current_reg(), 0, gen_code_expr(root->opr[0]));
                release_reg();
                break;

            case NODE_BAND:
                and_inst_create(get_current_reg()-1, gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_BOR:
                or_inst_create(get_current_reg()-1, gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_BXOR:
                xor_inst_create(get_current_reg()-1, gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_SLL:
                sllv_inst_create(get_current_reg()-1, gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_SRL:
                srlv_inst_create(get_current_reg()-1, gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_SRA:
                srav_inst_create(get_current_reg()-1, gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_BNOT:
                nor_inst_create(get_current_reg()-1, 0, gen_code_expr(root->opr[0]));
                break;

            case NODE_LT :
                slt_inst_create(get_current_reg()-1, gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_GT :
                slt_inst_create(get_current_reg()-1, gen_code_expr(root->opr[1]), gen_code_expr(root->opr[0]));
                release_reg();
                break;
            
            case NODE_LE :
                addiu_inst_create(get_current_reg(), gen_code_expr(root->opr[1]), 1);
                slt_inst_create(get_current_reg(), get_current_reg(), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_GE :
                addiu_inst_create(get_current_reg(), gen_code_expr(root->opr[0]), -1);
                slt_inst_create(get_current_reg(), get_current_reg(), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_EQ :
                xor_inst_create(get_current_reg(), gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                slti_inst_create(get_current_reg(),get_current_reg(),1);
                release_reg();
                break;

            case NODE_NE :
                xor_inst_create(get_current_reg()-1, gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                release_reg();
                break;
                
            case NODE_AND:
                and_inst_create(get_current_reg()-1, gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                release_reg();
                break;
                
            case NODE_OR :
                or_inst_create(get_current_reg()-1, gen_code_expr(root->opr[0]), gen_code_expr(root->opr[1]));
                release_reg();
                break;

            case NODE_NOT:
                nor_inst_create(get_current_reg(), 0, gen_code_expr(root->opr[0]));
                break;

            case NODE_AFFECT:
                if(root->opr[0]->decl_node->global_decl) { // sauvegarde dans l'espace global
                    allocate_reg();
                    lui_inst_create(get_current_reg(), 0x1001);
                    sw_inst_create(gen_code_expr(root->opr[1]), root->opr[0]->decl_node->offset, get_current_reg());
                    release_reg();
                }
                else {
                    sw_inst_create(gen_code_expr(root->opr[1]), root->opr[0]->decl_node->offset, 29);
                }
                release_reg();
                break;

            case NODE_IDENT:
                allocate_reg();
                if(root->decl_node->global_decl) { // chargement depuis l'espace global
                    lui_inst_create(get_current_reg(), 0x1001);
                    lw_inst_create(get_current_reg(), root->decl_node->global_decl, get_current_reg());
                }
                else {
                    lw_inst_create(get_current_reg(), root->decl_node->offset, 29);
                }
                break;

            case NODE_INTVAL:
            case NODE_BOOLVAL:
                allocate_reg();
                ori_inst_create(get_current_reg(),0,root->value);
                break;
        }
        for(int i = 1; i < expr_depth; i++) printf("\t");
        expr_depth--;
        if(root->nature == NODE_IDENT) {
            printf("%s line %d, reg = %d\n", root->ident, root->lineno, get_current_reg());
        }
        else {
            printf("%s line %d, reg = %d\n", node_nature2string(root->nature), root->lineno, get_current_reg());
        }
       
        return get_current_reg();
}

void gen_code_passe_2(node_t root) {

    switch(root->nature) {

        case NODE_PROGRAM:
            expr_depth = 0;
            in_print = false;
            data_sec_inst_create();
            break;

        case NODE_FUNC:
            for(int gsindex = 0; gsindex < get_global_strings_number(); gsindex++) {
                asciiz_inst_create(NULL,get_global_string(gsindex)); // pousser la ligne ".asciz <contenu de la chaîne>"
            }
            text_sec_inst_create(); // si on entre dans une fonction, c'est que c'est le main : on crée le segment .text et l'étiquette main
            label_str_inst_create("main");
            reset_temporary_max_offset();
            set_temporary_start_offset(root->offset);
            stack_allocation_inst_create();
            break;

        case NODE_DECL:
            if(root->opr[0]->global_decl) { // déclaration globale (en .data)
                if(root->opr[1] == NULL) {
                    word_inst_create(root->opr[0]->ident, 0); // déclaration d'une var globale sans init. : .word avec 0
                }
                else {
                    word_inst_create(root->opr[0]->ident, root->opr[1]->value); // déclaration d'une var globale avec init. : .word avec val init.
                }
            }
            else {
                if(root->opr[1] == NULL) { // déclaration locale sans init. : or avec 0
                    // on ne fait rien
                }
                else {
                    if(root->opr[1]->nature == NODE_IDENT) { // déclaration locale avec init. depuis variable
                        if(root->opr[1]->decl_node->global_decl) { // globale
                            lui_inst_create(get_current_reg(), 0x1001);
                            lw_inst_create(get_current_reg(), root->opr[1]->decl_node->offset, get_current_reg());
                        }
                        else { // locale
                            lw_inst_create(get_current_reg(), root->opr[1]->decl_node->offset, 29);
                        }
                    }
                    else { // déclaration locale avec init. par littéral (simple ori écrit en dur)
                        ori_inst_create(get_current_reg(), 0, root->opr[1]->value);
                    }

                sw_inst_create(get_current_reg(), root->opr[0]->offset, 29);
                }


            }
            break;


        case NODE_IF:;
            int32_t else_label = get_new_label();
            if(root->nops == 3) {  // cas if-then-else
                int32_t after_if_label = get_new_label();
                gen_code_expr(root->opr[0]); // test
                beq_inst_create(get_current_reg(), 0, else_label);
                gen_code_passe_2(root->opr[1]); // bloc d'expression "then";
                j_inst_create(after_if_label); // on sort du if
                label_inst_create(else_label);
                gen_code_passe_2(root->opr[1]);
                label_inst_create(after_if_label);
            }
            else { // cas if-then
                gen_code_expr(root->opr[0]); // test
                beq_inst_create(get_current_reg(), 0, else_label);
                gen_code_passe_2(root->opr[1]); // bloc d'expressions interne
                label_inst_create(else_label);
            }
            break;

        case NODE_FOR:; // ici il faut intercaler les labels et les instructions de bloc entre les insts 0, 1, et 2 (exemple I-7)
            int32_t loop_label_for = get_new_label();
            int32_t post_label_for = get_new_label();
            gen_code_expr(root->opr[0]); // instruction initiale (i = s)
            label_inst_create(loop_label_for); // _L1
            gen_code_expr(root->opr[1]); // test (i < e)
            beq_inst_create(get_current_reg(), 0, post_label_for); // beq pour sortir vers _L2
            gen_code_passe_2(root->opr[3]); // bloc d'expressions interne (sum = sum + i)
            gen_code_expr(root->opr[2]); // incrément (i = i+1)
            j_inst_create(loop_label_for); // j _L1
            label_inst_create(post_label_for); // _L2
            return;
            break;  

        case NODE_WHILE:; // une boucle while se comporte de manière similaire à une boucle for, sans inst. initiale ni incrément
            int32_t loop_label_while = get_new_label();
            int32_t post_label_while = get_new_label();
            label_inst_create(loop_label_while);
            gen_code_expr(root->opr[0]); // test
            beq_inst_create(get_current_reg(), 0, post_label_while); // beq pour sortir
            gen_code_passe_2(root->opr[1]); // bloc d'expressions interne
            j_inst_create(loop_label_while); 
            label_inst_create(post_label_while);
            break;

        case NODE_DOWHILE:; // une boucle dowhile place le teste et le beq APRES l'instruction bloc
            int32_t loop_label_do = get_new_label();
            int32_t post_label_do = get_new_label();
            label_inst_create(loop_label_do);
            gen_code_passe_2(root->opr[1]); // bloc d'expressions interne
            gen_code_expr(root->opr[0]); // test
            beq_inst_create(get_current_reg(), 0, post_label_do); // beq pour sortir
            j_inst_create(loop_label_do); 
            label_inst_create(post_label_do);
            break;

        case NODE_PRINT:
            in_print = true;
            break;

        case NODE_STRINGVAL: 
            if(in_print) { // charger une chaîne globale depuis .data
                // printf("%s, offset %d\n",root->str, root->offset);
                lui_inst_create(4,0x1001);
                ori_inst_create(4,4,root->offset);
                ori_inst_create(2,0,4);
                syscall_inst_create();
            }
            break;

        case NODE_IDENT:
            if(in_print) { // afficher une valeur de variable
                if(root->decl_node->global_decl) {
                    lui_inst_create(4,0x1001);
                    lw_inst_create(4,root->decl_node->offset,4);
                }
                else {
                    lw_inst_create(4,root->decl_node->offset,29);
                }
                ori_inst_create(2,0,1);
                syscall_inst_create();
                break;
            }
        
        default:
            break;
    }

    for(int n = 0; n < (root->nops); n++) {
        if(root->opr[n] != NULL) gen_code_passe_2(root->opr[n]);
    }

    switch(root->nature) {

        case NODE_PROGRAM:
            ori_inst_create(2, 0, 10); // fin du programme
            syscall_inst_create();
            break;

        case NODE_FUNC:
            stack_deallocation_inst_create(get_temporary_max_offset() + root->offset);
            break;

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
            gen_code_expr(root);
            release_reg();
            break;

        case NODE_PRINT:
            in_print = false;
            break;
        
        default:
            break;
    }
}
