/*------------------------------------------------------------------------
 *    Graphic library 2001-2002
 *    Graphic subroutines interface
 *    Author Djalel ABDEMOUCHE
 --------------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "SetProperty.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "InitObjects.h"

#include "PloEch.h"

#if WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

extern void updateScaleIfRequired( sciPointObj * pSubWin ) ;

extern BOOL update_specification_bounds(sciPointObj *psubwin, double *rect,int flag);

int plot2dn(integer ptype,char *logflags,double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint, BOOL flagNax, integer lstr1,integer lstr2);

/*------------------------------------------------
 * Objrect : 
 * On recupere la figure courante, puis on recupere la sous fenetre qui y est selectionnee
 *   puis on contruit le rectangle, qu on le place comme objet courant
 * ensuite il reste qu'appeler la fonction du dessin de l'objet 
 *-----------------------------------------------*/

void Objrect (x,y,width,height,foreground,background,isfilled,isline,n,hdl,flagstring)
     double *x,*y,*width,*height;
     int *foreground, *background;
     BOOL isfilled, isline;
     int n;
     long *hdl;
     BOOL flagstring;
{ 
  sciPointObj *psubwin;
  sciPointObj * newObj = NULL ;
  psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
  
  newObj = ConstructRectangle( psubwin ,*x,*y,*height, *width, 0, 0,
                               foreground, background, isfilled, isline, n, flagstring ) ;
  
  if ( newObj == NULL )
  {
    /* an error occured */
    *hdl = -1 ;
    return ;
  }
  sciSetCurrentObj( newObj ) ;
  
  *hdl=sciGetHandle(sciGetCurrentObj ()); 
}


/*----------------------------------------------
 * Objarc : 
 *-----------------------------------------------*/

void Objarc (angle1,angle2,x,y,width,height,foreground,background,isfilled,isline,hdl)
    int *angle1,*angle2;
    double *x,*y,*width,*height;
    int *foreground, *background;
    BOOL isfilled,isline;
    long *hdl;
{ 
  sciPointObj *psubwin, *pobj;
  psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

  sciSetCurrentObj (ConstructArc
         (psubwin,*x,*y,
	  *height, *width, *angle1, *angle2, foreground, background, isfilled, isline));
  pobj = sciGetCurrentObj();
 
  *hdl=sciGetHandle(pobj);
  sciDrawObjIfRequired(pobj);
 
}

/*------------------------------------------------
 * Objpoly : 
 *-----------------------------------------------*/

void Objpoly (x,y,n,closed,mark,hdl)
    integer n,closed;
    double *x,*y;
    long *hdl;
    int mark;
{ 
  sciPointObj *psubwin, *pobj;
  psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
  
  if (mark <= 0)
    { 
      int absmark = abs(mark);
      sciSetCurrentObj (ConstructPolyline(psubwin,x,y,PD0,closed,n,1,1,
					  NULL,NULL,&absmark,NULL,NULL,FALSE,FALSE,TRUE,FALSE));
    }
  else
    {
      sciSetCurrentObj (ConstructPolyline(psubwin,x,y,PD0,closed,n,1,1,
					  &mark,NULL,NULL,NULL,NULL,TRUE,FALSE,FALSE,FALSE));
    }
  
   pobj = sciGetCurrentObj();
   *hdl=sciGetHandle(pobj); 
}
  

/*------------------------------------------------
 * Objfpoly : 
 *-----------------------------------------------*/

