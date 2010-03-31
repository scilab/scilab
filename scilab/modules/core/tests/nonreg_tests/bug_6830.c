//-------------------------------------------------------------------------------------
/*
 * Scilab ( http://www.scilab.org/ )
 * Copyright (C) DIGITEO - Allan CORNET - 2009
 * 
 * This file is released into the public domain
 */
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
//-------------------------------------------------------------------------------------
static int PutOneValueOnStack(double v);
//-------------------------------------------------------------------------------------
int sci_callscistring(char *fname) 
{
      int m1 = 0, n1 = 0, l1 = 0;
      double v1 = 0.;

      int one = 1, l = 0;

      int rm1 = 0, rn1 = 0, rl1 = 0;
      double r = 0.;

      int positionFirstElementOnStackForScilabFunction = 0;
      int numberOfRhsOnScilabFunction = 0;
      int numberOfLhsOnScilabFunction = 0;
      int pointerOnScilabFunction = 0;
      char *name = NULL;

      CheckRhs(1,1);
      //CheckLhs(1,1);

      if (GetType(1) != sci_strings)
      {
            Scierror(999,_("%s: Wrong type for input argument #%d: A scilab function expected.\n"),fname,1);
            return 0;
      }

      GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
      name = cstk(l1);


      // creates a variable (double) on stack @ Rhs + 1
      CreateVar(Rhs + 1 , MATRIX_OF_DOUBLE_DATATYPE, &one, &one,&l);
      *stk(l) = 3.0;

      // creates a variable (double) on stack @ Rhs + 2
      CreateVar(Rhs + 2 , MATRIX_OF_DOUBLE_DATATYPE, &one, &one,&l);
      *stk(l) = 2.0;

      positionFirstElementOnStackForScilabFunction = Rhs + 1 ;

      numberOfRhsOnScilabFunction = 2;

      numberOfLhsOnScilabFunction = 1;


      C2F(scistring)(&positionFirstElementOnStackForScilabFunction, name,
       &numberOfLhsOnScilabFunction, &numberOfRhsOnScilabFunction,
            (unsigned long) strlen(name) );

      // result r is now on first position on stack
      GetRhsVar(Rhs + 1 , MATRIX_OF_DOUBLE_DATATYPE, &rm1, &rn1, &rl1);
      r = *stk(rl1);
      printf("r = %d\n",r);
      PutOneValueOnStack(r);

      return 0;
}
//-------------------------------------------------------------------------------------
static int PutOneValueOnStack(double v)
{
      int m = 1,n = 1, l = 0;

      CreateVar( Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &l );
      *stk(l) = v;
      LhsVar(1) = Rhs + 1;
      C2F(putlhsvar)();
      return 0;
}
//-------------------------------------------------------------------------------------
