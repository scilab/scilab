#include  "stack-c.h"

/*********************************************************
 * examples of an hand written interface 
 * Shows how to manually decode headers and variables data 
 *  header= GetData(k) returns a pointer (void *) to raw data (header+values) 
 *  associated with variable number k. 
 *  For instance, if k refers to a standard matrix, then GeData(k) will 
 *  return a pointer to a memory zone where four int blocks are used for 
 *  matrix header ([1,m,n,it]) and m*n*(it+1) blocks of double are used
 *  for storing data.
 *  Thus using the two instructions 
 *        int *header = GetData(1) 
 *        double *data = (double *) &header[4];
 *  we can access headers with 
 *  header[0]=1, header[1]=m, header[2]=n, header[3]=it                    
 *  and data with 
 *  data[0]= entry (1,1) of the matrix, data[1]=entry(2,1)  etc, columnwise  
 * 
 *  To create a user-defined object, use CreateData(k,n) where n is the     
 *  byte size of the newly created object with number k.                     
 *  Then fill header = (int *) GetData(k) by your stuff.                    
 *  You can, e.g., define values = (double *) header; and fill values[l+1], 
 *  values[l+2],  by double with l=size of header expressed in bytes.       
 *  header[0]=t defines the type t (integer) of the newly reated object     
 *  At the scilab prompt, use typename(name,t) to associate a name to the   
 *  newly created object, and allows overloaded operations on objects of    
 *  type t.
 *                                                                            
 *  The example below shows how                                             
 *  transforming or creating a short int (1 x 3) matrix            
 *  w=int16([3,5,7]); w1=ex16c(w)  gives w1=int16([33,44,55])      
 *  w1=trial()  creates w1=int16([33,44,55])                       
 *********************************************************/ 
 
int intex16c(fname)
     char* fname;
{ 
  int *header;
  short int *data;
  CheckRhs(0,1);  
  CheckLhs(1,1);
  if (Rhs == 0) 
    CreateData(1, 4*sizeof(int) + 3*sizeof(short int));
  header = GetData(1);
  /* Next line is necessary if Rhs == 0 and redundent if Rhs==1 */
  header[0]=8;  header[1]=1;  header[2]=3;  header[3]=2;  
  /*  header[0]=8 creates on object of type 8  (integer matrix)  */
  data = IC_INT16(&header[4]);  
  data[0]=33; data[1]=44; data[2]=55; 
  LhsVar(1)=1;
  return 0;
}


