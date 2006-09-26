/*------------------------------------------------------------------------*/
/* file: getPropertyAssignedValue.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : a set of functions used to get the values which will be         */
/*        assigned to handles properties from the stack                   */
/*------------------------------------------------------------------------*/


#include "getPropertyAssignedValue.h"
#include "stack-c.h"

/*-----------------------------------------------------------------------------------*/
BOOL isParameterHandle( int paramNum )
{
  return ( VarType( paramNum ) == 9 ) ;
}
/*-----------------------------------------------------------------------------------*/
BOOL isParameterDoubleMatrix( int paramNum )
{
  return ( VarType( paramNum ) == 1 ) ;
}
/*-----------------------------------------------------------------------------------*/
double getDoubleFromStack( int stackPointer )
{
  return *(stk( stackPointer ));
}
/*-----------------------------------------------------------------------------------*/
double * getDoubleMatrixFromStack( int stackPointer )
{
  return stk( stackPointer ) ;
}
/*-----------------------------------------------------------------------------------*/
char * getStringFromStack( int stackPointer )
{
  return cstk( stackPointer ) ;
}
/*-----------------------------------------------------------------------------------*/
unsigned long getHandleFromStack( int stackPointer )
{
  return (unsigned long) *(hstk( stackPointer )) ;
}
/*-----------------------------------------------------------------------------------*/
BOOL isStringParamEqual( int stackPointer, const char * str )
{
  if ( strcmp( getStringFromStack( stackPointer ), str ) == 0 )
  {
    return TRUE ;
  }
  else
  {
    return FALSE ;
  }
  return FALSE ;
}
/*-----------------------------------------------------------------------------------*/
