/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
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

package org.scilab.modules.graphic_objects.lighting;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.legend.Legend.LegendLocation;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Light class
 * @author Pedro SOUZA
 */
public class Light extends GraphicObject {

    public enum LightProperty {POSITION, DIRECTION, TYPE};

    public enum LightType {DIRECTIONAL, POINT;

    public static LightType intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return DIRECTIONAL;
            case 1:
                return POINT;
            default:
                return null;
        }
    }
                          };


    /** light position */
    Double[] position;

    /** light direction */
    Double[] direction;

    /** light type */
    LightType type;

    /** the light color */
    ColorTriplet lightColor;

    public Light() {
        super();
        lightColor = new ColorTriplet();

        position = new Double[] {0.0, 0.0, 1.0};
        direction = new Double[] {0.0, 0.0, 1.0};
        type = LightType.POINT;

        Double[] dark_gray = new Double[] {0.1, 0.1, 0.1};
        Double[] white = new Double[] {1.0, 1.0, 1.0};
        setAmbientColor(dark_gray);
        setDiffuseColor(white);
        setSpecularColor(white);
    }

    /** copy contructor */
    public Light(Light other) {
        super();
        position = new Double[3];
        direction = new Double[3];
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
    public UpdateStatus setPosition(Double[] pos) {
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
    public UpdateStatus setDirection(Double[] dir) {
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

    /** Sets the light's type from an integer */
    public UpdateStatus setLightTypeAsInteger(Integer i) {
        if (i >= 0 && i < LightType.values().length) {
            if (this.type != LightType.values()[i]) {
                this.type = LightType.values()[i];
                return UpdateStatus.Success;
            }
            return UpdateStatus.NoChange;
        }
        return UpdateStatus.Fail;
    }

    /** Get the light's type as integer */
    public Integer getLightTypeAsInteger() {
        return type.ordinal();
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
                    return getLightTypeAsInteger();
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
        if (property instanceof ColorTriplet.ColorTripletProperty) {
            ColorTriplet.ColorTripletProperty cp = (ColorTriplet.ColorTripletProperty)property;
            switch (cp) {
                case AMBIENTCOLOR:
                    return setAmbientColor((Double[])value);
                case DIFFUSECOLOR:
                    return setDiffuseColor((Double[])value);
                case SPECULARCOLOR:
                    return setSpecularColor((Double[])value);
            }
        } else if (property instanceof LightProperty) {
            LightProperty lp = (LightProperty)property;
            switch (lp) {
                case TYPE:
                    return setLightTypeAsInteger((Integer)value);
                case POSITION:
                    return setPosition((Double[])value);
                case DIRECTION:
                    return setDirection((Double[])value);
            }
        }

        return super.setProperty(property, value);
    }

    /** returns the ambient color of the light*/
    public Double[] getAmbientColor() {
        return lightColor.getAmbientColor();
    }

    /** Sets the ambient color */
    public UpdateStatus setAmbientColor(Double[] color) {
        return lightColor.setAmbientColor(color);
    }

    /** returns the dffuse color of the light*/
    public Double[] getDiffuseColor() {
        return lightColor.getDiffuseColor();
    }

    /** Sets the diffuse color of the light*/
    public UpdateStatus setDiffuseColor(Double[] color) {
        return lightColor.setDiffuseColor(color);
    }

    /** returns the specular color of the light*/
    public Double[] getSpecularColor() {
        return lightColor.getSpecularColor();
    }

    /** Sets the specular color of the light*/
    public UpdateStatus setSpecularColor(Double[] color) {
        return lightColor.setSpecularColor(color);
    }

    public Integer getType() {
        return __GO_LIGHT__;
    }
}
