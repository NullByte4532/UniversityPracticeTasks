#include <stdio.h>
#include "Tree.h"
int cmp(void* a, void* b){
	int a_, b_;
	a_=*(int*)a;
	b_=*(int*)b;
	if(a_==b_) return 0;
	if(a_>b_) return 1;
	if(a_<b_) return -1;
	
	return 0;
}
int treeprt(void* data, int lvl){
	printf("(%d; %d)\n", lvl, *(int*)data);
	return 0;
}
int main(){
	Tree* tree;
	int i, j;
	tree = tree_create(sizeof(int), cmp);
	for (i=0; i<8; i++){
		tree_addNode(tree, &i);
	}
	tree_walk(tree, treeprt, WK_PRE_ORDER, NULL);
	tree_balance(tree);
	for(j=0; j<100; j++){
	i++;
	tree_addNode(tree, &i);
	tree_balance(tree);
	i++;
	tree_addNode(tree, &i);
	i++;
	tree_addNode(tree, &i);
	tree_balance(tree);
	}
	for(j=0; j<10; j++){i++; tree_addNode(tree, &i);}
	tree_balance(tree);
	tree_balance(tree);
	
	printf("--------------------------------\n");
	tree_walk(tree, treeprt, WK_PRE_ORDER, NULL);
	printf("--------------------------------\n");
	printf("n=%d\n", tree_getNodeCount(tree));
	printf("h=%d\n", tree_getHeight(tree));
	printf("--------------------------------\n");
	i=10;
	tree_walk(tree, treeprt, WK_PRE_ORDER, &i);
	tree_delNode(tree, &i);
	printf("--------------------------------\n");
	tree_walk(tree, treeprt, WK_PRE_ORDER, &i); 
	tree_addNode(tree, &i);
	printf("--------------------------------\n");
	tree_walk(tree, treeprt, WK_PRE_ORDER, &i);
		
	tree_destroy(tree);
return 0;	
}
