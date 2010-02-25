package org.scilab.modules.graphic_objects;

/**
 * Line class
 * @author juliachs
 */
public class Line {
	/** Line style */
	enum LineType { SOLID, STYLE1, STYLE2, STYLE3, STYLE4, STYLE5, STYLE6 };
	
	/** Specifies whether the line is drawn or not */
	private boolean mode;

	/** Line style */
	private LineType lineStyle;

	/** Line thickness */
	private double thickness;

	/** Line color */
	private int color;
	
	//	 Methods: to be done
}
