/*------------------------------------------------------------------------
 *    Graphic library 2001-2002
 *    Graphic subroutines interface
 *    Author Djalel ABDEMOUCHE
 --------------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Entities.h"
#include "PloEch.h"

extern void update_specification_bounds(sciPointObj *psubwin, double *rect,int flag);

int plot2dn(integer ptype,char *logflags,double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint,integer lstr1,integer lstr2);

/*------------------------------------------------
 * Objrect : 
 * On recupere la figure courante, puis on recupere la sous fenetre qui y est selectionnee
 *   puis on contruit le rectangle, qu on le place comme objet courant
 * ensuite il reste qu'appeler la fonction du dessin de l'objet 
 *-----------------------------------------------*/

void Objrect (x,y,width,height,fillflag,fillcolor,n,hdl)
    double *x,*y,*width,*height;
    int fillflag, fillcolor,n;
    long *hdl;
{ 
  sciPointObj *psubwin;
  psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

  sciSetCurrentObj (ConstructRectangle
		    (psubwin ,*x,*y,*height, *width, 0, 0,fillflag, fillcolor ,n));
     
   *hdl=sciGetHandle(sciGetCurrentObj ()); 
   if (sciGetSurface(psubwin) != (sciPointObj *) NULL) Merge3d(psubwin);
   sciDrawObj(sciGetCurrentObj ());
   
}


/*----------------------------------------------
 * Objarc : 
 *-----------------------------------------------*/

void Objarc (angle1,angle2,x,y,width,height,color,fill,hdl)
    int *angle1,*angle2;
    double *x,*y,*width,*height;
    int color,fill;
    long *hdl;
{ 
  sciSetCurrentObj (ConstructArc
         ((sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ()),*x,*y,
	  *height, *width, *angle1, *angle2, color, fill));
  *hdl=sciGetHandle(sciGetCurrentObj ()); 
  sciDrawObj(sciGetCurrentObj ());
 
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
  sciPointObj *psubwin;
  psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

  sciSetCurrentObj (ConstructPolyline(psubwin,x,y,PD0,closed,n,1,0)); 
  if (mark <= 0)
    { 
      sciSetIsMark(sciGetCurrentObj(), TRUE);
      sciSetMarkStyle (sciGetCurrentObj(),(-mark));
    }
   else
     {
      sciSetIsMark(sciGetCurrentObj(), FALSE);
      sciSetForeground (sciGetCurrentObj(), mark);
     }
  *hdl=sciGetHandle(sciGetCurrentObj ()); 
  if (sciGetSurface(psubwin) != (sciPointObj *) NULL) Merge3d(psubwin);
  sciDrawObj(sciGetCurrentObj ());
 
}


/*------------------------------------------------
 * Objfpoly : 
 *-----------------------------------------------*/

void Objfpoly (x,y,n,style,hdl)
    integer n,style;
    double *x,*y;
    long * hdl;
{ 
  long hdltab[2];
  sciPointObj *psubwin;
  psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
  
    sciSetCurrentObj (ConstructPolyline(psubwin,x,y,PD0,1,n,1,5)); 
    sciSetForeground (sciGetCurrentObj(), abs(style));
    hdltab[0]=sciGetHandle(sciGetCurrentObj ()); 
 
    if (style > 0) {
      sciSetCurrentObj (ConstructPolyline(psubwin,x,y,PD0,1,n,1,0)); 
      hdltab[1]=sciGetHandle(sciGetCurrentObj ()); 
      sciSetCurrentObj(ConstructAgregation (hdltab, 2)); }

  if (sciGetSurface(psubwin) != (sciPointObj *) NULL) Merge3d(psubwin);
  sciDrawObj(sciGetCurrentObj ());
  *hdl=sciGetHandle(sciGetCurrentObj ()); 
 
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
  sciPointObj *psubwin;
  psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

  n2=n1;
  fx=x;fy=y;
  sciSetCurrentObj (ConstructSegs(psubwin,type,
				  x,y,n1,n2,fx,fy,flag,style,arsize,colored,arfact)); 
  if (sciGetSurface(psubwin) != (sciPointObj *) NULL) Merge3d(psubwin);
  sciDrawObj(sciGetCurrentObj ());  
    
}
/*-----------------------------------------------------------
 * Objstring:
 *-----------------------------------------------------------*/

