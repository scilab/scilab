/*----------------------------------------------------------------------------------------*/
/* FILE      : clipping.c                                                                 */
/* COPYRIGHT : INRIA 2006                                                                 */
/* AUTHORS   : Jean-Baptiste Silvy, Jean-Philipe Chancelier                               */
/* DESC.     : Management of clipping when drawing                                        */
/*----------------------------------------------------------------------------------------*/

#include "clipping.h"
#include "Math.h"
#include "GetProperty.h"

/*-------------------------------------------------------------------------------------*/
/* Functions taken from periX11.c used here to work on every plateform                 */
/*-------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------*/

int sciClipPoint(integer x, integer y, SClipRegion * clipping )
{
  integer ret_val = 0;

  if (x < clipping->leftX) ret_val |= (char)0x01;
  else if (x > clipping->rightX) ret_val |= (char)0x02;
  if (y < clipping->bottomY) ret_val |= (char)0x04;
  else if (y > clipping->topY ) ret_val |= (char)0x08;
  return ret_val;
}

/*-------------------------------------------------------------------------------------*/

void sciClipLine( integer       x1      ,
                  integer       yy1     ,
                  integer       x2      ,
                  integer       y2      ,
                  integer     * x1n     ,
                  integer     * yy1n    ,
                  integer     * x2n     ,
                  integer     * y2n     ,
                  integer     * flag    ,
                  SClipRegion * clipping  )
{
  integer  x_intr[2], y_intr[2]; /* used to store intersection points 
				  * at most two points 
				  */
  integer x, y, dx, dy, count, pos1, pos2; 

  *x1n=x1  ;
  *yy1n=yy1;
  *x2n=x2  ;
  *y2n=y2  ;
  *flag=4  ;

  pos1 = sciClipPoint(x1, yy1, clipping ) ;
  pos2 = sciClipPoint(x2, y2 , clipping ) ;
  if (pos1 || pos2) {
    if (pos1 & pos2) { *flag=0;return;}	  
    /* segment is totally out. */

    /* Here part of the segment MAy be inside. test the intersection
     * of this segment with the 4 boundaries for hopefully 2 intersections
     * in. If non found segment is totaly out.
     */
    count = 0;
    dx = x2 - x1;
    dy = y2 - yy1;

    /* Find intersections with the x parallel bbox lines: */
    if (dy != 0) {
      x = (integer) ((int) (clipping->bottomY - y2)  * ((double) dx / (double) dy) + x2);
      /* Test for clipping.bottomY boundary. */
      if (x >= clipping->leftX && x <= clipping->rightX ) {
	x_intr[count] = x;
	y_intr[count++] = clipping->bottomY ;
      }
      x = (integer)((clipping->topY - y2) * ((double) dx / (double) dy) + x2); 
      /* Test for clipping.topY boundary. */
      if ( x >= clipping->leftX && x <= clipping->rightX ) {
	x_intr[count] = x;
	y_intr[count++] = clipping->topY;
      }
    }
    if ( count < 2 )
      {
	/* Find intersections with the y parallel bbox lines: */
	if (dx != 0) {
	  y = (integer)((clipping->leftX - x2) * ((double) dy / (double) dx) + y2);   
	  /* Test for clipping.leftX boundary. */
	  if (y >= clipping->bottomY && y <= clipping->topY) {
	    x_intr[count] = clipping->leftX;
	    y_intr[count++] = y;
	  }
	  if ( count < 2 ) 
	    {
	      y = (integer)((clipping->rightX - x2) * ((double) dy / (double) dx) + y2);  
	      /* Test for clipping.rightX boundary. */
	      if (y >= clipping->bottomY && y <= clipping->topY) {
		x_intr[count] = clipping->rightX;
		y_intr[count++] = y;
	      }
	    }
	}
      }

    if (count == 2) {
      if (pos1 && pos2) {	   /* Both were out - update both */
	*x1n = x_intr[0];
	*yy1n = y_intr[0];
	*x2n = x_intr[1];
	*y2n = y_intr[1];
	*flag=3;return;
      }
      else if (pos1) {       /* Only x1/yy1 was out - update only it */
	if (dx * (x2 - x_intr[0]) + dy * (y2 - y_intr[0]) >= 0) {
	  *x1n = x_intr[0];
	  *yy1n = y_intr[0];
	  *flag=1;return;
	}
	else {
	  *x1n = x_intr[1];
	  *yy1n = y_intr[1];
	  *flag=1;return;
	}
      }
      else {	         /* Only x2/y2 was out - update only it */
	if (dx * (x_intr[0] - x1) + dy * (y_intr[0] - yy1) >= 0) {
	  *x2n = x_intr[0];
	  *y2n = y_intr[0];
	  *flag=2;return;
	}
	else {
	  *x2n = x_intr[1];
	  *y2n = y_intr[1];
	  *flag=2;return;
	}
      }
    }
    else 
      {
	/* count != 0 */
	*flag=0;
        return;
      }
  }
}

