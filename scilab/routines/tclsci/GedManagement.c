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
  Tcl_Interp * gedInterp = getGedInterpreter() ;
  if ( isGedAlive() )
  {
    /* try to close the ticks editor */
    Tcl_Eval( gedInterp, "catch { destroy .ticks }" ) ;
    /* try to close the editor */
    Tcl_Eval( gedInterp, "catch { destroy .axes }"  ) ;
    return 0 ;
  }
  else
  {
    return 1 ;
  }
}

/*-----------------------------------------------------------------------------------------*/

int isGedAlive( void )
{
  Tcl_Interp * gedInterp = getGedInterpreter() ;
  if ( gedInterp != NULL )
  {
    /* check is sciGedIsAlive variable exists */
    if ( Tcl_GetVar( gedInterp, "sciGedIsAlive", TCL_GLOBAL_ONLY ) )
    {
      return TRUE ;
    }
    else
    {
      return FALSE ;
    }
  }
  else
  {
    /* tcl has not been initialized yet */
    return FALSE ;
  }
}

/*-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------*/

Tcl_Interp * getGedInterpreter( void )
{
  return TCLinterp ;
}

/*-----------------------------------------------------------------------------------------*/
