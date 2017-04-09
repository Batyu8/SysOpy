#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void print_menu(){
    printf("\n1. Ustawienie rygla do odczytu na wybrany znak pliku\n");
    printf("2. Ustawienie rygla do zapisu na wybrany znak pliku\n");
    printf("3. Wyświetlenie listy zaryglowanych znaków pliku\n");
    printf("4. Zwolnienie wybranego rygla\n");
    printf("5. Odczyt wybranego znaku pliku\n");
    printf("6. Zmiana wybranego znaku pliku\n");
    printf("0. Wyjście\n\n");


}

int lock_reg(int des, int cmd, int type, off_t offset, int whence, off_t len){
    struct flock lock;
    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;

    return(fcntl(des,cmd,&lock));
}

int lock_check(int des, struct flock *lock) {

    if (fcntl(des, F_GETLK,lock) < 0) {
        printf("Cannot check if byte is locked\n");
        return -1;
    }
    if (lock->l_type == F_UNLCK){
        return (0);
    }
    return 1;
  }

void go(char *filename) {
    off_t len = 1;
    int whence = SEEK_SET;
    int type;
    int cmd;
    int option = 0;
    off_t index;

    int line_size = 0;
    char *word;
    int des = open(filename,O_RDWR);

    while (option != '0') {
        print_menu();
        option = getchar();
        printf("\n");

        if (option > '0' && option < '7' && option != '3') {

            int flag = 1;
            while (flag != 0) {
                if (option == '4') {
                    word = "unlock";
                } else if (option == '5') {
                    word = "read";
                } else if (option == '6') {
                    word = "write";
                } else {
                    word = "lock";
                }
                printf("Enter index of byte you want to %s: ", word);
                line_size = scanf("%ld", &index);
                printf("\n");
                if (line_size != EOF && line_size != 0) {
                    flag = 0;

                } else {
                    printf("Wrong argument\n");
                    line_size = scanf("%*s");
                }

            }
            if (option == '4' || option == '1' || option == '2') {

                if (option != '4') {
                    char *line = malloc(sizeof(char)*9);
                    flag = 1;
                    while (flag != 0) {
                        printf("Do you want to wait until you can lock the file? [y/n] ");
                        line_size = scanf("%s", line);
                        printf("\n");
                        if (line_size == 1 && (strcmp(line, "y") == 0 || strcmp(line, "n") == 0)) {
                            flag = 0;
                        } else {
                            printf("Wrong argument\n");
                        }
                    }
                    if (strcmp(line, "y") == 0) {
                        cmd = F_SETLKW;
                    } else {
                        cmd = F_SETLK;
                    }
                    if (option == '1') {
                        type = F_RDLCK;
                    } else {
                        type = F_WRLCK;
                    }

                    free(line);

                }
                else {
                    type = F_UNLCK;
                    cmd = F_SETLK;
                }

                int result = lock_reg(des, cmd, type, index, whence, len);
                if (result != 0) {
                    printf("Could not %s byte\n", word);
                }
                else{
                    printf("Successful %s of byte nr: %ld\n",word,index);
                }

            }
            if (option == '5' || option == '6') {
                char *character = malloc(sizeof(char));

                if(lseek(des,index,whence) != index){
                    printf("Cannot change pointer in file\n");
                }
                else {
                    if (option == '5') {
                        if (read(des, character, 1) != 1) {
                            printf("Cannot read byte nr %li \n", index);
                        } else {
                            printf("Byte nr %li is %s", index, character);
                        }
                    }
                    else {
                        flag = 1;
                        while (flag != 0) {
                            printf("Enter char to %s \n",word);
                            line_size = scanf("%s", character);
                            printf("\n");
                            if (line_size == 1) {
                                flag = 0;
                            }
                            else {
                                printf("Wrong argument\n");
                            }
                        }
                        if (write(des, character, 1) != 1) {
                            printf("Cannot write byte nr %li\n", index);
                        } else {
                            printf("Wrote %s in byte nr %li ", character, index);
                        }

                    }
                }
                free(character);
            }

        }
        else if (option == '3') {
            struct flock *lock = malloc(sizeof(struct flock));
            index = 0;
            struct stat stats;
            if(fstat(des,&stats) < 0){
                printf("Cannot read file \n");
            }
            else{
                off_t size = stats.st_size;
                printf("Size of file: %li\n",size);
                int i = 0;
                int result;
                while(i < size){
                    lock->l_whence = whence;
                    lock->l_len = 1;
                    lock->l_type = F_WRLCK;
                    lock->l_start = i;
                    result = lock_check(des,lock);
                    if(result == -1){
                        printf("Cannot check if byte nr %i is locked\n",i);
                    }
                    else if(result == 1){
                        char *ltype;
                        if(lock->l_type == F_RDLCK){
                            ltype = "read";
                        }
                        else{
                            ltype = "write";
                        }
                        printf("Byte nr %i is %s locked. Owner PID: %i\n",i,ltype,lock->l_pid);
                    }
                    i++;
                }
            }
            free(lock);
        }
        else if(option != '0'){
            printf("Wrong argument\n");
        }
        while (getchar() != '\n');

    }
    close(des);
}


int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Wrong number of arguments\n");
        return -1;
    }

    int des = open(argv[1],O_RDWR);
    if(des < 0){
        printf("Cannot open file\n");
        return -1;
    }
    else{
        close(des);
    }
    go(argv[1]);

    return 0;
}