/*-------------------------------------------------------------------------------------*/

void sciDrawInsideSegments( integer       iib, 
                            integer       iif, 
                            integer     * vx , 
                            integer     * vy ,
                            SClipRegion * clipping )
{
  integer x1n,y1n,x11n,y11n,x2n,y2n,flag2=0,flag1=0;
  integer npts;
  integer * vxNew ; /* the drawn vector */
  integer * vyNew ;
  int close = 0 ;
  /* backup of the first and last bounds of the bounds of the drawn vector
     which may be changed */
  int prevBounds[4] ;

  npts = ( iib > 0) ? iif-iib+2  : iif-iib+1;  
  
  if ( iib > 0) 
  {
    sciClipLine( vx[iib-1], vy[iib-1],
                 vx[iib]  , vy[iib]  ,
                 &x1n     , &y1n     ,
                 &x2n     , &y2n     ,
                 &flag1   , clipping ) ;
  }
  sciClipLine( vx[iif-1], vy[iif-1],
               vx[iif]  , vy[iif]  ,
               &x11n    , &y11n    ,
               &x2n     , &y2n     ,
               &flag2   , clipping    ) ;
  vxNew = &vx[Max(0,iib-1)] ;
  vyNew = &vy[Max(0,iib-1)] ;

  prevBounds[0] = vxNew[0]      ;
  prevBounds[1] = vyNew[0]      ;
  prevBounds[2] = vxNew[npts-1] ;
  prevBounds[3] = vyNew[npts-1] ;

  if ( iib > 0 && (flag1==1||flag1==3) )
  {
    vxNew[0] = x1n ;
    vyNew[0] = y1n ;
  }
  if ( flag2==2 || flag2==3 )
  {
    vxNew[npts-1] = x2n ;
    vyNew[npts-1] = y2n ;
  }

  
  C2F (dr) ("xuclines", "xv", &npts, vxNew, vyNew, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);

  /* restore the possibly changed bounds */
  vxNew[0]      = prevBounds[0] ;
  vyNew[0]      = prevBounds[1] ;
  vxNew[npts-1] = prevBounds[2] ;
  vyNew[npts-1] = prevBounds[3] ;

}

/*-------------------------------------------------------------------------------------*/

