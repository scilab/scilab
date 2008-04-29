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
