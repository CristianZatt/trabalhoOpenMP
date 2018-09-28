#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int DIM = 3;

int main(int argc, char* argv[]) {
	if (argc != 4) {
		printf("Usage: ./kmeans N K NUM_THREADS\n");
		exit(1);
	}
	int i, j, k, n, c;
	double dmin, dx;
	double *x, *mean, *sum;
	int *cluster, *count, color;
	int flips;
	double temp;
	
	k = atoi(argv[1]);
	n = atoi(argv[2]);
	int max_threads;
	max_threads = atoi(argv[3]);

	x = (double *)malloc(sizeof(double)*DIM*n);
	mean = (double *)malloc(sizeof(double)*DIM*k);
	sum= (double *)malloc(sizeof(double)*DIM*k);
	cluster = (int *)malloc(sizeof(int)*n);
	count = (int *)malloc(sizeof(int)*k);

	for (i = 0; i < k; i++) {
		mean[i*DIM]   = (double)rand() / (double)RAND_MAX;
		mean[i*DIM+1] = (double)rand() / (double)RAND_MAX;
		mean[i*DIM+2] = (double)rand() / (double)RAND_MAX;

	}
	for (i = 0; i < n; i++) {
        x[i*DIM]   = (double)rand() / (double)RAND_MAX;
        x[i*DIM+1] = (double)rand() / (double)RAND_MAX;
        x[i*DIM+2] = (double)rand() / (double)RAND_MAX;
    }

	for (i = 0; i<n; i++)
		cluster[i] = 0;
	flips = n;

	double tempoinicial = omp_get_wtime();
	omp_set_num_threads(max_threads);

	while (flips>0) {
		flips = 0;

		for (j = 0; j < k; j++) {
			count[j] = 0;
			for (i = 0; i < DIM; i++){
				sum[j*DIM+i] = 0.0;
			}
		}

		#pragma omp parallel default(shared)
		{
			#pragma omp for private(c, j, dmin, color) reduction(+: dx) schedule(static)
			for (i = 0; i < n; i++) {
				dmin = -1; 
				color = cluster[i];
				for (c = 0; c < k; c++) {
					dx = 0.0;
						for (j = 0; j < DIM; j++) {
							dx +=  (x[i*DIM+j] - mean[c*DIM+j])*(x[i*DIM+j] - mean[c*DIM+j]);
						}
					if (dx < dmin || dmin == -1) {
						color = c;
						dmin = dx;
					}

				}
				if (cluster[i] != color) {
					flips++;
					cluster[i] = color;
				}
			}

		}

		for (i = 0; i < n; i++) {
			count[cluster[i]]++;
			for (j = 0; j < DIM; j++){
				sum[cluster[i]*DIM+j] += x[i*DIM+j];
			}
		}

		for (i = 0; i < k; i++) {
			for (j = 0; j < DIM; j++) {
				mean[i*DIM+j] = sum[i*DIM+j]/count[i];
			}
		}
	}
	double tempofinal = omp_get_wtime();
	printf("Tempo decorrido = %.16f \n", tempofinal - tempoinicial);

	free(x);
	free(mean);
	free(sum);
	free(cluster);
	free(count);

	return(0);
}