/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
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
/* file: axesScale.c                                                      */
/* desc : Contains functions to compute scale changes in a specific       */
/*        subwindow.                                                      */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "axesScale.h"
#include "math_graphics.h"
#include "MALLOC.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "sciprint.h"
#include "PloEch.h"
#include "GraphicZoom.h"
#include "Vertices.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "SetJavaProperty.h"
#include "GraphicSynchronizerInterface.h"
#include "DrawingBridge.h"
#include "CurrentObjectsManagement.h"
#include "Interaction.h"
#include "DoublyLinkedList.h"

/*------------------------------------------------------------------------------*/
void zoomSubwin(sciPointObj * pSubwin, int posX, int posY, int width, int height);
void zoomFigure(sciPointObj * pFigure, int posX, int posY, int width, int height);
/*------------------------------------------------------------------------------*/
double InvAxis( double min, double max, double u )
{
  /*return (u-min) / (max-min) * min + (u-max) / (min-max) * max ;*/
  return min + max - u ; /* ahah little simplification */
}
/*------------------------------------------------------------------------------*/
int ReverseDataFor3DXonly( sciPointObj * psubwin, double xvect[], int n1 )
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin) ;
  int cmp;

  if( ppsubwin->axes.reverse[0] )
  {
    /* agir sur x */
    if( ppsubwin->logflags[0] == 'l' )
    {
      for( cmp = 0 ; cmp < n1 ; cmp++ )
      {
        xvect[cmp] = log10( xvect[cmp] ) ;
        xvect[cmp] = InvAxis( ppsubwin->FRect[0], ppsubwin->FRect[2], xvect[cmp] ) ;
        xvect[cmp] = exp10( xvect[cmp] ) ;
      }
    }
    else
    {
      for( cmp = 0 ; cmp < n1 ; cmp++ )
      {
        xvect[cmp] = InvAxis( ppsubwin->FRect[0], ppsubwin->FRect[2], xvect[cmp] ) ;
      }
    }
  }

  return 0 ;
}
/*------------------------------------------------------------------------------*/
int ReverseDataFor3DYonly( sciPointObj * psubwin, double yvect[], int n1 )
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  int cmp;

  if( ppsubwin->axes.reverse[1] )
  {
    /* agir sur y */
    if(ppsubwin->logflags[1]=='l'){
      for(cmp=0;cmp<n1;cmp++)
      {
        yvect[cmp] = log10(yvect[cmp]);
        yvect[cmp] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],yvect[cmp]);
        yvect[cmp] = exp10(yvect[cmp]);
      }
    }
    else
    {
      for(cmp=0;cmp<n1;cmp++)
      {
        yvect[cmp] =  InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],yvect[cmp]);
      }
    }
  }

  return 0;
}
/*------------------------------------------------------------------------------*/
int ReverseDataFor3DZonly( sciPointObj * psubwin, double zvect[], int n1 )
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);
  int cmp;

  if( zvect == NULL ) { return 0 ; }

  if( ppsubwin->axes.reverse[2] )
  {
    /* agir sur z */
    if(ppsubwin->logflags[2]=='l')
    {
      for(cmp=0;cmp<n1;cmp++)
      {
        zvect[cmp] = log10(zvect[cmp]);
        zvect[cmp] = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],zvect[cmp]);
        zvect[cmp] = exp10(zvect[cmp]);
      }
    }
    else
    {
      for(cmp=0;cmp<n1;cmp++)
      {
        zvect[cmp] =  InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],zvect[cmp]);
      }
    }
  }

  return 0;
}
/*------------------------------------------------------------------------------*/
int ReverseDataFor3D( sciPointObj * psubwin, double xvect[], double yvect[], double zvect[], int n1 )
{

  ReverseDataFor3DXonly(psubwin, xvect, n1);
  ReverseDataFor3DYonly(psubwin, yvect, n1);
  ReverseDataFor3DZonly(psubwin, zvect, n1);

  return 0;
}
/*------------------------------------------------------------------------------*/
int trans3d( sciPointObj * pobj,
             integer       n   ,
             integer       xm[],
             integer       ym[],
             double        x[] ,
             double        y[] ,
             double        z[]  )
{
  integer i;
  double tmpx,tmpy,tmpz;
  double  tmpx1,tmpy1,tmpz1;
  /* TEST F.Leray 20.04.04: I fix HERE temporarily BOOL cube_scaling = FALSE; */
  BOOL cube_scaling;
  /* Test to enable reverse axis in 3D */ /* F.Leray 14.10.04 */
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(pobj);

  double *xtmp = NULL;
  double *ytmp = NULL;
  double xz1 ;
  double yz1 ;

  if (sciGetEntityType(pobj) != SCI_SUBWIN) { return 1 ; }

  xtmp = MALLOC(n*sizeof(double));
  ytmp = MALLOC(n*sizeof(double));

  cube_scaling = pSUBWIN_FEATURE(pobj)->cube_scaling;

  if( !cube_scaling)
  {
    if (z == (double *) NULL)
    {
      for ( i=0 ; i < n ; i++)
      {
        /* F.Leray 19.10.04 */
        /* Test to export logscale in 3D */

        if(ppsubwin->logflags[0] == 'l')
        {
          tmpx = log10(x[i]);
        }
        else
        {
          tmpx = x[i];
        }

        if(ppsubwin->logflags[1] == 'l')
        {
          tmpy = log10(y[i]);
        }
        else
        {
          tmpy = y[i];
        }

        if(ppsubwin->logflags[2] == 'l'){
          sciprint(_("Warning: Value on z data is negative or zero while logarithmic scale enabled\n"));
          sciprint(_("Object not drawn\n"));
          FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
          return 0;
        }

        xz1 = TX3D(tmpx,tmpy,0.0);
        yz1 = TY3D(tmpx,tmpy,0.0);
        xm[i] = inint( xz1 ) ;
        ym[i] = inint( yz1 ) ;

        if( isZoom3dOn() )
        {
          Store3DPixelValues(pobj,xm[i],ym[i],tmpx,tmpy,0.); /* stockage des xm, ym pour le zoom */
        }

        if ( finite(xz1)==0||finite(yz1)==0 ){
          FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
          return(0);
        }
      }
    }
    else /* z != NULL */
    {
      for ( i=0 ; i < n ; i++) /* cas 3d le + general */
      {

        /* F.Leray 19.10.04 */
        /* Test to export logscale in 3D */

        if(ppsubwin->logflags[0] == 'l')
          tmpx = log10(x[i]);
        else
          tmpx = x[i];

        if(ppsubwin->logflags[1] == 'l')
          tmpy = log10(y[i]);
        else
          tmpy = y[i];

        if(ppsubwin->logflags[2] == 'l')
          tmpz = log10(z[i]);
        else
          tmpz = z[i];

        
        xz1 = TX3D(tmpx,tmpy,tmpz);
        yz1 = TY3D(tmpx,tmpy,tmpz);
        xm[i] = inint( xz1 ) ;
        ym[i] = inint( yz1 ) ;

        if( isZoom3dOn() )
          Store3DPixelValues(pobj,xm[i],ym[i],tmpx,tmpy,tmpz); /* stockage des xm, ym pour le zoom */

        if ( finite(xz1)==0||finite(yz1)==0 ){
          FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
          return(0);
        }
        /* 	} */
      }
      FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
    }
  }
  else   /* cube_scaling == TRUE now */
  {
    if (z == (double *) NULL)
    {
      for ( i=0 ; i < n ; i++)
      {

        /* F.Leray 03.11.04 */
        /* Test to export logscale in 3D */

        if(ppsubwin->logflags[0] == 'l')
          tmpx = log10(x[i]);
        else
          tmpx = x[i];

        if(ppsubwin->logflags[1] == 'l')
          tmpy = log10(y[i]);
        else
          tmpy = y[i];

        if(ppsubwin->logflags[2] == 'l'){
          sciprint(_("Warning: Value on z data is negative or zero while logarithmic scale enabled\n"));
          sciprint(_("Object not drawn\n"));
          FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
          return 0;
        }

        tmpx1 = tmpx;
        tmpy1 = tmpy;
        tmpz1 = 0.;

        tmpx=(tmpx-pSUBWIN_FEATURE (pobj)->FRect[0])/(pSUBWIN_FEATURE (pobj)->FRect[2]-pSUBWIN_FEATURE (pobj)->FRect[0]);
        tmpy=(tmpy-pSUBWIN_FEATURE (pobj)->FRect[1])/(pSUBWIN_FEATURE (pobj)->FRect[3]-pSUBWIN_FEATURE (pobj)->FRect[1]);
        tmpz=(0.-pSUBWIN_FEATURE (pobj)->FRect[4])/(pSUBWIN_FEATURE (pobj)->FRect[5]-pSUBWIN_FEATURE (pobj)->FRect[4]); /* Adding F.Leray 04.11.04 */

        xz1 = TX3D(tmpx,tmpy,tmpz);
        yz1 = TY3D(tmpx,tmpy,tmpz);
        xm[i] = inint( xz1 ) ;
        ym[i] = inint( yz1 ) ;

        if( isZoom3dOn())
          Store3DPixelValues(pobj,xm[i],ym[i],tmpx1,tmpy1,tmpz1); /* stockage des xm, ym pour le zoom */

        if ( finite(xz1)==0||finite(yz1)==0 ){
          FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
          return(0);
        }
      }
    }
    else /* z != NULL */
    {
      for ( i=0 ; i < n ; i++)
      {

        /* F.Leray 03.11.04 */
        /* Test to export logscale in 3D */

        if(ppsubwin->logflags[0] == 'l')
          tmpx = log10(x[i]);
        else
          tmpx = x[i];

        if(ppsubwin->logflags[1] == 'l')
          tmpy = log10(y[i]);
        else
          tmpy = y[i];

        if(ppsubwin->logflags[2] == 'l')
          tmpz = log10(z[i]);
        else
          tmpz = z[i];

        tmpx1 = tmpx;
        tmpy1 = tmpy;
        tmpz1 = tmpz;

        tmpx= (tmpx-pSUBWIN_FEATURE (pobj)->FRect[0])/(pSUBWIN_FEATURE (pobj)->FRect[2]-pSUBWIN_FEATURE (pobj)->FRect[0]);
        tmpy= (tmpy-pSUBWIN_FEATURE (pobj)->FRect[1])/(pSUBWIN_FEATURE (pobj)->FRect[3]-pSUBWIN_FEATURE (pobj)->FRect[1]);
        tmpz= (tmpz-pSUBWIN_FEATURE (pobj)->FRect[4])/(pSUBWIN_FEATURE (pobj)->FRect[5]-pSUBWIN_FEATURE (pobj)->FRect[4]); /* Adding F.Leray 28.04.04 */

        xz1 = TX3D(tmpx,tmpy,tmpz);
        yz1 = TY3D(tmpx,tmpy,tmpz);
        xm[i] = inint( xz1 ) ;
        ym[i] = inint( yz1 ) ;

        if( isZoom3dOn() )
          Store3DPixelValues(pobj,xm[i],ym[i],tmpx1,tmpy1,tmpz1); /* stockage des xm, ym pour le zoom */

        if ( finite(xz1)==0||finite(yz1)==0 ){
          FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
          return(0);
        }
      }
    }
  }
  FREE(xtmp); xtmp = NULL; FREE(ytmp); ytmp = NULL;
  return(1);
}
/*------------------------------------------------------------------------------*/
/**
 * Specify new zoom box for a subwin object.
 * @param zoomRect vector [xMin, yMin, xMax, yMax]
 */
