/*------------------------------------------------------------------------
 *    Graphic library 2001-2002
 *    Old Graphics Fonctions 
 *    Author Djalel ABDEMOUCHE, Jean-Baptiste Silvy
 *-------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "Graphics.h" 
#include "dr1Call.h"
#include "Plo2d.h"

/*------------------------------------------------
 * Xrect
 *-----------------------------------------------*/   
void Xrect ( char          * fname    ,
             unsigned long   fname_len,
             double        * x        ,
             double        * y        ,
             double        * width    ,
             double        * height    )
{ 
     integer v; 
     C2F(dr1)(fname,"v",&v,&v,&v,&v,&v,&v,x,y,width,height,fname_len,2L); 
}

/*------------------------------------------------
 * Xarc
 *-----------------------------------------------*/   
void Xarc ( char          * fname        ,
            unsigned long   fname_len,
            integer       * a1           ,
            integer       * a2           ,
            double        * x            ,
            double        * y            ,
            double        * width        ,
            double        * height        )
{ 
     integer v; 
     
     C2F(dr1)(fname,"v",&v,&v,&v,&v,a1,a2,x,y,width,height,fname_len,1L);
}

/*------------------------------------------------
 * Xrects
 *-----------------------------------------------*/   
void Xrects ( char          * fname    ,
              unsigned long   fname_len,
              integer       * vect1    ,
              integer         n        ,
              double        * vect2     )
{ 
     integer v;
     double dv; 
    
     C2F(dr1)(fname,"v",&v,vect1,&n,&v,&v,&v,vect2,&dv,&dv,&dv,fname_len,2L);
}

  
/*------------------------------------------------
 * Xarcs
 *-----------------------------------------------*/   
void Xarcs ( char          * fname    ,
             unsigned long   fname_len,
             integer       * vect1    ,
             integer         n        ,
             double        * vect2     )
{ 
     integer v;
     double dv; 
    
     C2F(dr1)(fname,"v",&v,vect1,&n,&v,&v,&v,vect2,&dv,&dv,&dv,fname_len,2L);
      
}

/*------------------------------------------------
 * Xfarcs
 *-----------------------------------------------*/   
void Xfarcs( char          * fname    ,
             unsigned long   fname_len,
             integer       * vect1    ,
             integer         n        ,
             double        * vect2     )
{ 
     integer v;
     double dv; 
 
     C2F(dr1)(fname,"v",&v,vect1,&n,&v,&v,&v,vect2,&dv,&dv,&dv,fname_len,2L);
}

/*------------------------------------------------
 * xpoly(xv,yv,dtype,[close])
 *-----------------------------------------------*/   
void Xpoly ( char          * fname    ,
             unsigned long   fname_len,
             integer         n        ,
             integer         close    ,
             double        * x        ,
             double        * y         )
{ 
     integer v; 
     double dv;
   
     C2F(dr1)(fname,"xv",&n,&v,&v,&close,&v,&v,x,y,&dv,&dv,0L,0L);
}

/*------------------------------------------------
 * xfpoly(xv,yv,[close])
 *-----------------------------------------------*/   
void Xfpoly ( integer   rempl ,
              integer   closed,
              double  * x     ,
              double  * y      )
{
     integer v; 
     double dv;
   
     C2F(dr1)("xarea","xvoid",&rempl,&v,&v,&closed,&v,&v,x,y,&dv,&dv,6L,5L);
}

/*------------------------------------------------
 * xpolys(xpols,ypols,[draw])
 *-----------------------------------------------*/   
void Xpolys ( integer * vect,
              integer   n   ,
              integer   m   ,
              double  * x   ,
              double  * y    )
{ 
     integer v; 
     double dv;
   
     C2F(dr1)("xpolys","v",&v,&v,vect,&n,&m,&v,x,y,&dv,&dv,7L,2L);
}


/*------------------------------------------------
 * xfpolys(xpols,ypols,[fill])
 *-----------------------------------------------*/   
void Xfpolys ( integer * vect,
               integer   val1,
               integer   val2,
               integer   n   ,
               integer   m   ,
               double  * x   ,
               double  * y   )
{ 
     integer v; 
     double dv;
   
     C2F(dr1)("xliness","v",&val1,&val2,vect,&n,&m,&v,x,y,&dv,&dv,8L,2L);
     
}
/*-----------------------------------------------------------
 *   xsegs(xv,yv,[style])
 *-----------------------------------------------------------*/
