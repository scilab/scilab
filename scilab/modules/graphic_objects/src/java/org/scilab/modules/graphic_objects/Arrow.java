package org.scilab.modules.graphic_objects;

/**
 * Arrow class
 * @author juliachs
 */
public class Arrow extends ContouredObject {
	/** Arrow base (x,y,z) coordinates */
	private double [] base;

	/** Arrow vector (x,y,z) coordinates */
	private double [] direction;

	/** Arrow size */
	private double arrowSize;

	/** Constructor */
	public Arrow() {
		super();
		base = null;
		direction = null;
		arrowSize = 0.0;
	}
	
	/**
	 * @return the arrowSize
	 */
	public double getArrowSize() {
		return arrowSize;
	}

	/**
	 * @param arrowSize the arrowSize to set
	 */
	public void setArrowSize(double arrowSize) {
		this.arrowSize = arrowSize;
	}

	/**
	 * @return the base
	 */
	public double[] getBase() {
		return base;
	}

	/**
	 * @param base the base to set
	 */
	public void setBase(double[] base) {
		this.base = base;
	}

	/**
	 * @return the direction
	 */
	public double[] getDirection() {
		return direction;
	}

	/**
	 * @param direction the direction to set
	 */
	public void setDirection(double[] direction) {
		this.direction = direction;
	}

	//	 Methods: to be done
}
