#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbook.h"



int cmpdates(Date *d1,Date *d2){ // earlier date = smaller date
    if(d1->year<d2->year) {
        return -1;
    }
    if(d1->year>d2->year) {
        return 1;
    }
    else{
        if(d1->month<d2->month) {
            return -1;
        }
        if(d1->month>d2->month) {
            return 1;
        }
        else{
            if(d1->day<d2->day) {
                return -1;
            }
            if(d1->day>d2->day) {
                return 1;
            }
            else {
                return 0;
            }
        }
    }
}
void print_Contact(Contact *contact){
    printf("\nName: %s %s\n",contact->name,contact->surname);
    printf("Birth date: %i-%i-%i\n",contact->b_date->day,contact->b_date->month,contact->b_date->year);
    printf("Email: %s\n",contact->email);
    printf("Phone: %s\n",contact->phone);
    printf("Address: %s\n",contact->address);
}


Contact *create_contact(char Name[], char Surname[], int Day, int Month, int Year, char Email[], char Phone[], char Address[]){

    Contact *new_contact = malloc(sizeof(struct Contact));
    Date *date = malloc(sizeof(struct Date));

    date->day = Day;
    date->month = Month;
    date->year = Year;

    new_contact->name = malloc((strlen(Name)+1)*sizeof(char));
    strcpy(new_contact->name,Name);

    new_contact->surname = malloc((strlen(Surname)+1)*sizeof(char));
    strcpy(new_contact->surname,Surname);

    new_contact->b_date = date;

    new_contact->email = malloc((strlen(Email)+1)*sizeof(char));
    strcpy(new_contact->email,Email);

    new_contact->phone = malloc((strlen(Phone)+1)*sizeof(char));
    strcpy(new_contact->phone,Phone);

    new_contact->address = malloc((strlen(Address)+1)*sizeof(char));
    strcpy(new_contact->address,Address);

    return new_contact;
}

List* create_list(){
    List *list = malloc(sizeof(struct List));
    list->first = NULL;
    list->last = NULL;
    list->number = 0;
    return list;
}


void add_listElem(List *list,ListElem *new_elem){

    if(list == NULL) {
        list = create_list();
    }

    if(list->last == NULL){
        list->last = new_elem;
        list->first = new_elem;
    }
    else{
        new_elem->prev = list->last;
        list->last->next = new_elem;
        list->last = new_elem;
    }
    list->number++;
}

void new_listElem(List *list, char Name[], char Surname[], int Day, int Month, int Year, char Email[], char Phone[], char Address[]){

    Contact *contact = create_contact(Name,Surname,Day,Month,Year,Email,Phone,Address);

    ListElem *new_elem = malloc(sizeof(struct ListElem));

    new_elem->prev = NULL;
    new_elem->next = NULL;
    new_elem->contact = contact;

    add_listElem(list,new_elem);
}


ListElem *remove_listElem(List *list, ListElem *listElem){

    if(list == NULL){
        printf("There is no book\n");
        return listElem;
    }

    if (list->first == NULL || list->last == NULL){
        printf("Book is empty\n");
        return listElem;
    }
    if (listElem == NULL){
        printf("Contact is empty\n");
        return listElem;
    }

    if (list->first == listElem){
        list->first = list->first->next;

        if (list->first != NULL) {
            list->first->prev = NULL;
        }
    }
    if (list->last == listElem){
        list->last = list->last->prev;

        if (list->last != NULL) {
            list->last->next = NULL;
        }
    }
    if (listElem != list->first && listElem != list->last) {
        if(listElem->next != NULL){
            listElem->next->prev = listElem->prev;
        }
        if(listElem->prev != NULL) {
            listElem->prev->next = listElem->next;
        }
    }
    listElem->next = NULL;
    listElem->prev = NULL;
    list->number--;
    return listElem;
}
void delete_listElem(List *list, ListElem *listElem){
    remove_listElem(list,listElem);
    free(listElem->contact->b_date);
    free(listElem->contact->name);
    free(listElem->contact->surname);
    free(listElem->contact->address);
    free(listElem->contact->email);
    free(listElem->contact->phone);
    free(listElem->contact);
    free(listElem);
}

