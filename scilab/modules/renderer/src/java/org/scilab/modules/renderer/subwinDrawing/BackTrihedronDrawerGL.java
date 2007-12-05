/*------------------------------------------------------------------------*/
/* file: BackTrihedronDrawerGL.java                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing only the backTrihedron of a subwin object         */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

/**
 * Class drawing only the backTrihedron of a subwin object
 * @author Jean-Baptiste Silvy
 */
public class BackTrihedronDrawerGL extends SubwinBoxDrawer {

	/**
	 * Default constuctor
	 */
	public BackTrihedronDrawerGL() {
		super();
	}
	
	
	/**
	 * An other seetting for parameters since line colro is not needed
	 * @param hiddenAxisColor color of back trihedron lines
	 * @param backgroundColor color of back trihedron facets
	 * @param lineStyle pattern to use for lines
	 * @param thickness thickness of the lines
	 */
	public void setBoxParameters(int hiddenAxisColor, int backgroundColor,
								 int lineStyle, float thickness) {
		setHiddenAxisColor(hiddenAxisColor);
		setBackgroundColor(backgroundColor);
		setLineStyle(lineStyle);
		setThickness(thickness);
	}
	
	/**
	 * Draw the box specified by its bounds.
	 */
	public void drawBox() {
		drawTrihedron();
	}

}
