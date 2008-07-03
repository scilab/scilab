/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: ColorMapManagement.c                                             */
/* desc : Several algorithm to deal with colormaps                        */
/*------------------------------------------------------------------------*/

#include "GetProperty.h"
#include "sciprint.h"
#include "getHandleProperty/SetPropertyStatus.h"
#include "MALLOC.h"
#include "localization.h"
/*------------------------------------------------------------------------*/
int LinearScaling2Colormap( sciPointObj * pobj );
void convertColorMap2BW( double * bwColorMap, double * colorMap, int colorMapSize );
/*------------------------------------------------------------------------*/
int LinearScaling2Colormap( sciPointObj * pobj )
{
  int i;
  int nbcol =  sciGetNumColors (pobj);	/* the number of the colors inside the current colormap */
  sciSurface * psurf = pSURFACE_FEATURE (pobj);

  double min,max;

  double indexmin = 1.;
  double indexmax = (double) nbcol;

  int nc = psurf->nc; /* the number of colors contained inside zcol matrix */


  if(psurf->zcol == NULL){
    sciprint(_("Color matrix is NULL: Can not build color scaled linearly into the current colormap"));
    return SET_PROPERTY_ERROR ;
  }

  if (((psurf->color = MALLOC (nc * sizeof (double))) == NULL)){
	  sciprint(_("%s: No more memory.\n"),"LinearScaling2Colormap");
	  return SET_PROPERTY_ERROR ;
  }

  /* get the min inside zcol */
  min = psurf->zcol[0];
  for(i=0;i<nc;i++)
    if(min > psurf->zcol[i]) min = psurf->zcol[i];

  /* get the max inside zcol */
  max = psurf->zcol[0];
  for(i=0;i<nc;i++)
    if(max < psurf->zcol[i]) max = psurf->zcol[i];

  if(min != max)
  {
    /* linear interpolation */
    double A = (indexmin-indexmax)/(min-max); 
    double B = (min*indexmax-indexmin*max)/(min-max);
    for(i=0;i<nc;i++)
      psurf->color[i] = A*psurf->zcol[i] + B + 0.1;
  }
  else
  {
    double C = (indexmin+indexmax)/2;
    for(i=0;i<nc;i++)
      psurf->color[i] = C;
  }

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
void convertColorMap2BW( double * bwColorMap, double * colorMap, int colorMapSize )
{
  int i ;
  for ( i = 0 ; i < colorMapSize ; i++ )
  {
    /* use parameter 0.299 for red, 0.587 for green and 0.114 for blue */
    double curColor = Max( Min(  0.299 * colorMap[i]
                               + 0.587 * colorMap[i + colorMapSize]
                               + 0.114 * colorMap[i + 2 * colorMapSize] , 1.0 ), 0.0 ) ;
    bwColorMap[i                   ] = curColor ;
    bwColorMap[i + colorMapSize    ] = curColor ;
    bwColorMap[i + 2 * colorMapSize] = curColor ;
  }
}
/*------------------------------------------------------------------------*/
