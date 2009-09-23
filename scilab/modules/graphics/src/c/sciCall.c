/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library
 *    Graphic subroutines interface
 --------------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "SetProperty.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "BasicAlgos.h"
#include "math_graphics.h"
#include "PloEch.h"
#include "Axes.h"
#include "Fec.h"
#include "GrayPlot.h"
#include "CurrentObjectsManagement.h"
#include "GraphicSynchronizerInterface.h"
#include "localization.h"
#include "MALLOC.h" /* MALLOC */
#include "Scierror.h"
#include "sciCall.h"
#include "DrawingBridge.h"
#include "Plot2d.h"
#include "HandleManagement.h"

/**
 * Put min and max of vector in dMin and dMax.
 * If no min and max can't be found (no finite data in dMin or dMax),
 * then use the default values
 */
static void getDrect(const double vector[], int nbElements,
										 double * dMin, double * dMax,
										 double defaultMin, double defaultMax);
/*------------------------------------------------
 * Objrect :
 * On recupere la figure courante, puis on recupere la sous fenetre qui y est selectionnee
 *   puis on contruit le rectangle, qu on le place comme objet courant
 * ensuite il reste qu'appeler la fonction du dessin de l'objet
 *-----------------------------------------------*/

void Objrect ( double * x         ,
               double * y         ,
               double * width     ,
               double * height    ,
               int    * foreground,
               int    * background,
               BOOL     isfilled  ,
               BOOL     isline    ,
               long   * hdl       )
{
  sciPointObj * newObj = NULL ;
  sciPointObj *psubwin;
  sciPointObj * pFigure = sciGetCurrentFigure();

  psubwin = sciGetCurrentSubWin();
  /* check if the auto_clear property is on and then erase everything */
  checkRedrawing() ;
  newObj = ConstructRectangle(psubwin ,*x,*y,*height, *width,
                              foreground, background, isfilled, isline) ;

  if ( newObj == NULL )
  {
    endFigureDataWriting(pFigure);
    /* an error occured */
    *hdl = -1 ;
    return ;
  }
  sciSetCurrentObj( newObj ) ;
  *hdl=sciGetHandle( newObj ) ;

}


/*----------------------------------------------
 * Objarc :
 *-----------------------------------------------*/

void Objarc( double * angle1    ,
             double * angle2    ,
             double * x         ,
             double * y         ,
             double * width     ,
             double * height    ,
             int    * foreground,
             int    * background,
             BOOL     isfilled  ,
             BOOL     isline    ,
             long   * hdl        )
{
  sciPointObj * psubwin;
  sciPointObj * pobj;

  psubwin = sciGetCurrentSubWin() ;
  checkRedrawing() ;
  pobj = ConstructArc(psubwin,*x,*y,
	              *height, *width, *angle1, *angle2, foreground, background, isfilled, isline);
  sciSetCurrentObj(pobj);

  *hdl=sciGetHandle(pobj);
}

/*------------------------------------------------
 * Objpoly :
 *-----------------------------------------------*/

void Objpoly ( double  * x     ,
               double  * y     ,
               int   n     ,
               int   closed,
               int       mark  ,
               long    * hdl    )
{
  sciPointObj * pFigure = NULL;
  sciPointObj * psubwin;
  sciPointObj * pobj;

  pFigure = sciGetCurrentFigure();
  psubwin = sciGetCurrentSubWin();


  checkRedrawing() ;
  if (mark <= 0)
    {
      int absmark = abs(mark);
      pobj = ConstructPolyline(psubwin,x,y,PD0,closed,n,1,
	         	       NULL,NULL,&absmark,NULL,NULL,FALSE,FALSE,TRUE,FALSE);
    }
  else
    {
      pobj = ConstructPolyline(psubwin,x,y,PD0,closed,n,1,
	                       &mark,NULL,NULL,NULL,NULL,TRUE,FALSE,FALSE,FALSE);
    }
  sciSetCurrentObj(pobj);
   *hdl=sciGetHandle(pobj);
}


/*------------------------------------------------
 * Objfpoly :
 *-----------------------------------------------*/

