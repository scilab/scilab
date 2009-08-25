/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_data_property.c                                              */
/* desc : function to modify in Scilab the data field of                  */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "BasicAlgos.h"
#include "set_data_property.h"
#include "ColorMapManagement.h"
#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the champ data is now set as a tlist (like for surface objects) */
/* setchampdata(pobj,cstk(l2), &l3, &numrow3, &numcol3, fname) */
int setchampdata( sciPointObj * pobj, AssignedList * tlist )
{

  sciSegs * ppsegs = pSEGS_FEATURE (pobj);

  int nbRow[4] ;
  int nbCol[4] ;

  double * vx  = NULL ;
  double * vy  = NULL ;
  double * vfx = NULL ;
  double * vfy = NULL ;

  /* get parameters */

  vx  = createCopyDoubleMatrixFromList( tlist, &nbRow[0], &nbCol[0] ) ;
  vy  = createCopyDoubleMatrixFromList( tlist, &nbRow[1], &nbCol[1] ) ;
  vfx = createCopyDoubleMatrixFromList( tlist, &nbRow[2], &nbCol[2] ) ;
  vfy = createCopyDoubleMatrixFromList( tlist, &nbRow[3], &nbCol[3] ) ;

  /* check dim */
  if ( nbCol[0] != 1 || nbCol[1] != 1 )
  {
    Scierror(999, _("%s: Wrong type for argument #%d: Columns vectors expected.\n"),"Tlist",1);
    FREE( vx  ) ;
    FREE( vy  ) ;
    FREE( vfx ) ;
    FREE( vfy ) ;

    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow[2] != nbRow[0] || nbCol[2] != nbRow[1] || nbRow[3] != nbRow[2] || nbCol[3] != nbCol[2] )
  {
    Scierror(999, _("%s: Wrong size for arguments #%d and #%d: Incompatible length.\n"),"Tlist",3,4);
    FREE( vx  ) ;
    FREE( vy  ) ;
    FREE( vfx ) ;
    FREE( vfy ) ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow[0] * nbCol[0] == 0 || nbRow[1] * nbCol[1] == 0 || nbRow[2] * nbCol[2] == 0 || nbRow[3] * nbCol[3] == 0 )
  {
    FREE( vx  ) ;
    FREE( vy  ) ;
    FREE( vfx ) ;
    FREE( vfy ) ;
    return sciReturnEmptyMatrix() ;
  } 

  /* ok everything ok we can allocate new values */
  /* Update the dimensions Nbr1 and Nbr2 */
  ppsegs->Nbr1 = nbRow[0] ;
  ppsegs->Nbr2 = nbRow[1] ;

  FREE( ppsegs->vx  ) ;
  FREE( ppsegs->vy  ) ;
  FREE( ppsegs->vfx ) ;
  FREE( ppsegs->vfy ) ;
  ppsegs->vx = vx;
  ppsegs->vy = vy;
  ppsegs->vfx = vfx;
  ppsegs->vfy = vfy;

  return SET_PROPERTY_SUCCEED ;
}

