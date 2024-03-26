
#ifndef _PASSE_1_H_
#define _PASSE_1_H_

#include "defs.h"
#include <string.h>

typedef struct expverif_s {
    node_nature nexpr;
    int nops;
    node_type ltype;
    node_type rtype;
    node_type outtype;
} expverif;

void analyse_passe_1(node_t root);

void init_exprverif();


#endif

