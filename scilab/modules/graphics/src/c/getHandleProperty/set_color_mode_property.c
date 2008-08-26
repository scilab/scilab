/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: set_color_mode_property.c                                        */
/* desc : function to modify in Scilab the color_mode field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "ColorMapManagement.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
int set_color_mode_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int flagcolor;
  sciSurface * psurf = NULL ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"color_mode") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SURFACE )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"color_mode") ;
    return SET_PROPERTY_ERROR ;
  }

  psurf =  pSURFACE_FEATURE (pobj) ;

  flagcolor = psurf->flagcolor;

  psurf->flag[0]= (integer) getDoubleFromStack( stackPointer ) ;

  if( flagcolor != 0 && flagcolor != 1 )
  {
    if( psurf->m3n * psurf->n3n == 0 )
    {
      /* There is not any color matrix/vect. in input : update the fake color one */
      int j,nc;

      if( flagcolor == 2 || flagcolor == 4 )
      {
        nc = psurf->dimzy; /* remind: dimzy always equal n3*/
      }
      else
      {
        nc = psurf->dimzx * psurf->dimzy;
      }

      FREE(psurf->zcol);


      if ((psurf->zcol = MALLOC ( nc * sizeof (double))) == NULL)
      {
        sciprint(_("%s: No more memory.\n"),"set_color_mode_property");
        return SET_PROPERTY_ERROR ;
      }

      for (j = 0;j < nc; j++)
      {
        /* nc value is dimzy*/
        psurf->zcol[j]= psurf->flag[0];
      }


      if ( flagcolor != 0 && flagcolor != 1 )
      { 
        /* We need to rebuild ...->color matrix */
        if ( psurf->cdatamapping == 0 )
        {
          /* scaled */
          FREE(psurf->color);
          LinearScaling2Colormap(pobj);
        }
        else
        {
          int nc_ = psurf->nc;

          FREE(psurf->color);
          psurf->color = NULL ;
          

          if ( nc_ > 0 )
          {
            if ((psurf->color = MALLOC (nc_ * sizeof (double))) == NULL)
            {
				sciprint(_("%s: No more memory\n"),"set_color_mode_property");
				return SET_PROPERTY_ERROR ;
            }
            doubleArrayCopy( psurf->color, psurf->zcol, nc_ ) ;
          }
          /* copy zcol that has just been freed and re-alloc + filled in */
        }
      }
    }
  }

  if ( psurf->typeof3d == SCI_FAC3D )
  {
    /* we have to deal with colors... and may be update because we just changed  psurf->flag[0]*/
    if( psurf->flagcolor == 0 )
    {
      pSURFACE_FEATURE (pobj)->izcol = 0;
    }
  }

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
