#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <time.h>
#include <limits.h>

long size = 0;

int file_check(const char *fpath, const struct stat *sb,
                int typeflag, struct FTW *ftwbuf){

    if(typeflag != FTW_F){
        return 0;
    }
    if(sb->st_size > size){
        return 0;
    }

    char *abs_path = malloc((PATH_MAX + 1)*sizeof(char));
    if(realpath(fpath,abs_path) == NULL){
        printf("Cannot get absolute path to file %s\n",fpath);
    }
    else{
        printf("%s ",abs_path);
    }
    printf("%li ",sb->st_size);
    printf( (S_ISDIR(sb->st_mode)) ? "d" : "-");
    printf( (sb->st_mode & S_IRUSR) ? "r" : "-");
    printf( (sb->st_mode & S_IWUSR) ? "w" : "-");
    printf( (sb->st_mode & S_IXUSR) ? "x" : "-");
    printf( (sb->st_mode & S_IRGRP) ? "r" : "-");
    printf( (sb->st_mode & S_IWGRP) ? "w" : "-");
    printf( (sb->st_mode & S_IXGRP) ? "x" : "-");
    printf( (sb->st_mode & S_IROTH) ? "r" : "-");
    printf( (sb->st_mode & S_IWOTH) ? "w" : "-");
    printf( (sb->st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");
    printf("%s\n",ctime(&sb->st_mtime));
    free(abs_path);

    return 0;
}


int main(int argc, char **argv){

    if(argc != 3){
        printf("Wrong number of parameters\n");
        return -1;
    }

    size = strtol(argv[2],NULL,10);
    if(size == 0){
        printf("Wrong number parameter\n");
        return -1;
    }

    if(nftw(argv[1],file_check,10,FTW_PHYS) <0){
        printf("Error while going through file tree\n");
        return -1;
    }

    return 0;
}

