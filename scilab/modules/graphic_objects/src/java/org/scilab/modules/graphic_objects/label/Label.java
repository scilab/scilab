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

package org.scilab.modules.graphic_objects.label;

import org.scilab.modules.graphic_objects.graphicObject.IVisitor;
import org.scilab.modules.graphic_objects.textObject.TextObject;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Label class
 * @author Manuel JULIACHS
 */
public class Label extends TextObject {
	/** Label properties names */
	public enum LabelProperty { FONTANGLE, AUTOPOSITION, POSITION, AUTOROTATION };
	
	/** Text orientation */
	private double fontAngle;
	
	 /** Specifies whether the label position is automatically computed or not */
	private boolean autoPosition;

	/** 3-element array: (x, y, z) position */
	private double[] position;

	/** Specifies whether the label orientation is automatically computed or not */
	private boolean autoRotation;

	/** Constructor */
	public Label() {
		super();
		fontAngle = 0.0;
		autoPosition = false;
		position = new double[3];
		autoRotation = false;

		Integer[] textDimensions = {1, 1};
		String[] textStrings = {""};

		setTextArrayDimensions(textDimensions);
		setTextStrings(textStrings);
	}

    @Override
    public void accept(IVisitor visitor) {
        visitor.visit(this);
    }

    /**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals(__GO_FONT_ANGLE__)) {
			return LabelProperty.FONTANGLE;
		} else if (propertyName.equals(__GO_AUTO_POSITION__)) {
			return LabelProperty.AUTOPOSITION;
		} else if (propertyName.equals(__GO_POSITION__)) {
			return LabelProperty.POSITION;
		} else if (propertyName.equals(__GO_AUTO_ROTATION__)) {
			return LabelProperty.AUTOROTATION;
		} else {
			return super.getPropertyFromName(propertyName);
		}
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getProperty(Object property) {
		if (property == LabelProperty.FONTANGLE) {
			return getFontAngle();
		} else if (property == LabelProperty.AUTOPOSITION) {
			return getAutoPosition();
		} else if (property == LabelProperty.POSITION) {
			return getPosition();
		} else if (property == LabelProperty.AUTOROTATION) {
			return getAutoRotation();
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
	public boolean setProperty(Object property, Object value) {
		if (property == LabelProperty.FONTANGLE) {
			setFontAngle((Double) value);
		} else if (property == LabelProperty.AUTOPOSITION) {
			setAutoPosition((Boolean) value);
		} else if (property == LabelProperty.POSITION) {
			setPosition((Double[]) value);
		} else if (property == LabelProperty.AUTOROTATION) {
			setAutoRotation((Boolean) value);
		} else {
			return super.setProperty(property, value);
		}

		return true;
	}
	
	/**
	 * @return the autoPosition
	 */
	public Boolean getAutoPosition() {
		return autoPosition;
	}

	/**
	 * @param autoPosition the autoPosition to set
	 */
	public void setAutoPosition(Boolean autoPosition) {
		this.autoPosition = autoPosition;
	}

	/**
	 * @return the autoRotation
	 */
	public Boolean getAutoRotation() {
		return autoRotation;
	}

	/**
	 * @param autoRotation the autoRotation to set
	 */
	public void setAutoRotation(Boolean autoRotation) {
		this.autoRotation = autoRotation;
	}

	/**
	 * @return the fontAngle
	 */
	public Double getFontAngle() {
		return fontAngle;
	}

	/**
	 * @param fontAngle the fontAngle to set
	 */
	public void setFontAngle(Double fontAngle) {
		if (autoRotation == true) {
			autoRotation = false;
		}

		this.fontAngle = fontAngle;
	}

	/**
	 * @return the position
	 */
	public Double[] getPosition() {
		Double[] retPosition = new Double[3];

		retPosition[0] = position[0];
		retPosition[1] = position[1];
		retPosition[2] = position[2];

		return retPosition;
	}

	/**
	 * @param position the position to set
	 */
	public void setPosition(Double[] position) {
		if (autoPosition == true) {
			autoPosition = false;
		}

		this.position[0] = position[0];
		this.position[1] = position[1];
		this.position[2] = position[2];
	}

	/**
	 * @return Type as String
	 */
	public String getType() {
		return "Label";
	}

}
