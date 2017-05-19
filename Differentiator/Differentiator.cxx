#include <cstring.h>
#include "Config.hxx"
typedef struct {
	char func[F_MAX_LEN];
	tree_node* arg1;
	tree_node* arg2;
} tree_node;
tree_node* constant(double c){
	tree_node* tmp;
	tmp=calloc(1, sizeof(tree_node));
	snprintf(tmp->func, F_MAX_LEN-1, "%f", c); 
	return tmp;
}
tree_node* zero(){
	return constant(0);
}
tree_node* summ(tree_node* a, tree_node* b){
	tree_node* tmp;
	if(isdigit(b->func[0])){
		sscanf(b->func, "%f", tmp_);
		if(tmp==0) return a;
	}
	if(isdigit(a->func[0])){
		sscanf(a->func, "%f", tmp_);
		if(tmp==0) return b;
	}
	tmp=calloc(1, sizeof(tree_node));
	tmp->func[0]='+';
	tmp->arg1=a;
	tmp->arg2=b;
	return tmp;
	
}
tree_node* sub(tree_node* a, tree_node* b){
	tree_node* tmp;
	if(isdigit(b->func[0])){
		sscanf(b->func, "%f", tmp_);
		if(tmp==0) return a;
	}
	tmp=calloc(1, sizeof(tree_node));
	tmp->func[0]='-';
	tmp->arg1=a;
	tmp->arg2=b;
	return tmp;
	
}
tree_node* divide(tree_node* a, tree_node* b){
	tree_node* tmp;
	if(isdigit(a->func[0])){
		sscanf(a->func, "%f", tmp_);
		if(tmp==0) return zero();
	}
	if(isdigit(b->func[0])){
		sscanf(b->func, "%f", tmp_);
		if(tmp==1) return a;
	}
	tmp=calloc(1, sizeof(tree_node));
	tmp->func[0]='/';
	tmp->arg1=a;
	tmp->arg2=b;
	return tmp;
	
}
tree_node* multiply(tree_node* a, tree_node* b){
	tree_node* tmp;
	double tmp_;
	if(isdigit(a->func[0])){
		sscanf(a->func, "%f", tmp_);
		if(tmp==0) return zero();
		if(tmp==1) return b;
	}
	if(isdigit(b->func[0])){
		sscanf(b->func, "%f", tmp_);
		if(tmp==0) return zero();
		if(tmp==1) return a;
	}
	tmp=calloc(1, sizeof(tree_node));
	tmp->func[0]='*';
	tmp->arg1=a;
	tmp->arg2=b;
	return tmp;
}
tree_node* power(tree_node* a, tree_node* b){
	tree_node* tmp;
	if(isdigit(a->func[0])){
		sscanf(a->func, "%f", tmp_);
		if(tmp==0) return 0;
		if(tmp==1) return 1;
	}
	if(isdigit(b->func[0])){
		sscanf(b->func, "%f", tmp_);
		if(tmp==0) return 1;
		if(tmp==1) return a;
	}
	tmp=calloc(1, sizeof(tree_node));
	tmp->func[0]='^';
	tmp->arg1=a;
	tmp->arg2=b;
	return tmp;
}
tree_node* diff(tree_node* f){
	if(isdigit(f->func[0])){
		return zero();
	}
	else if(!strcmp(f->func, "*")){
		return summ(multiply(diff(f->arg1), f->arg2), multiply(diff(f->arg2), f->arg1))
	}
	else if(!strcmp(f->func, "+")){
		return summ(diff(f->arg1), diff(f->arg2));
	}
	else if(!strcmp(f->func, "-")){
		return sub(diff(f->arg1), diff(f->arg2));
	}
	else if(!strcmp(f->func, "/")){
		return divide(sub(multiply(diff(f->arg1), f->arg2), multiply(diff(f->arg2), f->arg1)), power(f->arg2, constant(2)));
	}
}
int main(){
	
return 0;	
}
