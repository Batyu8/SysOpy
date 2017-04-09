#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <linux/limits.h>
#include <stdlib.h>


void file_info(char *filename, struct stat *info){

    char *abs_path = malloc((PATH_MAX + 1)*sizeof(char));
    if(realpath(filename,abs_path) == NULL){
        printf("Cannot print path to file %s\n",filename);
    }
    else{
        printf("%s ",abs_path);
    }
    printf("%li ",info->st_size);
    printf( (S_ISDIR(info->st_mode)) ? "d" : "-");
    printf( (info->st_mode & S_IRUSR) ? "r" : "-");
    printf( (info->st_mode & S_IWUSR) ? "w" : "-");
    printf( (info->st_mode & S_IXUSR) ? "x" : "-");
    printf( (info->st_mode & S_IRGRP) ? "r" : "-");
    printf( (info->st_mode & S_IWGRP) ? "w" : "-");
    printf( (info->st_mode & S_IXGRP) ? "x" : "-");
    printf( (info->st_mode & S_IROTH) ? "r" : "-");
    printf( (info->st_mode & S_IWOTH) ? "w" : "-");
    printf( (info->st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");
    printf("%s\n",ctime(&info->st_mtime));
    free(abs_path);
}

void file_check(char *filename, long size){

    DIR *dir;
    struct stat info;
    struct dirent *directory;

    if(lstat(filename,&info) < 0){
        printf("Error lstat\n");
        return;
    }

    if(S_ISREG(info.st_mode) != 0){
        if(info.st_size <= size){
            file_info(filename,&info);
        }
        return;
    }
    else if(S_ISDIR(info.st_mode) == 0) {
        return;
    }

    dir = opendir(filename);
    if(dir == NULL){
        printf("Cannot open directory %s\n",filename);
        return ;
    }

    while((directory = readdir(dir)) != NULL){
        if(strcmp(directory->d_name,".") != 0 && strcmp(directory->d_name,"..") != 0){
            char* name = malloc((strlen(filename)+strlen(directory->d_name)+2)*sizeof(char));
            name = strcpy(name,filename);
            name = strcat(name,"/");
            name = strcat(name,directory->d_name);
            file_check(name,size);
            free(name);
        }

    }
    closedir(dir);
    return;
}


int main(int argc, char **argv) {

    long size = 0;

    if(argc != 3){
        printf("Wrong number of parameters\n");
        return -1;
    }

    size = strtol(argv[2],NULL,10);
    if(size == 0){
        printf("Wrong number parameter\n");
        return -1;
    }

    file_check(argv[1],size);

    return 0;
}