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

package org.scilab.modules.graphic_objects.axis;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.arc.Arc.ArcDrawingMethod;
import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;
import org.scilab.modules.graphic_objects.textObject.Font;
import org.scilab.modules.graphic_objects.textObject.Font.FontProperty;

/**
 * Axis class
 * @author Manuel JULIACHS
 */
public class Axis extends ClippableContouredObject {
	/** Axis properties */
	private enum AxisProperty { TICKSDIRECTION, XTICKSCOORDS, YTICKSCOORDS, TICKSCOLOR, TICKSSEGMENT, TICKSLABELS,
		FORMATN, FONT };
		
	/** Default number of ticks */
	private static final int DEFAULT_NUMBER_OF_TICKS = 10;

	/** Ticks direction */
	private enum TicksDirection { TOP, BOTTOM, LEFT, RIGHT };
	
	/** Ticks direction */
	private TicksDirection ticksDirection;

	/** Ticks x-coordinate position vector */
	private double[] xTicksCoords; 

	/** Ticks y-coordinate position vector */
	private double[] yTicksCoords;
	
	/** Ticks color */
	private int ticksColor;

	/** Specifies whether the axis segment is drawn */
	private boolean ticksSegment;

	/** Ticks labels list */
	private ArrayList <String> ticksLabels;

	/** Label format */
	private String formatn;

	/** Font */
	private Font font;

