/*------------------------------------------------------------------------*/
/* file: GraphicZoom.c                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions to Zoom in the graphic window                */
/*------------------------------------------------------------------------*/

#include "GraphicZoom.h"

/**
 * To know if the zoom is activated
 */
static BOOL GlobalFlag_Zoom3dOn = FALSE ;
/*-------------------------------------------------------------------------------------*/
BOOL isZoom3dOn( void )
{
  return GlobalFlag_Zoom3dOn ;
}
/*-------------------------------------------------------------------------------------*/
void setZoom3dStatus( BOOL status )
{
  GlobalFlag_Zoom3dOn = status ;
}
/*-------------------------------------------------------------------------------------*/
