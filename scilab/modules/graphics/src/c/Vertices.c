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
/* file: Vertices.c                                                       */
/* desc : Contains functions to store vertices for the zoom               */
/*------------------------------------------------------------------------*/

#include "Vertices.h"
#include "MALLOC.h"
#include "GetProperty.h"
#include "GraphicZoom.h"
#include "sciprint.h"
#include "localization.h"

static int vertex_index = 0 ;
static Vertices * pHead  = NULL ;
static Vertices * pHead2 = NULL ;

/*-----------------------------------------------------------------------------------------*/
int  getVertexIndex( void )
{
  return vertex_index ;
}
/*-----------------------------------------------------------------------------------------*/
void setVertexIndex( int ind )
{
  vertex_index = ind ;
}
/*-----------------------------------------------------------------------------------------*/
int SetMinMaxVertices(Vertices *vertices_list, double *xmin, double *ymin, double *zmin,double *xmax, double *ymax, double *zmax)
{ 
  Vertices * pCurrent = vertices_list;

  *xmin = *xmax = vertices_list->value_x;
  *ymin = *ymax = vertices_list->value_y;
  *zmin = *zmax = vertices_list->value_z;

  while (pCurrent != NULL) {
    if(pCurrent->value_x >  *xmin) *xmin = pCurrent->value_x;
    if(pCurrent->value_y >  *ymin) *ymin = pCurrent->value_y;
    if(pCurrent->value_z >  *zmin) *zmin = pCurrent->value_z;

    if(pCurrent->value_x <  *xmax) *xmax = pCurrent->value_x;
    if(pCurrent->value_y <  *ymax) *ymax = pCurrent->value_y;
    if(pCurrent->value_z <  *zmax) *zmax = pCurrent->value_z;

    pCurrent = pCurrent->pNext;
  }

  return 0;
}
/*-----------------------------------------------------------------------------------------*/
int GetVerticesAt(Vertices *vertices_list, int *xm, int *ym, double *x, double *y, double *z)
{
  Vertices * pCurrent = vertices_list;

  if(pHead2 == (Vertices *) NULL)
    pHead2 = pCurrent;

  *xm = pHead2->value_xm;
  *ym = pHead2->value_ym;
  *x  = pHead2->value_x;
  *y  = pHead2->value_y;
  *z  = pHead2->value_z;

  pHead2 = pHead2->pNext;

  return 0;
}
/*-----------------------------------------------------------------------------------------*/
int RemoveNext(Vertices *pCurrent)
{
  Vertices * DeletedElement = pCurrent->pNext;
  Vertices * pNextNext =  pCurrent->pNext->pNext;

  FREE(DeletedElement); DeletedElement = (Vertices *) NULL;

  pCurrent->pNext = pNextNext;

  return 0;
}
/*-----------------------------------------------------------------------------------------*/
int FreeVertices(sciPointObj * psubwin)
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  Vertices * pCurrent = ppsubwin->vertices_list;

  if (pCurrent != NULL) {

    /*pCurrent = Tete; */
    while (pCurrent->pNext != NULL){
      RemoveNext(pCurrent);
    }
    FREE(ppsubwin->vertices_list); ppsubwin->vertices_list = (Vertices *) NULL;
  }

  pHead = (Vertices *) NULL;
  pHead2 = (Vertices *) NULL;

  return 0;
}
/*-----------------------------------------------------------------------------------------*/
/* Vertices * Tete = NULL; */

int Store3DPixelValues(sciPointObj * pobj, int xm, int ym, double x, double y, double z)
{

  sciPointObj * psubwin = sciGetParentSubwin(pobj);
  sciSubWindow *ppsubwin = pSUBWIN_FEATURE(psubwin);

  if( isZoom3dOn() )
  {
    Vertices * pCurrent = ppsubwin->vertices_list;

    if (pCurrent != NULL) {

      pCurrent = pHead;

      if(( pCurrent->pNext = (Vertices*) MALLOC(sizeof(Vertices))) == NULL){
        sciprint(_("%s: No more memory.\n"),"Store3DPixelValues");
        return -1;
      }

      pCurrent->pNext->pNext = NULL;

      pCurrent->pNext->value_xm = xm;
      pCurrent->pNext->value_ym = ym;
      pCurrent->pNext->value_x = x;
      pCurrent->pNext->value_y = y;
      pCurrent->pNext->value_z = z;

      pHead = pCurrent->pNext;

    }
    else
    { /* first element is created */
      if((ppsubwin->vertices_list = (Vertices*) MALLOC(sizeof(Vertices))) == NULL){
        sciprint(_("%s: No more memory.\n"),"Store3DPixelValues");
        return -1;
      }

      ppsubwin->vertices_list->value_xm = xm;
      ppsubwin->vertices_list->value_ym = ym;
      ppsubwin->vertices_list->value_x = x;
      ppsubwin->vertices_list->value_y = y;
      ppsubwin->vertices_list->value_z = z;

      ppsubwin->vertices_list->pNext = NULL;

      pHead = ppsubwin->vertices_list;

    }
    vertex_index++;
  }

  return 0;
}
/*-------------------------------------------------------------------------------------*/
