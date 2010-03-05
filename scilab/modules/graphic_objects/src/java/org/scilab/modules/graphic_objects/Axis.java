package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

/**
 * Axis class
 * @author juliachs
 */
public class Axis extends ClippableContouredObject {
	/** Ticks direction */
	private enum TicksDirection { TOP, BOTTOM, LEFT, RIGHT };
	
	/** Ticks direction */
	private TicksDirection ticksDirection;

	/** Ticks x-coordinate position vector */
	private double [] xTicksCoords; 

	/** Ticks y-coordinate position vector */
	private double [] yTicksCoords;
	
	/** Ticks color */
	private int ticksColor;

	/** Specifies whether the axis segment is drawn */
	private boolean ticksSegment;

	/** Ticks labels list */
	private ArrayList <String> ticksLabels;

	/** Label format */
	private String formatn;

	/** Font */
	private Font font;
	
	//	 Methods: to be done
}
