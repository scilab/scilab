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

import org.scilab.modules.graphic_objects.graphicObject.IVisitor;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Text class
 * @author Manuel JULIACHS
 */
public class Text extends ClippableTextObject {
	/** Text properties names */
	private enum TextProperty { FONTANGLE, POSITION, ALIGNMENT, BOX, TEXTBOX, TEXTBOXMODE, AUTODIMENSIONING };

	/** Alignment */
	private enum Alignment { LEFT, CENTER, RIGHT;

		/**
		 * Converts an integer to the corresponding enum
		 * @param intValue the integer value
		 * @return the alignment enum
		 */
		public static Alignment intToEnum(Integer intValue) {
			switch (intValue) {
				case 0:
					return Alignment.LEFT;
				case 1:
					return Alignment.CENTER;
				case 2:
					return Alignment.RIGHT;
				default:
					return null;
			}
		}

	};

	/** Text box mode */
	private enum TextBoxMode { OFF, CENTERED, FILLED;

		/**
		 * Converts an integer to the corresponding enum
		 * @param intValue the integer value
		 * @return the text box mode enum
		 */
		public static TextBoxMode intToEnum(Integer intValue) {
			switch (intValue) {
				case 0:
					return TextBoxMode.OFF;
				case 1:
					return TextBoxMode.CENTERED;
				case 2:
					return TextBoxMode.FILLED;
				default:
					return null;
			}
		}

	};

	/** Text angle */
	private double fontAngle;

	/** 3D coordinates position (3-element array)*/
	private double[] position;

	/** Text alignment */
	private Alignment alignment;

	/** Specifies whether a box is drawn */
	private boolean box;

	/** Text box dimensions (2-element array) */
	private double[] textBox;

	/** Text box mode */
	private TextBoxMode textBoxMode;

	/** Auto-dimensioning */
	private boolean autoDimensioning;

	/** Constructor */
	public Text() {
		super();
		fontAngle = 0.0;
		position = new double[3];
		alignment = null;
		box = false;
		textBox = new double[2];
		textBoxMode = null;
		autoDimensioning = false;
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
			return TextProperty.FONTANGLE;
		} else if (propertyName.equals(__GO_POSITION__)) {
			return TextProperty.POSITION;
		} else if (propertyName.equals(__GO_ALIGNMENT__)) {
			return TextProperty.ALIGNMENT;
		} else if (propertyName.equals(__GO_BOX__)) {
			return TextProperty.BOX;
		} else if (propertyName.equals(__GO_TEXT_BOX__)) {
			return TextProperty.TEXTBOX;
		} else if (propertyName.equals(__GO_TEXT_BOX_MODE__)) {
			return TextProperty.TEXTBOXMODE;
		} else if (propertyName.equals(__GO_AUTO_DIMENSIONING__)) {
			return TextProperty.AUTODIMENSIONING;
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
		if (property == TextProperty.FONTANGLE) {
			return getFontAngle();
		} else if (property == TextProperty.POSITION) {
			return getPosition();
		} else if (property == TextProperty.ALIGNMENT) {
			return getAlignment();
		} else if (property == TextProperty.BOX) {
			return getBox();
		} else if (property == TextProperty.TEXTBOX) {
			return getTextBox();
		} else if (property == TextProperty.TEXTBOXMODE) {
			return getTextBoxMode();
		} else if (property == TextProperty.AUTODIMENSIONING) {
			return getAutoDimensioning();
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
		if (property == TextProperty.FONTANGLE) {
			setFontAngle((Double) value);
		} else if (property == TextProperty.POSITION) {
			setPosition((Double[]) value);
		} else if (property == TextProperty.ALIGNMENT) {
			setAlignment((Integer) value);
		} else if (property == TextProperty.BOX) {
			setBox((Boolean) value);
		} else if (property == TextProperty.TEXTBOX) {
			setTextBox((Double[]) value);
		} else if (property == TextProperty.TEXTBOXMODE) {
			setTextBoxMode((Integer) value);
		} else if (property == TextProperty.AUTODIMENSIONING) {
			setAutoDimensioning((Boolean) value);
		} else {
			return super.setProperty(property, value);
		}

		return true;
	}

	/**
	 * @return the alignment
	 */
	public Integer getAlignment() {
		return getAlignmentAsEnum().ordinal();
	}

	/**
	 * @return the alignment
	 */
	public Alignment getAlignmentAsEnum() {
		return alignment;
	}

	/**
	 * @param alignment the alignment to set
	 */
	public void setAlignment(Integer alignment) {
		setAlignmentAsEnum(Alignment.intToEnum(alignment));
	}

	/**
	 * @param alignment the alignment to set
	 */
	public void setAlignmentAsEnum(Alignment alignment) {
		this.alignment = alignment;
	}

	/**
	 * @return the autoDimensioning
	 */
	public Boolean getAutoDimensioning() {
		return autoDimensioning;
	}

	/**
	 * @param autoDimensioning the autoDimensioning to set
	 */
	public void setAutoDimensioning(Boolean autoDimensioning) {
		this.autoDimensioning = autoDimensioning;
	}

	/**
	 * @return the box
	 */
	public Boolean getBox() {
		return box;
	}

	/**
	 * @param box the box to set
	 */
	public void setBox(Boolean box) {
		this.box = box;
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
		this.position[0] = position[0];
		this.position[1] = position[1];
		this.position[2] = position[2];
	}

	/**
	 * @return the textBox
	 */
	public Double[] getTextBox() {
		Double[] retTextBox = new Double[2];
		retTextBox[0] = textBox[0];
		retTextBox[1] = textBox[2];

		return retTextBox;
	}

	/**
	 * @param textBox the textBox to set
	 */
	public void setTextBox(Double[] textBox) {
		this.textBox[0] = textBox[0];
		this.textBox[1] = textBox[1];
	}

	/**
	 * @return the textBoxMode
	 */
	public Integer getTextBoxMode() {
		return getTextBoxModeAsEnum().ordinal();
	}

	/**
	 * @return the textBoxMode
	 */
	public TextBoxMode getTextBoxModeAsEnum() {
		return textBoxMode;
	}

	/**
	 * @param textBoxMode the textBoxMode to set
	 */
	public void setTextBoxMode(Integer textBoxMode) {
		setTextBoxModeAsEnum(TextBoxMode.intToEnum(textBoxMode));
	}

	/**
	 * @param textBoxMode the textBoxMode to set
	 */
	public void setTextBoxModeAsEnum(TextBoxMode textBoxMode) {
		this.textBoxMode = textBoxMode;
	}

	/**
	 * @return Type as String
	 */
	public String getType() {
		return "Text";
	}

}
