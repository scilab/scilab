/*------------------------------------------------------------------------
*    Graphic library
*    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
*    Copyright INRIA 2006
*    Jean-Baptiste Silvy
--------------------------------------------------------------------------*/

#ifndef _CHAMP_H_
#define _CHAMP_H_

int C2F(champ) (double *x, double *y, double *fx, double *fy, integer *n1, integer *n2, char *strflag, double *brect, double *arfact, integer lstr) ; 
int C2F(champ1) (double *x, double *y, double *fx, double *fy, integer *n1, integer *n2, char *strflag, double *brect, double *arfact, integer lstr) ; 

void champg(char *name, integer colored, double *x, double *y, double *fx, double *fy, integer *n1, 
            integer *n2, char *strflag, double *brect, double *arfact, integer lstr) ;

void getChampDataBounds( double   xCoords[]  ,
                        double   yCoords[]  ,
                        double   xLength[]  ,
                        double   yLength[]  ,
                        int      nbRow      ,
                        int      nbCol      ,
                        int      typeOfChamp,
                        double * xMin       ,
                        double * xMax       ,
                        double * yMin       ,
                        double * yMax        ) ;

void sciChamp2DRealToPixel( integer * xm         ,
                           integer * ym         ,
                           integer * zm         ,
                           integer * na         ,
                           integer * arsize     ,
                           double  * x          ,
                           double  * y          ,
                           double  * fx         ,
                           double  * fy         ,
                           integer * n1         ,
                           integer * n2         ,
                           double  * arfact     ,
                           int     * typeofchamp,
                           BOOL      clipping    ) ;

double computeGridMinGap( double gridX[], double gridY[], int nbRow, int nbCol ) ;

int computeArrowColor( double gridX[], double gridY[], int nbRow, int nbCol, int index ) ;

double getLongestVector( double vectX[], double vectY[], int nbRow, int nbCol, double scx, double scy ) ;

#endif
