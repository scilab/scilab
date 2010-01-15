/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - INRIA - Jean-Baptiste SILVY
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------------*/
/* File   : GedManagement.c                                                       */
/* Desc   : C functions to manage ged (only destroy for now)                      */
/*--------------------------------------------------------------------------------*/

#if defined(__linux__)
#define _GNU_SOURCE /* Bug 5673 fix: avoid dependency on GLIBC_2.7 */
#endif

#include "GedManagement.h"
#include "TCL_Global.h"
#include "GlobalTclInterp.h"

/*--------------------------------------------------------------------------------*/
/* return the ged interpreter (default interpreter for now) */
static Tcl_Interp * getGedInterpreter( void );
static void releaseGedInterpreter(void);

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
    releaseGedInterpreter();
    return 0 ;
  }
  else
  {
    releaseGedInterpreter();
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
    releaseGedInterpreter();
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

Tcl_Interp *getGedInterpreter( void )
{
  return getTclInterp();
}

void releaseGedInterpreter(void)
{
  releaseTclInterp();
}
/*--------------------------------------------------------------------------------*/
