/* ---------------------------------------------------------------
 * MPI pi Calculation Example - C Version 
 * Point-to-Point communications example
 *
 * FILE: mpi_pi_send.c
 * OTHER FILES: dboard.c
 * DESCRIPTION:  MPI pi calculation example program.  C Version.
 *   This program calculates pi using a "dartboard" algorithm.  See
 *   Fox et al.(1988) Solving Problems on Concurrent Processors, vol.1
 *   page 207.  All processes contribute to the calculation, with the
 *   master averaging the values for pi.
 *
 *   SPMD version:  Conditional statements check if the process 
 *   is the master or a worker. 
 *
 *   This version uses low level sends and receives to collect results
 *
 * AUTHOR: Roslyn Leibensperger. Converted to MPI: George L. Gusciora (1/25/95)
 * LAST REVISED: 12/14/95 Blaise Barney
 * --------------------------------------------------------------- */
#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
void srandom (unsigned seed);
double dboard (int darts);
#define DARTS 5000      /* number of throws at dartboard */
#define ROUNDS 100       /* number of times "darts" is iterated */
#define MASTER 0        /* task ID of master task */

int main(int argc,char *argv[])
{
	double	homepi,         /* value of pi calculated by current task */
		pi,             /* average of pi after "darts" is thrown */
		avepi,          /* average pi value for all iterations */
		pirecv,         /* pi received from worker */
		pisum;          /* sum of workers pi values */
	int	taskid,         /* task ID - also used as seed number */
		numtasks,       /* number of tasks */
		source,         /* source of incoming message */ 
		mtype,          /* message type */
		rc,             /* return code */
		i, n;
	MPI_Status status;

	/* Obtain number of tasks and task ID */
	rc = MPI_Init(&argc,&argv);
	rc = MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	rc = MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	printf("numtasks : %d\n",numtasks);
	printf("taskid : %d\n",taskid);
	/* Set seed for random number generator equal to task ID */
	srandom (taskid);

	avepi = 0;
	for (i = 0; i < ROUNDS; i++)
		{
			/* All tasks calculate pi using dartboard algorithm */
			homepi = dboard(DARTS);

			/* Workers send homepi to master */
			/* - Message type will be set to the iteration count */
			if (taskid != MASTER)
				{
					printf("On the Slave, taskid : %d\n",taskid);
					mtype = i;
					rc = MPI_Send(&homepi, 1, MPI_DOUBLE,
								  MASTER, mtype, MPI_COMM_WORLD);
					if (rc != MPI_SUCCESS)
						printf("%d: Send failure on round %d\n", taskid, mtype);
				} else
					{
					printf("On the Master, taskid : %d\n",taskid);

						/* Master receives messages from all workers */
						/* - Message type will be set to the iteration count */
						/* - Message source will be set to the wildcard DONTCARE: */
						/*   a message can be received from any task, as long as the */
						/*   message types match */
						/* - The return code will be checked, and a message displayed */
						/*   if a problem occurred */
						mtype = i;
						pisum = 0;
						for (n = 1; n < numtasks; n++)
							{
								rc = MPI_Recv(&pirecv, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
											  mtype, MPI_COMM_WORLD, &status);
								if (rc != MPI_SUCCESS) 
									printf("%d: Receive failure on round %d\n", taskid, mtype);
								/* keep running total of pi */
								pisum = pisum + pirecv;
							}
						/* Master calculates the average value of pi for this iteration */
						pi = (pisum + homepi)/numtasks;
						/* Master calculates the average value of pi over all iterations */
						avepi = ((avepi * i) + pi)/(i + 1); 
						printf("   After %3d throws, average value of pi = %10.8f\n",
							   (DARTS * (i + 1)),avepi);
					}    
		} 

	MPI_Finalize();
	return 0;
}
