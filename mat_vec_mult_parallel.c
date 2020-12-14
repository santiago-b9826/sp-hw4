/**
 *   \file my_it_mat_vect_mult.c
 *   \brief Multiplica iterativamente un matriz nxn 
 *          por un vector de n posiciones
 *
 *   \author Danny Múnera
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* función para generar <size> cantidad de datos aleatorios */
void gen_data(double *array, int size);
/* función para multiplicar iterativamente un matriz 
 * <m x n> por un vector de tam <n> */
void mat_vect_mult(double *A, double *x, double *y, int n, int it);
/* función para imprimir un vector llamado <name> de tamaño <m>*/
void print_vector(char *name, double *y, int m);
/* función para imprimir una matriz llamado <name> de tamaño <m x n>*/
void print_matrix(char *name, double *A, int m);

int main()
{
    double *A = NULL;
    double *x = NULL;
    double *y = NULL;
    int n, iters;
    long seed;

    clock_t start, end;
    double cpu_time_used;
    
    // Obtener las dimensiones
    // printf("Ingrese la dimensión n:\n");
    scanf("%d", &n);
    // printf("Ingrese el número de iteraciones:\n");
    scanf("%d", &iters);
    // printf("Ingrese semilla para el generador de números aleatorios:\n");
    scanf("%ld", &seed);
    srand(seed);

    // La matriz A tendrá una representación unidimensional
    A = malloc(sizeof(double) * n * n);
    x = malloc(sizeof(double) * n);
    y = malloc(sizeof(double) * n);
 
    start = clock();

    // Generar valores para las matrices
    gen_data(A, n * n);
    gen_data(x, n);

    /*
    print_vector("x", x, n);
    print_matrix("A", A, n);
    */
    mat_vect_mult(A, x, y, n, iters);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Total Time: %f\n", cpu_time_used);

    // print_vector("y", y, n);
    free(A);
    free(x);
    free(y);

    return 0;
}

void gen_data(double *array, int size)
{
    int i;
    for (i = 0; i < size; i++)
        array[i] = (double)rand() / (double)RAND_MAX;
}

void mat_vect_mult(double *A, double *x, double *y, int n, int it)
{
    int h, i, j;
    
#pragma acc data copyin(A[0:n*n], x[0:n]) copyout(y[0:n])
{
    for (h = 0; h < it; h++)
    {
#pragma acc parallel loop independent
        for (i = 0; i < n; i++)
        {
            y[i] = 0.0;
            for (j = 0; j < n; j++)
                y[i] += A[i * n + j] * x[j];
        }
        // x <= y
#pragma acc parallel loop independent
        for (i = 0; i < n; i++)
            x[i] = y[i];
    }
}
}

void print_vector(char *name, double *y, int m)
{
    int i;
    printf("\nVector %s\n", name);
    for (i = 0; i < m; i++)
        printf("%f ", y[i]);
    printf("\n");
}

void print_matrix(char *name, double *A, int m)
{
    int i, j;
    printf("\nMatrix %s\n", name);
    for (i = 0; i < m; i++){
        for (j = 0; j < m; j++)
            printf("%f ", A[i * m + j]);
        printf("\n");
    }
}
