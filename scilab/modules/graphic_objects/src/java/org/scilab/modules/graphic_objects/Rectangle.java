package org.scilab.modules.graphic_objects;

/**
 * Class Rectangle
 * @author juliachs
 */
public class Rectangle extends ClippableContouredObject {
	/** Upper-left corner (x,y,z) coordinates */
	private double [] upperLeftPoint;
	
	/** Width */
	private double width;

	/** Height */
	private double height;

	/** Constructor */
	public Rectangle() {
		super();
		upperLeftPoint = null;
		width = 0.0;
		height = 0.0;
	}

	/**
	 * @return the height
	 */
	public double getHeight() {
		return height;
	}

	/**
	 * @param height the height to set
	 */
	public void setHeight(double height) {
		this.height = height;
	}

	/**
	 * @return the upperLeftPoint
	 */
	public double[] getUpperLeftPoint() {
		return upperLeftPoint;
	}

	/**
	 * @param upperLeftPoint the upperLeftPoint to set
	 */
	public void setUpperLeftPoint(double[] upperLeftPoint) {
		this.upperLeftPoint = upperLeftPoint;
	}

	/**
	 * @return the width
	 */
	public double getWidth() {
		return width;
	}

	/**
	 * @param width the width to set
	 */
	public void setWidth(double width) {
		this.width = width;
	}
	
	//	 Methods: to be done
}
