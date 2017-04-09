#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <dlfcn.h>
#include "cbook.h"



char *Names[] = { "Wesley","Hayden","Colorado","Magee","Holmes",
                  "Ignatius","Lester","Lars","Nissim","Emerson",
                  "Emeron","Kieran","Gray","Maxwell","Walter",
                  "Lamar","Shad","Kamal","Colt","Jerome",
                  "Colorada","Ciaran","Jeremy","Todd","Jerom",};

char *Surnames[] = {"Horn", "Sears", "Schultz", "Winters", "Lester",
                    "Head", "Hebert", "Snyder", "Hammond", "Grimes",
                    "Emerson","Austin","Russell","Warner","Chan",
                    "Bartlett","Greene","Murray","Nash","Mayer",
                    "Clemons","Compton","Chan","Glenn","Beach",
                    "Wheeler","Lee","Reyes","Wilkinson","Torres",
                    "Walton","Mayer","Graves","Schneider","Boyle",
                    "Brady","Skinner","Allen","Simmons","Morrison",};

char *Addresses[] = {"Albi, Ap #433-9545 Lacus. Road", "Viesville|P.O. Box 958, 9725 Egestas. Av.",
                     "Panjim|P.O. Box 667, 2499 Neque Street", "Vezirköprü|Ap #331-4923 Elementum, Avenue",
                     "Drogenbos|Ap #400-6345 Rhoncus. Ave", "Ostra Vetere|2943 Bibendum Avenue", "ThimŽon|8766 Donec Rd.",
                     "Lairg|474-4546 Dui, St.", "Lancaster|102-5237 Ipsum Rd.", "Meeuwen-Gruitrode|807-5966 Odio. Ave",};

void gen_data(Tree *tree, List *list, int i, int j, int option){
    srand(0);
    char * domena = "@gmail.com";
    int day;
    int month;
    int year;
    char *email;
    char *address;
    char *phone;
    void (*new_leaf)(Tree* , char* , char* , int , int , int , char* , char* , char*);
    void (*new_listElem)(List* , char* , char* , int , int , int , char* , char* , char*);;
    void *handle;
    handle = dlopen("../build/libcontactbook_sh.so",RTLD_LAZY);

    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return ;
    }

    email = malloc((strlen(Names[i]) + strlen(Surnames[j]) + strlen(domena)+ 1)*sizeof(char));
    strcpy(email,Names[i]);
    strcat(email,Surnames[j]);
    strcat(email,domena);

    day = rand() % 28 + 1;
    month = rand() % 12 + 1;
    year = rand() % 70 + 1947;
    address = Addresses[rand() %10];
    phone = malloc(10*sizeof(char));
    long n =(rand()%9+1)*100000000 + (rand()%9+1)*10000000 + (rand()%9+1)*1000000 +
            (rand()%9+1)*100000 + (rand()%9+1)*10000 + (rand()%9+1)*1000 +
            (rand()%9+1)*100 + (rand()%9+1)*10 + rand()%9+1;
    snprintf(phone,10,"%li",n);
    if(option == 0){
        new_leaf = (void (*)(Tree* , char* , char* , int , int , int , char* , char* , char*)) dlsym(handle,"new_leaf");
        (*new_leaf)(tree,Names[i],Surnames[j],day,month,year,email,phone,address);
    }
    else if(option == 1){
        new_listElem = (void (*)(List* , char* , char* , int , int , int , char* , char* , char*)) dlsym(handle,"new_listElem");
        (*new_listElem)(list,Names[i],Surnames[j],day,month,year,email,phone,address);
    }
    else if(option == 2){
        new_leaf = (void (*)(Tree* , char* , char* , int , int , int , char* , char* , char*)) dlsym(handle,"new_leaf");
        (*new_leaf)(tree,"A","A",day,month,year,"AA@gmail.com",phone,address);
    }
    else if(option == 3){
        new_leaf = (void (*)(Tree* , char* , char* , int , int , int , char* , char* , char*)) dlsym(handle,"new_leaf");
        (*new_leaf)(tree,"B","B",day,month,year,"BB@gmail.com",phone,address);
    }
    free(phone);
    free(email);

    dlclose(handle);


}

