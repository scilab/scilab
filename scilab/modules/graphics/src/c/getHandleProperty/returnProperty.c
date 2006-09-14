/*------------------------------------------------------------------------*/
/* file: returnProperty.c                                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : a set of functions used to return values in Scilab              */
/*------------------------------------------------------------------------*/

#include "stack-c.h"
#include <string.h>

/*-----------------------------------------------------------------------------------*/
int sciReturnString( const char * value )
{
  int numRow   = 1 ;
  int numCol   = strlen( value ) ;
  int outIndex = 0 ;
  CreateVar(Rhs+1,"c",&numRow,&numCol,&outIndex);
  strncpy(cstk(outIndex),value, numCol);

  return  0 ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnInt( int value )
{

  int numRow   = 1 ;
  int numCol   = 1 ;
  int outIndex = 0 ;
  CreateVar( Rhs+1, "i", &numRow, &numCol, &outIndex ) ;
  *istk(outIndex) = value ;

  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnRowVector( const double values[], int nbValues )
{
  int numRow   = 1        ;
  int numCol   = nbValues ;
  int outIndex = 0        ;
  int i ;
  CreateVar(Rhs+1,"d",&numRow,&numCol,&outIndex) ;
  for ( i = 0 ; i < nbValues ; i++ )
  {
    stk(outIndex)[i] = values[i] ;
  }
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
