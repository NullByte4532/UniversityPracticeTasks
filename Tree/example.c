#include <stdio.h>
#include "Tree.h"
int cmp(void* a, void* b){ //compare function for integers
	int a_, b_;
	a_=*(int*)a;
	b_=*(int*)b;
	if(a_==b_) return 0;
	if(a_>b_) return 1;
	if(a_<b_) return -1;
	
	return 0;
}
int cmp2(void* a, void* b){ //compare function for doubles
	double a_, b_;
	a_=*(double*)a;
	b_=*(double*)b;
	if(a_==b_) return 0;
	if(a_>b_) return 1;
	if(a_<b_) return -1;
	
	return 0;
}
int treeprt(void* data, int lvl){ // function to print integers
	printf("(%d; %d)\n", lvl, *(int*)data);
	return 0;
}
int treeprt2(void* data, int lvl){ // function to print doubles
	printf("(%d; %d)\n", lvl, *(double*)data);
	return 0;
}
int main(){
	Tree* tree;
	Tree* tree2; 
	int i, j;  
	tree = tree_create(sizeof(int), cmp); //Create tree of integers
	for (i=0; i<8; i++){  //Fill tree with numbers from 0 to 7
		tree_addNode(tree, &i);
	}
	tree_walk(tree, treeprt, WK_PRE_ORDER, NULL); //print tree
	tree_balance(tree);  //balance
	for(j=0; j<100; j++){  //add more numbers and test balancing at different stages
	i++;
	tree_addNode(tree, &i);
	tree_balance(tree);
	i++;
	tree_addNode(tree, &i);
	i++;
	tree_addNode(tree, &i);
	tree_balance(tree);
	}
	for(j=0; j<10; j++){i++; tree_addNode(tree, &i);} //add more things to tree
	tree_balance(tree); //balance
	tree2 = tree_create(sizeof(double), cmp2); //meanwhile, lets create a second tree
	for (j=0; j<8; j++){  //Fill second tree with numbers from 0 to 7
		tree_addNode(tree2, &j);
	}
	tree_balance(tree2); //balance
	printf("--------------------------------\n");
	tree_walk(tree, treeprt, WK_PRE_ORDER, NULL); //print tree
	printf("--------------------------------\n");
	printf("n=%d\n", tree_getNodeCount(tree)); //print tree node count
	printf("h=%d\n", tree_getHeight(tree)); //print tree height
	printf("--------------------------------\n");
	i=10; 
	tree_walk(tree, treeprt, WK_PRE_ORDER, &i); //find and print only 10
	tree_delNode(tree, &i); //delete 10 from tree
	printf("--------------------------------\n"); 
	tree_walk(tree, treeprt, WK_PRE_ORDER, &i);  // try to find and print only 10
	tree_addNode(tree, &i); //add 10 back to tree
	printf("--------------------------------\n");
	tree_walk(tree, treeprt, WK_PRE_ORDER, &i);  //find and print only 10
	printf("--------------------------------\n");
	tree_walk(tree2, treeprt2, WK_PRE_ORDER, NULL); //print tree2
	tree_destroy(tree); //destroy trees and free memory
	tree_destroy(tree2);//
return 0;	
}
