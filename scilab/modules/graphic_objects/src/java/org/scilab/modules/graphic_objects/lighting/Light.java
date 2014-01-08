/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.lighting;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AMBIENTCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DIFFUSECOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DIRECTION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LIGHT_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SPECULARCOLOR__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.utils.LightType;

/**
 * Light class
 * @author Pedro SOUZA
 */
public class Light extends GraphicObject {

    public enum LightProperty {POSITION, DIRECTION, TYPE};

    /** light position */
    double[] position;

    /** light direction */
    double[] direction;

    /** light type */
    LightType type;

    /** the light color */
    ColorTriplet lightColor;

    public Light() {
        super();
        lightColor = new ColorTriplet();

        position = new double[] {0.0, 0.0, 1.0};
        direction = new double[] {0.0, 0.0, 1.0};
        type = LightType.POINT;

        double[] dark_gray = new double[] {0.1, 0.1, 0.1};
        double[] white = new double[] {1.0, 1.0, 1.0};
        setAmbientColor(dark_gray);
        setDiffuseColor(white);
        setSpecularColor(white);
    }

    /** copy contructor */
    public Light(Light other) {
        super();
        position = new double[3];
        direction = new double[3];
        type = other.type;
        lightColor = new ColorTriplet(other.lightColor);
        setPosition(other.position);
        setDirection(other.direction);
    }

    @Override
    public void accept(Visitor visitor) {
    }

    /** returns the light's position */
    public Double[] getPosition() {
        Double[] ret = new Double[] {position[0], position[1], position[2]};
        return ret;
    }

    /** Sets the light's position */
    public UpdateStatus setPosition(double[] pos) {
        if (pos.length != 3) {
            return UpdateStatus.Fail;
        }

        if (position[0] != pos[0] || position[1] != pos[1] || position[2] != pos[2]) {
            position[0] = pos[0];
            position[1] = pos[1];
            position[2] = pos[2];
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }

    /** returns the light's direction */
    public Double[] getDirection() {
        Double[] ret = new Double[] {direction[0], direction[1], direction[2]};
        return ret;
    }

    /** Sets the light's direction */
    public UpdateStatus setDirection(double[] dir) {
        if (dir.length != 3) {
            return UpdateStatus.Fail;
        }

        if (direction[0] != dir[0] || direction[1] != dir[1] || direction[2] != dir[2]) {
            direction[0] = dir[0];
            direction[1] = dir[1];
            direction[2] = dir[2];
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }

    /** Sets the light's type */
    public UpdateStatus setLightType(LightType type) {
        if (this.type != type) {
            this.type = type;
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }

    /** Get the light's type */
    public LightType getLightType() {
        return type;
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_AMBIENTCOLOR__ :
                return ColorTriplet.ColorTripletProperty.AMBIENTCOLOR;
            case __GO_DIFFUSECOLOR__ :
                return ColorTriplet.ColorTripletProperty.DIFFUSECOLOR;
            case __GO_SPECULARCOLOR__ :
                return ColorTriplet.ColorTripletProperty.SPECULARCOLOR;
            case __GO_LIGHT_TYPE__ :
                return LightProperty.TYPE;
            case __GO_POSITION__ :
                return LightProperty.POSITION;
            case __GO_DIRECTION__ :
                return LightProperty.DIRECTION;
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
        if (property instanceof ColorTriplet.ColorTripletProperty) {
            ColorTriplet.ColorTripletProperty cp = (ColorTriplet.ColorTripletProperty)property;
            switch (cp) {
                case AMBIENTCOLOR:
                    return getAmbientColor();
                case DIFFUSECOLOR:
                    return getDiffuseColor();
                case SPECULARCOLOR:
                    return getSpecularColor();
            }
        } else if (property instanceof LightProperty) {
            LightProperty lp = (LightProperty)property;
            switch (lp) {
                case TYPE:
                    return LightType.enumToInt(getLightType());
                case POSITION:
                    return getPosition();
                case DIRECTION:
                    return getDirection();
            }
        }

        return super.getProperty(property);
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public UpdateStatus setProperty(Object property, Object value) {
        return super.setProperty(property, value);
    }

    /** returns the ambient color of the light*/
    public Double[] getAmbientColor() {
        return lightColor.getAmbientColor();
    }

    /** Sets the ambient color */
    public UpdateStatus setAmbientColor(double[] color) {
        return lightColor.setAmbientColor(color);
    }

    /** returns the dffuse color of the light*/
    public Double[] getDiffuseColor() {
        return lightColor.getDiffuseColor();
    }

    /** Sets the diffuse color of the light*/
    public UpdateStatus setDiffuseColor(double[] color) {
        return lightColor.setDiffuseColor(color);
    }

    /** returns the specular color of the light*/
    public Double[] getSpecularColor() {
        return lightColor.getSpecularColor();
    }

    /** Sets the specular color of the light*/
    public UpdateStatus setSpecularColor(double[] color) {
        return lightColor.setSpecularColor(color);
    }

    public Integer getType() {
        return __GO_LIGHT__;
    }
}
