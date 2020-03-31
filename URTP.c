
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
//#include <conio.h>
#include <stdlib.h>

int TIME = 1000;
double h = 0.01;
double tau = 0.00001;
//double L = 10.0;
int N = 1000;
double y[1000][1000];


void podschet()
{
	//double fN = L / h;
	//printf("%d \n", N);
	/*float **y = (float**) malloc(TIME * sizeof(float*));
	for (int i = 0; i < TIME; i++)
	{
		y[i] = (float*) malloc(N * sizeof(float));
	}*/
	
	y[0][(int)(N / 2)] = 1.0;
	for(int t = 0; t < TIME - 1; t ++)
	{
		for(int x = 1; x < N - 1; x ++)
		{	
			//printf(" %.2f  \n", y[t + 1][x]);
			y[t + 1][x] = y[t][x] + (tau / (h * h))*(y[t][x - 1] - 2 * y[t][x] + y[t][x + 1]);
			//printf(" %.2f \n", y[t + 1][x]);
		}
	}
	for(int x = 0; x < N;	x ++)
	{
		printf(" %f \n", y[TIME - 1][x]);
	}
	/*for (int i = 0; i < TIME; i++)
	{
       	free(y[i]);
    }
    free(y);
    */	
}




int main()
{
	/*double fN = L / h;
	int N = (int) fN;
	//printf("%d \n", N);
	float **y = (float**) malloc(TIME * sizeof(float*));
	for (int i = 0; i < TIME; i++)
	{
		y[i] = (float*) malloc(N * sizeof(float));
	}
	
	y[0][(int)(N / 2)] = 1.0;*/
	podschet();

	/*for(int t = 0; t < TIME - 1; t ++)
	{
		for(int x = 1; x < N - 1; x ++)
		{	
			//printf(" %.2f  \n", y[t + 1][x]);
			y[t + 1][x] = y[t][x] + (tau / (h * h))*(y[t][x - 1] - 2 * y[t][x] + y[t][x + 1]);
			//printf(" %.2f \n", y[t + 1][x]);
		}
	}

	for(int x = 0; x < N;	x ++)
	{
		printf(" %f \n", y[TIME - 1][x]);
	}*/
	/*for (int i = 0; i < TIME; i++)
	{
        	free(y[i]);
    	}
    	free(y);*/
	return 0;
}
