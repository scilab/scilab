/*------------------------------------------------------------------------*/
/* file: DrawMarks.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : Routines to draw marks in the graphic window                    */
/*------------------------------------------------------------------------*/

#include "drawMarks.h"
#include "math_graphics.h"
#include "GetProperty.h"
#include "Xcall1.h"

/*--------------------------------------------------------------------------------------------------*/
int DrawNewMarks(sciPointObj * pobj, int n1, int *xm, int *ym, int *DPI)
{
  int style = sciGetMarkStyle(pobj);
  double size = (double) sciGetMarkSize(pobj);
  int foreground = sciGetMarkForeground(pobj);
  int background = sciGetMarkBackground(pobj);
  int i;

  int v = 0 ;
  double dv = 0.0 ;
  int flagx = 0;
  int old_foreground;

  int thick;
  int linestyle[6];
  int tabulated_marksize[] = {8,10,12,14,18,24};

  integer verbose = 0,old_thick, old_linestyle[6],narg = 0;
  int ixres = DPI[0]; /* only the x DPI is used here : */

  int pixel_offset = CheckPixelStatus() ; 
  double size_plus_one = 0.;
  double size_minus_one = 0.;


  linestyle[0] = 1;

  if( sciGetMarkSizeUnit(pobj) == 2 )
  {
    /* tabulated */ /* size is given relative to the array tabulated_marksize */
    if( (int) size <= 5)
    {
      if(style == 0 && size == 0)
      {
        size = 1; /* Singularity : To allow the drawing of a single pixel in old mode (tabulated) */
      }
      else
      {
        size = tabulated_marksize[(int) size] ;
      }
    }
    else
    {
      size = tabulated_marksize[5];
    }
  }

  size_plus_one = size + 1.;
  size_minus_one = size - 1.;

  /* We get the DPIs value : only the x DPI is used here : */
  /* we assum we have a square pixel (i.e. xDPI == yDPI or almost) */
  size = size * (ixres/72.);
  size_minus_one = size_minus_one * (ixres/72.);
  size_plus_one  = size_plus_one  * (ixres/72.);

  C2F (dr) ("xget", "foreground", &flagx, &old_foreground, &v, &v, &v,
    &v, &dv, &dv, &dv, &dv, 5L, 4096);

  C2F(dr)("xget","thickness",&verbose,&old_thick,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  C2F(dr)("xget","line style",&verbose,old_linestyle,&narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);

  for( i = 1 ; i < 6 ; i++ ) { linestyle[i] = old_linestyle[i] ; }

  /* My default for marks drawing */
  C2F (dr) ("xset", "line style", linestyle, PI0, PI0, PI0, PI0, PI0, PD0,
    PD0, PD0, PD0, 0L, 0L);

  thick = sciGetLineWidth(pobj) ; /* to answer the request from newsgroup to have marks thickness == line thickness every time */
  if( thick < 1 ) { thick = 1 ; }

  C2F(dr)("xset","thickness",&thick,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  /* end of default*/

  if( size < 2 )
  {
    /* represents a . (pixel) with editable foreground */
    for( i = 0 ; i < n1 ; i++ )
    {
      int two = 2 ;
      int closeflag = 0 ;
      int xmasterix[2];
      int ymasterix[2];

      C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
        &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
        &dv, &dv, &dv, &dv, 5L, 4096);

      xmasterix[0] = xm[i];
      xmasterix[1] = xm[i] + pixel_offset;
      ymasterix[0] = ym[i] ;
      ymasterix[1] = ym[i] ;
      C2F (dr) ("xlines", "xv", &two, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
    }
    return 0;
  }

 /* foreground = sciGetMarkForeground(pobj);
  background = sciGetMarkBackground(pobj);*/

  switch( style )
  {
  case 0:
    /* represents a simple full dot with editable foreground */
    for(i=0;i<n1;i++)
    {
      DrawMark_FullDot(xm[i], ym[i], size_minus_one, foreground, foreground, pixel_offset);
    }
    break;
  case 1:
    /* represents a plus with editable foreground */
    for(i=0;i<n1;i++)
    {
      DrawMark_Plus(xm[i], ym[i], (int)(size/2), foreground, pixel_offset);
    }
    break;
  case 2:
    /* represents a cross with editable foreground */
    for(i=0;i<n1;i++)
    {
      DrawMark_Cross(xm[i], ym[i], (int)(size/2), foreground, pixel_offset);
    }
    break;
  case 3:

    /* represents a circle AND a plus within this circle with editable foreground and background */
    for(i=0;i<n1;i++)
    {
      DrawMark_FullDot(xm[i], ym[i], size_minus_one, foreground, background, pixel_offset);
      DrawMark_Plus(xm[i], ym[i], (int)(size/2), foreground, pixel_offset);
    }
    break;
  case 4:
    /* represents a diamond with background == foreground  */
    for(i=0;i<n1;i++)
    {
      DrawMark_FullDiamond(xm[i], ym[i], (int)(size/2), foreground, foreground);
    }
    break;
  case 5:
    /* represents a diamond with both editable foreground and background  */
    for(i=0;i<n1;i++)
    {
      DrawMark_FullDiamond(xm[i], ym[i], (int)(size/2), foreground, background);
    }
    break;
  case 6:
    /* represents an upward-pointing triangle with both editable foreground and background  */
    for(i=0;i<n1;i++)
    {
      DrawMark_FullTriangleUp(xm[i], ym[i], (int)size, foreground, background);
    }
    break;
  case 7:
    /* represents a downward-pointing triangle with both editable foreground and background  */
    for(i=0;i<n1;i++)
    {
      DrawMark_FullTriangleDown(xm[i], ym[i], (int)size, foreground, background);
    }
    break;
  case 8:
    /* represents a diamond with a plus inside with both editable foreground and background  */
    for(i=0;i<n1;i++)
    {	
      DrawMark_FullDiamond(xm[i], ym[i], (int)(size/2), foreground, background);
      DrawMark_Plus(xm[i], ym[i], (int)(size/2), foreground, pixel_offset);
    }
    break;
  case 9:
    /* represents a circle with editable foreground and background */
    for(i=0;i<n1;i++)
    {
      DrawMark_FullDot(xm[i], ym[i], size_minus_one, foreground, background, pixel_offset);
    }
    break;
  case 10:
    /* Asterix case : we can modify only the foreground (background changes have no impact) */
    for(i=0;i<n1;i++)
    {
      DrawMark_Asterisk(xm[i], ym[i],(int) (size/2), foreground, pixel_offset);
    }
    break;
  case 11:
    /* represents a square with editable foreground and background */
    for(i=0;i<n1;i++)
    {
      DrawMark_FullSquare(xm[i], ym[i],(int) size_minus_one, foreground, background, pixel_offset);
    }
    break;
  case 12:
    /* represents a right-pointing triangle with both editable foreground and background  */
    for(i=0;i<n1;i++)
    {
      DrawMark_FullTriangleRight(xm[i], ym[i],(int) size, foreground, background);
    }
    break;
  case 13:
    /* represents a left-pointing triangle with both editable foreground and background  */
    for(i=0;i<n1;i++)
    {
      DrawMark_FullTriangleLeft(xm[i], ym[i],(int) size, foreground, background);
    }
    break;
  case 14:
    /* represents a pentagram with both editable foreground and background  */
    for(i=0;i<n1;i++)
    {
      DrawMark_FullPentagram(xm[i], ym[i], (int)(size/2), foreground, background);
    }
    break;
  default:
    break;
  }

  C2F (dr) ("xset", "line style", old_linestyle, PI0, PI0, PI0, PI0, PI0, PD0,
    PD0, PD0, PD0, 0L, 0L);

  C2F(dr)("xset","thickness",&old_thick,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  C2F (dr) ("xset", "dashes", &old_foreground, &old_foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &old_foreground, &old_foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  return 0; 
}
/*--------------------------------------------------------------------------------------------------*/
void DrawMarks3D(sciPointObj *pobj, int n1, int *xm, int *ym, int *DPI)
{
  if ( sciGetIsMark(pobj) )
  {
    integer v = 0 ;
    double dv = 0.0 ;
    int markForeground = sciGetMarkForeground(pobj);
    int markStyle = sciGetMarkStyle( pobj ) ;
    int markSize = sciGetMarkSize( pobj ) ;

    C2F (dr) ("xset", "dashes", &markForeground, &markForeground, &v, &v, &v, &v, &dv,
      &dv, &dv, &dv, 5L, 4096);
    C2F (dr) ("xset", "foreground", &markForeground, &markForeground, &v, &v, &v, &v,
      &dv, &dv, &dv, &dv, 5L, 4096);

    C2F (dr) ("xset", "mark", &markStyle, &markSize, PI0, PI0, PI0, PI0, PD0, PD0,
      PD0, PD0, 0L, 0L) ;

    DrawNewMarks(pobj,n1,xm,ym,DPI);
  }
}
/*--------------------------------------------------------------------------------------------------*/
/* center : xmi,ymi */
/* diameter : size */
int DrawMark_FullDot(int xmi, int ymi, double size, int foreground, int background, int pixel_offset)
{
  int x1 = (int) (xmi - size/2);
  int yy1= (int) (ymi - size/2);
  int w1 = (int) size+pixel_offset;
  int h1 = (int) size+pixel_offset; 
  integer v = 0;
  double dv = 0.0 ;

  integer angle1 = 0;
  integer angle2 = 64*360;

  C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xfarc", "xv", &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 0L);

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xarc", "xv", &x1, &yy1, &w1, &h1, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 0L);

  return 0;
}
/*--------------------------------------------------------------------------------------------------*/
/* center : xmi,ymi */
/* "+" half-size (minus centered pixel) : size */
int DrawMark_Plus(int xmi, int ymi, int size, int foreground, int pixel_offset)
{
  int two = 2 ;
  int xmasterix[2];
  int ymasterix[2];
  int closeflag=0;
  int v = 0 ;
  double dv = 0.0;

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  /* the "+" */
  xmasterix[0] = xmi -((int) size) ;
  xmasterix[1] = xmi +(int) size + pixel_offset;
  ymasterix[0] = 	ymasterix[1] = ymi;
  C2F (dr) ("xlines", "xv", &two, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);


  xmasterix[0] = xmasterix[1] = xmi;
  ymasterix[0] = ymi - (int) size;
  ymasterix[1] = ymi + (int) size + pixel_offset;
  C2F (dr) ("xlines", "xv", &two, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);

  return 0;
}
/*--------------------------------------------------------------------------------------------------*/
/* center : xmi,ymi */
/* "x" half-size (minus centered pixel) : size */
int DrawMark_Cross(int xmi, int ymi, int size, int foreground, int pixel_offset)
{
  int two = 2;
  int xmasterix[2];
  int ymasterix[2];
  int v = 0 ;
  int closeflag=0;
  double dv = 0.0 ;

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  /* the "/" */
  xmasterix[0] = (int) (xmi - size);
  xmasterix[1] = (int) (xmi + size + pixel_offset);
  ymasterix[0] = (int) (ymi + size);
  ymasterix[1] = (int) (ymi - size - pixel_offset);

  C2F (dr) ("xlines", "xv", &two, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  /* end */

  /* the "\" */
  xmasterix[0] = (int) (xmi - size);
  xmasterix[1] = (int) (xmi + size + pixel_offset);
  ymasterix[0] = (int) (ymi - size);
  ymasterix[1] = (int) (ymi + size + pixel_offset);
  C2F (dr) ("xlines", "xv", &two, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  /* end */

  return 0;
}
/*--------------------------------------------------------------------------------------------------*/
/* center : xmi,ymi */
/* represents a diamond with both editable foreground and background  */
int DrawMark_FullDiamond(int xmi, int ymi, int size, int foreground, int background)
{
  int four = 4 ;
  int one  = 1 ;
  int xmdiamond[4];
  int ymdiamond[4];
  int v = 0 ;
  double dv = 0.0 ;

  xmdiamond[0] = xmi - size;
  xmdiamond[1] = xmi;
  xmdiamond[2] = xmi + size;
  xmdiamond[3] = xmi;

  ymdiamond[0] = ymi;
  ymdiamond[1] = ymi + size;
  ymdiamond[2] = ymi;
  ymdiamond[3] = ymi - size;

  C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xarea", "v", &four, xmdiamond, ymdiamond, &one, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);

  if ( foreground != background )
  {
    C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
      &dv, &dv, &dv, 5L, 4096);
    C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
      &dv, &dv, &dv, &dv, 5L, 4096);

    C2F (dr) ("xlines", "xv", &four, xmdiamond, ymdiamond, &one, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  }

  return 0;
}
/*--------------------------------------------------------------------------------------------------*/
int DrawMark_FullTriangleUp(int xmi, int ymi, int size, int foreground, int background)
{
  int three = 3;
  int one=1;
  int xmtriangle[3];
  int ymtriangle[3];
  int v = 0 ;
  double dv = 0.0 ;

  xmtriangle[0] = (int) (xmi - size/2);
  xmtriangle[1] = (int) (xmi + size/2);
  xmtriangle[2] = xmi;

  ymtriangle[0] = (int) (ymi + size/3);
  ymtriangle[1] = (int) (ymi + size/3);
  ymtriangle[2] = (int) (ymi - 2*size/3);

  C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xarea", "v", &three, xmtriangle, ymtriangle, &one, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);

  if( foreground != background )
  {
    C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
      &dv, &dv, &dv, &dv, 5L, 4096);

    C2F (dr) ("xlines", "xv", &three, xmtriangle, ymtriangle, &one, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  }

  return 0;
}
/*--------------------------------------------------------------------------------------------------*/
int DrawMark_FullTriangleDown(int xmi, int ymi, int size, int foreground, int background)
{
  int three = 3 ;
  int one = 1;
  int xmtriangle[3];
  int ymtriangle[3];
  int v = 0 ;
  double dv = 0.0 ;

  xmtriangle[0] = (int) (xmi - size/2);
  xmtriangle[1] = (int) (xmi + size/2);
  xmtriangle[2] = (int) (xmi);

  ymtriangle[0] = (int) (ymi - size/3);
  ymtriangle[1] = (int) (ymi - size/3);
  ymtriangle[2] = (int) (ymi + 2*size/3);

  C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xarea", "v", &three, xmtriangle, ymtriangle, &one, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xlines", "xv", &three, xmtriangle, ymtriangle, &one, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);

  return 0;
}
/*--------------------------------------------------------------------------------------------------*/
int DrawMark_Asterisk(int xmi, int ymi, int size, int foreground, int pixel_offset)
{
  int two = 2;
  int xmasterix[2];
  int ymasterix[2];
  int sizecross = (int)(0.8*size);
  int v = 0 ;
  int closeflag = 0;
  double dv = 0.0 ;

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  /* the "+" */
  xmasterix[0] = xmi - size;
  xmasterix[1] = xmi + size +pixel_offset;
  ymasterix[0] = ymasterix[1] = ymi;
  C2F (dr) ("xlines", "xv", &two, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);

  xmasterix[0] = xmasterix[1] = xmi;
  ymasterix[0] = ymi - size;
  ymasterix[1] = ymi + size +pixel_offset;
  C2F (dr) ("xlines", "xv", &two, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  /* end */

  /* the "/" */
  xmasterix[0] =(int)( xmi - ceil(sizecross));
  xmasterix[1] =(int)( xmi + ceil(sizecross)+pixel_offset);
  ymasterix[0] = (int)(ymi + ceil(sizecross));
  ymasterix[1] = (int)(ymi - ceil(sizecross)-pixel_offset);
  C2F (dr) ("xlines", "xv", &two, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  /* end */

  /* the "\" */
  xmasterix[0] = (int)(xmi - ceil(sizecross));
  xmasterix[1] = (int)(xmi + ceil(sizecross)+pixel_offset);
  ymasterix[0] = (int)(ymi - ceil(sizecross));
  ymasterix[1] = (int)(ymi + ceil(sizecross)+pixel_offset);
  C2F (dr) ("xlines", "xv", &two, xmasterix, ymasterix, &closeflag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
  /* end */

  return 0;
}
/*--------------------------------------------------------------------------------------------------*/
int DrawMark_FullSquare(int xmi, int ymi, int size, int foreground, int background, int pixel_offset)
{
  int x1 = xmi - size/2;
  int yy1= ymi - size/2;
  int w1 = size;
  int h1 = size;
  int v = 0 ;
  double dv = 0.0 ;

  C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xfrect", "xv", &x1, &yy1, &w1, &h1, PI0, PI0, PD0, PD0, PD0,PD0, 5L, 0L);

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xrect", "xv", &x1, &yy1, &w1, &h1, PI0, PI0, PD0, PD0, PD0,PD0, 5L, 0L);


  return 0;
}
/*--------------------------------------------------------------------------------------------------*/
int DrawMark_FullTriangleRight(int xmi, int ymi, int size, int foreground, int background)
{
  int three = 3 ;
  int one = 1;
  int xmtriangle[3];
  int ymtriangle[3];
  int v = 0 ;
  double dv = 0.0 ;

  xmtriangle[0] = (int) (xmi - size/3);
  xmtriangle[1] = (int) (xmi - size/3);
  xmtriangle[2] = (int) (xmi + 2*size/3);

  ymtriangle[0] = (int) (ymi - size/2);
  ymtriangle[1] = (int) (ymi + size/2);
  ymtriangle[2] = ymi;

  C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xarea", "v", &three, xmtriangle, ymtriangle, &one, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xlines", "xv", &three, xmtriangle, ymtriangle, &one, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);

  return 0;
}
/*--------------------------------------------------------------------------------------------------*/
int DrawMark_FullTriangleLeft(int xmi, int ymi, int size, int foreground, int background)
{

  int three = 3 ;
  int one = 1;
  int xmtriangle[3];
  int ymtriangle[3];
  int v = 0 ;
  double dv = 0.0 ;

  xmtriangle[0] = (int) (xmi + size/3);
  xmtriangle[1] = (int) (xmi + size/3);
  xmtriangle[2] = (int) (xmi - 2*size/3);

  ymtriangle[0] = (int) (ymi - size/2);
  ymtriangle[1] = (int) (ymi + size/2);
  ymtriangle[2] = ymi;

  C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xarea", "v", &three, xmtriangle, ymtriangle, &one, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xlines", "xv", &three, xmtriangle, ymtriangle, &one, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);


  return 0;
}
/*--------------------------------------------------------------------------------------------------*/
int DrawMark_FullPentagram(int xmi, int ymi, int size, int foreground, int background)
{
  int ten = 10, one = 1;
  int xmpoints1[5],ympoints1[5];
  int xmpoints2[5],ympoints2[5];
  int xmpoints[10], ympoints[10];
  int v = 0 ;
  double dv = 0.0 ;
  double angle72 = DEG2RAD(72) ;
  double angle54 = DEG2RAD(54) ;
  double angle36 = DEG2RAD(36) ;

  double r2 = size*cos( angle72 )/cos( angle36 );

  xmpoints1[0] = (int)(xmi - size*cos(angle54));
  ympoints1[0] = (int)(ymi + size*sin(angle54));

  xmpoints1[1] = (int)(xmi + size*cos(angle54));
  ympoints1[1] = (int)(ymi + size*sin(angle54));

  xmpoints1[2] = (int)(xmi + (xmpoints1[1] - xmi) *cos(angle72) + (ympoints1[1] - ymi) *sin(angle72));
  ympoints1[2] = (int)(ymi - (xmpoints1[1] - xmi) *sin(angle72) + (ympoints1[1] - ymi) *cos(angle72));

  xmpoints1[3] = xmi;
  ympoints1[3] = ymi - size;

  xmpoints1[4] = (int)(xmi + (xmpoints1[0] - xmi) *cos(-angle72) + (ympoints1[0] - ymi) *sin(-angle72));
  ympoints1[4] = (int)(ymi - (xmpoints1[0] - xmi) *sin(-angle72) + (ympoints1[0] - ymi) *cos(-angle72));


  xmpoints2[0] = (int)( xmi - r2*sin(angle36));
  ympoints2[0] = (int)(ymi - size*cos(angle72));

  xmpoints2[1] = (int)(xmi + r2*sin(angle36));
  ympoints2[1] = (int)(ymi - size*cos(angle72));

  xmpoints2[2] = (int)(xmi + (xmpoints2[1] - xmi)*cos(-angle72) + (ympoints2[1] - ymi) *sin(-angle72));
  ympoints2[2] = (int)(ymi - (xmpoints2[1] - xmi)*sin(-angle72) + (ympoints2[1] - ymi) *cos(-angle72));

  xmpoints2[3] = xmi;
  ympoints2[3] = (int)(ymi + r2);

  xmpoints2[4] = (int)(xmi + (xmpoints2[0] - xmi)*cos(angle72) + (ympoints2[0] - ymi) *sin(angle72));
  ympoints2[4] = (int)(ymi - (xmpoints2[0] - xmi)*sin(angle72) + (ympoints2[0] - ymi) *cos(angle72));


  xmpoints[0] = xmpoints2[4];
  ympoints[0] = ympoints2[4];

  xmpoints[1] = xmpoints1[0];
  ympoints[1] = ympoints1[0];

  xmpoints[2] = xmpoints2[3];
  ympoints[2] = ympoints2[3];

  xmpoints[3] = xmpoints1[1];
  ympoints[3] = ympoints1[1];

  xmpoints[4] = xmpoints2[2];
  ympoints[4] = ympoints2[2];

  xmpoints[5] = xmpoints1[2];
  ympoints[5] = ympoints1[2];

  xmpoints[6] = xmpoints2[1];
  ympoints[6] = ympoints2[1];

  xmpoints[7] = xmpoints1[3];
  ympoints[7] = ympoints1[3];

  xmpoints[8] = xmpoints2[0];
  ympoints[8] = ympoints2[0];

  xmpoints[9] = xmpoints1[4];
  ympoints[9] = ympoints1[4];


  C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xarea", "v", &ten, xmpoints, ympoints, &one, PI0, PI0, PD0, PD0, PD0, PD0, 5L,0L);

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xlines", "xv", &ten, xmpoints, ympoints, &one, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);

  return 0;
}
/*--------------------------------------------------------------------------------------------------*/
int CheckPixelStatus( void )
{
  if( GetDriverId() == 0 ) /* X11 or Win32 driver */
  { 
    return 1 ;
  }
  else
  {
    return 0 ;
  }
}
/*--------------------------------------------------------------------------------------------------*/
void drawPolyMarks( sciPointObj * pObj, int nbMarks, int xCoord[], int yCoord[], int DPI[2] )
{
  int foreground = sciGetMarkForeground( pObj ) ;
  int v     = 0   ;
  double dv = 0.0 ;
  int markIdSize[2] ;

  markIdSize[0] = sciGetMarkStyle( pObj ) ;
  markIdSize[1] = sciGetMarkSize(  pObj ) ;

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);     
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F (dr) ("xset", "mark", &markIdSize[0], &markIdSize[1], PI0, PI0, PI0, PI0, PD0, PD0,
    PD0, PD0, 0L, 0L);

  DrawNewMarks( pObj, nbMarks, xCoord, yCoord, DPI ) ;
}
/*--------------------------------------------------------------------------------------------------*/
