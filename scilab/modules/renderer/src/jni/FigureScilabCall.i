/* Swig file: FigureScilabCallGuiManagement.i */
/*  swig -java -package org.scilab.modules.renderer.figureDrawing -outdir ../java/org/scilab/modules/renderer/figureDrawing/ FigureScilabCall.i */
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

/* JavaDoc for FigureScilabCall class */
%pragma(java) moduleclassmodifiers="
 /** 
  * Call from the JoGL dislay thread to figure drawing code using JNI 
  * @author Vincent COUVERT
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
";
void displayFigure(int figureId);

/* JavaDoc */
%javamethodmodifiers redrawFigure(int figureId) "
 /**
  * Redraw a figure and its all children inside the OpenGL context
  * @param figureId index of the figure to redraw
  */
";
void redrawFigure(int figureId);
