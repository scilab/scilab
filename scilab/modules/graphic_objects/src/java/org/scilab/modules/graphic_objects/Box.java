package org.scilab.modules.graphic_objects;

/**
 * Box class
 * @author juliachs
 */
public class Box {
	/** Box type */
	private static enum BoxType { ON, OFF, HIDDEN_AXES, BACK_HALF };

	/** Box type */
	private BoxType box;
	
	/** Hidden axis color */
	private int hiddenAxisColor;

	/** Specifies whether tight limits are enforced or not */
	private boolean tightLimits;

	/** Data bounding box (6-element array) */
	private double [] dataBounds;

	/** Data bounding box, as modified by ticks computation (6-element array) */
	private double [] realDataBounds;

	/** Magnified 3D sub-region (6-element array) */
	private double [] zoomBox;

	/** Indicates whether data bounds are updated when a new plot command is executed */ 
	private boolean autoScale;

	//	 Methods: to be done
}
