/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: Fec.h                                                            */
/* desc : Functions to interact with the graphic window                   */
/*------------------------------------------------------------------------*/

#ifndef _ACTIONS_H_
#define _ACTIONS_H_

/**
 * 3d rotation function 
 */
int scig_3drot(integer win_num) ;

/**
 * 2D Zoom 
 */
int scig_2dzoom(integer win_num) ;

/**
 * Unzoom function
 */
void scig_unzoom(integer win_num) ;

/**
 * clear window 
 */
void scig_erase(integer win_num) ;

/**
 * graphic Window selection 
 */
void scig_sel(integer win_num) ;

/**
 * Basic Replay : redraw recorded graphics 
 */
void scig_replay(integer win_num) ;

/**
 * Reload a saved graphic
 */
void scig_loadsg(int win_num, char *filename) ;

/* 
 * Basic Replay : expose graphics i.e 
 * if we have a pixmap we can perform a wshow 
 * else we perform a sgig_replay 
 */
void scig_expose(integer win_num) ;

/**
 * Redraw graphic window win_num  after resizing 
 */
void scig_resize(integer win_num) ;

/*
 * graphic Window raise 
 */
void scig_raise(integer win_num) ;

#endif /* _ACTIONS_H_ */