void delete_list(List *list){
    ListElem *el = list->first;
    if(el != NULL){
        while (el->next != NULL){
            el = el->next;
            delete_listElem(list,el->prev);
        }
        delete_listElem(list,list->last);
    }
    free(list);
}


ListElem* find_listElem(List *list, char* Name, char* Surname, char* Email){
    ListElem *temp = NULL;
    if (list == NULL){
        printf("There is no book\n");
        return temp;
    }
    if (list->first == NULL || list->last == NULL){
        printf("Book is empty\n");
        return temp;
    }
    temp = list->first;
    while(temp != NULL){
        if(strcmp(temp->contact->name,Name)==0 && strcmp(temp->contact->surname,Surname)==0 && strcmp(temp->contact->email,Email)==0) {
            return temp;
        }
        temp = temp->next;
    }
    printf("There is no contact with these parameters\n");
    return temp;
}
ListElem *cmp_nodes(ListElem *el1, ListElem *el2, enum Field field){
    ListElem *min = el2;
    switch(field){
        case SURNAME:
            if(strcmp(el1->contact->surname,el2->contact->surname)<0) {
                min = el1;
            }
            else if(strcmp(el1->contact->surname,el2->contact->surname)==0){
                if(strcmp(el1->contact->name,el2->contact->name)<0) {
                    min = el1;
                }
            }
            break;
        case BIRTHDATE:
            if(cmpdates(el1->contact->b_date,el2->contact->b_date)<0) {
                min = el1;
            }
            break;
        case EMAIL:
            if(strcmp(el1->contact->email,el2->contact->email)<0) {
                min = el1;
            }
            break;
        case PHONE:
            if(strcmp(el1->contact->phone,el2->contact->phone)<0) {
                min = el1;
            }
            break;
    }
    return min;
}

List* sort_list_by_field(List *list, enum Field field){
    if(list == NULL || list->first == list->last) {
        return list;
    }

    List *sorted = create_list();
    ListElem *min_elem;
    ListElem *temp;
    while(list->first != NULL){
        temp = list->first;
        min_elem = list->first;
        while(temp != NULL){
            min_elem = cmp_nodes(min_elem,temp,field);
            temp = temp->next;
        }
        min_elem = remove_listElem(list, min_elem);
        add_listElem(sorted,min_elem);
    }
    free(list);
    return sorted;
}


void print_list(List *list){
    ListElem *temp;
    temp = list->first;
    while(temp != NULL){
        print_Contact(temp->contact);
        temp = temp->next;
    }

}



Tree* create_tree(){
    Tree *tree;
    tree = malloc(sizeof(Tree));
    tree->root = NULL;
    tree->sorted_by = SURNAME;
    return tree;
}

int cmp_leaves(Leaf *leaf1,Leaf *leaf2,enum Field field){
    int result = 2;    
    if(leaf1 == NULL || leaf2 == NULL){
        printf("Can't compare NULL\n");
        return result;
    }
    
    switch(field){
        case SURNAME:
            result = strcmp(leaf1->contact->surname,leaf2->contact->surname);
            if(result == 0) result = strcmp(leaf1->contact->name,leaf2->contact->name);
            break;
        case BIRTHDATE:
            result = cmpdates(leaf1->contact->b_date,leaf2->contact->b_date);
            break;
        case EMAIL:
            result = strcmp(leaf1->contact->email,leaf2->contact->email);
            break;
        case PHONE:
            result = strcmp(leaf1->contact->phone,leaf2->contact->phone);
            break;
    }
    return result;
}

void add_leaf(Tree *tree, Leaf *leaf){
    if(tree->root == NULL) {
        tree->root = leaf;
    }
    else {
        Leaf *temp = tree->root;
        Leaf *parent = NULL;
        while(temp != NULL){
            parent = temp;
            if(cmp_leaves(leaf,temp,tree->sorted_by)<0) temp = temp->left;
            else temp = temp->right;
        }
        leaf->parent = parent;
        if(cmp_leaves(leaf,parent,tree->sorted_by)<0) parent->left = leaf;
        else parent->right = leaf;
    }
}

