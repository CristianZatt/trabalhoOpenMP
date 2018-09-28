#pragma once
#ifndef __MULTIPLY_MATRIX_H__
#define __MULTIPLY_MATRIX_H__

// 행렬 곱셈에 성공하면 1, 실패하면(행렬 크기 불일치) 0을 반환.
int **multiplySquareMatix(int **matrix1, int length1, int **matrix2, int length2);

// 행렬의 값을 보여줌.
void showSquareMatrix(int **matrix, int length);

// 2차원 행렬을 생성하고 행렬에 최대 max의 랜덤값을 부여함.
int **createSquareMatrix(int **matrix, int length, int max);

// 동적으로 생성한 2차원 배열을 해제함.
void freeSquareMatrix(int **matrix);


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

#endif