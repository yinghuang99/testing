
#ifndef _SENSOR_
#define _SENSOR_

#include "utility.h"


void executeSensor(int my_rank, int adjacent_up_rank,
	int adjacent_down_rank,
	int adjacent_left_rank,
	int adjacent_right_rank,
	int base_rank);

#endif 