/*------------------------------------------------------------------------*/
/* file: drawPolylineEntity.h                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a Polyline handle             */
/*------------------------------------------------------------------------*/

#include "drawPolylineEntity.h"
#include "GetProperty.h"
#include "MALLOC.h"
#include "math_graphics.h"
#include "sciprint.h"
#include "clipping.h"
#include "axesScale.h"
#include "drawMarks.h"
#include "Xcall1.h"
#include "Plo3d.h"
#include "periScreen.h"
#include "BasicAlgos.h"
#include "PloEch.h"

/*------------------------------------------------------------------------------------*/
int drawPolylineEntity( sciPointObj * pObj )
{

  sciPolyline * ppPoly = pPOLYLINE_FEATURE(pObj) ;
  sciPointObj * parentSubWin = sciGetParentSubwin( pObj ) ;
  int foreground = sciGetForeground( pObj ) ;
  int background = sciGetBackground( pObj ) ;
  int lineWidth = sciGetLineWidth(pObj) ;
  int lineStyle = sciGetLineStyle(pObj) ;
  int markStyle      = sciGetMarkStyle(pObj) ;
  int markSize       = sciGetMarkSize( pObj) ;
  int markForeground = sciGetMarkForeground(pObj) ;
  int v = 0 ;
  double dv = 0.0 ;
  char logFlags[4] ;
  int nb_curves = 0 ;
  int * curves_size = NULL ; /* for SCI_POLYLINE */
  double ** xMat = NULL ;
  double ** yMat = NULL ;
  double ** zMat = NULL ;
  int closeFlag = ppPoly->closed ;
  int i ;
  int DPI[2] ;

  if ( !sciGetVisibility(pObj) ) { return 0 ; }

  GetDPIFromDriver( DPI ) ;

  logFlags[0] = 'g';
  logFlags[1] = pSUBWIN_FEATURE(parentSubWin)->logflags[0]; /* F.Leray 26.10.04 Pb when logscale on and data is <= 0 for clipping */
  logFlags[2] = pSUBWIN_FEATURE(parentSubWin)->logflags[1];
  logFlags[3] = '\0' ;



  /* //////////////////////////////////////////////////////////////// */
  if ( BuildXYZvectForClipping_IfNanOrLogON(pObj,parentSubWin,&nb_curves, &xMat, &yMat, &zMat, &curves_size) < 0 )
  {
    return -1 ;
  }
  /* //////////////////////////////////////////////////////////////// */

#ifdef _MSC_VER 
  flag_DO = MaybeSetWinhdc();
#endif

  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "thickness", &lineWidth, PI0, PI0, PI0, PI0, PI0, PD0,
    PD0, PD0, PD0, 0L, 0L);
  C2F (dr) ("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, PD0,
    PD0, PD0, PD0, 0L, 0L);

#ifdef _MSC_VER 
  if ( flag_DO == 1) { ReleaseWinHdc() ; }
#endif

  sciClip(pObj);


#ifdef _MSC_VER 
  flag_DO = MaybeSetWinhdc ();
