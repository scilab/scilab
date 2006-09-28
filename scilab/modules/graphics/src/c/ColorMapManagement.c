/*------------------------------------------------------------------------*/
/* file: ColorMapManagement.c                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : Several algorithm to deal with colormaps                        */
/*------------------------------------------------------------------------*/

#include "GetProperty.h"
#include "sciprint.h"
#include "getHandleProperty/SetPropertyStatus.h"
#include "MALLOC.h"

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
    sciprint("Color matrix is NULL ; can not build color scaled linearly into the current colormap");
    return SET_PROPERTY_ERROR ;
  }

  if (((psurf->color = MALLOC (nc * sizeof (double))) == NULL)){
    sciprint("Allocation failed for color in LinearScaling2Colormap");
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
