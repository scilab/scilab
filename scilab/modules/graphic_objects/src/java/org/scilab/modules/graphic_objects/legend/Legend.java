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

package org.scilab.modules.graphic_objects.legend;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.graphic_objects.textObject.ClippableTextObject;

/**
 * Legend class
 * @author Manuel JULIACHS
 */
public class Legend extends ClippableTextObject {
	/** Legend properties names */
	private enum LegendProperty { LINKS, LEGENDLOCATION, POSITION };
	
	/** Legend location */
	private enum LegendLocation { IN_UPPER_RIGHT, IN_UPPER_LEFT, IN_LOWER_RIGHT, IN_LOWER_LEFT,
		OUT_UPPER_RIGHT, OUT_UPPER_LEFT, OUT_LOWER_RIGHT, OUT_LOWER_LEFT,
		UPPER_CAPTION, LOWER_CAPTION, BY_COORDINATES };
	
	/** List of the polylines referred to */
	private ArrayList <Polyline> links;

	/** Legend location */
	private LegendLocation legendLocation;

	/** 2D position relative to the parent axes bounds */
	private double[] position;

	/** Constructor */
	public Legend() {
		super();
		this.links = null;
		this.legendLocation = null;
		position = new double[2];
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("Links")) {
			return LegendProperty.LINKS;
		} else if (propertyName.equals("LegendLocation")) {
			return LegendProperty.LEGENDLOCATION;
		} else if (propertyName.equals("Position")) {
			return LegendProperty.POSITION;
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
		if (property == LegendProperty.LINKS) {
			return getLinks();
		} else if (property == LegendProperty.LEGENDLOCATION) {
			return getLegendLocation();
		} else if (property == LegendProperty.POSITION) {
			return getPosition();
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
		if (property == LegendProperty.LINKS) {
			setLinks((ArrayList<Polyline>) value);
		} else if (property == LegendProperty.LEGENDLOCATION) {
			setLegendLocation((LegendLocation) value);
		} else if (property == LegendProperty.POSITION) {
			setPosition((Double[]) value);
		} else {
			return super.setPropertyFast(property, value);
		}

		return true;
	}

	/**
	 * @return the legendLocation
	 */
	public LegendLocation getLegendLocation() {
		return legendLocation;
	}

	/**
	 * @param legendLocation the legendLocation to set
	 */
	public void setLegendLocation(LegendLocation legendLocation) {
		this.legendLocation = legendLocation;
	}

	/**
	 * @return the links
	 */
	public ArrayList<Polyline> getLinks() {
		return links;
	}

	/**
	 * @param links the links to set
	 */
	public void setLinks(ArrayList<Polyline> links) {
		this.links = links;
	}

	/**
	 * @return the position
	 */
	public Double[] getPosition() {
		Double[] retPosition = new Double[2];
		retPosition[0] = position[0];
		retPosition[1] = position[1];

		return retPosition;
	}

	/**
	 * @param position the position to set
	 */
	public void setPosition(Double[] position) {
		this.position[0] = position[0];
		this.position[1] = position[1];
	}

}
