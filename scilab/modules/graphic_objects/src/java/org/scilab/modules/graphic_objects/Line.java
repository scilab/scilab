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

	/** Constructor */
	public Line() {
		mode = false;
		lineStyle = null;
		thickness = 0.0;
		color = 0;
	}

	/**
	 * @return the color
	 */
	public int getColor() {
		return color;
	}

	/**
	 * @param color the color to set
	 */
	public void setColor(int color) {
		this.color = color;
	}

	/**
	 * @return the lineStyle
	 */
	public LineType getLineStyle() {
		return lineStyle;
	}

	/**
	 * @param lineStyle the lineStyle to set
	 */
	public void setLineStyle(LineType lineStyle) {
		this.lineStyle = lineStyle;
	}

	/**
	 * @return the mode
	 */
	public boolean isMode() {
		return mode;
	}

	/**
	 * @param mode the mode to set
	 */
	public void setMode(boolean mode) {
		this.mode = mode;
	}

	/**
	 * @return the thickness
	 */
	public double getThickness() {
		return thickness;
	}

	/**
	 * @param thickness the thickness to set
	 */
	public void setThickness(double thickness) {
		this.thickness = thickness;
	}
	
	//	 Methods: to be done
}
