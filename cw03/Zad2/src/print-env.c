#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv){

    if(argc != 2){
        printf("Wrong number of arguments\n");
        return 1;
    }

    char *value;
    if((value = getenv(argv[1])) != NULL){
        printf("Variable %s = %s\n",argv[1],value);
    }
    else{
        printf("There is no variable with name: %s\n",argv[1]);
        return 1;
    }

    return 0;
}