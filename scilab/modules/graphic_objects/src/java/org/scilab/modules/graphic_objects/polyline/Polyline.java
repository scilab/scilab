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

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Polyline class
 * @author Manuel JULIACHS
 */
public class Polyline extends ClippableContouredObject {
	/** TBD: data */
	// Data data -> Data Model
	/* TBD: properties relative to the data model */
	/** Polyline properties names */
	private enum PolylineProperty { CLOSED, ARROWSIZEFACTOR, POLYLINESTYLE,
		INTERPCOLORVECTOR, INTERPCOLORVECTORSET, INTERPCOLORMODE,
		XSHIFT, YSHIFT, ZSHIFT, BARWIDTH };

	/** Specifies whether the polyline is closed */
	private boolean closed;
	
	/** Determines the arrow size */
	private double arrowSizeFactor;

	/** Polyline drawing style (normal, staircase, bar, etc.) */
	private int polylineStyle;
	
	/** Interpolation color vector (3- or 4-element array) */
	private int[] interpColorVector;

	/** Specifies whether the interpolation color vector has been set */
	private boolean interpColorVectorSet;

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
		arrowSizeFactor = 1.0;
		polylineStyle = 1;
		interpColorVector = new int[4];
		interpColorVectorSet = false;
		interpColorMode = false;
		xShift = null;
		yShift = null;
		zShift = null;
		barWidth = 0.0;
	}

    @Override
    public void accept(Visitor visitor) throws ObjectRemovedException {
        visitor.visit(this);
    }

    /**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(int propertyName) {
		switch (propertyName) { 
		case __GO_CLOSED__ :
			return PolylineProperty.CLOSED;
		case __GO_ARROW_SIZE_FACTOR__ :
			return PolylineProperty.ARROWSIZEFACTOR;
		case __GO_POLYLINE_STYLE__ :
			return PolylineProperty.POLYLINESTYLE;
		case __GO_INTERP_COLOR_VECTOR__ :
			return PolylineProperty.INTERPCOLORVECTOR;
		case __GO_INTERP_COLOR_VECTOR_SET__ :
			return PolylineProperty.INTERPCOLORVECTORSET;
		case __GO_INTERP_COLOR_MODE__ :
			return PolylineProperty.INTERPCOLORMODE;
		case __GO_X_SHIFT__ :
			return PolylineProperty.XSHIFT;
		case __GO_Y_SHIFT__ :
			return PolylineProperty.YSHIFT;
		case __GO_Z_SHIFT__ :
			return PolylineProperty.ZSHIFT;
		case __GO_BAR_WIDTH__ :
			return PolylineProperty.BARWIDTH;
		default :
			return super.getPropertyFromName(propertyName);
		}
	}	

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getProperty(Object property) {
		if (property == PolylineProperty.CLOSED) {
			return getClosed();
		} else if (property == PolylineProperty.ARROWSIZEFACTOR) {
			return getArrowSizeFactor();
		} else if (property == PolylineProperty.POLYLINESTYLE) {
			return getPolylineStyle();
		} else if (property == PolylineProperty.INTERPCOLORVECTOR) {
			return getInterpColorVector();
		} else if (property == PolylineProperty.INTERPCOLORVECTORSET) {
			return getInterpColorVectorSet();
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
			return super.getProperty(property);	
		}
	}

	/**
	 * Fast property set method
	 * @param property the property to set
	 * @param value the property value
	 * @return true if the property has been set, false otherwise
	 */
	public UpdateStatus setProperty(Object property, Object value) {
		synchronized (this) {
	    if (property == PolylineProperty.CLOSED) {
			setClosed((Boolean) value);
		} else if (property == PolylineProperty.ARROWSIZEFACTOR) {
			return setArrowSizeFactor((Double) value);
		} else if (property == PolylineProperty.POLYLINESTYLE) {
			return setPolylineStyle((Integer) value);
		} else if (property == PolylineProperty.INTERPCOLORVECTOR) {
			setInterpColorVector((Integer[]) value);
		} else if (property == PolylineProperty.INTERPCOLORVECTORSET) {
			setInterpColorVectorSet((Boolean) value);
		} else if (property == PolylineProperty.INTERPCOLORMODE) {
			return setInterpColorMode((Boolean) value);
		} else if (property == PolylineProperty.XSHIFT) {
			setXShift((double[]) value);
		} else if (property == PolylineProperty.YSHIFT) {
			setYShift((double[]) value);
		} else if (property == PolylineProperty.ZSHIFT) {
			setZShift((double[]) value);
		} else if (property == PolylineProperty.BARWIDTH) {
			setBarWidth((Double) value);
		} else {
			return super.setProperty(property, value);
		}
		return UpdateStatus.Success;
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
	public UpdateStatus setArrowSizeFactor(Double arrowSizeFactor) {
		if (this.arrowSizeFactor == arrowSizeFactor) {
		    return UpdateStatus.NoChange;
		}
		this.arrowSizeFactor = arrowSizeFactor;
		return UpdateStatus.Success;
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
	public UpdateStatus setInterpColorMode(Boolean interpColorMode) {
		if (this.interpColorMode == interpColorMode) {
		    return UpdateStatus.NoChange;
		}
		this.interpColorMode = interpColorMode;
		return UpdateStatus.Success;
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
		if (interpColorVectorSet == false) {
			interpColorVectorSet = true;
		}

		for(int i = 0; i < interpColorVector.length; i++) {
			this.interpColorVector[i] = interpColorVector[i];
		}
	}

	/**
	 * @return the interpColorVectorSet
	 */
	public Boolean getInterpColorVectorSet() {
		return interpColorVectorSet;
	}

	/**
	 * @param interpColorVectorSet the interpColorVectorSet to set
	 */
	public void setInterpColorVectorSet(Boolean interpColorVectorSet) {
		this.interpColorVectorSet = interpColorVectorSet;
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
	public UpdateStatus setPolylineStyle(Integer polylineStyle) {
		if (this.polylineStyle == polylineStyle) {
		    return UpdateStatus.NoChange;
		}
		this.polylineStyle = polylineStyle;
		return UpdateStatus.Success;
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

	/**
	 * @return Type as String
	 */
	public Integer getType() {
		return GraphicObjectProperties.__GO_POLYLINE__;
	}

}
