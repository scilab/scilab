package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

public class Polyline extends ClippableContouredObject {
	/** TBD: data */
	// Data data -> Data Model

	/** Specifies whether the polyline is closed */
	private boolean closed;
	
	/** Determines the arrow size */
	private double arrowSizeFactor;

	/** Polyline drawing style (normal, staircase, bar, etc.) */
	private int polylineStyle;
	
	/** Interpolation color vector */
	private ArrayList <Integer> interpColorVector;

	/** Specifies whether interpolated shading is used */
	private boolean interpColorMode;

	/** Shift applied to the line points along the x-axis */
	private ArrayList <Double> xShift;

	/** Shift applied to the line points along the y-axis */
	private ArrayList <Double> yShift;

	/** Shift applied to the line points along the z-axis */
	private ArrayList <Double> zShift;

	/** Bar width */
	private double barWidth;
	
	//	 Methods: to be done
}
