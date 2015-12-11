/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
