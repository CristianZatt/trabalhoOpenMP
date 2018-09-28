#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int max_threads;


int **multiplySquareMatix(int **matrix1, int length1, int **matrix2, int length2, int tipo);

void showSquareMatrix(int **matrix, int length);

int **createSquareMatrix(int **matrix, int length, int max);

void freeSquareMatrix(int **matrix);

void printBanchmarkMultiply(int tipo, double start, double t_criacao, double end, int count);

void printBanchmarkSum(int tipo, double start, double end);

int **addMatixValues(int **matrix, int length, int tipo);

int main(){
	double start = 0, end = 0, t_criacao;
	int **matrix1 = NULL, **matrix2 = NULL, **result = NULL;
	int length, max = 1, tipo;
	int i;
	int count;

	fputs("Tamanho da matriz:  ", stdout);
	scanf("%d", &length);
	getchar();

	fputs("Multiplicacoes: ", stdout);
	scanf("%d", &count);
	getchar();

	fputs("Numero de threads: ", stdout);
	scanf("%d", &max_threads);
	getchar();
	omp_set_num_threads(max_threads);
	
	for (tipo = 0; tipo < 4; i++){
		start = omp_get_wtime();
		matrix1 = createSquareMatrix(matrix1, length, max);	matrix2 = createSquareMatrix(matrix2, length, max);
		t_criacao = omp_get_wtime();
		
		for (i = 0; i < count; i++) result = multiplySquareMatix(matrix1, length, matrix2, length, tipo);

		end = omp_get_wtime();
		
		printBanchmarkMultiply(tipo, start, t_criacao, end, count);
	}
	
	end = omp_get_wtime();
	
	for (tipo = 0; tipo < 1; i++){
		start = omp_get_wtime();
		addMatixValues(result, length, tipo);
		end = omp_get_wtime();
		printBanchmarkSum(tipo, start, end);
	}
	
	
	freeSquareMatrix(matrix1); freeSquareMatrix(matrix2); freeSquareMatrix(result);
	return 0;
}

void printBanchmarkMultiply(int tipo, double start, double t_criacao, double end, int count){
	printf("\n MTP Tipo: %i", tipo);
	printf("\nTime of creation : %.16f sec", t_criacao - start);
	printf("\nAverage elasped time of one multiplication : %.16f sec", (end - t_criacao) / count);
	printf("\nTotal time of multiplications : %.16f sec", end - t_criacao);
	printf("\nTotal time of execution : %.16f sec\n", end - start);
}

void printBanchmarkSum(int tipo, double start, double end){
	printf("\n SUM Tipo: %i", tipo);
	printf("\nTime of sum : %.16f sec\n", end - start);
}


int **multiplySquareMatix(int **matrix1, int length1, int **matrix2, int length2, int tipo){
	int i, j, k;
	int **result = NULL;
	result = createSquareMatrix(result, length1, 0);
	if(tipo == 0){
		for (i = 0; i < length1; i++)for (j = 0; j < length2; j++)for (k = 0; k < length1; k++)result[i][j] += matrix1[i][k] * matrix2[k][j];
	}
	if(tipo == 1){
		#pragma omp parallel for schedule(static) private(i,j,k)shared(matrix1,matrix2,result)
		for (i = 0; i < length1; i++)for (j = 0; j < length2; j++)for (k = 0; k < length1; k++)result[i][j] += matrix1[i][k] * matrix2[k][j];
	}
	if(tipo == 2){
		#pragma omp parallel for schedule(dynamic) collapse(1) private(i,j,k)shared(matrix1,matrix2,result)
		for (i = 0; i < length1; i++)for (j = 0; j < length2; j++)for (k = 0; k < length1; k++)result[i][j] += matrix1[i][k] * matrix2[k][j];
	}
	if(tipo == 3){
		#pragma omp parallel for schedule(dynamic) collapse(2) private(i,j,k)shared(matrix1,matrix2,result)
		for (i = 0; i < length1; i++)for (j = 0; j < length2; j++)for (k = 0; k < length1; k++)result[i][j] += matrix1[i][k] * matrix2[k][j];
	}
	if(tipo == 4){
		#pragma omp parallel for schedule(dynamic) collapse(3) private(i,j,k)shared(matrix1,matrix2,result)
		for (i = 0; i < length1; i++)for (j = 0; j < length2; j++)for (k = 0; k < length1; k++)result[i][j] += matrix1[i][k] * matrix2[k][j];
	}
	return result;
}

int **addMatixValues(int **matrix, int length, int tipo){
	int i, j;
	int sum = 0;
	if(tipo == 0){
		#pragma omp parallel for reduction (+:sum)
		for (i=0;i<length;i++)for (j=0;j<length;j++)sum+=matrix[i][j];
	}
}

void showSquareMatrix(int **matrix, int length){
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

int **createSquareMatrix(int **matrix, int length, int max){
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

void freeSquareMatrix(int **matrix){
	free(matrix[0]);
	free(matrix);
}