void Xsegs ( integer * style,
             integer   flag ,
             integer   n    ,
             double  * x    ,
             double  * y    ,
             double    arsize )
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
void Xstring ( char          * fname    ,
               unsigned long   fname_len,
               integer         str      ,
               double          x        ,
               double          y        ,
               double          angle    ,
               double        * box       )
{ 
     integer v; 
     double dv;
   
     C2F(dr1)("xstring",fname,&v,&v,&v,&str,&v,&v,&x,&y,&angle,&dv,8L,fname_len);
     C2F(dr1)("xstringl",fname,&v,&v,&v,&v,&v,&v,&x,&y,box,&dv,9L,fname_len);
}

/*------------------------------------------------
 *  Xtitle(str)
 *-----------------------------------------------*/   
void Xtitle ( char * str, int n )
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
void Xdrawaxis ( char      dir      ,
                 char      tics     ,
                 double  * x        ,
                 int     * nx       ,
                 double  * y        ,
                 int     * ny       ,
                 char   ** val      ,
                 int       sub_int  ,
                 char    * format   ,
                 int       fontsize ,
                 int       textcolor,
                 int       ticscolor,
                 char      flag     ,
                 int       seg_flag  )
{
  int fontstyle = 0;
     sci_axis(dir,tics,x,nx,y,ny,val,sub_int,format,fontsize,textcolor,fontstyle,ticscolor,flag,seg_flag); 
     
}

/*------------------------------------------------
 *  plot2d(x,y,[style,strf,leg,rect,nax]) 
 *-----------------------------------------------*/   
void Xplot2d ( double    x[]      ,
               double    y[]      ,
               integer * n1       ,
               integer * n2       ,
               integer   style[]  ,
               char      strflag[],
               char      legend[] ,
               double    brect[]  ,
               integer   aaint[]   )
{ 
  
     C2F(plot2d)(x,y,n1,n2,style,strflag,legend,brect,aaint,4L, bsiz);
     
}


/*------------------------------------------------
 *  grayplot(x,y,z,[strf,rect,nax])
 *-----------------------------------------------*/   
void Xgrayplot ( double    x[]      ,
                 double    y[]      ,
                 double    z[]      ,
                 integer * n1       ,
                 integer * n2       ,
                 char      strflag[],
                 double    brect[]  ,
                 integer   aaint[]   )
{ 
  C2F(xgray)(x,y,z,n1,n2,strflag, brect, aaint, 0,bsiz);
}
/*------------------------------------------------
 *  matplot(z,[strf,rect,nax]) 
 *-----------------------------------------------*/   
void Xmatplot ( double    z[]      ,
                integer * n1       ,
                integer * n2       ,
                char      strflag[],
                double    brect[]  ,
                integer   aaint[]   )
{
  C2F(xgray1)(z,n1,n2,strflag, brect, aaint, 0,bsiz);
}
/*------------------------------------------------
 *  Matplot1 (z,xrect) 
 *-----------------------------------------------*/   
void Xmatplot1 ( double    z[],
                 integer * n1 ,
                 integer * n2 ,
                 double    xrect[] ) 
{ 
  C2F(xgray2)(z, n1, n2,xrect);
}
/*------------------------------------------------
 *  3D Plotting of surfaces given by z=f(x,y)
 *-----------------------------------------------*/   
void Xplot3d ( char    * fname ,
               integer * isfac ,
               integer * izcol ,
               double    x[]   ,
               double    y[]   ,
               double    z[]   ,
               integer * zcol  ,
               integer * m     ,
               integer * n     ,
               double  * theta ,
               double  * alpha ,
               char    * legend,
               integer * iflag ,
               double  * ebox   )
{ 
    

   if (*isfac== 1) 
    { 
       if (*izcol == 0) 
	 { if (strcmp(fname,"plot3d1")==0)
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
    { if (strcmp(fname,"plot3d1")==0)
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
void Xnumb ( char    * fname,
             integer   n    ,
             integer   flag ,
             double  * x    ,
             double  * y    ,
             double  * angle,
             double  * box   )
{ 
     integer v; 
    
     C2F(dr1)("xnum","xv",&v,&v,&v,&v,&n,&flag,x,y,angle,box,5L,3L);  
}

/*------------------------------------------------
 * fec(x,y,triangles,func,[strf,leg,rect,nax,zminmax,colminmax,extremes_col, with_mesh]);
 *-----------------------------------------------*/   
void Xfec ( double    x[]        ,
            double    y[]        ,
            double    noeud[]    ,
            double  * fun        ,
            integer * n          ,
            integer * m          ,
            char      strflag[]  ,
            char      legend[]   ,
            double    brect[]    ,
            integer   aaint[]    ,
            double    Zminmax[]  ,
            integer   Colminmax[],
            integer   ColOut[]   ,
            int       WithMesh    )
{ 
    
     C2F(fec)(x,y,noeud,fun,n,m,strflag,legend,brect,aaint,
         Zminmax,Colminmax,ColOut,WithMesh,0,4L,bsiz);
     
}
