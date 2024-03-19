
#include <stdio.h>

#include "defs.h"
#include "miniccutils.h"
#include "passe_1.h"

extern int trace_level;

void analyse_passe_1(node_t root) {

    if(root == NULL) return;
    switch(root->nature) {
        case NODE_PROGRAM:
            push_global_context();
            break;

        case NODE_BLOCK:
            global_env = false;
            push_context();
            break;

        case NODE_IDENT:
            int eadr = env_add_element(root->ident,root);
            printf("%d\n",eadr);
            if(eadr < 0) { // réutilisation
                root->decl_node = get_decl_node(root->ident);
            }
            else { // déclaration
                printf("Decl : %s, eadr = %d\n",root->ident, eadr);
                root->global_decl = global_env;
                root->offset = eadr;
            }
            // root->type = last_type;
            break;

        case NODE_STRINGVAL:
            root->offset = add_string(root->str);
            break;

    
        case NODE_FUNC:
            global_env = false;
            reset_env_current_offset();
            break;

        case NODE_TYPE:
            // last_type = root->type;
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

        case NODE_FUNC:
            root->offset = get_env_current_offset();
            break;

        default:
            break;
    }

}

  
    