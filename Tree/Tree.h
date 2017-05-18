#include <stddef.h>
typedef int (*cmp_func)(void*, void*); //compare function
typedef int (*do_func)(void*, int); //callback function
typedef struct stree_node tree_node;
typedef struct sTree Tree;
typedef struct sstack_node stack_node;
struct stree_node{void* data; tree_node* right; tree_node* left;};
struct sTree{tree_node* root; size_t datalen; cmp_func cmp; stack_node* stack;stack_node* stack_l;};
struct sstack_node { tree_node** data; stack_node* next;};

enum wk_method {WK_PRE_ORDER, WK_IN_ORDER, WK_POST_ORDER};
typedef enum wk_method wk_method;




Tree* tree_create(size_t datalen, cmp_func cmp); //Creates tree, returns pointer. Requires data length and compare function. In case of failure returns NULL
int tree_addNode(Tree* tree, void* dataPtr); //Adds a node to an existing tree. Requires a pointer to tree and a pointer to data. Returns error codes 1-memory allocation error, 2-tree pointer is null, 3-data pointer is null
int tree_delNode(Tree* tree, void* dataPtr); //Deletes an existing node and all its children from a tree. Requires a pointer to tree and a pointer to data. Returns error codes 4- not found, 2-tree pointer is null, 3-data pointer is null
int tree_getHeight(Tree* tree); //Returns height of the tree. -1 if pointer is null
int tree_getNodeCount(Tree* tree); //Returns number of nodes in the tree. -1 if pointer is null
int tree_destroy(Tree* tree); //Destroys the tree. Returns error code 1 if pointer is null
int tree_walk(Tree* tree, do_func callback, wk_method walk_method, void* search_for); //Walks the tree.
/* Requires a pointer to tree, callback function, walk method, pointer to what to look for.
 * If search_for is specified, Tree works as a binary search tree and callback is called on every match. walk_method is ignored.
 * If search_for is NULL, callback is called on every node, order is determined by walk_method.
 * Returns error codes 2- tree pointer is null. 3- callback is null. 1- callback failed. 5- unknown walk method.
 */
int tree_balance(Tree* tree); //balances the tree. Returns 1 if tree pointer is null.
