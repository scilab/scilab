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

package org.scilab.modules.graphic_objects.contouredObject;

import org.scilab.modules.graphic_objects.contouredObject.Line.LinePropertyType;
import org.scilab.modules.graphic_objects.contouredObject.Line.LineType;
import org.scilab.modules.graphic_objects.contouredObject.Mark.MarkPropertyType;
import org.scilab.modules.graphic_objects.contouredObject.Mark.MarkSizeUnitType;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Contoured object class 
 * @author Manuel JULIACHS
 */
public abstract class ContouredObject extends GraphicObject {
	/** ContouredObject properties */
	public enum ContouredObjectPropertyType { LINE, FILLMODE, BACKGROUND, MARK };
	
	/** Line property */
	private Line line;

	/** Specifies whether the object is filled or not */
	private boolean fillMode;

	/** Background (fill) color */
	private int background;

	/** Mark property */
	private Mark mark;

	/** Default constructor */
	public ContouredObject() {
		line = new Line();
		fillMode = false;
		background = 0;
		mark = new Mark();
	}

	public ContouredObject clone() {
		ContouredObject copy = (ContouredObject) super.clone();

		copy.line = new Line(line);
		copy.mark = new Mark(mark);

		return copy;
	}
	
	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(int propertyName) {
		switch (propertyName) {
		case __GO_LINE__ :
			return ContouredObjectPropertyType.LINE;
		case __GO_LINE_MODE__ :
			return LinePropertyType.MODE;
		case __GO_LINE_STYLE__ :
			return LinePropertyType.LINESTYLE;
		case __GO_LINE_THICKNESS__ :
			return LinePropertyType.THICKNESS;
		case __GO_LINE_COLOR__ :
			return LinePropertyType.COLOR;
		case __GO_FILL_MODE__ :
			return ContouredObjectPropertyType.FILLMODE;
		case __GO_BACKGROUND__ :
			return ContouredObjectPropertyType.BACKGROUND;
		case __GO_MARK__ :
			return ContouredObjectPropertyType.MARK;
		case __GO_MARK_MODE__ :
			return MarkPropertyType.MODE;
		case __GO_MARK_STYLE__ :
			return MarkPropertyType.STYLE;
		case __GO_MARK_SIZE_UNIT__ :
			return MarkPropertyType.SIZEUNIT;
		case __GO_MARK_SIZE__ :
			return MarkPropertyType.SIZE;
		case __GO_MARK_FOREGROUND__ :
			return MarkPropertyType.FOREGROUND;
		case __GO_MARK_BACKGROUND__ :
			return MarkPropertyType.BACKGROUND;
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
		if (property == ContouredObjectPropertyType.LINE) {
			return getLine();
		} else if (property == LinePropertyType.MODE) {
			return getLineMode();
		} else if (property == LinePropertyType.LINESTYLE) {
			return getLineStyle();
		} else if (property == LinePropertyType.THICKNESS) {
			return getLineThickness();
		} else if (property == LinePropertyType.COLOR) {
			return getLineColor();
		} else if (property == ContouredObjectPropertyType.FILLMODE) {
			return getFillMode();
		} else if (property == ContouredObjectPropertyType.BACKGROUND) {
			return getBackground();
		} else if (property == ContouredObjectPropertyType.MARK) {
			return getMark();
		} else if (property == MarkPropertyType.MODE) {
			return getMarkMode();
		} else if (property == MarkPropertyType.STYLE) {
			return getMarkStyle();
		} else if (property == MarkPropertyType.SIZEUNIT) {
			return getMarkSizeUnit();
		} else if (property == MarkPropertyType.SIZE) {
			return getMarkSize();
		} else if (property == MarkPropertyType.FOREGROUND) {
			return getMarkForeground();
		} else if (property == MarkPropertyType.BACKGROUND) {
			return getMarkBackground();
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
		if (property == ContouredObjectPropertyType.LINE) {
			setLine((Line) value);
		} else if (property == LinePropertyType.MODE) {
			return setLineMode((Boolean) value);
		} else if (property == LinePropertyType.LINESTYLE) {
			setLineStyle((Integer) value);
		} else if (property == LinePropertyType.THICKNESS) {
			setLineThickness((Double) value);
		} else if (property == LinePropertyType.COLOR) {
			return setLineColor((Integer) value);
		} else if (property == ContouredObjectPropertyType.FILLMODE) {
			return setFillMode((Boolean) value);
		} else if (property == ContouredObjectPropertyType.BACKGROUND) {
			setBackground((Integer) value);
		} else if (property == ContouredObjectPropertyType.MARK) {
			setMark((Mark) value);
		} else if (property == MarkPropertyType.MODE) {
			return setMarkMode((Boolean) value);
		} else if (property == MarkPropertyType.STYLE) {
			return setMarkStyle((Integer) value);
		} else if (property == MarkPropertyType.SIZEUNIT) {
			setMarkSizeUnit((Integer) value);
		} else if (property == MarkPropertyType.SIZE) {
			return setMarkSize((Integer) value);
		} else if (property == MarkPropertyType.FOREGROUND) {
			this.setMarkForeground((Integer) value);
		} else if (property == MarkPropertyType.BACKGROUND) {
			this.setMarkBackground((Integer) value);
		} else {
			return super.setProperty(property, value);
		}

		return UpdateStatus.Success;
	}

	/**
	 * @return the background
	 */
	public Integer getBackground() {
		return background;
	}

	/**
	 * @param background the background to set
	 */
	public void setBackground(Integer background) {
		this.background = background;
	}

	/**
	 * @return the fillMode
	 */
	public Boolean getFillMode() {
		return fillMode;
	}

	/**
	 * @param fillMode the fillMode to set
	 */
	public UpdateStatus setFillMode(Boolean fillMode) {
		if (this.fillMode == fillMode) {
		    return UpdateStatus.NoChange;
		}
		this.fillMode = fillMode;
		return UpdateStatus.Success;
	}

	/**
	 * @return the line
	 */
	public Line getLine() {
		return line;
	}

	/**
	 * @param line the line to set
	 */
	public void setLine(Line line) {
		this.line = line;
	}

	/**
	 * Get the line color
	 * @return the color
	 */
	public Integer getLineColor() {
		return line.getColor();
	}

	/**
	 * Set the line color
	 * @param color the color to set
	 */
	public UpdateStatus setLineColor(Integer color) {
		return line.setColor(color);
	}

	/**
	 * Get the line style
	 * @return the lineStyle
	 */
	public Integer getLineStyle() {
		return getLineStyleAsEnum().asScilabIndex();
	}

	/**
	 * Get the line style
	 * @return the lineStyle
	 */
	public LineType getLineStyleAsEnum() {
		return line.getLineStyle();
	}

	/**
	 * Set the line style
	 * @param lineStyle the lineStyle to set
	 */
	public void setLineStyle(Integer lineStyle) {
		setLineStyleAsEnum(LineType.fromScilabIndex(lineStyle));
	}

	/**
	 * Set the line style
	 * @param lineStyle the lineStyle to set
	 */
	public void setLineStyleAsEnum(LineType lineStyle) {
		line.setLineStyle(lineStyle);
	}

	/**
	 * Return the line mode
	 * @return the mode
	 */
	public Boolean getLineMode() {
		return line.getMode();
	}

	/**
	 * Set the line mode
	 * @param mode the mode to set
	 */
	public UpdateStatus setLineMode(Boolean mode) {
		return line.setMode(mode);
	}

	/**
	 * Get the line thickness
	 * @return the thickness
	 */
	public Double getLineThickness() {
		return line.getThickness();
	}

	/**
	 * Set the line thickness
	 * @param thickness the thickness to set
	 */
	public void setLineThickness(Double thickness) {
		line.setThickness(thickness);
	}

	/**
	 * @return the mark
	 */
	public Mark getMark() {
		return mark;
	}

	/**
	 * @param mark the mark to set
	 */
	public void setMark(Mark mark) {
		this.mark = mark;
	}

	/**
	 * Get the mark background
	 * @return the background
	 */
	public Integer getMarkBackground() {
		return mark.getBackground();
	}

	/**
	 * Set the mark background
	 * @param background the background to set
	 */
	public void setMarkBackground(Integer background) {
		mark.setBackground(background);
	}

	/**
	 * Get the mark foreground
	 * @return the foreground
	 */
	public Integer getMarkForeground() {
		return mark.getForeground();
	}

	/**
	 * Set the mark foreground
	 * @param foreground the foreground to set
	 */
	public void setMarkForeground(Integer foreground) {
		mark.setForeground(foreground);
	}

	/**
	 * Get the mark size unit
	 * @return the markSizeUnit
	 */
	public Integer getMarkSizeUnit() {
		return getMarkSizeUnitAsEnum().ordinal();
	}

	/**
	 * Get the mark size unit
	 * @return the markSizeUnit
	 */
	public MarkSizeUnitType getMarkSizeUnitAsEnum() {
		return mark.getMarkSizeUnit();
	}

	/**
	 * Set the mark size unit
	 * @param markSizeUnit the markSizeUnit to set
	 */
	public void setMarkSizeUnit(Integer markSizeUnit) {
		setMarkSizeUnitAsEnum(MarkSizeUnitType.intToEnum(markSizeUnit));
	}

	/**
	 * Set the mark size unit
	 * @param markSizeUnit the markSizeUnit to set
	 */
	public void setMarkSizeUnitAsEnum(MarkSizeUnitType markSizeUnit) {
		mark.setMarkSizeUnit(markSizeUnit);
	}

	/**
	 * Get the mark size
	 * @return the size
	 */
	public Integer getMarkSize() {
		return mark.getSize();
	}

	/**
	 * Set the mark size
	 * @param size the size to set
	 */
	public UpdateStatus setMarkSize(Integer size) {
		return mark.setSize(size);
	}

	/**
	 * Get the mark mode
	 * @return the mode
	 */
	public Boolean getMarkMode() {
		return mark.getMode();
	}

	/**
	 * Set the mark mode
	 * @param mode the mode to set
	 */
	public UpdateStatus setMarkMode(Boolean mode) {
		return mark.setMode(mode);
	}

	/**
	 * Get the mark style
	 * @return the style
	 */
	public Integer getMarkStyle() {
		return mark.getStyle();
	}

	/**
	 * Set the mark style
	 * @param style the style to set
	 */
	public UpdateStatus setMarkStyle(Integer style) {
		return mark.setStyle(style);
	}

}