/*--------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the grayplot data is now set as a tlist (like for surface and champ objects) */
/* setgrayplot(pobj,cstk(l2), &l3, &numrow3, &numcol3, fname) */
int setgrayplotdata( sciPointObj * pobj, AssignedList * tlist )
{

  sciGrayplot * ppgrayplot = pGRAYPLOT_FEATURE (pobj);

  int nbRow[3] ;
  int nbCol[3] ;

  double * pvecx = NULL ;
  double * pvecy = NULL ;
  double * pvecz = NULL ;

  pvecx = createCopyDoubleMatrixFromList( tlist, &nbRow[0], &nbCol[0] ) ;
  pvecy = createCopyDoubleMatrixFromList( tlist, &nbRow[1], &nbCol[1] ) ;
  pvecz = createCopyDoubleMatrixFromList( tlist, &nbRow[2], &nbCol[2] ) ;

  if ( nbCol[0] != 1 || nbCol[1] != 1 )
  {
    Scierror(999, _("%s: Wrong type for argument #%d: Columns vectors expected.\n"),"Tlist",1);
    FREE( pvecx ) ;
    FREE( pvecy ) ;
    FREE( pvecz ) ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow[2] != nbRow[0] || nbCol[2] != nbRow[1] )
  {
    Scierror(999, _("%s: Wrong size for arguments #%d: Incompatible length.\n"),"Tlist","Tlist",3);
    FREE( pvecx ) ;
    FREE( pvecy ) ;
    FREE( pvecz ) ;
    return 0;
  }

  if ( nbRow[0] * nbCol[0] == 0 || nbRow[1] * nbCol[1] == 0 || nbRow[2] * nbCol[2] == 0 )
  {
    FREE( pvecx ) ;
    FREE( pvecy ) ;
    FREE( pvecz ) ;
    return sciReturnEmptyMatrix() ;
  } 

  /* Update the dimensions nx and ny */
  ppgrayplot->nx = nbRow[0] ;
  ppgrayplot->ny = nbRow[1] ;

  /* Free the old values... */
  FREE(ppgrayplot->pvecx) ;
  FREE(ppgrayplot->pvecy) ;
  FREE(ppgrayplot->pvecz) ;
  ppgrayplot->pvecx = pvecx;
  ppgrayplot->pvecy = pvecy;
  ppgrayplot->pvecz = pvecz;

  return SET_PROPERTY_SUCCEED ;
}
/*--------------------------------------------------------------------------*/
/* set3ddata(pobj,cstk(l2), &l3, &numrow3, &numcol3) */
int set3ddata( sciPointObj * pobj, AssignedList * tlist )
{
  sciSurface * psurf = pSURFACE_FEATURE (pobj);


  int m1, n1, m2, n2, m3, n3 ;
  int m3n, n3n, ii;

  double * pvecx = NULL ;
  double * pvecy = NULL ;
  double * pvecz = NULL ;
  int dimvectx = 0 ;
  int dimvecty = 0 ;

  // number of specified colors
  int nc = 0 ;

  /* no copy now we just perfrom tests on the matrices */
  pvecx = getCurrentDoubleMatrixFromList( tlist, &m1, &n1 ) ;
  pvecy = getCurrentDoubleMatrixFromList( tlist, &m2, &n2 ) ;
  pvecz = getCurrentDoubleMatrixFromList( tlist, &m3, &n3 ) ;

  if ( m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1 )
  {
    if ( !(m1 == m2 && m2 == m3 && n1 == n2 && n2 == n3) )
    {
			Scierror(999, _("%s: Wrong size for arguments #%d, #%d and #%d: Incompatible length.\n"),"Tlist",1,2,3);
      return SET_PROPERTY_ERROR ;
    }
  }
  else 
  {
    if ( m2 * n2 != n3 )
    {
			Scierror(999, _("%s: Wrong size for arguments #%d and #%d: Incompatible length.\n"),"Tlist",2,3);
      return SET_PROPERTY_ERROR ;
    }
    if ( m1 * n1 != m3 )
    {
			Scierror(999, _("%s: Wrong size for arguments #%d and #%d: Incompatible length.\n"),"Tlist",1,3);
      return SET_PROPERTY_ERROR ;
    }
    if ( m1 * n1 <= 1 || m2 * n2 <= 1 ) 
    {
			Scierror(999, _("%s: Wrong size for arguments #%d and #%d: Should be >= %d.\n"),"Tlist",1,2,2);
      return SET_PROPERTY_ERROR ;
    }
  }

  if ( m1 * n1 == 0 || m2 * n2 == 0 || m3 * n3 == 0 )
  {
    return sciReturnEmptyMatrix() ;
  } 

  /* get color size if exists */
  if ( getAssignedListNbElement( tlist ) == 4 )
  {
    getCurrentDoubleMatrixFromList( tlist, &m3n, &n3n ) ;
    if ( m3n * n3n == m3 * n3 )
    {
      /* the color is a matrix, with same size as Z */
      psurf->izcol = 2 ; 
    }
    else if (m3n * n3n == n3 && (m3n == 1 || n3n == 1))
    {
      /* a vector with as many colors as facets */
      psurf->izcol = 1 ;
    }
    else
    {
      Scierror(999, _("Wrong size for %s element: A %d-by-%d matrix or a vector of size %d expected.\n"), "color", m3, n3, n3);
      return SET_PROPERTY_ERROR ;
    }
  }
  else
  {
    m3n = 0 ;
    n3n = 0 ;
    psurf->izcol = 0;
  }

  if ( m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1 ) /* NG beg */
  { /* case isfac=1;*/
    if( psurf->isfac != 1 )
    {
      Scierror(999, _("Can not change the %s of graphic object: its type is %s.\n"),"typeof3d","SCI_PLOT3D");
      return SET_PROPERTY_ERROR ;
    }
  }
  else
  { 
    /* case isfac=0;*/
    if(psurf->isfac != 0)
    {
      Scierror(999, _("Can not change the %s of graphic object: its type is %s.\n"),"typeof3d","SCI_FAC3D");
      return SET_PROPERTY_ERROR ;
    }
  }


  /* check the monotony on x and y */

  if ( psurf->isfac == 1 )
  {
    /* x is considered as a matrix */
    dimvectx = -1 ;
  }
  else if ( m1 == 1 ) /* x is a row vector */
  {
    dimvectx = n1 ;
  }
  else if ( n1 == 1 ) /* x is a column vector */
  {
    dimvectx = m1 ;
  }
  else /* x is a matrix */
  {
    dimvectx = -1 ;
  }

  if ( dimvectx > 1 )
  {
    int monotony = checkMonotony( pvecx, dimvectx ) ;
    if ( monotony == 0 )
    {
      Scierror(999, _("%s: Wrong value: Vector is not monotonous.\n"),"Objplot3d");
      return SET_PROPERTY_ERROR;
    }

    psurf->flag_x = monotony ;
  }

  if ( psurf->isfac == 1 )
  {
    /* x is considered as a matrix */
    dimvecty = -1 ;
  }
  else if(m2 == 1) /* y is a row vector */
  {
    dimvecty = n2 ;
  }
  else if(n2 == 1) /* y is a column vector */
  {
    dimvecty = m2 ;
  }
  else /* y is a matrix */
  {
    dimvecty = -1 ;
  }
  if( dimvecty > 1 )
  {
    int monotony = checkMonotony( pvecy, dimvecty ) ;
    if ( monotony == 0 )
    {
      Scierror(999, _("%s: Wrong value: Vector is not monotonous.\n"),"Objplot3d");
      return SET_PROPERTY_ERROR;
    }

    psurf->flag_y = monotony ;
  }

  /* Update of the dimzx, dimzy depends on  m3, n3: */
  psurf->dimzx = m3;
  psurf->dimzy = n3;


  /* Free the old values... */
  FREE(psurf->pvecx); psurf->pvecx = NULL;
  FREE(psurf->pvecy); psurf->pvecy = NULL;
  FREE(psurf->pvecz); psurf->pvecz = NULL;
  /* ...even on zcol wich must have been initialized like others or set to NULL in case there was no color before
  The FREE macro tests the NULL pointer existence... */
  FREE(psurf->zcol); psurf->zcol = NULL;
  /* If we had a previous color matrix/vector and we do not specify a new one, I consider we are losing it.*/
  /* That's why we make a FREE as follows:*/
  FREE(psurf->inputCMoV);psurf->inputCMoV = NULL; /* F.Leray 23.03.04*/

  /* copy the values now */
  rewindAssignedList( tlist ) ;
  pvecx = createCopyDoubleMatrixFromList( tlist, &m1, &n1 ) ;
  pvecy = createCopyDoubleMatrixFromList( tlist, &m2, &n2 ) ;
  pvecz = createCopyDoubleMatrixFromList( tlist, &m3, &n3 ) ;


  if( getAssignedListNbElement( tlist ) == 4 ) /* F.Leray There is a color matrix */
  {
    int j ;
    int i ;

    psurf->inputCMoV = createCopyDoubleMatrixFromList( tlist, &m3n, &n3n ) ;


    if( psurf->flagcolor == 2 || psurf->flagcolor == 4 )
    { /* case of SCI_PLOT3D avoid */
      nc = psurf->dimzy ;
    }
    else if( psurf->flagcolor == 3 )
    {
      nc = psurf->dimzx * psurf->dimzy ;
    }
    else
    {
      nc = 0 ;
    }

    if ( nc > 0 )
    {
      if ((psurf->zcol = MALLOC (nc * sizeof (double))) == NULL)
      {
        FREE(pvecx); pvecx = (double *) NULL;
        FREE(pvecy); pvecy = (double *) NULL;
        FREE(pvecz); pvecz = (double *) NULL;
        return SET_PROPERTY_ERROR ;
      }
    }

    /* case flagcolor == 2*/
    if( psurf->flagcolor == 2 && ( m3n == 1 || n3n == 1) ) /* it means we have a vector in Color input: 1 color per facet in input*/
    {
      doubleArrayCopy( psurf->zcol, psurf->inputCMoV, nc ) ;
      /* We have just enough information to fill the psurf->zcol array*/
      /* nc value is dimzx*dimzy == m3 * n3 */
    }
    else if( psurf->flagcolor == 2 ) /* it means we have a matrix in Color input: 1 color per vertex in input*/
    {
      /* We have too much information and we take only the first dimzy colors to fill the psurf->zcol array*/
      /* NO !! Let's do better; F.Leray 08.05.04 : */
      /* We compute the average value (sum of the value of the nf=m3n vertices on a facet) / (nb of vertices per facet which is nf=m3n) */
      /* in our example: m3n=4 and n3n=400 */
      for ( j = 0 ; j < nc ; j++)   /* nc value is dimzy*/
      {
        double tmp = 0.0 ;
        for(ii=0;ii<m3n;ii++)
        {
          tmp = tmp +  psurf->inputCMoV[j * m3n + ii] ;
        }
        tmp = tmp / m3n ;
        psurf->zcol[j] = tmp;
      }
    }
    /* case flagcolor == 3*/
    else if( psurf->flagcolor==3 && ( m3n==1 || n3n ==1) ) /* it means we have a vector in Color input: 1 color per facet in input*/
    {
      /* We have insufficient info. to fill the entire zcol array of dimension nc = dimzx*dimzy*/
      /* We repeat the data:*/
      for(i = 0; i< psurf->dimzy; i++)
      {
        for (j = 0;j < psurf->dimzx; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
        {
          psurf->zcol[psurf->dimzx*i+j] = psurf->inputCMoV[i];  /* DJ.A 2003 */
        }
      }
    }
    else if( psurf->flagcolor==3 ) /* it means we have a matrix in Color input: 1 color per vertex in input*/
    {
      /* We have just enough information to fill the psurf->zcol array*/
      /* nc value is dimzy*/
      doubleArrayCopy( psurf->zcol, psurf->inputCMoV, nc ) ;
    }  
    /* case flagcolor == 4*/
    else if(psurf->flagcolor==4 && ( m3n==1 || n3n ==1)) /* it means we have a vector in Color input: 1 color per facet in input*/
    {
      /* We have insufficient info. to fill the entire zcol array of dimension nc = dimzx*dimzy*/
      /* We repeat the data:*/
      /* nc value is dimzx*dimzy == m3n * n3n */
      doubleArrayCopy( psurf->zcol, psurf->inputCMoV, nc ) ;
    }
    else if ( psurf->flagcolor == 4 ) /* it means we have a matrix in Color input: 1 color per vertex in input*/
    {
      /* input : color matrix , we use 1 color per facet with Matlab selection mode (no average computed) */
      /* HERE is the difference with case 2 */
      for ( j = 0 ; j < nc ; j++ )   /* nc value is dimzy*/
      {
        psurf->zcol[j] = psurf->inputCMoV[j*m3n] ;
      }
    }

  }
  else /* else we put the value of the color_mode flag[0]*/
  {
    if(psurf->flagcolor == 2 || psurf->flagcolor == 4)
    { /* case of SCI_PLOT3D avoid */
      nc = psurf->dimzy;
    }
    else if(psurf->flagcolor == 3)
    {
      nc = psurf->dimzx *  psurf->dimzy;
    } 
    else
    {
      nc=0;
    }

    if ( nc > 0)
    {
      if ((psurf->zcol = MALLOC (nc * sizeof (double))) == NULL)
      {
        FREE(pvecx); pvecx = (double *) NULL;
        FREE(pvecy); pvecy = (double *) NULL;
        FREE(pvecz); pvecz = (double *) NULL;
        return -1;
      }
    }

    /* case flagcolor == 2*/
    if(psurf->flagcolor==2 || psurf->flagcolor==4) /* we have to fill a Color vector */
    {
      int j ;
      for (j = 0;j < nc; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
      {
        psurf->zcol[j] = psurf->flag[0];
      }
    }
    else if(psurf->flagcolor==3) /* we have to fill a color matrix */
    {
      int i ;
      for ( i = 0 ; i < psurf->dimzx * psurf->dimzy ; i++ )
      {
        psurf->zcol[i] = psurf->flag[0];
      }
    }
    else
    {
      /* case flagcolor == 0 or 1 */
      psurf->zcol = NULL;
      psurf->izcol = 0;
    }
  }

  psurf->pvecx = pvecx;
  psurf->pvecy = pvecy;
  psurf->pvecz = pvecz;

  psurf->nc = nc; /*Adding F.Leray 23.03.04*/

  psurf->m1 = m1;
  psurf->m2 = m2;
  psurf->m3 = m3;
  psurf->n1 = n1;
  psurf->n2 = n2;
  psurf->n3 = n3;

  /* Update nx, ny a,d nz */
  psurf->nx = m1 * n1;
  psurf->ny = m2 * n2;
  psurf->nz = m3 * n3;
  psurf->m3n = m3n; /* If m3n and n3n are 0, then it means that no color matrix/vector was in input*/
  psurf->n3n = n3n;

  /* We need to rebuild ...->color matrix */
  if( psurf->flagcolor != 0 && psurf->flagcolor != 1 )
  { 
    if(psurf->cdatamapping == 0)
    {
      /* scaled */
      FREE(psurf->color);
      LinearScaling2Colormap(pobj);
    }
    else
    {
      FREE(psurf->color);
      psurf->color = NULL ;

      if( nc > 0 )
      {
        if ((psurf->color = MALLOC (nc * sizeof (double))) == NULL)
        {
          return -1;
        }
        doubleArrayCopy( psurf->color, psurf->zcol, nc ) ;
      }
      /* copy zcol that has just been freed and re-alloc + filled in */
    }
  }
  return SET_PROPERTY_SUCCEED ;
}
/*--------------------------------------------------------------------------*/
int CheckAndUpdate_x_shift(sciPointObj * pobj, int numrow)
{
  sciPolyline * ppolyline = pPOLYLINE_FEATURE(pobj) ;
  double * new_bar = NULL ;

  if( ppolyline->x_shift == NULL )
  {
    return SET_PROPERTY_ERROR ;
  }

  if( ppolyline->n1 == numrow )
  {
    return SET_PROPERTY_UNCHANGED ;
  }

  new_bar = createNewArrayFromSource( numrow, ppolyline->x_shift, ppolyline->n1 ) ;

  if ( new_bar == NULL )
  {
    Scierror(999, _("%s: No more memory.\n"),"set_data_property") ;
    return SET_PROPERTY_ERROR ;
  }

  FREE( ppolyline->x_shift ) ;
  ppolyline->x_shift = new_bar ;

  return SET_PROPERTY_SUCCEED ;
}
/*--------------------------------------------------------------------------*/
int CheckAndUpdate_y_shift(sciPointObj * pobj, int numrow)
{
  sciPolyline * ppolyline = pPOLYLINE_FEATURE(pobj) ;
  double * new_bar = NULL ;

  if( ppolyline->y_shift == NULL )
  {
    return SET_PROPERTY_ERROR ;
  }

  if( ppolyline->n1 == numrow )
  {
    return SET_PROPERTY_UNCHANGED ;
  }

  new_bar = createNewArrayFromSource( numrow, ppolyline->y_shift, ppolyline->n1 ) ;

  if ( new_bar == NULL )
  {
    Scierror(999, _("%s: No more memory.\n"),"CheckAndUpdate_y_shift") ;
    return SET_PROPERTY_ERROR ;
  }

  FREE( ppolyline->y_shift ) ;
  ppolyline->y_shift = new_bar ;

  return SET_PROPERTY_SUCCEED ;
}
/*--------------------------------------------------------------------------*/
int CheckAndUpdate_z_shift(sciPointObj * pobj, int numrow)
{
  sciPolyline * ppolyline = pPOLYLINE_FEATURE(pobj) ;
  double * new_bar = NULL ;

  if( ppolyline->z_shift == NULL )
  {
    return SET_PROPERTY_ERROR ;
  }

  if( ppolyline->n1 == numrow )
  {
    return SET_PROPERTY_UNCHANGED ;
  }

  new_bar = createNewArrayFromSource( numrow, ppolyline->z_shift, ppolyline->n1 ) ;

  if ( new_bar == NULL )
  {
    Scierror(999, _("%s: No more memory.\n"),"CheckAndUpdate_z_shift") ;
    return SET_PROPERTY_ERROR ;
  }

  FREE( ppolyline->z_shift ) ;
  ppolyline->z_shift = new_bar ;

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
int set_data_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  if( sciGetEntityType(pobj) == SCI_SEGS && pSEGS_FEATURE(pobj)->ptype == 1 )
  {
    AssignedList * tlist = NULL ;
    int status = -1 ;

    if( !isParameterTlist( valueType ) )
    {
      Scierror(999, "Incorrect argument, must be a Tlist!\n") ;
      return SET_PROPERTY_ERROR ;
    }
  
    /* we should have 4 properties in the tlist */
    tlist = createAssignedList( 3, 4 ) ;
    if ( tlist == NULL )
    {
      return SET_PROPERTY_ERROR ;
    }

    status = setchampdata( pobj, tlist ) ;
    destroyAssignedList( tlist ) ;
    return status ;
  }
  else if((sciGetEntityType(pobj) == SCI_GRAYPLOT) && (pGRAYPLOT_FEATURE(pobj)->type == 0)) /* case 0: real grayplot */
  {
    AssignedList * tlist = NULL ;
    int status = -1 ;

    if( !isParameterTlist( valueType ) )
    {
      Scierror(999, _("Wrong type for input argument: Tlist expected.\n"));
      return SET_PROPERTY_ERROR ;
    }

    /* we should have 3 properties in the tlist */
    tlist = createAssignedList( 3, 3 ) ;
    if ( tlist == NULL )
    {
      return SET_PROPERTY_ERROR ;
    }

    status = setgrayplotdata( pobj, tlist ) ;
    destroyAssignedList( tlist ) ;
    return status ;
  }
  else if(sciGetEntityType(pobj) == SCI_SURFACE)
  {
    AssignedList * tlist = NULL ;
    int status = -1 ;
    int listSize = 0 ;

    if( !isParameterTlist( valueType ) )
    {
      Scierror(999, _("Wrong type for input argument: Tlist expected.\n"));
      return SET_PROPERTY_ERROR ;
    }

    listSize = getStackListNbElement( 3 ) ;

    if ( listSize == 3 )
    {
      tlist = createAssignedList( 3, 3 ) ;
    }
    else if ( listSize == 4 )
    {
      tlist = createAssignedList( 3, 4 ) ;
    }
    else
    {
      Scierror(999, _("Wrong size for input argument: %d or %d expected.\n"),4,5);
      return SET_PROPERTY_ERROR ;
    }

    if ( tlist == NULL )
    {
      return SET_PROPERTY_ERROR ;
    }

    status = set3ddata( pobj, tlist ) ;
    destroyAssignedList( tlist ) ;
    return status ;

  }
  else  /* F.Leray 02.05.05 : "data" case for others (using sciGetPoint routine inside GetProperty.c) */
  {
    if ( !isParameterDoubleMatrix( valueType ) )
    {
      Scierror(999, _("Incompatible type for property %s.\n"),"data") ;
      return SET_PROPERTY_ERROR ;
    }

    if ( sciGetEntityType(pobj) == SCI_POLYLINE )
    {
      CheckAndUpdate_x_shift( pobj, nbRow ) ; /* used only on Polyline */
      CheckAndUpdate_y_shift( pobj, nbRow ) ; /* used only on Polyline */
      CheckAndUpdate_z_shift( pobj, nbRow ) ; /* used only on Polyline */
    }

    return sciSetPoint( pobj, getDoubleMatrixFromStack( stackPointer ), &nbRow, &nbCol );
  }
  return SET_PROPERTY_ERROR ;
  
}
/*------------------------------------------------------------------------*/
