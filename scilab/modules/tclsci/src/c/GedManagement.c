/*--------------------------------------------------------------------------------*/
/* Copyright INRIA 2006                                                                    */
/* File   : GedManagement.c                                                                */
/* Author : Jean-Baptiste Silvy                                                            */
/* Desc   : C functions to manage ged (only destroy for now)                               */
/*--------------------------------------------------------------------------------*/

#include "GedManagement.h"
#include "TCL_Global.h"
/*--------------------------------------------------------------------------------*/
/* return the ged interpreter (default interpreter for now) */
static Tcl_Interp * getGedInterpreter( void ) ;
/*--------------------------------------------------------------------------------*/
int sciDestroyGed( int figureId )
{
  Tcl_Interp * gedInterp = getGedInterpreter() ;
  if ( isGedOpenedOn( figureId ) )
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

/*--------------------------------------------------------------------------------*/
int isGedOpenedOn( int figureId )
{
  Tcl_Interp * gedInterp = getGedInterpreter() ;
  if ( gedInterp != NULL )
  {
    /* check is sciGedIsAlive variable exists */
    char * sGedWindowNum = (char * ) Tcl_GetVar( gedInterp, "sciGedIsAlive", TCL_GLOBAL_ONLY ) ;
    if ( sGedWindowNum )
    {
      /* get the number of the window ged modified */
      int iGedWindowNum ;
      sscanf( sGedWindowNum, "%d", &iGedWindowNum ) ;
      /* check if ged is opened on the checked window */
      if ( iGedWindowNum == figureId )
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
      return FALSE ;
    }
  }
  else
  {
    /* tcl has not been initialized yet */
    return FALSE ;
  }
}

/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/

static Tcl_Interp * getGedInterpreter( void )
{
  return TCLinterp ;
}

/*--------------------------------------------------------------------------------*/
