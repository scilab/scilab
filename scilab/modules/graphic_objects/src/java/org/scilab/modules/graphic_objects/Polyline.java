package org.scilab.modules.graphic_objects;

/**
 * Polyline class
 * @author juliachs
 */
public class Polyline extends ClippableContouredObject {
	/** TBD: data */
	// Data data -> Data Model

	/** Specifies whether the polyline is closed */
	private boolean closed;
	
	/** Determines the arrow size */
	private double arrowSizeFactor;

	/** Polyline drawing style (normal, staircase, bar, etc.) */
	private int polylineStyle;
	
	/** Interpolation color vector (3- or 4-element array) */
	private int [] interpColorVector;

	/** Specifies whether interpolated shading is used */
	private boolean interpColorMode;

	/** Shift applied to the line points along the x-axis */
	private double [] xShift;

	/** Shift applied to the line points along the y-axis */
	private double [] yShift;

	/** Shift applied to the line points along the z-axis */
	private double [] zShift;

	/** Bar width */
	private double barWidth;
	
	//	 Methods: to be done
}
