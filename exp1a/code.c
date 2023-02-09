#include <stdio.h>
#include <math.h>

static double log_2;

// (3/2)^n
double f1(int n)
{
    return pow(1.5, n);
}

// n^3
int f2(int n)
{
    return pow(n, 3);
}

// 2^n
unsigned long long f3(int n)
{
    return pow(2, n);
}

// (log2 n)
double f4(double n)
{
    return (log(n) / log_2);
}

// (log2 n)^2
double f5(int n)
{
    return pow(f4(n), 2);
}

// log2 (n!)
double f6(int n)
{
    if (n == 0)
        return 0;
    double ans = 0;
    for (int i = 1; i <= n; i++)
    {
        ans += log(i);
    }
    ans /= log_2;
    return ans;
}

// ln(ln(n))
double f7(int n)
{
    return log(log(n));
}

// nlog(n)
double f8(int n)
{
    return n * f4(n);
}

// (log2 (n))^(log2 (n))
double f9(int n)
{
    double a = f4(n);
    return pow(a, a);
}

// n^(log2(log2(n)))
double f10(int n)
{
    return pow(n, f4(f4(n)));
}

// n!
unsigned long long f11(int n)
{
    if (n == 0 || n == 1)
        return 1;
    return n * f11(n - 1);
}

int main()
{
    log_2 = log(2);
    printf("n  |(3/2)^n                    |n^3     |2^n                  |log2(n)   |(log2(n))^2|(log2(n!))  |ln(ln(n))  |nlog2(n) |(log2(n))^(log2(n))|n^(log2(log2(n)))|n!                   |\n");
    for (int i = 0; i < 181; i++)
    {
        printf("-");
    }
    printf("\n");
    for (int i = 0; i <= 100; i++)
    {
        printf("%3d", i);
        printf(" | %23.3f", f1(i));
        printf(" | %7d", f2(i));
        printf(" | %19llu", f3(i));
        printf(" | %8.3f", f4(i));
        printf(" | %9.3f", f5(i));
        printf(" | %10.3f", f6(i));
        printf(" | %9.3f", f7(i));
        printf(" | %7.3f", f8(i));
        printf(" | %17.3f", f9(i));
        printf(" | %15.3f", f10(i));
        printf(" | ");
        if (i <= 20)
            printf("%19llu |", f11(i));
        printf("\n");
        for (int i = 0; i < 181; i++)
        {
            printf("-");
        }
        printf("\n");
    }
}