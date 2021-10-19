#include "altimeter.h"

void executeAltimeter(HeightValues* hvalues, int* count, bool* execute_flag, int GRID_DIMS_M, int GRID_DIMS_N)
{
	// index where current reading will be stored
	int index = 0;

	// continue unless base station sets the flag to false
	while (*execute_flag == true)
	{
		// position where current reading will be stored
		HeightValues* cv = &hvalues[index];

		// we will use fifo approach , the value which is inserted first we be removed first
		// update the index, if last index is reached then we move to first index using modulus operator
		index = (index + 1) % SIZE_HValue;

		// if the array is not filled then update the count
		if (*count != SIZE_HValue)
		{
			*count = (*count) + 1;
		}

		// get current time
		char* timeStr = getTimeString();
		timeStr[getEOLIndex(timeStr)] = '\0';

		// get current reading of height
		float reading = getRand(_THRESHOLD, RAND_MAX_LIMIT);

		// get dimesions of grid
		int cx = rand() % GRID_DIMS_M;
		int cy = rand() % GRID_DIMS_N;

		// update the the corresponding values in the array
		strcpy(cv->timeStr, timeStr);
		cv->coordinate_x = cx;
		cv->coordinate_y = cy;
		cv->height = reading;

		// printf("%s-- %g, %d, %d\n", timeStr, reading, cx, cy);
		sleep(CYCLE_DURATION);
	}
}