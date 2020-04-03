#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef int arr_res[4];

void main()
{
    key_t key = 1234;

    int shmid = shmget(key, sizeof(int) * 20, IPC_CREAT | 0666);
    
    int *value;
    value = shmat(shmid, NULL, 0);
    int res[4][5] = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

    int first[4][2] = {{1,2},{3,4},{4,3},{2,1}};

    int second[2][5] = {{1,2,2,2,1},{1,2,2,2,1}};


    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 5; ++j){
            int tot = 0;
            for (int k = 0; k < 2; ++k){
                tot += first[i][k] * second[k][j];
            }
            res[i][j] = tot;
            printf("%d\t", res[i][j]);
        }
        printf("\n");
    }
    
    memcpy(value, res, sizeof(int) * 20);
}