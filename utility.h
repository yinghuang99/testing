
#ifndef _Utility_
#define _Utility_

#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>

#define _ITERATIONS 100
#define RAND_STR_LEN 12
#define RAND_MIN_LIMIT 5700
#define RAND_MAX_LIMIT 6500
#define _THRESHOLD 6000
#define CYCLE_DURATION 1

#define TIME_STR_LEN 20

#define SIZE_HValue 120

#define BASE_MSG_LEN 40

#define COUNT_STR_LEN 2

typedef struct _HeightValues
{
	char timeStr[TIME_STR_LEN];
	int coordinate_x;
	int coordinate_y;
	float height;

} HeightValues;

float getRand(int min, int max);

char* getTimeString();

int getEOLIndex(char* str);

#endif 