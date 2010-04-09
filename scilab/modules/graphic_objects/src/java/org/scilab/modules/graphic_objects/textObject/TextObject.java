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

package org.scilab.modules.graphic_objects.textObject;

import org.scilab.modules.graphic_objects.contouredObject.ContouredObject;
import org.scilab.modules.graphic_objects.textObject.Font.FontProperty;
import org.scilab.modules.graphic_objects.textObject.FormattedText.FormattedTextProperty;


/**
 * TextObject class
 * @author Manuel JULIACHS
 */
public class TextObject extends ContouredObject {
	/** TextObject properties names */
	private enum TextObjectProperty { TEXT };

	/** Formatted text object */
	private FormattedText text;

	/** Constructor */
	public TextObject() {
		super();
		text = new FormattedText();
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("Text")) {
			return TextObjectProperty.TEXT;
		} else if (propertyName.equals("TextString")) { // To be modified
			return FormattedText.FormattedTextProperty.TEXT;
		} else if (propertyName.equals("Font")) {
			return FormattedText.FormattedTextProperty.FONT;
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
		if (property == TextObjectProperty.TEXT) {
			return getText();
		} else if (property == FormattedText.FormattedTextProperty.TEXT) {
			return getTextString();
		} else if (property == FormattedText.FormattedTextProperty.FONT) {
			return getFont();
		} else if (property == Font.FontProperty.STYLE) {
			return getFontStyle();
		} else if (property == Font.FontProperty.SIZE) {
			return getFontSize();
		} else if (property == Font.FontProperty.COLOR) {
			return getFontColor();
		} else if (property == Font.FontProperty.FRACTIONAL) {
			return getFontFractional();
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
		if (property == TextObjectProperty.TEXT) {
			setText((FormattedText) value);
		} else if (property == FormattedText.FormattedTextProperty.TEXT) {
			setTextString((String) value);
		} else if (property == FormattedText.FormattedTextProperty.FONT) {
			setFont((Font) value);
		} else if (property == Font.FontProperty.STYLE) {
			setFontStyle((Integer) value);
		} else if (property == Font.FontProperty.SIZE) {
			setFontSize((Double) value);
		} else if (property == Font.FontProperty.COLOR) {
			setFontColor((Integer) value);
		} else if (property == Font.FontProperty.FRACTIONAL) {
			setFontFractional((Boolean) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the text
	 */
	public FormattedText getText() {
		return text;
	}

	/**
	 * @param text the text to set
	 */
	public void setText(FormattedText text) {
		this.text = text;
	}

	/**
	 * @return the text string
	 */
	public String getTextString() {
		return text.getText();
	}

	/**
	 * @param text the text string to set
	 */
	public void setTextString(String text) {
		this.text.setText(text);
	}

	/**
	 * @return the font
	 */
	public Font getFont() {
		return text.getFont();
	}

	/**
	 * @param font the font to set
	 */
	public void setFont(Font font) {
		text.setFont(font);
	}

	/**
	 * @return the font style
	 */
	public Integer getFontStyle() {
		return text.getFont().getStyle();
	}

	/**
	 * @param style the font style to set
	 */
	public void setFontStyle(Integer style) {
		text.getFont().setStyle(style);
	}

	/**
	 * @return the font color
	 */
	public Integer getFontColor() {
		return text.getFont().getColor();
	}

	/**
	 * @param color the font color to set 
	 */
	public void setFontColor(Integer color) {
		text.getFont().setColor(color);
	}

	/**
	 * @return the font size
	 */
	public Double getFontSize() {
		return text.getFont().getSize();
	}

	/**
	 * @param size the font size to set
	 */
	public void setFontSize(Double size) {
		text.getFont().setSize(size);
	}

	/**
	 * @return the font fractional
	 */
	public Boolean getFontFractional() {
		return text.getFont().getFractional();
	}

	/**
	 * @param fractional the font fractional to set
	 */
	public void setFontFractional(Boolean fractional) {
		text.getFont().setFractional(fractional);
	}

}
