#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>

typedef struct data {
	int msize;
	int threadNum;
	int threadCount;
	float** A;	//  A*B=C
	float** B;
	float** C;
} TaskData;

float randrange(float min, float max) {
	return (float)(rand())/RAND_MAX*(max - min) + min;
}



void* worker(void* taskData) { 
	TaskData* data = (TaskData*)taskData;
	int i,j,k;
	float sum = 0;
	for(i = data->threadNum ; i < data->msize; i = i + data->threadCount) {
		for(k = 0; k < data->msize; k++) {
			for( j = 0; j < data->msize; j++) {
				sum = sum + data->A[i][j] * data->B[j][k];
				data->C[i][k] = sum;
			}
			sum = 0;
		}
	}	 
}


void randInitMatrices(float** a, float** b, int size, float min, float max) {
	srand(time(NULL));
	int i, j;
	for( i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			a[i][j] = randrange(min, max);
			b[i][j] = randrange(min, max);
		}
	}
}


void printMatrix(float** matrix, int size) {
	int i, j;
	for (i = 0; i < size; i++) {
		for(j = 0; j < size ; j++) {
			if(j == size - 1) printf("%f\n", matrix[i][j]);
			else printf("%f ", matrix[i][j]);
		}
	}
}


int main(int argc, char** argv){
	int threadCount;
	int msize;
	int i;
	if(argc!=3){
		printf("Error: Wrong number of arguments\n");
		exit(-1);
	}
	srand(453212);
	msize = atoi(argv[1]);
	threadCount = atoi(argv[2]);
	float** A = calloc(msize, sizeof(float*));
	float** B = calloc(msize, sizeof(float*));
	float** C = calloc(msize, sizeof(float*));
	for(i = 0; i < msize; i++) {
		A[i] = calloc(msize, sizeof(float));
		B[i] = calloc(msize, sizeof(float));
		C[i] = calloc(msize, sizeof(float));
	}
	pthread_t* threads = calloc(threadCount, sizeof(pthread_t));
	TaskData* taskData = calloc(threadCount, sizeof(TaskData));
	randInitMatrices(A, B, msize, 1.0, 100.0);
	printf("A:\n**************************\n");
	printMatrix(A, msize);
	printf("**************************\nB:\n**************************\n");
	printMatrix(B, msize);
	printf("**************************\n");
	for(i = 0; i < threadCount; i++){
		taskData[i].threadNum = i;
		taskData[i].threadCount = threadCount;
		taskData[i].msize = msize;
		taskData[i].A = A;
		taskData[i].B = B;
		taskData[i].C = C;
		
		pthread_create(&(threads[i]), NULL, &worker, &taskData[i]);
	}
	
	for(i = 0; i < threadCount; i++) {
		pthread_join(threads[i], NULL);
	}
	printf("C:\n**************************\n");
	printMatrix(C, msize);
	printf("**************************\n");
	free(threads);
	free(taskData);
	for(i = 0; i < msize; i++) {
		free(A[i]);
		free(B[i]);
		free(C[i]);
	}

	free(A);
	free(B);
	free(C);
	return 0;
}

