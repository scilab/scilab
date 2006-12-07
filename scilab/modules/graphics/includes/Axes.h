/*======================================================================================*/
/* Copyrigth INRIA 2006                                                                 */
/* File   : Axes.h                                                                      */
/* Author : Jean-Baptiste Silvy INRIA                                                   */
/* Desc   : Various functions to manipulate SubWindow objects                           */
/*======================================================================================*/

#ifndef __SCI_AXES_H__
#define __SCI_AXES_H__

#include "ObjectStructure.h"

/*--------------------------------------------------------------------------------------*/
/* check if the graduation text foo is inside the old_rect */
/* in this case it should not been displayed */
BOOL CheckDisplay( double fact_h, double fact_w, char logflag, char *foo,int *posi,int *fontid,int *old_rect) ;

/* The unit is the pixel */
/* return 0 if the point is inside the rect */
/* 1 if it is actually outside the rect */
/* (rect[0],rect[1]) : upper left point of the bounding box  in pixel */
/* (rect[2],rect[3]) : width and height in pixel */
/* point[0] : x component */
/* point[1] : y component */
BOOL IsInsideRectangle(int * rect, int *point) ;


/*---------------------------------------------------------------------
 *Trace l'enveloppe convexe de la boite contenant le dessin 
 * et renvoit dans InsideU et InsideD les indices des points dans xbox et ybox
 * qui sont sur les 2 tri\`edres a l'interieur de l'enveloppe convexe
 *---------------------------------------------------------------------*/
void Convex3d_Box(double *xbox, double *ybox, integer *InsideU, integer *InsideD, char *legend, integer *flag, double *bbox);

/* get the bounding rectangle of a label of a ticks */
/* similar to stringBoundingRect but only for ticks labels */
void getTicksLabelBox( double   fact_h   ,
                       double   fact_w   , 
                       char   * label    ,
                       int      pos[2]   ,
                       int      fontId[2],
                       char     logflag  ,
                       int      bbox[4]  ,
                       BOOL     changeFont ) ;

/* remove the concealed ticks before display on the X and Y axis */
BOOL checkXAxes2dTics( sciPointObj * psubwin  ,
                       char          side     ,
                       double        y        ,
                       char        * cFormat  ,
                       int           fontId[2],
                       int           smallersize ) ;

/* remove the concealed ticks before display on the X and Y axis */
BOOL checkYAxes2dTics( sciPointObj * psubwin  ,
                       char          side     ,
                       double        x        ,
                       char        * cFormat  ,
                       int           fontId[2],
                       int           smallersize ) ;


void updateXaxesTics( sciPointObj * psubwin  ,
                      char          side     ,
                      double        y        ,
                      int           fontId[2],
                      int           smallersize ) ;

void updateYaxesTics( sciPointObj * psubwin  ,
                      char          side     ,
                      double        x        ,
                      int           fontId[2],
                      int           smallersize ) ;

/* remove in the ticks array the indices i such as removedTicks[i] */
/* is true. The value nbtics is an in-out variable */
void removeBadTicks( double * ticks, BOOL * removedTicks, int * nbTicks ) ;

/* remove an element in the array from translating the next
   elements on step backward */
void removeIndex( double * changedArray, int size, int ind ) ;

/* compute the needed formats to display the ticks of a subWin */
void findFormat( sciPointObj * pSubWin, char formatX[5], char formatY[5] ) ;

/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/

/* clear a subwindow from all of its children */
void clearSubWin( sciPointObj * pSubWin ) ;

/* reinit a subwindow (but don't change position) */
void reinitSubWin( sciPointObj * pSubWin ) ;

/* reinit the viewing angles of a subwindow */
void initSubWinAngles( sciPointObj * pSubWin ) ;

/* set the size and position of the subwindow to the default */
void initSubWinSize( sciPointObj * pSubWin ) ;

/* set the data_bounds of the axes to the default value */
void initSubWinBounds( sciPointObj * pSubWin ) ;

/* reinit the selected subwindow if the auto_clear property is set to on */
/* return TRUE if the window has been redrawn */
BOOL checkRedrawing( void ) ;

/*--------------------------------------------------------------------------------------*/
/**
* Update the current scale factors from a modified SubWindow.
* This is sometimes required when the redraw is disable.
*/
void updateSubWinScale( sciPointObj * pSubWin ) ;

/**
* update the scale factors only if the SubWindow won't be automatically redrawn.
* Should be called before dr("xstringl") for example.
*/
void updateScaleIfRequired( sciPointObj * pSubWin ) ;
/*--------------------------------------------------------------------------------------*/
/**
 * Draw the grid of an axes object if needed.
 */
void drawAxesGrid( sciPointObj * psubwin ) ;
/*--------------------------------------------------------------------------------------*/

#endif /*__SCI_AXES_H_*/
