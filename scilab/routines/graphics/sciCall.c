/*------------------------------------------------------------------------
 *    Graphic library 2001-2002
 *    New Graphics Fonctions
 --------------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Entities.h"
#include "PloEch.h"

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
   sciSetCurrentObj (ConstructRectangle
         ((sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ()),*x,*y,
                     *height, *width, 0, 0,fillflag, fillcolor ,n));
     
   *hdl=sciGetHandle(sciGetCurrentObj ()); 
   sciDrawObj(sciGetCurrentObj ());
   
}


/*----------------------------------------------
 * Objarc : 
 *-----------------------------------------------*/

void Objarc (angle1,angle2,x,y,width,height,color,fill,hdl)
    double *angle1,*angle2;
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
  sciSetCurrentObj (ConstructPolyline
         ((sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ()),x,y,
                     closed,n,1,0)); 
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
  sciDrawObj(sciGetCurrentObj ());
 
}


/*------------------------------------------------
 * Objfpoly : 
 *-----------------------------------------------*/

void Objfpoly (x,y,n,style,hdl)
    integer n,style;
    double *x,*y;
    long *hdl;
{ 
  
  sciSetCurrentObj (ConstructPatch
		      ((sciPointObj *)
		       sciGetSelectedSubWin (sciGetCurrentFigure ()), x, y, n));
  if (style < 0)
      sciSetForeground (sciGetCurrentObj(), -(style));
   else
      sciSetForeground (sciGetCurrentObj(), style);
  *hdl=sciGetHandle(sciGetCurrentObj ()); 
  sciDrawObj(sciGetCurrentObj ());
 
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

  n2=n1;
  fx=x;fy=y;
    sciSetCurrentObj (ConstructSegs
		      ((sciPointObj *)
		       sciGetSelectedSubWin (sciGetCurrentFigure ()),type,
	                x,y,n1,n2,fx,fy,flag,style,arsize,colored,arfact)); 
     sciDrawObj(sciGetCurrentObj ());  
    
}
/*-----------------------------------------------------------
 * Objstring:
 *-----------------------------------------------------------*/

void Objstring(fname,fname_len,str,x,y,angle,box,wy,hdl)
     char *fname; 
     unsigned long fname_len; 
     integer str;
     double x,y,*angle,*box,wy;
     long *hdl;
{ 
  integer v;
  double dv;
  integer x1,yy1,n=1,rect1[4];
   
  sciSetCurrentObj (ConstructText
			((sciPointObj *)
               		 sciGetSelectedSubWin (sciGetCurrentFigure ()), fname,
			 strlen (fname), x, y,wy));
     *hdl= sciGetHandle(sciGetCurrentObj ());
     sciSetFontOrientation (sciGetCurrentObj (), *angle *  10); 
     sciDrawObj(sciGetCurrentObj ());
          

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
void Objplot3d (fname,isfac,izcol,x,y,z,zcol,m,n,theta,alpha,legend,iflag,ebox)
     double x[],y[],z[];
     double *theta,*alpha,*ebox;
     integer *isfac,*n,*m,*iflag,*izcol,*zcol;
     char *fname,*legend;
{  
    sciTypeOf3D typeof3d;
    integer flagcolor;
     
      
    if (*isfac== 1) { 
      if (*izcol == 0) { 
	if (strncmp(fname,"plot3d1",7)==0) {
	  typeof3d = SCI_FAC3D;
	  flagcolor=1;
	}
	else {
	  typeof3d = SCI_FAC3D;
	  flagcolor=0;
	}
      }
      else if (*izcol == 2) {
	 typeof3d = SCI_FAC3D;
	 flagcolor=3;
      }
      
      else { 
	typeof3d = SCI_FAC3D; 
	 flagcolor=2;
      }
      
    } 
    else  if  (*isfac== 0) {
      if (strncmp(fname,"plot3d1",7)==0) {
	typeof3d = SCI_PLOT3D;
	flagcolor=1;
      }
      else {
	typeof3d = SCI_PLOT3D;
	flagcolor=0;
      }
    }
    else {
      typeof3d = SCI_PARAM3D1 ;
      flagcolor=1;
    }
    
    sciSetCurrentObj (ConstructSurface
		       ((sciPointObj *)
			sciGetSelectedSubWin (sciGetCurrentFigure ()), typeof3d,
			x, y, z, zcol, *izcol, *m, *n, *theta, *alpha, legend,iflag,ebox,flagcolor));
     sciDrawObj(sciGetCurrentObj ());
     
       
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
