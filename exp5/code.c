#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

void printParanthesis(int **s, int i, int j)
{
    if (i == j)
        printf("A%d", i + 1);
    else
    {
        printf("(");
        printParanthesis(s, i, s[i][j]);
        printf("*");
        printParanthesis(s, s[i][j] + 1, j);
        printf(")");
    }
}

void parenthesizeMatrixChain(int *p, int n)
{
    int m[n][n];
    // int s[n][n];
    int **s = malloc(sizeof(int *) * n);
    for (int i = 0; i < n; i++)
        s[i] = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
        m[i][i] = 0;
    int j, cost;
    for (int chain_length = 2; chain_length <= n; chain_length++)
    {
        for (int i = 0; i <= n - chain_length; i++) // i is the starting index of a matrix subchain
        {
            j = i + chain_length - 1; // j is the ending index of the matrix subchain
            m[i][j] = INT_MAX;
            for (int k = i; k <= j - 1; k++)
            {
                cost = m[i][k] + m[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
                if (cost < m[i][j])
                {
                    m[i][j] = cost;
                    s[i][j] = k;
                }
            }
        }
    }
    printParanthesis(s, 0, n - 1);
    for (int i = 0; i < n; i++)
        free(s[i]);
    free(s);
}

int main()
{
    printf("Enter the number of matrices: ");
    int n;
    scanf("%d", &n);
    int p[n + 1];
    printf("Enter the array of matrix dimensions: ");
    for (int i = 0; i < n + 1; i++)
        scanf("%d", p + i);
    parenthesizeMatrixChain(p, n);
}