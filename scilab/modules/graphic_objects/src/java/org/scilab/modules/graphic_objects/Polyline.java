package org.scilab.modules.graphic_objects;

/**
 * Polyline class
 * @author juliachs
 */
public class Polyline extends ClippableContouredObject {
	/** TBD: data */
	// Data data -> Data Model

	/** Specifies whether the polyline is closed */
	private boolean closed;
	
	/** Determines the arrow size */
	private double arrowSizeFactor;

	/** Polyline drawing style (normal, staircase, bar, etc.) */
	private int polylineStyle;
	
	/** Interpolation color vector (3- or 4-element array) */
	private int [] interpColorVector;

	/** Specifies whether interpolated shading is used */
	private boolean interpColorMode;

	/** Shift applied to the line points along the x-axis */
	private double [] xShift;

	/** Shift applied to the line points along the y-axis */
	private double [] yShift;

	/** Shift applied to the line points along the z-axis */
	private double [] zShift;

	/** Bar width */
	private double barWidth;

	/** Constructor */
	public Polyline() {
		super();
		closed = false;
		arrowSizeFactor = 0.0;
		polylineStyle = 0;
		interpColorVector = null;
		interpColorMode = false;
		xShift = null;
		yShift = null;
		zShift = null;
		barWidth = 0.0;
	}

	/**
	 * @return the arrowSizeFactor
	 */
	public double getArrowSizeFactor() {
		return arrowSizeFactor;
	}

	/**
	 * @param arrowSizeFactor the arrowSizeFactor to set
	 */
	public void setArrowSizeFactor(double arrowSizeFactor) {
		this.arrowSizeFactor = arrowSizeFactor;
	}

	/**
	 * @return the barWidth
	 */
	public double getBarWidth() {
		return barWidth;
	}

	/**
	 * @param barWidth the barWidth to set
	 */
	public void setBarWidth(double barWidth) {
		this.barWidth = barWidth;
	}

	/**
	 * @return the closed
	 */
	public boolean isClosed() {
		return closed;
	}

	/**
	 * @param closed the closed to set
	 */
	public void setClosed(boolean closed) {
		this.closed = closed;
	}

	/**
	 * @return the interpColorMode
	 */
	public boolean isInterpColorMode() {
		return interpColorMode;
	}

	/**
	 * @param interpColorMode the interpColorMode to set
	 */
	public void setInterpColorMode(boolean interpColorMode) {
		this.interpColorMode = interpColorMode;
	}

	/**
	 * @return the interpColorVector
	 */
	public int[] getInterpColorVector() {
		return interpColorVector;
	}

	/**
	 * @param interpColorVector the interpColorVector to set
	 */
	public void setInterpColorVector(int[] interpColorVector) {
		this.interpColorVector = interpColorVector;
	}

	/**
	 * @return the polylineStyle
	 */
	public int getPolylineStyle() {
		return polylineStyle;
	}

	/**
	 * @param polylineStyle the polylineStyle to set
	 */
	public void setPolylineStyle(int polylineStyle) {
		this.polylineStyle = polylineStyle;
	}

	/**
	 * @return the xShift
	 */
	public double[] getXShift() {
		return xShift;
	}

	/**
	 * @param shift the xShift to set
	 */
	public void setXShift(double[] shift) {
		xShift = shift;
	}

	/**
	 * @return the yShift
	 */
	public double[] getYShift() {
		return yShift;
	}

	/**
	 * @param shift the yShift to set
	 */
	public void setYShift(double[] shift) {
		yShift = shift;
	}

	/**
	 * @return the zShift
	 */
	public double[] getZShift() {
		return zShift;
	}

	/**
	 * @param shift the zShift to set
	 */
	public void setZShift(double[] shift) {
		zShift = shift;
	}
	
	//	 Methods: to be done
}
