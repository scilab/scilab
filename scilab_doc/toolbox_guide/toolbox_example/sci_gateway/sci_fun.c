#include "stack-c.h"

extern void fun1(double * ,  int, int *, double **, double *);

int sci_fun(char *fname)
{
        int la, ma, na, m=1, nx, i, lx, ls;
        double * x, s;

        /* 1 - Check the number of inputs and outputs arguments */
        /* You can use the variables: Lhs and Rhs */
        int minlhs=1, maxlhs=2, minrhs=1, maxrhs=1;
        CheckRhs(minrhs,maxrhs) ;
        CheckLhs(minlhs,maxlhs) ;

        /* 2 - Check the rhs type, get the rows number (ma) and the columns number (na) of rhs, and its adress (la) in the Scilab stack (first position) */
        GetRhsVar(1, "d", &ma, &na, &la); 

        /* 3 - Check rhs is a vector */
        if(ma!=0 && na!=0 )
        {
                if(ma!=1 && na!=1)
                {
                        cerro("input argument must be a vector");
                        return(0);
                }
        }

        fun1(stk(la), na*ma, &nx, &x, &s);

        /* 4 - Create the place for the first output argument x ( a vector of doubles, size: 1*nx ) to the address lx in the Scilab stack (second position) */
        CreateVar(2, "d", &m, &nx, &lx);

        /* if there are two outputs variables then: Create the place for the second output s ( a double, size 1*1) to the adress ls in the Scilab stack (third position) */ 
        /* get the value of s, and put it in the Scilab stack */
        if(Lhs==2)
        {
                CreateVar(3, "d", &m, &m, &ls);
                *stk(ls)=s;
        }

        /* get the components of x, and put them in the Scilab stack */
        for(i=0;i<nx;i++) 
                stk(lx)[i]=x[i];

        /* free memory */
        free(x);

        /* 5 - Specification of outputs variables */
        LhsVar(1) = 2;
        if(Lhs==2)
                LhsVar(2) = 3;
        return 0;
}























