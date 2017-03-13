#ifndef __hashtable_H_INCLUDED__ 
#define __hashtable_H_INCLUDED__

struct Tablelem{
	int val;
	char key[32];
};
struct ListElem{
	Tablelem val;
	ListElem* next;
	ListElem* prev;
};

class List{
	public:
		Tablelem* get();
		Tablelem* getNext();
		int step();
		int back();
		void rewind();
		void add(Tablelem* val);
		void del();
		List();
		~List();
		int getLength();
	private:
		void Lfree(ListElem*);
		ListElem* rootPtr_;
		ListElem* curPtr_;
		int len;
	
};
class HashTable{
	public:
		void add(char* key, int el);
		int get(char* key);
		void del(char* key);
		HashTable(int size);
		~HashTable();
	private:
		List** table_;
		int tsize_;
		unsigned int hash(unsigned char *str);
};



#endif 
