/*-----------------------------------------------------------------------------------------*/
/* Copyright INRIA 2006                                                                    */
/* File   : GedManagement.c                                                                */
/* Author : Jean-Baptiste Silvy                                                            */
/* Desc   : C functions to manage ged (only destroy for now)                               */
/*-----------------------------------------------------------------------------------------*/

#include "GedManagement.h"

/*-----------------------------------------------------------------------------------------*/

int sciDestroyGed( void )
{
  if ( TCLinterp == NULL )
  {
    Scierror( 999, TCL_ERROR13, "sciDestroyGed" ) ;
    return 1 ;
  }
  else
  {
    /* try to close the ticks editor */
    Tcl_Eval( TCLinterp, "catch { destroy .ticks }" ) ;
    /* try to close the editor */
    Tcl_Eval( TCLinterp, "catch { destroy .axes }"  ) ;
    return 0 ;
  }
}
/*------------------------------------------------------------------------------------------*/
