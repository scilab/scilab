#include  "stack-c.h"

/*   
Example of ND Array in Scilab (here 3 dimensions).  
FD   for Mengzhen (Jan-2006) and digiplantes...
usage: w=ex19c()
returns a 2x2x2 array filled with entries taken from values.

*/
#define DIM 32
#define VAL  2+(3+header[4])-1
#define HEAD  ((3+header[4]) + (3+header[4]) -1) -1
#define SIZE_DIMS  4 + round( (ndim-3)/2)
#define SIZE_VALUES  2 + prod

/*********************************************************
  ndim = # of dimensions
  dims = int array of dimensions, dimension ndim
  prod = product of dimensions = dims[0]*dims[1]*dims[2] (int)
  values = double array of values, dimension prod
 *********************************************************/ 
 
int intex19c(fname)
     char* fname;
{ 
#define  FIRST_DIM  2
#define  SECOND_DIM 2
#define  THIRD_DIM  2
#define  TOTAL_ELEMENTS (FIRST_DIM * SECOND_DIM * THIRD_DIM)
  static  int   ndim = 3, dims[3] = {FIRST_DIM, SECOND_DIM, THIRD_DIM};
  static double values[8] = {1,2,3,4,5,6,7,8};


  int *header;  double *data;
  int i,prod;

  static int template[32]={
  17,3,1,12,0,0,10,1,3,0,1,3,7,14,17,22,13,18,22,28,14,23,29,27,18,14,28,
  23,8,1,0,4};

  CheckRhs(0,1); CheckLhs(1,1);
  /*   1: set ndim and prod here:   */
  ndim=3;
  prod=TOTAL_ELEMENTS;     
  /************************************/

  CreateData(1, (33+ndim)*sizeof(int) + (2+prod)*sizeof(double));   /*  don't change! */
  header = GetData(1); data = (double *) &header[0];  
  for ( i = 0 ; i < DIM ; i++ )
    header[i]=template[i];
  
  header[4]=header[3] + SIZE_DIMS;
  header[5]=header[4] + SIZE_VALUES;
  header[30]=ndim;

  /*   entries = real (prod x 1) vector */ 
  header[HEAD+0]=1;
  header[HEAD+1]=prod;
  header[HEAD+2]=1;
  header[HEAD+3]=0;

  /*   2: fill with dimensions  */
  header[DIM+0]=  FIRST_DIM; 
  header[DIM+1]=  SECOND_DIM; 
  header[DIM+2]=  THIRD_DIM;   /*  etc ...*/ 
 /************************************/
    /*  3: fill with values  */
   for ( i = 0 ; i < prod ; i++ )
    data[VAL+i] =  values[i];
 /************************************/

  LhsVar(1)=1;
  return 0;
}

