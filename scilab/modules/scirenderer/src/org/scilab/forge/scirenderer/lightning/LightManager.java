/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.forge.scirenderer.lightning;

import org.scilab.forge.scirenderer.shapes.appearance.Material;

/**
 * Light manager interface.
 *
 * @author Pierre Lando
 */
public interface LightManager {

    /**
     * The default lightning status.
     */
    boolean DEFAULT_LIGHTNING_STATUS = false;

    /**
     * Return the number of available light.
     * @return the number of available light.
     */
    int getLightNumber();

    /**
     * Return the i-th light.
     * {@code null} is returned if i is not a valid index.
     * @param i the given index.
     * @return  the i-th light.
     */
    Light getLight(int i);

    /**
     * Set the lightning status.
     * Lighting is initially disabled.
     * When it is enabled, light sources that are enabled contribute to the lighting calculation.
     * @param isLightningEnable the new lightning status.
     */
    void setLightningEnable(boolean isLightningEnable);

    /**
     * Return the lightning status.
     * @return the lightning status.
     */
    boolean isLightningEnable();

    /**
     * Set the material properties used for lighting.
     * @param material the material.
     */
    void setMaterial(Material material);
}
