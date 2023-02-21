#include <stdio.h>
#include <stdlib.h>
#include <time.h>    // required to use clock function if executing program on linux
#ifdef _WIN32        // to check if OS is windows (_WIN32 is a macro defined on every windows based gcc compiler)
#include <windows.h> // required to use windows api for time measurement if executing on windows
#endif

// prototypes:
void mergeSort(int *, int);
void quickSort(int *, int);
void mergeSortActual(int *, int, int, int *);
void merge(int *, int, int, int, int *);
void quickSortActual(int *, int, int);
int partition(int *, int, int);
void deepCopy(int *, int *, int);
void printArray(int *, int);
void generateRandomNumbers();
void LinuxMain();
#ifdef _WIN32
void WindowsMain();
#endif

int compareCounter; // global variable to keep count of comparison operations

int main()
{
    // generateRandomNumbers();
#ifdef _WIN32
    WindowsMain();
#else
    LinuxMain();
#endif
    return 0;

    // printf("Enter length of array: ");
    // int size;
    // scanf("%d", &size);
    // int arr[size];
    // for (int i = 0; i < size; i++)
    // {
    //     printf("arr[%d] = ",i);
    //     scanf("%d", &arr[i]);
    // }
    // printf("Input array: ");
    // printArray(arr, size);
    // quickSort(arr, size);
    // printf("After sorting: ");
    // printArray(arr, size);
}

void printArray(int *arr, int len)
{
    for (int i = 0; i < len; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void mergeSort(int *arr, int len)
{
    compareCounter = 0;
    int holder[len];
    int mid = (len - 1) / 2;
    mergeSortActual(arr, 0, mid, holder);
    mergeSortActual(arr, mid + 1, len - 1, holder);
    merge(arr, 0, mid, len - 1, holder);
}

void mergeSortActual(int *arr, int low, int high, int *holder)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        mergeSortActual(arr, low, mid, holder);
        mergeSortActual(arr, mid + 1, high, holder);
        merge(arr, low, mid, high, holder);
    }
}

void merge(int *arr, int low, int mid, int high, int *holder)
{
    int len = high - low + 1;
    int i = low, j = mid + 1;
    int temp_idx = i;
    while (i <= mid && j <= high)
    {
        compareCounter++;
        if (arr[i] < arr[j])
            holder[temp_idx++] = arr[i++];
        else
            holder[temp_idx++] = arr[j++];
    }
    if (i > mid)
    {
        while (j <= high)
            holder[temp_idx++] = arr[j++];
    }
    else if (j > high)
    {
        while (i <= mid)
            holder[temp_idx++] = arr[i++];
    }
    for (int i = low; i <= high; i++)
        arr[i] = holder[i];
}

void quickSort(int *arr, int len)
{
    compareCounter = 0;
    int part = partition(arr, 0, len - 1);
    quickSortActual(arr, 0, part);
    quickSortActual(arr, part + 1, len - 1);
}

void quickSortActual(int *arr, int low, int high)
{
    if (low < high)
    {
        int part = partition(arr, low, high);
        quickSortActual(arr, low, part - 1);
        quickSortActual(arr, part + 1, high);
    }
}

int partition(int *arr, int low, int high)
{
    int pivot = arr[low];

    // Hoare partition scheme:
    // int i = low + 1, j = high, temp;
    // while (j > i)
    // {
    //     compareCounter++;
    //     while (i < high && pivot > arr[i])
    //         i++;
    //     compareCounter++;
    //     while (j > low && pivot <= arr[j])
    //         j--;
    //     if (i < j)
    //     {
    //         temp = arr[i];
    //         arr[i] = arr[j];
    //         arr[j] = temp;
    //     }
    // }
    // arr[low] = arr[j];
    // arr[j] = pivot;
    // return j;

    // Lomuto partition scheme:
    int i = low, temp;
    for (int j = low + 1; j <= high; j++)
    {
        compareCounter++;
        if (arr[j] < pivot)
        {
            i++;
            temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
        }
    }
    arr[low] = arr[i];
    arr[i] = pivot;
    return i;
}

void deepCopy(int *source, int *dest, int len)
{
    for (int i = 0; i < len; i++)
        dest[i] = source[i];
}

void generateRandomNumbers()
{
    FILE *fptr = fopen("rand_num.txt", "w");
    time_t cur_time;
    srand((unsigned int)time(&cur_time));
    for (int i = 0; i < 100000; i++)
        fprintf(fptr, "%d\n", rand());
    fclose(fptr);
}

#ifdef _WIN32
void WindowsMain()
{
    FILE *rand_num = fopen("rand_num.txt", "r");
    FILE *dest = fopen("output.txt", "w");
    fprintf(dest, "size   | merge-sort-time | quick-sort-time | merge-sort-comparisons | quick-sort-comparisons\n");
    double time1, time2;
    LARGE_INTEGER clock_freq, start, end;
    int mergeComparisons, quickComparisons;
    QueryPerformanceFrequency(&clock_freq);
    for (int size = 100; size <= 100000; size += 100)
    {
        int arr1[size];
        int arr2[size];
        for (int j = 0; j < size; j++)
            fscanf(rand_num, "%d", &arr1[j]);
        fseek(rand_num, 0, SEEK_SET);
        deepCopy(arr1, arr2, size);

        compareCounter = 0;
        QueryPerformanceCounter(&start);
        mergeSort(arr1, size);
        QueryPerformanceCounter(&end);
        time1 = (double)(end.QuadPart - start.QuadPart) * 1000.0 / clock_freq.QuadPart;
        mergeComparisons = compareCounter;

        QueryPerformanceCounter(&start);
        quickSort(arr2, size);
        QueryPerformanceCounter(&end);
        time2 = (double)(end.QuadPart - start.QuadPart) * 1000.0 / clock_freq.QuadPart;
        quickComparisons = compareCounter;

        fprintf(dest, "%6d | %15.4f | %15.4f | %22d | %22d\n", size, time1, time2, mergeComparisons, quickComparisons);
        printf("Size %d done!\n", size);
    }
    fclose(rand_num);
    fclose(dest);
}
#endif

void LinuxMain()
{
    FILE *rand_num = fopen("rand_num.txt", "r");
    FILE *dest = fopen("output.txt", "w");
    fprintf(dest, "size   | merge-sort-time | quick-sort-time | merge-sort-comparisons | quick-sort-comparisons\n");
    double time1, time2;
    clock_t start, end;
    int mergeComparisons, quickComparisons;
    for (int size = 100; size <= 100000; size += 100)
    {
        int arr1[size];
        int arr2[size];
        for (int j = 0; j < size; j++)
            fscanf(rand_num, "%d", &arr1[j]);
        fseek(rand_num, 0, SEEK_SET);
        deepCopy(arr1, arr2, size);

        start = clock();
        mergeSort(arr1, size);
        end = clock();
        time1 = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        mergeComparisons = compareCounter;

        start = clock();
        quickSort(arr2, size);
        end = clock();
        time2 = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        quickComparisons = compareCounter;

        fprintf(dest, "%6d | %15.4f | %15.4f | %22d | %22d\n", size, time1, time2, mergeComparisons, quickComparisons);
        printf("Size %d done!\n", size);
    }
    fclose(rand_num);
    fclose(dest);
}
