# Passe 2

La passe de génération de code, ou passe 2, effectue un parcours de l’arbre au cours duquel sont générées
les instructions assembleur du programme. Il n’y a plus de vérification à effectuer au cours de cette passe,
sauf éventuellement à l’aide d’asserts.

 • La fonction void stack_allocation_inst_create() crée une instruction d’allocation en
pile, qui sera mise à jour par la suite : en effet, au début de l’analyse d’une fonction MiniC
lors de la passe 2, on ne sait pas encore de combien de temporaires on aura besoin en pile.
Cette valeur sera connue à la fin de l’analyse de la fonction MiniC, et mise à jour à partir de la
valeur passée en paramètre à la fonction void stack_deallocation_inst_create(int32_t
val), qui doit donc être appelée pour désallouer la place en pile nécessaire.

Les fonctions get_global_strings_number() et get_global_string() sont à appeler lors de la passe 2.
• int32_t get_global_strings_number() : retourne le nombre de chaines de caractères
littérales. Cette fonction devrait être utilisée pour la déclaration des chaines littérales en
section .data.
• char * get_global_string(int32_t index) : retourne la chaine de caractères littérale
d’index index, qui doit être strictement inférieur à la valeur retournée par
get_global_strings_number(). Cette fonction devrait être utilisée pour la déclaration des
chaines littérales en section .data.
• free_global_strings() : libère la mémoire allouée pour les chaines littérales.


## 6 Implémentation de l’allocateur de registres


# Test OK/KO : 

## Verif (passe 1) : 

### KO : 
 * 22 Sur les opérateurs avec int et bool et peut être int int ou bool bool 
 * Verifier quand on met plusieurs fonctions
 * printf("Expected 2 operands around '=' sign on line %d\n", expr->lineno);   
 * printf("Different types around '=' sign on line %d\n", expr->lineno);   
 * printf("Error : declared function other than main on line %d\n", root->lineno);
 * 

