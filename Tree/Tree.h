#ifndef __Tree_H_INCLUDED__ 
#define __Tree_H_INCLUDED__
#include <stddef.h>
typedef int (*cmp_func)(void*, void*); //compare function
typedef int (*do_func)(void*, int); //callback function. First parameter is a pointer to data, second- height at which node is located. Must return 0 in case of success
typedef struct stree_node tree_node; 
typedef struct sTree Tree;
typedef struct sstack_node stack_node;
struct stree_node{void* data; tree_node* right; tree_node* left;}; //structure for tree node
struct sTree{tree_node* root; size_t datalen; cmp_func cmp;}; //structure for tree 'object'

enum wk_method {WK_PRE_ORDER, WK_IN_ORDER, WK_POST_ORDER}; //enum defining walk methods
enum err_code {OK, ERR_NULL_POINTER, ERR_MALLOC_FAIL, ERR_CALLBACK_FAIL, ERR_NOT_FOUND, ERR_UNKNOWN_WK_METHOD};
typedef enum wk_method wk_method;

typedef enum err_code err_code;


Tree* tree_create(size_t datalen, cmp_func cmp); //Creates tree, returns pointer. Requires data length and compare function. In case of failure returns NULL
err_code tree_addNode(Tree* tree, void* dataPtr); //Adds a node to an existing tree. Requires a pointer to tree and a pointer to data. Returns error code
err_code tree_delNode(Tree* tree, void* dataPtr); //Deletes an existing node and all its children from a tree. Requires a pointer to tree and a pointer to data. Returns error code
int tree_getHeight(Tree* tree); //Returns height of the tree. -1 if pointer is null
int tree_getNodeCount(Tree* tree); //Returns number of nodes in the tree. -1 if pointer is null
err_code tree_destroy(Tree** tree); //Destroys the tree. Returns error code
err_code tree_walk(Tree* tree, do_func callback, wk_method walk_method, void* search_for); //Walks the tree.
/* Requires a pointer to tree, callback function, walk method, pointer to what to look for.
 * If search_for is specified, Tree works as a binary search tree and callback is called on every match. walk_method is ignored.
 * If search_for is NULL, callback is called on every node, order is determined by walk_method.
 * Returns error code
 */
err_code tree_balance(Tree* tree); //balances the tree. Returns 1 if tree pointer is null.
#endif
