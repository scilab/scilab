/*------------------------------------------------------------------------
 *    Graphic library 2001-2002
 *    Old Graphics Fonctions 
 *    Author Djalel ABDEMOUCHE
 *-------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include "../stack-c.h"
/*------------------------------------------------
 * Xrect
 *-----------------------------------------------*/   
void Xrect (fname,fname_len,x,y,width,height)
     char *fname; 
     unsigned long fname_len; 
     double *x,*y,*width,*height;
{ 
     integer v; 
    
     C2F(dr1)(fname,"v",&v,&v,&v,&v,&v,&v,x,y,width,height,fname_len,2L); 
    
}


/*------------------------------------------------
 * Xarc
 *-----------------------------------------------*/   
void Xarc (fname,fname_len,a1,a2,x,y,width,height)
     char *fname; 
     unsigned long fname_len; 
     double *x,*y,*width,*height; 
     integer *a1,*a2;

{ 
     integer v; 
     
     C2F(dr1)(fname,"v",&v,&v,&v,&v,a1,a2,x,y,width,height,fname_len,1L);
}

/*------------------------------------------------
 * Xrects
 *-----------------------------------------------*/   
void Xrects (fname,fname_len,vect1,n,vect2)
     char *fname; 
     unsigned long fname_len; 
     integer n,*vect1;
     double *vect2; 
{ 
     integer v;
     double dv; 
    
     C2F(dr1)(fname,"v",&v,vect1,&n,&v,&v,&v,vect2,&dv,&dv,&dv,fname_len,2L);
      
}

  
/*------------------------------------------------
 * Xarcs
 *-----------------------------------------------*/   
void Xarcs (fname,fname_len,vect1,n,vect2)
     char *fname; 
     unsigned long fname_len; 
     integer n,*vect1;
     double *vect2; 
{ 
     integer v;
     double dv; 
    
     C2F(dr1)(fname,"v",&v,vect1,&n,&v,&v,&v,vect2,&dv,&dv,&dv,fname_len,2L);
      
}

/*------------------------------------------------
 * Xfarcs
 *-----------------------------------------------*/   
void Xfarcs (fname,fname_len,vect1,n,vect2)
     char *fname; 
     unsigned long fname_len; 
     integer n,*vect1;
     double *vect2; 
{ 
     integer v;
     double dv; 
 
     C2F(dr1)(fname,"v",&v,vect1,&n,&v,&v,&v,vect2,&dv,&dv,&dv,fname_len,2L);
      
}

/*------------------------------------------------
 * xpoly(xv,yv,dtype,[close])
 *-----------------------------------------------*/   
void Xpoly (fname,fname_len,n,close,x,y)
     char *fname; 
     unsigned long fname_len; 
     double *x,*y;
     integer n,close; 
{ 
     integer v; 
     double dv;
   
     C2F(dr1)(fname,"xv",&n,&v,&v,&close,&v,&v,x,y,&dv,&dv,0L,0L);
}

/*------------------------------------------------
 * xfpoly(xv,yv,[close])
 *-----------------------------------------------*/   
void Xfpoly (rempl,closed,x,y)
     integer rempl,closed; 
     double *x,*y;
{ 
     integer v; 
     double dv;
   
     C2F(dr1)("xarea","xvoid",&rempl,&v,&v,&closed,&v,&v,x,y,&dv,&dv,6L,5L);
}

/*------------------------------------------------
 * xpolys(xpols,ypols,[draw])
 *-----------------------------------------------*/   
void Xpolys (vect,n,m,x,y)
     integer n,m; 
     integer *vect;
     double *x,*y;
{ 
     integer v; 
     double dv;
   
     C2F(dr1)("xpolys","v",&v,&v,vect,&n,&m,&v,x,y,&dv,&dv,7L,2L);
}


/*------------------------------------------------
 * xfpolys(xpols,ypols,[fill])
 *-----------------------------------------------*/   
void Xfpolys (vect,val1,val2,n,m,x,y)
     integer val1,val2,n,m; 
     integer *vect;
     double *x,*y;
{ 
     integer v; 
     double dv;
   
     C2F(dr1)("xliness","v",&val1,&val2,vect,&n,&m,&v,x,y,&dv,&dv,8L,2L);
     
}
/*-----------------------------------------------------------
 *   xsegs(xv,yv,[style])
 *-----------------------------------------------------------*/
void Xsegs (style,flag,n,x,y,arsize)
     integer *style,flag,n; 
     double *x,*y;
     double arsize;
{ 
     integer v; 
     double dv;    
     if (arsize == 0)
        C2F(dr1)("xsegs","v",style,&flag,&n,&v,&v,&v,x,y,&dv,&dv,6L,2L);
     else 
        C2F(dr1)("xarrow","v",style,&flag,&n,&v,&v,&v,x,y,&arsize,&dv,7L,2L);
}

/*------------------------------------------------
 *  xstring(x,y,str,[angle,box])
 *-----------------------------------------------*/   
void Xstring (fname,fname_len,str,x,y,angle,box)
     char *fname; 
     unsigned long fname_len; 
     integer str;
     double angle,*box;
     double x,y;
{ 
     integer v; 
     double dv;
   
     C2F(dr1)("xstring",fname,&v,&v,&v,&str,&v,&v,&x,&y,&angle,&dv,8L,fname_len);
     C2F(dr1)("xstringl",fname,&v,&v,&v,&v,&v,&v,&x,&y,box,&dv,9L,fname_len);
}

