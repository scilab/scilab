/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.lighting;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * Light class
 * @author Pedro SOUZA
 */
public class Light extends ColorTriplet {

    public enum LightProperty {ENABLED, POSITION, DIRECTION, TYPE};
    
    public enum LightType {DIRECTIONAL, POINT};

    /** enables lighting */
    boolean enabled;

    /** light position */
    Double[] position;

    /** light direction */
    Double[] direction;

    /** light type */
    LightType type;

    public Light() {
        super();
        enabled = false;
        ambient[0] = ambient[1] = ambient[2] = 1.0;
        diffuse[0] = diffuse[1] = diffuse[2] = 1.0;
        specular[0] = specular[1] = specular[2] = 1.0;

        position = new Double[] {0.0, 0.0, 0.0};
        //normalized (1, 1, -1)
        direction = new Double[] {0.577350269, 0.577350269, -0.577350269};
        type = LightType.DIRECTIONAL;
    }

    /** copy contructor */
    public Light(Light other) {
        super((ColorTriplet)other);
        position = new Double[3];
        direction = new Double[3];
        enabled = other.enabled;
        type = other.type;
        setPosition(other.position);
        setDirection(other.direction);
    }

    /** enables/disables lighting */
    public UpdateStatus setEnabled(Boolean status) {
        if (enabled != status) {
            enabled = status;
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }

    /** returns lighting status */
    public Boolean isEnabled() {
        return enabled;
    }

    /** returns the light's position */
    public Double[] getPosition() {
        Double[] ret = new Double[]{position[0], position[1], position[2]};
        return ret;
    }

    /** Sets the light's position */
    public UpdateStatus setPosition(Double[] pos) {
        if (pos.length != 3) return UpdateStatus.Fail;

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
        Double[] ret = new Double[]{direction[0], direction[1], direction[2]};
        return ret;
    }

    /** Sets the light's direction */
    public UpdateStatus setDirection(Double[] dir) {
        if (dir.length != 3) return UpdateStatus.Fail;

        if (direction[0] != dir[0] || direction[1] != dir[1] || direction[2] != dir[2]) {
            direction[0] = dir[0];
            direction[1] = dir[1];
            direction[2] = dir[2];
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }

    /** Sets the light's type from an integer */
    public UpdateStatus setTypeAsInteger(Integer i) {
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
    public Integer getTypeAsInteger() {
        return type.ordinal();
    }

    /** Sets the light's type */
    public UpdateStatus setType(LightType type) {
        if (this.type != type) {
            this.type = type;
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }

    /** Get the light's type */
    public LightType getType() {
        return type;
    }

}
