#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h> //library thread

typedef struct pokemon_s{
    char name[20];
    char type[20];
    int AP;
    int price;
    int isRele;
}pokemon_t;

typedef struct wildpok_s{
    char name[20];
    char type[20];
    int price;
    int capRate;
    int escRate;
    int catch;
}wildpok_t;

typedef struct pokeBag_s{
    int pokeball;
    int lullaby;
    int pokem;
    int berry;
    pokemon_t * pt[7];
}pokeBag_t;

typedef struct gameplay_s{
    int state;
    int isRun;
    int carpok;
    int capmode;
    //int pokeball;
    int sumpok;
    pokeBag_t * pb;
}gameplay_t;

pokemon_t * catchpok(wildpok_t * wp){
    pokemon_t * newpok = malloc (sizeof(pokemon_t));
    memset(newpok,0,sizeof(newpok));
    strcpy(newpok->name,wp->name);
    strcpy(newpok->type, wp->type);
    newpok->price = wp->price;
    newpok->AP = 100;
    return newpok;
}

void changeState(int next, int * state){
    *state = next;
}

int addpok(pokeBag_t * pb, int a){
    if(pb->pokem==7){
        printf("Pokemon Full\n");
        return 0;
    }
    return 1;
}

void * releThread(void * ptr){

}

void * pokeThread(void * ptr){
    gameplay_t * gp = (gameplay_t *) ptr;
    while(gp->isRun){
        int i;
        for(i=0;i<gp->pb->pokem;i++){
            gp->pb->pt[i]->AP -= 10;
        }
    }
}

void * wpThread(void * ptr){
    // gameplay_t * gp = (gameplay_t *) ptr;
    // wildpok_t * wp = (wildpok_t *) ptr;
    // while(gp->isRun){
    //     if(wp->escRate<=rand()%100){
    //         wp->catch=1;
    //         catchpok(wp);
    //     }
    //     sleep(20);
    // }
}

void capmode(gameplay_t * gp){
    wildpok_t * wp = malloc(sizeof(wildpok_t));
    wp->catch=1;
    pthread_t thread1;
    int  iret1;
    iret1 = pthread_create( &thread1, NULL, &wpThread, (void *)gp); //membuat thread pertama
    if(iret1<0){ //jika eror
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }
    while(wp->catch){
        printf("---CAPTURE MODE---\n(1) Tangkap!\n(2) Item\n(3) Keluar\n");
        fflush(stdout);
        int a;
        scanf("%d",&a);
        if(!wp->catch){
            printf("Pokemonnya lepas anjir\n");
            break;
        }
        if(a==1){
            if(gp->pb->pokeball>0)
                gp->pb->pokeball--;
            else{
                printf("Pokeball kosong\n");
            }
        }
        else if(a==2){
            if(gp->pb->lullaby>0)
                gp->pb->lullaby--;
        }
        else{
            wp->catch=0;
            changeState(0,&gp->state);
        }
    }
}

void *carpok( void * ptr){
    gameplay_t * gp = (gameplay_t *) ptr;
    while(gp->carpok){
        if(rand()%10<6){
            printf("Ketemu!\n");
            fflush(stdout);
            printf("Go to Capture Mode! Press \"1\"\n");
            changeState(3,&gp->state);
            gp->carpok = 0;
            break;
        }
        sleep(10);
    }
}

void normod(gameplay_t * gp){
    int b;
    if(!gp->carpok){
        printf("---Normal Mode---\nSelect Menu\n(1) Cari Pokemon\n(2) Pokedex\n(3) Shop\n");
    }
    else{
        printf("---Normal Mode---\nSelect Menu\n(1) Berhenti Mencari\n(2) Pokedex\n(3) Shop\n");
    }
    scanf("%d",&b);
    if(b==1){
        if(!gp->carpok){
            gp->carpok=1;
            pthread_t thread1;
            int  iret1;
            iret1 = pthread_create( &thread1, NULL, carpok, (void *)gp); //membuat thread pertama
            if(iret1<0){ //jika eror
                fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
                exit(EXIT_FAILURE);
            }
        }
        else{
            gp->carpok=0;
        }
    }
    else if(b==2){
        //pokedex
        changeState(1,&gp->state);
    }
    else if(b==3){
        //shop
        changeState(2,&gp->state);
    }
    else{

    }
}

void shop (gameplay_t * gp){
    printf("---SHOP---\n");
}

void pokemon( void * ptr ){
    char normpok[5][20];
    char rarepok[5][20];
    char legpok[5][20];
    //80%, 5% setiap 20 detik, 70%, 80
    strcpy(normpok[0], "Bulbasaur");
    strcpy(normpok[1], "Charmander");
    strcpy(normpok[2], "Squirtle");
    strcpy(normpok[3], "Rattata");
    strcpy(normpok[4], "Caterpie");
    //15%, 10% setiap 20 detik, 50%, 100
    strcpy(rarepok[0], "Pikachu");
    strcpy(rarepok[1], "Eevee");
    strcpy(rarepok[2], "Jigglypuff");
    strcpy(rarepok[3], "Snorlax");
    strcpy(rarepok[4], "Dragonite");
    //5%, 20% setiap 20 detik, 30%, 200
    strcpy(legpok[0], "Mew");
    strcpy(legpok[1], "Mewtwo");
    strcpy(legpok[2], "Moltres");
    strcpy(legpok[3], "Zapdos");
    strcpy(legpok[4], "Articuno");
}

void pokedex(gameplay_t * gp){
    printf("---POKEDEX---\n");
    fflush(stdout);
    if(!gp->sumpok){
        printf("Pokemon kosong!\n");
    }
    else{
        int i;
        for(i=0;i<gp->sumpok;i++){
            printf("---(%d)---\n%s\n",i+1,gp->pb->pt[i]->name);
            printf("TYPE : %s\n",gp->pb->pt[i]->type);
            printf("AP : %d\n",gp->pb->pt[i]->AP);
            printf("POKEDOLLAR : %d\n",gp->pb->pt[i]->price);
        }
        //printf("BERRY : %d\n",gp->pb->berry);
        printf("(1) Lepas\n(2) Beri Berry\n");
        int a;
        scanf("%d",&a);
        if(a==1){

        }
        else{
            if(gp->pb->berry){
                if(gp->pb->pokem){
                    gp->pb->berry--;
                    printf("Sucess\n");
                    int i;
                    for(i=0;i<gp->pb->pokem;i++){
                        gp->pb->pt[i]->AP += 10;
                    }
                }
            }
        }
    }
}

int main(){
    gameplay_t * gp = malloc(sizeof(gameplay_t));
    memset(gp, 0, sizeof(gp));
    //gp->pb->pokeball=100;
    int * isRun = &gp->isRun;
    *isRun=1;
    int * gameState = &gp->state;
    *gameState = 0;
    while(*isRun){
        if(*gameState == 0){
            normod(gp);
        }
        else if(*gameState == 1){
            pokedex(gp);
        }
        else if(*gameState == 2){
            shop(gp);
        }
        else if(*gameState == 3){
            capmode(gp);
        }
    }
    return 0;
}
