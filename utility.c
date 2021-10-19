#include "utility.h"


// get random number between minimum and maximum value
float getRand(int min, int max)
{
	float number = rand() / (float)(RAND_MAX);
	number = min + number * (max - min);
	return number;
}

// get current time string
char* getTimeString()
{
	time_t t1 = time(NULL);
	return ctime(&t1);
}


// get index of first end of line character in the string
int getEOLIndex(char* str)
{
	int idx = 0;
	while (str[idx] != '\n')
	{
		idx++;
	}
	return idx;
}
