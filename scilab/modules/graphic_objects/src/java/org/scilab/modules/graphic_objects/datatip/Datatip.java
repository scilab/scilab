/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.datatip;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

import java.text.DecimalFormat;


public class Datatip extends Text {

    /** Stores the data that will be shown(tip)*/
    Double[] tipData;
    /** false = display only (X, Y), true = display (X, Y, Z)*/
    Boolean use3component;
    /** Displayed number format*/
    DecimalFormat tipTextFormat;


    enum DatatipObjectProperty { TIP_DATA, TIP_ORIENTATION, TIP_3COMPONENT };
    enum TipOrientation { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT;

                          /**
                           * Transform a integer to a TipOrientation enum.
                           */
    public static TipOrientation intToEnum(Integer i) {
        switch (i) {
            case 0:
                return TipOrientation.TOP_LEFT;
            case 1:
                return TipOrientation.TOP_RIGHT;
            case 2:
                return TipOrientation.BOTTOM_LEFT;
            case 3:
                return TipOrientation.BOTTOM_RIGHT;
            default:
                return TipOrientation.TOP_RIGHT;
        }
    }

                        };

    TipOrientation currentOrientation;

    /**
     * Initializes the datatip, setup format, orientation and mark.
     */
    public Datatip() {
        super();
        tipData = new Double[] {0.0, 0.0, 0.0};
        use3component = false;
        setOrientationAsEnum(TipOrientation.TOP_RIGHT);
        tipTextFormat = new DecimalFormat("#.####");
        setBox(true);
        setLineMode(true);
        setFillMode(true);
        setBackground(-2);
        setClipState(1);

        setMarkMode(true);
        setMarkSize(8);
        setMarkBackground(-1);
        setMarkForeground(-1);
        setMarkStyle(11);
    }

    @Override
    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    /**
     * Convert the property name to the DatatipObjectProperty enum.
     */
    public Object getPropertyFromName(String propertyName) {
        if (propertyName.equals(__GO_DATATIP_DATA__)) {
            return DatatipObjectProperty.TIP_DATA;
        } else if (propertyName.equals(__GO_DATATIP_ORIENTATION__)) {
            return DatatipObjectProperty.TIP_ORIENTATION;
        } else if (propertyName.equals(__GO_DATATIP_3COMPONENT__)) {
            return DatatipObjectProperty.TIP_3COMPONENT;
        } else {
            return super.getPropertyFromName(propertyName);
        }
    }

    /**
     * @return the datatip property
     */
    public Object getProperty(Object property) {
        if (property == DatatipObjectProperty.TIP_DATA) {
            return getTipData();
        } else if (property == DatatipObjectProperty.TIP_ORIENTATION) {
            return getOrientation();
        } else if (property == DatatipObjectProperty.TIP_3COMPONENT) {
            return isUsing3Component();
        } else {
            return super.getProperty(property);
        }
    }

    /**
     * Set the datatip property
     * @param property the property
     * @param value the new property value.
     */
    public UpdateStatus setProperty(Object property, Object value) {
        if (property == DatatipObjectProperty.TIP_DATA) {
            setTipData((Double[]) value);
        } else if (property == DatatipObjectProperty.TIP_ORIENTATION) {
            setOrientation((Integer) value);
        } else if (property == DatatipObjectProperty.TIP_3COMPONENT) {
            setUse3Component((Boolean)value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * Set the data tip that will be shown
     * @param data A 3 element vector with the data (x, y, z).
     */
    public void setTipData(Double[] data) {
        tipData[0] = data[0];
        tipData[1] = data[1];
        tipData[2] = data[2];
        updateText();
    }

    /**
     * Get the current tip data
     * @return the tip data
     */
    public Double[] getTipData() {
        Double[] ret = new Double[3];
        ret[0] = tipData[0];
        ret[1] = tipData[1];
        ret[2] = tipData[2];
        return ret;
    }

    /**
     * Get the current textbox orientation as an integer
     * @return current orientation.
     */
    public Integer getOrientation() {
        return getOrientationAsEnum().ordinal();
    }

    /**
     * Get the current oriantation as a enum
     * @return the current orientation
     */
    public TipOrientation getOrientationAsEnum() {
        return currentOrientation;
    }

    /**
     * Set the current orientation (updating the text position)
     * @param orientation the new orientation (integer).
     */
    public void setOrientation(Integer orientation) {
        currentOrientation = TipOrientation.intToEnum(orientation);
    }

    /**
     * Set the current orientation (updating the text position)
     * @param orientation the new orientation (TipOrientation enum).
     */
    public void setOrientationAsEnum(TipOrientation orientation) {
        currentOrientation = orientation;
    }

    /**
     * @return true if the datatip is displaying the Z component, false otherwise.
     */
    public Boolean isUsing3Component() {
        return use3component;
    }

    /**
     * If true set the Z component to be displayed.
     * @param useZ True to enable display the Z component, false to disable. 
     */
    public void setUse3Component(Boolean useZ) {
        use3component = useZ;
        updateText();
    }



    /**
     * Update the text from the datatip base on current tipData value.
     */
    void updateText() {
        String[] textArray = new String[] {"X:", "Y:", "Z:"};
        textArray[0] += tipTextFormat.format(tipData[0]);
        textArray[1] += tipTextFormat.format(tipData[1]);
        textArray[2] += tipTextFormat.format(tipData[2]);

        Integer[] dim = new Integer[2];
        dim[0] = use3component ? 3 : 2;
        dim[1] = 1;

        setTextArrayDimensions(dim);
        setTextStrings(textArray);
    }

    @Override
    public Double[] getPosition(){
        return getTipData();
    }

    /**
     * @return Type as String
     */
    public String getType() {
        return "Datatip";
    }

}
