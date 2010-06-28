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

package org.scilab.modules.graphic_objects.surface;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;

/**
 * Surface class
 * @author Manuel JULIACHS
 */
public abstract class Surface extends ClippableContouredObject {
	// TBD Data -> Data Model
	/* TBD: properties relative to the data model */
	/** Surface properties names */
	private enum SurfaceProperty { SURFACEMODE, COLORMODE, COLORFLAG };

	/** Specifies whether the surface is drawn or not */
	private boolean surfaceMode;

	/** Specifies the front face color (when colorFlag == 0) */
	private int colorMode;

	/** Specifies how facet colors are computed */
	private int colorFlag;

	/** Constructor */	
	public Surface() {
		super();
		surfaceMode = false;
		colorMode = 0;
		colorFlag = 0;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("SurfaceMode")) {
			return SurfaceProperty.SURFACEMODE;
		} else if (propertyName.equals("ColorMode")) {
			return SurfaceProperty.COLORMODE;
		} else if (propertyName.equals("ColorFlag")) {
			return SurfaceProperty.COLORFLAG;
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
		if (property == SurfaceProperty.SURFACEMODE) {
			return getSurfaceMode();
		} else if (property == SurfaceProperty.COLORMODE) {
			return getColorMode();
		} else if (property == SurfaceProperty.COLORFLAG) {
			return getColorFlag();
		} else {
			return super.getPropertyFast(property);	
		}
	}

	/**
	 * Fast property set method
	 * @param property the property to set
	 * @param value the property value
	 * @return true if the property has been set, false otherwise
	 */
	public boolean setPropertyFast(Object property, Object value) {

		if (property == SurfaceProperty.SURFACEMODE) {
			setSurfaceMode((Boolean) value);
		} else if (property == SurfaceProperty.COLORMODE) {
			setColorMode((Integer) value);
		} else if (property == SurfaceProperty.COLORFLAG) {
			setColorFlag((Integer) value);
		} else {
			return super.setPropertyFast(property, value);
		}

		return true;
	}	

	/**
	 * @return the colorFlag
	 */
	public Integer getColorFlag() {
		return colorFlag;
	}

	/**
	 * @param colorFlag the colorFlag to set
	 */
	public void setColorFlag(Integer colorFlag) {
		this.colorFlag = colorFlag;
	}

	/**
	 * @return the colorMode
	 */
	public Integer getColorMode() {
		return colorMode;
	}

	/**
	 * @param colorMode the colorMode to set
	 */
	public void setColorMode(Integer colorMode) {
		this.colorMode = colorMode;
	}

	/**
	 * @return the surfaceMode
	 */
	public Boolean getSurfaceMode() {
		return surfaceMode;
	}

	/**
	 * @param surfaceMode the surfaceMode to set
	 */
	public void setSurfaceMode(Boolean surfaceMode) {
		this.surfaceMode = surfaceMode;
	}

}
