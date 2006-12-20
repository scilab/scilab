/*------------------------------------------------------------------------*/
/* file: drawSegsEntity.h                                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a segs handle                  */
/*------------------------------------------------------------------------*/

#include "drawSegsEntity.h"
#include "GetProperty.h"
#include "periScreen.h"
#include "BasicAlgos.h"
#include "PloEch.h"
#include "math_graphics.h"
#include "Xcall1.h"
#include "drawMarks.h"
#include "clipping.h"
#include "MALLOC.h"
#include "axesScale.h"
#include "Champ.h"
#include "MALLOC.h"
#include "clipping.h"

/*---------------------------------------------------------------------------------*/
int drawSegsEntity( sciPointObj * pObj )
{
  sciSegs * ppSegs = pSEGS_FEATURE( pObj ) ;
  sciPointObj * parentSubWin = sciGetParentSubwin( pObj ) ;
  int foreground = sciGetForeground( pObj ) ;
  int lineWidth = sciGetLineWidth( pObj ) ;
  int lineStyle = sciGetLineStyle( pObj ) ;
  int markStyle = sciGetMarkStyle( pObj ) ;
  int v = 0 ;
  double dv = 0.0 ;
  int verbose = 0 ;
  int DPI[2] ;

  if ( !sciGetVisibility(pObj) ) { return 0 ; }

  sciClip(pObj);

  GetDPIFromDriver( DPI ) ;

#ifdef _MSC_VER
  flag_DO = MaybeSetWinhdc() ;
#endif

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "thickness", &lineWidth, PI0, PI0, PI0, PI0, PI0, PD0,
    PD0, PD0, PD0, 0L, 0L);    
  C2F (dr) ("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, PD0,
    PD0, PD0, PD0, 0L, 0L);
  C2F (dr) ("xset", "mark", &markStyle, &lineWidth, PI0, PI0, PI0, PI0, PD0, PD0,
    PD0, PD0, 0L, 0L);
#ifdef _MSC_VER
  if ( flag_DO == 1) { ReleaseWinHdc () ; }
