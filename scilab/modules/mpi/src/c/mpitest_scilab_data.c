/*
 * Scilab data mpi data
 */
#include <mpi.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 128
#define TAG 1
#define SOURCE 0



int main(int argc, char *argv[])
{
  char buff[BUFSIZE];
  char buffRecv[BUFSIZE];

  int numprocs;
  int myid;
  int i;
  MPI_Status stat; 

  /* The test matrix */
  double A[] = {1,3,3,2,3,4};   /* Declare the matrix */
  int rowA = 1, colA = 6; /* Size of the matrix */

  MPI_Init(&argc,&argv); /* all MPI programs start with MPI_Init; all 'N' processes exist thereafter */

  MPI_Comm_size(MPI_COMM_WORLD,&numprocs); /* find out how big the SPMD world is */
  MPI_Comm_rank(MPI_COMM_WORLD,&myid); /* and this processes' rank is */

  MPI_Datatype matrixOfDouble;
  int ierr = MPI_Type_contiguous(rowA*colA, MPI_DOUBLE, &matrixOfDouble);
  if (ierr != MPI_SUCCESS) 
  {
      fprintf(stderr,"an error occurred");
  }
  ierr = MPI_Type_commit(&matrixOfDouble);
  if (ierr != MPI_SUCCESS) 
  {
      fprintf(stderr,"an error occurred");
  }

  /* At this point, all the programs are running equivalently, the rank is used to
     distinguish the roles of the programs in the SPMD model, with rank 0 often used
     specially... */
  if(myid == 0)
  {
    printf("%d: We have %d processors\n", myid, numprocs);
    for(i=0;i<numprocs;i++)
    {
      sprintf(buff, "Hello %d! ", i);
      printf("plop2\n");
      fflush(NULL);
      MPI_Send(&A, 1, matrixOfDouble, i, TAG, MPI_COMM_WORLD);

//      MPI_Send(buff, BUFSIZE, MPI_CHAR, i, TAG, MPI_COMM_WORLD);
    }
    for(i=1;i<numprocs;i++)
    {
        int count, j;
        MPI_Probe( MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, &stat );
        MPI_Get_elements( &stat, matrixOfDouble, &count);
        double BRecv[count];
        MPI_Recv(BRecv, BUFSIZE, matrixOfDouble, i, TAG, MPI_COMM_WORLD, &stat);
      for (j=0; j < count; j++) {
          printf("B[%d]: %f\n",j,BRecv[j]);
      }
        printf("%d: %s\n", myid, buff);
    }
  }
  else
  {
      int count, j;
      MPI_Probe( MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, &stat );
      MPI_Get_elements( &stat, matrixOfDouble, &count);
      double ARecv[count];
//      MPI_Recv(ARecv, 1, matrixOfDouble, SOURCE, TAG, MPI_COMM_WORLD, &stat);
      MPI_Recv(ARecv, count, MPI_DOUBLE, SOURCE, TAG, MPI_COMM_WORLD, &stat);
      for (j=0; j < count; j++) {
          ARecv[j]=ARecv[j]*myid;
          printf("__plop__: %f\n",ARecv[j]);
      }
      printf("Processor %d\n", myid);

      /* send to rank 0: */
      MPI_Send(&ARecv, 1, matrixOfDouble, 0, TAG, MPI_COMM_WORLD);
  }

  MPI_Finalize(); /* MPI Programs end with MPI Finalize; this is a weak synchronization point */
  return 0;
}
