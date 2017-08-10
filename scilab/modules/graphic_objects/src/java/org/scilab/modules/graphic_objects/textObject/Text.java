/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2012 - DIGITEO - Manuel JULIACHS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.graphic_objects.textObject;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Text class
 * @author Manuel JULIACHS
 */
public class Text extends ClippableTextObject {
    /** Text properties names */
    private enum TextProperty { FONTANGLE, POSITION, CORNERS, ALIGNMENT, BOX, TEXTBOX, TEXTBOXMODE, AUTODIMENSIONING };

    /** Alignment */
    public enum Alignment { LEFT, CENTER, RIGHT;

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

    /** 3D coordinates position (3-element array) */
    private double[] position;

    /**
     * The 3D coordinates of the Text's four corners.
     * They are stored in clockwise order, starting from the lower-left corner.
     */
    private double[][] corners;

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
        corners = new double[4][3];
        alignment = Alignment.CENTER;
        box = false;
        textBox = new double[2];
        textBoxMode = TextBoxMode.OFF;
        autoDimensioning = false;
    }

    @Override
    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_FONT_ANGLE__ :
                return TextProperty.FONTANGLE;
            case __GO_POSITION__ :
                return TextProperty.POSITION;
            case __GO_CORNERS__ :
                return TextProperty.CORNERS;
            case __GO_ALIGNMENT__ :
                return TextProperty.ALIGNMENT;
            case __GO_BOX__ :
                return TextProperty.BOX;
            case __GO_TEXT_BOX__ :
                return TextProperty.TEXTBOX;
            case __GO_TEXT_BOX_MODE__ :
                return TextProperty.TEXTBOXMODE;
            case __GO_AUTO_DIMENSIONING__ :
                return TextProperty.AUTODIMENSIONING;
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
        if (property == TextProperty.FONTANGLE) {
            return getFontAngle();
        } else if (property == TextProperty.POSITION) {
            return getPosition();
        } else if (property == TextProperty.CORNERS) {
            return getCorners();
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
    public UpdateStatus setProperty(Object property, Object value) {
        if (property == TextProperty.FONTANGLE) {
            setFontAngle((Double) value);
        } else if (property == TextProperty.POSITION) {
            setPosition((Double[]) value);
        } else if (property == TextProperty.CORNERS) {
            setCorners((Double[]) value);
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

        return UpdateStatus.Success;
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
    public UpdateStatus setAlignment(Integer alignment) {
        return setAlignmentAsEnum(Alignment.intToEnum(alignment));
    }

    /**
     * @param alignment the alignment to set
     */
    public UpdateStatus setAlignmentAsEnum(Alignment alignment) {
        this.alignment = alignment;
        return UpdateStatus.Success;
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
    public UpdateStatus setAutoDimensioning(Boolean autoDimensioning) {
        this.autoDimensioning = autoDimensioning;
        return UpdateStatus.Success;
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
    public UpdateStatus setBox(Boolean box) {
        this.box = box;
        return UpdateStatus.Success;
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
    public UpdateStatus setFontAngle(Double fontAngle) {
        this.fontAngle = fontAngle;
        return UpdateStatus.Success;
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
    public UpdateStatus setPosition(Double[] position) {
        this.position[0] = position[0];
        this.position[1] = position[1];
        this.position[2] = position[2];
        return UpdateStatus.Success;
    }

    /**
     * Returns the coordinates of the Text's corners
     * Coordinates are returned as consecutive (x,y,z) triplets, each triplet
     * corresponding to one corner, starting from the lower-left one and
     * going in clockwise order.
     * @return the corners' coordinates (12-element array)
     */
    public Double[] getCorners() {
        Double[] retCorners = new Double[12];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                retCorners[3 * i + j] = corners[i][j];
            }
        }

        return retCorners;
    }

    /**
     * Sets the coordinates of the Text's corners
     * The coordinates to set are given as consecutive (x,y,z) triplets, each triplet
     * corresponding to one corner, starting from the lower-left one and going in
     * clockwise order.
     * @param coordinates the corners' coordinates (12-element array)
     */
    public UpdateStatus setCorners(Double[] coordinates) {
        if (coordinates.length != 12) {
            return UpdateStatus.NoChange;
        }

        corners[0][0] = coordinates[0];
        corners[0][1] = coordinates[1];
        corners[0][2] = coordinates[2];

        corners[1][0] = coordinates[3];
        corners[1][1] = coordinates[4];
        corners[1][2] = coordinates[5];

        corners[2][0] = coordinates[6];
        corners[2][1] = coordinates[7];
        corners[2][2] = coordinates[8];

        corners[3][0] = coordinates[9];
        corners[3][1] = coordinates[10];
        corners[3][2] = coordinates[11];

        return UpdateStatus.Success;
    }

    /**
     * @return the textBox
     */
    public Double[] getTextBox() {
        Double[] retTextBox = new Double[2];
        retTextBox[0] = textBox[0];
        retTextBox[1] = textBox[1];

        return retTextBox;
    }

    /**
     * @param textBox the textBox to set
     */
    public UpdateStatus setTextBox(Double[] textBox) {
        this.textBox[0] = textBox[0];
        this.textBox[1] = textBox[1];
        return UpdateStatus.Success;
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
    public UpdateStatus setTextBoxMode(Integer textBoxMode) {
        return setTextBoxModeAsEnum(TextBoxMode.intToEnum(textBoxMode));
    }

    /**
     * @param textBoxMode the textBoxMode to set
     */
    public UpdateStatus setTextBoxModeAsEnum(TextBoxMode textBoxMode) {
        this.textBoxMode = textBoxMode;
        return UpdateStatus.Success;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_TEXT__;
    }

}
