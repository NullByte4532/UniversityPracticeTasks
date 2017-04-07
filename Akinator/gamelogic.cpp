#include <cstdio>
#include <malloc.h>
#include <cstring>
#include "gamelogic.h"
void readq(FILE* f, char* ptr, int n){
	int i;
	char c;
	for(i=0; i<n; i++){
	c=fgetc(f);
	if(c==';'){ptr[i]='\0'; return;}
	ptr[i]=c;	
	}
}
Tree::Tree():
root(NULL),
cur(NULL)
{}
void Tree::del(node* ptr){
	if(ptr){
	del(ptr->yes);
	del(ptr->no);
	free(ptr->question);
	free(ptr);
	}
}
Tree::~Tree(){
	del(root);
}
char* Tree::get(){
	return cur->question;	
}
void Tree::add(char* question, answer current, char* new_name){
	node* ptr;
	node* ptr2;
	ptr=(node*) calloc(1, sizeof(node));
	ptr2=(node*) calloc(1, sizeof(node));
	ptr2->question=(char*)calloc(1+strlen(new_name), sizeof(char));
	ptr->question=(char*)calloc(1+strlen(question), sizeof(char));
	strcpy(ptr2->question, new_name);
	strcpy(ptr->question, question);
	*prev=ptr;
	if(current==YES){
		ptr->yes=cur;
		ptr->no=ptr2;	
	}else{
		ptr->no=cur;
		ptr->yes=ptr2;
	}
	
}
char Tree::step(answer direction){
	if(direction==YES){
		prev=&cur->yes;
		cur=cur->yes;
	}else{
		prev=&cur->no;
		cur=cur->no;
	}
	
	
	if(cur->no) return 0;
	else return 1;
}
void Tree::rewind(){
	cur=root;
}
void Tree::save_(FILE*f, node* ptr){
	fprintf(f,"(%s; ", ptr->question);
	if(ptr->yes) save_(f, ptr->yes);
	else fprintf(f, "N");
	fprintf(f,"; ");
	if(ptr->no) save_(f, ptr->no);
	else fprintf(f, "N");
	fprintf(f,") ");
}
void Tree::save(FILE* f){
	save_(f, root);
}
node* Tree::load_(FILE* f){
	char tmp[128];
    node* ptr;
    ptr=(node*)calloc(1, sizeof(node));
    if (fgetc(f)=='N'){ fgetc(f); fgetc(f); return NULL;}
    readq(f, tmp, 127);
    fgetc(f);
	ptr->question=(char*)calloc(1+strlen(tmp), sizeof(char));
    strcpy(ptr->question, tmp);
    ptr->yes=load_(f);
    if(ptr->yes){ fgetc(f); fgetc(f);}
    ptr->no=load_(f);
    if(ptr->yes){ fgetc(f); fgetc(f);}
	return ptr;
}
void Tree::load(FILE* f){
	root=load_(f);
	cur=root;
}

