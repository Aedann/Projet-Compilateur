
#include <stdio.h>

#include "utils/miniccutils.h"
#include "defs.h"
#include "passe_2.h"



void gen_code_passe_2(node_t root) {
    switch(root->nature) {

        case NODE_PROGRAM:
            data_sec_inst_create();
            for(int gsindex = 0; gsindex < get_global_strings_number(); gsindex++) {
                asciiz_inst_create(NULL,get_global_string(gsindex));
            }
            break;


        case NODE_FUNC:
            text_sec_inst_create();
            label_str_inst_create("main");
            reset_temporary_max_offset();
            set_temporary_start_offset(root->offset);
            stack_allocation_inst_create();
            break;

        case NODE_IDENT:
            if(root->global_decl) { // global decl = we're in the .data segment
                word_inst_create(root->ident,root->value);
            }
            else {
                if(reg_available()) {
                    allocate_reg();
                }
                else {
                    push_temporary(get_current_reg());
                }
            }
            break;

        case NODE_FOR:
        case NODE_WHILE:
        case NODE_DOWHILE:
            label_inst_create(get_new_label());
            break;
        
        default:
            break;
    }

    for(int n = 0; n < (root->nops); n++) {
        if(root->opr[n] != NULL) gen_code_passe_2(root->opr[n]);
    }

    switch(root->nature) {

        case NODE_FUNC:
            stack_deallocation_inst_create(get_temporary_max_offset());
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
            break;
        
        default:
            break;
    }
}
  