void Objfpoly (x,y,n,style,hdl,shading)
     integer n, *style, shading;
     double *x,*y;
     long * hdl;
{ 
  int fillcolor, contourcolor;
  sciPointObj *psubwin, *pobj;
  int closed = 1; /* we close the polyline by default */
  psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
  

  if(shading == 2)
    {
      /* interpolated shading is "on" */
      sciSetCurrentObj (ConstructPolyline(psubwin,x,y,PD0,closed,n,
					  1,1,NULL,style,NULL,NULL,NULL,FALSE,TRUE,FALSE,TRUE));
    }
  else
    {
      /* flat mode is "on" */
      if (*style < 0){
	fillcolor = abs(*style);
	sciSetCurrentObj (ConstructPolyline(psubwin,x,y,PD0,closed,n,
					    1,1,NULL,&fillcolor,NULL,NULL,NULL,FALSE,TRUE,FALSE,FALSE));
      }
      else if (*style == 0){
	contourcolor = sciGetForeground(psubwin);
	sciSetCurrentObj (ConstructPolyline(psubwin,x,y,PD0,closed,n,
					    1,1,&contourcolor,NULL,NULL,NULL,NULL,TRUE,FALSE,FALSE,FALSE));
      }
      else{ /* *style > 0*/
	fillcolor = *style;
	contourcolor = sciGetForeground(psubwin);
	sciSetCurrentObj (ConstructPolyline(psubwin,x,y,PD0,closed,n,
					    1,1,&contourcolor,&fillcolor,NULL,NULL,NULL,TRUE,TRUE,FALSE,FALSE));
      }
      
      pobj = sciGetCurrentObj();
      *hdl=sciGetHandle(sciGetCurrentObj ());  
    }
}


/*-----------------------------------------------------------
 *   Objsegs :
 *-----------------------------------------------------------*/
void Objsegs (style,flag,n1,x,y,arsize)
     integer *style,flag,n1;
     double *x,*y;
     double arsize;
{ 
  integer type=0,n2, colored=0;
  double *fx,*fy,arfact=1.0;
  int typeofchamp = -1; /* no champ here, only segs ; this info is useless */
  sciPointObj *psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

  n2=n1;
  fx=x;fy=y;
  sciSetCurrentObj (ConstructSegs(psubwin,type,
				  x,y,n1,n2,fx,fy,flag,style,arsize,colored,arfact,typeofchamp)); 
  sciDrawObjIfRequired(sciGetCurrentObj ()); 
}
/*-----------------------------------------------------------
 * Objstring:
 *-----------------------------------------------------------*/

/* box is an OUTPUT re-used inside matdes.c in scixstring */
void Objstring(fname,fname_len,str,x,y,angle,box,wh,hdl,fill,foreground,background,isboxed,isline,isfilled)
     char *fname;
     unsigned long fname_len; 
     integer str;
     double x,y,*angle,*box,*wh;
     int fill;
     long *hdl;
     int *foreground, *background;
     BOOL isboxed,isline,isfilled;
{
  integer v ;
  double dv ;
  integer x1,yy1,n=1,rect1[4];
  sciPointObj *psubwin, *pobj;
   
  psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
  sciSetCurrentObj (ConstructText
			(psubwin, fname,
			 strlen (fname), x, y,wh, fill,foreground,background,isboxed,isline,isfilled));
  pobj=sciGetCurrentObj ();
  *hdl= sciGetHandle(pobj);
  sciSetFontOrientation (pobj, (int) (*angle *  10));

  sciDrawObjIfRequired(pobj);
          
  updateScaleIfRequired( sciGetParentSubwin( pobj ) ) ;

  x1 = XDouble2Pixel(x);
  yy1 = YDouble2Pixel(y);
  C2F(dr)("xstringl",fname,&x1,&yy1,rect1,&v,&v,&v,&dv,&dv,&dv,&dv,9L,fname_len) ;
  C2F(echelle2d)(box,box+1,rect1,rect1+1,&n,&n,"i2f",3L);
  C2F(echelle2dl)(box+2,box+3,rect1+2,rect1+3,&n,&n,"i2f");
 
}
/*-----------------------------------------------------------
 * Objtitle:
 *-----------------------------------------------------------*/

void Objtitle(str,n,hdl)
     char* str;
     int n;
     long *hdl;
{ 
  sciSetCurrentObj (ConstructTitle
  		((sciPointObj *)
             		 sciGetSelectedSubWin (sciGetCurrentFigure ()),str,n));
  
  *hdl=sciGetHandle(sciGetCurrentObj ()); 
  sciDrawObjIfRequired(sciGetCurrentObj ()); 
}


/*------------------------------------------------
 *  plot2d 
 *-----------------------------------------------*/   

void Objplot2d (ptype,logflags,x,y,n1,n2,style,strflag,legend,brect,aaint,flagNax)
     double x[],y[],brect[];
     int ptype;
     integer *n1,*n2,style[],aaint[];
     char legend[],strflag[],logflags[];
     BOOL flagNax;
{ 
     
     plot2dn(ptype,logflags,x,y,n1,n2,style,strflag,legend,brect,aaint,flagNax,4L,bsiz); 
         
}

