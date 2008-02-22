/*
**  -*- C -*-
**
** GlobalTclInterp.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Thu Feb  7 10:16:24 2008 bruno
** Last update Tue Feb 19 22:48:08 2008 bruno
**
** Copyright INRIA 2008
*/

#ifndef __GLOBALTCLINTERP_H___
#define __GLOBALTCLINTERP_H___

#include "tcl.h"
#include "tk.h"


/*
** Initialize the global interpreter.
*/
void initTclInterp(void);

/*
** Delete the global interpreter.
*/
void deleteTclInterp(void);

/*
** Get the Global Interpreter
*/
Tcl_Interp *getTclInterp(void);

/*
** Release Tcl Interp after use.
*/
void releaseTclInterp(void);

#endif /* !__GLOBALTCLINTERP_H___ */
