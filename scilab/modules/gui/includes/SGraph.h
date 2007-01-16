/*--------------------------------------------------------------------------*/
/* Copyright INRIA 2007                                                     */
/* Author : Jean-Baptiste Silvy                                             */
/* Desc: X11 functions for the graphic window management                    */
/*--------------------------------------------------------------------------*/

#include "bcg.h"

/**
 *	Description: used to move the panner and the viewport interactively 
 *                   through scilab command.
 *	Arguments: ScilabXgc : structure associated to a Scilab Graphic window
 *                 x,y : the x,y point of the graphic window to be moved at 
 *                 the up-left position of the viewport
 *	Returns: none.
 */

void SciViewportMove(struct BCG *ScilabXgc, int x, int y) ;

/**
 *	Description: used to get panner position through scilab command.
 *	Arguments: ScilabXgc : structure associated to a Scilab Graphic window
 *                 x,y : the returned position 
 */

void SciViewportGet(struct BCG *ScilabXgc, int *x, int *y) ;
