#include "sensor.h"

void executeSensor(int my_rank, int adjacent_up_rank, int adjacent_down_rank,
	int adjacent_left_rank, int adjacent_right_rank, int base_rank)
{
	int iter = 0;

	// to store adjacent nodes to each sensor
	float adj_val_left;
	float adj_val_right;
	float adj_val_up;
	float adj_val_down;

	// simple moving average will be stored in this variable
	float moving_average = 0.0f;
	double total = 0.0f;
	int n_count = 0;

	// execute all the iterations
	for (iter = 0; iter < _ITERATIONS; iter++)
	{
		// generate a reading
		float reading = getRand(RAND_MIN_LIMIT, RAND_MAX_LIMIT);
		total += reading;
		n_count++;

		// compute the simple moving average
		moving_average = total / n_count;


		// regenerate the reading unless the threshold value is reached
		while (moving_average < _THRESHOLD)
		{
			// generate a random between the given limits
			reading = getRand(RAND_MIN_LIMIT, RAND_MAX_LIMIT);
			total += reading;

			// compute the new moving average
			moving_average = total / n_count;
			n_count++;
			sleep(CYCLE_DURATION);
		}

		int match_count = 0;

		MPI_Request request;


		// share the moving average with the adjacent nodes


		// send the value to the sensor below
		if (adjacent_down_rank != -1)
		{
			MPI_Isend(&moving_average, 1, MPI_FLOAT, adjacent_down_rank, 101, MPI_COMM_WORLD, &request);
		}

		// send the moving average to upper sensor
		if (adjacent_up_rank != -1)
		{
			MPI_Isend(&moving_average, 1, MPI_FLOAT, adjacent_up_rank, 101, MPI_COMM_WORLD, &request);
		}

		// send the moving average to the left sensor
		if (adjacent_left_rank != -1)
		{
			MPI_Isend(&moving_average, 1, MPI_FLOAT, adjacent_left_rank, 101, MPI_COMM_WORLD, &request);
		}

		// send the moving average to the right sensor
		if (adjacent_right_rank != -1)
		{
			MPI_Isend(&moving_average, 1, MPI_FLOAT, adjacent_right_rank, 101, MPI_COMM_WORLD, &request);
		}



		// receive the moving averages from the adjacent node
		if (adjacent_down_rank != -1)
		{

			MPI_Recv(&adj_val_down, 1, MPI_FLOAT, adjacent_down_rank, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			

			// if the difference between the received and current node's average is less than threshold
			// then add one to match counts
			if (abs(moving_average - adj_val_down) <= 100)
			{
				match_count++;
			}
		}

		if (adjacent_up_rank != -1)
		{
			MPI_Recv(&adj_val_up, 1, MPI_FLOAT, adjacent_up_rank, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// if the difference between the received and current node's average is less than threshold
			// then add one to match counts
			if (abs(moving_average - adj_val_up) <= 100)
			{
				match_count++;
			}

		}

		if (adjacent_left_rank != -1)
		{
			MPI_Recv(&adj_val_left, 1, MPI_FLOAT, adjacent_left_rank, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// if the difference between the received and current node's average is less than threshold
			// then add one to match counts
			if (abs(moving_average - adj_val_left) <= 100)
			{
				match_count++;
			}
		}

		if (adjacent_right_rank != -1)
		{
			MPI_Recv(&adj_val_right, 1, MPI_FLOAT, adjacent_right_rank, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// if the difference between the received and current node's average is less than threshold
			// then add one to match counts
			if (abs(moving_average - adj_val_right) <= 100)
			{
				match_count++;
			}
		}



		// if enough matches are found then send a message to base node
		if (match_count >= 2)
		{
			// convert the moving average to string
			char moving_num_str[RAND_STR_LEN];
			snprintf(moving_num_str, RAND_STR_LEN, "%f", moving_average);



			// get current time, i.e., event time
			char* timeStr = getTimeString();


			// convert the count of matches found to string
			char match_count_str[COUNT_STR_LEN];

			snprintf(match_count_str, COUNT_STR_LEN, "%d", match_count);



			// create multiple sections of the message that is to be send to the base,
			// we do so that we don't need to send to many messages and
			// we send all the information as a one message
			int size_curr = 0, size_total = 0;
			char message_base[BASE_MSG_LEN];

			// first copy timestamp to the message
			strcpy(message_base, timeStr);


			// update the index i.e.,  the position after the timestamp in the message
			size_curr = strlen(timeStr);
			size_total = size_curr;



			// save match counts after the timestamp
			size_curr = strlen(match_count_str);
			strcpy(&message_base[size_total], match_count_str);
			message_base[size_curr + size_total] = '\n';

			// update the position the place after the match counts
			size_total += (size_curr + 1);

			// save moving average after the match counts 
			size_curr = strlen(moving_num_str);
			strcpy(&message_base[size_total], moving_num_str);

			// save terminating characters at the end of the message
			message_base[size_curr + size_total] = '\n';
			message_base[size_curr + size_total + 1] = '\0';

			// send the message to base station
			MPI_Send(message_base, BASE_MSG_LEN, MPI_CHAR, base_rank, 112, MPI_COMM_WORLD);
			// size_total += size_curr;

			// printf("%d----%s\n", my_rank, moving_num_str);
		}




		// printf("%d-----iter %d\n", my_rank, iter);
	}

	// send the termianting message to base
	char message_base[BASE_MSG_LEN] = "end";
	MPI_Send(message_base, BASE_MSG_LEN, MPI_CHAR, base_rank, 112, MPI_COMM_WORLD);
	// printf("%d sensor -- ended\n", my_rank );
}