/*------------------------------------------------
 *  grayplot
 *-----------------------------------------------*/   
void Objgrayplot (x,y,z,n1,n2,strflag,brect,aaint,flagNax) 
     double x[],y[],z[],brect[];
     integer *n1,*n2,aaint[];
     char strflag[];
     BOOL flagNax;
{ 
     
     C2F(xgray)(x,y,z,n1,n2,strflag, brect, aaint, flagNax, bsiz );
     
         
}/*------------------------------------------------
 *  Matplot
 *-----------------------------------------------*/   
void Objmatplot (z,n1,n2,strflag,brect,aaint,flagNax) 
     double z[],brect[];
     integer *n1,*n2,aaint[];
     char strflag[];
     BOOL flagNax;
{ 
     
     C2F(xgray1)(z,n1,n2,strflag, brect, aaint, flagNax, bsiz);
     
         
}
/*------------------------------------------------
 *  Matplot1
 *-----------------------------------------------*/   
void Objmatplot1 (z,n1,n2,xrect) 
     double z[],xrect[];
     integer *n1,*n2;
{ 
    
     C2F(xgray2)(z, n1, n2,xrect);
     
         
}

/*------------------------------------------------
 *  plot3d 
 *-----------------------------------------------*/   
void Objplot3d (fname,isfac,izcol,x,y,z,zcol,m,n,theta,alpha,legend,iflag,ebox,m1,n1,m2,n2,m3,n3,m3n,n3n)
     double x[],y[],z[];
     double *theta,*alpha,*ebox;
     integer *isfac,*n,*m,*iflag,*izcol;
     double *zcol;
     integer * m1, *n1, *m2, *n2, *m3, *n3, *m3n, *n3n;/*Adding F.Leray 12.03.04 and 19.03.04*/
     char *fname,*legend; 
     /* F.Leray 25.04.05 : warning here legends means "X@Y@Z": it is labels writings!! */
     /* legends has not the same meaning than inside plot2dn (there, it is really the legends of the plotted curves)*/
{  
  sciTypeOf3D typeof3d;
  integer flagcolor;  
  long *hdltab;
  int i, ok, mn;
  sciPointObj *psubwin = NULL, *pobj = NULL;
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

  sciPointObj  * pNewSurface = NULL ;
  sciSubWindow * ppsubwin = NULL;
  BOOL bounds_changed = FALSE; /* cannot be used here because we have to force redrawing since there is no way to avoid merge (=> complete redraaw) */

  /* Initialisation drect A.C pour debuggueur */
  drect[0]=0;
  drect[1]=0;
  drect[2]=0;
  drect[3]=0;
  drect[4]=0;
  drect[5]=0;


  if (!(sciGetGraphicMode (sciGetSelectedSubWin (sciGetCurrentFigure ())))->addplot) { 
    sciXbasc(); 
    initsubwin();
    sciRedrawFigure();
  } 

  /* =================================================
   * Force SubWindow properties according to arguments 
   * ================================================= */

  psubwin= (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
  
  /* Force psubwin->is3d to TRUE: we are in 3D mode */
  pSUBWIN_FEATURE (psubwin)->is3d = TRUE;
  pSUBWIN_FEATURE (psubwin)->project[2] = 1; /* to force z axis display */

  ppsubwin = pSUBWIN_FEATURE(psubwin);
  /* F.Leray 25.04.05 replace the default labels by the user labels if specified */
  loc=(char *) MALLOC( (strlen(legend)+1)*sizeof(char));
  if ( loc == NULL)    
    Scistring("Objplot3d : No more Place to store all the labels\n");
  
  strcpy(loc,legend);
  
  /*   legx=strtok_r(loc,"@",&buff); */
  legx=strtok(loc,"@");
  
  if(legx == NULL) {
    char empty[]= "";
    sciSetText(ppsubwin->mon_x_label, empty , strlen(empty));
  }
  else
    sciSetText(ppsubwin->mon_x_label, legx , strlen(legx));
   
  /*   legy=strtok_r((char *)0,"@",&buff); */
  legy=strtok((char *)0,"@");
  if(legy == NULL) {
    char empty[]= "";
    sciSetText(ppsubwin->mon_y_label, empty , strlen(empty));
  }
  else
    sciSetText(ppsubwin->mon_y_label, legy , strlen(legy));
 
  /*   legz=strtok_r((char *)0,"@",&buff); */
  legz=strtok((char *)0,"@");
  if(legz == NULL) {
    char empty[]= "";
    sciSetText(ppsubwin->mon_z_label, empty , strlen(empty));
  }
  else
    sciSetText(ppsubwin->mon_z_label, legz , strlen(legz));
   
  /* Force psubwin->logflags to linear */
  pSUBWIN_FEATURE (psubwin)->logflags[0]='n';
  pSUBWIN_FEATURE (psubwin)->logflags[1]='n';
  pSUBWIN_FEATURE (psubwin)->logflags[2]='n'; /* add z logscale default here */
 
  
  pSUBWIN_FEATURE (psubwin)->axes.flag[0] = iflag[0]; /* mode: treatment of hidden parts */
  if (iflag[1] != 0){
    if (iflag[1]<7)
      pSUBWIN_FEATURE (psubwin)->axes.flag[1] = iflag[1]; /* type: scaling (no more useful)  */
    else
      pSUBWIN_FEATURE (psubwin)->axes.flag[1] = iflag[1]-6; /* type: scaling (no more useful)  */
  }
  
  if( pSUBWIN_FEATURE (psubwin)->FirstPlot == FALSE && (iflag[2] == 0 || iflag[2] == 1))
    { /* Nothing to do: we leave as before */}
  else{
    pSUBWIN_FEATURE (psubwin)->axes.flag[2] = iflag[2]; /* box: frame around the plot      */
    
    if(iflag[2] == 0 || iflag[2] == 1){
      pSUBWIN_FEATURE (psubwin)->axes.rect = 0; /* for 2d use only (when switching to 2d mode) */

      if (pSUBWIN_FEATURE(psubwin)->FirstPlot) {
	pSUBWIN_FEATURE (psubwin)->axes.axes_visible[0] = FALSE;
	pSUBWIN_FEATURE (psubwin)->axes.axes_visible[1] = FALSE;
	pSUBWIN_FEATURE (psubwin)->axes.axes_visible[2] = FALSE;
	pSUBWIN_FEATURE (psubwin)->axes.rect = 0;
      }
      /*else no changes : the axes visible properties are driven by the previous plot */
    }
    else if(iflag[2] == 3){
      pSUBWIN_FEATURE (psubwin)->axes.rect = 1; /* for 2d use only (when switching to 2d mode) */
      
      pSUBWIN_FEATURE (psubwin)->axes.axes_visible[0] = FALSE;
      pSUBWIN_FEATURE (psubwin)->axes.axes_visible[1] = FALSE;
      pSUBWIN_FEATURE (psubwin)->axes.axes_visible[2] = FALSE;
      
      sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_x_label,TRUE);
      sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_y_label,TRUE);
      sciSetVisibility(pSUBWIN_FEATURE (psubwin)->mon_z_label,TRUE);
    }
    else if(iflag[2] == 4){
      pSUBWIN_FEATURE (psubwin)->axes.rect = 1;

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

  ok=0;  
  if (ChildrenCounter(psubwin)>0) ok=1;
  
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
      mn=(*m1) * (*n1); 
      drect[0] = (double) Mini(x, mn); /*xmin*/
      drect[1] = (double) Maxi(x, mn); /*xmax*/
      mn=(*m2) * (*n2); 
      drect[2] = (double) Mini(y, mn); /*ymin*/
      drect[3] = (double) Maxi(y, mn); /*ymax*/
      mn=(*m3) * (*n3); 
      drect[4] = (double) Mini(z, mn); /*zmin*/
      drect[5] = (double) Maxi(z, mn); /*zmax*/
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
      bounds_changed = update_specification_bounds(psubwin, drect,3);
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
    else if(*m1 == 1)
    { /* x is a row vector */
      dimvectx = *n1;
    }
    else if(*n1 == 1)
    { /* x is a column vector */
      dimvectx = *m1;
    }
    else
    { 
      dimvectx = -1;
    }
    
    if(dimvectx>1){
      /* test the monotony on x*/
      if(x[0] >= x[1]) /* decreasing */
	{
	  int i;
	  for(i=1;i<dimvectx-1;i++)
	    {
	      if(x[i] < x[i+1])
		{
		  Scierror(999,"Objplot3d: x vector is not monotonous \t\n");
		  return;
		}
	    }
	  flag_x = -1;
	}
      else /* x[0] < x[1]*/
	{
	  for(i=1;i<dimvectx-1;i++)
	    {
	      if(x[i] > x[i+1])
		{
		  Scierror(999,"Objplot3d: x vector is not monotonous \t\n");
		  return;
		}
	    }
	  flag_x = 1;
	}
    }
    
    if ( *isfac == 1 )
    {
      /* y is considered as a matrix */
      dimvecty = -1 ;
    }
    else if(*m2 == 1) /* y is a row vector */
    {
      dimvecty = *n2;
    }
    else if(*n2 == 1) /* y is a column vector */
    {
      dimvecty = *m2;
    }
    else /* y is a matrix */
    {
      dimvecty = -1;
    }
   
    if(dimvecty>1){
      /* test the monotony on y*/
      if(y[0] >= y[1]) /* decreasing */
	{
	  int i;
	  for(i=1;i<dimvecty-1;i++)
	  {
	    if(y[i] < y[i+1])
	      {
		Scierror(999,"Objplot3d: y vector is not monotonous \t\n");
		return;
	      }
	  }
	  flag_y = -1;
	}
      else /* y[0] < y[1]*/
	{
	  for(i=1;i<dimvecty-1;i++)
	    {
	      if(y[i] > y[i+1])
		{
		  Scierror(999,"Objplot3d: y vector is not monotonous \t\n");
		  return;
		}
	    }
	  flag_y = 1;
	}
    }

    pNewSurface = ConstructSurface( psubwin, typeof3d,
                                    x, y, z, zcol, *izcol, *m, *n, iflag,ebox,flagcolor,
                                    isfac,m1,n1,m2,n2,m3,n3,m3n,n3n) ;
    
    if ( pNewSurface == NULL ) { return ; }

    sciSetCurrentObj( pNewSurface ) ;
        
    pSURFACE_FEATURE (sciGetCurrentObj())->flag_x = flag_x;
    pSURFACE_FEATURE (sciGetCurrentObj())->flag_y = flag_y;
    
    
  }
  else
    {
      if ((hdltab = MALLOC (*n * sizeof (long))) == NULL) {
	Scierror(999,"%s: No more memory available\r\n",fname);
	return; 
      }
      for (i = 0; i < *n; ++i) { 
	/* F.Leray Pb here: In fact we do not create a Surface but one or several 3D Polylines
	   Pb comes when wanting to access the fields "surface_color" or "flag" for example
	   in function sciSet (cf. matdes.c). 
	   Question 1: Are these properties accessible from a SCI_PARAM3D1 ?
	   Question 2: Is "flag" obsolete and replaced by "color_mode"?*/

	/* 	sciSetCurrentObj (ConstructPolyline */
	/* 			  ((sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ()), */
	/* 			   &(x[*m * i]),&(y[*m * i]),&(z[*m * i]),0,*m,1,1));   */

	if ((*n > 0) && (zcol != (double *)NULL)) {
	  if ((int) zcol[i] > 0){
	    int intzcol = (int) zcol[i];
	    sciSetCurrentObj (ConstructPolyline
			      ((sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ()),
			       &(x[*m * i]),&(y[*m * i]),&(z[*m * i]),0,*m,*n,1,
			       &intzcol,NULL,NULL,NULL,NULL,TRUE,FALSE,FALSE,FALSE));  
	  }
	  else {
	    int intzcol = (int) -zcol[i];
	    sciSetCurrentObj (ConstructPolyline
			      ((sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ()),
			       &(x[*m * i]),&(y[*m * i]),&(z[*m * i]),0,*m,*n,1,
			       NULL,NULL,&intzcol,NULL,NULL,FALSE,FALSE,TRUE,FALSE));  
	  }
	}
	else { /* default case, nothing is given */
	  int curcolor = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ())); /* current color equivalent for new graphics mode */
	  sciSetCurrentObj (ConstructPolyline
			    ((sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ()),
			     &(x[*m * i]),&(y[*m * i]),&(z[*m * i]),0,*m,*n,1,
			     &curcolor,NULL,NULL,NULL,NULL,TRUE,FALSE,FALSE,FALSE));  
	}

	pobj = sciGetCurrentObj();
	
	hdltab[i]=sciGetHandle(pobj); 
      }


