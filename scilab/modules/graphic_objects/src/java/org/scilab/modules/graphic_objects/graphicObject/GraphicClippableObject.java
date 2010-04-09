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

package org.scilab.modules.graphic_objects.graphicObject;

import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty.ClipStateType;
import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty.ClippablePropertyType;

/**
 * GraphicClippableObject class 
 * @author Manuel JULIACHS
 */
public abstract class GraphicClippableObject extends GraphicObject {
	/** GraphicClippableObject properties names */
	private enum GraphicClippableObjectProperty { CLIPPROPERTY };

	/** Stores the actual clipping state */
	private ClippableProperty clipProperty;

	/** Default constructor */
	public GraphicClippableObject() {
		super();
		clipProperty = new ClippableProperty();
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("ClipProperty")) {
			return GraphicClippableObjectProperty.CLIPPROPERTY;
		} else if (propertyName.equals("ClipState")) {
			return ClippablePropertyType.CLIPSTATE;
		} else if (propertyName.equals("ClipBox")) {
			return ClippablePropertyType.CLIPBOX;
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
		if (property == GraphicClippableObjectProperty.CLIPPROPERTY) {
			return getClipProperty();
		} else if (property == ClippableProperty.ClippablePropertyType.CLIPSTATE) {
			return getClipState();
		} else if (property == ClippableProperty.ClippablePropertyType.CLIPBOX) {
			return getClipBox();
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
		if (property == GraphicClippableObjectProperty.CLIPPROPERTY) {
			setClipProperty((ClippableProperty) value);
		} else if (property == ClippableProperty.ClippablePropertyType.CLIPSTATE) {
			setClipState((ClipStateType) value);
		} else if (property == ClippableProperty.ClippablePropertyType.CLIPBOX) {
			setClipBox((Double[]) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the clipProperty
	 */
	public ClippableProperty getClipProperty() {
		return clipProperty;
	}

	/**
	 * @param clipProperty the clipProperty to set
	 */
	public void setClipProperty(ClippableProperty clipProperty) {
		this.clipProperty = clipProperty;
	}

	/**
	 * @return the clipBox
	 */
	public Double[] getClipBox() {
		return clipProperty.getClipBox();
	}

	/**
	 * @param clipBox the clipBox to set
	 */
	public void setClipBox(Double[] clipBox) {
		clipProperty.setClipBox(clipBox);
	}

	/**
	 * @return the clipState
	 */
	public ClipStateType getClipState() {
		return clipProperty.getClipState();
	}

	/**
	 * @param clipState the clipState to set
	 */
	public void setClipState(ClipStateType clipState) {
		clipProperty.setClipState(clipState);
	}

}
