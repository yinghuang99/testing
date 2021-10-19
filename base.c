#include "base.h"

void executeBase(HeightValues* hvalues, int* count, int sensors_count, int GRID_DIMS_M, int GRID_DIMS_N)
{

	int total_events_detected = 0;
	int true_events = 0;
	int false_events = 0;

	// continue unless all the sensors are terminated
	while (sensors_count > 0)
	{

		// receive a message from any source node
		MPI_Status status;
		char message_base[BASE_MSG_LEN];
		MPI_Recv(message_base, BASE_MSG_LEN, MPI_CHAR, MPI_ANY_SOURCE, 112, MPI_COMM_WORLD, &status);


		// see if that is a signal that sensor is terminated
		if (strcmp(message_base, "end") == 0)
		{
			sensors_count--;
		}
		else
		{

			// get event node rank 
			int event_node = status.MPI_SOURCE;

			printf("Event happended at the node %d\n", event_node);


			// find all the adjacents node to the event node as we did in driver.c
			int i = event_node / GRID_DIMS_N;
			int j = event_node % GRID_DIMS_N;

			printf("Adjacent nodes are: ");

			// left node
			if (j > 0)
			{
				printf("%d ", event_node - 1);
			}

			// right node
			if (j < (GRID_DIMS_N - 1))
			{
				printf("%d ", event_node + 1);
			}


			// upper node
			if (i > 0)
			{
				printf("%d ", event_node - GRID_DIMS_N);
			}


			// below node
			if (i < (GRID_DIMS_M - 1))
			{
				printf("%d ", event_node + GRID_DIMS_N);
			}
			printf("\n");





			// get the timestamp from the message, as multiple sections in the message
			// are separated by endline so get the first endline index
			// and retieve the message till endline
			int k = getEOLIndex(message_base);
			message_base[k] = '\0';
			char* timestamp = message_base;
			printf("Event timestamp = %s\n", timestamp);



			// retrieve the next section of the message which is number of matches when event
			// is detected 
			char* message_next = &message_base[k + 1];
			k = getEOLIndex(message_next);
			message_next[k] = '\0';
			printf("Number of matches = %s\n", message_next);


			// retrieve the next section of the message which is the height reading
			message_next = &message_next[k + 1];
			k = getEOLIndex(message_next);
			message_next[k] = '\0';
			char* heightStr = message_next;
			float height;


			// convert height string to flaot
			sscanf(heightStr, "%f", &height);
			printf("Reading = %f\n", height);
			total_events_detected++;

			bool is_flase_event = true;

			// see if the match exists in the altimeter readings

			for (k = 0; k < *count; k++)
			{
				if (/*strcmp(timestamp, hvalues[k].timeStr) == 0 && */ hvalues[k].coordinate_x == i 
					&& hvalues[k].coordinate_y == j && abs(hvalues[k].height - height) < 100)
				{
					is_flase_event = false;
				}
			}

			// based on the match decide if the alert is true or false
			if (is_flase_event)
			{
				false_events++;
				printf("False event\n*******\n");
			}
			else
			{
				true_events++;
				printf("True event\n*******\n");
			}
		}
	}

	printf("Total events detected: %d\n", total_events_detected);
	printf("False events detected: %d\n", false_events);
	printf("True events detected: %d\n", true_events);
	printf("Messages per event: %d\n", 1);

	// printf("everyone ended.....\n");
}