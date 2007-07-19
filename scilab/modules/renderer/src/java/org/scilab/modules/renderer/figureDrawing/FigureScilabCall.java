/*------------------------------------------------------------------------*/
/* file: FigureScilabCall.java                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : set of native functions used to draw figure                     */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.figureDrawing;

/**
 * set of native functions used to draw figure
 * @author Jean-Baptiste Silvy
 */
public class FigureScilabCall {

	/**
	 * Default constructor
	 * Do nothing it is just a set of functions
	 */
	public FigureScilabCall() { }
	
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
		try {
			System.loadLibrary("scirenderer");
		} catch (SecurityException e) {
			System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
			e.printStackTrace(System.err);
		} catch (UnsatisfiedLinkError e) {
			System.err.println("The native library scirenderer does not exist or cannot be found.");
			e.printStackTrace(System.err);
		}
	}
}