void Objstring(fname,fname_len,str,x,y,angle,box,wh,fill,hdl)
     char *fname; 
     unsigned long fname_len; 
     integer str;
     double x,y,*angle,*box,*wh;
     int fill;
     long *hdl;
{ 
  integer v;
  double dv;
  integer x1,yy1,n=1,rect1[4];
  sciPointObj *pobj;
   
  sciSetCurrentObj (ConstructText
			((sciPointObj *)
               		 sciGetSelectedSubWin (sciGetCurrentFigure ()), fname,
			 strlen (fname), x, y,wh));
  pobj=sciGetCurrentObj ();
  *hdl= sciGetHandle(pobj);
  sciSetFontOrientation (pobj, (int) (*angle *  10)); 
  pTEXT_FEATURE (pobj)->fill=fill;
  sciDrawObj(pobj);
          

  x1 = XDouble2Pixel(x);
  yy1 = YDouble2Pixel(y);
  C2F(dr)("xstringl",fname,&x1,&yy1,rect1,&v,&v,&v,&dv,&dv,&dv,&dv,9L,fname_len);
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
  sciDrawObj(sciGetCurrentObj ()); 
}


/*------------------------------------------------
 *  plot2d 
 *-----------------------------------------------*/   

void Objplot2d (ptype,logflags,x,y,n1,n2,style,strflag,legend,brect,aaint)
     double x[],y[],brect[];
     int ptype;
     integer *n1,*n2,style[],aaint[];
     char legend[],strflag[],logflags[];
{ 
     
     plot2dn(ptype,logflags,x,y,n1,n2,style,strflag,legend,brect,aaint,4L,bsiz); 
         
}

/*------------------------------------------------
 *  grayplot
 *-----------------------------------------------*/   
void Objgrayplot (x,y,z,n1,n2,strflag,brect,aaint) 
     double x[],y[],z[],brect[];
     integer *n1,*n2,aaint[];
     char strflag[];
{ 
     
     C2F(xgray)(x,y,z,n1,n2,strflag, brect, aaint, bsiz);
     
         
}/*------------------------------------------------
 *  Matplot
 *-----------------------------------------------*/   
