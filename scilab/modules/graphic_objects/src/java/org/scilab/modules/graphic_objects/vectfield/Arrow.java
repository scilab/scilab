/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Manuel JULIACHS
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

package org.scilab.modules.graphic_objects.vectfield;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ARROW_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BASE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DIRECTION__;

import org.scilab.modules.graphic_objects.contouredObject.ContouredObject;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;


/**
 * Arrow class
 * @author Manuel JULIACHS
 */
public class Arrow extends ContouredObject {
    /** Arrow properties names */
    public enum ArrowProperty { BASE, BASEX, BASEY, BASEZ, DIRECTION, DIRECTIONX, DIRECTIONY, DIRECTIONZ, ARROWSIZE };

    /** Arrow base (x,y,z) coordinates */
    private double[] base;

    /** Arrow vector (x,y,z) coordinates */
    private double[] direction;

    /** Arrow size */
    private double arrowSize;

    /** Constructor */
    public Arrow() {
        super();
        base = new double[] {0.0, 0.0, 0.0};
        direction = new double[] {0.0, 0.0, 0.0};
        arrowSize = -1.0;
    }

    /** Clone method */
    public Arrow clone() {
        Arrow copy = (Arrow) super.clone();

        copy.base = new double[] {0.0, 0.0, 0.0};
        copy.direction = new double[] {0.0, 0.0, 0.0};

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
            case __GO_BASE__ :
                return ArrowProperty.BASE;
            case __GO_DIRECTION__ :
                return ArrowProperty.DIRECTION;
            case __GO_ARROW_SIZE__ :
                return ArrowProperty.ARROWSIZE;
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
        if (property == ArrowProperty.BASE) {
            return getBase();
        } else if (property == ArrowProperty.DIRECTION) {
            return getDirection();
        } else if (property == ArrowProperty.ARROWSIZE) {
            return getArrowSize();
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
        if (property == ArrowProperty.BASE) {
            setBase((Double[]) value);
        } else if (property == ArrowProperty.DIRECTION) {
            setDirection((Double[]) value);
        } else if (property == ArrowProperty.ARROWSIZE) {
            setArrowSize((Double) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the arrowSize
     */
    public Double getArrowSize() {
        return arrowSize;
    }

    /**
     * @param arrowSize the arrowSize to set
     */
    public UpdateStatus setArrowSize(Double arrowSize) {
        this.arrowSize = arrowSize;
        return UpdateStatus.Success;
    }

    /**
     * @return the base
     */
    public Double[] getBase() {
        Double[] retBase = new Double[3];

        retBase[0] = base[0];
        retBase[1] = base[1];
        retBase[2] = base[2];

        return retBase;
    }

    /**
     * @param base the base to set
     */
    public UpdateStatus setBase(Double[] base) {
        this.base[0] = base[0];
        this.base[1] = base[1];
        this.base[2] = base[2];
        return UpdateStatus.Success;
    }

    /**
     * @return the direction
     */
    public Double[] getDirection() {
        Double[] retDirection = new Double[3];

        retDirection[0] = direction[0];
        retDirection[1] = direction[1];
        retDirection[2] = direction[2];

        return retDirection;
    }

    /**
     * @param direction the direction to set
     */
    public UpdateStatus setDirection(Double[] direction) {
        this.direction[0] = direction[0];
        this.direction[1] = direction[1];
        this.direction[2] = direction[2];
        return UpdateStatus.Success;
    }

    public Integer getType() {
        return -1;
    }

}
