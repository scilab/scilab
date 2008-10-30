/* This file is released into the public domain */
#include <string>
extern "C" 
{
	#include "stack-c.h"

	int sci_cpp_find(char *fname) 
	{
  	int m1 = 0, n1 = 0, l1 = 0;
  	char *inputString1 = NULL, *inputString2 = NULL;
  	int m2 = 0, n2 = 0, l2;
  	int m3 = 0, n3 = 0;
  	
  	/* Where we will store the position */
  	double *position = NULL; 
  	
  	/* Check the number of input argument */
  	CheckRhs(2,2); 
  	
  	/* Check the number of output argument */
  	CheckLhs(1,1); 
  	
  	/* Retrieve the first input argument */
  	GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1); 
  	inputString1 = cstk(l1);
  	
  	/* Retrieve the second input argument */
  	GetRhsVar(2, STRING_DATATYPE, &m2, &n2, &l2); 

  	inputString2 = cstk(l2);
  	
  	std::string myMessage (inputString1);
  	std::string search (inputString2);
  		
  	m3=1;
  	n3=1;
  	position = new double[1];
  	if (myMessage.find(search) != std::string::npos) 
  	{
    	/* The actual operation */
    	position[0] = myMessage.find(search); 
  	} 
  	else 
  	{
    	position[0] = -1; /* Substring not found */
  	}
  	
  	/* Create the output argument */
  	CreateVarFromPtr(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE ,&m3,&n3,&position); 

  	LhsVar(1) = Rhs+1;
  	delete[] position;
  	return 0;
	}
}
