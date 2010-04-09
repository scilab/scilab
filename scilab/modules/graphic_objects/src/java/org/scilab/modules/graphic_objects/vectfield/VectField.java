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

package org.scilab.modules.graphic_objects.vectfield;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.graphicObject.GraphicClippableObject;

/**
 * VectField class
 * @author Manuel JULIACHS
 */
public abstract class VectField extends GraphicClippableObject {
	/** VectField properties names */
	private enum VectFieldProperty { ARROWS };

	/** List of arrows composing the field */
	private ArrayList <Arrow> arrows;

	/** Constructor */
	public VectField() {
		super();
		arrows = null;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("Arrows")) {
			return VectFieldProperty.ARROWS;
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
		if (property == VectFieldProperty.ARROWS) {
			return getArrows();
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
		if (property == VectFieldProperty.ARROWS) {
			setArrows((ArrayList<Arrow>) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the arrows
	 */
	public ArrayList<Arrow> getArrows() {
		return arrows;
	}

	/**
	 * @param arrows the arrows to set
	 */
	public void setArrows(ArrayList<Arrow> arrows) {
		this.arrows = arrows;
	}

}
