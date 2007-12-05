/*------------------------------------------------------------------------*/
/* file: HaflBoxDrawerGL.java                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing half of a subwin object box                       */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

/**
 * Class drawing only the backTrihedron of a subwin object
 * @author Jean-Baptiste Silvy
 */
public class HalfBoxDrawerGL extends SubwinBoxDrawer {

	/**
	 * Default constuctor
	 */
	public HalfBoxDrawerGL() {
		super();
	}
	
	
	/**
	 * Draw the box specified by its bounds.
	 */
	public void drawBox() {
		int concealedCornerIndex = findConcealedCorner();
		drawTrihedron(concealedCornerIndex);
		drawHalfBox(concealedCornerIndex);
	}

}