void Objfpoly ( double  * x    ,
                double  * y    ,
                int   n    ,
                int * style,
                long    * hdl  ,
                int   shading )
{
  int fillcolor, contourcolor;
  sciPointObj *psubwin, *pobj;
  int closed = 1; /* we close the polyline by default */
  psubwin = sciGetCurrentSubWin();

  checkRedrawing() ;

  if(shading == 2)
    {
      /* interpolated shading is "on" */
      pobj = ConstructPolyline(psubwin,x,y,PD0,closed,n,
	 		       1,NULL,style,NULL,NULL,NULL,FALSE,TRUE,FALSE,TRUE);
    }
  else
    {
      /* flat mode is "on" */
      if (*style < 0){
	fillcolor = abs(*style);
	pobj = ConstructPolyline(psubwin,x,y,PD0,closed,n,
			         1,NULL,&fillcolor,NULL,NULL,NULL,FALSE,TRUE,FALSE,FALSE);
      }
      else if (*style == 0){
	contourcolor = sciGetForeground(psubwin);
	pobj = ConstructPolyline(psubwin,x,y,PD0,closed,n,
	                         1,&contourcolor,NULL,NULL,NULL,NULL,TRUE,FALSE,FALSE,FALSE);
      }
      else{ /* *style > 0*/
	fillcolor = *style;
	contourcolor = sciGetForeground(psubwin);
	pobj = ConstructPolyline(psubwin,x,y,PD0,closed,n,
	                         1,&contourcolor,&fillcolor,NULL,NULL,NULL,TRUE,TRUE,FALSE,FALSE);
      }

    }
    sciSetCurrentObj(pobj);
    *hdl=sciGetHandle(pobj);

}


/*-----------------------------------------------------------
 *   Objsegs :
 *-----------------------------------------------------------*/
void Objsegs ( int * style,
               int   flag ,
               int   n1   ,
               double  * x    ,
               double  * y    ,
               double  * z    ,
               double    arsize )
{
  int type=0, colored=0;
  double *fx =NULL,*fy = NULL; // No fx or fy
  int typeofchamp = -1; /* no champ here, only segs ; this info is useless */
  sciPointObj *psubwin = NULL;

  checkRedrawing() ;
  psubwin = sciGetCurrentSubWin();
  sciSetCurrentObj(
    ConstructSegs(psubwin,type,
                  x,y,z,n1,n1, (z==NULL ? 0 : n1),     // x, y and z have the same size n1
                  fx,fy,flag,style,arsize,colored,typeofchamp));
}
/*-----------------------------------------------------------
 * Objstring:
 *-----------------------------------------------------------*/

/* box is an OUTPUT re-used inside matdes.c in scixstring */
void Objstring( char            ** fname      ,
                int                nbRow      ,
                int                nbCol      ,
                double             x          ,
                double             y          ,
                double           * angle      ,
                double             box[4]     ,
                BOOL               autoSize   ,
                double             userSize[2],
                long             * hdl        ,
                BOOL               centerPos  ,
                int              * foreground ,
                int              * background ,
                BOOL               isboxed    ,
                BOOL               isline     ,
                BOOL               isfilled   ,
                sciTextAlignment   alignment   )
{
  sciPointObj * psubwin = NULL;
  sciPointObj * pobj = NULL;
  sciPointObj * pFigure = NULL;

  startGraphicDataWriting();
  pFigure = sciGetCurrentFigure();
  psubwin = sciGetCurrentSubWin();
  endGraphicDataWriting();

  checkRedrawing() ;

  startFigureDataWriting(pFigure);
  sciSetCurrentObj( ConstructText( psubwin   ,
                                   fname     ,
                                   nbRow     ,
                                   nbCol     ,
                                   x         ,
                                   y         ,
                                   autoSize  ,
                                   userSize  ,
                                   centerPos ,
                                   foreground,
                                   background,
                                   isboxed   ,
                                   isline    ,
                                   isfilled  ,
                                   alignment  ) ) ;
  endFigureDataWriting(pFigure);

  startFigureDataReading(pFigure);
  pobj=sciGetCurrentObj ();
  *hdl= sciGetHandle(pobj);
  sciSetFontOrientation (pobj, *angle);

  sciDrawObj(pobj);
  endFigureDataReading(pFigure);

}


