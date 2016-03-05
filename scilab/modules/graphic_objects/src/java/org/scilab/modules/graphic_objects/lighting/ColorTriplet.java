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

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * Color triplet used for lighting.
 * @author Pedro SOUZA
 */

public class ColorTriplet {

    /** color triplet properties */
    public enum ColorTripletProperty {AMBIENTCOLOR, DIFFUSECOLOR, SPECULARCOLOR};
    /** ambient color */
    protected Double[] ambient;
    /** diffuse color */
    protected Double[] diffuse;
    /** specular color */
    protected Double[] specular;

    public ColorTriplet() {
        ambient = new Double[] {0.0, 0.0, 0.0};
        diffuse = new Double[] {0.0, 0.0, 0.0};
        specular = new Double[] {0.0, 0.0, 0.0};
    }

    /** copy constructor */
    public ColorTriplet(ColorTriplet other) {
        ambient = new Double[3];
        diffuse = new Double[3];
        specular = new Double[3];
        setAmbientColor(other.ambient);
        setDiffuseColor(other.diffuse);
        setSpecularColor(other.specular);
    }


    /** returns the ambient color */
    public Double[] getAmbientColor() {
        Double[] ret = new Double[] {ambient[0], ambient[1], ambient[2]};
        return ret;
    }

    /** Sets the ambient color */
    public UpdateStatus setAmbientColor(Double[] color) {
        if (color.length != 3 || !isColorValid(color)) {
            return UpdateStatus.Fail;
        }

        if (ambient[0] != color[0] || ambient[1] != color[1] || ambient[2] != color[2]) {
            ambient[0] = color[0];
            ambient[1] = color[1];
            ambient[2] = color[2];
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }

    /** returns the dffuse color */
    public Double[] getDiffuseColor() {
        Double[] ret = new Double[] {diffuse[0], diffuse[1], diffuse[2]};
        return ret;
    }

    /** Sets the diffuse color */
    public UpdateStatus setDiffuseColor(Double[] color) {
        if (color.length != 3 || !isColorValid(color)) {
            return UpdateStatus.Fail;
        }

        if (diffuse[0] != color[0] || diffuse[1] != color[1] || diffuse[2] != color[2]) {
            diffuse[0] = color[0];
            diffuse[1] = color[1];
            diffuse[2] = color[2];
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }

    /** returns the specular color */
    public Double[] getSpecularColor() {
        Double[] ret = new Double[] {specular[0], specular[1], specular[2]};
        return ret;
    }

    /** Sets the specular color */
    public UpdateStatus setSpecularColor(Double[] color) {
        if (color.length != 3 || !isColorValid(color)) {
            return UpdateStatus.Fail;
        }

        if (specular[0] != color[0] || specular[1] != specular[1] || specular[2] != color[2]) {
            specular[0] = color[0];
            specular[1] = color[1];
            specular[2] = color[2];
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }

    private boolean isColorValid(Double[] color) {
        for (Double d : color) {
            if (d < 0.0 || d > 1.0) {
                return false;
            }
        }
        return true;
    }
}
