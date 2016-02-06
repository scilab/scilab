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

package org.scilab.modules.graphic_objects.label;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;
import org.scilab.modules.graphic_objects.textObject.TextObject;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Label class
 * @author Manuel JULIACHS
 */
public class Label extends TextObject {
    /** Label properties names */
    public enum LabelProperty { FONTANGLE, AUTOPOSITION, POSITION, CORNERS, AUTOROTATION };

    /** Text orientation */
    private double fontAngle;

    /** Specifies whether the label position is automatically computed or not */
    private boolean autoPosition;

    /** 3-element array: (x, y, z) position */
    private double[] position;

    /**
     * The 3D coordinates of the Label's four corners.
     * They are stored in clockwise order, starting from the lower-left corner.
     */
    private double[][] corners;

    /** Specifies whether the label orientation is automatically computed or not */
    private boolean autoRotation;

    /** Constructor */
    public Label() {
        super();
        fontAngle = 0.0;
        autoPosition = false;
        position = new double[3];
        corners = new double[4][3];
        autoRotation = false;

        Integer[] textDimensions = {1, 1};
        String[] textStrings = {""};

        setTextArrayDimensions(textDimensions);
        setTextStrings(textStrings);
    }

    /**
     * Clone method
     */
    public Label clone() {
        Label copy = (Label) super.clone();

        copy.position = new double[3];
        for (int i = 0; i < position.length; i++) {
            copy.position[i] = position[i];
        }

        return copy;
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
                return LabelProperty.FONTANGLE;
            case __GO_AUTO_POSITION__ :
                return LabelProperty.AUTOPOSITION;
            case __GO_POSITION__ :
                return LabelProperty.POSITION;
            case __GO_CORNERS__ :
                return LabelProperty.CORNERS;
            case __GO_AUTO_ROTATION__ :
                return LabelProperty.AUTOROTATION;
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
        if (property == LabelProperty.FONTANGLE) {
            return getFontAngle();
        } else if (property == LabelProperty.AUTOPOSITION) {
            return getAutoPosition();
        } else if (property == LabelProperty.POSITION) {
            return getPosition();
        } else if (property == LabelProperty.CORNERS) {
            return getCorners();
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
    public UpdateStatus setProperty(Object property, Object value) {
        if (property == LabelProperty.FONTANGLE) {
            setFontAngle((Double) value);
        } else if (property == LabelProperty.AUTOPOSITION) {
            setAutoPosition((Boolean) value);
        } else if (property == LabelProperty.POSITION) {
            setPosition((Double[]) value);
        } else if (property == LabelProperty.CORNERS) {
            setCorners((Double[]) value);
        } else if (property == LabelProperty.AUTOROTATION) {
            setAutoRotation((Boolean) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
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
    public UpdateStatus setAutoPosition(Boolean autoPosition) {
        this.autoPosition = autoPosition;
        return UpdateStatus.Success;
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
    public UpdateStatus setAutoRotation(Boolean autoRotation) {
        this.autoRotation = autoRotation;
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
        if (autoRotation == true) {
            autoRotation = false;
        }

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
        if (autoPosition == true) {
            autoPosition = false;
        }

        this.position[0] = position[0];
        this.position[1] = position[1];
        this.position[2] = position[2];
        return UpdateStatus.Success;
    }

    /**
     * Returns the coordinates of the Label's corners
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
     * Sets the coordinates of the Label's corners
     * The coordinates to set are given as consecutive (x,y,z) triplets, each triplet
     * corresponding to one corner, starting from the lower-left one and going in
     * clockwise order.
     * @param coordinates the corners' coordinates (12-element array)
     */
    public UpdateStatus setCorners(Double [] coordinates) {
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
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_LABEL__;
    }

}