/*       	if ((*n > 0) && (zcol != (double *)NULL)) { */
/*       	  if ((int) zcol[i] > 0){ */
/*       	    sciSetForeground (pobj, (int) zcol[i]); */
/*       	    sciSetIsMark(pobj, FALSE); */
/*       	    sciSetIsLine(pobj,  TRUE); */
/*       	  } */
/*       	  else { */
/*       /\* 	    sciSetMarkSizeUnit(pobj,2); /\\* force switch to tabulated mode : old syntax *\\/ *\/ */
/*        	    sciSetIsMark(pobj,TRUE); */
/*       	    sciSetIsLine(pobj,FALSE); */
/*       	    sciSetMarkStyle(pobj,(int) -zcol[i]); */
/*       	  } */
/*       	} */
/*       	hdltab[i]=sciGetHandle(pobj); */
/*             } */


      /** construct Compound and make it current object**/
      if ( *n>1 ) sciSetCurrentObj (ConstructCompound (hdltab, *n));  
      FREE(hdltab);
    }

  /* =================================================
   * Merge with previous 3D plots (build a set of facets)
   * ================================================= */

  if (ok==1) Merge3d(psubwin);

  /* =================================================
   * Redraw Figure
   * ================================================= */
  {
    sciPointObj * pfig = sciGetCurrentFigure () ;
    sciDrawObj( pfig ) ;
  }
  /*   EraseAndOrRedraw(sciGetSelectedSubWin (sciGetCurrentFigure ())); /\* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 *\/ */
  pSUBWIN_FEATURE(psubwin)->FirstPlot=FALSE;
   
  FREE(loc); loc = NULL;
       
}
/*-----------------------------------------------------------
 * Objaxis:
 *-----------------------------------------------------------*/