#endif

  if ( ppSegs->ptype == 0 ) /* ptype == 0 F.Leray : This is NOT A champ */
  {  
    int n = ppSegs->Nbr1 ;
    int * xCoords = NULL ;
    int * yCoords = NULL ;
    int * pstyle  = NULL ;
    int i ;

    xCoords = MALLOC( n * sizeof(int) ) ;
    if ( xCoords == NULL )
    {
      sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
      return -1 ;
    }

    yCoords = MALLOC( n * sizeof(int) ) ;
    if ( yCoords == NULL )
    {
      FREE( xCoords ) ;
      sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
      return -1 ;
    }

    if ((pstyle = MALLOC ( (n/2) * sizeof(integer))) == NULL) { return -1 ; } /* SS 19.04*/

    pstyle = MALLOC( (n/2) * sizeof(int) ) ;
    if ( pstyle == NULL )
    {
      FREE( xCoords ) ;
      FREE( yCoords ) ;
      sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
      return -1 ;
    }

    if ( ppSegs->iflag == 1)
    {
      for ( i =0 ; i < (n/2) ; i++ )
      {
        pstyle[i] = sciGetGoodIndex( pObj, ppSegs->pstyle[i] ) ;
      }
    }
    else
    {
      pstyle[0] = sciGetGoodIndex( pObj, ppSegs->pstyle[0]) ;
    }

    if ( sciGetIs3d( parentSubWin ) ) 
    {
      /* 3d mode */
      double * xvect = NULL;
      double * yvect = NULL;
      double * zvect = NULL;

      xvect = createDoubleArrayCopy( ppSegs->vx, n ) ;
      if ( xvect == NULL )
      {
        FREE( xCoords ) ;
        FREE( yCoords ) ;
        FREE( pstyle  ) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      yvect = createDoubleArrayCopy( ppSegs->vy, n ) ;
      if ( yvect == NULL )
      {
        FREE( xCoords ) ;
        FREE( yCoords ) ;
        FREE( pstyle  ) ;
        FREE( xvect   ) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      if ( ppSegs->vz != NULL )
      {
        zvect = createDoubleArrayCopy( ppSegs->vz, n ) ;
        if ( zvect == NULL )
        {
          FREE( xCoords ) ;
          FREE( yCoords ) ;
          FREE( pstyle  ) ;
          FREE( xvect   ) ;
          FREE( yvect   ) ;
          sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
          return -1 ;
        }
      }

      ReverseDataFor3D(parentSubWin,xvect,yvect,zvect,n);

      trans3d(parentSubWin,n,xCoords,yCoords,xvect,yvect,zvect);

      FREE( xvect ) ;
      FREE( yvect ) ;
      if ( zvect != NULL )
      {
        FREE( zvect ) ;
      }
    }
    else
    {
      for ( i = 0 ; i < n ; i++ )
      {
        xCoords[i] = XScale( ppSegs->vx[i] ) ; 
        yCoords[i] = YScale( ppSegs->vy[i] ) ;
      }
    }
#ifdef _MSC_VER 
    flag_DO = MaybeSetWinhdc();
#endif

    if( sciGetIsMark(pObj) )
    {
      drawPolyMarks( pObj, n, xCoords, yCoords, DPI ) ;
    }

    if( sciGetIsLine( pObj ) )
    {

      C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
        &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "thickness", &lineWidth, PI0, PI0, PI0, PI0, PI0, PD0,
        PD0, PD0, PD0, 0L, 0L);    
      C2F (dr) ("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, PD0,
        PD0, PD0, PD0, 0L, 0L);

      if ( ppSegs->arrowsize == 0 )
      {
        /* only lines */
        C2F(dr)("xsegs","v",xCoords,yCoords,&n,pstyle,&ppSegs->iflag,
          PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
      else
      {
        int arrowSize = computeRealArrowSize( pObj, n, xCoords, yCoords ) ;      		    
        C2F(dr)("xarrow","v",xCoords,yCoords,&n,&arrowSize,pstyle,&ppSegs->iflag,PD0,PD0,PD0,PD0,0L,0L);
        /* with C2F(dr)("xarrow",... did not work: why? What does (dr1) routine make more than (dr) in New Graphics mode ?? */
        /* Answer : dr deals with pixels value (data: xm and ym are integers!!) whereas dr1 deals with double value coming from the user */
        /* This is true for old and new graphics mode. */
      }

    }


#ifdef _MSC_VER 
    if ( flag_DO == 1) ReleaseWinHdc ();
#endif 
    FREE( xCoords ) ;
    FREE( yCoords ) ;
    FREE( pstyle  ) ;
  }
  else    /*ptype == 1*/ /* ptype == 1 F.Leray : This IS A champ */
  {
    int xz[10] ;
    int useColor = 0 ;
    int narg = 0 ;
    int nbArrow = 0 ;
    int arrowSize = 0 ;
    int * xCoords = NULL ;
    int * yCoords = NULL ;
    int * zCoords = NULL ;
    int n = 2 * ppSegs->Nbr1 * ppSegs->Nbr2 ;


#ifdef _MSC_VER 
    flag_DO = MaybeSetWinhdc();
#endif
    C2F(dr)("xget","use color",&verbose, &useColor, &narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
    if (useColor)
    {
      C2F(dr)("xget","color",&verbose,xz,&narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
    }
    else
    {
      C2F(dr)("xget","line style",&verbose,xz,&narg,&v,&v,&v,&dv,&dv,&dv,&dv,0L,0L);
    }
#ifdef _MSC_VER 
    if ( flag_DO == 1) { ReleaseWinHdc () ; }
#endif 

    


    xCoords = MALLOC( n * sizeof (int) ) ;
    if ( xCoords == NULL )
    {
      sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
      return -1 ;
    }

    yCoords = MALLOC( n * sizeof (int) ) ;
    if( yCoords == NULL )
    {
      FREE( xCoords ) ;
      sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
      return -1 ;
    }

    if ( ppSegs->typeofchamp == 1 )
    { /* champ1 has been called */

      zCoords = MALLOC ( (n/2) * sizeof(int) ) ;

      if ( zCoords == NULL ) 
      {
        FREE( xCoords ) ;
        FREE( yCoords ) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }      
    }

    sciChamp2DRealToPixel(xCoords,yCoords,zCoords,&nbArrow,&arrowSize,
      ppSegs->vx,ppSegs->vy,ppSegs->vfx,
      ppSegs->vfy,&(ppSegs->Nbr1),
      &(ppSegs->Nbr2),&(ppSegs->parfact),&(ppSegs->typeofchamp), FALSE ) ;
#ifdef _MSC_VER 
    flag_DO = MaybeSetWinhdc();
#endif

    /** size of arrow **/
    /* a bit tricky */

    /* test if we are in 3d HERE */
    if ( sciGetIs3d( parentSubWin ) )
    {
      double * xvect = NULL;
      double * yvect = NULL;
      int i ;

      xvect = MALLOC( n * sizeof(double) ) ;
      if ( xvect == NULL )
      {
        FREE( xCoords ) ;
        FREE( yCoords ) ;
        if ( zCoords != NULL ) { FREE( zCoords ) ; }
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      yvect = MALLOC( n * sizeof(double) ) ;
      if ( yvect == NULL )
      {
        FREE( xCoords ) ;
        FREE( yCoords ) ;
        if ( zCoords != NULL ) { FREE(zCoords) ; }
        FREE( xvect ) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      for( i = 0 ; i < n ; i++ )
      {
        xvect[i] = XPi2R(xCoords[i]);
        yvect[i] = YPi2R(yCoords[i]);
      }


      /* F.Leray 06.12.04 */
      /* A REVOIR : ne marche pas en 3D */
      ReverseDataFor3D(parentSubWin,xvect,yvect,NULL,n);

      trans3d(parentSubWin,n,xCoords,yCoords,xvect,yvect,NULL);

      FREE(xvect) ;
      FREE(yvect) ;
    }

    if ( sciGetIsMark( pObj ) )
    {
      drawPolyMarks( pObj, n, xCoords, yCoords, DPI ) ;
    }

    if( sciGetIsLine( pObj ) )
    {
      int arrowSize =  computeRealArrowSize( pObj, nbArrow, xCoords, yCoords ) ;

      C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
        &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "thickness", &lineWidth, PI0, PI0, PI0, PI0, PI0, PD0,
        PD0, PD0, PD0, 0L, 0L);    
      C2F (dr) ("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, PD0,
        PD0, PD0, PD0, 0L, 0L);
      if( ppSegs->typeofchamp == 0 )
      {
        int sflag = 0 ;
        C2F(dr)("xarrow","v",xCoords,yCoords,&nbArrow,&arrowSize,xz,&sflag,&dv,&dv,&dv,&dv,0L,0L);
      }
      else
      {
        int sflag = 1 ;
        C2F(dr)("xarrow","v",xCoords,yCoords,&nbArrow,&arrowSize,zCoords,&sflag,&dv,&dv,&dv,&dv,0L,0L);
      }
    }


#ifdef _MSC_VER 
    if ( flag_DO == 1) { ReleaseWinHdc(); }
#endif 
    FREE(xCoords) ;
    FREE(yCoords) ;
    if( zCoords != NULL )
    {
      FREE( zCoords ) ;
    }
  }  
  
  sciUnClip(pObj);

  return 0 ;
}
/*---------------------------------------------------------------------------------*/
/**
 * Compute the arrow size which must be used in (dr)(xarrows).
 * @param[in] pSegs object of which we are displaying arrows.
 * @param[in] X coordinates in pixels of the segments
 * @param[in] Y coordinates in pixels of the segments
 * @return    Size to use in xarrows.
 */
int computeRealArrowSize( sciPointObj * pSegs, int nbSegs, int xCoord[], int yCoord[] )
{
  sciSegs * ppSegs = pSEGS_FEATURE( pSegs ) ;
  double arrowSize = ppSegs->arrowsize ;
  int one = 1 ;

  if ( ppSegs->ptype == 0 )
  {
    if ( arrowSize > 0 )
    {
      int iAs2 = 0 ;
      int iAs  = 0 ;
      C2F(echelle2dl)( &arrowSize, &arrowSize, &iAs, &iAs2, &one, &one, "f2i" ) ;
      return iAs / 2 ;
    }
    else
    {
      /* taken from Xcall1.c */
      int i ;
      double length = 0.0 ;
      for ( i = 0 ; i < nbSegs / 2 ; i++ )
      {
        double dx ;
        double dy ;
        dx=( xCoord[2*i+1] - xCoord[2*i] ) ;
        dy=( yCoord[2*i+1] - yCoord[2*i] ) ;
        length += sqrt( dx * dx + dy * dy ) ;
      }
      if ( nbSegs != 0 )
      {
        length /= nbSegs / 2 ;
      }
      return round( -arrowSize * length ) ;
    }
  }
  else
  {
    if ( ppSegs->arrowsize >= 0 )
    {
      double arsize1 = ((double) Cscale.WIRect1[2])/(5*( ppSegs->Nbr1));
      double arsize2 = ((double) Cscale.WIRect1[3])/(5*( ppSegs->Nbr2));
      double arsize  =  (arsize1 < arsize2) ? inint(arsize1*10.0) : inint(arsize2*10.0) ;
      return (int)((arsize)*(ppSegs->arrowsize));
    }
  }

  return 0 ;

}
/*---------------------------------------------------------------------------------*/
