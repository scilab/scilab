/*------------------------------------------------------------------------*/
/* file: drawLegendEntity.c                                               */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a legend handle               */
/*------------------------------------------------------------------------*/

#include "drawLegendEntity.h"
#include "GetProperty.h"
#include "Xcall1.h"
#include "periScreen.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "PloEch.h"

void Legends( integer * style, integer * n1, char * legend ) ;

/*----------------------------------------------------------------------------------------*/
/**
 *  legend="leg1@leg2@leg3@...."             
 * legend contain legends separated by '@'
 * if nlegend is the number of legends stored in legend
 * then the function Legends draw  Min(*n1,6,nlegends) legends
 */
void Legends( integer * style, integer * n1, char * legend )
{
  int rect[4] ;
  int xx = 0  ;
  int yy = 0  ;
  char *leg,*loc;
  double xi,xi1,yi,yi1,xoffset,yoffset;  
  int i;
  integer verbose=0,narg,xz[10],fg;

  loc = MALLOC( (strlen(legend)+1)*sizeof(char));
  if ( loc == NULL )
  {
    sciprint( "Impossible to draw legends, memory full.\n" ) ;
    return ;
  }


  C2F(dr)("xstringl","pl",&xx,&yy,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);	

  C2F(dr)("xget","foreground",&verbose,&fg,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xget","line style",&verbose,xz,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","line style",(i=1,&i),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  C2F(dr)("xget","color",&verbose,xz+6,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset","color",&fg,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  strcpy(loc,legend);

  /* length for the tick zone associated to the legend */
  xoffset= (Cscale.wdim[0]*Cscale.subwin_rect[2]*(1- Cscale.axis[0] - Cscale.axis[1]))/12.0;
  /* y offset between legends */
  yoffset= (Cscale.wdim[1]*Cscale.subwin_rect[3]*Cscale.axis[3])/5.0;

  /* x position of the legends in pixel if n <= 3 */ 
  xi = Cscale.wdim[0]*Cscale.subwin_rect[2]*Cscale.axis[0]
  + Cscale.subwin_rect[0]*Cscale.wdim[0];
  /* x position of the legends in pixel if n > 3 */ 
  xi1 = xi + Cscale.wdim[0]*Cscale.subwin_rect[2]*(1.0 - (Cscale.axis[0]+Cscale.axis[1]))/2.0 ;

  /* y position of x-axis in pixel */
  yi1 = Cscale.wdim[1]*Cscale.subwin_rect[3]*Cscale.axis[2]
  + Cscale.subwin_rect[1]*Cscale.wdim[1] 
  + Cscale.subwin_rect[3]*Cscale.wdim[1]*(1.0 - (Cscale.axis[2]+Cscale.axis[3])) ; /* ouf !! */

  for ( i = 0 ; i < *n1 && i < 6 ; i++)
  {  
    integer xs,ys,flag=0,polyx[2],polyy[2],lstyle[1];
    double angle=0.0;

    if (i >= 3)
    { 
      /* down left point for string display */
      xi= xi1;
      yi= yi1 +(i-3)*yoffset+3.0*yoffset;}
    else
    { 
      yi= yi1 + (i)*yoffset+3.0*yoffset;
    }
    xs=inint(xi+1.2*xoffset);
    ys=inint(yi);

    if ( i==0) leg=strtok(loc,"@"); else leg=strtok((char *)0,"@");
    if (leg != 0) 
    {
      C2F(dr)("xset","color",&fg,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xstring",leg,&xs,&ys,PI0,&flag,PI0,PI0,&angle,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xset","color",xz+6,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      if (style[i] > 0)
      { 
        integer n=1,p=2;
        polyx[0]=inint(xi);polyx[1]=inint(xi+xoffset);
        polyy[0]=inint(yi - rect[3]/2.0);polyy[1]=polyy[0];
        lstyle[0]=style[i];
        C2F(dr)("xpolys","v",polyx,polyy,lstyle,&n,&p
          ,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
      else
      { 
        integer n=1,p=1;
        polyx[0]=inint(xi+xoffset);
        polyy[0]=inint(yi- rect[3]/2.0);
        lstyle[0]=style[i];
        C2F(dr)("xpolys","v",polyx,polyy,lstyle,&n,&p
          ,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    }
  }
  FREE(loc);
  C2F(dr)("xset","line style",xz,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

}


/*----------------------------------------------------------------------------------------*/
int drawLegendEntity( sciPointObj * pObj )
{
  sciLegend * ppLegend = pLEGEND_FEATURE(pObj) ;

  int verbose = 0 ;
  int foreground = sciGetForeground( pObj ) ;
  int fontWidth = sciGetFontDeciWidth( pObj ) / 100 ;
  int fontStyle = sciGetFontStyle( pObj ) ;
  int oldForeground = 0 ;
  int oldDashStyle = 0 ;
  int v = 0 ;
  double dv = 0.0 ;
  int i ;

  if ( !sciGetVisibility(pObj) ) { return 0 ; }

  C2F(dr1)("xget", "dashes", &verbose, &oldDashStyle, &v, &v, &v,&v, &dv, &dv, &dv, &dv, 5L, 4096);
  C2F(dr1)("xget", "foreground", &verbose, &oldForeground, &v, &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 4096);


#ifdef _MSC_VER 
  flag_DO=MaybeSetWinhdc();
#endif
  C2F (dr1) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr1) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F(dr)("xset","font",&fontStyle,&fontWidth,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);


  /*permet la mise a jour des legendes correspondantes aux entites associees */
  for ( i = 0; i < ppLegend->nblegends; i++ )
  {
    sciPointObj * curObj = ppLegend->pptabofpointobj[i] ;
    if ( sciGetIsMark( curObj ) )
    {
      ppLegend->pstyle[i] = -sciGetMarkStyle( curObj );
    }
    else
    {
      ppLegend->pstyle[i] = sciGetForeground( curObj );
    }
  }
  Legends( ppLegend->pstyle, &(ppLegend->nblegends), getStrMatElement(sciGetText(pObj),0,0) ) ;


  /* restore the graphic context */

  C2F (dr1) ("xset", "dashes", &oldDashStyle, &v, &v, &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 6L);
  C2F (dr1) ("xset", "foreground", &oldForeground, &v, &v, &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 10L);
#ifdef _MSC_VER 
  if ( flag_DO == 1) ReleaseWinHdc ();
#endif

  return 0 ;

}
/*----------------------------------------------------------------------------------------*/
