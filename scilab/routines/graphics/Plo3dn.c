/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 2003-2004 Inria/Djalel Abdemouche
 --------------------------------------------------------------------------*/

#include <string.h> 
#include <math.h>
#include <stdio.h>
#include "Math.h"
#include "PloEch.h"
#include "Entities.h"    

extern double C2F(dsort)();
/*-------------------------------------------------------------------------
 *   plo3dn function  ( 3D Plotting of surfaces given by z=f(x,y)  )
 *-------------------------------------------------------------------------*/

void C2F(plot3dn)(sciPointObj *pobj, double *x, double *y, double *z, integer *p, integer *q)
{
  static integer fg1,dc;
  integer polysize,npoly,whiteid,verbose=0,narg;
  integer *polyx,*polyy,*fill;
  static integer cache;
  static double zmin,zmax;
  integer i,j,flagcolor;  
  sciPointObj *psubwin;

  psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ()); 

  zmin  = pSUBWIN_FEATURE(psubwin)->SRect[4]; /*zmin*/
  zmax  = pSUBWIN_FEATURE(psubwin)->SRect[5]; /*zmax*/
  cache = pSUBWIN_FEATURE(psubwin)->hiddenstate;

  fg1       = pSURFACE_FEATURE (pobj)->hiddencolor;   
  dc        = pSURFACE_FEATURE (pobj)->flag[0];
  flagcolor = pSURFACE_FEATURE (pobj)->flagcolor;

   /** initialisation **/
  polyx = graphic_alloc(0,5*(*q),sizeof(int));
  polyy = graphic_alloc(1,5*(*q),sizeof(int));
  fill  = graphic_alloc(2,(*q),sizeof(int));
  if ( (polyx == NULL) || (polyy == NULL) || (fill  == NULL)) 
    {
      Scistring("plot3dg_ : malloc No more Place\n");
      return;
    }
 

  C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  /*  if (fg1<=0) fg1=dc;*/
  for ( i =0 ; i < (*q)-1 ; i++)   fill[i]= dc ;
  polysize=5;npoly= (*q)-1;

  /** The 3d plot **/
  /** Choix de l'ordre de parcourt **/
  switch (cache)
    {
    case 0 : 
      for ( i =0 ; i < (*p)-1 ; i++)
	{
	  int npolyok=0;
	  for ( j =0 ; j < (*q)-1 ; j++)
	    {
	     npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax,
				x,y,z,i,j,npolyok,p,dc,fg1);
	    }
	  if ( npolyok != 0) 
	    C2F(dr)("xliness","str",polyx,polyy,fill,&npolyok,&polysize
		    ,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      break;
    case 1 : 
      for ( i =0 ; i < (*p)-1 ; i++)
	{
	  int npolyok=0;
	  for ( j =0  ; j < (*q)-1  ; j++)
	    {
	      npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax,
				 x,y,z,i,(*q)-2-j,npolyok,p,dc,fg1);
	   }
	  if ( npolyok != 0) 
	    C2F(dr)("xliness","str",polyx,polyy,fill,&npolyok,&polysize
		    ,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      break;
    case 2 : 
      for ( i =(*p)-2 ; i >=0  ; i--)
	{
	  int npolyok=0;
	  for ( j = 0 ; j < (*q)-1 ; j++)
	    {
	     npolyok +=     (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax,
				    x,y,z,i,(*q)-2-j,npolyok,p,dc,fg1);
	   }
	  if ( npolyok != 0) 
	    C2F(dr)("xliness","str",polyx,polyy,fill,&npolyok,&polysize
		    ,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      break;
    case 3 : 
      for ( i =(*p)-2 ; i >=0  ; i--)
	{
	  int npolyok=0;
	  for ( j =0 ; j < (*q)-1 ; j++)
	    {
	     npolyok += (Gen3DPoints)(flagcolor,polyx,polyy,fill,whiteid,zmin,zmax,
				x,y,z,i,j,npolyok,p,dc,fg1);
	   }
	  if ( npolyok != 0) 
	    C2F(dr)("xliness","str",polyx,polyy,fill,&npolyok,&polysize
		    ,PI0,PD0,PD0,PD0,PD0,0L,0L);
	}
      break;
    }

}

void C2F(fac3dn)(sciPointObj *pobj, double *x, double *y, double *z, integer *cvect, integer *p, integer *q)
{
  integer polysize,npoly,whiteid,verbose=0,narg,fg1;
  integer *polyx,*polyy,*locindex,fill[4]; 
  static double zmin,zmax,*polyz;
  integer i,flag,iflag; 
  sciPointObj *psubwin;
 

  psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ()); 

  zmin  = pSUBWIN_FEATURE(psubwin)->SRect[4]; /*zmin*/
  zmax  = pSUBWIN_FEATURE(psubwin)->SRect[5]; /*zmax*/
 
  fg1        = pSURFACE_FEATURE (pobj)->hiddencolor; 
  flag       = pSURFACE_FEATURE (pobj)->flag[0];
  iflag      = pSURFACE_FEATURE (pobj)->flagcolor;

  polyz = graphic_alloc(5,(*q),sizeof(double));
  if ( (polyz == NULL) && (*q) != 0)
    {
      Scistring("plot3dg_ : malloc No more Place\n");
      return;
    }
  /** Allocation  **/
  polyx = graphic_alloc(0,(*p)+1L,sizeof(int));
  polyy = graphic_alloc(1,(*p)+1L,sizeof(int));
  locindex = graphic_alloc(2,(*q),sizeof(int));
  if ( ( polyx == NULL) ||  ( polyy== NULL) || ( locindex== NULL) )
    {
      Scistring("plot3dg_ : malloc No more Place\n");
      return;
    }

  /** le tri des facettes **/
  for ( i =0 ; i < *q ; i++)
    {
      double zdmin1, zdmin,xmoy=0.00,ymoy=0.00,zmoy=0.00;
      int j=0 ;
      zdmin1=  TRZ(x[ (*p)*i]  ,y[(*p)*i]  ,z[(*p)*i]);
      for ( j= 0 ; j < *p ; j++) 
	{
	  xmoy += x[ j +(*p)*i];  ymoy += y[ j +(*p)*i];  zmoy += z[ j +(*p)*i];
	  zdmin =  TRZ(x[ j +(*p)*i]  ,y[ j +(*p)*i]  ,z[ j +(*p)*i]);
	  if ( zdmin1 < zdmin ) zdmin1= zdmin;
	}
         polyz[i]=  TRZ(xmoy,ymoy,zmoy);
    }
  C2F(dsort)(polyz,q,locindex); 
  for ( i =0 ; i < (*q) ; i++)
    {
      locindex[i] -= 1;  
      if ( locindex[i] >= *q) 
	sciprint (" index[%d]=%d\r\n",i,locindex[i]);
      locindex[i] = Min(Max(0,locindex[i]),*q-1);
    }
  polysize=(*p)+1;
  npoly=1; 

  C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  fill[0]=  flag; 
  /*if (fg1<=0) fg1= flag;*/
 
  for ( i = (*q)-1 ; i>= 0 ; i--)
    {
      int j,nok=0;
      for ( j =0 ; j < (*p) ; j++)
	{ 
	if (trans3d(psubwin ,1, &(polyx[j]),&(polyy[j]),&(x[(*p)*locindex[i]+j]),
		&(y[(*p)*locindex[i]+j]),&(z[(*p)*locindex[i]+j]))==0)
	    {
	      nok=1;break; 
	    }
	}
      if ( nok == 0) 
	{
	  polyx[(*p)]=polyx[0];
	  polyy[(*p)]=polyy[0];
	  fill[0]=  flag;
	   if (( *p >= 2 && ((polyx[1]-polyx[0])*(polyy[2]-polyy[0])-
			     (polyy[1]-polyy[0])*(polyx[2]-polyx[0])) <  0) && (fg1>=0))
	    { 
	      if (iflag != 0)
		fill[0] = (flag < 0 ) ? -fg1 : fg1 ;
	      else
		fill[0] = (flag != 0 ) ? fg1 : flag ;
	      /*if (flag==0) fill[0]=0;*/
	       if (fill[0]>0) 
		{
		  C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&polysize,
			  PI0,PD0,PD0,PD0,PD0,0L,0L); 
		}
	    }
	  else if ( iflag == 1) 
	    {
	      double zl=0;
	      int k;
	      for ( k= 0 ; k < *p ; k++) zl+= z[(*p)*locindex[i]+k];

	      fill[0]=inint((whiteid-1)*((zl/(*p))-zmin)/(zmax-zmin))+1;
	      if ( flag < 0 ) fill[0]=-fill[0];
	      C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&polysize ,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    }
	  else if ( iflag == 2) 
	    {
	      fill[0]= cvect[locindex[i]];
	      if ( flag < 0 ) fill[0]=-fill[0];
	      C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&polysize ,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    }
	  else if (iflag ==3) { 
	      int k;
              ;
	      if ( (*p) != 3 && (*p) !=4 ) {
                Scistring("plot3d1 : interpolated shading is only allowed for polygons with 3 or 4 vertices\n");
 		return;
	      } else {
       	        for ( k= 0 ; k < *p ; k++) fill[k]= cvect[(*p)*locindex[i]+k];
		 shade(polyx,polyy,fill,*p,flag);
	      }
	  }
	  else C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&polysize ,PI0,PD0,PD0,PD0,PD0,0L,0L);

	}
    } 
}