/* draw the segement defined by (vx[index-1],vy[index-1]) (vx[index],vy[index]) */
void sciDrawOutsideSegment( integer index, 
                            integer *vx  ,
                            integer *vy  , 
                            SClipRegion * clipping )
{
  /** The segment is out but can cross the box **/
  integer segX[2],segY[2],flag;
  sciClipLine( vx[index-1], vy[index-1]  ,
               vx[index]  , vy[index]    ,
               &segX[0]   , &segY[0],
               &segX[1]   , &segY[1]  ,
               &flag      , clipping  ) ;
  if ( flag == 3 )
  {
    int nbPoints = 2 ;
    int close    = 0 ;
    C2F (dr) ("xuclines", "xv", &nbPoints, segX, segY, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  }
}

/*-------------------------------------------------------------------------------------*/

/* 
 *  returns the first (vx[.],vy[.]) point inside 
 *  xleft,xright,ybot,ytop bbox. begining at index ideb
 *  or zero if the whole polyline is out 
 */

integer sciFirstInClipRegion( integer       n   ,
                              integer       ideb,
                              integer     * vx  ,
                              integer     * vy  ,
                              SClipRegion * clipping )
{
  integer i;
  for (i=ideb  ; i < n ; i++)
  {
    if (    vx[i] >= clipping->leftX 
         && vx[i] <= clipping->rightX 
         && vy[i] >= clipping->bottomY 
         && vy[i] <= clipping->topY    )
    {
      return(i);
    }
  }
  return(-1);
}

/*-------------------------------------------------------------------------------------*/

/* 
 *  returns the first (vx[.],vy[.]) point outside
 *  xleft,xright,ybot,ytop bbox.
 *  or zero if the whole polyline is out 
 */
integer sciFirstOutClipRegion( integer       n   ,
                               integer       ideb,
                               integer     * vx  ,
                               integer     * vy  ,
                               SClipRegion * clipping )
{
  integer i;
  for ( i=ideb  ; i < n ; i++ )
  {
    if (    vx[i] < clipping->leftX 
         || vx[i] > clipping->rightX  
         || vy[i] < clipping->bottomY
         || vy[i] > clipping->topY   ) 
    {
      return(i);
    }
  }
  return(-1);
}

/*-------------------------------------------------------------------------------------*/

/* check every segment of the polyline and draw only the part which is in the */
/* clip region */
void C2F(clipPolyLine)( integer       n     , 
                        integer     * vx    , 
                        integer     * vy    , 
                        integer       closed,
                        SClipRegion * clipping )
{ 
  integer iib,iif,ideb=0,vxl[2],vyl[2];
  
  while (1) 
  { 
    integer j;
    iib = sciFirstInClipRegion( n, ideb, vx, vy, clipping ) ;
    if (iib == -1)
    { 
      for (j=ideb+1; j < n; j++)
      {
        sciDrawOutsideSegment( j, vx, vy, clipping ) ;
      }
      break;
    }
    else
    { 
      if ( iib - ideb > 1) 
      {
        /* un partie du polygine est totalement out de ideb a iib -1 */
        /* mais peu couper la zone */
        for ( j = ideb + 1 ; j < iib; j++ )
        {
          sciDrawOutsideSegment(j,vx,vy, clipping ) ;
        }
      }
    }
    iif = sciFirstOutClipRegion( n, iib, vx, vy, clipping ) ;
    if ( iif == -1 ) 
    {
      /* special case the polyligne is totaly inside */
      if (iib == 0) 
      {
        C2F (dr) ("xuclines", "xv", &n, vx, vy, &closed, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L) ;
        return ;
      }
      else
      { 
	sciDrawInsideSegments( iib, n-1, vx, vy, clipping ) ;
      }
      break;
    }

    sciDrawInsideSegments( iib, iif, vx, vy, clipping ) ;
    ideb=iif;
  }
  if ( closed )
  {
    /* The polyligne is closed we consider the closing segment */
    integer x1n,y1n,x2n,y2n,flag1=0;
    vxl[0]=vx[n-1];vxl[1]=vx[0];vyl[0]=vy[n-1];vyl[1]=vy[0];
    sciClipLine( vxl[0], vyl[0],
                 vxl[1], vyl[1],
                 &x1n  , &y1n  ,
                 &x2n  , &y2n  ,
                 &flag1, clipping ) ;
    
    if ( flag1==0 ) { return ; }

    if ( flag1==1 || flag1==3 )
    {
      vxl[0] = x1n ;
      vyl[0] = y1n ;
    }
    if ( flag1==2 || flag1==3 )
    {
      vxl[1] = x2n ;
      vyl[1] = y2n ;
    }
    
    {
      int nbPoints = 2 ;
      int close = 0 ;
      C2F (dr) ("xuclines", "xv", &nbPoints, vxl, vyl, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
    }
    
    
  }
}

/*-------------------------------------------------------------------------------------*/
/*
 * @memo get the real clipping (the set one the axe or the window borders) 
 * in pixels of a graphic object
 */
void getPixelClipping( sciPointObj * pObj, SClipRegion * clipping )
{
  double * clipRegion = sciGetClipping( pObj ) ;
  clipping->leftX = XDouble2Pixel( clipRegion[0] ) ;
  clipping->rightX = XDouble2Pixel( clipRegion[0] + clipRegion[2] ) ;
  clipping->bottomY = YDouble2Pixel( clipRegion[1] ) ;
  clipping->topY = YDouble2Pixel( clipRegion[1] - clipRegion[3] ) ;
  
}
/*-------------------------------------------------------------------------------------*/
