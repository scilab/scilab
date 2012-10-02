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
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Surface class
 * @author Manuel JULIACHS
 */
public abstract class Surface extends ClippableContouredObject {
	// TBD Data -> Data Model
	/* TBD: properties relative to the data model */
	/** Surface properties names */
	private enum SurfaceProperty { SURFACEMODE, COLORMODE, COLORFLAG, HIDDENCOLOR };

	/** Specifies whether the surface is drawn or not */
	private boolean surfaceMode;

	/** Specifies the front face color (when colorFlag == 0) */
	private int colorMode;

	/** Specifies how facet colors are computed */
	private int colorFlag;

	/** Back-facing facets color */
	private int hiddenColor;

	/** Constructor */	
	public Surface() {
		super();
		surfaceMode = false;
		colorMode = 0;
		colorFlag = 0;
		hiddenColor = 0;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(int propertyName) {
		switch (propertyName) {
		case __GO_SURFACE_MODE__ :
			return SurfaceProperty.SURFACEMODE;
		case __GO_COLOR_MODE__ :
			return SurfaceProperty.COLORMODE;
		case __GO_COLOR_FLAG__ :
			return SurfaceProperty.COLORFLAG;
		case __GO_HIDDEN_COLOR__ :
			return SurfaceProperty.HIDDENCOLOR;
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
		if (property == SurfaceProperty.SURFACEMODE) {
			return getSurfaceMode();
		} else if (property == SurfaceProperty.COLORMODE) {
			return getColorMode();
		} else if (property == SurfaceProperty.COLORFLAG) {
			return getColorFlag();
		} else if (property == SurfaceProperty.HIDDENCOLOR) {
			return getHiddenColor();
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

		if (property == SurfaceProperty.SURFACEMODE) {
			setSurfaceMode((Boolean) value);
		} else if (property == SurfaceProperty.COLORMODE) {
			setColorMode((Integer) value);
		} else if (property == SurfaceProperty.COLORFLAG) {
			setColorFlag((Integer) value);
		} else if (property == SurfaceProperty.HIDDENCOLOR) {
			setHiddenColor((Integer) value);
		} else {
			return super.setProperty(property, value);
		}

		return UpdateStatus.Success;
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
	 * @return the hiddenColor
	 */
	public Integer getHiddenColor() {
		return hiddenColor;
	}

	/**
	 * @param hiddenColor the hiddenColor to set
	 */
	public void setHiddenColor(Integer hiddenColor) {
		this.hiddenColor = hiddenColor;
	}

}
