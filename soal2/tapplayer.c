#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "header/conio.h"

#define PORT 8080

int hitung_string(char* str, char* sub){
    int res = 0;
    char sep[] = "\n";
    char* token = strtok(str, sep);
    while(token != NULL){
        if(strcmp(token, sub) == 0){
            res++;
        }

        token = strtok(NULL, sep);
    }

    return res;
}

int win_check(char* str){
    char flag[] = "aerosolismic_pwd_flag_for_winning";
    char *flagger = strstr(str, flag);
    if (strstr(str, flag) != NULL)
    {
        return 1;
    }

    return 0;
}

int main_menu(){
    int res;
    printf("Main Menu\n");
    printf("[1] Register\n");
    printf("[2] Login\n");
    printf("Pilih menu yang Anda inginkan > ");
    scanf("%d", &res);
    
    return res;
}

// void win(){
//     system('/bin/sh');
// }

char* get_usern(){
    char* u[512];
    printf("Masukkan username Anda > ");
    scanf("%s", *u);
    return *u;
}

char* get_passwd(){
    char* u[512];
    printf("Masukkan password Anda > ");
    scanf("%s", *u);
    return *u;
}

int menu_player(){
    int res;
    printf("Selamat datang, player\n");
    printf("[1] Find Match\n");
    printf("[2] Logout\n");
    printf("Pilih menu yang Anda inginkan > ");
    scanf("%d", &res);
    
    return res;
}

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    // -------------------------------
    int session = 0;
    char username[512]; char raw_username[512];
    char password[512];

    while(1){
        bzero(buffer, 0);
        if(session == 0){
            int menu = main_menu();
            char daftar[] = "";
            if(menu == 1){
                // char username[512];
                // char password[512];
                char method[] = "daftar";
                printf("Masukkan username Anda > ");
                scanf("%s", username);
                strcpy(raw_username, username);
                printf("Masukkan password Anda > ");
                scanf("%s", password);
                
                strcat(daftar, method);
                strcat(daftar, "\n");
                strcat(daftar, username);
                strcat(daftar, "\n");
                strcat(daftar, password);
                printf("%s\n", daftar);
                send(sock, daftar, strlen(daftar), 0);
                // strcpy(daftar, "");
            }else{
                char method[] = "login";
                printf("Masukkan username Anda > ");
                scanf("%s", username);
                strcpy(raw_username, username);
                printf("Masukkan password Anda > ");
                scanf("%s", password);
                
                strcat(daftar, method);
                strcat(daftar, "\n");
                strcat(daftar, username);
                strcat(daftar, "\n");
                strcat(daftar, password);
                send(sock, daftar, strlen(daftar), 0);
            }
            strcpy(daftar, "");
            valread = read(sock, buffer, 1024);
            if(strcmp(buffer, "1") == 0){
                session = 1;
                printf("Masuk ><\n");
            }else{
                printf("Gagal masuk...\n");
            }
        }else{
            int menu = menu_player();
            if(menu == 2){
                session = 0;
            }else{
                char req[] = "";
                int play = 0;
                strcat(req, "ready");
                strcat(req, "\n");
                strcat(req, raw_username);
                send(sock, req, strlen(req), 0);
                
                printf("Menunggu pemain...\n");

                while(1){
                    char req[] = "";
                    strcat(req, "wait");
                    strcat(req, "\n");
                    strcat(req, raw_username);
                    send(sock, req, strlen(req), 0);
                    read(sock, buffer, 1024);
                    // printf("%s\n",buffer); 
                    sleep(1);
                    printf("Menunggu pemain...\n");
                    if((buffer[0] == '1')){
                        break;
                    }
                }
                // gameplay
                printf("Tap spasi secepatnya!\n");
                int health = 100;
                getch();
                while(1){
                    char inputan;
                    if((inputan = getch()) == ' '){
                        char req[] = "";
                        printf("Hit!!\n");
                        strcat(req, "attack");
                        strcat(req, "\n");
                        strcat(req, raw_username);
                        send(sock, req, strlen(req), 0);
                    }
                    int nread;
                    nread = read(sock, buffer, sizeof(buffer)-1);
                    buffer[nread]='\0';    
                    int attacked;
                    if(win_check(buffer)){
                        break;
                    }

                    attacked = hitung_string(buffer, raw_username);
                    char username_poss[] = "";
                    strcat(username_poss, "1");
                    strcat(username_poss, raw_username);
                    attacked += hitung_string(buffer, username_poss);


                    if(attacked > 0){
                        printf("Kamu diserang :(\n");
                        health -= 10 * attacked;
                        printf("Health : %d\n", health);
                    }
                    fflush(stdout);
                    buffer[0]='\0';
                    
                    if(health <= 0){
                        char req[] = "";
                        strcat(req, "lose");
                        strcat(req, "\n");
                        strcat(req, raw_username);
                        send(sock, req, strlen(req), 0);
                        break;
                    }
                }

                if(health <= 0){
                    printf("Game selesai, kamu kalah :(\n");
                    strcpy(buffer, "");
                }else{
                    printf("Game selesai, kamu menang!\n");
                    strcpy(buffer, "");
                }
            }
        }
    }
    return 0;
}