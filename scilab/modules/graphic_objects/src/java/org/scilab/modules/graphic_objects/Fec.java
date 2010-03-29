package org.scilab.modules.graphic_objects;

/**
 * Fec class
 * @author juliachs
 */
public class Fec extends ClippableContouredObject {
	// TBD: Data
	// + triangles	-> Data Model

	/** Z Bounds: 2-element array */
	private double [] zBounds;
	
	/** 2-element array */
	private int [] outsideColor;

	/** 2-element array */
	private int [] colorRange;

	/** Constructor */
	public Fec() {
		super();
		zBounds = null;
		outsideColor = null;
		colorRange = null;
	}

	/**
	 * @return the colorRange
	 */
	public int[] getColorRange() {
		return colorRange;
	}

	/**
	 * @param colorRange the colorRange to set
	 */
	public void setColorRange(int[] colorRange) {
		this.colorRange = colorRange;
	}

	/**
	 * @return the outsideColor
	 */
	public int[] getOutsideColor() {
		return outsideColor;
	}

	/**
	 * @param outsideColor the outsideColor to set
	 */
	public void setOutsideColor(int[] outsideColor) {
		this.outsideColor = outsideColor;
	}

	/**
	 * @return the zBounds
	 */
	public double[] getZBounds() {
		return zBounds;
	}

	/**
	 * @param bounds the zBounds to set
	 */
	public void setZBounds(double[] bounds) {
		zBounds = bounds;
	}

	// 	Methods: to be done
}
