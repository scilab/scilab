/*------------------------------------------------------------------------*/
/* file: drawFecEntity.c                                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a fec handle                  */
/*------------------------------------------------------------------------*/

#include "drawFecEntity.h"
#include "MALLOC.h"
#include "GetProperty.h"
#include "Xcall1.h"
#include "periScreen.h"
#include "sciCall.h"
#include "sciprint.h"
#include "PloEch.h"
#include "axesScale.h"
#include "Fec.h"
#include "Vertices.h"

/*---------------------------------------------------------------------------------*/
int drawFecEntity( sciPointObj * pObj )
{
  sciFec      * ppFec        = pFEC_FEATURE(pObj)       ;
  sciPointObj * parentSubWin = sciGetParentSubwin(pObj) ;
  int           curLineStyle = 0                        ;
  int           lineStyle    = 1                        ;
  int           verbose      = 0                        ;
  int           nbNode       = ppFec->Nnode             ;
  int         * xCoords      = NULL                     ;
  int         * yCoords      = NULL                     ;
  int           narg         = 1                        ;
  int           i                                       ;

  if ( !sciGetVisibility( pObj ) ) { return 0 ; }

  xCoords = MALLOC( nbNode * sizeof(int) ) ;
  if ( xCoords == NULL )
  {
    sciprint("Can not allocate temporary vector, memory full.\n") ;
    return -1 ;
  }

  yCoords = MALLOC( nbNode * sizeof(int) ) ;
  if ( yCoords == NULL )
  {
    FREE( xCoords ) ;
    sciprint("Can not allocate temporary vector, memory full.\n") ;
    return -1 ;
  }

  if ( sciGetIs3d( parentSubWin ) )
  {
    double * xVect = NULL;
    double * yVect = NULL;

    xVect = MALLOC ( nbNode * sizeof(double) ) ;
    if ( xVect == NULL )
    {
      FREE( xCoords ) ;
      FREE( yCoords ) ;
      return -1 ;
    }

    yVect = MALLOC ( nbNode * sizeof(double) ) ;
    if ( yVect == NULL )
    {
      FREE( xVect   ) ;
      FREE( xCoords ) ;
      FREE( yCoords ) ;
      return -1 ;
    }

    for( i = 0 ; i < nbNode ; i++ )
    {
      xVect[i] = ppFec->pvecx[i];
      yVect[i] = ppFec->pvecy[i];
    }

    ReverseDataFor3DXonly( parentSubWin, xVect, nbNode ) ;
    ReverseDataFor3DYonly( parentSubWin, yVect, nbNode ) ;

    trans3d( parentSubWin, nbNode, xCoords, yCoords, xVect, yVect, NULL ) ;

    FREE(xVect) ;
    FREE(yVect) ;
  }
  else
  {
    for ( i = 0 ; i < nbNode ; i++ )
    {
      xCoords[i] = XScale( ppFec->pvecx[i] ) ; 
      yCoords[i] = YScale( ppFec->pvecy[i] ) ;
    }
  }

#ifdef _MSC_VER
  flag_DO = MaybeSetWinhdc();
#endif

  /* need to put line style to plain otherwise bug 1872 occurs */
  C2F(dr)("xget","line style",&verbose,&curLineStyle,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr)("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);

  newfec( xCoords,yCoords,ppFec->pnoeud,ppFec->pfun,
    &ppFec->Nnode,&ppFec->Ntr,
    ppFec->zminmax,ppFec->colminmax,
    ppFec->colout, ppFec->with_mesh ) ;

  C2F(dr)("xset", "line style", &curLineStyle, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);

#ifdef _MSC_VER
  if ( flag_DO == 1) { ReleaseWinHdc() ; }
#endif

  FREE( xCoords ) ;
  FREE( yCoords ) ;

  return 0 ;

}
/*---------------------------------------------------------------------------------*/