void Objdrawaxis (dir,tics,x,nx,y,ny,val,subint,format,font,textcol,ticscol,flag,seg,nb_tics_labels)
     char dir,tics ,*format,flag,*val[]; 
     double *x,*y;
     int *nx,*ny;
     int subint,font,textcol,ticscol, seg,nb_tics_labels;
{ 
      
  sciSetCurrentObj (ConstructAxes 
		    ((sciPointObj *)
		     sciGetSelectedSubWin (sciGetCurrentFigure ()),
		     dir,tics,x,*nx,y,*ny,val,subint,format,font,textcol,ticscol,flag,seg,nb_tics_labels));  
  sciDrawObjIfRequired(sciGetCurrentObj ());
     
  /* F.Leray 10.03.04: In fact we use ConstructAxes AND NOT ConstructAxis to draw
     one axis. ConstructAxis is apparently unused!!*/
     
}

/*-----------------------------------------------------------
 * Objnumb:
 *-----------------------------------------------------------*/

void Objnumb(fname,fname_len,n,flag,x,y,angle,box)
     char *fname; 
     unsigned long fname_len; 
     integer n,flag;
     double x,y,*angle,*box;
{ 
  /*** faire une macro scilab sur xstring ****/

   
}

/*------------------------------------------------
 * fec
 *-----------------------------------------------*/   
void Objfec (x,y,noeud,fun,n,m,strflag,legend,brect,aaint,Zminmax,Colminmax,ColOut,WithMesh,flagNax)
     double x[],y[],brect[],noeud[],Zminmax[];
     integer *n,*m,aaint[],Colminmax[],ColOut[];
     char legend[],strflag[];
     double *fun;
     BOOL WithMesh, flagNax;
{ 
  C2F(fec)(x,y,noeud,fun,n,m,strflag,legend,brect,aaint,
	   Zminmax,Colminmax,ColOut,WithMesh,flagNax,4L,bsiz);
}
