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

package org.scilab.modules.graphic_objects.polyline;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;

/**
 * Polyline class
 * @author Manuel JULIACHS
 */
public class Polyline extends ClippableContouredObject {
	/** TBD: data */
	// Data data -> Data Model
	/* TBD: properties relative to the data model */
	/** Polyline properties names */
	private enum PolylineProperty { CLOSED, ARROWSIZEFACTOR, POLYLINESTYLE, INTERPCOLORVECTOR, INTERPCOLORMODE,
		XSHIFT, YSHIFT, ZSHIFT, BARWIDTH };

	/** Specifies whether the polyline is closed */
	private boolean closed;
	
	/** Determines the arrow size */
	private double arrowSizeFactor;

	/** Polyline drawing style (normal, staircase, bar, etc.) */
	private int polylineStyle;
	
	/** Interpolation color vector (3- or 4-element array) */
	private int[] interpColorVector;

	/** Specifies whether interpolated shading is used */
	private boolean interpColorMode;

	/** Shift applied to the line points along the x-axis */
	private double[] xShift;

	/** Shift applied to the line points along the y-axis */
	private double[] yShift;

	/** Shift applied to the line points along the z-axis */
	private double[] zShift;

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
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("Closed")) {
			return PolylineProperty.CLOSED;
		} else if (propertyName.equals("ArrowSizeFactor")) {
			return PolylineProperty.ARROWSIZEFACTOR;
		} else if (propertyName.equals("PolylineStyle")) {
			return PolylineProperty.POLYLINESTYLE;
		} else if (propertyName.equals("InterpColorVector")) {
			return PolylineProperty.INTERPCOLORVECTOR;
		} else if (propertyName.equals("InterpColorMode")) {
			return PolylineProperty.INTERPCOLORMODE;
		} else if (propertyName.equals("XShift")) {
			return PolylineProperty.XSHIFT;
		} else if (propertyName.equals("YShift")) {
			return PolylineProperty.YSHIFT;
		} else if (propertyName.equals("ZShift")) {
			return PolylineProperty.ZSHIFT;
		} else if (propertyName.equals("BarWidth")) {
			return PolylineProperty.BARWIDTH;
		} else {
			return super.getPropertyFromName(propertyName);
		}
	}	

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getPropertyFast(Object property) {
		if (property == PolylineProperty.CLOSED) {
			return getClosed();
		} else if (property == PolylineProperty.ARROWSIZEFACTOR) {
			return getArrowSizeFactor();
		} else if (property == PolylineProperty.POLYLINESTYLE) {
			return getPolylineStyle();
		} else if (property == PolylineProperty.INTERPCOLORVECTOR) {
			return getInterpColorVector();
		} else if (property == PolylineProperty.INTERPCOLORMODE) {
			return getInterpColorMode();
		} else if (property == PolylineProperty.XSHIFT) {
			return getXShift();
		} else if (property == PolylineProperty.YSHIFT) {
			return getYShift();
		} else if (property == PolylineProperty.ZSHIFT) {
			return getZShift();
		} else if (property == PolylineProperty.BARWIDTH) {
			return getBarWidth();
		} else {
			return super.getPropertyFast(property);	
		}
	}

	/**
	 * Fast property set method
	 * @param property the property to set
	 * @param value the property value
	 */
	public void setPropertyFast(Object property, Object value) {
		if (property == PolylineProperty.CLOSED) {
			setClosed((Boolean) value);
		} else if (property == PolylineProperty.ARROWSIZEFACTOR) {
			setArrowSizeFactor((Double) value);
		} else if (property == PolylineProperty.POLYLINESTYLE) {
			setPolylineStyle((Integer) value);
		} else if (property == PolylineProperty.INTERPCOLORVECTOR) {
			setInterpColorVector((Integer[]) value);
		} else if (property == PolylineProperty.INTERPCOLORMODE) {
			setInterpColorMode((Boolean) value);
		} else if (property == PolylineProperty.XSHIFT) {
			setXShift((double[]) value);
		} else if (property == PolylineProperty.YSHIFT) {
			setYShift((double[]) value);
		} else if (property == PolylineProperty.ZSHIFT) {
			setZShift((double[]) value);
		} else if (property == PolylineProperty.BARWIDTH) {
			setBarWidth((Double) value);
		} else {
			super.getPropertyFast(property);	
		}
	}

	/**
	 * @return the arrowSizeFactor
	 */
	public Double getArrowSizeFactor() {
		return arrowSizeFactor;
	}

	/**
	 * @param arrowSizeFactor the arrowSizeFactor to set
	 */
	public void setArrowSizeFactor(Double arrowSizeFactor) {
		this.arrowSizeFactor = arrowSizeFactor;
	}

	/**
	 * @return the barWidth
	 */
	public Double getBarWidth() {
		return barWidth;
	}

	/**
	 * @param barWidth the barWidth to set
	 */
	public void setBarWidth(Double barWidth) {
		this.barWidth = barWidth;
	}

	/**
	 * @return the closed
	 */
	public Boolean getClosed() {
		return closed;
	}

	/**
	 * @param closed the closed to set
	 */
	public void setClosed(Boolean closed) {
		this.closed = closed;
	}

	/**
	 * @return the interpColorMode
	 */
	public Boolean getInterpColorMode() {
		return interpColorMode;
	}

	/**
	 * @param interpColorMode the interpColorMode to set
	 */
	public void setInterpColorMode(Boolean interpColorMode) {
		this.interpColorMode = interpColorMode;
	}

	/**
	 * @return the interpColorVector
	 */
	public Integer[] getInterpColorVector() {
		Integer[] retVector = new Integer[interpColorVector.length];
		for (int i = 0; i < interpColorVector.length; i++) {
			retVector[i] = interpColorVector[i];
		}

		return retVector;
	}

	/**
	 * @param interpColorVector the interpColorVector to set
	 */
	public void setInterpColorVector(Integer[] interpColorVector) {
		for(int i = 0; i < interpColorVector.length; i++) {
			this.interpColorVector[i] = interpColorVector[i];
		}
	}

	/**
	 * @return the polylineStyle
	 */
	public Integer getPolylineStyle() {
		return polylineStyle;
	}

	/**
	 * @param polylineStyle the polylineStyle to set
	 */
	public void setPolylineStyle(Integer polylineStyle) {
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
	
}
