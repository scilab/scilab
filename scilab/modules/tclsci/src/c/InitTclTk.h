/*--------------------------------------------------------------------------*/
/* INRIA 2005/2007 */
/* @author Allan CORNET */
/* @author Sylvestre Ledru <sylvestre.ledru@inria.fr> */
/*--------------------------------------------------------------------------*/ 
#ifndef __INITTCLTK__
#define __INITTCLTK__

/*#include "TCL_Global.h"*/
#include "BOOL.h"

/**
 * Initialize TCL/TK
 * @return if started (TRUE) or not (FALSE)
 */
BOOL initTCLTK(void);

/**
 * @TODO add comment
 *
 * @return <ReturnValue>
 */
int OpenTCLsci(void);

/**
 * TODO : comment 
 * @return
 */
BOOL CloseTCLsci(void);

/**
 * Set if tcl/tk is started or not
 * @param isTkSet if enable or not
 */
void setTkStarted(BOOL isTkSet);

/**
 * Set if tcl/tk is started or not
 * @return says if TCL/TK is started (TRUE) or not (FALSE)
 */
BOOL isTkStarted(void);

#endif /* __INITTCLTK__ */
/*--------------------------------------------------------------------------*/ 
