#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

extern char **environ;

int parse_line(char *line){

    printf("\nParsing line %s",line);
    if(line[0] == '#'){

        char *name = strsep(&line," \n");
        char *value = strsep(&line," \n");

        if(line == NULL || *line != 0){
            printf("Wrong definition of environment variable\n");
            return 1;
        }

        if(value == NULL){
            if(unsetenv(name) != 0){
                printf("Could not remove variable %s\n",name);
                return 1;
            }
        }
        else{
            name++;
            if(setenv(name,value,1) != 0){
                printf("Could not add/change variable %s",name);
                return 1;
            }
        }
        printf("Variable %s = %s\n",name,value);

    }
    else{
        char *argv[5];
        int i = 0;
        int flag = 0;
        while(i < 5 && flag == 0){
            argv[i] = strsep(&line," \n");
            if(*argv[i] == 0) argv[i] = NULL;
            if(argv[i] == NULL){
                flag = 1;
            }
            i++;
        }

        pid_t pid = fork();
        if(pid < 0){
            printf("Cannot fork\n");
            exit(1);
        }
        else if(pid == 0){ //potomek
            if(execvp(argv[0],argv) == -1){
                printf("Cannot execute command %s\n",argv[0]);
                exit(1);
            }

            exit(0);
        }
        else{
            int status;
            wait(&status);
            if(WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0){
                printf("Failure of child process\n");
                return 1;
            }


        }
    }

    return 0;
}

int parse_file(FILE *fp){

    char *line = NULL;
    size_t len = 0;
    int flag = 0;
    while(getline(&line,&len,fp) != -1 && flag == 0){
        flag = parse_line(line);
    }
    return flag;
}

int main(int argc, char **argv) {


    FILE *fp;

    if(argc != 2){
        printf("Wrong number of arguments\n");
        return 1;
    }

    fp = fopen(argv[1],"r");
    if(fp == NULL){
        printf("Cannot open file\n");
        return 1;
    }

    if(parse_file(fp) != 0){
        printf("Error while reading file\n");
    }


    fclose(fp);

    return 0;
}