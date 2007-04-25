/*------------------------------------------------------------------------*/
/* file: drawGrayplotEntity.c                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a grayplot handle             */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "drawGrayplotEntity.h"
#include "GetProperty.h"
#include "clipping.h"
#include "sciprint.h"
#include "Gray.h"
#include "BasicAlgos.h"
#include "axesScale.h"
#include "math_graphics.h"
#include "Xcall1.h"
#include "PloEch.h"
#include "MALLOC.h"
#include "periScreen.h"
#include "math_graphics.h"

/*-------------------------------------------------------------------------------------*/
int drawGrayplotEntity( sciPointObj * pObj )
{
  sciGrayplot * ppGray = pGRAYPLOT_FEATURE( pObj ) ;
  sciPointObj * parentSubWin = sciGetParentSubwin( pObj ) ;
  int n1 = ppGray->nx ;
  int n2 = ppGray->ny ;

  if ( !sciGetVisibility(pObj) ) { return 0 ; }

  switch ( ppGray->type )
  {

  case 0:  /* Grayplot case */
    if( !sciGetIs3d( parentSubWin ) )
    {
      int * xCoords = NULL ;
      int * yCoords = NULL ;
      int i ;
      int j ;

      /* F.Leray 19.05.05 : Now I use only xliness (not xfrect) */
      /* to better manage axes reversing */

      xCoords = MALLOC( n1 * n2 * sizeof(int) ) ;
      if ( xCoords == NULL )
      {
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      yCoords = MALLOC( n1 * n2 * sizeof(int) ) ;
      if ( yCoords == NULL )
      {
        FREE( xCoords ) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      for ( i = 0 ; i < n1 ; i++ )  /* on x*/
      {
        for ( j = 0 ; j < n2 ; j++ )  /* on y */
        {
          xCoords[i+j*n1]= XScale(ppGray->pvecx[i]);
          yCoords[j+i*n2]= YScale(ppGray->pvecy[j]);
        }
      }

#ifdef _MSC_VER
        flag_DO = MaybeSetWinhdc();
#endif
        frame_clip_on();

        if ( strncmp( ppGray->datamapping,"scaled", 6) == 0 )
        {
          GraySquareScaled(xCoords,yCoords,ppGray->pvecz,n1,n2); /* SS 03/01/03 */
        }
        else
        {
          GraySquareDirect(xCoords,yCoords,ppGray->pvecz,n1,n2);
        }

        frame_clip_off();
#ifdef _MSC_VER
        if ( flag_DO == 1) { ReleaseWinHdc() ; }
#endif

        FREE(xCoords);
        FREE(yCoords);

    }
    else
    {
      /*3D version */
      double * xvect   = NULL;
      double * yvect   = NULL;
      int    * xCoords = NULL ;
      int    * yCoords = NULL ;
      int i ;
      int j ;

      xvect = createDoubleArrayCopy( ppGray->pvecx, n1 ) ;
      if ( xvect == NULL )
      {
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      yvect = createDoubleArrayCopy( ppGray->pvecy, n2 ) ;
      if ( yvect == NULL )
      {
        FREE(xvect) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      xCoords = MALLOC( n1 * n2 * sizeof(int) ) ;
      if ( xCoords == NULL )
      {
        FREE(xvect) ;
        FREE(yvect) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      yCoords = MALLOC( n1 * n2 * sizeof(int) ) ;
      if ( yCoords == NULL )
      {
        FREE(xvect) ;
        FREE(yvect) ;
        FREE(xCoords) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      ReverseDataFor3DXonly(parentSubWin,xvect,n1);
      ReverseDataFor3DYonly(parentSubWin,yvect,n2);

      for ( i =0 ; i < n1 ; i++)  /* on x*/
      {
        for ( j =0 ; j < n2 ; j++)  /* on y */
        {
          trans3d(parentSubWin,1,&xCoords[i+j*n1],&yCoords[j+i*n2],
          &xvect[i],&yvect[j],NULL);
        }
      }

#ifdef _MSC_VER
      flag_DO = MaybeSetWinhdc();
#endif
      frame_clip_on(); 

      /* draw the filled projected rectangle */

      for (i = 0 ; i < (n1)-1 ; i++)
      {
        for (j = 0 ; j < (n2)-1 ; j++)
        {
          integer vertexx[5], vertexy[5];
          int five = 5 ;
          int one = 1;
          double zmoy,zmax,zmin,zmaxmin;
          integer verbose= 0 ;
          integer whiteid,fill[1],cpat,xz[2];
          double * z = ppGray->pvecz;
          int narg = 0 ;
          zmin = Mini(z,(n1)*(n2));
          zmax = Maxi(z,(n1)*(n2));
          zmaxmin = zmax - zmin ;

          if (zmaxmin <= SMDOUBLE) { zmaxmin = SMDOUBLE ; }
          C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          C2F(dr)("xget","pattern",&verbose,&cpat,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
          C2F(dr)("xget","wdim",&verbose,xz,&narg, PI0, PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);


          if(strncmp(ppGray->datamapping,"scaled", 6) == 0)
          {
            /* color for current rectangle */
            zmoy=1/4.0*(z[i+n1*j]+z[i+n1*(j+1)]+z[i+1+n1*j]+z[i+1+n1*(j+1)]);

            fill[0]=1 + inint((whiteid-1)*(zmoy-zmin)/(zmaxmin));  

            fill[0] = - fill[0]; /* not to have contour with foreground color around the rectangle */
          }
          else /* "direct" mode is used */
          {
            fill[0] = - (int) z[j+n1*i];
          }

          vertexx[0] = xCoords[i+n1*j];
          vertexx[1] = xCoords[i+n1*(j+1)];
          vertexx[2] = xCoords[i+1+n1*(j+1)];
          vertexx[3] = xCoords[i+1+n1*j];
          vertexx[4] = xCoords[i+n1*j];

          vertexy[0] = yCoords[j+n2*i];
          vertexy[1] = yCoords[j+1+n2*i];
          vertexy[2] = yCoords[j+1+n2*(i+1)];
          vertexy[3] = yCoords[j+n2*(i+1)];
          vertexy[4] = yCoords[j+n2*i];

          C2F(dr)("xliness","str",vertexx,vertexy,fill,&one,&five,
            PI0,PD0,PD0,PD0,PD0,0L,0L);
        }
      }

      frame_clip_off();  
#ifdef _MSC_VER
      if ( flag_DO == 1) ReleaseWinHdc();
      
#endif

      FREE( xCoords ) ;
      FREE( yCoords ) ;
      FREE( xvect   )  ;
      FREE( yvect   )  ;
    }
    break;
  case 1: /* Matplot case */
    {
      /* In this case (and inside Matplot1 too but Matplot and Matplot1 are almost the same), */
      /* dim x = n2 and dim y = n1 (cf. scimatplot in matdes.c) */ /* F.Leray 20.05.05 */
      sciSubWindow * ppsubwin = pSUBWIN_FEATURE(parentSubWin) ;

      if( !sciGetIs3d(parentSubWin) )
      {
        int * xCoords = NULL ;
        int * yCoords = NULL ;
        int j ;
  
        xCoords = MALLOC( n2 * sizeof(int) ) ;
        if ( xCoords == NULL )
        {
          sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
          return -1 ;
        }

        yCoords = MALLOC( n1 * sizeof(int) ) ;
        if ( yCoords == NULL )
        {
          FREE( xCoords ) ;
          sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
          return -1 ;
        }

        for ( j =0 ; j < n2 ; j++) { xCoords[j] = XScale(j+0.5) ; }
        for ( j =0 ; j < n1 ; j++) { yCoords[j] = YScale(((n1-1)-j+0.5)) ; }
#ifdef _MSC_VER
        flag_DO = MaybeSetWinhdc();
#endif
        frame_clip_on(); 
        if( ppsubwin->axes.reverse[0] || ppsubwin->axes.reverse[1] )
        {
          GraySquare1_NGreverse(xCoords,yCoords,ppGray->pvecz,n1,n2,parentSubWin);
        }
        else
        {
          GraySquare1(xCoords,yCoords,ppGray->pvecz,n1,n2);  
        }
        frame_clip_off();
#ifdef _MSC_VER
        if ( flag_DO == 1) { ReleaseWinHdc() ; }
#endif

        FREE( xCoords );
        FREE( yCoords ) ;
      }
      else
      {
        /* 3D version */
        double * xvect   = NULL ;
        double * yvect   = NULL ;
        int    * xCoords = NULL ;
        int    * yCoords = NULL ;
        int i ;
        int j ;

        /* Warning here (Matplot case) : n1 becomes n2 and vice versa */
        xvect = MALLOC( n2 * sizeof(double) ) ;
        if ( xvect == NULL )
        {
          sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
          return -1 ;
        }

        yvect = MALLOC( n1 * sizeof(double) ) ;
        if ( yvect == NULL )
        {
          FREE( xvect ) ;
          sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
          return -1 ;
        }

        for ( i = 0 ; i < n2 ; i++ ) { xvect[i] = i + 0.5 ; }
        for ( i = 0 ; i < n1 ; i++ ) { yvect[i] = n1 - 1 - i + 0.5 ; }

        xCoords = MALLOC( n1 * n2 * sizeof(int) ) ;
        if ( xCoords == NULL )
        {
          FREE( xvect ) ;
          FREE( yvect ) ;
          sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
          return -1 ;
        }

        yCoords = MALLOC( n1 * n2 * sizeof(int) ) ;
        if ( yCoords == NULL )
        {
          FREE( xvect ) ;
          FREE( yvect ) ;
          FREE( xCoords ) ;
          sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
          return -1 ;
        }

        ReverseDataFor3DXonly(parentSubWin,xvect,n2);
        ReverseDataFor3DYonly(parentSubWin,yvect,n1);


        for ( i =0 ; i < n2 ; i++)  /* on x*/
        {
          for ( j =0 ; j < n1 ; j++)  /* on y */
          {
            trans3d(parentSubWin,1,&xCoords[i+j*n2],&yCoords[j+i*n1],&xvect[i],&yvect[j],NULL);
          }
        }
#ifdef _MSC_VER
        flag_DO = MaybeSetWinhdc();
#endif
        frame_clip_on(); 

        /* draw the filled projected rectangle */
        for (i = 0 ; i < (n2)-1 ; i++)
        {
          for (j = 0 ; j < (n1)-1 ; j++)
          {
            integer vertexx[5], vertexy[5];
            int five = 5, one = 1;
            integer fill;

            fill = (int ) - ppGray->pvecz[(n1-1)*i+j];

            vertexx[0] = xCoords[i+n2*j];
            vertexx[1] = xCoords[i+n2*(j+1)];
            vertexx[2] = xCoords[i+1+n2*(j+1)];
            vertexx[3] = xCoords[i+1+n2*j];
            vertexx[4] = xCoords[i+n2*j];

            vertexy[0] = yCoords[j+n1*i];
            vertexy[1] = yCoords[j+1+n1*i];
            vertexy[2] = yCoords[j+1+n1*(i+1)];
            vertexy[3] = yCoords[j+n1*(i+1)];
            vertexy[4] = yCoords[j+n1*i];

            C2F(dr)("xliness","str",vertexx,vertexy,&fill,&one,&five,
              PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
        }

        frame_clip_off();  
#ifdef _MSC_VER
        if ( flag_DO == 1) { ReleaseWinHdc() ; }
#endif

        FREE( xCoords ) ;
        FREE( yCoords ) ;
        FREE( xvect   ) ;
        FREE( yvect   ) ;
      }
    }
    break;
  case 2: /* Matplot1 case */
    /* In this case (and inside Matplot too but Matplot and Matplot1 are almost the same), */
    /* dim x = n2 and dim y = n1 (cf. scimatplot in matdes.c) */ /* F.Leray 20.05.05 */

    if( !sciGetIs3d(parentSubWin ) )
    {
      int * xCoords = NULL ;
      int * yCoords = NULL ;
      double xx[2] ;
      double yy[2];   
      int px1[2] ;
      int py1[2] ;
      int pn1 = 1 ;
      int pn2 = 2 ;
      int j ;

      xCoords = MALLOC( n2 * sizeof(int) ) ;
      if ( xCoords == NULL )
      {
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      yCoords = MALLOC( n1 * sizeof(int) ) ;
      if ( yCoords == NULL )
      {
        FREE( xCoords ) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      xx[0]=ppGray->pvecx[0];
      xx[1]=ppGray->pvecx[2];
      yy[0]=ppGray->pvecx[1];
      yy[1]=ppGray->pvecx[3];
      /** Boundaries of the frame **/
      C2F(echelle2d)(xx,yy,px1,py1,&pn1,&pn2,"f2i",3L); 
      for ( j =0 ; j < n2 ; j++)
      {
        xCoords[j]= (int) (( px1[1]*j + px1[0]*((n2-1)-j) )/(n2-1));
      }

      for ( j =0 ; j < n1 ; j++)
      {
        yCoords[j]= (int) (( py1[0]*j + py1[1]*((n1-1)-j) )/ (n1-1));
      }

#ifdef _MSC_VER
      flag_DO = MaybeSetWinhdc();
#endif

      frame_clip_on(); 
      GraySquare1(xCoords,yCoords,ppGray->pvecz,n1,n2); 
      frame_clip_off();
#ifdef _MSC_VER
      if ( flag_DO == 1 ) { ReleaseWinHdc() ; }
#endif
      FREE(xCoords) ;
      FREE(yCoords) ;
    }
    else{
      /* 3D version */
      double * xvect   = NULL;
      double * yvect   = NULL;
      int    * xCoords = NULL ;
      int    * yCoords = NULL ;
      double xx[2] ;
      double yy[2] ;
      int  i ;
      int  j ;

      /* Warning here (Matplot case) : n1 becomes n2 and vice versa */
      xvect = MALLOC( n2 * sizeof(double) ) ;
      if ( xvect == NULL )
      {
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      yvect = MALLOC( n1 * sizeof(double) ) ;
      if ( yvect == NULL )
      {
        FREE( xvect ) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      xCoords = MALLOC( n1 * n2 * sizeof(int) ) ;
      if ( xCoords == NULL )
      {
        FREE( xvect ) ;
        FREE( yvect ) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      yCoords = MALLOC( n1 * n2 * sizeof(int) ) ;
      if ( yCoords == NULL )
      {
        FREE( xvect ) ;
        FREE( yvect ) ;
        FREE( xCoords ) ;
        sciprint( "Unable to allocate temporary vector, memory full.\n" ) ;
        return -1 ;
      }

      xx[0]=ppGray->pvecx[0];
      xx[1]=ppGray->pvecx[2];
      yy[0]=ppGray->pvecx[1];
      yy[1]=ppGray->pvecx[3];


      /** Boundaries of the frame **/
      for ( i =0 ; i < n2 ; i++)
      {
        xvect[i]= (( xx[1]*i + xx[0]*((n2-1)-i) )/(n2-1));
      }

      for ( j = 0 ; j < n1 ; j++ )
      {
        yvect[j]= (( yy[0]*j + yy[1]*((n1-1)-j) )/ (n1-1)); 
      }

      ReverseDataFor3DXonly(parentSubWin,xvect,n2);
      ReverseDataFor3DYonly(parentSubWin,yvect,n1);


      for ( i =0 ; i < n2 ; i++)  /* on x*/
      {
        for ( j =0 ; j < n1 ; j++)  /* on y */
        {
          trans3d(parentSubWin,1,&xCoords[i+j*n2],&yCoords[j+i*n1],
          &xvect[i],&yvect[j],NULL);
        }
      }
#ifdef _MSC_VER
      flag_DO = MaybeSetWinhdc();
#endif
      frame_clip_on(); 

      /* draw the filled projected rectangle */
      for ( i = 0 ; i < (n2)-1 ; i++ )
      {
        for ( j = 0 ; j < (n1)-1 ; j++ )
        {
          integer vertexx[5], vertexy[5];
          int five = 5, one = 1;
          integer fill;

          fill = (int) - ppGray->pvecz[(n1-1)*i+j];

          vertexx[0] = xCoords[i+n2*j];
          vertexx[1] = xCoords[i+n2*(j+1)];
          vertexx[2] = xCoords[i+1+n2*(j+1)];
          vertexx[3] = xCoords[i+1+n2*j];
          vertexx[4] = xCoords[i+n2*j];

          vertexy[0] = yCoords[j+n1*i];
          vertexy[1] = yCoords[j+1+n1*i];
          vertexy[2] = yCoords[j+1+n1*(i+1)];
          vertexy[3] = yCoords[j+n1*(i+1)];
          vertexy[4] = yCoords[j+n1*i];

          C2F(dr)("xliness","str",vertexx,vertexy,&fill,&one,&five,
            PI0,PD0,PD0,PD0,PD0,0L,0L);
        }
      }

      frame_clip_off();  
#ifdef _MSC_VER
      if ( flag_DO == 1 ) { ReleaseWinHdc() ; }
#endif
      FREE( xCoords ) ;
      FREE( yCoords ) ;
      FREE( xvect   ) ;
      FREE( yvect   ) ;
    }
    break;
  default:
    break;
  }

  return 0 ;
}
/*-------------------------------------------------------------------------------------*/
