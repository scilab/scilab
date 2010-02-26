/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Call from Java to figure drawing code using JNI
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "FigureScilabCall.h"
#include "DrawableFigure.h"
#include "DrawingBridge.h"
#include "getHandleDrawer.h"
#include "GraphicSynchronizerInterface.h"
#include "GiwsException.hxx"

extern "C"
{
#include "WindowList.h"
#include "GetProperty.h"
#include "Interaction.h"
#include "Axes.h"
#include "axesScale.h"
#include "HandleManagement.h"
#include "Scierror.h"
#include "sciprint.h"
}

/*--------------------------------------------------------------------------*/
void displayFigure(int figureId)
{
  startGraphicDataReading();
  sciPointObj * curFig = getFigureFromIndex(figureId) ;
  endGraphicDataReading();

  if ( curFig == NULL )
  {
    return ;
  }

  try 
  {
	  startFigureDataDisplaying(curFig);
	  (sciGraphics::getFigureDrawer(curFig))->display() ;
	  endFigureDataDisplaying(curFig);
  } catch (const GiwsException::JniException& e)
  {
	  Scierror(999, "A native error occurred during the drawing of the figure.\nDescription: %s\nException Name: %s\nScilab's graphic may become unstable.\nPlease report a bug on http://bugzilla.scilab.org/ with your example and the result of getdebuginfo().\n",e.getJavaDescription().c_str(), e.getJavaExceptionName().c_str());
  } catch (const std::exception& e)
  {
	  sciprint("Error: %s\n",e.what());
	  Scierror(999, "An error occurred during the drawing of the figure.\nPlease report a bug on http://bugzilla.scilab.org/ with your example and the result of getdebuginfo().\n");
  }
}
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
void redrawFigure(int figureId)
{
  startGraphicDataReading();
  sciPointObj * curFig = getFigureFromIndex(figureId) ;
  endGraphicDataReading();

  if ( curFig == NULL )
  {
    return ;
  }

  startFigureDataDisplaying(curFig);
  (sciGraphics::getFigureDrawer(curFig))->familyHasChanged();
  endFigureDataDisplaying(curFig);
}
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
void redrawSubwins(int figureId)
{
  startGraphicDataReading();
  sciPointObj * curFig = getFigureFromIndex(figureId) ;
  endGraphicDataReading();

  if ( curFig == NULL )
  {
    return ;
  }

  startFigureDataDisplaying(curFig);
  (sciGraphics::getFigureDrawer(curFig))->redrawSubwins() ;
  endFigureDataDisplaying(curFig);
}
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
void rotateSubwin(long long subwinHandle, double deltaAlpha, double deltaTheta)
{
	startGraphicDataReading();
	sciPointObj * pSubwin = sciGetPointerFromHandle((long) subwinHandle) ;
	sciPointObj * parentFigure = sciGetParentFigure(pSubwin);
  endGraphicDataReading();

  if ( pSubwin == NULL || parentFigure == NULL )
  {
    return ;
  }

  startFigureDataWriting(parentFigure);
	updateViewingAngles(pSubwin, deltaAlpha, deltaTheta);
  endFigureDataWriting(parentFigure);
}
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
long long getClickedSubwinHandle(int figureId, int clickXCoord, int clickYCoord)
{
	startGraphicDataReading();
	sciPointObj * curFig = getFigureFromIndex(figureId) ;
  endGraphicDataReading();

  if ( curFig == NULL )
  {
    return 0;
  }

  startFigureDataReading(curFig);
	sciPointObj * clickedSubwin = getClickedSubwin(curFig, clickXCoord, clickYCoord);
  endFigureDataReading(curFig);

	// Will return 0 if clicked subwin is null or the handle otherwise
	return sciGetHandle(clickedSubwin);

}
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
void zoomObject(long long objectHandle, int x1, int y1, int x2, int y2)
{
	startGraphicDataReading();
	sciPointObj * pObj = sciGetPointerFromHandle((long) objectHandle) ;
	sciPointObj * parentFigure = sciGetParentFigure(pObj);
  endGraphicDataReading();

  if ( pObj == NULL || parentFigure == NULL )
  {
    return ;
  }

  startFigureDataWriting(parentFigure);
	sciZoomObject(pObj, x1, y1, x2, y2);
  endFigureDataWriting(parentFigure);
}
/*--------------------------------------------------------------------------*/
void unzoomSubwinHandle(long long subwinHandle)
{
	startGraphicDataReading();
	sciPointObj * pSubwin = sciGetPointerFromHandle((long) subwinHandle) ;
	sciPointObj * parentFigure = sciGetParentFigure(pSubwin);
  endGraphicDataReading();

	if ( pSubwin == NULL || parentFigure == NULL )
  {
    return ;
  }

  startFigureDataWriting(parentFigure);
	unzoomSubwin(pSubwin);
  endFigureDataWriting(parentFigure);
}
/*--------------------------------------------------------------------------*/
