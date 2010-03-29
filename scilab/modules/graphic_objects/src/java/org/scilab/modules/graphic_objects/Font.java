package org.scilab.modules.graphic_objects;

/**
 * Font class
 * @author juliachs
 */
public class Font {
	/** Font style */
	private int style;

	/** Font size */
	private double size;

	/** Font color */
	private int color;

	/** Specifies whether fractional font sizes are used or not */
	private boolean fractional;

	/** Constructor */
	public Font() {
		this.style = 0;
		this.size = 0.0;
		this.color = 0;
		this.fractional = false;
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
	 * @return the fractional
	 */
	public boolean isFractional() {
		return fractional;
	}

	/**
	 * @param fractional the fractional to set
	 */
	public void setFractional(boolean fractional) {
		this.fractional = fractional;
	}

	/**
	 * @return the size
	 */
	public double getSize() {
		return size;
	}

	/**
	 * @param size the size to set
	 */
	public void setSize(double size) {
		this.size = size;
	}

	/**
	 * @return the style
	 */
	public int getStyle() {
		return style;
	}

	/**
	 * @param style the style to set
	 */
	public void setStyle(int style) {
		this.style = style;
	}
	
	//	 Methods: to be done
}
