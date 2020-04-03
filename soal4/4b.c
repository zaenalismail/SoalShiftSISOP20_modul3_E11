#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

int factorial[4][5] = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

typedef struct data{
	int i_val;
	int j_val;
} data_t;


void* factorize(void* args){
	data_t *data = (data_t*) args;

	for (int i = 0; i < 5; ++i)
	{
		int t = 1;
		for (int j = 1; j <= factorial[data->i_val][i]; ++j)
		{
			t += j;
		}
		factorial[data->i_val][i] = t; 
	}
}
char* pad(int a){
	//num of pad
	int padding = 15;
	char res[] = ""; int p = 0;
	while(a > 0){
		a /= 10;
		p++;
		// printf("%d\n", a);
	}

	// printf("%d\n", p);

	for (int i = 0; i < padding - p; ++i)
	{
		printf(" ");
	}
	// return res;
}

void main()
{
	key_t key = 1234;
	int *value;

	int shmid = shmget(key, sizeof(int) * 20, IPC_CREAT | 0666);
	value = shmat(shmid, NULL, 0);

	// sleep(5);
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 5; ++j){
			printf("%d\t", value[i*5 + j]);
		}

		printf("\n");
	}
	
	memcpy(factorial, value, sizeof(int) * 20);


	//initialize thread
	pthread_t threads[4];
	data_t thread_data[4]; 
	for (int i = 0; i < 4; ++i)
	{
		thread_data[i].i_val = i;
		int thr_stat = pthread_create(&threads[i], NULL, factorize, &thread_data[i]);
		if(thr_stat < 0){
			printf("Gagal membuat thread\n");
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	printf("\n\n\nFactorial : \n");

	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 5; ++j){
			printf("%d", factorial[i][j]);
			pad(factorial[i][j]);
		}

		printf("\n");
	}

	shmdt(value);
	shmctl(shmid, IPC_RMID, NULL);
}