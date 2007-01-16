/*------------------------------------------------------------------------*/
/* file: Fec.h                                                            */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
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
