/*------------------------------------------------------------------------*/
/* file: FullBoxDrawerGL.java                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing all of a subwin object bounding box               */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

/**
 * Class drawing only the backTrihedron of a subwin object
 * @author Jean-Baptiste Silvy
 */
public class FullBoxDrawerGL extends SubwinBoxDrawer {

	/**
	 * Default constuctor
	 */
	public FullBoxDrawerGL() {
		super();
	}
	
	
	/**
	 * Draw the box specified by its bounds.
	 */
	public void drawBox() {
		int concealedCornerIndex = findConcealedCorner();
		drawTrihedron(concealedCornerIndex);
		drawHalfBox(concealedCornerIndex);
		drawFrontTrihedron(concealedCornerIndex);
	}

}
