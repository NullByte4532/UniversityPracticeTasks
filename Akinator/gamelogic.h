#ifndef GAMELOGIC
#define GAMELOGIC
#include <cstdio>
#include <malloc.h>
#include <cstring>

enum answer {YES, NO};
struct node {char* question; node* yes; node* no;};
class Tree{
    public:
    Tree();
    ~Tree();
    void add(char* question, answer current, char* new_name);
    char* get();
    char step(answer direction);
    void rewind();
    void load(FILE* f);
    void save(FILE* f);
    private:
    node* root;
    node* cur;
    node** prev;
    void del(node* ptr);
    void save_(FILE*f, node* ptr);
    node* load_(FILE* f);
};
void readq(FILE* f, char* ptr, int n);

#endif // GAMELOGIC

