#include "mex.h"

void mexFunction(nlhs, plhs, nrhs, prhs)
     int nlhs, nrhs;
     mxArray *plhs[]; mxArray *prhs[];
{
  int ROWS=3;
  int rows;
  int columns;
  static char *STRINGS[3] ={"123456789",
			    "qwertyuiop",
			    "----------"};
  mxArray *array_ptr;
  array_ptr=mxCreateCharMatrixFromStrings(ROWS, STRINGS);
  rows=mxGetM(array_ptr);
  columns=mxGetN(array_ptr);
  mexPrintf("Rows %d; Columns %d\n", rows, columns);
  plhs[0]=array_ptr;
}  