#endif


  for( i = 0 ; i < nb_curves ; i++ )
  {
    int n1 = curves_size[i];
    int    * xCoords = NULL ;
    int    * yCoords = NULL ;
    double * xVect   = NULL ;
    double * yVect   = NULL ;
    double * zVect   = NULL ;
    BOOL     drawLine = FALSE ;
    int      resultTrans3d = 1 ;
    int      one = 1 ;

    if( n1 == 0 ) { continue ; }

    xCoords = MALLOC( 2 * n1 * sizeof(int   ) ) ;
    yCoords = MALLOC( 2 * n1 * sizeof(int   ) ) ;
    xVect   = MALLOC( 2 * n1 * sizeof(double) ) ;
    yVect   = MALLOC( 2 * n1 * sizeof(double) ) ;
    zVect   = MALLOC( 2 * n1 * sizeof(double) ) ;

    if (    xCoords == NULL || yCoords == NULL
         || xVect   == NULL || yVect   == NULL || zVect == NULL )
    {
      FREE( xCoords ) ;
      FREE( yCoords ) ;
      FREE( xVect   ) ;
      FREE( yVect   ) ;
      FREE( zVect   ) ;
      sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
      return -1 ;
    }

    /**DJ.Abdemouche 2003**/
    switch ( ppPoly->plot )
    {
    case 1:  /* case plot2d, xpoly */
      drawLine = TRUE;
      if ( sciGetIs3d(parentSubWin) )
      {
        /* axes reverse is tested and xvect, yvect are changed if needed here */
        ReverseDataFor3D(parentSubWin,xMat[i],yMat[i],zMat[i],n1);

        resultTrans3d = trans3d(parentSubWin,n1,xCoords,yCoords,xMat[i],yMat[i],zMat[i]);
      }
      else
      {
        /* In 2d, the axes reverse is done inside XScale, YScale... routines. */
        C2F (echelle2d) (xMat[i],yMat[i], xCoords, yCoords, &n1, &one, "f2i",3L); 
      }
      break ;
    case 2:
      drawLine = TRUE ;
      if ( sciGetIs3d(parentSubWin) )
      {
        if( zMat[i] == NULL )
        {
          FREE( zVect ) ;
          zVect = NULL ;
        }

        ReverseDataFor3D(parentSubWin,xMat[i],yMat[i],zMat[i],n1);

        Plo2dTo3d(2,&one,&n1,xMat[i],yMat[i],zMat[i],xVect,yVect,zVect);
        resultTrans3d = trans3d(parentSubWin,2*n1,xCoords,yCoords,xVect,yVect,zVect);
      }
      else
      {
        Plo2d2RealToPixel(&one,&n1,xMat[i],yMat[i],xCoords,yCoords,logFlags);
      }
      n1 = 2 * n1 ;
      break;
    case 3:
      {
        int nn1 = 0 ;
        drawLine = FALSE;
        if ( sciGetIs3d(parentSubWin) )
        {
          if( zMat[i] == NULL )
          {
            FREE(zVect) ;
            zVect = NULL ;
          }

          ReverseDataFor3D(parentSubWin,xMat[i],yMat[i],zMat[i],n1);

          Plo2dTo3d(3,&one,&n1,xMat[i],yMat[i],zMat[i],xVect,yVect,zVect);
          resultTrans3d = trans3d(parentSubWin, 2 * n1,xCoords,yCoords,xVect,yVect,zVect);
        }
        else
        {
          Plo2d3RealToPixel(&one,&n1,xMat[i],yMat[i],xCoords,yCoords,logFlags);
        }

        if( resultTrans3d == 1 )
        {
          nn1 = n1 * 2 ;
        }

        /* add mark support even for bar plot lines */
        if ( sciGetIsMark(pObj) )
        {

          C2F (dr) ("xset", "dashes", &markForeground, &markForeground, &v, &v, &v, &v, &dv,
            &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "foreground", &markForeground, &markForeground, &v, &v, &v, &v,
            &dv, &dv, &dv, &dv, 5L, 4096);

          C2F (dr) ("xset", "mark", &markStyle, &markSize, PI0, PI0, PI0, PI0, PD0, PD0,
            PD0, PD0, 0L, 0L);   

          DrawNewMarks(pObj,nn1,xCoords,yCoords,DPI);
        }

        if ( sciGetIsLine(pObj) )
        {
          int iflag = 0;
          C2F(dr)("xsegs","v",xCoords,yCoords,&nn1,&foreground,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
        }
        /**DJ.Abdemouche 2003**/
        n1 = one ;
      }
      break;
    case 4:
      {
        int nn2 = 0 ;
        double arsize1 = 0.0 ;
        double arsize2 = 0.0 ;
        int arrowSize = 0 ;

        drawLine = FALSE;
        
        if ( sciGetIs3d(parentSubWin) )
        {
          if( zMat[i] == NULL )
          {
            FREE(zVect) ;
            zVect = NULL ;
          }

          ReverseDataFor3D( parentSubWin, xMat[i], yMat[i], zMat[i], n1 ) ;

          Plo2dTo3d( 4, &one, &n1, xMat[i], yMat[i],zMat[i],xVect,yVect,zVect);

          resultTrans3d = trans3d(parentSubWin,2 * n1,xCoords,yCoords,xVect,yVect,zVect);
        }
        else
        {
          Plo2d4RealToPixel(&one,&n1,xMat[i],yMat[i],xCoords,yCoords,logFlags); 
        }

        nn2 = 2 * (n1) - 1 ;
        arsize1 = Cscale.WIRect1[2] / 70.0 ;
        arsize2 = Cscale.WIRect1[3] / 70.0 ;
        arrowSize = ( (arsize1 < arsize2) ? inint(10.0*arsize1) : inint(10.0*arsize2) ) ;

        if(resultTrans3d == 1)
        {
          /*integer lstyle_=sciGetForeground(pObj) ;*/
          int iflag = 0 ;

          /* add mark support even for arrow line style */
          if ( sciGetIsMark(pObj) )
          {

            C2F (dr) ("xset", "dashes", &markForeground, &markForeground, &v, &v, &v, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", &markForeground, &markForeground, &v, &v, &v, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xset", "mark", &markStyle, &markSize, PI0, PI0, PI0, PI0, PD0, PD0,
              PD0, PD0, 0L, 0L);   

            DrawNewMarks(pObj,nn2,xCoords,yCoords,DPI);
          }

          if ( sciGetIsLine(pObj) )
          {

            C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "thickness", &lineWidth, PI0, PI0, PI0, PI0, PI0, PD0,
              PD0, PD0, PD0, 0L, 0L);
            C2F (dr) ("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, PD0,
              PD0, PD0, PD0, 0L, 0L);

              arrowSize = round( ppPoly->arsize_factor * arrowSize ) ;

            C2F(dr)("xarrow","v",xCoords,yCoords,&nn2,&arrowSize,&foreground,&iflag,PD0,PD0,PD0,PD0,0L,0L);
          }
        }
      }
      break;
    case 5: /* case xfpoly */
      drawLine = TRUE;
      if ( sciGetIs3d(parentSubWin) )
      {
        ReverseDataFor3D(parentSubWin,xMat[i],yMat[i],zMat[i],n1);
        resultTrans3d = trans3d(parentSubWin,n1,xCoords,yCoords,xMat[i],yMat[i],zMat[i]);
      }
      else
      {
        C2F(echelle2d)( xMat[i],yMat[i], xCoords, yCoords, &n1, &one, "f2i",3L);
      }

      sciClip(pObj);

      if(resultTrans3d == 1)
      {
        C2F (dr)("xarea", "xv", &n1, xCoords, yCoords, &closeFlag, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 2L ) ; /* 3 : strlen("xv") */
      }
      break;
    case 6: /* 'Matlab' bar */
      {
        /* get the number of polylines sisters with bar property "on" */
        double * y_shift = ppPoly->y_shift ;
        double barWidth =  ppPoly->bar_width;
        double barX[4] ;
        double barY[4] ;
        double barZ[4];
        int pixX[4] ;
        int pixY[4];
        int four = 4;

        drawLine = TRUE;

        if ( sciGetIs3d(parentSubWin) )
        {
          int j ;

          for( j = 0 ; j < n1 ; j++ )
          {
            barX[0] = xMat[i][j] - barWidth / 2 ;
            barX[1] = barX[0] ;
            barX[2] = xMat[i][j] + barWidth / 2 ;
            barX[3] = barX[2] ;

            if( y_shift == NULL )
            {
              barY[0] = 0. ;
              barY[3] = 0. ;
            }
            else
            {
              barY[0] = y_shift[i] ;
              barY[3] = y_shift[i] ;
            }
            barY[1] = yMat[i][j] ;
            barY[2] = barY[1] ;

            if( zMat[i] == NULL )
            {
              barZ[0] = 0.0 ;
              barZ[1] = 0.0 ;
              barZ[2] = 0.0 ;
              barZ[3] = 0.0 ; /* cas log a revoir */
            }
            else
            {
              barZ[0] = zMat[i][j] ;
              barZ[1] = zMat[i][j] ;
              barZ[2] = zMat[i][j] ;
              barZ[3] = zMat[i][j] ;
            }

            ReverseDataFor3D( parentSubWin, barX, barY, barZ, four ) ;
            resultTrans3d = trans3d(parentSubWin,four,pixX,pixY,barX,barY,barZ);

            C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xarea", "xv", &four, pixX, pixY, &closeFlag, PI0, PI0, PD0, PD0, PD0, PD0, 5L,2L); /* 2L : strlen("xv") */

            C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            /* encapsulate this for a clip check on each segment */
            C2F (dr) ("xlines", "xv", &four, pixX, pixY, &one, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
          }

          ReverseDataFor3D(parentSubWin,xMat[i],yMat[i],zMat[i],n1);
          resultTrans3d = trans3d(parentSubWin,n1,xCoords,yCoords,xMat[i],yMat[i],zMat[i]);
        }
        else
        {
          int j ;

          for( j = 0 ; j < n1 ; j++ )
          {
            barX[0] = xMat[i][j] - barWidth/2;
            barX[1] = barX[0] ;
            barX[2] = xMat[i][j] + barWidth/2;
            barX[3] = barX[2] ;

            if ( y_shift == NULL )
            {
              barY[0] = 0.0 ;
              barY[3] = 0.0 ;
            }
            else
            {
              barY[0] = y_shift[j] ;
              barY[3] = y_shift[j] ;
            }
            barY[1] = yMat[i][j];
            barY[2] = barY[1] ;

            C2F(echelle2d)(barX,barY, pixX, pixY, &four, &one, "f2i",3L);

            C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xarea", "xv", &four, pixX, pixY, &closeFlag, PI0, PI0, PD0, PD0, PD0, PD0, 5L,2L); /* 2L : strlen("xv") */

            C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xlines", "xv", &four, pixX, pixY, &one, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
          }
          C2F (echelle2d) (xMat[i],yMat[i], xCoords, yCoords, &n1, &one, "f2i",3L); 
        }
      }
      break;
    case 7: /* 'Matlab' barh */
      {
        double * y_shift = ppPoly->y_shift ;
        double barX[4] ;
        double barY[4] ;
        double barZ[4];
        int pixX[4] ;
        int pixY[4];
        double barWidth =  ppPoly->bar_width ;
        int four = 4 ;
  
        int j ;

        drawLine = TRUE ;

        for( j = 0 ; j < n1 ; j++ )
        {
          double temp = xMat[i][j];
          xMat[i][j] = yMat[i][j];
          yMat[i][j] = temp ;
        }

        if ( sciGetIs3d(parentSubWin) )
        {

          for( j = 0 ; j < n1 ; j++ )
          {
            barY[0] = yMat[i][j] + barWidth / 2 ;
            barY[1] = barY[0] ;
            barY[2] = yMat[i][j] - barWidth / 2 ;
            barY[3] = barY[2] ;

            if( y_shift ==  NULL)
            {
              barX[0] = 0.0 ;
              barX[3] = 0.0 ;
            }
            else
            {
              barX[0] = y_shift[i] ;
              barX[3] = y_shift[i] ;
            }
            barX[1] = xMat[i][j] ;
            barX[2] = barX[1] ;

            if( zMat[i] == NULL )
            {
              barZ[0] = 0.0 ;
              barZ[1] = 0.0 ;
              barZ[2] = 0.0 ;
              barZ[3] = 0.0 ; /* cas log a revoir */
            }
            else
            {
              barZ[0] = zMat[i][j] ;
              barZ[1] = barZ[0] ;
              barZ[2] = barZ[0] ;
              barZ[3] = barZ[0] ;
            }

            ReverseDataFor3D(parentSubWin,barX,barY,barZ,four);
            resultTrans3d = trans3d(parentSubWin,four,pixX,pixY,barX,barY,barZ);

            C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xarea", "xv", &four, pixX, pixY, &closeFlag, PI0, PI0, PD0, PD0, PD0, PD0, 5L,2L); /* 2L : strlen("xv") */

            C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xlines", "xv", &four, pixX, pixY, &one, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
          }

          ReverseDataFor3D(parentSubWin,xMat[i],yMat[i],zMat[i],n1);
          resultTrans3d = trans3d(parentSubWin,n1,xCoords,yCoords,xMat[i],yMat[i],zMat[i]);
        }
        else
        {

          for( j = 0 ; j < n1 ; j++ )
          {
            barY[0] = yMat[i][j] + barWidth/2;
            barY[1] = barY[0] ;
            barY[2] = yMat[i][j] - barWidth/2;
            barY[3] = barY[2] ;

            if( y_shift == NULL )
            {
              barX[0] = 0.0 ;
              barX[3] = 0.0 ;
            }
            else
            {
              barX[0] = y_shift[j] ;
              barX[3] = y_shift[j] ;
            }
            barX[1] = xMat[i][j] ;
            barX[2] = barX[1] ;

            C2F (echelle2d) (barX,barY, pixX, pixY, &four, &one, "f2i",3L);

            C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xarea", "xv", &four, pixX, pixY, &closeFlag, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 2L ) ; /* 2L : strlen("xv") */

            C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xlines", "xv", &four, pixX, pixY, &one, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
          }
          C2F (echelle2d) (xMat[i],yMat[i], xCoords, yCoords, &n1, &one, "f2i",3L); 
        }
      }
      break;
    default:
      sciprint ("This Polyline cannot be drawn !\n");
      break ;
    }

    if(resultTrans3d == 1 && drawLine)
    {
      if(sciGetIsFilled(pObj) && ppPoly->plot != 5) /* No filling if mode plot == 5 is selected */
      {
        if( !ppPoly->isinterpshaded )
        {
          /* flat mode */

          C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, &dv,
            &dv, &dv, &dv, 5L, 4096);
          C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v, &v,
            &dv, &dv, &dv, &dv, 5L, 4096);

          C2F (dr) ("xarea", "xv", &n1, xCoords, yCoords, &closeFlag, PI0, PI0, PD0, PD0, PD0, PD0, 5L,2L);
        }
        else
        {
          /* interp. shading */
          scilab_shade ( xCoords, yCoords, ppPoly->scvector, ppPoly->n1, 0);
        }
      }

      if ( sciGetIsMark(pObj) )
      {

        C2F (dr) ("xset", "dashes", &markForeground, &markForeground, &v, &v, &v, &v, &dv,
          &dv, &dv, &dv, 5L, 4096);
        C2F (dr) ("xset", "foreground", &markForeground, &markForeground, &v, &v, &v, &v,
          &dv, &dv, &dv, &dv, 5L, 4096);

        C2F (dr) ("xset", "mark", &markStyle, &markSize, PI0, PI0, PI0, PI0, PD0, PD0,
          PD0, PD0, 0L, 0L);   

        DrawNewMarks(pObj,n1,xCoords,yCoords,DPI);
      }

      if ( sciGetIsLine(pObj) )
      {

        C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
          &dv, &dv, &dv, 5L, 4096);
        C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
          &dv, &dv, &dv, &dv, 5L, 4096);
        C2F (dr) ("xset", "thickness", &lineWidth, PI0, PI0, PI0, PI0, PI0, PD0,
          PD0, PD0, PD0, 0L, 0L);
        C2F (dr) ("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, PD0,
          PD0, PD0, PD0, 0L, 0L);

        C2F (dr) ("xlines", "xv", &n1, xCoords, yCoords, &closeFlag, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);

      }
    }

