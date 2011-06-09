/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.uicontrol;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTUNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_HORIZONTALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOXTOP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOXTOP_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RELIEF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDERSTEP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VERTICALALIGNMENT__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.IVisitor;

public class Uicontrol extends GraphicObject {


	private enum UicontrolProperty {
		STYLE,
		BACKGROUNDCOLOR,
		ENABLE,
		FONTANGLE,
		FONTNAME,
		FONTSIZE,
		FONTUNITS,
		FONTWEIGHT,
		FOREGROUNDCOLOR,
		HORIZONTALALIGNMENT,
		LISTBOXTOP,
		LISTBOXTOP_SIZE,
		MAX,
		MIN,
		POSITION,
		SLIDERSTEP,
		STRING,
		STRING_SIZE,
		RELIEF,
		UNITS,
		VALUE,
		VALUE_SIZE,
		VERTICALALIGNMENT
	};

	private enum UicontrolStyle {
		PUSHBUTTON
	};

	private String styleEnumToString(UicontrolStyle style) {
		if(style == UicontrolStyle.PUSHBUTTON) {
			return __GO_UI_PUSHBUTTON__;
		}
		return null;
	}

	private UicontrolStyle stringToStyleEnum(String style) {
		if (style.equals(__GO_UI_PUSHBUTTON__)) {
			return UicontrolStyle.PUSHBUTTON;
		}
		return null;
	}
	
	private UicontrolStyle style;
	private Double[] backgroundColor = {0.0, 0.0, 0.0};
	private boolean enable = true;
	private String fontAngle = "normal";
	private String fontName = "helvetica";
	private double fontSize = 10.0;
	private String fontUnits = "points";
	private String fontWeight = "normal";
	private Double[] foregroundColor = {0.0, 0.0, 0.0};
	private String horizontalAlignment = "center";
	private Integer[] listboxTop = null; 
	private int max = 1; 
	private int min = 0; 
	private Double[] position = {20.0, 40.0, 40.0, 20.0};
	private String relief = "raised";
	private Double[] sliderStep = {0.01, 0.1};
	private String[] string = {""};
	private String units = "pixels";
	private Integer[] value = null; 
	private String verticalAlignment = "middle";

	public Uicontrol() {
		super();
	}

