package org.scilab.modules.graphic_objects;

/**
 * Axes class
 * @author juliachs
 */
public class Axes extends GraphicObject {
	/** Specifies the grid position relative to the graphics entities */
	private static enum GridPosition { FOREGROUND, BACKGROUND };

	/** 3-element array (properties of the X, Y and Z axes) */
	private AxisProperty [] axes;

	/** Grid position */
	private GridPosition gridPosition;
	
	/** Title label */
	private Label title;

	/** Specifies whether the Axes subwindow is cleared when a new plot command is performed */ 
	private boolean autoClear;

	/** Specifies whether the Axes background is filled or not  */
	private boolean filled;

	/** Camera */
	private Camera camera;

	/** Box: properties related to the data bounds */
	private Box box;
	
	/** Margins enclosing the drawing area (left, right, top, bottom) */
	private double [] margins;
	
	/**
	 * Axes bounds relative to their parent figure's
	 * drawing area (x, y, width, height)
	 */
	private double [] axesBounds;

	//	 Methods: to be done
}
