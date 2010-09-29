/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.axes;

/**
 * Box class
 * @author Manuel JULIACHS
 */
public class Box {
	/** Box properties names */
	public enum BoxProperty { BOX, HIDDENAXISCOLOR, TIGHTLIMITS, DATABOUNDS, REALDATABOUNDS, ZOOMENABLED, ZOOMBOX, AUTOSCALE, FIRSTPLOT };

	/** Box type */
	public static enum BoxType { OFF, ON, HIDDEN_AXES, BACK_HALF;

		/**
		 * Converts an integer to the corresponding enum
		 * @param intValue the integer value
		 * @return the box type enum
		 */
		public static BoxType intToEnum(Integer intValue) {
			switch (intValue) {
				case 0:
					return BoxType.OFF;
				case 1:
					return BoxType.ON;
				case 2:
					return BoxType.HIDDEN_AXES;
				case 3:
					return BoxType.BACK_HALF;
				default:
					return null;
			}
		}
	}

	/** Box type */
	private BoxType box;

	/** Hidden axis color */
	private int hiddenAxisColor;

	/** Specifies whether tight limits are enforced or not */
	private boolean tightLimits;

	/** Data bounding box (6-element array) */
	private double[] dataBounds;

	/** Data bounding box, as modified by automatic ticks computation (6-element array) */
	private double[] realDataBounds;

	/** Speficies whether zooming is enabled or not */
	private boolean zoomEnabled;

	/** Magnified 3D sub-region (6-element array) */
	private double[] zoomBox;

	/** Indicates whether data bounds are updated when a new plot command is executed */
	private boolean autoScale;

	/**
         * Indicates whether no high-level drawing function has yet been called (true) or
         * has been called at least once (false)
         */
	private boolean firstPlot;

	/** Constructor */
	public Box() {
		box = BoxType.OFF;
		hiddenAxisColor = 0;
		tightLimits = false;
		dataBounds = new double[6];
		realDataBounds = new double[6];
		zoomEnabled = false;
		zoomBox = new double[6];
		autoScale = false;
		firstPlot = true;
	}

	/**
	 * @return the autoScale
	 */
	public Boolean getAutoScale() {
		return autoScale;
	}

	/**
	 * @param autoScale the autoScale to set
	 */
	public void setAutoScale(Boolean autoScale) {
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
	public Double[] getDataBounds() {
		Double[] retDataBounds = new Double[6];

		for (int i = 0; i < retDataBounds.length; i++) {
			retDataBounds[i] = dataBounds[i];
		}

		return retDataBounds;
	}

	/**
	 * @param dataBounds the dataBounds to set
	 */
	public void setDataBounds(Double[] dataBounds) {
		for (int i = 0; i < this.dataBounds.length; i++) {
			this.dataBounds[i] = dataBounds[i];
		}
	}

	/**
	 * @return the hiddenAxisColor
	 */
	public Integer getHiddenAxisColor() {
		return hiddenAxisColor;
	}

	/**
	 * @param hiddenAxisColor the hiddenAxisColor to set
	 */
	public void setHiddenAxisColor(Integer hiddenAxisColor) {
		this.hiddenAxisColor = hiddenAxisColor;
	}

	/**
	 * @return the realDataBounds
	 */
	public Double[] getRealDataBounds() {
		Double[] retRealDataBounds = new Double[6];

		for (int i = 0; i < retRealDataBounds.length; i++) {
			retRealDataBounds[i] = realDataBounds[i];
		}

		return retRealDataBounds;
	}

	/**
	 * @param realDataBounds the realDataBounds to set
	 */
	public void setRealDataBounds(Double[] realDataBounds) {
		for (int i = 0; i < this.realDataBounds.length; i++) {
			this.realDataBounds[i] = realDataBounds[i];
		}
	}

	/**
	 * @return the tightLimits
	 */
	public Boolean getTightLimits() {
		return tightLimits;
	}

	/**
	 * @param tightLimits the tightLimits to set
	 */
	public void setTightLimits(Boolean tightLimits) {
		this.tightLimits = tightLimits;
	}

	/**
	 * @return the zoomEnabled
	 */
	public Boolean getZoomEnabled() {
		return zoomEnabled;
	}

	/**
	 * @param zoomEnabled the zoomEnabled to set
	 */
	public void setZoomEnabled(Boolean zoomEnabled) {
		this.zoomEnabled = zoomEnabled;
	}

	/**
	 * @return the zoomBox
	 */
	public Double[] getZoomBox() {
		Double[] retZoomBox = new Double[6];
 
		for (int i = 0; i < retZoomBox.length; i++) {
			retZoomBox[i] = zoomBox[i];
		}

		return retZoomBox;
	}

	/**
	 * @param zoomBox the zoomBox to set
	 */
	public void setZoomBox(Double[] zoomBox) {
		for (int i = 0; i < this.zoomBox.length; i++) {
			this.zoomBox[i] = zoomBox[i];
		}
	}

	/**
	 * @return the firstPlot
	 */
	public Boolean getFirstPlot() {
		return firstPlot;
	}

	/**
	 * @param firstPlot the firstPlot to set
	 */
	public void setFirstPlot(Boolean firstPlot) {
		this.firstPlot = firstPlot;
	}


}
