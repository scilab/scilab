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

import org.scilab.modules.graphic_objects.lighting.ColorTriplet;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * Material class
 * @author Pedro SOUZA
 */

public class Material extends ColorTriplet {

    /** material properties */
    public enum MaterialProperty {COLOR_MATERIAL, SHININESS};
    /** Specifies if the sufarce color will be used as diffuse color */
    private Boolean useColorMaterial;

    /** Specular shininess parameter*/
    private Double shininess;


    public Material() {
        super();
        useColorMaterial = true;
        ambient[0] = ambient[1] = ambient[2] = 0.0;
        diffuse[0] = diffuse[1] = diffuse[2] = 1.0;
        specular[0] = specular[1] = specular[2] = 1.0;
        shininess = 2.0;
    }

    /**
     * Sets color-material mode, if true
     * the surface color is used as diffuse color.
     */
    public UpdateStatus setColorMaterialMode(Boolean status) {
        if (useColorMaterial != status) {
            useColorMaterial = status;
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }

    /** returns the color-material status*/
    public Boolean getColorMaterialMode() {
        return useColorMaterial;
    }

    /** Sets shininess level*/
    public UpdateStatus setShininess(Double s) {
        if (shininess != s) {
            shininess = s;
            return UpdateStatus.Success;
        }
        return UpdateStatus.NoChange;
    }

    /** Gets shininess level*/
    public Double getShininess() {
        return shininess;
    }

}
