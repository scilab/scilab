/*------------------------------------------------------------------------*/
/* file: MiddleXTicksDrawerGL.java                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing ticks on of X axis placed on the middle of the    */
/*        axes box                                                        */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

/**
 * Class drawing ticks on of X axis placed on the middle of the axes box
 * @author Jean-Baptiste Silvy
 */
public class MiddleXTicksDrawerGL extends XTicksDrawerGL {

	/**
	 * Default constructor
	 */
	public MiddleXTicksDrawerGL() {
		super();
	}
	
	/**
	 * Compute the Y coordinate of the X axis segment
	 * @param zCoordinate Z coordinate of the X axis segment alredy computed by findZCoordinate
	 * @return Y coordinate of the segment to draw
	 */
	public double findYCoordinate(double zCoordinate) {
		return findMiddleYCoordinate(zCoordinate);
	}

	
	/**
	 * Find the Z coordinate of the X axis segment.
	 * X axis segment is always displayed on the bottom of tyhe box.
	 * @return Z coordinate of the segment to draw
	 */
	public double findZCoordinate() {
		return findUpperZCoordinate();
	}

}
