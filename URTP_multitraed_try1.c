#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <pthread.h>

#define thread_count 1
//pthread_mutex_t mutex;
pthread_cond_t cond_var;
//gcc -pthread URTP_multitraed_try1.c -o test.exe

int TIME = 5000;
double h = 0.01;
double tau = 0.00001;
double L = 10;
int counter = 0;


typedef struct{
	int ind;
	int ctime;
	int fN;
	double** array1;
} pthrData;



void* func(void* thr_Data)
{
	pthrData *data = (pthrData*) thr_Data;
	int prom = (int)((data->fN) / thread_count);
	
	pthread_mutex_lock(&(param.mutex));
    counter++;
    if (counter == thread_count)
    {
    	counter = 0;
    	pthread_cond_broadcast(&cond_var);
    }
    else
    {
    	while (pthread_cond_wait(&cond_var, &(param.mutex)) != 0);
    }
    pthread_mutex_unlock(&(param.mutex));

    for( int x = (data->ind)*prom; x < ((data->ind) + 1)*prom; x ++)
    {
    	data->array1[data->ctime + 1][x] = data->array1[data->ctime][x] + (tau / (h * h))*(data->array1[data->ctime][x - 1] - 2 * data->array1[data->ctime][x] + data->array1[data->ctime][x + 1]);
    }
    return NULL;
}


use_mutex_t param;


int main()
{
	pthread_t threads[thread_count];
	pthread_mutex_init(&(param.mutex),NULL);
	pthread_cond_init(&cond_var,NULL);

	int N = 5000;

	double **y = (double**) malloc(TIME * sizeof(double*));
	for (int i = 0; i < TIME; i++)
	{
		y[i] = (double*) malloc(N * sizeof(double));
	}
	
	y[0][(int)(N / 2)] = 1.0;
	
	pthrData* threadData = (pthrData*) malloc(N * sizeof(pthrData));

	

	for(int t = 0; t < TIME - 1; t ++)
	{
		for(int i = 0; i < thread_count; i ++)
		{
			threadData[i].ind = i;
			threadData[i].ctime = t;
			threadData[i].fN = N;
			threadData[i].array1 = y;
			pthread_create(&threads[i], NULL, func, &threadData[i]);
		}
		for (int i=0; i<thread_count; i++)
		{
			pthread_join(threads[i],NULL);
		}
		y[t + 1][0] = 0;
		y[t + 1][N] = 0;
	}
	pthread_mutex_destroy(&(param.mutex));
	pthread_cond_destroy(&cond_var);
	/*for(int x = 0; x < N;	x ++)
	{
		printf(" %f \n", y[TIME - 1][x]);
	}*/
	for (int i = 0; i < TIME; i++)
	{
        free(y[i]);
    }
    free(y);
    free(threadData);

    return 0;
}