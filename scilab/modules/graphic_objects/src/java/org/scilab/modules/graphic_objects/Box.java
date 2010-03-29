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

	/** Data bounding box, as modified by automatic ticks computation (6-element array) */
	private double [] realDataBounds;

	/** Magnified 3D sub-region (6-element array) */
	private double [] zoomBox;

	/** Indicates whether data bounds are updated when a new plot command is executed */ 
	private boolean autoScale;

	/** Constructor */
	public Box() {
		box = BoxType.OFF;
		hiddenAxisColor = 0;
		tightLimits = false;
		dataBounds = null;
		realDataBounds = null;
		zoomBox = null;
		autoScale = false;
	}

	/**
	 * @return the autoScale
	 */
	public boolean isAutoScale() {
		return autoScale;
	}

	/**
	 * @param autoScale the autoScale to set
	 */
	public void setAutoScale(boolean autoScale) {
		this.autoScale = autoScale;
	}

	/**
	 * @return the box
	 */
	public BoxType getBox() {
		return box;
	}

	/**
	 * @param box the box to set
	 */
	public void setBox(BoxType box) {
		this.box = box;
	}

	/**
	 * @return the dataBounds
	 */
	public double[] getDataBounds() {
		return dataBounds;
	}

	/**
	 * @param dataBounds the dataBounds to set
	 */
	public void setDataBounds(double[] dataBounds) {
		this.dataBounds = dataBounds;
	}

	/**
	 * @return the hiddenAxisColor
	 */
	public int getHiddenAxisColor() {
		return hiddenAxisColor;
	}

	/**
	 * @param hiddenAxisColor the hiddenAxisColor to set
	 */
	public void setHiddenAxisColor(int hiddenAxisColor) {
		this.hiddenAxisColor = hiddenAxisColor;
	}

	/**
	 * @return the realDataBounds
	 */
	public double[] getRealDataBounds() {
		return realDataBounds;
	}

	/**
	 * @param realDataBounds the realDataBounds to set
	 */
	public void setRealDataBounds(double[] realDataBounds) {
		this.realDataBounds = realDataBounds;
	}

	/**
	 * @return the tightLimits
	 */
	public boolean isTightLimits() {
		return tightLimits;
	}

	/**
	 * @param tightLimits the tightLimits to set
	 */
	public void setTightLimits(boolean tightLimits) {
		this.tightLimits = tightLimits;
	}

	/**
	 * @return the zoomBox
	 */
	public double[] getZoomBox() {
		return zoomBox;
	}

	/**
	 * @param zoomBox the zoomBox to set
	 */
	public void setZoomBox(double[] zoomBox) {
		this.zoomBox = zoomBox;
	}

	//	 Methods: to be done
}
