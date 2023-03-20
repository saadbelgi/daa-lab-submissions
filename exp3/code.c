#include <stdio.h>
#include <stdlib.h>

// prototypes
int **addSquareMatrices(int **a, int **b, int n, int a_p, int a_q, int b_p, int b_q);
int **subtractSquareMatrices(int **a, int **b, int n, int a_p, int a_q, int b_p, int b_q);
int **strassensMultiplication(int **a, int **b, int n);
int **actualStrassensMultiplication(int **a, int **b, int n, int a_p, int a_q, int b_p, int b_q);
int **mallocSqaureMatrix(int n);
void freeSquareMatrix(int **mat, int n);

int **mallocSqaureMatrix(int n)
{
    int **new = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        new[i] = malloc(n * sizeof(int));
    return new;
}

void freeSquareMatrix(int **mat, int n)
{
    for (int i = 0; i < n; i++)
        free(mat[i]);
    free(mat);
}

/*
n is the order of the square (sub)matrix, (p,q) is the coordinate of top-left (first) element of the submatrix (relative to the original matrix)
If a is a 4x4 matrix and if n = 2, a_p = 2, a_q = 2, then matrix of interest is the following square submatrix of a:
a[2][2]  a[2][3]
a[3][2]  a[3][3]
*/
int **addSquareMatrices(int **a, int **b, int n, int a_p, int a_q, int b_p, int b_q)
{
    int **sum = mallocSqaureMatrix(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            sum[i][j] = a[a_p + i][a_q + j] + b[b_p + i][b_q + j];
    }
    return sum;
}

int **subtractSquareMatrices(int **a, int **b, int n, int a_p, int a_q, int b_p, int b_q)
{
    int **diff = mallocSqaureMatrix(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            diff[i][j] = a[a_p + i][a_q + j] - b[b_p + i][b_q + j];
    }
    return diff;
}

int **strassensMultiplication(int **a, int **b, int n)
{
    return actualStrassensMultiplication(a, b, n, 0, 0, 0, 0);
}

