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

    public enum LightProperty {ENABLED};

    /** enables lighting */
    boolean enabled;

    public Light() {
        super();
        enabled = false;
        ambient[0] = ambient[1] = ambient[2] = 1.0;
        diffuse[0] = diffuse[1] = diffuse[2] = 1.0;
        specular[0] = specular[1] = specular[2] = 1.0;
    }

    /** copy contructor */
    public Light(Light other) {
        super();
        enabled = false;
        ambient[0] = other.ambient[0];
        ambient[1] = other.ambient[1];
        ambient[2] = other.ambient[2];
        diffuse[0] = other.diffuse[0];
        diffuse[1] = other.diffuse[1];
        diffuse[2] = other.diffuse[2];
        specular[0] = other.specular[0];
        specular[1] = other.specular[1];
        specular[2] = other.specular[2];
        enabled = other.enabled;
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

}
