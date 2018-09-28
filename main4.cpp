#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int max_threads;


int **multiplySquareMatix(int **matrix1, int length1, int **matrix2, int length2);

void showSquareMatrix(int **matrix, int length);

int **createSquareMatrix(int **matrix, int length, int max);

void freeSquareMatrix(int **matrix);

int main()
{
	time_t start = 0, end = 0, t_criacao;
	int **matrix1 = NULL, **matrix2 = NULL, **result = NULL;
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




int **multiplySquareMatix(int **matrix1, int length1, int **matrix2, int length2) 
{
	int i, j, k;
	int **result = NULL;
	result = createSquareMatrix(result, length1, 0);

	for (i = 0; i < length1; i++)
	{
		for (j = 0; j < length2; j++)
		{
			for (k = 0; k < length1; k++)
				result[i][j] += matrix1[i][k] * matrix2[k][j];
		}
	}

	return result;
}

void showSquareMatrix(int **matrix, int length)
{
	int i, j;
	for (i = 0; i < length; i++)
	{
		for (j = 0; j < length; j++)
		{
			printf("%5d ", matrix[i][j]);
		}
		puts("");
	}
	puts("");
}

int **createSquareMatrix(int **matrix, int length, int max)
{
	int i;
	matrix = (int **)calloc(length, sizeof(int *));
	matrix[0] = (int *)calloc(length * length, sizeof(int));
	for (i = 1; i < length; i++)
	{
		matrix[i] = matrix[i - 1] + length;
	}

	for (i = 0; i < length * length; i++)
	{
		matrix[0][i] = rand(); // % (max + 1);

	}
	return matrix;
}

void freeSquareMatrix(int **matrix)
{
	free(matrix[0]);
	free(matrix);
}