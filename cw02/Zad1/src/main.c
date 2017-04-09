#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

typedef struct Options{
    char* operation;
    char* system;
    char* filename;
    long record_n;
    long r_size;
} Options;


int parse_arg(char* arg, int argc, int number, Options* options){

    long records = 0;
    long size = 0;
    if(argc == 5){
        switch(number){
            case 1:
                if(strcmp(arg,"generate")==0) {
                    options->operation = "generate";
                }
                else {
                    printf("Wrong operation\n");
                    return -1;
                }
                break;
            case 2:
                options->filename = arg;
                break;
            case 3:
                records = strtol(arg,NULL,10);
                if(records == 0 || records == LONG_MAX || records == LONG_MIN){
                    printf("Wrong number of records\n");
                    return -1;
                }
                else {
                    options->record_n = records;
                }
                break;
            case 4:
                size = strtol(arg,NULL,10);
                if(size == 0 || size == LONG_MAX || size == LONG_MIN){
                    printf("Wrong record size\n");
                    return -1;
                }
                else {
                    options->r_size = size;
                }
                break;
        }
    }
    else {
        switch(number){
            case 1:
                if(strcmp(arg,"shuffle")==0) {
                    options->operation = "shuffle";
                }
                else if(strcmp(arg,"sort")== 0){
                    options->operation = "sort";
                }
                else {
                    printf("Wrong operation\n");
                    return -1;
                }

                break;
            case 2:
                if(strcmp(arg,"sys") == 0){
                    options->system = "sys";
                }
                else if(strcmp(arg,"lib") == 0){
                    options->system = "lib";
                }
                else{
                    printf("Wrong system\n");
                    return -1;
                }
                break;
            case 3:
                options->filename = arg;
                break;
            case 4:
                records = strtol(arg,NULL,10);
                if(records == 0 || records == LONG_MAX || records == LONG_MIN){
                    printf("Wrong number of records\n");
                    return -1;
                }
                else {
                    options->record_n = records;
                }
                break;
            case 5:
                size = strtol(arg,NULL,10);
                if(size == 0 || size == LONG_MAX || size == LONG_MIN){
                    printf("Wrong record size\n");
                    return -1;
                }
                else {
                    options->r_size = size;
                }
                break;
        }
    }
    return 0;
}


Options* parse_args(int argc, char** argv, Options* options){

    if(argc != 5 && argc != 6){
        printf("Wrong number of parameters\n");
        return NULL;
    }
    options = malloc(sizeof(Options));
    for(int i=1;i<argc;i++){
        if(parse_arg(argv[i],argc,i,options) != 0){
            free(options);
            return NULL;
        }
    }
    return options;
}
void free_buffs(char *buff1, char *buff2){
    free(buff1);
    free(buff2);
}


int generate(Options *options){
    int source = open("/dev/urandom",O_RDONLY);
    int dest = open(options->filename,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);
    if(source < 0){
        printf("Opening source error\n");
        return -1;
    }
    if(dest < 0){
        printf("Opening destination error\n");
        close(dest);
        return -1;
    }
    char *buffer = malloc(options->r_size*sizeof(char));
    for(int i=0;i<options->record_n;i++){

        int j = read(source,buffer,options->r_size);

        if(j == -1){
            printf("Reading source error\n");
            free(buffer);
            return -1;
        }

        j = write(dest,buffer,options->r_size);
        if(j == -1){
            printf("Writing into destination error\n");
            free(buffer);
            return -1;
        }
    }

    free(buffer);
    return 0;
}



