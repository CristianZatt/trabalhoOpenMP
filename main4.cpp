#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MultiplyMatrix.h"
#include <omp.h>

int main()
{
	time_t start = 0, end = 0, t_criacao;
	int **matrix1 = NULL, **matrix2 = NULL, **result = NULL, max_threads;
	int length, max = 1;
	int i;
	int count;

	srand((size_t)time(NULL));

	fputs("Input the width or height of the square matrix : ", stdout);
	scanf("%d", &length);
	getchar();

	fputs("How many times do you want to multiply : ", stdout);
	scanf("%d", &count);
	getchar();

	fputs("How many threads do you want : ", stdout);
	scanf("%d", &max_threads);
	getchar();

	start = clock();

	matrix1 = createSquareMatrix(matrix1, length, max);
	matrix2 = createSquareMatrix(matrix2, length, max);

	t_criacao = clock();

	for (i = 0; i < count; i++)
	{
		
		result = multiplySquareMatix(matrix1, length, matrix2, length);
	}
	end = clock();

	showSquareMatrix(result, length);

	freeSquareMatrix(matrix1);
	freeSquareMatrix(matrix2);
	freeSquareMatrix(result);

	printf("\nTime of creation : %lf sec", ((double)(t_criacao - start) / CLOCKS_PER_SEC));
	printf("\nAverage elasped time of one multiplication : %lf sec", ((double)(end - t_criacao - start) / CLOCKS_PER_SEC) / count);
	printf("\nTotal time of multiplications : %lf sec", ((double)(end - t_criacao - start) / CLOCKS_PER_SEC));
	printf("\nTotal time of execution : %lf sec\n", ((double)(end - start) / CLOCKS_PER_SEC));
	system("pause");
	return 0;
}