#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int max_threads, gloops = 0;

int **multiplySquareMatix(int **matrix1, int length1, int **matrix2, int length2, int tipo);

void showSquareMatrix(int **matrix, int length);

int **createSquareMatrix(int **matrix, int length, int max);

void freeSquareMatrix(int **matrix);

void printBanchmarkMultiply(int tipo, double start, double t_criacao, double end, int count);

void printBanchmarkSum(double sumResult, int tipo, double start, double end);

double sumMatrixValues(int **matrix, int length, int tipo);

int main(){
	double start = 0, end = 0, t_criacao;
	int **matrix1 = NULL, **matrix2 = NULL, **result = NULL;
	int length, max = 1, tipo;
	int i, y = 0;
	int count;

	fputs("Tamanho da matriz:  ", stdout);
	scanf("%d", &length);
	getchar();

	fputs("Multiplicacoes: ", stdout);
	scanf("%d", &count);
	getchar();

	fputs("Numero maximo de threads: ", stdout);
	scanf("%d", &max_threads);
	getchar();
	for (int t = 8; t <= max_threads; t++){
		omp_set_num_threads(t);
		printf("\n +++++++++++\n THREADS: %i", t);
		for (tipo = y; tipo <= 6; tipo++){
			start = omp_get_wtime();
			matrix1 = createSquareMatrix(matrix1, length, max);	matrix2 = createSquareMatrix(matrix2, length, max);
			t_criacao = omp_get_wtime();
			
			for (i = 0; i < count; i++) result = multiplySquareMatix(matrix1, length, matrix2, length, tipo);

			end = omp_get_wtime();
			
			printBanchmarkMultiply(tipo, start, t_criacao, end, count);
		}

		for (tipo = y; tipo <= 2; tipo++){
			double sumResult = 0;
			start = omp_get_wtime();
			sumResult = sumMatrixValues(result, length, tipo);
			end = omp_get_wtime();
			printBanchmarkSum(sumResult, tipo, start, end);
		}
		y = 1;
	}
	freeSquareMatrix(matrix1); freeSquareMatrix(matrix2); freeSquareMatrix(result);
	return 0;
}

void printBanchmarkMultiply(int tipo, double start, double t_criacao, double end, int count){
	printf("\n MTP Tipo: %i", tipo);
	printf("\nTempo de criacao : %.16f sec", t_criacao - start);
	printf("\nTempo medio de uma multiplicacao : %.16f sec", (end - t_criacao) / count);
	printf("\nTempo total das multiplicacoes : %.16f sec", end - t_criacao);
	printf("\nTempo total de execucao : %.16f sec\n", end - start);
}

void printBanchmarkSum(double sumResult, int tipo, double start, double end){
	printf("\n SUM Tipo: %i", tipo);
	printf("\n Resultado: %i", sumResult);
	printf("\nTempo da soma : %.16f sec\n", end - start);
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
		#pragma omp parallel for schedule(static) collapse(2) private(i,j,k)shared(matrix1,matrix2,result)
		for (i = 0; i < length1; i++)for (j = 0; j < length2; j++)for (k = 0; k < length1; k++)result[i][j] += matrix1[i][k] * matrix2[k][j];
	}
	if(tipo == 3){
		#pragma omp parallel for schedule(static) collapse(3) private(i,j,k)shared(matrix1,matrix2,result)
		for (i = 0; i < length1; i++)for (j = 0; j < length2; j++)for (k = 0; k < length1; k++)result[i][j] += matrix1[i][k] * matrix2[k][j];
	}
	if(tipo == 4){
		#pragma omp parallel for schedule(dynamic) private(i,j,k)shared(matrix1,matrix2,result)
		for (i = 0; i < length1; i++)for (j = 0; j < length2; j++)for (k = 0; k < length1; k++)result[i][j] += matrix1[i][k] * matrix2[k][j];
	}
	if(tipo == 5){
		#pragma omp parallel for schedule(dynamic) collapse(2) private(i,j,k)shared(matrix1,matrix2,result)
		for (i = 0; i < length1; i++)for (j = 0; j < length2; j++)for (k = 0; k < length1; k++)result[i][j] += matrix1[i][k] * matrix2[k][j];
	}
	if(tipo == 6){
		#pragma omp parallel for schedule(dynamic) collapse(3) private(i,j,k)shared(matrix1,matrix2,result)
		for (i = 0; i < length1; i++)for (j = 0; j < length2; j++)for (k = 0; k < length1; k++)result[i][j] += matrix1[i][k] * matrix2[k][j];
	}
	return result;
}

double sumMatrixValues(int **matrix, int length, int tipo){
	int i, j;
	double sum = 0, x = 0;
	if(tipo == 0){
		for (i=0;i<length;i++)for(j=0;j<length;j++)sum+=matrix[i][j];
	}
	if(tipo == 1){
		#pragma omp parallel for reduction (+:sum) collapse(2)
		for (i=0;i<length;i++)for(j=0;j<length;j++)sum+=matrix[i][j];
	}
	if(tipo == 2){
		#pragma omp parallel for reduction (+:sum) collapse(2)
		for (i=0;i<length;i++){
			for(j=0;j<length;j++){
				sum+=matrix[i][j];
				#pragma omp critical
				{
					gloops = gloops + omp_get_thread_num();
				}
			}
		}
	}
	return sum;
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