#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <pthread.h>

#define thread_count 11
//gcc -pthread URTP_multitraed_try1.c -o test.exe
#define TIME 300000
#define N 9240

pthread_mutex_t mutex;
pthread_cond_t cond_var;

double h = 0.1;
double tau = 0.001;
int counter = 0;
double y[2][N];


void* func(void* args)
{
	long tc = (long) args;
	long prom = (long)(N / thread_count);
	//printf("%d\n", tc);
	for (int t = 0; t < TIME; t ++)
	{
		y[(t+1)%2][0] = 0;
		y[(t+1)%2][N] = 0;
		if (tc == 0)
		{
			
			for (long x = 1; x < prom * (tc + 1); x ++)
			{
				y[(t + 1) % 2][x] = y[t % 2][x] + (tau / (h * h))*(y[t % 2][x - 1] - 2 * y[t % 2][x] + y[t % 2][x + 1]);
			}
		}
		if (tc == thread_count - 1)
		{
			
			for (long x = prom * tc; x < N - 1; x ++)
			{
				y[(t + 1) % 2][x] = y[t % 2][x] + (tau / (h * h))*(y[t % 2][x - 1] - 2 * y[t % 2][x] + y[t % 2][x + 1]);
			}
		}
		if ((tc != 0) && (tc != thread_count - 1))
		{
			//printf("%d\n", counter); 
			for (long x = prom * tc; x < prom * (tc + 1); x ++)
			{
				y[(t + 1) % 2][x] = y[t % 2][x] + (tau / (h * h))*(y[t % 2][x - 1] - 2 * y[t % 2][x] + y[t % 2][x + 1]);
			}
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
	for (long tc = 0; tc < thread_count; tc ++ )
	{	
		//printf("%d\n", tc);
		pthread_create(&threads[tc], NULL, func, (void*) tc);
	}
	for (long tc = 0; tc < thread_count; tc ++ )
	{	
		pthread_join(threads[tc], NULL);
	}
	
	/*for(int x = 0; x < N;	x ++)
	{
		printf(" %f \n", y[(TIME -1) % 2][x]);
	}*/
	
	pthread_mutex_destroy(&(mutex));
	pthread_cond_destroy(&cond_var);
	
    return 0;
}
