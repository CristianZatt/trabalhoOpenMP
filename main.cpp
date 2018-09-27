#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv){
	#pragma omp parallel
	{
		printf("Parallel region is executed by thread ID %d \n", omp_get_thread_num());

		if(omp_get_thread_num() == 2 ){
			printf("Thread %d does things differently \n",omp_get_thread_num());
		}
	}

	//How Many Threads 
	// 1. #pragma omp parallel if(n>5)
	// 2. #pragma omp parallel num_threads(8)
	// 3. omp_set_num_threads(8)
	// 4. export OMP_NUM_THREADS=4 on ~/.bashrc
	// 5. Do not write like above things, it will auto setting depends on System.

	printf(" HOW MANY THREADS! \n");
	int n  =6;

	omp_set_num_threads(10);

	#pragma omp parallel if(n>3) num_threads(n)
	{
		printf("Parallel region is executed by thread ID %d \n", omp_get_thread_num());

		if(omp_get_thread_num() == 2 ){
			printf("Thread %d does things differently \n",omp_get_thread_num());
		}
	}
}