/*------------------------------------------------
 *  plot2d
 *-----------------------------------------------*/

void Objplot2d ( int       ptype     ,
                 char      logflags[],
                 double    x[]       ,
                 double    y[]       ,
                 int * n1        ,
                 int * n2        ,
                 int   style[]   ,
                 char      strflag[] ,
                 char      legend[]  ,
                 double    brect[]   ,
                 int   aaint[]   ,
                 BOOL      flagNax    )
{
  plot2dn(ptype,logflags,x,y,n1,n2,style,strflag,legend,brect,aaint,flagNax,4L,bsiz);
}

/*------------------------------------------------
 *  grayplot
 *-----------------------------------------------*/
void Objgrayplot ( double    x[]      ,
                   double    y[]      ,
                   double    z[]      ,
                   int * n1       ,
                   int * n2       ,
                   char      strflag[],
                   double    brect[]  ,
                   int   aaint[]  ,
                   BOOL      flagNax   )
{
  C2F(xgray)(x,y,z,n1,n2,strflag, brect, aaint, flagNax, bsiz );
}

/*------------------------------------------------
 *  Matplot
 *-----------------------------------------------*/
void Objmatplot (double    z[]      ,
                 int * n1       ,
                 int * n2       ,
                 char      strflag[],
                 double    brect[]  ,
                 int    aaint[]  ,
                 BOOL      flagNax   )
{
  C2F(xgray1)(z,n1,n2,strflag, brect, aaint, flagNax, bsiz);
}

/*------------------------------------------------
 *  Matplot1
 *-----------------------------------------------*/
void Objmatplot1 ( double    z[],
                   int * n1 ,
                   int * n2 ,
                   double    xrect[] )
{
  C2F(xgray2)(z, n1, n2,xrect);
}

/*------------------------------------------------
 *  plot3d
 *-----------------------------------------------*/