void test_with_time(List **list, Tree **tree, struct rusage* sutime,struct timeval* r_time, int i, int j, int testnr){

    FILE *f = fopen("results.txt", "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    List* (*create_list)();
    List* (*sort_list_by_field)(List*,enum Field);
    ListElem* (*find_listElem)(List*,char*,char*,char*);
    void (*delete_listElem)(List*,ListElem*);

    Tree* (*create_tree)();
    Tree* (*rebuild_tree_by_field)(Tree*,enum Field);
    Leaf* (*find_leaf)(Tree*,char*,char*,char*);
    void (*delete_leaf)(Tree*,Leaf*);
    void (*delete_tree)(Tree*);

    void *handle;
    handle = dlopen("../build/libcontactbook_sh.so",RTLD_LAZY);

    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return ;
    }
    char* test_name;
    ListElem *listElem;
    Leaf *leaf = NULL;
    Tree *temp_tree = NULL;
    if(testnr == 3){
        leaf = (*tree)->root;
    }
    if(testnr == 4 || testnr == 6){

        create_tree = (Tree* (*)()) dlsym(handle, "create_tree");
        temp_tree = (*create_tree)();

        for(int n=0;n<999;n++){
            gen_data(temp_tree,*list,0,0,2);
        }
        gen_data(temp_tree,*list,0,0,3);
    }



    int err = getrusage(RUSAGE_SELF,sutime);
    if(err != 0){
        printf("Error nr %i",errno);
        return;
    }
    err = gettimeofday(r_time,NULL);
    if(err != 0){
        printf("Error nr %i",errno);
        return;
    }
    double stime = -(sutime->ru_stime.tv_sec*1000000.0 + sutime->ru_stime.tv_usec);//microseconds
    double utime = -(sutime->ru_utime.tv_sec*1000000.0 + sutime->ru_utime.tv_usec);
    double rtime = -(r_time->tv_sec*1000000.0 + r_time->tv_usec);

    switch(testnr){
        case 0:
            create_list = (List* (*)()) dlsym(handle, "create_list");
            *list = (*create_list)();
            test_name = "Test: Creating list";
            break;
        case 1:
            gen_data(*tree,*list,i,j,0);
            test_name = "Test: Adding element to list";
            break;
        case 2:

            sort_list_by_field = (List* (*)(List*,enum Field)) dlsym(handle, "sort_list_by_field");
            *list = (*sort_list_by_field)(*list,EMAIL);
            test_name = "Test: Sorting list by email";
            break;
        case 3:
            find_listElem = (ListElem* (*)(List*,char*,char*,char*)) dlsym(handle, "find_listElem");
            listElem = (*find_listElem)(*list,"Ciaran","Allen","CiaranAllen@gmail.com");
            test_name = "Test: Optimistic search of element in list";
            break;
        case 4:
            find_listElem = (ListElem* (*)(List*,char*,char*,char*)) dlsym(handle, "find_listElem");
            listElem = (*find_listElem)(*list,"Wesley","Winters","WesleyWinters@gmail.com");
            test_name = "Test: Pesimistic search of element in list";
            break;
        case 5:
            test_name = "Test: Optimistic removal of element in list";
            find_listElem = (ListElem* (*)(List*,char*,char*,char*)) dlsym(handle, "find_listElem");
            listElem = (*find_listElem)(*list,"Ciaran","Allen","CiaranAllen@gmail.com");
            delete_listElem = (void (*)(List*,ListElem*)) dlsym(handle, "delete_listElem");
            (*delete_listElem)(*list,listElem);
            break;
        case 6:
            test_name = "Test: Pesimistic removal of element in list";
            find_listElem = (ListElem* (*)(List*,char*,char*,char*)) dlsym(handle, "find_listElem");
            listElem = (*find_listElem)(*list,"Wesley","Winters","WesleyWinters@gmail.com");
            delete_listElem = (void (*)(List*,ListElem*)) dlsym(handle, "delete_listElem");
            (*delete_listElem)(*list,listElem);
            break;
    }


    err = getrusage(RUSAGE_SELF,sutime);
    if(err != 0){
        printf("Error nr %i",errno);
        return;
    }
    err = gettimeofday(r_time,NULL);
    if(err != 0){
        printf("Error nr %i",errno);
        return;
    }
    stime += (sutime->ru_stime.tv_sec*1000000.0 + sutime->ru_stime.tv_usec);
    utime += (sutime->ru_utime.tv_sec*1000000.0 + sutime->ru_utime.tv_usec);
    rtime += (r_time->tv_sec*1000000.0 + r_time->tv_usec);

    fprintf(f,"\n\n%s\n",test_name);
    fprintf(f,"System time: %.1f microseconds\n",stime);
    fprintf(f,"User time: %.1f microseconds\n",utime);
    fprintf(f,"Real time: %.1f microseconds\n",rtime);


    err = getrusage(RUSAGE_SELF,sutime);
    if(err != 0){
        printf("Error nr %i",errno);
        return;
    }
    err = gettimeofday(r_time,NULL);
    if(err != 0){
        printf("Error nr %i",errno);
        return;
    }
    stime = -(sutime->ru_stime.tv_sec*1000000.0 + sutime->ru_stime.tv_usec);//microseconds
    utime = -(sutime->ru_utime.tv_sec*1000000.0 + sutime->ru_utime.tv_usec);
    rtime = -(r_time->tv_sec*1000000.0 + r_time->tv_usec);

    switch(testnr){
        case 0:
            create_tree = (Tree* (*)()) dlsym(handle, "create_tree");
            *tree = (*create_tree)();
            test_name = "Test: Creating tree";
            break;
        case 1:
            gen_data(*tree,*list,i,j,1);
            test_name = "Adding element to tree";
            break;
        case 2:
            rebuild_tree_by_field = (Tree* (*)(Tree*,enum Field)) dlsym(handle,"rebuild_tree_by_field");
            *tree = rebuild_tree_by_field(*tree,EMAIL);
            test_name = "Sorting tree by email";
            break;
        case 3:
            test_name = "Test: Optimistic search of element in tree";
            find_leaf = (Leaf* (*)(Tree*,char*,char*,char*)) dlsym(handle,"find_leaf");
            leaf = (*find_leaf)(*tree,leaf->contact->name,leaf->contact->surname,leaf->contact->email);
            break;
        case 4:
            test_name = "Test: Pesimistic search of element in tree";
            find_leaf = (Leaf* (*)(Tree*,char*,char*,char*)) dlsym(handle,"find_leaf");
            leaf = (*find_leaf)(temp_tree,"B","B","BB@gmail.com");
            break;
        case 5:
            test_name = "Test: Optimistic removal of element in tree";
            delete_leaf = (void (*)(Tree*,Leaf*)) dlsym(handle,"delete_leaf");
            delete_leaf(*tree,(*tree)->root);
            break;
        case 6:
            test_name = "Test: Pesimistic removal of element in tree";
            find_leaf = (Leaf* (*)(Tree*,char*,char*,char*)) dlsym(handle,"find_leaf");
            leaf = find_leaf(temp_tree,"B","B","BB@gmail.com");
            delete_leaf = (void (*)(Tree*,Leaf*)) dlsym(handle,"delete_leaf");
            delete_leaf(*tree,leaf);
            break;
    }

    err = getrusage(RUSAGE_SELF,sutime);
    if(err != 0){
        printf("Error nr %i",errno);
        return;
    }
    err = gettimeofday(r_time,NULL);
    if(err != 0){
        printf("Error nr %i",errno);
        return;
    }
    stime += (sutime->ru_stime.tv_sec*1000000.0 + sutime->ru_stime.tv_usec);
    utime += (sutime->ru_utime.tv_sec*1000000.0 + sutime->ru_utime.tv_usec);
    rtime += (r_time->tv_sec*1000000.0 + r_time->tv_usec);

    fprintf(f,"\n\n%s\n",test_name);
    fprintf(f,"System time: %.1f microseconds\n",stime);
    fprintf(f,"User time: %.1f microseconds\n",utime);
    fprintf(f,"Real time: %.1f microseconds\n",rtime);

    if(testnr == 4 || testnr == 6){
        delete_tree = (void (*)(Tree*)) dlsym(handle,"delete_tree");
        delete_tree(temp_tree);
    }

    dlclose(handle);
    fclose(f);
}


void test(){

    void *handle;
    handle = dlopen("../build/libcontactbook_sh.so",RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return ;

    }

       List *list;
       Tree *tree;

       struct rusage sutime;
       struct timeval rtime;

       for(int n=0;n<7;n++){
           if(n==1){
               for(int i=0;i<25;i++){
                   for(int j=0;j<40;j++){
                       test_with_time(&list,&tree,&sutime,&rtime,i,j,n);
                   }
               }
           }
           else{
               test_with_time(&list,&tree,&sutime,&rtime,0,0,n);
           }
       }



    void (*delete_list)(List*);
    delete_list = (void (*)(List*)) dlsym(handle, "delete_list");
    (*delete_list)(list);
    void (*delete_tree)(Tree*);
    delete_tree = (void (*)(Tree*)) dlsym(handle, "delete_tree");
    (*delete_tree)(tree);

    dlclose(handle);
}

int main(){

    test();


    return 0;
}