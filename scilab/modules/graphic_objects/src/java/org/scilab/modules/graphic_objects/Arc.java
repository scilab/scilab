package org.scilab.modules.graphic_objects;

/**
 * Arc class
 * @author juliachs
 */
public class Arc extends ClippableContouredObject {
	/** Arc drawing method */
	public enum ArcDrawingMethod { NURBS, LINES };

	/** Bounding box upper-left point (x,y,z) coordinates */
	private double [] upperLeftPoint;

	/** Bounding box width */
	private double width;

	/** Bounding box height */
	private double height;

	/** Start angle */
	private double startAngle;

	/** End angle */
	private double endAngle;
	
	/** Specifies which drawing method to use */
	ArcDrawingMethod arcDrawingMethod;
	
	/**
	 * Constructor
	 */
	public Arc() {
		arcDrawingMethod = ArcDrawingMethod.LINES;
		upperLeftPoint = null;
		width = 0.0;
		height = 0.0;
		startAngle = 0.0;
		endAngle = 0.0;
	}

	/**
	 * @return the arcDrawingMethod
	 */
	public ArcDrawingMethod getArcDrawingMethod() {
		return arcDrawingMethod;
	}

	/**
	 * @param arcDrawingMethod the arcDrawingMethod to set
	 */
	public void setArcDrawingMethod(ArcDrawingMethod arcDrawingMethod) {
		this.arcDrawingMethod = arcDrawingMethod;
	}

	/**
	 * @return the endAngle
	 */
	public double getEndAngle() {
		return endAngle;
	}

	/**
	 * @param endAngle the endAngle to set
	 */
	public void setEndAngle(double endAngle) {
		this.endAngle = endAngle;
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
	 * @return the startAngle
	 */
	public double getStartAngle() {
		return startAngle;
	}

	/**
	 * @param startAngle the startAngle to set
	 */
	public void setStartAngle(double startAngle) {
		this.startAngle = startAngle;
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