Leaf *minimum(Tree *tree){
    Leaf *min = tree->root;
    while(min != NULL && min->left != NULL){
        min = min->left;
    }
    return min;
}


void new_leaf(Tree *tree, char Name[], char Surname[], int Day, int Month, int Year, char Email[], char Phone[], char Address[]){

    Contact *new_contact = create_contact(Name,Surname,Day,Month,Year,Email,Phone,Address);
    Leaf *leaf = malloc(sizeof(Leaf));
    leaf->contact = new_contact;
    leaf->left = NULL;
    leaf->right = NULL;
    leaf->parent = NULL;

    add_leaf(tree,leaf);
}

Leaf *successor_leaf(Leaf *leaf){
    if(leaf == NULL){
        printf("Nulls have no succesors\n");
    }
    Leaf *successor = leaf;

    if(successor->right != NULL){
        successor = successor->right;
        while(successor->left != NULL) {
            successor = successor->left;
        }
        return successor;
    }
    Leaf *y = successor->parent;
    while(y != NULL && successor == y->right){
        successor = y;
        y = y->parent;
    }
    return y;
}

void transplant(Tree *tree, Leaf *u, Leaf *v){
    if(u->parent == NULL)
        tree->root = v;
    else if(u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if(v != NULL)
        v->parent = u->parent;
}

Leaf *remove_leaf(Tree *tree, Leaf *leaf){

    if(tree->root == NULL || leaf == NULL){
        printf("Empty root or leaf pointer\n");
        return leaf;
    }

    if(leaf->left == NULL) transplant(tree,leaf,leaf->right);
    else if(leaf->right == NULL) transplant(tree,leaf,leaf->left);
    else {
        Leaf *y = successor_leaf(leaf);
        if(y->parent != leaf){
            transplant(tree,y,y->right);
            y->right = leaf->right;
            y->right->parent = y;
        }
        transplant(tree,leaf,y);
        y->left = leaf->left;
        y->left->parent = y;
    }

    leaf->left = NULL;
    leaf->right = NULL;
    leaf->parent = NULL;
    return leaf;
}

Tree*  rebuild_tree_by_field(Tree *tree, enum Field field){
    if(field != SURNAME && field != BIRTHDATE && field != PHONE && field != EMAIL){
        printf("Cannot sort by that field\n");
        return tree;
    }
    Tree *rebuilt = create_tree();
    rebuilt->sorted_by = field;
    Leaf *temp = minimum(tree);
    Leaf *next;
    while(temp != NULL){
        next = successor_leaf(temp);
        temp = remove_leaf(tree,temp);
        add_leaf(rebuilt,temp);
        temp = next;

    }
    free(tree);

    return rebuilt;
}

void delete_leaf(Tree *tree,Leaf *leaf){
    leaf = remove_leaf(tree,leaf);
    free(leaf->contact->phone);
    free(leaf->contact->name);
    free(leaf->contact->surname);
    free(leaf->contact->b_date);
    free(leaf->contact->email);
    free(leaf->contact->address);
    free(leaf->contact);
    free(leaf);
}


void delete_tree(Tree *tree){

    while(tree->root != NULL){
        delete_leaf(tree,tree->root);
    }
    free(tree);
}


Leaf* find_leaf(Tree *tree, char *Name, char *Surname, char *Email){
    if(tree->root == NULL) {
        printf("There are no elements in the tree\n");
        return tree->root;
    }
    Leaf *temp = minimum(tree);
    while(temp != NULL){
        if(strcmp(temp->contact->surname,Surname)==0 && strcmp(temp->contact->surname,Surname)==0 &&
           strcmp(temp->contact->surname,Surname)==0){
            return temp;
        }
        temp = successor_leaf(temp);
    }
    if(temp == NULL){
        printf("There is no contact with these parameters\n");

    }
    return temp;
}
void print_tree(Leaf *root){

    if(root == NULL) {
        return;
    }
    if(root->left == NULL && root->right == NULL){
        print_Contact(root->contact);
    }
    else {
        if (root->left != NULL) print_tree(root->left);
        if (root->right != NULL) print_tree(root->right);
        print_Contact(root->contact);
    }
}



