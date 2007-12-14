/*------------------------------------------------------------------------*/
/* file: sci_xclick.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xclick routine                                    */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xclick.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "CurrentObjectsManagement.h"
#include "gw_graphics.h"
#include "CallJxclick.h"

/*--------------------------------------------------------------------------*/
int sci_xclick(char *fname,unsigned long fname_len)
{
  int one = 1, three = 3, rep = 0;
  int istr;
  //int iflag = 0;

  int mouseButtonNumber = 0;
  double xCoordinate = 0.0;
  double yCoordinate = 0.0;
  int windowID = 0;
  char * menuCallback;

  CheckRhs(-1,1) ;
  CheckLhs(1,5) ;

  //iflag = ( Rhs >= 1) ? 1 :0;

  // Select current figure or create it
  sciGetCurrentFigure();

  // Call Java xclick
  CallJxclick();

  // Get return values
  mouseButtonNumber = getJxclickMouseButtonNumber();
  xCoordinate = getJxclickXCoordinate();
  yCoordinate = getJxclickYCoordinate();
  windowID = getJxclickWindowID();
  menuCallback = getJxclickMenuCallback();

  if (Lhs == 1)
  {
    LhsVar(1) = Rhs+1;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&one,&three,&rep);
    *stk(rep) = (double) mouseButtonNumber;
    *stk(rep + 1) = xCoordinate;
    *stk(rep + 2) = yCoordinate;
  }
  else 
  {
    LhsVar(1) = Rhs+1;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&rep); 
    *stk(rep) = (double) mouseButtonNumber;
  }

  if (Lhs >= 2) 
  { 
    LhsVar(2) = Rhs+2;
    CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&rep);
    *stk(rep) = xCoordinate;
  }
 
  if (Lhs >= 3)
  { 
    LhsVar(3) = Rhs+3;
    CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&rep);
    *stk(rep) = yCoordinate;
  }
  
  if (Lhs >=4) 
  { 
    LhsVar(4) = Rhs+4;
    CreateVar(Rhs+4,MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&rep);
    *stk(rep) = (double) windowID;
  }
  
  if (Lhs >= 5) 
  {
    LhsVar(5) = Rhs+5;
    istr = (int)strlen(menuCallback);
    CreateVar(Rhs+5,STRING_DATATYPE,&istr,&one,&rep); 
    strncpy(cstk(rep),menuCallback,istr);
  }
  return 0;
}
/*--------------------------------------------------------------------------*/
