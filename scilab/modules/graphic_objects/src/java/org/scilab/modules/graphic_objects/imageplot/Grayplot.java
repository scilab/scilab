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

package org.scilab.modules.graphic_objects.imageplot;

/**
 * Grayplot class
 * @author Manuel JULIACHS
 */
public class Grayplot extends Imageplot {
	/* TBD: properties relative to the data model */ 
	/** Grayplot properties names */
	private enum GrayplotProperty { DATAMAPPING };

	/** Data mapping */
	private enum DataMapping { SCALED, DIRECT };

	/** Specifies how colors are mapped to values */
	private DataMapping dataMapping;

	/** Constructor */
	public Grayplot() {
		super();
		dataMapping = DataMapping.DIRECT;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("DataMapping")) {
			return  GrayplotProperty.DATAMAPPING;
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
		if (property == GrayplotProperty.DATAMAPPING) {
			return getDataMapping();
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
		if (property == GrayplotProperty.DATAMAPPING) {
			setDataMapping((DataMapping) value);
		} else {
			return super.setPropertyFast(property, value);
		}

		return true;
	}

	/**
	 * @return the dataMapping
	 */
	public DataMapping getDataMapping() {
		return dataMapping;
	}

	/**
	 * @param dataMapping the dataMapping to set
	 */
	public void setDataMapping(DataMapping dataMapping) {
		this.dataMapping = dataMapping;
	}

}
