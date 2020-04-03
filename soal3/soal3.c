#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <unistd.h>
#define GetCurrentDir getcwd
#include <pthread.h> //library thread

void * fffuuu(void * ptr){
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    char *arg = (char *)ptr;
    char dirname[100];
    char *eks = strrchr(arg,'.');
    char ext[100];
    for (int i = 0; i < strlen(eks); i++){
        ext[i] = tolower(eks[i]);
    }
    char *newname = strrchr(arg, '/');
    int leng2 = strlen(newname);
    int lengt = strlen(arg);
    int newleng = lengt - leng2;
    int i;
    char path[300];
    
    for(i=0; i<=newleng; i++){
        path[i]=arg[i];
    }
    char nfile[100];
    for(i=0;i<strlen(newname)-strlen(eks)-1;i++){
        nfile[i]=newname[i+1];
    }
    printf("%s\n",nfile);
    strcat(nfile,ext);
    // printf("%s\n",ext);
    printf("%s\n",nfile);
    //printf("%d\n%d\n",leng2,lengt);
    printf("%s\n",path);
    if (!ext) {
        //no extension
        sprintf(dirname,"%s/Unknown/",buff);
        struct stat sb;
        

        if (stat(dirname, &sb) == 0 && S_ISDIR(sb.st_mode)) {
            printf("Directory exist\n");
        }
        else {
            if((mkdir(dirname,00777))==-1) {
                fprintf(stdout,"error in creating dir\n");
                return 0;
            }
        }
        DIR *dr = opendir(path);
        struct dirent *dir;
        if (dr){
            while ((dir = readdir(dr)) != NULL){
                //printf("%s\n",dir->d_name);
                if(strcmp(nfile,dir->d_name)==0){
                    char from[100], to[100];
                    sprintf(to,"%s",dirname);
                    strcpy(from,path);
                    strcat(from,dir->d_name);
                    strcat(to,dir->d_name);
                    rename(from,to);
                }
            }
            closedir(dr);
        }
    }
    else{
        sprintf(dirname,"%s/%s/",buff,ext+1);
        struct stat sb;

        if (stat(dirname, &sb) == 0 && S_ISDIR(sb.st_mode)) {
            printf("Directory exist\n");
        }
        else {
            if((mkdir(dirname,00777))==-1) {
                fprintf(stdout,"error in creating dir\n");
                return 0;
            }
        }
        DIR *dr = opendir(path);
        struct dirent *dir;
        if (dr){
            while ((dir = readdir(dr)) != NULL){
                //printf("%s\n",dir->d_name);
                if(strcmp(nfile,dir->d_name)==0){
                    char from[100], to[100];
                    sprintf(to,"%s",dirname);
                    strcpy(from,path);
                    strcat(from,dir->d_name);
                    strcat(to,dir->d_name);
                    rename(from,to);
                }
            }
            closedir(dr);
        }
    }
}

void * cccuuu (void * ptr){
    //printf("masuk");
    //fflush(stdout);
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    char dirname[100];
    char dirname2[100];
    sprintf(dirname,"%s/",buff);
    DIR *dr = opendir(dirname);
    struct dirent *dir;
    if (dr){
        while ((dir = readdir(dr)) != NULL){
            char nmfl[100];
            strcpy(nmfl,dir->d_name);
            char *ext = strrchr(nmfl,'.');
            if(!ext){
                sprintf(dirname2,"%s/Unknown/",buff);
                struct stat sb;
                if (stat(dirname, &sb) == 0 && S_ISDIR(sb.st_mode)) {
                    printf("Directory exist\n");
                }
                else {
                if((mkdir(dirname,00777))==-1) {
                    fprintf(stdout,"error in creating dir\n");
                    return 0;
                    }
                }
                char from[100], to[100];
                sprintf(to,"%s",dirname2);
                strcpy(from,dirname);
                strcat(from,dir->d_name);
                strcat(to,dir->d_name);
                rename(from,to);
            }
            else{
                sprintf(dirname2,"%s/%s/",buff,ext+1);
                struct stat sb;

                if (stat(dirname2, &sb) == 0 && S_ISDIR(sb.st_mode)) {
                    printf("Directory exist\n");
                }
                else {
                    if((mkdir(dirname2,00777))==-1) {
                        fprintf(stdout,"error in creating dir\n");
                        return 0;
                    }
                }
                char from[100], to[100];
                sprintf(to,"%s",dirname2);
                strcpy(from,dirname);
                strcat(from,dir->d_name);
                strcat(to,dir->d_name);
                rename(from,to);
            }
        }
        closedir(dr);
    }
}

