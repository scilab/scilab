#include "stack-c.h"
extern int vectsum(int n, double * a, double * b, double * y); 

void sci_sumab(char *fname){  
int l1, m1, n1, l2, m2, n2, l3, n;   
                
/* 1 - Check the number of inputs/outputs arguments  */  
int minlhs=1, maxlhs=1, minrhs=2, maxrhs=2; 
CheckRhs(minrhs,maxrhs) ; 
CheckLhs(minlhs,maxlhs) ; 

/* 2 - Check inputs arguments type, and get the size and the adress in the Scilab stack of the inputs arguments */  
GetRhsVar(1, "d", &m1, &n1, &l1);
GetRhsVar(2, "d", &m2, &n2, &l2);
                
/* 3 - Check that the inputs arguments have the same size */
/* it's possible to use the chekdims and getscalar functions to do these checks*/ 
n=m2*n2; 
if( n1!=n2 || m1!=m2) 
{
        cerro("inputs arguments must have the same size"); 
        return 0; 
}       
if(n1!=0 && m1!=0)    
        if(n1!=1 && m1!=1)   
        {
                cerro("inputs arguments must be vectors");    
                return(0); 
        }
                 
                
/* 4 - Create a new variable corresponding to the output argument */ 
CreateVar(3,"d",&m2,&n2,&l3);   

/* 5 -call vectmab routine to perform a+b */
vectsum(n,stk(l1),stk(l2),stk(l3));  
                
/* 6 - Specif the ouput argument */  
LhsVar(1) = 3;  
return 0;
}