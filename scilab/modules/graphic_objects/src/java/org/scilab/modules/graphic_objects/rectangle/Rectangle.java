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

package org.scilab.modules.graphic_objects.rectangle;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Class Rectangle
 * @author Manuel JULIACHS
 */
public class Rectangle extends ClippableContouredObject {
    /** Rectangle properties names */
    private enum RectangleProperty { UPPERLEFTPOINT, WIDTH, HEIGHT };

    /** Upper-left corner (x,y,z) coordinates */
    private double[] upperLeftPoint;

    /** Width */
    private double width;

    /** Height */
    private double height;

    /** Constructor */
    public Rectangle() {
        super();
        upperLeftPoint = new double[3];
        width = 0.0;
        height = 0.0;
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
            case __GO_UPPER_LEFT_POINT__ :
                return RectangleProperty.UPPERLEFTPOINT;
            case __GO_WIDTH__ :
                return RectangleProperty.WIDTH;
            case __GO_HEIGHT__ :
                return RectangleProperty.HEIGHT;
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
        if (property == RectangleProperty.UPPERLEFTPOINT) {
            return getUpperLeftPoint();
        } else if (property == RectangleProperty.WIDTH) {
            return getWidth();
        } else if (property == RectangleProperty.HEIGHT) {
            return getHeight();
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
        if (property == RectangleProperty.UPPERLEFTPOINT) {
            setUpperLeftPoint((Double[]) value);
        } else if (property == RectangleProperty.WIDTH) {
            setWidth((Double) value);
        } else if (property == RectangleProperty.HEIGHT) {
            setHeight((Double) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the height
     */
    public Double getHeight() {
        return height;
    }

    /**
     * @param height the height to set
     */
    public UpdateStatus setHeight(Double height) {
        this.height = height;
        return UpdateStatus.Success;
    }

    /**
     * @return the upperLeftPoint
     */
    public Double[] getUpperLeftPoint() {
        Double[] retPoint = new Double[3];

        retPoint[0] = upperLeftPoint[0];
        retPoint[1] = upperLeftPoint[1];
        retPoint[2] = upperLeftPoint[2];

        return retPoint;
    }

    /**
     * @param upperLeftPoint the upperLeftPoint to set
     */
    public UpdateStatus setUpperLeftPoint(Double[] upperLeftPoint) {
        this.upperLeftPoint[0] = upperLeftPoint[0];
        this.upperLeftPoint[1] = upperLeftPoint[1];
        this.upperLeftPoint[2] = upperLeftPoint[2];
        return UpdateStatus.Success;
    }

    /**
     * @return the width
     */
    public Double getWidth() {
        return width;
    }

    /**
     * @param width the width to set
     */
    public UpdateStatus setWidth(Double width) {
        this.width = width;
        return UpdateStatus.Success;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_RECTANGLE__;
    }

}