int shuffle_sys(Options *options){
    
    int source = open(options->filename,O_RDWR);
    if(source < 0){
        printf("Opening source error\n");

        return -1;
    }
    
    srand(time(NULL));
    int size = options->r_size;
    int records = options->record_n;
    char* buff1 = malloc(size*sizeof(char));
    char* buff2 = malloc(size*sizeof(char));
    int record;

    for(int i=0;i<records;i++){
        record = rand()%(records-i);
        if(record != records-1-i){
            if(lseek(source,record*size,SEEK_SET) < 0){
                free_buffs(buff1,buff2);
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(read(source,buff1,size) < 0){
                free_buffs(buff1,buff2);
                printf("Cannot read file \n");
                return -1;
            }
            if(lseek(source,(records-1-i)*size,SEEK_SET) < 0){
                free_buffs(buff1,buff2);
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(read(source,buff2,size) < 0){
                free_buffs(buff1,buff2);
                printf("Cannot read file \n");
                return -1;
            }
            if(lseek(source,record*size,SEEK_SET) < 0){
                free_buffs(buff1,buff2);
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(write(source,buff2,size) < 0){
                free_buffs(buff1,buff2);
                printf("Cannot write in file \n");
                return -1;
            }
            if(lseek(source,(records-1-i)*size,SEEK_SET) < 0){
                free_buffs(buff1,buff2);
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(write(source,buff1,size) < 0){
                free_buffs(buff1,buff2);
                printf("Cannot write in file \n");
                return -1;
            }
        }

    }

    free_buffs(buff1,buff2);
    return 0;

}

int shuffle_lib(Options *options){

    FILE *f = fopen(options->filename,"r+");

    if(f == NULL){
        printf("Error opening file\n");
        return -1;
    }

    int size = options->r_size;
    int records = options->record_n;

    char *buff1 = malloc(size*sizeof(char));
    char *buff2 = malloc(size*sizeof(char));
    int record;
    srand(time(NULL));

    for(int i=0;i<records;i++) {
        record = rand() % (records - i);
        if (record != records - 1 - i) {
            if(fseek(f,record*size,SEEK_SET) < 0){
                free_buffs(buff1,buff2);
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(fread(buff1,sizeof(char),size,f) != size){
                free_buffs(buff1,buff2);
                printf("Cannot read from file\n");
                return -1;
            }
            if(fseek(f,(records-1-i)*size,SEEK_SET) < 0){
                free_buffs(buff1,buff2);
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(fread(buff2,sizeof(char),size,f) != size){
                free_buffs(buff1,buff2);
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(fseek(f,(records-1-i)*size,SEEK_SET) < 0){
                free_buffs(buff1,buff2);
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(fwrite(buff1,sizeof(char),size,f) != size){
                free_buffs(buff1,buff2);
                printf("Cannot write into file");
                return -1;
            }
            if(fseek(f,record*size,SEEK_SET) < 0){
                free_buffs(buff1,buff2);
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(fwrite(buff2,sizeof(char),size,f) != size){
                free_buffs(buff1,buff2);
                printf("Cannot write into file");
                return -1;
            }


        }
    }

    free_buffs(buff1,buff2);
    return 0;
}


int shuffle(Options *options){


    if(strcmp(options->system,"sys") == 0){
        if(shuffle_sys(options) < 0){
            printf("System libraries shuffling error\n");
            return -1;
        }
        else{
            return 0;
        }
    }
    else if(strcmp(options->system,"lib") == 0){
        if(shuffle_lib(options) < 0){
            printf("C libraries shuffling error\n");
            return -1;
        }
        else {
            return 0;
        }
    }
    printf("Wrong system chosen\n");
    return -1;

}
int cmprec(char* rec1, char* rec2, int size){

    int i = 0;
    while(i < size && rec1[i] == rec2[i]){
        i++;
    }
    if(i == size){
        return 0;
    }
    else{
        if(rec1[i] < rec2[i]){
            return -1;
        }
        else {
            return 1;
        }
    }
}


int sort_lib(Options *options, FILE *f, char *buff1, char *buff2){

    int size = options->r_size;
    int records = options->record_n;

    int i = 0;
    int swaps = 1;

    while(i < records - 1 && swaps != 0){

        swaps = 0;

        for(int j=0;j<records-1-i;j++){

            if(fseek(f,j*size,SEEK_SET) < 0){
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(fread(buff1,sizeof(char),size,f) != size){
                printf("Cannot read file \n");
                return -1;
            }
            if(fread(buff2,sizeof(char),size,f) != size){
                printf("Cannot read file \n");
                return -1;
            }
            if(cmprec(buff1,buff2,size) > 0){
                char* temp = buff1;
                buff1 = buff2;
                buff2 = temp;
                swaps++;
            }
            if(fseek(f,j*size,SEEK_SET) < 0){
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(fwrite(buff1,sizeof(char),size,f) != size){
                printf("Cannot write to file \n");
                return -1;
            }
            if(fwrite(buff2,sizeof(char),size,f) != size){
                printf("Cannot write to file \n");
                return -1;
            }
        }

        i++;
    }
    return 0;

}
int sort_sys(Options *options, int source, char *buff1, char *buff2){

    int size = options->r_size;
    int records = options->record_n;
    int i = 0;
    int swaps = 1;

    while(i < records - 1 && swaps != 0){

        swaps = 0;

        for(int j=0;j<records-1-i;j++){

            if(lseek(source,j*size,SEEK_SET) < 0){
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(read(source,buff1,size) < 0){
                printf("Cannot read file \n");
                return -1;
            }
            if(read(source,buff2,size) < 0){
                printf("Cannot read file \n");
                return -1;
            }
            if(cmprec(buff1,buff2,size) > 0){
                char* temp = buff1;
                buff1 = buff2;
                buff2 = temp;
                swaps++;
            }
            if(lseek(source,j*size,SEEK_SET) < 0){
                printf("Cannot change position of pointer in file\n");
                return -1;
            }
            if(write(source,buff1,size) < 0){
                printf("Cannot write to file \n");
                return -1;
            }
            if(write(source,buff2,size) < 0){
                printf("Cannot write to file \n");
                return -1;
            }
        }

        i++;
    }

    return 0;
}

int sort(Options *options){

    char* buff1 = malloc(options->r_size*sizeof(char));
    char* buff2 = malloc(options->r_size*sizeof(char));

    if(strcmp(options->system,"sys") == 0){

        int source = open(options->filename,O_RDWR);
        if(source < 0){
            printf("Opening source error\n");
            return -1;
        }

        if(sort_sys(options, source, buff1,buff2) < 0){
            free_buffs(buff1,buff2);
            printf("Sorting error\n");
            return -1;
        }
    }
    else if(strcmp(options->system,"lib") == 0){

        FILE *f = fopen(options->filename,"r+");

        if(f == NULL){
            printf("Error opening file\n");
            return -1;
        }

        if(sort_lib(options, f, buff1, buff2) < 0){
            free_buffs(buff1,buff2);
            printf("Sorting error\n");
            return -1;
        }
    }

    free_buffs(buff1,buff2);
    return 0;
}

int operate(Options *options){


    if(strcmp(options->operation,"generate") == 0){
        if(generate(options) < 0){
            printf("Generate error\n");
            return -1;
        }
    }
    else if(strcmp(options->operation,"shuffle") == 0){
        if (shuffle(options) < 0){
            printf("Shuffle error\n");
            return -1;
        }
    }
    else{
        if(sort(options) < 0){
            printf("Sort error\n");
            return -1;
        }
    }
    return 0;
}

int main(int argc, char** argv) {

    Options *options = parse_args(argc,argv,options);
    if(options == NULL){
        printf("Error while parsing command line parameters\n");
        return -1;
    }

    printf("\nOperation: %s\n",options->operation);
    printf("System: %s\n",options->system);
    printf("Filename: %s\n",options->filename);
    printf("Number of records: %li\n",options->record_n);
    printf("Record size: %li\n",options->r_size);

    if(operate(options) < 0){
        return -1;
    }

    free(options);

    return 0;
}