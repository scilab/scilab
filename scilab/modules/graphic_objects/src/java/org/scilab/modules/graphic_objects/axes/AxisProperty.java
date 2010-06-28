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

package org.scilab.modules.graphic_objects.axes;

import java.util.ArrayList;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.textObject.FormattedText;


/**
 * Axis property class
 * @author Manuel JULIACHS
 */
public class AxisProperty {
	/**
	 * AxisProperty properties names
	 */
	public enum AxisPropertyProperty { VISIBLE, REVERSE, GRIDCOLOR, LABEL, AXISLOCATION,
		LOGFLAG, TICKS, UNKNOWNPROPERTY };

	/**
	 * Axis location type
	 * BOTTOM, TOP, MIDDLE, ORIGIN are allowed for an x-axis,
	 * LEFT, RIGHT, MIDDLE or ORIGIN are allowed for a y-axis
	 */
	public static enum AxisLocation { BOTTOM, TOP, MIDDLE, ORIGIN, LEFT, RIGHT };
	
	/** Specifies whether the axis is visible or not */
	private boolean visible;
	
	/** Specifies whether the axis is reversed or not */
	private boolean reverse;
	
	/** Grid color */
	private int gridColor;

	/** Axis label */
	private Label label;

	/** Axis location */
	private AxisLocation axisLocation;

	/** Specifies whether logarithmic coordinates are used or not */
	private boolean logFlag;

	/** Ticks property */
	private TicksProperty ticks;

	/** Constructor */
	public AxisProperty() {
		visible = false;
		reverse = false;
		gridColor = 0;
		label = null;
		axisLocation = null;
		ticks = new TicksProperty();
		logFlag = false;
	}
	
	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("Visible")) {
			return AxisPropertyProperty.VISIBLE;
		} else if (propertyName.equals("Reverse")) {
			return AxisPropertyProperty.REVERSE;
		} else if (propertyName.equals("GridColor")) {
			return AxisPropertyProperty.GRIDCOLOR;
		} else if (propertyName.equals("Label")) {
			return AxisPropertyProperty.LABEL;
		} else if (propertyName.equals("AxisLocation")) {
			return AxisPropertyProperty.AXISLOCATION;
		} else if (propertyName.equals("LogFlag")) {
			return AxisPropertyProperty.LOGFLAG;
		} else if (propertyName.equals("Ticks")) {
			return AxisPropertyProperty.TICKS;
		} else {
			return AxisPropertyProperty.UNKNOWNPROPERTY;
		}
	}
	
	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getPropertyFast(Object property) {
		if (property == AxisPropertyProperty.VISIBLE) {
			return getVisible();
		} else if (property == AxisPropertyProperty.REVERSE) {
			return getReverse();
		} else if (property == AxisPropertyProperty.GRIDCOLOR) {
			return getGridColor();
		} else if (property == AxisPropertyProperty.LABEL) {
			return getLabel();
		} else if (property == AxisPropertyProperty.AXISLOCATION) {
			return getAxisLocation();
		} else if (property == AxisPropertyProperty.LOGFLAG) {
			return getLogFlag();
		} else if (property == AxisPropertyProperty.TICKS) {
			return getTicks();
		} else {
			return null;
		}
	}

	/**
	 * Fast property set method
	 * @param property the property to set
	 * @param value the property value
	 * @return true if the property has been set, false otherwise
	 */
	public boolean setPropertyFast(Object property, Object value) {
		if (property == AxisPropertyProperty.VISIBLE) {
			setVisible((Boolean) value);
		} else if (property == AxisPropertyProperty.REVERSE) {
			setReverse((Boolean) value);
		} else if (property == AxisPropertyProperty.GRIDCOLOR) {
			setGridColor((Integer) value);
		} else if (property == AxisPropertyProperty.LABEL) {
			setLabel((Label) value);
		} else if (property == AxisPropertyProperty.AXISLOCATION) {
			setAxisLocation((AxisLocation) value);
		} else if (property == AxisPropertyProperty.LOGFLAG) {
			setLogFlag((Boolean) value);
		} else if (property == AxisPropertyProperty.TICKS) {
			setTicks((TicksProperty) value);
		}

		return true;
	}
	
	/**
	 * @return the axisLocation
	 */
	public AxisLocation getAxisLocation() {
		return axisLocation;
	}

	/**
	 * @param axisLocation the axisLocation to set
	 */
	public void setAxisLocation(AxisLocation axisLocation) {
		this.axisLocation = axisLocation;
	}

	/**
	 * @return the gridColor
	 */
	public Integer getGridColor() {
		return gridColor;
	}

	/**
	 * @param gridColor the gridColor to set
	 */
	public void setGridColor(Integer gridColor) {
		this.gridColor = gridColor;
	}

	/**
	 * @return the label
	 */
	public Label getLabel() {
		return label;
	}

	/**
	 * @param label the label to set
	 */
	public void setLabel(Label label) {
		this.label = label;
	}

	/**
	 * @return the logFlag
	 */
	public Boolean getLogFlag() {
		return logFlag;
	}

	/**
	 * @param logFlag the logFlag to set
	 */
	public void setLogFlag(Boolean logFlag) {
		this.logFlag = logFlag;
	}

	/**
	 * @return the reverse
	 */
	public Boolean getReverse() {
		return reverse;
	}

	/**
	 * @param reverse the reverse to set
	 */
	public void setReverse(Boolean reverse) {
		this.reverse = reverse;
	}

	/**
	 * @return the ticks
	 */
	public TicksProperty getTicks() {
		return ticks;
	}

	/**
	 * @param ticks the ticks to set
	 */
	public void setTicks(TicksProperty ticks) {
		this.ticks = ticks;
	}

	/**
	 * @return the visible
	 */
	public Boolean getVisible() {
		return visible;
	}

	/**
	 * @param visible the visible to set
	 */
	public void setVisible(Boolean visible) {
		this.visible = visible;
	}

	/**
	 * @return the autoticks
	 */
	public Boolean getAutoTicks() {
		return ticks.getAuto();
	}

	/**
	 * @param autoticks the autoticks to set
	 */
	public void setAutoTicks(Boolean autoticks) {
		ticks.setAuto(autoticks);
	}

	/**
	 * @return the ticks locations
	 */
	public Double[] getTicksLocations() {
		return ticks.getLocations();
	}

	/**
	 * @param ticksLocations the ticks locations to set
	 */
	public void setTicksLocations(Double[] ticksLocations) {
		ticks.setLocations(ticksLocations);
	}

	/**
	 * @return the ticks labels
	 */
	public ArrayList<FormattedText> getTicksLabels() {
		return ticks.getLabels();
	}

	/**
	 * @param labels the labels to set
	 */
	public void setTicksLabels(ArrayList<FormattedText> labels) {
		ticks.setLabels(labels);
	}

	/**
	 * @return the number of subticks
	 */
	public Integer getSubticks() {
		return ticks.getSubticks();
	}

	/**
	 * @param subticks the number of subticks to set
	 */
	public void setSubticks(Integer subticks) {
		ticks.setSubticks(subticks);
	}

}
