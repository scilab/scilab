
/* Swig file: FigureScilabCallGuiManagement.i */
/*  swig -java -package org.scilab.modules.renderer.jni -outdir ../java/org/scilab/modules/renderer/jni/ FigureScilabCall.i */
%module FigureScilabCall


%{
#include "figureDrawing/FigureScilabCall.h"
%}

/* JavaDoc for FigureScilabCallJNI class */
%pragma(java) jniclassclassmodifiers=%{
 /** 
  * Call from the JoGL display thread to figure drawing code using JNI 
  * @author Jean-Baptiste Silvy
  * @copyright INRIA 2007
  */
public class%}

/* Constructor for FigureScilabCallJNI class */
%pragma(java) jniclasscode="
  /**
   * Constructor. Should not be called
   */
  protected FigureScilabCallJNI() {
	throw new UnsupportedOperationException();
  }";

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("scirenderer");
    } catch (SecurityException e) {
		System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
		e.printStackTrace(System.err);
	} catch (UnsatisfiedLinkError e)	{
		System.err.println("The native library scirenderer does not exist or cannot be found.");
		e.printStackTrace(System.err);
    }
  }
%}

/* JavaDoc for FigureScilabCall class */
%pragma(java) moduleclassmodifiers="
 /** 
  * Call from the JoGL dislay thread to figure drawing code using JNI 
  * @author Jean-Baptiste Silvy
  * @copyright INRIA 2007 
  */
public class";

/* Constructor for FigureScilabCall class */
%pragma(java) modulecode="
 /**
  * Constructor. Should not be called.
  */
 protected FigureScilabCall() {
	throw new UnsupportedOperationException();
 }";

/* JavaDoc */
%javamethodmodifiers displayFigure(int figureId) "
 /**
  * Draw a figure inside the OpenGL context
  * @param figureId index of the figure to redraw
  */
public";
void displayFigure(int figureId);

/* JavaDoc */
%javamethodmodifiers redrawFigure(int figureId) "
 /**
  * Redraw a figure and its all children inside the OpenGL context
  * @param figureId index of the figure to redraw
  */
public";
void redrawFigure(int figureId);

/* JavaDoc */
%javamethodmodifiers redrawSubwins(int figureId) "
 /**
  * Redraw all the subwindows inside a figure.
  * This should be called when the figure is resized.
  * @param figureId index of the figure to redraw
  */
public";
void redrawSubwins(int figureId);

/* JavaDoc */
%javamethodmodifiers rotateSubwin(long long subwinHandle, double deltaAlpha, double deltaTheta) "
 /**
  * Modify a subwin viewing angles by a certain amount.
  * @param subwinHandle handle of the subwin to modify
  * @param deltaAlpha modify the alpha angle with this value
  * @param deltaTheta modify the theta angle with this value
  */
public";
void rotateSubwin(long long subwinHandle, double deltaAlpha, double deltaTheta);

/* JavaDoc */
%javamethodmodifiers getClickedSubwinHandle(int figureId, int clickXCoord, int clickYCoord) "
 /**
  * Modify a subwin viewing angles by a certain amount.
  * @param figureId index of the figure to check
  * @param clickXCoord x coordinate of the mouse click
  * @param clickYCoord y coordinate of the mouse click
  * @return handle of the clicked subwin or 0 if no subwin was clicked
  */
public";
long long getClickedSubwinHandle(int figureId, int clickXCoord, int clickYCoord);

/* JavaDoc */
%javamethodmodifiers zoomObject(long long objectHandle, int x1, int y1, int x2, int y2) "
 /**
  * Zoom a figure or an axis handle using a selection area.
  * @param objectHandle handle of the figure o subwin to modify
  * @param x1 X coordinate of a corner of the selection
  * @param y1 Y coordinate of a corner of the selection
  * @param x2 X coordinate of the oppoiste corner of the selection
  * @param y2 Y coordinate of the opposite corner of the selection
  */
public";
void zoomObject(long long objectHandle, int x1, int y1, int x2, int y2);

/* JavaDoc */
%javamethodmodifiers unzoomSubwinHandle(long long subwinHandle) "
 /**
  * Reset zoom of a subwindow.
  * @param subwinHandle handle of the subwin to modify
  */
public";
void unzoomSubwinHandle(long long subwinHandle);