	public String getType() {
		return __GO_UICONTROL__;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals(__GO_STYLE__)) {
			return UicontrolProperty.STYLE;
		} else if (propertyName.equals(__GO_UI_BACKGROUNDCOLOR__)) {
			return UicontrolProperty.BACKGROUNDCOLOR;
		} else if (propertyName.equals(__GO_UI_ENABLE__)) {
			return UicontrolProperty.ENABLE;
		} else if (propertyName.equals(__GO_UI_FONTANGLE__)) {
			return UicontrolProperty.FONTANGLE;
		} else if (propertyName.equals(__GO_UI_FONTNAME__)) {
			return UicontrolProperty.FONTNAME;
		} else if (propertyName.equals(__GO_UI_FONTSIZE__)) {
			return UicontrolProperty.FONTSIZE;
		} else if (propertyName.equals(__GO_UI_FONTUNITS__)) {
			return UicontrolProperty.FONTUNITS;
		} else if (propertyName.equals(__GO_UI_FONTWEIGHT__)) {
			return UicontrolProperty.FONTWEIGHT;
		} else if (propertyName.equals(__GO_UI_FOREGROUNDCOLOR__)) {
			return UicontrolProperty.FOREGROUNDCOLOR;
		} else if (propertyName.equals(__GO_UI_HORIZONTALALIGNMENT__)) {
			return UicontrolProperty.HORIZONTALALIGNMENT;
		} else if (propertyName.equals(__GO_UI_LISTBOXTOP__)) {
			return UicontrolProperty.LISTBOXTOP;
		} else if (propertyName.equals(__GO_UI_LISTBOXTOP_SIZE__)) {
			return UicontrolProperty.LISTBOXTOP_SIZE;
		} else if (propertyName.equals(__GO_UI_MAX__)) {
			return UicontrolProperty.MAX;
		} else if (propertyName.equals(__GO_UI_MIN__)) {
			return UicontrolProperty.MIN;
		} else if (propertyName.equals(__GO_POSITION__)) {
			return UicontrolProperty.POSITION;
		} else if (propertyName.equals(__GO_UI_RELIEF__)) {
			return UicontrolProperty.RELIEF;
		} else if (propertyName.equals(__GO_UI_SLIDERSTEP__)) {
			return UicontrolProperty.SLIDERSTEP;
		} else if (propertyName.equals(__GO_UI_STRING__)) {
			return UicontrolProperty.STRING;
		} else if (propertyName.equals(__GO_UI_STRING_SIZE__)) {
			return UicontrolProperty.STRING_SIZE;
		} else if (propertyName.equals(__GO_UI_UNITS__)) {
			return UicontrolProperty.UNITS;
		} else if (propertyName.equals(__GO_UI_VALUE__)) {
			return UicontrolProperty.VALUE;
		} else if (propertyName.equals(__GO_UI_VALUE_SIZE__)) {
			return UicontrolProperty.VALUE_SIZE;
		} else if (propertyName.equals(__GO_UI_VERTICALALIGNMENT__)) {
			return UicontrolProperty.VERTICALALIGNMENT;
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
		if (property == UicontrolProperty.STYLE) {
			return getStyle();
		} else if (property == UicontrolProperty.BACKGROUNDCOLOR) {
			return getBackgroundColor();
		} else if (property == UicontrolProperty.ENABLE) {
			return getEnable();
		} else if (property == UicontrolProperty.FONTANGLE) {
			return getFontAngle();
		} else if (property == UicontrolProperty.FONTNAME) {
			return getFontName();
		} else if (property == UicontrolProperty.FONTSIZE) {
			return getFontSize();
		} else if (property == UicontrolProperty.FONTUNITS) {
			return getFontUnits();
		} else if (property == UicontrolProperty.FONTWEIGHT) {
			return getFontWeight();
		} else if (property == UicontrolProperty.FOREGROUNDCOLOR) {
			return getForegroundColor();
		} else if (property == UicontrolProperty.HORIZONTALALIGNMENT) {
			return getHorizontalAlignment();
		} else if (property == UicontrolProperty.LISTBOXTOP) {
			return getListboxTop();
		} else if (property == UicontrolProperty.LISTBOXTOP_SIZE) {
			return getListboxTopSize();
		} else if (property == UicontrolProperty.MAX) {
			return getMax();
		} else if (property == UicontrolProperty.MIN) {
			return getMin();
		} else if (property == UicontrolProperty.POSITION) {
			return getUiPosition();
		} else if (property == UicontrolProperty.RELIEF) {
			return getRelief();
		} else if (property == UicontrolProperty.SLIDERSTEP) {
			return getSliderStep();
		} else if (property == UicontrolProperty.STRING) {
			return getString();
		} else if (property == UicontrolProperty.STRING_SIZE) {
			return getString().length;
		} else if (property == UicontrolProperty.UNITS) {
			return getUnits();
		} else if (property == UicontrolProperty.VALUE) {
			return getUiValue();
		} else if (property == UicontrolProperty.VALUE_SIZE) {
			return getUiValueSize();
		} else if (property == UicontrolProperty.VERTICALALIGNMENT) {
			return getVerticalAlignment();
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
		if (property == UicontrolProperty.STYLE) {
			setStyle((String) value);
		} else if (property == UicontrolProperty.BACKGROUNDCOLOR) {
			setBackgroundColor((Double[]) value);
		} else if (property == UicontrolProperty.ENABLE) {
			setEnable((Boolean) value);
		} else if (property == UicontrolProperty.FONTANGLE) {
			setFontAngle((String) value);
		} else if (property == UicontrolProperty.FONTNAME) {
			setFontName((String) value);
		} else if (property == UicontrolProperty.FONTSIZE) {
			setFontSize((Double) value);
		} else if (property == UicontrolProperty.FONTUNITS) {
			setFontUnits((String) value);
		} else if (property == UicontrolProperty.FONTWEIGHT) {
			setFontWeight((String) value);
		} else if (property == UicontrolProperty.FOREGROUNDCOLOR) {
			setForegroundColor((Double[]) value);
		} else if (property == UicontrolProperty.HORIZONTALALIGNMENT) {
			setHorizontalAlignment((String) value);
		} else if (property == UicontrolProperty.LISTBOXTOP) {
			setListboxTop((Integer[]) value);
		} else if (property == UicontrolProperty.MAX) {
			setMax((Integer) value);
		} else if (property == UicontrolProperty.MIN) {
			setMin((Integer) value);
		} else if (property == UicontrolProperty.POSITION) {
			setUiPosition((Double[]) value);
		} else if (property == UicontrolProperty.RELIEF) {
			setRelief((String) value);
		} else if (property == UicontrolProperty.SLIDERSTEP) {
			setSliderStep((Double[]) value);
		} else if (property == UicontrolProperty.STRING) {
			setString((String[]) value);
		} else if (property == UicontrolProperty.UNITS) {
			setUnits((String) value);
		} else if (property == UicontrolProperty.VALUE) {
			setUiValue((Integer[]) value);
		} else if (property == UicontrolProperty.VERTICALALIGNMENT) {
			setVerticalAlignment((String) value);
		} else {
			return super.setProperty(property, value);
		}

		return true;
	}

	/* Style */
	public String getStyle() {
		return styleEnumToString(this.style);
	}

	public void setStyle(String style) {
		this.style = stringToStyleEnum(style);
	}

	/* Background Color */
	public Double[] getBackgroundColor() {
		return this.backgroundColor;
	}

	public void setBackgroundColor(Double[] colors) {
		this.backgroundColor = colors;
	}

	/* Enable */
	public boolean getEnable() {
		return this.enable;
	}

	public void setEnable(boolean status) {
		this.enable = status;
	}

	/* FontAngle */
	public String getFontAngle() {
		return this.fontAngle;
	}

	public void setFontAngle(String fontAngle) {
		this.fontAngle = fontAngle;
	}

	/* FontName */
	public String getFontName() {
		return this.fontName;
	}

	public void setFontName(String fontName) {
		this.fontName = fontName;
	}

	/* FontSize */
	public double getFontSize() {
		return this.fontSize;
	}

	public void setFontSize(double fontSize) {
		this.fontSize = fontSize;
	}

	/* FontUnits */
	public String getFontUnits() {
		return this.fontUnits;
	}

	public void setFontUnits(String fontUnits) {
		this.fontUnits = fontUnits;
	}

	/* FontWeight */
	public String getFontWeight() {
		return this.fontWeight;
	}

	public void setFontWeight(String fontWeight) {
		this.fontWeight = fontWeight;
	}

	/* Foreground Color */
	public Double[] getForegroundColor() {
		return this.foregroundColor;
	}

	public void setForegroundColor(Double[] colors) {
		this.foregroundColor = colors;
	}

	/* Horizontal Alignment */
	public String getHorizontalAlignment() {
		return this.horizontalAlignment;
	}

	public void setHorizontalAlignment(String alignment) {
		this.horizontalAlignment = alignment;
	}

	/* Listbox Top */
	public Integer getListboxTopSize() {
		return (listboxTop != null ? listboxTop.length : 0); 
	}

	public Integer[] getListboxTop() {
		return this.listboxTop;
	}

	public void setListboxTop(Integer[] listboxTop) {
		this.listboxTop = listboxTop;
	}

	/* Max */
	public Integer getMax() {
		return this.max;
	}

	public void setMax(Integer max) {
		this.max = max;
	}

	/* Min */
	public Integer getMin() {
		return this.min;
	}

	public void setMin(Integer min) {
		this.min = min;
	}
	
	/* Position */
	public Double[] getUiPosition() {
		return this.position;
	}

	public void setUiPosition(Double[] position) {
		this.position = position;
	}

	/* Relief */
	public String getRelief() {
		return this.relief;
	}

	public void setRelief(String relief) {
		this.relief = relief;
	}

	/* String */
	public String[] getString() {
		return this.string;
	}

	public void setString(String[] string) {
		this.string = string;
	}

	/* Slider Step */
	public Double[] getSliderStep() {
		return this.sliderStep;
	}

	public void setSliderStep(Double[] sliderStep) {
		this.sliderStep = sliderStep;
	}

	/* Units */
	public String getUnits() {
		return this.units;
	}

	public void setUnits(String units) {
		this.units = units;
	}

	/* Value */
	public Integer getUiValueSize() {
		return (value != null ? value.length : 0); 
	}
	
	public Integer[] getUiValue() {
		return this.value;
	}

	public void setUiValue(Integer[] value) {
		this.value = value;
	}

	/* Vertical Alignment */
	public String getVerticalAlignment() {
		return this.verticalAlignment;
	}

	public void setVerticalAlignment(String alignment) {
		this.verticalAlignment = alignment;
	}

	public void accept(IVisitor visitor) {

	}

}