void Objplot3d ( char    * fname ,
                 int * isfac ,
                 int * izcol ,
                 double    x[]   ,
                 double    y[]   ,
                 double    z[]   ,
                 double  * zcol  ,
                 int * m     ,
                 int * n     ,
                 double  * theta ,
                 double  * alpha ,
                 char    * legend,
                 int * iflag ,
                 double  * ebox  ,
                 int * m1    , /*Adding F.Leray 12.03.04 and 19.03.04*/
                 int * n1    ,
                 int * m2    ,
                 int * n2    ,
                 int * m3    ,
                 int * n3    ,
                 int * m3n   ,
                 int * n3n    )
     /* F.Leray 25.04.05 : warning here legends means "X@Y@Z": it is labels writings!! */
     /* legends has not the same meaning than inside plot2dn (there, it is really the legends of the plotted curves)*/
{
	sciTypeOf3D typeof3d;
	int flagcolor;
	long *hdltab;
	int i;
	sciPointObj *psubwin = NULL, *pobj = NULL;
	sciPointObj * parentFigure = NULL;
	double drect[6];
	char * loc = NULL;
	char * legx = NULL;
	char * legy = NULL;
	char * legz = NULL;
	/*   char * buff = NULL; */
	int flag_x = 1;
	int flag_y = 1;
	int dimvectx = -1;
	int dimvecty = -1;
	char linLogFlags[3] = {'n','n','n'};

	sciPointObj  * pNewSurface = NULL ;
	sciSubWindow * ppsubwin = NULL;
	BOOL bounds_changed = FALSE; /* cannot be used here because we have to force redrawing since there is no way to avoid merge (=> complete redraaw) */

	/* Initialisation drect A.C pour debuggueur */
	drect[0] = 0.0;
	drect[1] = 0.0;
	drect[2] = 0.0;
	drect[3] = 0.0;
	drect[4] = 0.0;
	drect[5] = 0.0;

	/* =================================================
	* Force SubWindow properties according to arguments
	* ================================================= */
	startGraphicDataWriting();
	parentFigure = sciGetCurrentFigure();
	psubwin= sciGetCurrentSubWin();
	endGraphicDataWriting();

	startFigureDataWriting(parentFigure);
	checkRedrawing() ;

	/* Force psubwin->is3d to TRUE: we are in 3D mode */
	pSUBWIN_FEATURE (psubwin)->is3d = TRUE;
	pSUBWIN_FEATURE (psubwin)->project[2] = 1; /* to force z axis display */

	ppsubwin = pSUBWIN_FEATURE(psubwin);

	if ( legend != NULL )
	{
		/* F.Leray 25.04.05 replace the default labels by the user labels if specified */
		loc=(char *) MALLOC( (strlen(legend)+1)*sizeof(char));
		if ( loc == NULL) {
			Scierror(999, _("%s: No more memory.\n"),"Objplot3d");
		}

		strcpy(loc,legend);

		/*   legx=strtok_r(loc,"@",&buff); */
		legx=strtok(loc,"@");

		if (legx != NULL)
		{
			sciSetText( ppsubwin->mon_x_label, &legx , 1, 1 ) ;
		}

		/*   legy=strtok_r((char *)0,"@",&buff); */
		legy=strtok((char *)NULL,"@"); /* NULL to begin at the last read character */
		if ( legy != NULL )
		{
			sciSetText( ppsubwin->mon_y_label, &legy , 1, 1 ) ;
		}

		/*   legz=strtok_r((char *)0,"@",&buff); */
		legz=strtok((char *)NULL,"@");
		if ( legz != NULL )
		{
			sciSetText( ppsubwin->mon_z_label, &legz , 1, 1 ) ;
		}
	}

	/* Force psubwin->logflags to linear */
	sciSetLogFlags(psubwin, linLogFlags);


	pSUBWIN_FEATURE (psubwin)->axes.flag[0] = iflag[0]; /* mode: treatment of hidden parts */
	if (iflag[1] != 0){
		if (iflag[1]<7)
			pSUBWIN_FEATURE (psubwin)->axes.flag[1] = iflag[1]; /* type: scaling (no more useful)  */
		else
			pSUBWIN_FEATURE (psubwin)->axes.flag[1] = iflag[1]-6; /* type: scaling (no more useful)  */
	}

	if( pSUBWIN_FEATURE (psubwin)->FirstPlot == FALSE && (iflag[2] == 0 || iflag[2] == 1))
	{
		/* Nothing to do: we leave as before */
	}
	else
	{
		pSUBWIN_FEATURE (psubwin)->axes.flag[2] = iflag[2]; /* box: frame around the plot      */

		if(iflag[2] == 0 || iflag[2] == 1)
		{

			if (pSUBWIN_FEATURE(psubwin)->FirstPlot)
			{
				pSUBWIN_FEATURE (psubwin)->axes.axes_visible[0] = FALSE;
				pSUBWIN_FEATURE (psubwin)->axes.axes_visible[1] = FALSE;
				pSUBWIN_FEATURE (psubwin)->axes.axes_visible[2] = FALSE;
				pSUBWIN_FEATURE (psubwin)->axes.rect = BT_OFF ;

				sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_x_label,FALSE);
				sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_y_label,FALSE);
				sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_z_label,FALSE);
			}
			/*else no changes : the axes visible properties are driven by the previous plot */
		}
		else if(iflag[2] == 2)
		{
			pSUBWIN_FEATURE (psubwin)->axes.rect = BT_HIDDEN_AXES ; /* for 2d use only (when switching to 2d mode) */

			pSUBWIN_FEATURE (psubwin)->axes.axes_visible[0] = FALSE;
			pSUBWIN_FEATURE (psubwin)->axes.axes_visible[1] = FALSE;
			pSUBWIN_FEATURE (psubwin)->axes.axes_visible[2] = FALSE;

			sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_x_label,FALSE);
			sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_y_label,FALSE);
			sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_z_label,FALSE);
		}
		else if(iflag[2] == 3)
		{
			pSUBWIN_FEATURE (psubwin)->axes.rect = BT_ON ; /* for 2d use only (when switching to 2d mode) */

			pSUBWIN_FEATURE (psubwin)->axes.axes_visible[0] = FALSE;
			pSUBWIN_FEATURE (psubwin)->axes.axes_visible[1] = FALSE;
			pSUBWIN_FEATURE (psubwin)->axes.axes_visible[2] = FALSE;

			sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_x_label,TRUE);
			sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_y_label,TRUE);
			sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_z_label,TRUE);
		}
		else if(iflag[2] == 4)
		{
			pSUBWIN_FEATURE (psubwin)->axes.rect = BT_ON ;

			pSUBWIN_FEATURE (psubwin)->axes.axes_visible[0] = TRUE;
			pSUBWIN_FEATURE (psubwin)->axes.axes_visible[1] = TRUE;
			pSUBWIN_FEATURE (psubwin)->axes.axes_visible[2] = TRUE;

			sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_x_label,TRUE);
			sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_y_label,TRUE);
			sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_z_label,TRUE);
		}
	}

	pSUBWIN_FEATURE (psubwin)->alpha  = *alpha;
	pSUBWIN_FEATURE (psubwin)->theta  = *theta;

	if ((sciGetGraphicMode (psubwin)->autoscaling)) {
		/* compute and merge new specified bounds with psubwin->Srect */
		switch (iflag[1])  {
		case 0:  /* do not change psubwin->Srect */
			break;
		case 1 : case 3 : case 5 : case 7 : /* Force psubwin->Srect=ebox */
			drect[0] = ebox[0]; /*xmin*/
			drect[2] = ebox[2]; /*ymin*/
			drect[1] = ebox[1]; /*xmax*/
			drect[3] = ebox[3]; /*ymax*/
			drect[4] = ebox[4]; /*zmin*/
			drect[5] = ebox[5]; /*zmax*/
			break;
		case 2 : case 4 : case 6 : case 8:/* Force psubwin->Srect to the x and y bounds */
			getDrect(x, (*m1) * (*n1), &drect[0], &drect[1],
					     pSUBWIN_FEATURE(psubwin)->SRect[0], pSUBWIN_FEATURE(psubwin)->SRect[1]);

			getDrect(y, (*m2) * (*n2), &drect[2], &drect[3],
					     pSUBWIN_FEATURE(psubwin)->SRect[2], pSUBWIN_FEATURE(psubwin)->SRect[3]);

			getDrect(z, (*m3) * (*n3), &drect[4], &drect[5],
					     pSUBWIN_FEATURE(psubwin)->SRect[4], pSUBWIN_FEATURE(psubwin)->SRect[5]);
			
			break;
		}
		if (!pSUBWIN_FEATURE(psubwin)->FirstPlot ) { /* merge psubwin->Srect and drect */
			drect[0] = Min(pSUBWIN_FEATURE(psubwin)->SRect[0],drect[0]); /*xmin*/
			drect[1] = Max(pSUBWIN_FEATURE(psubwin)->SRect[1],drect[1]); /*xmax*/
			drect[2] = Min(pSUBWIN_FEATURE(psubwin)->SRect[2],drect[2]); /*ymin*/
			drect[3] = Max(pSUBWIN_FEATURE(psubwin)->SRect[3],drect[3]); /*ymax*/
			drect[4] = Min(pSUBWIN_FEATURE(psubwin)->SRect[4],drect[4]); /*zmin*/
			drect[5] = Max(pSUBWIN_FEATURE(psubwin)->SRect[5],drect[5]); /*zmax*/
		}
		if (iflag[1] != 0)
		{
			bounds_changed = update_specification_bounds(psubwin, drect,3);
		}
	}

	if(iflag[1] != 0)
		pSUBWIN_FEATURE(psubwin)->isoview = (BOOL)( iflag[1] == 3 || iflag[1] == 4 ||
		iflag[1] == 5 || iflag[1] == 6);

	/* =================================================
	* Analyze arguments to find entity type
	* ================================================= */

	if (*isfac== 1) {
		if (*izcol == 0) {
			if (strcmp(fname,"plot3d1")==0) {typeof3d = SCI_FAC3D;   flagcolor=1;}
			else                               {typeof3d = SCI_FAC3D;   flagcolor=0;}
		}
		else if (*izcol == 2)                {typeof3d = SCI_FAC3D;   flagcolor=3;}
		else                                 {typeof3d = SCI_FAC3D;   flagcolor=2;}
	}
	else  if  (*isfac== 0) {
		if (strcmp(fname,"plot3d1")==0)   {typeof3d = SCI_PLOT3D;  flagcolor=1;}
		else                                 {typeof3d = SCI_PLOT3D;  flagcolor=0;}
	}
	else                                   {typeof3d = SCI_PARAM3D1;flagcolor=1;}


	/* =================================================
	* Construct the Entities
	* ================================================= */

	if ( typeof3d != SCI_PARAM3D1 ) {/*Distinction here between SCI_PARAM3D1 and others*/


		if ( *isfac == 1 )
		{
			/* x is considered as a matrix */
			dimvectx = -1 ;
		}
		else if ( *m1 == 1 ) /* x is a row vector */
		{
			dimvectx = *n1;
		}
		else if ( *n1 == 1 ) /* x is a column vector */
		{
			dimvectx = *m1;
		}
		else /* x is a matrix */
		{
			dimvectx = -1;
		}

		if ( dimvectx > 1 )
		{
			int monotony = checkMonotony( x, dimvectx ) ;
			if ( monotony == 0 )
			{
				endFigureDataWriting(parentFigure);
				Scierror(999, _("%s: x vector is not monotonous.\n"),"Objplot3d");
				return;
			}

			flag_x = monotony ;
		}

		if ( *isfac == 1 )
		{
			/* y is considered as a matrix */
			dimvecty = -1 ;
		}
		else if ( *m2 == 1 ) /* y is a row vector */
		{
			dimvecty = *n2 ;
		}
		else if ( *n2 == 1 ) /* y is a column vector */
		{
			dimvecty = *m2 ;
		}
		else /* y is a matrix */
		{
			dimvecty = -1 ;
		}

		if( dimvecty > 1 )
		{
			/* test the monotony on y*/
			int monotony = checkMonotony( y, dimvecty ) ;
			if ( monotony == 0 )
			{
				endFigureDataWriting(parentFigure);
				Scierror(999, _("%s: x vector is not monotonous.\n"),"Objplot3d");
				return ;
			}

			flag_y = monotony ;
		}

		pNewSurface = ConstructSurface( psubwin, typeof3d,
			x, y, z, zcol, *izcol, *m, *n, iflag,ebox,flagcolor,
			isfac,m1,n1,m2,n2,m3,n3,m3n,n3n) ;

		if ( pNewSurface == NULL )
		{
			endFigureDataWriting(parentFigure);
			return ;
		}

		sciSetCurrentObj( pNewSurface ) ;

		pSURFACE_FEATURE (pNewSurface)->flag_x = flag_x;
		pSURFACE_FEATURE (pNewSurface)->flag_y = flag_y;

		/* Force clipping */
		sciSetIsClipping(pNewSurface, 0);  /* clipgrf */


	}
	else
	{
		if ((hdltab = MALLOC (*n * sizeof (long))) == NULL) {
			endFigureDataWriting(parentFigure);
			Scierror(999,"%s: No more memory.\n",fname);
			return;
		}
		for (i = 0; i < *n; ++i) {
			/* F.Leray Pb here: In fact we do not create a Surface but one or several 3D Polylines
			Pb comes when wanting to access the fields "surface_color" or "flag" for example
			in function sciSet (cf. matdes.c).
			Question 1: Are these properties accessible from a SCI_PARAM3D1 ?
			Question 2: Is "flag" obsolete and replaced by "color_mode"?*/

			if ((*n > 0) && (zcol != (double *)NULL)) {
				if ((int) zcol[i] > 0){
					int intzcol = (int) zcol[i];
					sciSetCurrentObj (ConstructPolyline
						(sciGetCurrentSubWin(),
						&(x[*m * i]),&(y[*m * i]),&(z[*m * i]),0,*m,1,
						&intzcol,NULL,NULL,NULL,NULL,TRUE,FALSE,FALSE,FALSE));
				}
				else {
					int intzcol = (int) -zcol[i];
					sciSetCurrentObj (ConstructPolyline
						(sciGetCurrentSubWin(),
						&(x[*m * i]),&(y[*m * i]),&(z[*m * i]),0,*m,1,
						NULL,NULL,&intzcol,NULL,NULL,FALSE,FALSE,TRUE,FALSE));
				}
			}
			else { /* default case, nothing is given */
				int curcolor = sciGetForeground(sciGetCurrentSubWin()); /* current color equivalent for new graphics mode */
				sciSetCurrentObj (ConstructPolyline
					(sciGetCurrentSubWin(),
					&(x[*m * i]),&(y[*m * i]),&(z[*m * i]),0,*m,1,
					&curcolor,NULL,NULL,NULL,NULL,TRUE,FALSE,FALSE,FALSE));
			}

			pobj = sciGetCurrentObj();

			/* Force clipping */
		  sciSetIsClipping(pobj, 0);  /* clipgrf */

			hdltab[i]=sciGetHandle(pobj);
		}


		/** construct Compound and make it current object**/
		if ( *n>1 ) sciSetCurrentObj (ConstructCompound (hdltab, *n));
		FREE(hdltab);
	}

	/* =================================================
	* Redraw Figure
	* ================================================= */
	// subwin has been modified
	forceRedraw(psubwin);
	endFigureDataWriting(parentFigure);
	sciDrawObj(sciGetCurrentFigure ());

	pSUBWIN_FEATURE(psubwin)->FirstPlot=FALSE;

	FREE(loc); loc = NULL;

}
/*-----------------------------------------------------------
 * Objaxis:
 *-----------------------------------------------------------*/

