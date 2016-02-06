/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
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

package org.scilab.modules.graphic_objects.fec;

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Fec class
 * @author Manuel JULIACHS
 */
public class Fec extends ClippableContouredObject {
    // TBD: Data
    // + triangles	-> Data Model
    // TBD properties relative to the data model
    /** Fec properties names */
    private enum FecProperty { ZBOUNDS, OUTSIDECOLOR, COLORRANGE  };

    /** Z Bounds: 2-element array */
    private double[] zBounds;

    /** 2-element array */
    private int[] outsideColor;

    /** 2-element array */
    private int[] colorRange;

    /** Constructor */
    public Fec() {
        super();
        zBounds = new double[2];
        outsideColor = new int[2];
        colorRange = new int[2];
    }

    @Override
    public void accept(Visitor visitor) throws ObjectRemovedException {
        visitor.visit(this);
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_Z_BOUNDS__ :
                return FecProperty.ZBOUNDS;
            case __GO_OUTSIDE_COLOR__ :
                return FecProperty.OUTSIDECOLOR;
            case __GO_COLOR_RANGE__ :
                return FecProperty.COLORRANGE;
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
        if (property == FecProperty.ZBOUNDS) {
            return getZBounds();
        } else if (property == FecProperty.OUTSIDECOLOR) {
            return getOutsideColor();
        } else if (property == FecProperty.COLORRANGE) {
            return getColorRange();
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
        if (property == FecProperty.ZBOUNDS) {
            setZBounds((Double[]) value);
        } else if (property == FecProperty.OUTSIDECOLOR) {
            setOutsideColor((Integer[]) value);
        } else if (property == FecProperty.COLORRANGE) {
            setColorRange((Integer[]) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the colorRange
     */
    public Integer[] getColorRange() {
        Integer[] retColorRange = new Integer[2];
        retColorRange[0] = colorRange[0];
        retColorRange[1] = colorRange[1];

        return retColorRange;
    }

    /**
     * @param colorRange the colorRange to set
     */
    public UpdateStatus setColorRange(Integer[] colorRange) {
        this.colorRange[0] = colorRange[0];
        this.colorRange[1] = colorRange[1];
        return UpdateStatus.Success;
    }

    /**
     * @return the outsideColor
     */
    public Integer[] getOutsideColor() {
        Integer[] retOutsideColor = new Integer[2];
        retOutsideColor[0] = outsideColor[0];
        retOutsideColor[1] = outsideColor[1];

        return retOutsideColor;
    }

    /**
     * @param outsideColor the outsideColor to set
     */
    public UpdateStatus setOutsideColor(Integer[] outsideColor) {
        this.outsideColor[0] = outsideColor[0];
        this.outsideColor[1] = outsideColor[1];
        return UpdateStatus.Success;
    }

    /**
     * @return the zBounds
     */
    public Double[] getZBounds() {
        Double[] retZBounds = new Double[2];
        retZBounds[0] = zBounds[0];
        retZBounds[1] = zBounds[1];

        return retZBounds;
    }

    /**
     * @param bounds the zBounds to set
     */
    public UpdateStatus setZBounds(Double[] zBounds) {
        this.zBounds[0] = zBounds[0];
        this.zBounds[1] = zBounds[1];
        return UpdateStatus.Success;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_FEC__;
    }

}
