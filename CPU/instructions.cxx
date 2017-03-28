void cpu::op_mov(){
	double tmp;
	tmp=rargD();
	wargD(tmp);
}
void cpu::op_push(){
		checkError(stack.Push(rargD()));
}
void cpu::op_pop(){
	double tmp;
	checkError(stack.Pop(&tmp));
	wargD(tmp);
}

void cpu::op_add(){
		double a, b;
		checkError(stack.Pop(&a));
		checkError(stack.Pop(&b));
		checkError(stack.Push(a+b));
}


void cpu::op_sub(){
		double a, b;
		checkError(stack.Pop(&a));
		checkError(stack.Pop(&b));
		checkError(stack.Push(a-b));
}

void cpu::op_mul(){
		double a, b;
		checkError(stack.Pop(&a));
		checkError(stack.Pop(&b));
		checkError(stack.Push(a*b));
}

void cpu::op_div(){
		double a, b;
		checkError(stack.Pop(&a));
		checkError(stack.Pop(&b));
		checkError(stack.Push(a/b));
}

void cpu::op_jmp(){
	eip.i=(unsigned)(rargI());
}

void cpu::op_jez(){
	double a;
	checkError(stack.Peek(&a));
	if(a==0){
		op_jmp();
	}else{rargI();}
}

void cpu::op_jnz(){
	double a;
	checkError(stack.Peek(&a));
	if(a!=0){
		op_jmp();
	}else{rargI();}
}

void cpu::op_jlz(){
	double a;
	checkError(stack.Peek(&a));
	if(a<0){
		op_jmp();
	}else{rargI();}
}

void cpu::op_jgz(){
	double a;
	checkError(stack.Peek(&a));
	if(a>0){
		op_jmp();
	}else{rargI();}
}

void cpu::op_hlt(){
	sysexit();
}

void cpu::op_call(){
	checkError(callstack.Push(eip.d));
	op_jmp();
}

void cpu::op_ret(){
	checkError(callstack.Pop(&eip.d));
}
void cpu::op_nop(){
return;	
}
