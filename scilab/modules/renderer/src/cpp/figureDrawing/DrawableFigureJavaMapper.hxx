/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Contains mapping of java method used by DrawableFigure
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_FIGURE_JAVA_MAPPER_HXX_
#define _DRAWABLE_FIGURE_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include <string>
#include "DrawableFigureGL.hxx"

namespace sciGraphics
{

class DrawableFigureJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  DrawableFigureJavaMapper( void ) ;

  virtual ~DrawableFigureJavaMapper( void ) ;

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int parentFigureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for figures
  virtual void drawCanvas(void);

  virtual void drawBackground();
  
  virtual void setLogicalOp(int logicOpIndex);


  virtual void setColorMapData(const double rgbmat[], int nbColor);
  virtual void getColorMapData(double rgbmat[]);
  virtual int getColorMapSize(void);

  virtual int getCanvasWidth(void);
  virtual int getCanvasHeight(void);
  virtual int setCanvasSize(int width, int height);

  virtual int getWindowPosX(void);
  virtual int getWindowPosY(void);
  virtual void setWindowPosition(int posX, int posY);

  virtual int getWindowWidth(void);
  virtual int getWindowHeight(void);
  virtual void setWindowSize(int width, int height);

  virtual void setInfoMessage(const char * infoMessage);
	virtual void getInfoMessage(char * infoMessage);
	virtual int getInfoMessageLength(void);

  virtual void setAutoResizeMode(bool onOrOff);
  virtual bool getAutoResizeMode(void);

  virtual void getViewport(int viewport[4]);
  virtual void setViewport(const int viewport[4]);

  virtual void setBackgroundColor(int colorIndex);

  virtual void rubberBox(bool isClick, const int initialRect[4], int endRect[4], int * usedButton);

	virtual void interactiveZoom(long objectHandle);

  virtual void setTitle(const char * title);

  virtual void showWindow(void);

  virtual void setNbSubwins(int nbSubwins);

	virtual void openGraphicCanvas(void);

	virtual void closeGraphicCanvas(void);

	virtual void interactiveRotation(void);

	virtual void setUseSingleBuffer(bool useSingleBuffer);

  int getAntialiasingQuality(void);
	
	void setAntialiasingQuality(int quality);

  static bool isAbleToCreateFigure(void);
  /*----------------------------------------------------------------------*/

protected:

  /** giws generated file */
  org_scilab_modules_renderer_figureDrawing::DrawableFigureGL * m_pJavaObject;

};

}

#endif /* _DRAWABLE_FIGURE_JAVA_MAPPER_HXX_ */

