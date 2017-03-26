/*
 * HashTable.cxx
 * 
 * Copyright 2017 NullByte4532 <nullbyte4532@nullbyte4532-Lenovo-G510>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
 



#include <iostream>
#include <malloc.h>
#include <cassert>
#include <string.h>
#include "HashTable.hxx"
unsigned int HashTable::hash(unsigned char *str){
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash%tsize_;
}
List::List(){
	len=0;
	return;
}
int List::getLength(){
	return len;
}
void List::Lfree(ListElem* elp){
	if(elp){
		Lfree(elp->next);
		free(elp);
	}
}
List::~List(){
	Lfree(rootPtr_);
}
void List::add(Tablelem* val){
	ListElem* tmp;
	
	if(!curPtr_){
		if(!rootPtr_){
			rootPtr_=(ListElem*)calloc(1, sizeof(ListElem));
			curPtr_=rootPtr_;
		}else{
			curPtr_=rootPtr_;
			tmp=(ListElem*)calloc(1, sizeof(ListElem));
			tmp->next=curPtr_->next;
			curPtr_->next=tmp;
			tmp->next->prev=tmp;
			tmp->prev=curPtr_;
			curPtr_=tmp;
		}
		
	}else{
		tmp=(ListElem*)calloc(1, sizeof(ListElem));
		tmp->next=curPtr_->next;
		curPtr_->next=tmp;
		tmp->next->prev=tmp;
		tmp->prev=curPtr_;
		curPtr_=tmp;	
	}
	curPtr_->val=*val;
	len+=1;
	printf("ADDED: len=%d\n", len);
}
Tablelem* List::get(){
	return &(curPtr_->val);
}
void List::del(){
	ListElem* tmp;
	if(curPtr_){
		if(curPtr_==rootPtr_){
			tmp=curPtr_->next;
			rootPtr_=tmp;
			free(curPtr_);
			curPtr_=tmp;
			tmp->prev=NULL;
			
		}else{
			curPtr_->prev->next=curPtr_->next;
			if(curPtr_->next) curPtr_->next->prev=curPtr_->prev;
			tmp=curPtr_->prev;
			free(curPtr_);
			curPtr_=tmp;
		}
		len--;
	}
}
int List::step(){
	if(curPtr_->next){
		curPtr_=curPtr_->next;
		return 1;
	}else{
		return 0;
	}
}
int List::back(){
	if(curPtr_->prev){
		curPtr_=curPtr_->prev;
		return 1;
	}else{
		return 0;
	}
}
Tablelem* List::getNext(){
	assert(step());
	return get();
}
void List::rewind(){
	curPtr_=rootPtr_;
}
HashTable::HashTable(int size){
	int i;
	tsize_=size;
	table_=(List**)calloc(size, sizeof(List*));
	for(i=0; i<size; i++){
		table_[i]=new List();
	}
}
void HashTable::add(char* key, int el){
	Tablelem tmp;
	unsigned int h;
	tmp.val=el;
	strcpy(tmp.key, key);
	h=hash((unsigned char*)key);
	table_[h]->add(&tmp);
}
void HashTable::del(char* key){
	List* l;
	int i;
	Tablelem* tmp;
	l=table_[hash((unsigned char*)key)];
	l->rewind();
	for (i=0; i<l->getLength(); i++){
		tmp=l->get();
		if(!strcmp(tmp->key, key)){
			l->del();
			return;
		}
		l->step();
	}
}
int HashTable::get(char* key){
	List* l;
	int i;
	Tablelem* tmp;
	unsigned int h;
	h=hash((unsigned char*)key);
	l=table_[h];
	l->rewind();
	for (i=0; i<l->getLength(); i++){
		tmp=l->get();
		if(!strcmp(tmp->key, key)){
			return tmp->val;
		}
		l->step();
	}
	return -1;
}
HashTable::~HashTable(){
	int i;
	for(i=0; i<tsize_; i++){
		free(table_[i]);
	}
	free(table_);
}