	/** Constructor */
	public Axis() {
		super();
		ticksDirection = TicksDirection.TOP;
		xTicksCoords = new double[DEFAULT_NUMBER_OF_TICKS];
		yTicksCoords = null;
		ticksColor = 0;
		ticksSegment = false;
		ticksLabels = new ArrayList<String>(DEFAULT_NUMBER_OF_TICKS);
		formatn = null;
		font = null;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("TicksDirection")) {
			return AxisProperty.TICKSDIRECTION;
		} else if (propertyName.equals("XTicksCoords")) {
			return AxisProperty.XTICKSCOORDS;
		} else if (propertyName.equals("YTicksCoords")) {
			return AxisProperty.YTICKSCOORDS;
		} else if (propertyName.equals("TicksColor")) {
			return AxisProperty.TICKSCOLOR;
		} else if (propertyName.equals("TicksSegment")) {
			return AxisProperty.TICKSSEGMENT;
		} else if (propertyName.equals("TicksLabels")) {
			return AxisProperty.TICKSLABELS;
		} else if (propertyName.equals("Formatn")) {
			return AxisProperty.FORMATN;
		} else if (propertyName.equals("Font")) {
			return AxisProperty.FONT;
		} else if (propertyName.equals("Style")) {
			return Font.FontProperty.STYLE;
		} else if (propertyName.equals("Size")) {
			return Font.FontProperty.SIZE;
		} else if (propertyName.equals("Color")) {
			return Font.FontProperty.COLOR;
		} else if (propertyName.equals("Fractional")) {
			return Font.FontProperty.FRACTIONAL;
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
		if (property == AxisProperty.TICKSDIRECTION) {
			return getTicksDirection();
		} else if (property == AxisProperty.XTICKSCOORDS) {
			return getXTicksCoords();
		} else if (property == AxisProperty.YTICKSCOORDS) {
			return getYTicksCoords();
		} else if (property == AxisProperty.TICKSCOLOR) {
			return getTicksColor();
		} else if (property == AxisProperty.TICKSSEGMENT) {
			return getTicksSegment();
		} else if (property == AxisProperty.TICKSLABELS) {
			return getTicksLabels();
		} else if (property == AxisProperty.FORMATN) {
			return getFormatn();
		} else if (property == AxisProperty.FONT) {
			return getFont();
		} else if (property == Font.FontProperty.STYLE) {
			return getStyle();
		} else if (property == Font.FontProperty.SIZE) {
			return getSize();
		} else if (property == Font.FontProperty.COLOR) {
			return getColor();
		} else if (property == Font.FontProperty.FRACTIONAL) {
			return getFractional();
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
		if (property == AxisProperty.TICKSDIRECTION) {
			setTicksDirection((TicksDirection) value);
		} else if (property == AxisProperty.XTICKSCOORDS) {
			setXTicksCoords((Double[]) value);
		} else if (property == AxisProperty.YTICKSCOORDS) {
			setYTicksCoords((Double[]) value);
		} else if (property == AxisProperty.TICKSCOLOR) {
			setTicksColor((Integer) value);
		} else if (property == AxisProperty.TICKSSEGMENT) {
			setTicksSegment((Boolean) value);
		} else if (property == AxisProperty.TICKSLABELS) {
			setTicksLabels((ArrayList<String>) value);
		} else if (property == AxisProperty.FORMATN) {
			setFormatn((String) value);
		} else if (property == AxisProperty.FONT) {
			setFont((Font) value);
		} else if (property == Font.FontProperty.STYLE) {
			setStyle((Integer) value);
		} else if (property == Font.FontProperty.SIZE) {
			setSize((Double) value);
		} else if (property == Font.FontProperty.COLOR) {
			setColor((Integer) value);
		} else if (property == Font.FontProperty.FRACTIONAL) {
			setFractional((Boolean) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the font
	 */
	public Font getFont() {
		return font;
	}

	/**
	 * @param font the font to set
	 */
	public void setFont(Font font) {
		this.font = font;
	}

	/**
	 * @return the font style
	 */
	public Integer getStyle() {
		return font.getStyle();
	}

	/**
	 * @param style the font style to set
	 */
	public void setStyle(Integer style) {
		font.setStyle(style);
	}

	/**
	 * @return the font size
	 */
	public Double getSize() {
		return font.getSize();
	}

	/**
	 * @param size the font size to set
	 */
	public void setSize(Double size) {
		font.setSize(size);
	}

	/**
	 * @return the font color
	 */
	public Integer getColor() {
		return font.getColor();
	}

	/**
	 * @param color the font color to set
	 */
	public void setColor(Integer color) {
		font.setColor(color);
	}

	/**
	 * @return the font fractional
	 */
	public Boolean getFractional() {
		return font.getFractional();
	}

	/**
	 * @param fractional the font fractional to set
	 */
	public void setFractional(Boolean fractional) {
		font.setFractional(fractional);
	}

	/**
	 * @return the formatn
	 */
	public String getFormatn() {
		return formatn;
	}

	/**
	 * @param formatn the formatn to set
	 */
	public void setFormatn(String formatn) {
		this.formatn = formatn;
	}

	/**
	 * @return the ticksColor
	 */
	public Integer getTicksColor() {
		return ticksColor;
	}

	/**
	 * @param ticksColor the ticksColor to set
	 */
	public void setTicksColor(Integer ticksColor) {
		this.ticksColor = ticksColor;
	}

	/**
	 * @return the ticksDirection
	 */
	public TicksDirection getTicksDirection() {
		return ticksDirection;
	}

	/**
	 * @param ticksDirection the ticksDirection to set
	 */
	public void setTicksDirection(TicksDirection ticksDirection) {
		this.ticksDirection = ticksDirection;
	}

	/**
	 * @return the ticksLabels
	 */
	public ArrayList<String> getTicksLabels() {
		return ticksLabels;
	}

	/**
	 * @param ticksLabels the ticksLabels to set
	 */
	public void setTicksLabels(ArrayList<String> ticksLabels) {
		this.ticksLabels = ticksLabels;
	}

	/**
	 * @return the ticksSegment
	 */
	public Boolean getTicksSegment() {
		return ticksSegment;
	}

	/**
	 * @param ticksSegment the ticksSegment to set
	 */
	public void setTicksSegment(Boolean ticksSegment) {
		this.ticksSegment = ticksSegment;
	}

	/**
	 * @return the xTicksCoords
	 */
	public Double[] getXTicksCoords() {
		Double[] retXTicksCoords = new Double[xTicksCoords.length];

		for (int i = 0; i < xTicksCoords.length; i++) {
			retXTicksCoords[i] = xTicksCoords[i];
		}

		return retXTicksCoords;
	}

	/**
	 * @param ticksCoords the xTicksCoords to set
	 */
	public void setXTicksCoords(Double[] ticksCoords) {
		for (int i = 0; i < xTicksCoords.length; i++) {
			xTicksCoords[i] = ticksCoords[i];
		}
	}

	/**
	 * @return the yTicksCoords
	 */
	public Double[] getYTicksCoords() {
		Double[] retYTicksCoords = new Double[yTicksCoords.length];

		for (int i = 0; i < yTicksCoords.length; i++) {
			retYTicksCoords[i] = yTicksCoords[i];
		}

		return retYTicksCoords;
	}

	/**
	 * @param ticksCoords the yTicksCoords to set
	 */
	public void setYTicksCoords(Double[] ticksCoords) {
		for (int i = 0; i < yTicksCoords.length; i++) {
			yTicksCoords[i] = ticksCoords[i];
		}
	}

}
