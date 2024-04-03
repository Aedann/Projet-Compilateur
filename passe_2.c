
#include <stdio.h>

#include "utils/miniccutils.h"
#include "defs.h"
#include "passe_2.h"



void gen_code_passe_2(node_t root) {
    switch(root->nature) {

        case NODE_FUNC:
            reset_temporary_max_offset();
            set_temporary_start_offset(root->offset);
            stack_allocation_inst_create();
            break;

        case NODE_IDENT:
            if(reg_available()) {
                allocate_reg();
            }
            else {
                push_temporary(get_current_reg());
            }
            break;
        
        default:
            break;
    }

    for(int n = 0; n < (root->nops); n++) {
        if(root->opr[n] != NULL) gen_code_passe_2(root->opr[n]);
    }

    switch(root->nature) {

        case NODE_FUNC:
            stack_deallocation_inst_create(root->offset + get_temporary_max_offset());
            break;
        
        default:
            break;
    }
}
  
