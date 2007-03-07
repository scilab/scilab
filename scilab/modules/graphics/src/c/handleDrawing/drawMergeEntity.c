/*------------------------------------------------------------------------*/
/* file: drawMergeEntity.c                                                */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a Merge handle                */
/*------------------------------------------------------------------------*/

#include "drawMergeEntity.h"
#include "GetProperty.h"
#include "Xcall1.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "axesScale.h"
#include "PloEch.h"
#include "periScreen.h"
#include "drawMarks.h"
#include "Plo3d.h"
#include "Champ.h"
#include "handleDrawing/drawSegsEntity.h"

/*-------------------------------------------------------------------------------------*/
int drawMergeEntity( sciPointObj * pObj )
{
  sciPointObj * parentSubWin = sciGetParentSubwin(pObj) ;
  int DPI[2] ;
  
  GetDPIFromDriver(DPI) ;

  if ( !pSUBWIN_FEATURE(parentSubWin)->facetmerge ) { return 0 ; } 

  DrawMerge3d( parentSubWin, pObj, DPI ) ;  /* TEST on sciGetVisibility inside */
  return 0 ;
}
/*-------------------------------------------------------------------------------------*/
extern double C2F(dsort)() ;
void DrawMerge3d( sciPointObj * psubwin, sciPointObj * pmerge, int * DPI )
{
  int N,i,j,index,p,max_p,n1,npoly;
  double * dist;
  double X[5],Y[5],Z[5];
  double * Zoriginal = NULL; /* used to conserve Z wether or not z axis is reversed ! (see plo3dn.c) */
  double * x = NULL ;
  double * y = NULL ;
  double * z = NULL ;
  sciPointObj *pobj; 
  int *locindex;
  int *polyx,*polyy,fill[20];/* here we suppose there is no more than 20 edge in a facet */
  int k1,iflag;
  int pstyle = 0;
  int whiteid,verbose=0,narg;
  static double zmin,zmax,xmoy,ymoy,zmoy,zl;
  int context[6];


  sciSubWindow * ppsubwin = pSUBWIN_FEATURE (psubwin);
  sciMerge     * ppMerge  = pMERGE_FEATURE (pmerge) ;
  int u;
  /* change to static arrays : indeed, no need to recopy each time the entire data of a given object */
  double   xtmp[2] ; 
  double   ytmp[2] ;
  double   ztmp[4] ;

  double * verticesX = NULL ; /* normally max size is 4 for facets (2 for lines and segs) but may be one day we will manage greater complex patchs (that is why the 10) */
  double * verticesY = NULL ;  /* no it can have any size, but display of facets with more than 4 edges might not be accurate */
  double * verticesZ = NULL ;

#ifdef _MSC_VER 
  int hdcflag;
#endif

  if(sciGetEntityType (psubwin) != SCI_SUBWIN) return;
  N = ppMerge->N ; /* total number of elements */

  dist     = MALLOC( N * sizeof(double) ) ;
  locindex = MALLOC( N * sizeof(int   ) ) ;

  if ( dist == NULL || locindex == NULL )
  {
    FREE( dist ) ;
    FREE( locindex ) ;
    sciprint("DrawMerge3d : MALLOC No more Place\n");
    return;
  }

  /* ========================================================================
  * compute drawing order (painter algorithm) *
  may be moved into Merge3d
  * ========================================================================*/
  max_p=0; /* the maximum number of edge in a facet */
  for ( i =0 ; i < N ; i++) { /* loop on element*/
    pobj=(sciPointObj *) sciGetPointerFromHandle (ppMerge->from_entity[i]);
    index = ppMerge->index_in_entity[i];

    /*compute element coordinates */
    switch (sciGetEntityType (pobj)) {  
    case SCI_SURFACE:
      if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D) { /* x,y,Z */
        int l,k,n1_,n2;

        n1_= pSURFACE_FEATURE (pobj)->dimzx;
        n2= pSURFACE_FEATURE (pobj)->dimzy;
        l=(int)(index/(n1_-1));
        k=index-l*(n1_-1);

        xtmp[0] = pSURFACE_FEATURE (pobj)->pvecx[k];
        xtmp[1] = pSURFACE_FEATURE (pobj)->pvecx[k+1];

        /* I didn't use ReverseDataFor3D because dim n1 is not the same for x, y and z  */
        if(ppsubwin->axes.reverse[0] == TRUE){
          /* agir sur x */
          if(ppsubwin->logflags[0]=='l'){
            for(u=0;u<2;u++){
              xtmp[u] = log10(xtmp[u]);
              xtmp[u] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp[u]);
              xtmp[u] = exp10(xtmp[u]);
            }
          }
          else {
            xtmp[0] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp[0]);
            xtmp[1] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp[1]);
          }
        }

        ytmp[0] = pSURFACE_FEATURE (pobj)->pvecy[l];
        ytmp[1] = pSURFACE_FEATURE (pobj)->pvecy[l+1];

        if(ppsubwin->axes.reverse[1] == TRUE){ 
          /* agir sur y */
          if(ppsubwin->logflags[1]=='l'){
            for(u=0;u<2;u++){
              ytmp[u] = log10(ytmp[u]);
              ytmp[u] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp[u]);
              ytmp[u] = exp10(ytmp[u]);
            }
          }
          else{
            ytmp[0] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp[0]);
            ytmp[1] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp[1]);
          }
        }

        ztmp[0] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1_];
        ztmp[1] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1_+n1_];
        ztmp[2] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1_+n1_+1];
        ztmp[3] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1_+1];


        if(ppsubwin->axes.reverse[2] == TRUE){ 
          /* agir sur z */
          if(ppsubwin->logflags[2]=='l'){
            for(u=0;u<4;u++){
              ztmp[u] = log10(ztmp[u]);
              ztmp[u] = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp[u]);
              ztmp[u] = exp10(ztmp[u]);
            }
          }
          else{
            for(u=0;u<4;u++)
              ztmp[u] = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp[u]);
          }
        }

        n2= pSURFACE_FEATURE (pobj)->dimzy;
        X[0]=X[1]=xtmp[0];
        X[2]=X[3]=xtmp[1];

        Z[0]=ztmp[0];
        Z[1]=ztmp[1];
        Z[2]=ztmp[2];
        Z[3]=ztmp[3];

        Y[0]=Y[3]=ytmp[0];
        Y[1]=Y[2]=ytmp[1];

        Zoriginal = &(pSURFACE_FEATURE (pobj)->pvecz[k+l*n1_]);

        p=4;
        x=X;y=Y;z=Z;

      }
      else
      {
        /* facets */
        p = pSURFACE_FEATURE (pobj)->dimzx; /* number of edges in the facets */

        verticesX = MALLOC( p * sizeof(double) ) ;
        verticesY = MALLOC( p * sizeof(double) ) ;
        verticesZ = MALLOC( p * sizeof(double) ) ;

        if ( verticesX == NULL || verticesY == NULL || verticesZ == NULL )
        {
          FREE(verticesX) ;
          FREE(verticesY) ;
          FREE(verticesZ) ;
          sciprint( "sciDrawObj: Unable to allocate temporary vector, memory full.\n" ) ;
          return ;
        }

        retrieveFacetVertices( pobj, index, verticesX, verticesY, verticesZ, &Zoriginal ) ;

        x = verticesX ;
        y = verticesY ;
        z = verticesZ ;

      }
      break;
    case  SCI_POLYLINE:
      {
        sciPolyline * ppPolyLine = pPOLYLINE_FEATURE (pobj) ;
        n1 = ppPolyLine->n1 ;
        p  = 0 ;
        if ( sciGetIsMark(pobj) ) { p = 1 ; } /* F.Leray 20.01.05 A REVOIR ICI*/
        if ( sciGetIsLine(pobj) ) { p = 2 ; }

        if ( ppPolyLine->plot != 2 && sciGetIsMark(pobj) == 1 )
        {
          xtmp[0] = ppPolyLine->pvx[index];
          xtmp[1] = xtmp[0] ;

          ytmp[0] = ppPolyLine->pvy[index];
          ytmp[1] = ytmp[0] ; /* used by trans3d + drawing : case 0,1 and 4 */

          if( ppPolyLine->pvz != NULL )
          {
            ztmp[0] = ppPolyLine->pvz[index];
            ztmp[1] = ztmp[0];
          }
        }
        else
        {
          xtmp[0] = ppPolyLine->pvx[index];
          xtmp[1] = ppPolyLine->pvx[index+1];

          ytmp[0] = ppPolyLine->pvy[index];
          ytmp[1] = ppPolyLine->pvy[index+1]; /* used by trans3d + drawing : case 0,1 and 4 */
          if( ppPolyLine->pvz != NULL )
          {
            ztmp[0] = ppPolyLine->pvz[index];
            ztmp[1] = ppPolyLine->pvz[(index+1)%n1];
          }
        }

        if( ppPolyLine->pvz != NULL )
        {
          ReverseDataFor3D(psubwin,xtmp,ytmp,ztmp,2);
        }
        else
        {
          ReverseDataFor3D(psubwin,xtmp,ytmp,(double *) NULL,2);
        }

        switch ( ppPolyLine->plot )
        {
        case 0: case 1: case 4: /*linear interpolation */
          x=&(xtmp[0]);
          y=&(ytmp[0]);
          if ( ppPolyLine->pvz != (double *) NULL) 
            z=&(ztmp[0]);
          else
            z=(double *)NULL;
          break;
        case 2: /* staircase */ /* just for completion  */
          X[0]=xtmp[0];
          X[1]=xtmp[1];
          Y[0]=ytmp[0];
          Y[1]=ytmp[0];
          if (ppPolyLine->pvz != (double *) NULL) {
            Z[0]=ztmp[0];
            Z[1]=ztmp[0];
            z=Z;
          }
          else 
            z=(double *)NULL;
          x=X;y=Y;
          break;
        case 3 : /* vertical bar */ /* just for completion  */
          X[0]=xtmp[0];
          X[1]=xtmp[0];
          Y[0]=0.0;
          if(ppsubwin->logflags[1]=='l') /* when logscale on Y, special treatment because we can not have Y == 0 */
            Y[0] = ppsubwin->FRect[1];
          Y[1]=ytmp[0];
          if ( ppPolyLine->pvz != (double *) NULL) {
            Z[0]=ztmp[0];
            Z[1]=ztmp[0];
            z=Z;
          }
          else 
            z=(double *)NULL;
          x=X;y=Y;
          break;
        case 5: /* patch */
          x=xtmp;
          y=ytmp;
          if ( ppPolyLine->pvz != (double *) NULL)
            z=ztmp;
          else
            z= (double *) NULL;
          break;
        }
      }
      break;
    case SCI_SEGS:
      {
        sciSegs * ppSegs = pSEGS_FEATURE(pobj) ;
        p = 2;
      /***************/
        if ( ppSegs->ptype == 0 )
        {
          xtmp[0] = ppSegs->vx[2*index];
          xtmp[1] = ppSegs->vx[2*index+1];

          ytmp[0] = ppSegs->vy[2*index];
          ytmp[1] = ppSegs->vy[2*index+1];

          if( ppSegs->vz != NULL )
          {
            ztmp[0] = ppSegs->vz[2*index];
            ztmp[1] = ppSegs->vz[2*index+1];
          }

        }
        else
        {
          /* vx is size n1, vy n2 and vfx and vfy n1 x n2 */
          int column = index / ppSegs->Nbr1 ;
          int row    = index - column * ppSegs->Nbr1 ; 
          double maxGap = 0.707 * computeGridMinGap( ppSegs->vx, ppSegs->vy, ppSegs->Nbr1, ppSegs->Nbr2 ) ;

          xtmp[0] = ppSegs->vx[row] ;
          ytmp[0] = ppSegs->vy[column] ;

          if (ppSegs->typeofchamp == 0 )
          {
            double maxNorm = getLongestVector( ppSegs->vfx, ppSegs->vfy, ppSegs->Nbr1, ppSegs->Nbr2, 1.0, 1.0 ) ;
            xtmp[1] = ppSegs->vx[row]    + maxGap * ppSegs->vfx[index] / maxNorm ;
            ytmp[1] = ppSegs->vy[column] + maxGap * ppSegs->vfy[index] / maxNorm ;
          }
          else
          {
            /* colored */
            double norm = sqrt( ppSegs->vfx[index] * ppSegs->vfx[index] + ppSegs->vfy[index] * ppSegs->vfy[index] ) ;
            xtmp[1] = ppSegs->vx[row]    + maxGap * ppSegs->vfx[index] / norm ;
            ytmp[1] = ppSegs->vy[column] + maxGap * ppSegs->vfy[index] / norm ; ;
          }


          if( ppSegs->vz != NULL )
          {
            ztmp[0] = ppSegs->vz[row] ;
            ztmp[1] = ppSegs->vz[row] + ppSegs->vfz[index] ;
          }

        }

        if( ppSegs->vz != NULL )
        {
          ReverseDataFor3D(psubwin,xtmp,ytmp,ztmp,2);
        }
        else
        {
          ReverseDataFor3D(psubwin,xtmp,ytmp, NULL,2);
        }
        /**************/

        X[0]=xtmp[0];
        X[1]=xtmp[1];
        Y[0]=ytmp[0];
        Y[1]=ytmp[1];
        if ( ppSegs->vz != (double *) NULL) {
          Z[0]=ztmp[0];
          Z[1]=ztmp[1];
          z=Z;
        }
        else
        {
          z=(double *)NULL;
        }
        x=X;y=Y;
      }
      break;
    case  SCI_RECTANGLE: 
      {
        double rectx[4],recty[4],rectz[4];
        p = 5;

        rectx[0]= rectx[3] =pRECTANGLE_FEATURE (pobj)->x;
        rectx[1]= rectx[2] =pRECTANGLE_FEATURE (pobj)->x+pRECTANGLE_FEATURE (pobj)->width;   
        recty[0]= recty[1] =pRECTANGLE_FEATURE (pobj)->y;   
        recty[2]= recty[3] =pRECTANGLE_FEATURE (pobj)->y-pRECTANGLE_FEATURE (pobj)->height;
        rectz[0]= rectz[1]=rectz[2]= rectz[3]=pRECTANGLE_FEATURE (pobj)->z;

        ReverseDataFor3D(psubwin, rectx, recty, rectz, 4);

        X[0]=X[1]=X[4]=rectx[0];
        Y[0]=Y[3]=Y[4]=recty[0];
        X[2]=X[3]=rectx[1];
        Y[1]=Y[2]=recty[2];
        Z[0]=Z[1]=Z[2]=Z[3]=Z[4]=rectz[0];
        x=X;y=Y;z=Z; 
      }
      break;
    default:
      p = 0;
    }
    /* each element is represented by its barycenter */
    xmoy=0.0;ymoy=0.0;zmoy=0.0;
    if (z != (double *)NULL) {
      for ( j= 0 ; j < p ; j++) {
        xmoy += x[j];  ymoy += y[j];  zmoy += z[j];
      }
    }
    else {
      for ( j= 0 ; j < p ; j++) {
        xmoy += x[j];  ymoy += y[j];
      }
    }
    /* Compute the distance from the observer */
    dist[i]=  TRZ(xmoy/p,ymoy/p,zmoy/p);
    max_p=Max(max_p,p);

  } /* END of FOR here F.Leray 01.12.04 */

  /* sort the distance in decreasing order */
  C2F(dsort)(dist,&N,locindex); 

  FREE(verticesX) ;
  FREE(verticesY) ;
  FREE(verticesZ) ;

  /* ========================================================================
  * draw each element in the order given by locindex
  * ========================================================================*/
  C2F(dr)("xget","lastpattern",&verbose,&whiteid,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  zmin = pSUBWIN_FEATURE (psubwin)->SRect[4];
  zmax = pSUBWIN_FEATURE (psubwin)->SRect[5];

  polyx = MALLOC( (max_p+1) * sizeof(int) ) ;
  polyy = MALLOC( (max_p+1) * sizeof(int) ) ;

  if ( polyx == NULL || polyy == NULL )
  {
    FREE( polyx ) ;
    FREE( polyy ) ;
    FREE( dist ) ;
    FREE( locindex ) ;
    sciprint("DrawMerge3d : MALLOC No more Place\n");

    return;
  }

  npoly=1; 
  for ( i = N ; i>0 ; i--) { /* loop on elements */
    int j2,nok;
    j2=locindex[i-1]-1;
    index= ppMerge->index_in_entity[j2];
    pobj=(sciPointObj *) sciGetPointerFromHandle (ppMerge->from_entity[j2]);
    if ( sciGetRealVisibility(pobj) ) {
      /* build the element coordinates */
      switch (sciGetEntityType (pobj)) {  
      case SCI_SURFACE:
        if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D) { /* x,y,Z */
          int l,k,n1_,n2;

          n1_= pSURFACE_FEATURE (pobj)->dimzx;
          n2= pSURFACE_FEATURE (pobj)->dimzy;
          l=(int)(index/(n1_-1));
          k=index-l*(n1_-1);

          xtmp[0] = pSURFACE_FEATURE (pobj)->pvecx[k];
          xtmp[1] = pSURFACE_FEATURE (pobj)->pvecx[k+1];

          /* I didn't use ReverseDataFor3D because dim n1 is not the same for x, y and z  */
          if(ppsubwin->axes.reverse[0] == TRUE){
            /* agir sur x */
            if(ppsubwin->logflags[0]=='l'){
              for(u=0;u<2;u++){
                xtmp[u] = log10(xtmp[u]);
                xtmp[u] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp[u]);
                xtmp[u] = exp10(xtmp[u]);
              }
            }
            else {
              xtmp[0] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp[0]);
              xtmp[1] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],xtmp[1]);
            }
          }

          ytmp[0] = pSURFACE_FEATURE (pobj)->pvecy[l];
          ytmp[1] = pSURFACE_FEATURE (pobj)->pvecy[l+1];

          if(ppsubwin->axes.reverse[1] == TRUE){ 
            /* agir sur y */
            if(ppsubwin->logflags[1]=='l'){
              for(u=0;u<2;u++){
                ytmp[u] = log10(ytmp[u]);
                ytmp[u] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp[u]);
                ytmp[u] = exp10(ytmp[u]);
              }
            }
            else{
              ytmp[0] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp[0]);
              ytmp[1] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],ytmp[1]);
            }
          }

          ztmp[0] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1_];
          ztmp[1] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1_+n1_];
          ztmp[2] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1_+n1_+1];
          ztmp[3] = pSURFACE_FEATURE (pobj)->pvecz[k+l*n1_+1];


          if(ppsubwin->axes.reverse[2] == TRUE){ 
            /* agir sur z */
            if(ppsubwin->logflags[2]=='l'){
              for(u=0;u<4;u++){
                ztmp[u] = log10(ztmp[u]);
                ztmp[u] = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp[u]);
                ztmp[u] = exp10(ztmp[u]);
              }
            }
            else{
              for(u=0;u<4;u++)
                ztmp[u] = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],ztmp[u]);
            }
          }

          n2= pSURFACE_FEATURE (pobj)->dimzy;
          X[0]=X[1]=xtmp[0];
          X[2]=X[3]=xtmp[1];
          Z[0]=ztmp[0];
          Z[1]=ztmp[1];
          Z[2]=ztmp[2];
          Z[3]=ztmp[3];

          Y[0]=Y[3]=ytmp[0];
          Y[1]=Y[2]=ytmp[1];

          Zoriginal = &(pSURFACE_FEATURE (pobj)->pvecz[k+l*n1_]);

          p=4;
          x=X;y=Y;z=Z;

        }
        else
        {
          /* facets */
          p = pSURFACE_FEATURE (pobj)->dimzx; /* number of edges in the facets */

          verticesX = MALLOC( p * sizeof(double) ) ;
          verticesY = MALLOC( p * sizeof(double) ) ;
          verticesZ = MALLOC( p * sizeof(double) ) ;

          if ( verticesX == NULL || verticesY == NULL || verticesZ == NULL )
          {
            FREE(verticesX) ;
            FREE(verticesY) ;
            FREE(verticesZ) ;
            sciprint( "sciDrawObj: Unable to allocate temporary vector, memory full.\n" ) ;
            return ;
          }

          retrieveFacetVertices( pobj, index, verticesX, verticesY, verticesZ, &Zoriginal ) ;

          x = verticesX ;
          y = verticesY ;
          z = verticesZ ;
        }
        break;
      case  SCI_POLYLINE:
        {
          sciPolyline * ppPolyLine = pPOLYLINE_FEATURE( pobj ) ;
          n1 = ppPolyLine->n1 ;
          p=0;
          if ( sciGetIsMark( pobj ) ) { p=1 ; } /* F.Leray 20.01.05 A REVOIR ICI*/
          if ( sciGetIsLine( pobj ) ) { p=2 ; }

          /* check if we want to draw segments or marks */
          if ( ppPolyLine->plot != 2 && sciGetIsMark(pobj) == 1 )
          {
            xtmp[0] = ppPolyLine->pvx[index];
            xtmp[1] = xtmp[0] ;

            ytmp[0] = ppPolyLine->pvy[index];
            ytmp[1] = ytmp[0] ; /* used by trans3d + drawing : case 0,1 and 4 */
            if(ppPolyLine->pvz != NULL)
            {
              ztmp[0] = ppPolyLine->pvz[index];
              ztmp[1] = ztmp[0] ;
            }
          }
          else
          {
            xtmp[0] = ppPolyLine->pvx[index];
            xtmp[1] = ppPolyLine->pvx[index+1];

            ytmp[0] = ppPolyLine->pvy[index];
            ytmp[1] = ppPolyLine->pvy[index+1]; /* used by trans3d + drawing : case 0,1 and 4 */

            if(ppPolyLine->pvz != NULL)
            {
              ztmp[0] = ppPolyLine->pvz[index];
              ztmp[1] = ppPolyLine->pvz[(index+1)%n1];
            }
          }

          if(ppPolyLine->pvz != NULL)
            ReverseDataFor3D(psubwin,xtmp,ytmp,ztmp,2);
          else
            ReverseDataFor3D(psubwin,xtmp,ytmp,(double *) NULL,2);

          switch (ppPolyLine->plot) {
      case 0: case 1: case 4: /*linear interpolation */
        x=&(xtmp[0]);
        y=&(ytmp[0]);
        if (ppPolyLine->pvz != (double *) NULL) 
          z=&(ztmp[0]);
        else
          z=(double *)NULL;
        break;
      case 2: /* staircase */ /* just for completion  */
        X[0]=xtmp[0];
        X[1]=xtmp[1];
        Y[0]=ytmp[0];
        Y[1]=ytmp[0];
        if (ppPolyLine->pvz != (double *) NULL) {
          Z[0]=ztmp[0];
          Z[1]=ztmp[0];
          z=Z;
        }
        else 
          z=(double *)NULL;
        x=X;y=Y;
        break;
      case 3 : /* vertical bar */ /* just for completion  */
        X[0]=xtmp[0];
        X[1]=xtmp[0];
        Y[0]=0.0;
        if(ppsubwin->logflags[1]=='l') /* when logscale on Y, special treatment because we can not have Y == 0 */
          Y[0] = ppsubwin->FRect[1];
        Y[1]=ytmp[0];
        if (ppPolyLine->pvz != (double *) NULL) {
          Z[0]=ztmp[0];
          Z[1]=ztmp[0];
          z=Z;
        }
        else 
          z=(double *)NULL;
        x=X;y=Y;
        break;
      case 5: /* patch*/
        x=xtmp;
        y=ytmp;
        if (ppPolyLine->pvz != (double *) NULL)
          z=ztmp;
        else
          z= (double *) NULL;
        break;
          }
        }
        break;
      case  SCI_SEGS:
        {
          sciSegs * ppSegs = pSEGS_FEATURE(pobj) ;
          p = 2;
          /***************/

          if (ppSegs->ptype == 0) /* ptype == 0 F.Leray : This is NOT A champ */
          {  
            if (ppSegs->iflag == 1) {
              pstyle=sciGetGoodIndex(pobj, ppSegs->pstyle[index]);
            }
            else{
              pstyle=sciGetGoodIndex(pobj, ppSegs->pstyle[0]);
            }
          }
          else
          {
            pstyle = 0 ;
          }

          iflag = ppSegs->iflag;

          /***************/
          if ( ppSegs->ptype == 0 )
          {
            xtmp[0] = ppSegs->vx[2*index];
            xtmp[1] = ppSegs->vx[2*index+1];

            ytmp[0] = ppSegs->vy[2*index];
            ytmp[1] = ppSegs->vy[2*index+1];

            if( ppSegs->vz != NULL )
            {
              ztmp[0] = ppSegs->vz[2*index];
              ztmp[1] = ppSegs->vz[2*index+1];
            }

          }
          else
          {
            /* vx is size n1, vy n2 and vfx and vfy n1 x n2 */
            int column = index / ppSegs->Nbr1 ;
            int row    = index - column * ppSegs->Nbr1 ;
            double maxGap = 0.707 * computeGridMinGap( ppSegs->vx, ppSegs->vy, ppSegs->Nbr1, ppSegs->Nbr2 ) ;

            xtmp[0] = ppSegs->vx[row] ;
            ytmp[0] = ppSegs->vy[column] ;

            if (ppSegs->typeofchamp == 0 )
            {
              double maxNorm = getLongestVector( ppSegs->vfx, ppSegs->vfy, ppSegs->Nbr1, ppSegs->Nbr2, 1.0, 1.0 ) ;
              xtmp[1] = ppSegs->vx[row]    + maxGap * ppSegs->vfx[index] / maxNorm ;
              ytmp[1] = ppSegs->vy[column] + maxGap * ppSegs->vfy[index] / maxNorm ;
            }
            else
            {
              /* colored */
              double norm = sqrt( ppSegs->vfx[index] * ppSegs->vfx[index] + ppSegs->vfy[index] * ppSegs->vfy[index] ) ;
              xtmp[1] = ppSegs->vx[row]    + maxGap * ppSegs->vfx[index] / norm ;
              ytmp[1] = ppSegs->vy[column] + maxGap * ppSegs->vfy[index] / norm ; ;
            }


            if( ppSegs->vz != NULL )
            {
              ztmp[0] = ppSegs->vz[row] ;
              ztmp[1] = ppSegs->vz[row] + ppSegs->vfz[index] ;
            }
          }


          if(ppSegs->vz != NULL)
            ReverseDataFor3D(psubwin,xtmp,ytmp,ztmp,2);
          else
            ReverseDataFor3D(psubwin,xtmp,ytmp,(double *) NULL,2);

          /**************/

          X[0]=xtmp[0];
          X[1]=xtmp[1];
          Y[0]=ytmp[0];
          Y[1]=ytmp[1];
          if (ppSegs->vz != (double *) NULL) {
            Z[0]=ztmp[0];
            Z[1]=ztmp[1];
            z=Z;
          }
          else
            z=(double *)NULL;
          x=X;y=Y;
        }
        break;
      case  SCI_RECTANGLE: 
        {
          double rectx[4],recty[4],rectz[4];
          p = 5;

          pstyle=0; /* arevoir */
          iflag=0; /* arevoir */

          rectx[0]= rectx[3] =pRECTANGLE_FEATURE (pobj)->x;
          rectx[1]= rectx[2] =pRECTANGLE_FEATURE (pobj)->x+pRECTANGLE_FEATURE (pobj)->width;   
          recty[0]= recty[1] =pRECTANGLE_FEATURE (pobj)->y;   
          recty[2]= recty[3] =pRECTANGLE_FEATURE (pobj)->y-pRECTANGLE_FEATURE (pobj)->height;
          rectz[0]= rectz[1]=rectz[2]= rectz[3]=pRECTANGLE_FEATURE (pobj)->z;

          ReverseDataFor3D(psubwin, rectx, recty, rectz, 4);

          X[0]=X[1]=X[4]=rectx[0];
          Y[0]=Y[3]=Y[4]=recty[0];
          X[2]=X[3]=rectx[1];
          Y[1]=Y[2]=recty[2];
          Z[0]=Z[1]=Z[2]=Z[3]=Z[4]=rectz[0];
          x=X;y=Y;z=Z; 
        }
        break;
      default:
        p = 0;
      }

      nok = 0;

      if (p > 0) {
        /* project 3D on 2D coordinates */
        if (z != (double *)NULL) {
          for ( j2 =0 ; j2 < p ; j2++) { 
            if (trans3d(psubwin ,1, &(polyx[j2]),&(polyy[j2]),&(x[j2]),&(y[j2]),&(z[j2]))==0) {
              nok=1;break; 
            }
          }
        }
        else {
          double zz=0.0;
          for ( j2 =0 ; j2 < p ; j2++) { 
            if (trans3d(psubwin ,1, &(polyx[j2]),&(polyy[j2]),&(x[j2]),&(y[j2]),&zz)==0) {
              nok=1;break; 
            }
          }
        }

        if(nok == 1) continue;

        /* draw element */
        context[0] = sciGetForeground (pobj);	
        context[1] = sciGetLineWidth (pobj);
        context[2] = sciGetLineStyle (pobj); 
        context[3] = 0;
        context[4] = sciGetMarkStyle(pobj);
        context[5] = sciGetLineWidth (pobj);
#ifdef _MSC_VER
        hdcflag=MaybeSetWinhdc();
#endif
        C2F (dr) ("xset", "thickness",  context+1, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
        C2F (dr) ("xset", "line style", context+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L); 
        C2F (dr) ("xset", "mark", context+4, context+5, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 4L, 4L);

        if (sciGetEntityType (pobj)==SCI_SURFACE) {
          int fg1  = pSURFACE_FEATURE (pobj)->hiddencolor;
          int flag = pSURFACE_FEATURE (pobj)->flag[0];
          int facteur = 1;

          polyx[p] = polyx[0] ;
          polyy[p] = polyy[0] ;
          p++ ; /*close the facet*/
          /* beware we got twice the first point now */

          /* facteur is used below */
          if(ppsubwin->axes.reverse[0] == TRUE) facteur = -facteur;
          if(ppsubwin->axes.reverse[1] == TRUE) facteur = -facteur;
          if(ppsubwin->axes.reverse[2] == TRUE) facteur = -facteur;

          if ((((polyx[1]-polyx[0])*(polyy[2]-polyy[0])-(polyy[1]-polyy[0])*(polyx[2]-polyx[0]))*facteur <  0) &&
            (fg1 >= 0)) { /* hidden face */

              if ( pSURFACE_FEATURE (pobj)->flagcolor != 0)
                fill[0] = (flag < 0 ) ? -fg1 : fg1 ;
              else
                fill[0] = (flag != 0 ) ? fg1 : flag ;
              if(sciGetIsLine(pobj)){
                C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 6L);
                C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 10L);
                C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
              if(sciGetIsMark(pobj))
                DrawMarks3D(pobj,5*npoly,polyx,polyy,DPI);
          }
          else {
            switch ( pSURFACE_FEATURE (pobj)->flagcolor) {
            case 0:
              fill[0]= flag ;
              if(sciGetIsLine(pobj)){
                C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 6L);
                C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 10L);
                C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p, PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
              if(sciGetIsMark(pobj))
                DrawMarks3D(pobj,5*npoly,polyx,polyy,DPI);
              break;
            case 1:
              zl = 0.0 ;
              for ( k1= 0 ; k1 < p - 1 ; k1++) { zl += z[k1] ; }
              zl /= p - 1.0 ;
              /* for ( k1= 0 ; k1 < p ; k1++) zl+= Zoriginal[k1]; */ /* F.Leray 01.12.04 : DO NOT REPLACE z by ztmp here : zmin & zmax are computed to work with z ! */
              fill[0] = inint( (whiteid-1)* ( zl - zmin ) / ( zmax- zmin ) ) + 1 ;

              if ( flag  < 0 ) { fill[0] = -fill[0] ; }
              if ( sciGetIsLine(pobj) )
              {
                C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 6L);
                C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 10L);
                C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p ,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
              if(sciGetIsMark(pobj))
                DrawMarks3D(pobj,5*npoly,polyx,polyy,DPI);
              break;
            case 2:
              fill[0]= (int) pSURFACE_FEATURE (pobj)->color[index];
              if ( flag < 0 ) fill[0]=-fill[0];
              if(sciGetIsLine(pobj)){
                C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 6L);
                C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 10L);
                C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p ,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
              if(sciGetIsMark(pobj))
                DrawMarks3D(pobj,5*npoly,polyx,polyy,DPI);
              break;
            case 3:
              p--;
              if ( (p) != 3 && (p) !=4 ) {
                sciprint("Interpolated shading is only allowed for polygons with 3 or 4 vertices\n");
                return;
              }
              else  {
                /* shade needs (int*) color */
                integer *cvect = NULL,ik;
                sciSurface * ppsurface = pSURFACE_FEATURE (pobj);

                if((cvect=MALLOC(ppsurface->nc*sizeof(integer)))==NULL){
                  sciprint("Allocation failed in merge for color matrix\n");
                  return;
                }    

                for(ik=0;ik<ppsurface->nc;ik++) 
                  cvect[ik] = (int) ppsurface->color[ik];

                C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 6L);
                C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 10L);

                scilab_shade(polyx,polyy,&(cvect[p*index]),p,ppsurface->flag[0]);
                FREE(cvect); cvect = NULL;
                if (sciGetIsMark (pobj))
                  DrawMarks3D (pobj, p,polyx,polyy,DPI);
              }
              break;
            case 4: /* new case for "flat" mode matlab compatibility */
              fill[0]= (int) pSURFACE_FEATURE (pobj)->color[index];
              if ( flag < 0 ) fill[0]=-fill[0];
              if(sciGetIsLine(pobj)){
                C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 6L);
                C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, PD0, PD0, PD0, 5L, 10L);
                C2F(dr)("xliness","str",polyx,polyy,fill,&npoly,&p ,PI0,PD0,PD0,PD0,PD0,0L,0L);
              }
              if(sciGetIsMark(pobj))
                DrawMarks3D(pobj,5*npoly,polyx,polyy,DPI);
              break;
            }
          }
        } /* end SCI_SURFACE*/
        else if(sciGetEntityType (pobj)==SCI_SEGS)
        {
          /* PSEGS here ! */
          sciSegs * ppSegs = pSEGS_FEATURE(pobj) ;
          if ( sciGetIsMark(pobj) )
          {
            integer v;
            double dv=0;
            int x_[4], markidsizenew[2];

            x_[0] = sciGetMarkForeground(pobj);

            markidsizenew[0] = sciGetMarkStyle(pobj);
            markidsizenew[1] = sciGetMarkSize (pobj);

            C2F (dr) ("xset", "dashes", x_, x_, x_+4, x_+4, x_+4, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", x_, x_, x_+4, x_+4, x_+4, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
              PD0, PD0, 0L, 0L);
            DrawNewMarks(pobj,p,polyx,polyy,DPI);
          }

          if(sciGetIsLine(pobj)){
            C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, 
              PD0, PD0, PD0, 5L, 6L);
            C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, 
              PD0, PD0, PD0, 5L, 10L);
            C2F (dr) ("xset", "thickness",  context+1, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
            C2F (dr) ("xset", "line style", context+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
            if ( ppSegs->ptype == 0 )
            {
              C2F(dr)("xsegs","v",polyx,polyy,&p,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
            }
            else
            {
              // Champ
              int arrowSize = computeRealArrowSize( pobj, p, polyx, polyy ) ;
              if( ppSegs->typeofchamp == 0 )
              {
                int sflag = 0 ;
                C2F(dr)("xarrow","v",polyx,polyy,&p,&arrowSize,&pstyle,&sflag,PD0,PD0,PD0,PD0,0L,0L);
              }
              else
              {
                int sflag = 1 ; /* colored */
                int arrowColor = computeArrowColor( ppSegs->vfx, ppSegs->vfy, ppSegs->Nbr1, ppSegs->Nbr2, index ) ;
                C2F(dr)("xarrow","v",polyx,polyy,&p,&arrowSize,&arrowColor,&sflag,PD0,PD0,PD0,PD0,0L,0L);
              }
            } 
          }
        }
        else if(sciGetEntityType (pobj)==SCI_RECTANGLE) { /* RECTANGLE case here ! */
          if( sciGetIsFilled(pobj) )
          {
            integer v;
            double dv=0;
            int x_[4],close=1;
            char str[2] = "xv";
            x_[0] = sciGetBackground(pobj);

            C2F (dr) ("xset", "dashes", x_, x_, x_+4, x_+4, x_+4, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", x_, x_, x_+4, x_+4, x_+4, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xarea", str, &p, polyx, polyy, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
          }

          if (sciGetIsMark(pobj) == TRUE){
            integer v;
            double dv=0;
            int x_[4], markidsizenew[2];

            x_[0] = sciGetMarkForeground(pobj);

            markidsizenew[0] = sciGetMarkStyle(pobj);
            markidsizenew[1] = sciGetMarkSize (pobj);

            C2F (dr) ("xset", "dashes", x_, x_, x_+4, x_+4, x_+4, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", x_, x_, x_+4, x_+4, x_+4, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
              PD0, PD0, 0L, 0L);
            DrawNewMarks(pobj,p,polyx,polyy,DPI);
          }

          if(sciGetIsLine(pobj)){
            C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, 
              PD0, PD0, PD0, 5L, 6L);
            C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, 
              PD0, PD0, PD0, 5L, 10L);
            C2F (dr) ("xset", "thickness",  context+1, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
            C2F (dr) ("xset", "line style", context+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L); 
            C2F(dr)("xsegs","v",polyx,polyy,&p,&pstyle,&iflag,PI0,PD0,PD0,PD0,PD0,0L,0L);
          }
        }
        else { /* POLYLINE case starts here ! */
          /* special case 5 */
          if(sciGetIsFilled(pobj) == TRUE && pPOLYLINE_FEATURE (pobj)->plot != 5) /* No filling if mode plot == 5 is selected */
          {
            integer v;
            double dv=0;
            int x_[4],close=1;
            char str[2] = "xv";
            x_[0] = sciGetBackground(pobj);

            C2F (dr) ("xset", "dashes", x_, x_, x_+4, x_+4, x_+4, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", x_, x_, x_+4, x_+4, x_+4, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xarea", str, &p, polyx, polyy, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,strlen(str));
          }

          if (sciGetIsMark(pobj) == TRUE){
            integer v;
            double dv=0;
            int x_[4], markidsizenew[2];

            x_[0] = sciGetMarkForeground(pobj);

            markidsizenew[0] = sciGetMarkStyle(pobj);
            markidsizenew[1] = sciGetMarkSize (pobj);

            C2F (dr) ("xset", "dashes", x_, x_, x_+4, x_+4, x_+4, &v, &dv,
              &dv, &dv, &dv, 5L, 4096);
            C2F (dr) ("xset", "foreground", x_, x_, x_+4, x_+4, x_+4, &v,
              &dv, &dv, &dv, &dv, 5L, 4096);

            C2F (dr) ("xset", "mark", &markidsizenew[0], &markidsizenew[1], PI0, PI0, PI0, PI0, PD0, PD0,
              PD0, PD0, 0L, 0L);
            DrawNewMarks(pobj,p,polyx,polyy,DPI);
          }

          if(sciGetIsLine(pobj)){
            int un = 1, deux = 2;
            context[3] = 0;

            C2F (dr) ("xset", "dashes",     context,   context,   context+3, context+3, context+3, PI0, PD0, 
              PD0, PD0, PD0, 5L, 6L);
            C2F (dr) ("xset", "foreground", context,   context,   context+3, context+3, context+3, PI0, PD0, 
              PD0, PD0, PD0, 5L, 10L);
            C2F (dr) ("xset", "thickness",  context+1, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 5L, 9L);
            C2F (dr) ("xset", "line style", context+2, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
            C2F (dr) ("xlines", "xv", &deux, polyx, polyy, &un, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
          }
        }
      }
    }

#ifdef _MSC_VER
    if ( hdcflag == 1) ReleaseWinHdc ();
#endif	  

  }
  FREE(dist);FREE(locindex);FREE(polyx);FREE(polyy);
  FREE(verticesX) ;
  FREE(verticesY) ;
  FREE(verticesZ) ;
}

/*------------------------------------------------------------------------------------------*/
void retrieveFacetVertices( sciPointObj *  pobj       ,
                           int            facetIndex ,
                           double         verticesX[],
                           double         verticesY[],
                           double         verticesZ[],
                           double      ** zOriginal   )
{
  int nbFacets = pSURFACE_FEATURE (pobj)->dimzx ;
  int i ;

  for ( i = 0 ; i < nbFacets ; i++ )
  {
    verticesX[i] = pSURFACE_FEATURE(pobj)->pvecx[facetIndex * nbFacets + i] ;
    verticesY[i] = pSURFACE_FEATURE(pobj)->pvecy[facetIndex * nbFacets + i] ;
    verticesZ[i] = pSURFACE_FEATURE(pobj)->pvecz[facetIndex * nbFacets + i] ;
  }

  ReverseDataFor3D( pobj, verticesX, verticesY, verticesZ, nbFacets ) ;

  *zOriginal = &(pSURFACE_FEATURE(pobj)->pvecz[facetIndex * nbFacets]) ;
}
/*------------------------------------------------------------------------------------------*/
