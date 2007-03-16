/*------------------------------------------------------------------------*/
/* file: drawSurfaceEntity.h                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a surface handle              */
/*------------------------------------------------------------------------*/

#include "drawSurfaceEntity.h"
#include "GetProperty.h"
#include "periScreen.h"
#include "Xcall1.h"
#include "Plo3dn.h"

/*--------------------------------------------------------------------------------------*/
int drawSurfaceEntity( sciPointObj * pObj )
{
  sciSurface  * ppSurface = pSURFACE_FEATURE(pObj) ;
  sciPointObj * parentSubWin = sciGetParentSubwin(pObj) ;
  int lineWidth = sciGetLineWidth( pObj ) ;
  int lineStyle = sciGetLineStyle( pObj ) ;
  int markStyle = sciGetMarkStyle( pObj ) ;
  int DPI[2] ;

  if (!sciGetVisibility(pObj)) { return 0 ; }
    
  GetDPIFromDriver(DPI) ;
  
#ifdef _MSC_VER
  flag_DO = MaybeSetWinhdc();
#endif
  C2F (dr) ("xset", "thickness",  &lineWidth, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 4L, 9L);
  C2F (dr) ("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 4L, 10L); /*D.A 17/12/2003*/
  C2F (dr) ("xset", "mark", &markStyle, &lineWidth, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 4L, 4L);
#ifdef _MSC_VER
  if ( flag_DO == 1) { ReleaseWinHdc() ; }
#endif

#ifdef _MSC_VER
  flag_DO = MaybeSetWinhdc();
#endif
  switch(ppSurface->typeof3d)
  {
  case SCI_FAC3D:
    C2F(fac3dn)(pObj,ppSurface->pvecx,ppSurface->pvecy,
      ppSurface->pvecz,
      ppSurface->color,
      &ppSurface->dimzx,&ppSurface->dimzy, DPI);

    break;
  case SCI_PLOT3D:
    C2F(plot3dn)(pObj,ppSurface->pvecx,ppSurface->pvecy,
      ppSurface->pvecz,
      &ppSurface->dimzx,&ppSurface->dimzy, DPI);
    break;
  default:
    break;
  }
#ifdef _MSC_VER
  if ( flag_DO == 1) { ReleaseWinHdc() ; }
#endif

  return 0 ;

}
/*--------------------------------------------------------------------------------------*/