int sciZoom2D(sciPointObj * subwin, const double zoomRect[4])
{
  double zoomBox[6];

  // add Z scale to data bounds.
  sciGetDataBounds(subwin, zoomBox);
  zoomBox[0] = zoomRect[0];
  zoomBox[1] = zoomRect[1];
  zoomBox[2] = zoomRect[2];
  zoomBox[3] = zoomRect[3];

  return sciZoom3D(subwin, zoomBox);

}
/*------------------------------------------------------------------------------*/
/**
 * Specify a new zoom box for a subwin object
 * @param zoomBox vector [xMin, yMin, xMax, yMax, zMin, zMax].
 */
int sciZoom3D(sciPointObj * subwin, const double zoomBox[6])
{
  // convert zoomBox to [xMin, xMax, yMin, yMax, zMin, zMax]
  double zoomBox3D[6];
  zoomBox3D[0] = zoomBox[0];
  zoomBox3D[1] = zoomBox[2];
  zoomBox3D[2] = zoomBox[1];
  zoomBox3D[3] = zoomBox[3];
  zoomBox3D[4] = zoomBox[4];
  zoomBox3D[5] = zoomBox[5];

  if (!checkDataBounds(subwin, zoomBox3D[0], zoomBox3D[1], zoomBox3D[2],
                       zoomBox3D[3], zoomBox3D[4], zoomBox3D[5]))
  {
    return SET_PROPERTY_ERROR;
  }

  sciSetZoomBox(subwin, zoomBox3D);

  sciSetZooming(subwin, TRUE);

  return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------------*/
/**
 * get the zoom box to dispplay in Scilab for a sunwin object
 * @param[out] zoomBox [xMin, yMin, xMax, yMax, zMin, zMax];
 */
void sciGetZoom3D(sciPointObj * pObj, double zoomBox[6])
{
  double temp;

  // here we get [xMin, xMax, yMin, yMax, zMin, zMax]
  // we need to switch xMax and yMin
  sciGetZoomBox(pObj, zoomBox);
  temp = zoomBox[1];
  zoomBox[1] = zoomBox[2];
  zoomBox[2] = temp;
}
/*------------------------------------------------------------------------------*/
int sciZoomRect(sciPointObj * pObj, const double zoomRect[4])
{
  int status = SET_PROPERTY_ERROR;
  if (sciGetEntityType(pObj) == SCI_FIGURE)
  {
    status = sciFigureZoom2D(pObj, zoomRect);
  }
  else if (sciGetEntityType(pObj) == SCI_SUBWIN)
  {
    status = sciZoom2D(pObj, zoomRect);
  }

  /* redraw everything */
  if (status == SET_PROPERTY_SUCCEED)
  {
    sciDrawObj(pObj);
  }

  return status;
}
/*------------------------------------------------------------------------------*/
int sciDefaultZoom2D(const double zoomRect[4])
{
  sciPointObj * curFigure = NULL;
  startGraphicDataWriting();
  curFigure = sciGetCurrentFigure();
  endGraphicDataWriting();

  return sciZoomRect(curFigure, zoomRect);
}
/*------------------------------------------------------------------------------*/
int sciFigureZoom2D(sciPointObj * figure, const double zoomRect[4])
{
  /* try to zoom on all the subwindows */
  sciSons * pSons = sciGetSons(figure);
  while (pSons != NULL)
  {
    sciPointObj * curObj = pSons->pointobj;
    if (sciGetEntityType(curObj) == SCI_SUBWIN)
    {
      int status = sciZoom2D(curObj, zoomRect);
      if (status == SET_PROPERTY_SUCCEED)
      {
        forceRedraw(curObj);
      }
      else
      {
        return SET_PROPERTY_ERROR;
      }
    }
    pSons = pSons->pnext;
  }

  return SET_PROPERTY_SUCCEED;

}
/*------------------------------------------------------------------------------*/
/**
 * Try to zoom on a single subwindow using a selection area
 */
void zoomSubwin(sciPointObj * pSubwin, int posX, int posY, int width, int height)
{
  if (sciJavaZoomRect(pSubwin, posX, posY, width, height))
  {
    /* subwindow has been zoomed */
    /* force zooming */
    sciSetZooming(pSubwin, TRUE);

    // window has changed
    forceRedraw(pSubwin);
  }
}
/*------------------------------------------------------------------------------*/
/**
 * Zoom a figure using an already computed selection area
 */
void zoomFigure(sciPointObj * pFigure, int posX, int posY, int width, int height)
{
  /* try to zoom on all the subwindows */
  sciSons * pSons = sciGetSons(pFigure);
  while (pSons != NULL)
  {
    sciPointObj * curObj = pSons->pointobj;
    if (sciGetEntityType(curObj) == SCI_SUBWIN)
    {
      zoomSubwin(curObj, posX, posY, width, height);
    }
    pSons = pSons->pnext;
  }
}
/*------------------------------------------------------------------------------*/
/**
 * Perform an interactive zoom (rectangular selection +  zoom)
 * @param pObj object on which the zoom will be applied.
 *             Might be a Figure or a Subwindow. If it is a figure the zoom
 *             is applied to the axes children of the figure
 */
void sciInteractiveZoom(sciPointObj * pObj)
{
  int selectionRectangleCorners[4];
  int x;
  int y;
  int w;
  int h;
  int button;

  sciPointObj * parentFigure = sciGetParentFigure(pObj);
  char * currentInfoMessage = sciGetInfoMessage(parentFigure);
  char * curInfoMessageCopy = NULL;

  /* copy the info message to be able to reset it after zooming */
  curInfoMessageCopy = MALLOC((strlen(currentInfoMessage) + 1) * sizeof(char));

  if (curInfoMessageCopy == NULL)
  {
    sciprint(_("%s: No more memory.\n"), "Interactive zoom");
  }
  strcpy(curInfoMessageCopy, currentInfoMessage);

  startFigureDataWriting(parentFigure);
  /* display how to use zoom in the info bar */
  sciSetInfoMessage(parentFigure, _("Click on the figure to select zooming area. Click again to stop."));
  endFigureDataWriting(parentFigure);

  /* create a rubber box to select a rectangular area */
  pixelRubberBox(parentFigure, TRUE, TRUE, NULL, selectionRectangleCorners, &button);

  /* convert found data to [x,y,w,h] */
  x = Min(selectionRectangleCorners[0], selectionRectangleCorners[2]);
  y = Min(selectionRectangleCorners[1], selectionRectangleCorners[3]);
  w = Abs(selectionRectangleCorners[0] - selectionRectangleCorners[2]);
  h = Abs(selectionRectangleCorners[1] - selectionRectangleCorners[3]); 

  /* Zoom using the found pixels */
  startFigureDataWriting(parentFigure);
  if (sciGetEntityType(pObj) == SCI_FIGURE)
  {
    zoomFigure(pObj, x, y, w, h);
  }
  else if (sciGetEntityType(pObj) == SCI_SUBWIN)
  {
    zoomSubwin(pObj, x, y, w, h);
  }
  // restore info message
  sciSetInfoMessage(parentFigure, curInfoMessageCopy);
  endFigureDataWriting(parentFigure);

  FREE(curInfoMessageCopy);

  /* redraw */
  sciDrawObj(parentFigure);
}
/*------------------------------------------------------------------------------*/
/**
 * Perform a zoom rect (rectangular selection + zoom) on the current figure
 */
void sciDefaultInteractiveZoom(void)
{
  sciPointObj * curFigure;

  startGraphicDataWriting();
  curFigure = sciGetCurrentFigure();
  endGraphicDataWriting();

 
  /* zoom current figure */
  sciInteractiveZoom(curFigure);
}
/*------------------------------------------------------------------------------*/
/**
 * Check if the follawing data bounds can be used as new data bounds for the subwin object
 * @return TRUE if values can be used, false otherwise
 */
BOOL checkDataBounds(sciPointObj * pObj, double xMin, double xMax,
                     double yMin, double yMax, double zMin, double zMax)
{
  char logFlags[3];
  sciGetLogFlags(pObj, logFlags);

  /* check if there is not an inf within the values */
  /* since this has not any meaning */
  if (    !finite(xMin) || !finite(xMax)
       || !finite(yMin) || !finite(yMax)
       || !finite(zMin) || !finite(zMax) )
  {
    sciprint("Error : data bounds values must be finite.");
    return FALSE ;
  }


  /* check if the bounds are corrects */
  /* allows equality with bounds since it is working */
  if ( xMin > xMax || yMin > yMax || zMin > zMax )
  {
    sciprint("Error : Min and Max values for one axis do not verify Min <= Max.\n");
    return FALSE ;
  }

  /* check for logflags that values are greater than 0 */
  if (   ( logFlags[0] == 'l' && xMin <= 0.0 )
      || ( logFlags[1] == 'l' && yMin <= 0.0 )
      || ( logFlags[2] == 'l' && zMin <= 0.0 ) )
  {
    sciprint("Error: Bounds on axis must be strictly positive to use logarithmic mode.\n" ) ;
    return FALSE ;
  }

  return TRUE;
}
/*------------------------------------------------------------------------------*/
/**
 * Unzoom a single subwindow
 */
void sciUnzoomSubwin(sciPointObj * subwin)
{
  int currentStatus;
  sciPointObj * parentFig = sciGetParentFigure(subwin);
  startFigureDataWriting(parentFig);
  currentStatus = sciSetZooming(subwin, FALSE);

  if (currentStatus == 0)
  {
    /* redraw only if needed */
    forceRedraw(subwin);
  }

  endFigureDataWriting(parentFig);

}
/*------------------------------------------------------------------------------*/
/**
 * Unzoom all the subwindows contained in a figure
 */
void sciUnzoomFigure(sciPointObj * figure)
{
  /* Copy subwins into the array */ 
  sciSons * pSons = sciGetSons(figure);
  while (pSons != NULL)
  {
    sciPointObj * curObj = pSons->pointobj;
    if (sciGetEntityType(curObj) == SCI_SUBWIN)
    {
      sciUnzoomSubwin(curObj);
    }
    pSons = pSons->pnext;
  }
}
/*------------------------------------------------------------------------------*/
/**
 * Un zoom all the subwindow of a figure
 */
void sciUnzoomAll(void)
{
  sciPointObj * pFigure = NULL;

  startGraphicDataWriting();
  pFigure = sciGetCurrentFigure();
  endGraphicDataWriting();

  /* unzoom current figure */
  sciUnzoomFigure(pFigure);

  sciDrawObj(pFigure);
}
/*------------------------------------------------------------------------------*/
/**
 * Unzoom a set of subwindows and figures
 * @param zoomedObjects array of figure and subwindow objects
 */
void sciUnzoomArray(sciPointObj * zoomedObjects[], int nbObjects)
{
  int i;
  /* list of parent figure to redraw */
  DoublyLinkedList * redrawnFigures = DoublyLinkedList_new();
  for (i = 0; i < nbObjects; i++)
  {
    sciPointObj * parentFigure = sciGetParentFigure(zoomedObjects[i]);

    if (sciGetEntityType(zoomedObjects[i]) == SCI_FIGURE)
    {
      /* Unzoom all subwindows of the figure */
      sciUnzoomFigure(zoomedObjects[i]);
    }
    else if (sciGetEntityType(zoomedObjects[i]) == SCI_SUBWIN)
    {
      /* Unzoom only figure */
      sciUnzoomSubwin(zoomedObjects[i]);
    }
    
    if (List_find(redrawnFigures, parentFigure) == NULL)
    {
      /* figure not already added for redraw */
      redrawnFigures = List_push(redrawnFigures, parentFigure);
    }
  }

  /* redraw only needed figures */
  while (!List_is_empty(redrawnFigures))
  {
    sciPointObj * curFigure = NULL;
    redrawnFigures = List_pop(redrawnFigures, (void**)&curFigure);
    sciDrawObj(curFigure);
  }

  List_free(redrawnFigures);
}
/*--------------------------------------------------------------------------------*/
void updateSubwinScale(sciPointObj * pSubwin)
{
  sciJavaUpdateSubwinScale(pSubwin);
}
/*------------------------------------------------------------------------------*/
void updateTextBounds(sciPointObj * pText)
{
  /* update coordinates transformation if needed */
  updateSubwinScale(sciGetParentSubwin(pText));

  /* compute the boundign box of the text */
  sciJavaUpdateTextBoundingBox(pText);
}
/*------------------------------------------------------------------------------*/
