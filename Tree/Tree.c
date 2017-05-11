#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "Tree.h"
/*
typedef int (*cmp_func)(void*, void*);
typedef int (*do_func)(void*);
typedef struct stree_node tree_node;
typedef struct sTree Tree;
struct stree_node{void* data; tree_node* right; tree_node* left; int height; int ncount;};
struct sTree{tree_node* root; size_t datalen; cmp_func cmp;};
*/
Tree* tree_create(size_t datalen, cmp_func cmp){
	Tree* tmp;
	tmp=calloc(1, sizeof(Tree));
	if(tmp){
	tmp->datalen=datalen;
	tmp->cmp=cmp;
	return tmp;
	}
	else return NULL;
}  
int tree_addNode_(Tree* tree, tree_node** cur, void* dataPtr){
	void* tmp;
	int res;
	if(!(*cur)){
		*cur=calloc(1, sizeof(tree_node));
		tmp=calloc(1,tree->datalen);
		if(!(*cur && tmp)) return 1;
		memcpy(tmp, dataPtr, tree->datalen);
		(*cur)->data=tmp;
		return 0;
	}else{
		res=(tree->cmp)((*cur)->data, dataPtr);
		if(res<0) tmp=&((*cur)->left);
		else tmp=&((*cur)->right);
		res=tree_addNode_(tree, tmp, dataPtr);
		return res;
		
	}
}
int tree_del_by_ptr_(Tree* tree, tree_node** ptr){
	int res=0;
	if(*ptr){
	free((*ptr)->data);
	res=res||tree_del_by_ptr_(tree,&(*ptr)->right);
	res=res||tree_del_by_ptr_(tree,&(*ptr)->left);
	free(*ptr);
	*ptr=NULL;
	return res;
	}else return 0;
	
}
int tree_delNode_(Tree* tree, tree_node** cur, void* dataPtr){
	void* tmp;
	int res;
	if(!(*cur)){
		return 4;
	}else{
		res=(tree->cmp)((*cur)->data, dataPtr);
		if(res<0) tmp=&((*cur)->left);
		else{ tmp=&((*cur)->right); if(!res){return tree_del_by_ptr_(tree, cur);}}; 
		res=tree_delNode_(tree, tmp, dataPtr);
		return res;
		
	}
}
int tree_addNode(Tree* tree, void* dataPtr){
	if(!tree) return 2;
	if(!dataPtr) return 3;
	return tree_addNode_(tree, &(tree->root), dataPtr);
}
int tree_delNode(Tree* tree, void* dataPtr){
	if(!tree) return 2;
	if(!dataPtr) return 3;
	return tree_delNode_(tree, &(tree->root), dataPtr);
}
int max(int a, int b){
if(a>b) return a;
else return b;		
}
int tree_getHeight_(tree_node* ptr){
	if(ptr){
		return 1+max(tree_getHeight_(ptr->right),tree_getHeight_(ptr->left));
		
	}else return 0;
	
}
int tree_getNodeCount_(tree_node* ptr){
	if(ptr){
		return 1+tree_getHeight_(ptr->right)+tree_getHeight_(ptr->left);
		
	}else return 0;
	
}
int tree_getHeight(Tree* tree){
	
	return tree_getHeight_(tree->root);
}
int tree_getNodeCount(Tree* tree){
	
	return tree_getNodeCount_(tree->root);
}
int tree_destroy(Tree* tree){
	int ret;
	ret= tree_del_by_ptr_(tree, &tree->root);
	free(tree);
	return ret;
}
int tree_walk_pre(tree_node* ptr, do_func callback){
	int ret=0;
	ret=callback(ptr->data);
	ret=ret||tree_walk_pre(ptr->left, callback);
	ret=ret||tree_walk_pre(ptr->right, callback);
	return ret;
}
int tree_walk_in(tree_node* ptr, do_func callback){
	int ret=0;
	ret=ret||tree_walk_pre(ptr->left, callback);
	ret=ret||callback(ptr->data);
	ret=ret||tree_walk_pre(ptr->right, callback);
	return ret;
}
int tree_walk_post(tree_node* ptr, do_func callback){
	int ret=0;
	ret=ret||tree_walk_pre(ptr->left, callback);
	ret=ret||tree_walk_pre(ptr->right, callback);
	ret=ret||callback(ptr->data);
	return ret;
}
int tree_walk_search(Tree* tree, tree_node* cur, void* dataPtr, do_func callback){
	int res;
	int ret=0;
	if (!cur) return 0;
	res=(tree->cmp)(cur->data, dataPtr);
	if(res<0) ret=ret||tree_walk_search(tree, cur->left, dataPtr, callback);
	else{
	if(res==0) ret=ret||callback(cur->data);
	ret=ret||tree_walk_search(tree, cur->right, dataPtr, callback);
	}
	return ret;
}
int tree_walk(Tree* tree,  do_func callback, wk_method walk_method, void* search_for){
	if(search_for) return tree_walk_search(tree, tree->root, search_for, callback);
	switch(walk_method){
		case WK_PRE_ORDER:
			return tree_walk_pre(tree->root, callback);
		case WK_IN_ORDER:
			return tree_walk_in(tree->root, callback);
		case WK_POST_ORDER:
			return tree_walk_post(tree->root, callback);
		default:
			return 5;
	}
	
}
void tree_rotateLeft(tree_node** cur){
		tree_node* tmp_node;
		tmp_node=(*cur)->left;
		(*cur)->left=tmp_node->right;
		tmp_node->right=(*cur);
		(*cur) = tmp_node;
}
void tree_rotateRight(tree_node** cur){
		tree_node* tmp_node;
		tmp_node=(*cur)->right;
		(*cur)->right=tmp_node->left;
		tmp_node->left=(*cur);
		(*cur) = tmp_node;
}
int tree_balance(Tree* tree){
	
	
}
int main(){
	
	
return 0;	
}
