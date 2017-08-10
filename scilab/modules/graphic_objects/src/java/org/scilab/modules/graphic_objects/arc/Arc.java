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

package org.scilab.modules.graphic_objects.arc;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Arc class
 * @author Manuel JULIACHS
 */
public class Arc extends ClippableContouredObject {
    /** Arc properties names */
    public enum ArcProperty { UPPERLEFTPOINT, WIDTH, HEIGHT, STARTANGLE, ENDANGLE, ARCDRAWINGMETHOD  };

    /** Arc drawing method */
    public enum ArcDrawingMethod { NURBS, LINES;

                                   /**
                                    * Converts an integer to the corresponding enum
                                    * @param intValue the integer value
                                    * @return the arc drawing method enum
                                    */
    public static ArcDrawingMethod intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return ArcDrawingMethod.NURBS;
            case 1:
                return ArcDrawingMethod.LINES;
            default:
                return null;
        }
    }
                                 }

    /** Bounding box upper-left point (x,y,z) coordinates */
    private double[] upperLeftPoint;

    /** Bounding box width */
    private double width;

    /** Bounding box height */
    private double height;

    /** Start angle */
    private double startAngle;

    /** End angle */
    private double endAngle;

    /** Specifies which drawing method to use */
    private ArcDrawingMethod arcDrawingMethod;

    /**
     * Constructor
     */
    public Arc() {
        super();
        arcDrawingMethod = ArcDrawingMethod.LINES;
        upperLeftPoint = new double[] {0, 0, 0};
        width = 0.0;
        height = 0.0;
        startAngle = 0.0;
        endAngle = 0.0;
    }

    public Arc clone() {
        Arc copy = (Arc) super.clone();
        copy.upperLeftPoint = new double[] {0, 0, 0};
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
            case __GO_UPPER_LEFT_POINT__ :
                return ArcProperty.UPPERLEFTPOINT;
            case __GO_WIDTH__ :
                return ArcProperty.WIDTH;
            case __GO_HEIGHT__ :
                return ArcProperty.HEIGHT;
            case __GO_START_ANGLE__ :
                return ArcProperty.STARTANGLE;
            case __GO_END_ANGLE__ :
                return ArcProperty.ENDANGLE;
            case __GO_ARC_DRAWING_METHOD__ :
                return ArcProperty.ARCDRAWINGMETHOD;
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
        if (property == ArcProperty.UPPERLEFTPOINT) {
            return getUpperLeftPoint();
        } else if (property == ArcProperty.WIDTH) {
            return getWidth();
        } else if (property == ArcProperty.HEIGHT) {
            return getHeight();
        } else if (property == ArcProperty.STARTANGLE) {
            return getStartAngle();
        } else if (property == ArcProperty.ENDANGLE) {
            return getEndAngle();
        } else if (property == ArcProperty.ARCDRAWINGMETHOD) {
            return getArcDrawingMethod();
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
        if (property == ArcProperty.UPPERLEFTPOINT) {
            setUpperLeftPoint((Double[]) value);
        } else if (property == ArcProperty.WIDTH) {
            setWidth((Double) value);
        } else if (property == ArcProperty.HEIGHT) {
            setHeight((Double) value);
        } else if (property == ArcProperty.STARTANGLE) {
            setStartAngle((Double) value);
        } else if (property == ArcProperty.ENDANGLE) {
            setEndAngle((Double) value);
        } else if (property == ArcProperty.ARCDRAWINGMETHOD) {
            setArcDrawingMethod((Integer) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * Set the parent axes
     * @param axes the parent axes to set
     */
    /*
    public void setParent(Axes axes) {
    	super.setParent(axes);
    }
    */

    /**
     * @return the arcDrawingMethod
     */
    public Integer getArcDrawingMethod() {
        return getArcDrawingMethodAsEnum().ordinal();
    }

    /**
     * @return the arcDrawingMethod
     */
    public ArcDrawingMethod getArcDrawingMethodAsEnum() {
        return arcDrawingMethod;
    }

    /**
     * @param arcDrawingMethod the arcDrawingMethod to set
     */
    public UpdateStatus setArcDrawingMethod(Integer arcDrawingMethod) {
        setArcDrawingMethodAsEnum(ArcDrawingMethod.intToEnum(arcDrawingMethod));
        return UpdateStatus.Success;

    }

    /**
     * @param arcDrawingMethod the arcDrawingMethod to set
     */
    public UpdateStatus setArcDrawingMethodAsEnum(ArcDrawingMethod arcDrawingMethod) {
        this.arcDrawingMethod = arcDrawingMethod;
        return UpdateStatus.Success;
    }

    /**
     * @return the endAngle
     */
    public Double getEndAngle() {
        return endAngle;
    }

    /**
     * @param endAngle the endAngle to set
     */
    public UpdateStatus setEndAngle(Double endAngle) {
        this.endAngle = endAngle;
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
     * @return the startAngle
     */
    public Double getStartAngle() {
        return startAngle;
    }

    /**
     * @param startAngle the startAngle to set
     */
    public UpdateStatus setStartAngle(Double startAngle) {
        this.startAngle = startAngle;
        return UpdateStatus.Success;
    }

    /**
     * @return the upperLeftPoint
     */
    public Double[] getUpperLeftPoint() {
        Double[] returnedPoint = new Double[3];

        returnedPoint[0] = upperLeftPoint[0];
        returnedPoint[1] = upperLeftPoint[1];
        returnedPoint[2] = upperLeftPoint[2];

        return returnedPoint;
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

    public Integer getType() {
        return GraphicObjectProperties.__GO_ARC__;
    }
}
