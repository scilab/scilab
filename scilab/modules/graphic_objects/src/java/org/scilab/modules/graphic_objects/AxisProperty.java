package org.scilab.modules.graphic_objects;

/**
 * Axis property class
 * @author juliachs
 */
public class AxisProperty {
	/**
	 * Axis location type
	 * BOTTOM, TOP, MIDDLE, ORIGIN are allowed for an x-axis,
	 * LEFT, RIGHT, MIDDLE or ORIGIN are allowed for a y-axis
	 */
	private static enum AxisLocation { BOTTOM, TOP, MIDDLE, ORIGIN, LEFT, RIGHT };
	
	/** Specifies whether the axis is visible or not */
	private boolean visible;
	
	/** Specifies whether the axis is reversed or not */
	private boolean reverse;
	
	/** Grid color */
	private int gridColor;

	/** Axis label */
	private Label label;

	/** Axis location */
	private AxisLocation axisLocation;

	/** Ticks property */
	private TicksProperty ticks;

	/** Specifies whether logarithmic coordinates are used or not */
	private boolean logFlag;
	
	//	 Methods: to be done
}
