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


/**
 * Champ class
 * @author Manuel JULIACHS
 */
public class Champ extends VectField {
	/** Champ properties names */
	private enum ChampProperty { COLORED };

	/** Specifies whether vectors are colored or not */
	private boolean colored;

	/** Constructor */
	public Champ() {
		super();
		colored = false;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("Colored")) {
			return ChampProperty.COLORED;
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
		if (property == ChampProperty.COLORED) {
			return getColored();
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
		if (property == ChampProperty.COLORED) {
			setColored((Boolean) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the colored
	 */
	public Boolean getColored() {
		return colored;
	}

	/**
	 * @param colored the colored to set
	 */
	public void setColored(Boolean colored) {
		this.colored = colored;
	}

}
