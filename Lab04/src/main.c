//Keith Skinner
//Lab 04: Threads

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

struct matrixCell 
{ 
   int i; 
   int j; 
   int k; 
   int **a; 
   int **b; 
   int **c; 
};

void loadMatrix(int ***matrix, int m, int n)
// inserts values into a matrix 
{
    for (int i=0; i<m; ++i) {
        for (int j=0; j<n; ++i) {
            if (scanf("%d", &(*matrix)[i][j]) != 1) {
                perror("Expected more input"); 
                exit(errno);
            }
        }
    }
}

void allocateMatrix(int ***matrix, int m, int n)
// allocates the memory for a matrix
{
    (*matrix) = calloc(m, sizeof(int *));
    for(int i=0; i<m; ++i)
        (*matrix)[i] = calloc(n, sizeof(int));
}

void allocateAndLoadMatrices(int ***a, int ***b, int ***c, int m, int k, int n)
// allocates memory for the matrix
// fills matrix with values from stdin
{   
    //create and load matrix a
    allocateMatrix(a, m, k);
    loadMatrix(a, m, k);

    //create and load matrix b
    allocateMatrix(b, k, n);
    loadMatrix(b, k, n);

    //crate matrix c but load
    allocateMatrix(c, m, n);
}

void *calculate(void * data)
//Calculates the value of one elemnt of Matrix cell->c
{
    struct matrixCell *cell = data;

    for (; (cell->k) >= 0; --(cell->k) )
        (cell->c)[cell->i][cell->j] += 
            (cell->a)[cell->i][cell->k] * (cell->b)[cell->k][cell->j];

    pthread_exit(0);
}

void computeMatrix(int **a, int **b, int **c, int m, int k, int n)
//computes matrix c(m*n) from matricies a(m*k) and b(k*n)
{
    //allocate resources for the objects to be used in the theads so they don't become dangling refrences
    pthread_t *threadIDs = calloc(m*n, sizeof(pthread_t));
    struct matrixCell * cells = calloc(m*n, sizeof(struct matrixCell));
    
    //create m*n threads for the calculations
    for (int i=0; i<m; ++i) {
        for (int j=0; j<n; ++i) {
            cells[m*i+j] = (struct matrixCell) { i, j, k-1, a, b, c };
            pthread_create(&threadIDs[m*i+j], NULL /*attr*/, calculate, &cells[m*i+j]);
        }
    }

    //join threads once finished calculating
    for (int i=0; i<m*n; ++i)
        pthread_join(threadIDs[i], NULL/*retval*/);

    //clean up thread state
    free(threadIDs);
    free(cells);
}

void displayMatrix(int **matrix, int m, int n)
//display a single matrix to stdout
{
    for (int i=0; i<m; ++i) {
        for (int j=0; j<n-1; ++j)
            printf("%d\t", matrix[i][j]);
        printf("%d\n", matrix[i][n-1]);
    }
    printf("\n\n");
}

void displayMatrices(int **a, int **b, int **c, int m, int k, int n)
//print matrices a, b, and c to stdout
{
    printf("MATRIX A");
    displayMatrix(a, m, k);
    printf("MATRIX B");
    displayMatrix(b, k, n);
    printf("MATRIX C");
    displayMatrix(c, m, n);
}

void deallocateMatrix(int **matrix, int m) /*, int n)*/ //n is unused
//frees memory in matrix of size m*n
{
    for(m = m-1; m >= 0; --m)
        free( matrix[m] );
    free( matrix );
}

void deallocateMatrices(int ***a, int ***b, int ***c, int *m, int *k, int *n)
//frees memory in a, b, and c, and sets all values to NULL or 0
//assumes a is m*k, b is k*n, and c is m*n
{
    deallocateMatrix(*a, *m);
    deallocateMatrix(*b, *k);
    deallocateMatrix(*c, *m);
    *a = *b = *c = NULL;
    *m = *k = *n = 0;
}

int main(int argc, char * argv[]) 
{

    int m = 0;
    int k = 0;
    int n = 0;
    int **a = NULL;
    int **b = NULL;
    int **c = NULL;
    
    for (int i=1; i<argc; ++i) {
        if (freopen(argv[i], "r", stdin) == NULL) {
            perror("Could not open file");
            exit(errno);
        }
        scanf("%d %d %d", &m, &k, &n);
        allocateAndLoadMatrices(&a, &b, &c, m, k, n);
        computeMatrix(a, b, c, m, k, n);
        displayMatrices(a, b, c, m, k, n);
        deallocateMatrices(&a, &b, &c, &m, &k, &n);
    }
    return 0;
}



