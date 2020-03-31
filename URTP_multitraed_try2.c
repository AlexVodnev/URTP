#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <pthread.h>

#define thread_count 3
//gcc -pthread URTP_multitraed_try1.c -o test.exe
#define TIME 10
#define N 10

pthread_mutex_t mutex;
pthread_cond_t cond_var;

double h = 0.01;
double tau = 0.00001;
int counter = 0;
double y[TIME][N];


void* func(void* args)
{
	int *t = (int*) args;
	int prom = (int)(N / thread_count);
	//printf("%d \n", prom);
	
	if (counter == 0)
	{
			for (int x = 1; x < prom; x ++)
			{	
				y[*t + 1][x] = y[*t][x] + (tau / (h * h))*(y[*t][x - 1] - 2 * y[*t][x] + y[*t][x + 1]);
				//printf("%f\n", y[*t + 1][x]);
			}
	}
	if ((counter != 0) && (counter != thread_count - 1))
	{
		for (int x = prom * counter; x < prom * (counter + 1); x ++)
		{
			y[*t + 1][x] = y[*t][x] + (tau / (h * h))*(y[*t][x - 1] - 2 * y[*t][x] + y[*t][x + 1]);
			//printf("%f\n", y[*t + 1][x]);
		}
	}
	if (counter == thread_count - 1)
	{
		for (int x = prom * counter; x < N - 1; x ++)
		{
			y[*t + 1][x] = y[*t][x] + (tau / (h * h))*(y[*t][x - 1] - 2 * y[*t][x] + y[*t][x + 1]);
			//printf("%f\n", y[*t + 1][x]);
		}
	}
	
	pthread_mutex_lock(&(mutex));
    counter++;
    if (counter == thread_count)
    { 
    	counter = 0;
    	pthread_cond_broadcast(&cond_var);
    }
    else
    {
    	while (pthread_cond_wait(&cond_var, &(mutex)) != 0);
    }
    pthread_mutex_unlock(&(mutex));

    //printf("***************** \n");

    return NULL;
}


int main()
{
	pthread_t threads[thread_count];
	pthread_mutex_init(&(mutex),NULL);
	pthread_cond_init(&cond_var,NULL);

	y[0][(int)(N / 2)] = 1.0;
	//printf("%f \n", y[0][(int)(N / 2)]);
	for(int t = 0; t < TIME; t ++)
	{
		for (int tc = 0; tc < thread_count; tc ++ )
		{	
			pthread_create(&threads[tc],NULL, func, (void*) &t);
		}
		for (int tc = 0; tc < thread_count; tc ++ )
		{	
			pthread_join(threads[tc],NULL);
		}
	}
	for(int x = 0; x < N;	x ++)
	{
		printf(" %f \n", y[TIME - 1][x]);
	}
	
	pthread_mutex_destroy(&(mutex));
	pthread_cond_destroy(&cond_var);
	
    return 0;
}