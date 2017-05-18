#include <malloc.h>
#include <string.h>
#include "Tree.h"
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
int tree_destroy(Tree* tree){
	int ret;
	if(!tree) return 1;
	ret= tree_del_by_ptr_(tree, &tree->root);
	free(tree);
	return ret;
}
int tree_walk_pre(tree_node* ptr, do_func callback, int lvl){
	int ret=0;
	if (!ptr) return 0;
	ret=callback(ptr->data, lvl);
	ret=ret||tree_walk_pre(ptr->left, callback, lvl+1);
	ret=ret||tree_walk_pre(ptr->right, callback, lvl+1);
	return ret;
}
int tree_walk_in(tree_node* ptr, do_func callback, int lvl){
	int ret=0;
	if (!ptr) return 0;
	ret=ret||tree_walk_pre(ptr->left, callback, lvl+1);
	ret=ret||callback(ptr->data, lvl);
	ret=ret||tree_walk_pre(ptr->right, callback, lvl+1);
	return ret;
}
int tree_walk_post(tree_node* ptr, do_func callback, int lvl){
	int ret=0;
	if (!ptr) return 0;
	ret=ret||tree_walk_pre(ptr->left, callback, lvl+1);
	ret=ret||tree_walk_pre(ptr->right, callback, lvl+1);
	ret=ret||callback(ptr->data, lvl);
	return ret;
}
int tree_walk_search(Tree* tree, tree_node* cur, void* dataPtr, do_func callback, int lvl){
	int res;
	int ret=0;
	if (!cur) return 0;
	res=(tree->cmp)(cur->data, dataPtr);
	if(res<0) ret=ret||tree_walk_search(tree, cur->left, dataPtr, callback, lvl+1);
	else{
	if(res==0) ret=ret||callback(cur->data, lvl);
	ret=ret||tree_walk_search(tree, cur->right, dataPtr, callback, lvl+1);
	}
	return ret;
}
int tree_walk(Tree* tree,  do_func callback, wk_method walk_method, void* search_for){
	if(!tree) return 2;
	if(!callback) return 3;
	if(search_for) return tree_walk_search(tree, tree->root, search_for, callback, 1);
	switch(walk_method){
		case WK_PRE_ORDER:
			return tree_walk_pre(tree->root, callback, 1);
		case WK_IN_ORDER:
			return tree_walk_in(tree->root, callback, 1);
		case WK_POST_ORDER:
			return tree_walk_post(tree->root, callback, 1);
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


void push(Tree* tree, tree_node** data){
	stack_node* tmp;
	tmp=calloc(1, sizeof(stack_node));
	tmp->data=data;
	tmp->next=NULL;
	if(tree->stack){
		
		 tree->stack_l->next=tmp;
		 tree->stack_l=tmp;
	}
	else{
		
		 tree->stack_l=tmp;
		 tree->stack=tmp;
	}
}
tree_node** pop(Tree* tree){
	tree_node** tmp;
	stack_node* tmp_;
	tmp=tree->stack->data;
	tmp_=tree->stack;
	tree->stack=tree->stack->next;
	free(tmp_);
	return tmp;
}
void grabheights(int* hr, int* hl, tree_node** cur){
	*hr=tree_getHeight_((*cur)->right);
	*hl=tree_getHeight_((*cur)->left);
	
}
int tree_balance_(Tree* tree, tree_node** start){
	
		if(start && *start){
			tree_getHeight(tree);
			push(tree,start);
			while (tree->stack){
				tree_node** cur;
				int hr, hl;
				cur=pop(tree);
				
				grabheights(&hr, &hl, cur);
				while(-(hr-hl)>1){  tree_rotateLeft(cur); grabheights(&hr, &hl, cur);}
				while((hr-hl)>1){  tree_rotateRight(cur); grabheights(&hr, &hl, cur);}
				
				if((*cur)->right)push(tree,&(*cur)->right);
				if((*cur)->left)push(tree,&(*cur)->left);
			}
			
		} return 0;
	
}
int tree_balance(Tree* tree){
	if(!tree) return 1;
	tree_balance_(tree,&tree->root);
	return 0;
}