/*------------------------------------------------
 *  Xtitle(str)
 *-----------------------------------------------*/   
void Xtitle (str,n)
     char *str;
     int n;
{ 
     integer v; 
     double dv;
   
     C2F(dr1)("xstringa",str,&n,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,8L,2L);
}

/*------------------------------------------------
 * drawaxis (pos, xy_type, x, nx, y, ny, str, subtics
              format, fontsize,textcolor, 
              ticscolor, logflag, seg_flag)
 *-----------------------------------------------*/   
void Xdrawaxis (dir,tics,x,nx,y,ny,val,sub_int,format,fontsize,textcolor,ticscolor,flag,seg_flag)
     char dir,tics ,flag; 
     double *x,*y;
     char **val,*format;
     int *nx,*ny,sub_int;
     int fontsize,textcolor,ticscolor,seg_flag;
{ 
  int fontstyle = 0;
     sci_axis(dir,tics,x,nx,y,ny,val,sub_int,format,fontsize,textcolor,fontstyle,ticscolor,flag,seg_flag); 
     
}

/*------------------------------------------------
 *  plot2d(x,y,[style,strf,leg,rect,nax]) 
 *-----------------------------------------------*/   
void Xplot2d (x,y,n1,n2,style,strflag,legend,brect,aaint)
     double x[],y[],brect[];
     integer *n1,*n2,style[],aaint[];
     char legend[],strflag[];
{ 
  
     C2F(plot2d)(x,y,n1,n2,style,strflag,legend,brect,aaint,4L, bsiz);
     
}


/*------------------------------------------------
 *  grayplot(x,y,z,[strf,rect,nax])
 *-----------------------------------------------*/   
void Xgrayplot (x,y,z,n1,n2,strflag,brect,aaint)
     double x[],y[],z[],brect[];
     integer *n1,*n2,aaint[];
     char strflag[];
{ 
  
     C2F(xgray)(x,y,z,n1,n2,strflag, brect, aaint, bsiz);
     
}
/*------------------------------------------------
 *  matplot(z,[strf,rect,nax]) 
 *-----------------------------------------------*/   
void Xmatplot (z,n1,n2,strflag,brect,aaint)
     double z[],brect[];
     integer *n1,*n2,aaint[];
     char strflag[];
{ 
  
     C2F(xgray1)(z,n1,n2,strflag, brect, aaint, bsiz);
     
}
/*------------------------------------------------
 *  Matplot1 (z,xrect) 
 *-----------------------------------------------*/   
void Xmatplot1 (z,n1,n2,xrect) 
     double z[],xrect[];
     integer *n1,*n2;
{ 
    
     C2F(xgray2)(z, n1, n2,xrect);
     
         
}
/*------------------------------------------------
 *  3D Plotting of surfaces given by z=f(x,y)
 *-----------------------------------------------*/   
void Xplot3d (fname,isfac,izcol,x,y,z,zcol,m,n,theta,alpha,legend,iflag,ebox)
     double x[],y[],z[];
     double *theta,*alpha,*ebox;
     integer *isfac,*n,*m,*iflag,*izcol,*zcol;
     char *fname,*legend;
{ 
    

   if (*isfac== 1) 
    { 
       if (*izcol == 0) 
	 { if (strncmp(fname,"plot3d1",7)==0)
	   C2F(fac3d1)(x,y,z,zcol,m,n,theta,alpha,legend,iflag,ebox,bsiz); 
           else
	     C2F(fac3d)(x,y,z,zcol,m,n,theta,alpha,legend,iflag,ebox,bsiz);
         } 
       else if (*izcol == 2) 
	 C2F(fac3d3)(x,y,z,zcol,m,n,theta,alpha,legend,iflag,ebox,bsiz);
       else 
	 C2F(fac3d2)(x,y,z,zcol,m,n,theta,alpha,legend,iflag,ebox,bsiz);
     } 
   else if  (*isfac== 0) 
    { if (strncmp(fname,"plot3d1",7)==0)
      C2F(plot3d1)(x,y,z,m,n,theta,alpha,legend,iflag,ebox,bsiz);
      else
        C2F(plot3d)(x,y,z,m,n,theta,alpha,legend,iflag,ebox,bsiz);
    }
   else 
     C2F(param3d1)(x, y, z,m,n,izcol,zcol,theta, alpha,legend,iflag,ebox, bsiz); 
    
   
         
}
/*------------------------------------------------
 * xnumb(x,y,nums,[angles,box]) : 
 *-----------------------------------------------*/   
void Xnumb (fname,n,flag,x,y,angle,box)
     char *fname; 
     integer flag;
     double *x,*y,*angle,*box;
{ 
     integer v; 
    
     C2F(dr1)("xnum","xv",&v,&v,&v,&v,&n,&flag,x,y,angle,box,5L,3L);  
    
}

/*------------------------------------------------
 * fec(x,y,triangles,func,[strf,leg,rect,nax,zminmax,colminmax]);
 *-----------------------------------------------*/   
void Xfec (x,y,noeud,fun,n,m,strflag,legend,brect,aaint,Zminmax,Colminmax)
     double x[],y[],brect[],noeud[],Zminmax[]; 
     integer *n,*m,aaint[],Colminmax[];
     double *fun;
     char legend[],strflag[];
{ 
    
     C2F(fec)(x,y,noeud,fun,n,m,strflag,legend,brect,aaint,
         Zminmax,Colminmax,4L,bsiz);
     
}
