#ifndef ZAD1_LIBRARY_H
#define ZAD1_LIBRARY_H

enum Field {SURNAME, BIRTHDATE, EMAIL, PHONE};

typedef struct Date{
    int day;
    int month;
    int year;

} Date;

typedef struct Contact{
    char *name;
    char *surname;
    struct Date *b_date;
    char *email;
    char *phone;
    char *address;

} Contact;

typedef struct ListElem{
    struct Contact *contact;

    struct ListElem *next;
    struct ListElem *prev;

} ListElem;

typedef struct List {
    ListElem *first;
    ListElem *last;
    enum Field sorted_by;
    int number;

} List;

typedef struct Leaf{
    struct Contact *contact;

    struct Leaf *parent;
    struct Leaf *left;
    struct Leaf *right;

} Leaf;

typedef struct Tree{
    struct Leaf *root;
    enum Field sorted_by;
} Tree;

List* create_list(void);

void delete_list(List* list);

void new_listElem(List* list, char* Name, char* Surname, int Day, int Month, int Year, char* Email, char* Phone, char* Address);

void delete_listElem(List* list, ListElem* listElem);

ListElem* find_listElem(List* list, char* Name, char* Surname, char* Email);

List* sort_list_by_field(List* list, enum Field field);

void print_list(List* list);


Tree* create_tree(void);

void delete_tree(Tree* tree);

void new_leaf(Tree* tree, char* Name, char* Surname, int Day, int Month, int Year, char* Email, char* Phone, char* Address);

void delete_leaf(Tree *tree,Leaf* leaf);

Leaf* minimum(Tree* tree);

Tree *rebuild_tree_by_field(Tree* tree, enum Field field);

Leaf* find_leaf(Tree* tree, char* Name, char* Surname, char* Email);

void print_tree(Leaf* leaf);


#endif
