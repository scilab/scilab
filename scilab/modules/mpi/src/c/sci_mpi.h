
#ifndef __SCIMPI
#define __SCIMPI


#include <stdlib.h>
#include <mpi.h>

/* Create some static datastructure to store all the Request references */
MPI_Request *request;
int **listRequestPointer;
int *listRequestPointerSize;
static int REQUEST_MAXSIZE = 10000;

#endif /* __SCIMPI */
