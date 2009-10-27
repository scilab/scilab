#include <mpi.h>
#include "stack-c.h"

typedef int (*function_loadData)(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, void** _pdblReal);

typedef struct mappinpScilabMPI_struct{
	MPI_Datatype MPI;
	sci_types Scilab;
	void *data;
	int count;  /* Number of elements sent */
	function_loadData f;
} mappinpScilabMPI;
mappinpScilabMPI getMPIDataStructure(int position);