int **actualStrassensMultiplication(int **a, int **b, int n, int a_p, int a_q, int b_p, int b_q)
{
    int **prod = mallocSqaureMatrix(n);
    if (n == 2)
    {
        int p1 = a[a_p][a_q] * (b[b_p][b_q + 1] - b[b_p + 1][b_q + 1]);
        int p2 = (a[a_p][a_q] + a[a_p][a_q + 1]) * b[b_p + 1][b_q + 1];
        int p3 = (a[a_p + 1][a_q] + a[a_p + 1][a_q + 1]) * b[b_p][b_q];
        int p4 = a[a_p + 1][a_q + 1] * (b[b_p + 1][b_q] - b[b_p][b_q]);
        int p5 = (a[a_p][a_q] + a[a_p + 1][a_q + 1]) * (b[b_p][b_q] + b[b_p + 1][b_q + 1]);
        int p6 = (a[a_p][a_q + 1] - a[a_p + 1][a_q + 1]) * (b[b_p + 1][b_q] + b[b_p + 1][b_q + 1]);
        int p7 = (a[a_p][a_q] - a[a_p + 1][a_q]) * (b[b_p][b_q] + b[b_p][b_q + 1]);
        printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n", p1, p2, p3, p4, p5, p6, p7);
        prod[0][0] = p5 + p4 - p2 + p6;
        prod[0][1] = p1 + p2;
        prod[1][0] = p3 + p4;
        prod[1][1] = p5 + p1 - p3 - p7;
    }
    else
    {
        int x = n / 2;
        int **temp = subtractSquareMatrices(b, b, x, b_p, b_q + x, b_p + x, b_q + x);
        int **p1 = actualStrassensMultiplication(a, temp, x, a_p, a_q, 0, 0);
        freeSquareMatrix(temp, x);
        temp = addSquareMatrices(a, a, x, a_p, a_q, a_p, a_q + x);
        int **p2 = actualStrassensMultiplication(temp, b, x, 0, 0, b_p + x, b_q + x);
        freeSquareMatrix(temp, x);
        temp = addSquareMatrices(a, a, x, a_p + x, a_q, a_p + x, a_q + x);
        int **p3 = actualStrassensMultiplication(temp, b, x, 0, 0, b_p, b_q);
        freeSquareMatrix(temp, x);
        temp = subtractSquareMatrices(b, b, x, b_p + x, b_q, b_p, b_q);
        int **p4 = actualStrassensMultiplication(a, temp, x, a_p + x, a_q + x, 0, 0);
        freeSquareMatrix(temp, x);
        temp = addSquareMatrices(a, a, x, a_p, a_q, a_p + x, a_q + x);
        int **temp2 = addSquareMatrices(b, b, x, b_p, b_q, b_p + x, b_q + x);
        int **p5 = actualStrassensMultiplication(temp, temp2, x, 0, 0, 0, 0);
        freeSquareMatrix(temp, x);
        freeSquareMatrix(temp2, x);
        temp = subtractSquareMatrices(a, a, x, a_p, a_q + x, a_p + x, a_q + x);
        temp2 = addSquareMatrices(b, b, x, b_p + x, b_q, b_p + x, b_q + x);
        int **p6 = actualStrassensMultiplication(temp, temp2, x, 0, 0, 0, 0);
        freeSquareMatrix(temp, x);
        freeSquareMatrix(temp2, x);
        temp = subtractSquareMatrices(a, a, x, a_p, a_q, a_p + x, a_q);
        temp2 = addSquareMatrices(b, b, x, b_p, b_q, b_p, b_q + x);
        int **p7 = actualStrassensMultiplication(temp, temp2, x, 0, 0, 0, 0);
        freeSquareMatrix(temp, x);
        freeSquareMatrix(temp2, x);

        temp = addSquareMatrices(p5, p4, x, 0, 0, 0, 0);
        temp2 = addSquareMatrices(temp, p6, x, 0, 0, 0, 0);
        freeSquareMatrix(temp, x);
        temp = subtractSquareMatrices(temp2, p2, x, 0, 0, 0, 0);
        freeSquareMatrix(temp2, x);
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < x; j++)
                prod[i][j] = temp[i][j];
        }
        freeSquareMatrix(temp, x);
        temp = addSquareMatrices(p1, p2, x, 0, 0, 0, 0);
        for (int i = 0; i < x; i++)
        {
            for (int j = x; j < n; j++)
                prod[i][j] = temp[i][j - x];
        }
        freeSquareMatrix(temp, x);
        temp = addSquareMatrices(p3, p4, x, 0, 0, 0, 0);
        for (int i = x; i < n; i++)
        {
            for (int j = 0; j < x; j++)
                prod[i][j] = temp[i - x][j];
        }
        freeSquareMatrix(temp, x);
        temp = addSquareMatrices(p5, p1, x, 0, 0, 0, 0);
        temp2 = subtractSquareMatrices(temp, p3, x, 0, 0, 0, 0);
        freeSquareMatrix(temp, x);
        temp = subtractSquareMatrices(temp2, p7, x, 0, 0, 0, 0);
        for (int i = x; i < n; i++)
        {
            for (int j = x; j < n; j++)
                prod[i][j] = temp[i - x][j - x];
        }
        freeSquareMatrix(temp, x);
        freeSquareMatrix(temp2, x);
    }
    return prod;
}

int **normalMatrixMultiplication(int **a, int **b, int n)
{
    int **prod = mallocSqaureMatrix(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            prod[i][j] = 0;
            for (int k = 0; k < n; k++)
                prod[i][j] += a[i][k] * b[k][j];
        }
    }
    return prod;
}



int main()
{
    printf("Enter order of input matrices (should be a power of 2): ");
    int n;
    scanf("%d", &n);
    int **a = mallocSqaureMatrix(n);
    int **b = mallocSqaureMatrix(n);
    printf("Enter matrix elements of first matrix in row major order: ");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            scanf("%d", &a[i][j]);
    }
    printf("Enter matrix elements of second matrix in row major order: ");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            scanf("%d", &b[i][j]);
    }
    printf("Product of first and second matrices:\n");
    int **prod = strassensMultiplication(a, b, n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%15d", prod[i][j]);
        printf("\n");
    }
}