#include <stdio.h>
#include <stdlib.h>
#include <time.h>	 // required to use clock function if executing program on linux
#ifdef _WIN32		 // to check if OS is windows (_WIN32 is a macro defined on every windows based gcc compiler)
#include <windows.h> // required to use windows api for time measurement if executing on windows
#endif

/* clock function of time.h on linux provides precision upto microseconds,
but on windows precision upto only milliseconds is supported by clock function
on windows, the windows api has much better functions for measuring time. */

// prototypes:
void selectionSort(int *, int);
void insertionSort(int *, int);
void deepCopy(int *, int *, int);
void generateRandomNumbers();
void LinuxMain();
#ifdef _WIN32
void WindowsMain();
#endif

int main()
{
	generateRandomNumbers();
#ifdef _WIN32
	WindowsMain();
#else
	LinuxMain();
#endif
	return 0;
}

void selectionSort(int *arr, int len)
{
	int min_i, temp;
	for (int i = 0; i < len; i++)
	{
		min_i = i;
		for (int j = i + 1; j < len; j++)
		{
			if (arr[j] < arr[min_i])
				min_i = j;
		}
		if (i != min_i)
		{
			temp = arr[min_i];
			arr[min_i] = arr[i];
			arr[i] = temp;
		}
	}
}

void insertionSort(int *arr, int len)
{
	int key, pos;
	for (int i = 1; i < len; i++)
	{
		key = arr[i];
		pos = 0;
		for (int j = i - 1; j >= 0; j--)
		{
			if (arr[j] > key)
				arr[j + 1] = arr[j];
			else
			{
				pos = j + 1;
				break;
			}
		}
		arr[pos] = key;
	}
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
	fprintf(dest, "size   | selection-sort-time | insertion-sort-time\n");
	double time1, time2;
	LARGE_INTEGER clock_freq, start, end;
	QueryPerformanceFrequency(&clock_freq);
	for (int size = 100; size <= 100000; size += 100)
	{
		int arr1[size];
		int arr2[size];
		for (int j = 0; j < size; j++)
			fscanf(rand_num, "%d", &arr1[j]);
		fseek(rand_num, 0, SEEK_SET);
		deepCopy(arr1, arr2, size);

		QueryPerformanceCounter(&start);
		selectionSort(arr1, size);
		QueryPerformanceCounter(&end);
		time1 = (double)(end.QuadPart - start.QuadPart) * 1.0 / clock_freq.QuadPart;

		QueryPerformanceCounter(&start);
		insertionSort(arr2, size);
		QueryPerformanceCounter(&end);
		time2 = (double)(end.QuadPart - start.QuadPart) * 1.0 / clock_freq.QuadPart;

		fprintf(dest, "%6d | %19f | %19f\n", size, time1, time2);
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
	fprintf(dest, "size   | selection-sort-time | insertion-sort-time\n");
	double time1, time2;
	clock_t start, end;
	for (int size = 100; size <= 100000; size += 100)
	{
		int arr1[size];
		int arr2[size];
		for (int j = 0; j < size; j++)
			fscanf(rand_num, "%d", &arr1[j]);
		fseek(rand_num, 0, SEEK_SET);
		deepCopy(arr1, arr2, size);

		start = clock();
		selectionSort(arr1, size);
		end = clock();
		time1 = (double)(end - start) * 1.0 / CLOCKS_PER_SEC;

		start = clock();
		insertionSort(arr2, size);
		end = clock();
		time2 = (double)(end - start) * 1.0 / CLOCKS_PER_SEC;

		fprintf(dest, "%6d | %19f | %19f\n", size, time1, time2);
		printf("Size %d done!\n", size);
	}
	fclose(rand_num);
	fclose(dest);
}
