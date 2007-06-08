/*------------------------------------------------------------------------*/
/* file: FigureScilabCall.java                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : set of native functions used to draw figure                     */
/*------------------------------------------------------------------------*/


package org.scilab.modules.graphics.figureDrawing;

/**
 * set of native functions used to draw figure
 * @author Jean-Baptiste Silvy
 */
public class FigureScilabCall {

	/**
	 * Default constructor
	 * Do nothing it is just a set of functions
	 */
	FigureScilabCall() { }
	
	/**
	 * Draw the figure with figureId in the openGL context
	 * @param figureId number of the figure to redraw
	 */
	public native void displayFigure(int figureId);
	
	/**
	 * Redraw a figure and all its children
	 * @param figureId number of the figure to redraw
	 */
	public native void redrawFigure(int figureId);
	
	
	static
	{
		System.loadLibrary("scirenderer");
	}
}
