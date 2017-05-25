#include <malloc.h>
#include <string.h>
#include "Tree.h"
#include <math.h>
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
err_code tree_addNode_(Tree* tree, tree_node** cur, void* dataPtr){
	void* tmp;
	int res;
	if(!(*cur)){
		*cur=calloc(1, sizeof(tree_node));
		tmp=calloc(1,tree->datalen);
		if(!(*cur && tmp)) return ERR_MALLOC_FAIL;
		memcpy(tmp, dataPtr, tree->datalen);
		(*cur)->data=tmp;
		return OK;
	}else{
		res=(tree->cmp)((*cur)->data, dataPtr);
		if(res<0) tmp=&((*cur)->left);
		else tmp=&((*cur)->right);
		res=tree_addNode_(tree, tmp, dataPtr);
		return res;
		
	}
}
err_code tree_del_by_ptr_(Tree* tree, tree_node** ptr){
	int res=0;
	if(*ptr){
	free((*ptr)->data);
	res=res||tree_del_by_ptr_(tree,&(*ptr)->right);
	res=res||tree_del_by_ptr_(tree,&(*ptr)->left);
	free(*ptr);
	*ptr=NULL;
	return res;
	}else return OK;
	
}
err_code tree_delNode_(Tree* tree, tree_node** cur, void* dataPtr){
	void* tmp;
	int res;
	if(!(*cur)){
		return ERR_NOT_FOUND;
	}else{
		res=(tree->cmp)((*cur)->data, dataPtr);
		if(res<0) tmp=&((*cur)->left);
		else{ tmp=&((*cur)->right); if(!res){return tree_del_by_ptr_(tree, cur);}}; 
		res=tree_delNode_(tree, tmp, dataPtr);
		return res;
		
	}
}
err_code tree_addNode(Tree* tree, void* dataPtr){
	if(!tree) return ERR_NULL_POINTER;
	if(!dataPtr) return ERR_NULL_POINTER;
	return tree_addNode_(tree, &(tree->root), dataPtr);
}
err_code tree_delNode(Tree* tree, void* dataPtr){
	if(!tree) return ERR_NULL_POINTER;
	if(!dataPtr) return ERR_NULL_POINTER;
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
		return 1+tree_getNodeCount_(ptr->right)+tree_getNodeCount_(ptr->left);
		
	}else return 0;
	
}
int tree_getHeight(Tree* tree){
	if(!tree) return -1;
	return tree_getHeight_(tree->root);
}
int tree_getNodeCount(Tree* tree){
	if(!tree) return -1;
	return tree_getNodeCount_(tree->root);
}
err_code tree_destroy(Tree** _tree){
	int ret;
	Tree* tree;
	if(!_tree) return ERR_NULL_POINTER;
	tree=*_tree;
	if(!tree) return ERR_NULL_POINTER;
	ret= tree_del_by_ptr_(tree, &tree->root);
	free(tree);
	*_tree=NULL;
	return ret;
}
err_code tree_walk_pre(tree_node* ptr, do_func callback, int lvl){
	int ret=0;
	if (!ptr) return 0;
	ret=callback(ptr->data, lvl);
	ret=ret||tree_walk_pre(ptr->left, callback, lvl+1);
	ret=ret||tree_walk_pre(ptr->right, callback, lvl+1);
	return ret*ERR_CALLBACK_FAIL;
}
err_code tree_walk_in(tree_node* ptr, do_func callback, int lvl){
	int ret=0;
	if (!ptr) return 0;
	ret=ret||tree_walk_pre(ptr->left, callback, lvl+1);
	ret=ret||callback(ptr->data, lvl);
	ret=ret||tree_walk_pre(ptr->right, callback, lvl+1);
	return ret*ERR_CALLBACK_FAIL;
}
err_code tree_walk_post(tree_node* ptr, do_func callback, int lvl){
	int ret=0;
	if (!ptr) return 0;
	ret=ret||tree_walk_pre(ptr->left, callback, lvl+1);
	ret=ret||tree_walk_pre(ptr->right, callback, lvl+1);
	ret=ret||callback(ptr->data, lvl);
	return ret*ERR_CALLBACK_FAIL;
}
err_code tree_walk_search(Tree* tree, tree_node* cur, void* dataPtr, do_func callback, int lvl){
	int res;
	int ret=0;
	if (!cur) return 0;
	res=(tree->cmp)(cur->data, dataPtr);
	if(res<0) ret=ret||tree_walk_search(tree, cur->left, dataPtr, callback, lvl+1);
	else{
	if(res==0) ret=ret||callback(cur->data, lvl);
	ret=ret||tree_walk_search(tree, cur->right, dataPtr, callback, lvl+1);
	}
	return ret*ERR_CALLBACK_FAIL;
}
err_code tree_walk(Tree* tree,  do_func callback, wk_method walk_method, void* search_for){
	if(!tree) return ERR_NULL_POINTER;
	if(!callback) return ERR_NULL_POINTER;
	if(search_for) return tree_walk_search(tree, tree->root, search_for, callback, 1);
	switch(walk_method){
		case WK_PRE_ORDER:
			return tree_walk_pre(tree->root, callback, 1);
		case WK_IN_ORDER:
			return tree_walk_in(tree->root, callback, 1);
		case WK_POST_ORDER:
			return tree_walk_post(tree->root, callback, 1);
		default:
			return ERR_UNKNOWN_WK_METHOD;
	}
	
}


tree_node* leftRotate(tree_node* n){
		if(n->right!=NULL){
			tree_node* right = n->right;
			n->right=right->right;
			right->right=right->left;
			right->left=n->left;
			n->left=right;
			void* temp;
			temp=n->data;
			n->data=right->data;
			right->data=temp;
		}
		return n;
}

tree_node* rightRotate(tree_node* n){
		if(n->left!=NULL){
			tree_node* left = n->left;
			n->left=left->left;
			left->left=left->right;
			left->right=n->right;
			n->right=left;
			void* temp;
			temp=n->data;
			n->data=left->data;
			left->data=temp;
		}
		return n;
}

tree_node* createRightVine(tree_node* root){
	while(root->left!=NULL){
		root=rightRotate(root);
	}
	if(root->right!=NULL) root->right=createRightVine(root->right);
	return root;
}
tree_node* balanceVine(tree_node* root, int nodeCount){
	int times, i, j;
	times=(int)(log(nodeCount)/log(2));
	tree_node* newRoot;
	newRoot=root;
	for(i=0; i<times; i++){
		newRoot=leftRotate(newRoot);;
		root=newRoot->right;
		for(j=0; j<nodeCount/2-1; j++){
			root=leftRotate(root);
			root=root->right;
		}
		nodeCount=nodeCount>>1;
	}
	return newRoot;
}
err_code tree_balance(Tree* tree){
	if(!tree) return ERR_NULL_POINTER;
	tree_node* vine;
	vine=createRightVine(tree->root);
	tree->root=balanceVine(vine, tree_getNodeCount_(vine));
	return OK;
}
