/*--------------------------------------------------------------------------------*/
/* Copyright INRIA 2006                                                                    */
/* File   : GedManagement.h                                                                */
/* Author : Jean-Baptiste Silvy                                                            */
/* Desc   : C functions to manage ged (only destroy for now)                               */
/*--------------------------------------------------------------------------------*/

#ifndef __GEDMANAGEMENT_H_
#define __GEDMANAGEMENT_H_
/*--------------------------------------------------------------------------------*/
/* close the graphic editor linked with the window figureId */
int sciDestroyGed( int figureId ) ;
/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
/* return true if ged is opened on the window figureId */
int isGedOpenedOn( int figureId ) ;
/*--------------------------------------------------------------------------------*/
#endif /* __GEDMANAGEMENT_H_ */
