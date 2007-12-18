/*------------------------------------------------------------------------*/
/* file: RightYTicksDrawerGL.java                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing ticks on of Y axis placed on the right of the     */
/*        axes box                                                        */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

/**
 * Class drawing ticks on of Y axis placed on the left of the axes box.
 * @author Jean-Baptiste Silvy
 */
public class RightYTicksDrawerGL extends YTicksDrawerGL {

	/**
	 * Default consturctor
	 */
	public RightYTicksDrawerGL() {
		super();
	}
	
	
	/**
	 * Compute the X coordinate of the X axis segment
	 * @param zCoordinate Z coordinate of the Y axis segment already computed by findZCoordinate
	 * @return Y coordinate of the segment to draw
	 */
	public double findXCoordinate(double zCoordinate) {
		return findBackXCoordinate(zCoordinate);
	}

	/**
	 * Find the Z coordinate of the Y axis segment.
	 * Y axis segment is always displayed on the bottom of the box.
	 * @return Z coordinate of the segment to draw
	 */
	public double findZCoordinate() {
		return findUpperZCoordinate();
	}

}