void Objdrawaxis ( char     dir    ,
                   char     tics   ,
                   double * x      ,
                   int    * nx     ,
                   double * y      ,
                   int    * ny     ,
                   char   * val[]  ,
                   int      subint ,
                   char   * format ,
                   int      font   ,
                   int      textcol,
                   int      ticscol,
                   char     flag   ,
                   int      seg    ,
                   int      nb_tics_labels )
{
  checkRedrawing() ;
  sciSetCurrentObj (ConstructAxes
		    ((sciPointObj *)
		     sciGetCurrentSubWin(),
		     dir,tics,x,*nx,y,*ny,val,subint,format,font,textcol,ticscol,flag,seg,nb_tics_labels));
  sciDrawObjIfRequired(sciGetCurrentObj ());

}

/*-----------------------------------------------------------
 * Objnumb:
 *-----------------------------------------------------------*/

void Objnumb( char          * fname    ,
              unsigned long   fname_len,
              int         n        ,
              int         flag     ,
              double          x        ,
              double          y        ,
              double        * angle    ,
              double        * box       )
{
  /*** faire une macro scilab sur xstring ****/


}

/*------------------------------------------------
 * fec
 *-----------------------------------------------*/
void Objfec ( double    x[]        ,
              double    y[]        ,
              double    noeud[]    ,
              double  * fun        ,
              int * n          ,
              int * m          ,
              char      strflag[]  ,
              char      legend[]   ,
              double    brect[]    ,
              int   aaint[]    ,
              double    Zminmax[]  ,
              int   Colminmax[],
              int   ColOut[]   ,
              BOOL      WithMesh   ,
              BOOL      flagNax     )
{
  C2F(fec)(x,y,noeud,fun,n,m,strflag,legend,brect,aaint,
	   Zminmax,Colminmax,ColOut,WithMesh,flagNax,4L,bsiz);
}
/*------------------------------------------------------------------------*/
static void getDrect(const double vector[], int nbElements,
										 double * dMin, double * dMax,
										 double defaultMin, double defaultMax)
{
	if (containsOneFiniteElement(vector, nbElements))
	{
		*dMin = Mini(vector, nbElements);
		*dMax = Maxi(vector, nbElements);
	}
	else
	{
		*dMin = defaultMin;
		*dMax = defaultMax;
	}
}
/*------------------------------------------------------------------------*/
