
#ifndef __SCIMPI
#define __SCIMPI

#include <mpi.h>
/* Create some static datastructure to store all the Request references */
extern MPI_Request *request;
extern int **listRequestPointer;
extern int *listRequestPointerSize;

#endif /* __SCIMPI */