#ifdef _MSC_VER
    if ( flag_DO == 1) { ReleaseWinHdc () ; }
#endif

    FREE(xVect);
    FREE(yVect);
    FREE(zVect);

    FREE (xCoords);
    FREE (yCoords);

  }


  for( i = 0 ; i < nb_curves ; i++ )
  {
    int nbCurves;
    nbCurves = curves_size[i];

    if( nbCurves == 0 ) { continue ; }

    FREE(xMat[i]); xMat[i] = NULL;
    FREE(yMat[i]); yMat[i] = NULL;
    FREE(zMat[i]); zMat[i] = NULL;
  }
  FREE(xMat);
  FREE(yMat);
  FREE(zMat);
  FREE(curves_size); curves_size = NULL;

#ifdef _MSC_VER 
  if ( flag_DO == 1) { ReleaseWinHdc (); }
#endif  

  sciUnClip(pObj);

  return 0 ;

}
/*------------------------------------------------------------------------------------*/
int BuildXYZvectForClipping_IfNanOrLogON( sciPointObj *   ppolyline,
                                          sciPointObj *   psubwin  ,
                                          int         *   nb_curves,
                                          double      *** xvect    ,
                                          double      *** yvect    ,
                                          double      *** zvect    ,
                                          int         **  curves_size )
{
  int i ;
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin) ;
  sciPolyline * pppolyline = pPOLYLINE_FEATURE(ppolyline);
  int * indexGoodPoints = NULL;

  int value = 0;
  int nb = 0;

  int nbPoints = pppolyline->n1 ;

  int * store_data[3] = { NULL, NULL, NULL } ;
  double * pvx_plus_x_shift = NULL;
  double * pvy_plus_y_shift = NULL;
  double * pvz_plus_z_shift = NULL;

  double * x_shift = pppolyline->x_shift;
  double * y_shift = pppolyline->y_shift;
  double * z_shift = pppolyline->z_shift;

  if ( pppolyline->n1 == 0 )
  {
    *nb_curves   = 0    ;
    *xvect       = NULL ;
    *yvect       = NULL ;
    *zvect       = NULL ;
    *curves_size = 0    ;
    return 0 ;
  }

  pvx_plus_x_shift = MALLOC( nbPoints * sizeof(double) ) ;
  if ( pvx_plus_x_shift == NULL )
  {
    sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
    return -1 ;
  }

  if( x_shift != NULL )
  { /* if shift is not NULL, its size is n1 */
    for( i = 0 ; i < nbPoints ; i++ )
    {
      pvx_plus_x_shift[i] = pppolyline->pvx[i] + x_shift[i] ;
    }
  }
  else
  {
    for( i = 0 ; i < nbPoints ; i++ )
    {
      pvx_plus_x_shift[i] = pppolyline->pvx[i];
    }
  }

  pvy_plus_y_shift = MALLOC( nbPoints * sizeof(double) ) ;
  if ( pvy_plus_y_shift == NULL )
  {
    FREE( pvx_plus_x_shift ) ;
    sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
    return -1 ;
  }

  if( y_shift != NULL )
  { /* if shift is not NULL, its size is n1 */
    for( i = 0 ; i < nbPoints ; i++ )
    {
      pvy_plus_y_shift[i] = pppolyline->pvy[i] + y_shift[i] ;
    }
  }
  else
  {
    for ( i = 0 ; i < nbPoints ; i++ )
    {
      pvy_plus_y_shift[i] = pppolyline->pvy[i] ;
    }
  }


  if( pppolyline->pvz != NULL || z_shift != NULL )
  {
    pvz_plus_z_shift = MALLOC( nbPoints * sizeof(double) ) ;
    if ( pvz_plus_z_shift == NULL )
    {
      FREE( pvx_plus_x_shift ) ;
      FREE( pvy_plus_y_shift ) ;
      sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
      return -1 ;
    }

    if ( z_shift != NULL && pppolyline->pvz != NULL)
    { /* if shift is not NULL, its size is n1 */
      for( i = 0 ; i < nbPoints ; i++ )
      {
        pvz_plus_z_shift[i] = pppolyline->pvz[i] + z_shift[i] ;
      }
    }
    else if ( z_shift != NULL && pppolyline->pvz == NULL )
    {
      for ( i = 0 ; i < nbPoints ; i++ )
      {
        pvz_plus_z_shift[i] = z_shift[i] ;
      }
    }
    else if ( z_shift == NULL && pppolyline->pvz != NULL )
    {
      for( i = 0 ; i < nbPoints ; i++ )
      {
        pvz_plus_z_shift[i] = pppolyline->pvz[i] ;
      }
    }
  }

  for( i = 0 ; i < 3 ; i++ )
  {
    store_data[i] = MALLOC ( nbPoints * sizeof(int) ) ;
    if ( store_data[i] == NULL )
    {
      int j ;
      for ( j = 0 ; j < i ; j++ )
      {
        FREE( store_data[i] ) ;
      }
      FREE( pvx_plus_x_shift ) ;
      FREE( pvy_plus_y_shift ) ;
      FREE( pvz_plus_z_shift ) ;
      sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
      return -1 ;
    }
  }

  indexGoodPoints = MALLOC( nbPoints * sizeof(integer) ) ;
  if ( indexGoodPoints == NULL )
  {
    FREE( store_data[0] ) ;
    FREE( store_data[1] ) ;
    FREE( store_data[2] ) ;
    FREE( pvx_plus_x_shift ) ;
    FREE( pvy_plus_y_shift ) ;
    FREE( pvz_plus_z_shift ) ;
    sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
    return -1 ;
  }

  for( i = 0 ; i < nbPoints ; i++ )
  {
    indexGoodPoints[i] = nbPoints + 1000;
  }

  /* ICI dans mon exemple plot2d indexGoodPoints[i] = 63+1 = 64 */

  /*  we search for != %nan */
  for( i = 0 ; i < nbPoints ; i++ )
  {
    if ( pvz_plus_z_shift == NULL )
    {
      if( (finite(pvx_plus_x_shift[i]) == 1) &&
          (finite(pvy_plus_y_shift[i]) == 1) )
      {
        indexGoodPoints[i] = 1 ;  /* x and y are finite numbers */
      }
      else
      {
        indexGoodPoints[i] = -1 ;
      }
    }
    else
    {
      if( (finite(pvx_plus_x_shift[i]) == 1) &&
          (finite(pvy_plus_y_shift[i]) == 1) &&
          (finite(pvz_plus_z_shift[i]) == 1))
      {
        indexGoodPoints[i] = 1 ;
      }
      else
      {
        indexGoodPoints[i] = -1;
      }

    }
  }


  /* we search for values <= 0 if log_flag */
  for( i = 0 ; i < nbPoints ; i++ )
  {
    if( ppsubwin->logflags[0] == 'l' )
    {
      if((indexGoodPoints[i] == 1) && (pvx_plus_x_shift[i] <= 0))
      {
        indexGoodPoints[i] = -1;
      }
    }

    if(ppsubwin->logflags[1] == 'l') 
    {
      if((indexGoodPoints[i] == 1) && (pvy_plus_y_shift[i] <= 0))
      {
        indexGoodPoints[i] = -1;
      }
    }

    if(pppolyline->pvz != NULL && ppsubwin->logflags[2] == 'l') 
    {
      if((indexGoodPoints[i] == 1) && (pvz_plus_z_shift[i] <= 0))
      {
        indexGoodPoints[i] = -1;
      }
    }

  }

  value = indexGoodPoints[0]; /* -1 ou 1 */


  *nb_curves = 0;
  if( value == 1 )
  { /* we begin by a draw point/line */
    

    int j = 0 ;
    int k = 0 ;

    *nb_curves = 0 ;

    while( j < nbPoints )
    {

      for( i = j ; i < nbPoints ; i++ )
      {
        if(indexGoodPoints[i] == 1)
        {
          continue ;
        }
        else
        {
          break ;
        }
      }

      store_data[0][(*nb_curves)] = j ;
      store_data[1][(*nb_curves)] = i ;
      store_data[2][(*nb_curves)] = i - j ;

      *nb_curves = *nb_curves + 1; /* STOCKER AUSSI LES INDEXES EXTREMITES OU C'EST EGAL A  1 !! */


      for( k = i ; k < nbPoints ; k++ )
      {
        if(indexGoodPoints[k] == -1)
        {
          continue ;
        }
        else
        {
          break ;
        }
      }
      j = k ;
    }
  }
  else if( value == -1 )
  {
    /* we begin with a not drawn point/line */

    int j = 0 ;
    int k = 0 ;

    *nb_curves = 0 ;

    while( j < nbPoints )
    {

      for( i = j ; i < nbPoints ; i++ )
      {
        if(indexGoodPoints[i] == -1)
        {
          continue ;
        }
        else
        {
          break ;
        }
      }

      for ( k = i ; k < nbPoints ; k++ )
      {
        if(indexGoodPoints[k] == 1)
        {
          continue ;
        }
        else
        {
          break ;
        }
      }

      store_data[0][(*nb_curves)] = i;
      store_data[1][(*nb_curves)] = k;
      store_data[2][(*nb_curves)] = k-i;


      *nb_curves = *nb_curves + 1;
      j = k ;

    }
  }
  else
  {
    sciprint("Impossible case in CheckClippingNanLogON\n");
  }


  nb = *nb_curves;

  /* XYZ vect building */
  *xvect = MALLOC( nb * sizeof(double *) ) ;
  *yvect = MALLOC( nb * sizeof(double *) ) ;
  *zvect = MALLOC( nb * sizeof(double *) ) ;
  if ( *xvect == NULL || *yvect == NULL || *zvect == NULL )
  {
    FREE( store_data[0] ) ;
    FREE( store_data[1] ) ;
    FREE( store_data[2] ) ;
    FREE( pvx_plus_x_shift ) ;
    FREE( pvy_plus_y_shift ) ;
    FREE( pvz_plus_z_shift ) ;
    FREE( indexGoodPoints ) ;
    FREE( *xvect ) ;
    FREE( *yvect ) ;
    FREE( *zvect ) ;
    sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
    return -1 ;
  }

  for( i = 0 ; i < nb ; i++ )
  {
    int j ;
    int cmpteur = 0;
    /* Allocating arrays x, y and zvect */
    if( store_data[2][i] > 0 )
    {

      (*xvect)[i] = MALLOC( store_data[2][i] * sizeof(double) ) ;
      (*yvect)[i] = MALLOC( store_data[2][i] * sizeof(double) ) ;
      if(pvz_plus_z_shift == NULL)
      {
        (*zvect)[i] = NULL ;
      }
      else
      {
        (*zvect)[i] = MALLOC( store_data[2][i] * sizeof(double) ) ;
      }

      if ( ( *xvect)[i] == NULL || (*yvect)[i] == NULL 
          || ( pvz_plus_z_shift != NULL && (*zvect)[i] == NULL ) )
      {
        int k ;
        for ( k = 0 ; k <= i ; k++ )
        {
          FREE( (*xvect)[k] ) ;
          FREE( (*yvect)[k] ) ;
          if ( pvz_plus_z_shift != NULL ) { FREE( (*zvect)[k] ) ; }
        }
        FREE( store_data[0] ) ;
        FREE( store_data[1] ) ;
        FREE( store_data[2] ) ;
        FREE( pvx_plus_x_shift ) ;
        FREE( pvy_plus_y_shift ) ;
        FREE( pvz_plus_z_shift ) ;
        FREE( indexGoodPoints ) ;
        FREE( *xvect ) ;
        FREE( *yvect ) ;
        FREE( *zvect ) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }
    }
    for(j=store_data[0][i];j<store_data[1][i];j++)
    {
      (*xvect)[i][cmpteur] = pvx_plus_x_shift[j];
      (*yvect)[i][cmpteur] = pvy_plus_y_shift[j];
      if(pvz_plus_z_shift != NULL)
      {
        (*zvect)[i][cmpteur] = pvz_plus_z_shift[j];
      }

      cmpteur++;

    }
  }

  *curves_size = createIntArrayCopy( store_data[2], nb ) ;
  if ( *curves_size == NULL )
  {
    int k ;
    for ( k = 0 ; k < nb ; k++ )
    {
      FREE( (*xvect)[k] ) ;
      FREE( (*yvect)[k] ) ;
      FREE( (*zvect)[k] ) ;
    }
    FREE( store_data[0] ) ;
    FREE( store_data[1] ) ;
    FREE( store_data[2] ) ;
    FREE( pvx_plus_x_shift ) ;
    FREE( pvy_plus_y_shift ) ;
    FREE( pvz_plus_z_shift ) ;
    FREE( indexGoodPoints ) ;
    FREE( *xvect ) ;
    FREE( *yvect ) ;
    FREE( *zvect ) ;
    sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
    return -1 ;
  }


  FREE( store_data[0] ) ;
  FREE( store_data[1] ) ;
  FREE( store_data[2] ) ;
  FREE(indexGoodPoints); indexGoodPoints = NULL;
  FREE(pvx_plus_x_shift); pvx_plus_x_shift = NULL;
  FREE(pvy_plus_y_shift); pvy_plus_y_shift = NULL;
  FREE(pvz_plus_z_shift); pvz_plus_z_shift = NULL;
  return 0;
}
/*------------------------------------------------------------------------------------*/
void Plo2dTo3d(integer type, integer *n1, integer *n2, double x[], double y[], double z[], double xOut[], double yOut[], double zOut[])
{
  /* JBS : happy understanding */
  integer i,j;
  switch (type)
  {
  case 2:
    /** Computing y/z-values **/
    for ( i=0 ; i < (*n2) ; i++)
      for (j=0 ; j< (*n1) ; j++)
      {
        yOut[2*i+1+2*(*n2)*j]= yOut[2*i+2*(*n2)*j]= y[i+(*n2)*j];
        if (z == NULL)
          zOut = (double *) NULL;
        else
          zOut[2*i+1+2*(*n2)*j]= zOut[2*i+2*(*n2)*j]= z[i+(*n2)*j];
      }
      /*ym[2*i+1+2*(*n2)*j]= ym[2*i+2*(*n2)*j]= YScale(y[i+(*n2)*j]);*/

      /** Computing x-values **/
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=1 ; i < (*n2) ; i++)
        {
          xOut[2*i+2*(*n2)*j]= x[i+(*n2)*j];
          xOut[2*i-1+2*(*n2)*j]=xOut[2*i+2*(*n2)*j];
        }
        xOut[2*(*n2)*j]= x[(*n2)*j];
        xOut[2*(*n2)-1+ 2*(*n2)*j]= xOut[2*(*n2-1)+ 2*(*n2)*j];
      }
      break;
  case 3:
    /** Computing y-values **/
    for ( i=0 ; i < (*n2) ; i++)
      for (j=0 ; j< (*n1) ; j++)
      {
        yOut[2*i+1+2*(*n2)*j]= 0.0;
        yOut[2*i+2*(*n2)*j]= y[i+(*n2)*j];
      }
      /** Computing x/z-values **/
      for (j=0 ; j< (*n1) ; j++)
      {
        for ( i=0 ; i < (*n2) ; i++)
        {
          xOut[2*i+2*(*n2)*j]= x[i+(*n2)*j];
          xOut[2*i+1+2*(*n2)*j]=xOut[2*i+2*(*n2)*j]; 
          if (z == NULL)
            zOut = (double *) NULL;
          else
          {
            zOut[2*i+2*(*n2)*j]= z[i+(*n2)*j];
            zOut[2*i+1+2*(*n2)*j]=zOut[2*i+2*(*n2)*j];
          }
        }
      }
      break;
  case 4: 
    /** Computing y-values **/
    for ( i=0 ; i < (*n2) ; i++)
      for (j=0 ; j< (*n1) ; j++)
        yOut[2*i+2*(*n2)*j]= y[i+(*n2)*j];
    for ( i=0 ; i < (*n2)-1 ; i++)
      for (j=0 ; j< (*n1) ; j++)
        yOut[2*i+1+2*(*n2)*j]=yOut[2*i+2+2*(*n2)*j]; 
    /** Computing x-values **/
    for (j=0 ; j< (*n1) ; j++)
      for ( i=0 ; i < (*n2) ; i++)
        xOut[2*i+2*(*n2)*j]= x[i+(*n2)*j];
    for (j=0 ; j< (*n1) ; j++)
      for ( i=0 ; i < (*n2)-1 ; i++)
        xOut[2*i+1+2*(*n2)*j]=xOut[2*i+2+2*(*n2)*j];
    /** Computing z-values **/
    if (z == NULL)
      zOut = (double *) NULL;
    else
    {
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2) ; i++)
          zOut[2*i+2*(*n2)*j]= z[i+(*n2)*j];
      for (j=0 ; j< (*n1) ; j++)
        for ( i=0 ; i < (*n2)-1 ; i++)
          zOut[2*i+1+2*(*n2)*j]=zOut[2*i+2+2*(*n2)*j];
    }
    break;
  default:
    break;
  } 
}
/*------------------------------------------------------------------------------------*/
