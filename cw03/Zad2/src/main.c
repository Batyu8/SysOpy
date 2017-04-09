#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/time.h>
#include <sys/resource.h>


int parse_line(char *line,int sec_limit,int mb_limit){

    printf("\nParsing line %s",line);
    if(line[0] == '#'){

        char *name = strsep(&line," \n");
        char *value = strsep(&line," \n");

        if(line == NULL || *line != 0){
            printf("Wrong definition of environment variable\n");
            return 1;
        }

        if(value == NULL){
            name++;
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
            if(argv[i] == NULL){
                flag = 1;
            }
            else if(*argv[i] == 0) {
                argv[i] = NULL;
                flag = 1;
            }

            i++;
        }

        struct rusage usage;
        if(getrusage(RUSAGE_CHILDREN,&usage) != 0){
             printf("Cannot read system and user time\n");
        }
        long stime = -(usage.ru_stime.tv_sec*1000000 + usage.ru_stime.tv_usec);
        long utime = -(usage.ru_utime.tv_sec*1000000 + usage.ru_utime.tv_usec);
        pid_t pid = fork();
        if(pid < 0){
            printf("Cannot fork\n");
            exit(1);
        }
        else if(pid == 0){ //potomek
            struct rlimit limit;
            limit.rlim_max = mb_limit*1048576;
            limit.rlim_cur = limit.rlim_max;
            if(setrlimit(RLIMIT_AS,&limit) != 0){
                printf("Cannot set limit for virtual memory\n");
                exit(1);
            }
            limit.rlim_max = sec_limit;
            limit.rlim_cur = limit.rlim_max;
            if(setrlimit(RLIMIT_CPU,&limit) != 0){
                printf("Cannot set limit for cpu time\n");
            }

            if(execvp(argv[0],argv) == -1){
                printf("Cannot execute command %s\n",argv[0]);
                exit(1);
            }

            exit(0);
        }
        else{
            int status;
            wait(&status);
            if(getrusage(RUSAGE_CHILDREN,&usage) != 0){
                printf("Cannot read system and user time\n");
            }
            stime += usage.ru_stime.tv_sec*1000000 + usage.ru_stime.tv_usec;
            utime += usage.ru_utime.tv_sec*1000000 + usage.ru_utime.tv_usec;
            printf("System time: %li\n",stime);
            printf("User time: %li\n",utime);
            if(WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0){
                printf("Failure of child process\n");
                return 1;
            }

        }
    }

    return 0;
}

int parse_file(FILE *fp, int sec_limit, int mb_limit){

    char *line = NULL;
    size_t len = 0;
    int flag = 0;
    while(getline(&line,&len,fp) != -1 && flag == 0){
        flag = parse_line(line,sec_limit,mb_limit);
    }
    return flag;
}

int main(int argc, char **argv) {


    FILE *fp;

    if(argc != 4){
        printf("Wrong number of arguments\n");
        return 1;
    }

    int sec_limit;
    if(sscanf(argv[2],"%i",&sec_limit) == EOF){
        printf("Wrong format of seconds limit argument\n");
        return 1;
    }
    int mb_limit;
    if(sscanf(argv[3],"%i",&mb_limit) == EOF){
        printf("Wrong format of megabytes limit argument\n");
        return 1;
    }

    fp = fopen(argv[1],"r");
    if(fp == NULL){
        printf("Cannot open file\n");
        return 1;
    }

    if(parse_file(fp,sec_limit,mb_limit) != 0){
        printf("Error while reading file\n");
        fclose(fp);
        return 1;
    }


    fclose(fp);

    return 0;
}