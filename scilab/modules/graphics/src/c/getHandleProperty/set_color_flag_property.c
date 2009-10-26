/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_color_flag_property.c                                        */
/* desc : function to modify in Scilab the color_flag field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "ColorMapManagement.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
int set_color_flag_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int flagcolor = (int) getDoubleFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real expected.\n"), "color_flag");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SURFACE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"color_flag") ;
    return SET_PROPERTY_ERROR ;
  }
  if ( nbRow * nbCol != 1 )
  {
    Scierror(999, _("Wrong size for '%s' property: Scalar expected.\n"), "color_flag");
    return SET_PROPERTY_ERROR ;
  }

  if ( pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D )
  {
    if ( flagcolor < 0 || flagcolor > 1 )
    {
      Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "color_flag", "0", "1");
      return SET_PROPERTY_ERROR ;
    }
    pSURFACE_FEATURE(pobj)->flagcolor = flagcolor ;
    return SET_PROPERTY_SUCCEED ;
  }
  else if ( pSURFACE_FEATURE (pobj)->typeof3d == SCI_FAC3D ) 
  {

    sciSurface * psurf = pSURFACE_FEATURE (pobj);

    if ( flagcolor < 0 || flagcolor > 4 )
    {
      Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "color_flag", "0, 1, 2, 3, 4");
      return SET_PROPERTY_ERROR ;
    }


    /* F.Leray 18.03.04 Something goes wrong here: No need to re-build/re-alloc zcol normally!!*/
    /* YES in fact but use of another fixed array named inputCMoV (inputColorMatrixorVector)*/

    if ( pSURFACE_FEATURE(pobj)->flagcolor == flagcolor )
    {
      return SET_PROPERTY_UNCHANGED ;
    }

    if( flagcolor == 0 || flagcolor == 1 )
    {
      pSURFACE_FEATURE(pobj)->izcol = 0;
    }
    else if( flagcolor == 2 ) /* Warning: here we need a color vector (one color per facet). N = 1 x dimzy  */
    {
      int m3n = pSURFACE_FEATURE (pobj)->m3n;
      int n3n = pSURFACE_FEATURE (pobj)->n3n;
      int nc = psurf->dimzy; /* rappel: dimzy always equal n3*/

      psurf->nc = nc;
      FREE(psurf->zcol); psurf->zcol = NULL;

      if( nc > 0 )
      {
        if ((psurf->zcol = MALLOC (nc * sizeof (double))) == NULL)
        {
          Scierror(999, _("%s: No more memory.\n"),"set_color_flag_property") ;
          return SET_PROPERTY_ERROR ;
        }
      }

      if( m3n * n3n != 0 )
      { /* There is either a color matrix or vector*/
        if( m3n==1 || n3n ==1)
        {
          /* nc value is dimzx*dimzy == m3n * n3n */
          doubleArrayCopy( psurf->zcol, psurf->inputCMoV, nc ) ;
          /* We have just enough information to fill the psurf->zcol array*/
        }
        else
        {
          /* We have too much information and we take only the first dimzy colors to fill the psurf->zcol array*/
          /* NO !! Let's do better; F.Leray 08.05.04 : */
          /* We compute the average value (sum of the value of the nf=m3n vertices on a facet) / (nb of vertices per facet which is nf=m3n) */
          /* in our example: m3n=4 and n3n=400 */
          int i ;
          int j ;
          for ( j = 0 ; j < nc ; j++ )   /* nc value is dimzy*/
          {
            double tmp = 0 ;
            for( i = 0 ; i < m3n ; i++)
            {
              tmp = tmp +  psurf->inputCMoV[j*m3n + i ];
            }
            tmp = tmp / m3n;
            psurf->zcol[j] = tmp;
          }
        }
      }
      else
      {
        int j ;
        /* There is no color matrix/vect. in input */
        for ( j = 0 ; j < psurf->dimzy ; j++ )   /* nc value is dimzy*/
        {
          psurf->zcol[j]= psurf->flag[0] ;
        }
      }

    }
    else if ( flagcolor == 3 ) /* interpolated (shading) case */
    {
      int m3n = pSURFACE_FEATURE (pobj)->m3n;
      int n3n = pSURFACE_FEATURE (pobj)->n3n;
      int nc = psurf->dimzx * psurf->dimzy;

      psurf->nc = nc;
      FREE(psurf->zcol); psurf->zcol = NULL;


      if ((psurf->zcol = MALLOC ( nc * sizeof (double))) == NULL)
      {
        Scierror(999, _("%s: No more memory.\n"),"set_color_flag_property") ;
        return SET_PROPERTY_ERROR ;
      }


      if( m3n * n3n != 0 ) /* There is either a color matrix or vector*/
      {
        if( m3n == 1 || n3n == 1 ) /* COLOR VECTOR */
        {
          int i ;
          int j ;
          /* We have insufficient info. to fill the entire zcol array of dimension nc = dimzx*dimzy*/
          /* We repeat the data:*/
          for(i = 0; i< psurf->dimzy; i++)
          {
            for (j = 0;j < psurf->dimzx; j++)
            {
              /* nc value is dimzx*dimzy == m3n * n3n */
              psurf->zcol[(psurf->dimzx)*i+j]= psurf->inputCMoV[i];
            }
          }               	
        }
        else /* COLOR MATRIX */
        {
          /* We have just enough information to fill the psurf->zcol array*/
          doubleArrayCopy( psurf->zcol, psurf->inputCMoV, (psurf->dimzx) * (psurf->dimzy) ) ;
        }
      }
      else
      {
        int i ;
        int j ;
        /* There is no color matrix/vect. in input */
        for(i = 0; i<  psurf->dimzy; i++)
        {
          for (j = 0;j <  psurf->dimzx; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
          {
            psurf->zcol[( psurf->dimzx)*i+j]= psurf->flag[0];
          }
        }
      }
    }
    /* New : case == 4 introduced on the 10.03.05 (F.Leray) to support faceted (flat) Matlab case */
    else if ( flagcolor == 4 ) /* Warning: here we need a color vector (one color per facet). N = 1 x dimzy  */
    {
      int m3n = pSURFACE_FEATURE (pobj)->m3n;
      int n3n = pSURFACE_FEATURE (pobj)->n3n;
      int nc = psurf->dimzy ; /* rappel: dimzy always equal n3*/

      psurf->nc = nc;
      FREE(psurf->zcol); psurf->zcol = NULL;

      if( nc > 0 )
      {
        if ((psurf->zcol = MALLOC (nc * sizeof (double))) == NULL)
        {
          Scierror(999, _("%s: No more memory.\n"),"set_color_flag_property") ;
          return SET_PROPERTY_ERROR ;
        }
      }

      if( m3n * n3n != 0 )
      {
        /* There is either a color matrix or vector */
        if ( m3n == 1 || n3n == 1 )
        {
          /* We have just enough information to fill the psurf->zcol array*/
          /* nc value is dimzx*dimzy == m3n * n3n */
          doubleArrayCopy(psurf->zcol, psurf->inputCMoV, nc ) ;
        }
        else
        {
          int j ;
          /* input : color matrix , we use 1 color per facet with Matlab selection mode (no average computed) */
          /* HERE is the difference with case 2 */
          for ( j = 0 ; j < nc ; j++ )
          {
            /* nc value is dimzy*/
            psurf->zcol[j] = psurf->inputCMoV[j*m3n];
          }
        }
      }
      else if (m3n * n3n == 0)
      {
        int j ;
        /* There is no color matrix/vect. in input */
        for ( j = 0 ; j < psurf->dimzy ; j++ )
        {
          /* nc value is dimzy*/
          psurf->zcol[j]= psurf->flag[0];
        }
      }
    }

    if( flagcolor != 0 && flagcolor != 1 )
    { 
      /* We need to rebuild ...->color matrix */
      if( psurf->cdatamapping == 0 )
      { /* scaled */
        FREE(psurf->color);
        psurf->color = NULL ;
        LinearScaling2Colormap(pobj);
      }
      else
      {
        int nc = psurf->nc;

        FREE(psurf->color);
        psurf->color = NULL ;

        if( nc > 0 )
        {
          if ((psurf->color = MALLOC (nc * sizeof (double))) == NULL)
          {
            Scierror(999, _("%s: No more memory.\n"),"set_color_flag_property") ;
            return SET_PROPERTY_ERROR;
          }
          doubleArrayCopy( psurf->color, psurf->zcol, nc ) ;
        }
        /* copy zcol that has just been freed and re-alloc + filled in */
      }
    }

    /* Finally, update the flagcolor */
    pSURFACE_FEATURE (pobj)->flagcolor = flagcolor;
  }

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
