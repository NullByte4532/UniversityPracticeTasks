typedef int (*cmp_func)(void*, void*); //compare function
typedef int (*do_func)(void*); //
typedef struct stree_node tree_node;
typedef struct sTree Tree;
struct stree_node{void* data; tree_node* right; tree_node* left;};
struct sTree{tree_node* root; size_t datalen; cmp_func cmp;};
enum wk_method {WK_PRE_ORDER, WK_IN_ORDER, WK_POST_ORDER};
typedef enum wk_method wk_method;




Tree* tree_create(size_t datalen, cmp_func cmp);
int tree_addNode(Tree* tree, void* dataPtr);
int tree_delNode(Tree* tree, void* dataPtr);
int tree_getHeight(Tree* tree);
int tree_getNodeCount(Tree* tree);
int tree_destroy(Tree* tree);
int tree_walk(Tree* tree, do_func callback, wk_method walk_method, void* search_for);
//----------------------------------------------------------------------------------
int tree_balance(Tree* tree);