void * ddduuu(void * ptr){
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    char *arg = (char *)ptr;
    char dirname[100];
    char *eks = strrchr(arg,'.');
    char ext[100];
    for (int i = 0; i < strlen(eks); i++){
        ext[i] = tolower(eks[i]);
    }
    char *newname = strrchr(arg, '/');
    int leng2 = strlen(newname);
    int lengt = strlen(arg);
    int newleng = lengt - leng2;
    int i;
    char path[300];
    char nfile[100];
    for(i=0;i<strlen(newname)-strlen(eks)-1;i++){
        nfile[i]=newname[i+1];
    }
    strcat(nfile,ext);
    //printf("%s\n",nfile);
    for(i=0; i<=newleng; i++){
        path[i]=arg[i];
    }
    
    //printf("%d\n%d\n",leng2,lengt);
    //printf("%s\n",path);
    if (!ext) {
        //no extension
        sprintf(dirname,"%s/Unknown/",buff);
        struct stat sb;

        if (stat(dirname, &sb) == 0 && S_ISDIR(sb.st_mode)) {
            printf("Directory exist\n");
        }
        else {
            if((mkdir(dirname,00777))==-1) {
                fprintf(stdout,"error in creating dir\n");
                return 0;
            }
        }
        DIR *dr = opendir(path);
        struct dirent *dir;
        if (dr){
            while ((dir = readdir(dr)) != NULL){
                if(strcmp(nfile,dir->d_name)==0){
                    char from[100], to[100];
                    sprintf(to,"%s/",buff);
                    strcpy(from,path);
                    strcat(from,dir->d_name);
                    strcat(to,dir->d_name);
                    rename(from,to);
                }
            }
            closedir(dr);
        }
    }
    else{
        sprintf(dirname,"%s/%s/",buff,ext+1);
        struct stat sb;

        if (stat(dirname, &sb) == 0 && S_ISDIR(sb.st_mode)) {
            printf("Directory exist\n");
        }
        else {
            if((mkdir(dirname,00777))==-1) {
                fprintf(stdout,"error in creating dir\n");
                return 0;
            }
        }
        DIR *dr = opendir(path);
        struct dirent *dir;
        if (dr){
            while ((dir = readdir(dr)) != NULL){
                if(strcmp(nfile,dir->d_name)==0){
                    char from[100], to[100];
                    sprintf(to,"%s",dirname);
                    strcpy(from,path);
                    strcat(from,dir->d_name);
                    strcat(to,dir->d_name);
                    rename(from,to);
                }
            }
            closedir(dr);
        }
    }
}

int main(int argc, char * argv[]){
    int i;
    if(strcmp(argv[1],"-f")==0){
        pthread_t thread[argc-2];
        int iret;
        for(i=2;i<argc;i++){
            iret = pthread_create( &thread[i-2], NULL, fffuuu, (void *)argv[i]);
            if(iret<0){ //jika eror
                fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
                exit(EXIT_FAILURE);
            }
        }
        for(int i=2;i<argc;i++){
            pthread_join(thread[i-2],NULL);
        }
    }
    else if(strcmp(argv[1],"*")==0){
        pthread_t thread1;
        int iret;
        iret = pthread_create( &thread1, NULL, cccuuu, NULL);
        if(iret<0){ //jika eror
            fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
            exit(EXIT_FAILURE);
        }
        pthread_join(thread1, NULL);
    }
    else if(strcmp(argv[1],"-d")==0){
        pthread_t thread[argc-2];
        int iret;
        for(i=2;i<argc;i++){
            iret = pthread_create( &thread[i-2], NULL, ddduuu, (void *)argv[i]);
            if(iret<0){ //jika eror
                fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
                exit(EXIT_FAILURE);
            }
        }
        for(int i=2;i<argc;i++){
            pthread_join(thread[i-2],NULL);
        }
    }
    return 0;
}