void Objmatplot (z,n1,n2,strflag,brect,aaint) 
     double z[],brect[];
     integer *n1,*n2,aaint[];
     char strflag[];
{ 
     
     C2F(xgray1)(z,n1,n2,strflag, brect, aaint, bsiz);
     
         
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
     integer *isfac,*n,*m,*iflag,*izcol,*zcol;
     integer * m1, *n1, *m2, *n2, *m3, *n3, *m3n, *n3n;/*Adding F.Leray 12.03.04 and 19.03.04*/
     char *fname,*legend; 
     /* F.Leray 25.04.05 : warning here legends means "X@Y@Z": it is labels writings!! */
     /* legends has not the same meaning than inside plot2dn (there it really the legends of the plotted curves)*/
{  
  sciTypeOf3D typeof3d;
  integer flagcolor;  
  long *hdltab;
  int i, ok, mn;
  sciPointObj *psubwin;
  double drect[6];
  char *loc,*legx,*legy,*legz;
  sciSubWindow * ppsubwin = NULL;

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

  ppsubwin = pSUBWIN_FEATURE(psubwin);
  /* F.Leray 25.04.05 replace the default labels by the user labels if specified */
  loc=(char *) MALLOC( (strlen(legend)+1)*sizeof(char));
  if ( loc == NULL)    
    Scistring("Objplot3d : No more Place to store all the labels\n");
  
  strcpy(loc,legend);
  
  legx=strtok(loc,"@");
  sciSetText(ppsubwin->mon_x_label, legx , strlen(legx));
   
  legy=strtok((char *)0,"@");
  sciSetText(ppsubwin->mon_y_label, legy , strlen(legy));
 
  legz=strtok((char *)0,"@");
  sciSetText(ppsubwin->mon_z_label, legz , strlen(legz));
   
  /* Force psubwin->logflags to linear */
  pSUBWIN_FEATURE (psubwin)->logflags[0]='n';
  pSUBWIN_FEATURE (psubwin)->logflags[1]='n';
  
  pSUBWIN_FEATURE (psubwin)->axes.flag[0] = iflag[0]; /* mode: treatment of hidden parts */
  if (iflag[1] != 0){
    if (iflag[1]<7)
      pSUBWIN_FEATURE (psubwin)->axes.flag[1] = iflag[1]; /* type: scaling (no more useful)  */
    else
      pSUBWIN_FEATURE (psubwin)->axes.flag[1] = iflag[1]-6; /* type: scaling (no more useful)  */
  }
  
  if( pSUBWIN_FEATURE (psubwin)->FirstPlot == FALSE && (iflag[2] == 0 || iflag[2] == 1))
    { /* Nothing to do: we leave as before */}
  else
    pSUBWIN_FEATURE (psubwin)->axes.flag[2] = iflag[2]; /* box: frame around the plot      */
  

  pSUBWIN_FEATURE (psubwin)->alpha  = *alpha;
  pSUBWIN_FEATURE (psubwin)->theta  = *theta; 

  if (pSUBWIN_FEATURE(psubwin)->FirstPlot) {
    pSUBWIN_FEATURE (psubwin)->project[2]= 1;
    pSUBWIN_FEATURE (psubwin)->is3d  = TRUE;
    pSUBWIN_FEATURE (psubwin)->isaxes  = TRUE;
  }

  ok=0;  
  if (sciGetSurface(psubwin) != (sciPointObj *) NULL)   ok=1;

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
    if (iflag[1] != 0) update_specification_bounds(psubwin, drect,3);
  } 
  
  if(iflag[1] != 0)
    pSUBWIN_FEATURE(psubwin)->isoview = (BOOL)( iflag[1] == 3 || iflag[1] == 4 ||
						iflag[1] == 5 || iflag[1] == 6);
  
  /* =================================================
   * Analyze arguments to find entity type 
   * ================================================= */

  if (*isfac== 1) { 
    if (*izcol == 0) { 
      if (strncmp(fname,"plot3d1",7)==0) {typeof3d = SCI_FAC3D;   flagcolor=1;}
      else                               {typeof3d = SCI_FAC3D;   flagcolor=0;}
    }
    else if (*izcol == 2)                {typeof3d = SCI_FAC3D;   flagcolor=3;}
    else                                 {typeof3d = SCI_FAC3D;   flagcolor=2;}
  } 
  else  if  (*isfac== 0) {
    if (strncmp(fname,"plot3d1",7)==0)   {typeof3d = SCI_PLOT3D;  flagcolor=1;}
    else                                 {typeof3d = SCI_PLOT3D;  flagcolor=0;}
  }
  else                                   {typeof3d = SCI_PARAM3D1;flagcolor=1;}


  /* =================================================
   * Construct the Entities
   * ================================================= */

  if ( typeof3d != SCI_PARAM3D1 ) /*Distinction here between SCI_PARAM3D1 and others*/
    sciSetCurrentObj (ConstructSurface
		      ((sciPointObj *)
		       psubwin, typeof3d,
		       x, y, z, zcol, *izcol, *m, *n, iflag,ebox,flagcolor,
		       isfac,m1,n1,m2,n2,m3,n3,m3n,n3n));
  else
    {
      if ((hdltab = malloc (*n * sizeof (long))) == NULL) {
	Scierror(999,"%s: No more memory available\r\n",fname);
	return; 
      }
      for (i = 0; i < *n; ++i) { 
	/* F.Leray Pb here: In fact we do not create a Surface but one or several 3D Polylines
	 Pb comes when wanting to access the fields "surface_color" or "flag" for example
	 in function sciSet (cf. matdes.c). 
	 Question 1: Are these properties accessible from a SCI_PARAM3D1 ?
	 Question 2: Is "flag" obsolete and replaced by "color_mode"?*/
	sciSetCurrentObj (ConstructPolyline
			  ((sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ()),
			   &(x[*m * i]),&(y[*m * i]),&(z[*m * i]),0,*m,1,0));  
	if ((*n > 0) && (zcol != (integer *)NULL)) {
	  if (zcol[i] >= 0)
	    sciSetForeground (sciGetCurrentObj(), zcol[i]);
	  else {
	    sciSetIsMark(sciGetCurrentObj(),1);
	    sciSetMarkStyle(sciGetCurrentObj(),-zcol[i]+1);
	  }
	}
	hdltab[i]=sciGetHandle(sciGetCurrentObj ()); 
      } 
      /** construct agregation and make it current object**/
      if ( *n>1 ) sciSetCurrentObj (ConstructAgregation (hdltab, *n));  
      FREE(hdltab);
    }

  /* =================================================
   * Merge with previous 3D plots (build a set of facets)
   * ================================================= */

  if (ok==1) Merge3d(psubwin);

  /* =================================================
   * Redraw Figure
   * ================================================= */

  sciDrawObj(sciGetCurrentFigure ());
  pSUBWIN_FEATURE(psubwin)->FirstPlot=FALSE;
   
  FREE(loc); loc = NULL;
       
}
/*-----------------------------------------------------------
 * Objaxis:
 *-----------------------------------------------------------*/

void Objdrawaxis (dir,tics,x,nx,y,ny,val,subint,format,font,textcol,ticscol,flag,seg)
     char dir,tics ,*format,flag,*val[]; 
     double *x,*y;
     int *nx,*ny;
     int subint,font,textcol,ticscol, seg;
{ 
      
  sciSetCurrentObj (ConstructAxes 
		    ((sciPointObj *)
		     sciGetSelectedSubWin (sciGetCurrentFigure ()),
		     dir,tics,x,*nx,y,*ny,val,subint,format,font,textcol,ticscol,flag,seg));  
  sciDrawObj(sciGetCurrentObj ());
     
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
void Objfec (x,y,noeud,fun,n,m,strflag,legend,brect,aaint,Zminmax,Colminmax)
     double x[],y[],brect[],noeud[],Zminmax[];
     integer *n,*m,aaint[],Colminmax[];
     char legend[],strflag[];
     double *fun;
{ 
  C2F(fec)(x,y,noeud,fun,n,m,strflag,legend,brect,aaint,
	   Zminmax,Colminmax,4L,bsiz);
}
