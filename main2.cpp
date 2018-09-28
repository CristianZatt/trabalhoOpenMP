#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]){   
    if (argc != 4) {
		printf("Usage: ./a X Y NUM_THREADS\n");
		exit(1);
	}
    int i, thread_id, x, y, max_threads;
    int g_loops, p_loops;
    g_loops = 0;

    x = atoi(argv[1]);
	y = atoi(argv[2]);
	max_threads = atoi(argv[3]);

    // Paraleliza este pedaço de código
    #pragma omp parallel private(p_loops, thread_id)
    {
        p_loops = 0;
        thread_id = omp_get_thread_num();

        // Paralelizando o loop
        #pragma omp for
        for (i=0; i<100000; ++i)
        {
            ++p_loops;
        }

        // parte crítica pois usa a variavel global acumulador de loops g_loops
        #pragma omp critical
        {
            printf("Thread %d is adding its iterations (%d) to sum (%d), ",
                   thread_id, p_loops, g_loops);
            g_loops += p_loops;
            printf(" total nloops is now %d.\n", g_loops);
        }
    }
    printf("Total # loop iterations is %d\n",
           g_loops);
    return 0;
}