/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.forge.scirenderer.implementation.g2d.lighting;

import org.scilab.forge.scirenderer.lightning.Light;
import org.scilab.forge.scirenderer.lightning.LightManager;
import org.scilab.forge.scirenderer.shapes.appearance.Material;
import org.scilab.forge.scirenderer.implementation.g2d.G2DDrawingTools;



/**
 * @author Pedro SOUZA
 */
public class G2DLightManager implements LightManager {

    /**
     * The drawing tools.
     */
    private final G2DDrawingTools drawingTools;

    /**
     * The lights.
     */
    private final G2DLight[] lights;

    /**
     * The current lightning status.
     */
    private boolean isLightningEnable = DEFAULT_LIGHTNING_STATUS;

    private Material material;

    /**
     * Default constructor.
     * @param drawingTools the drawing tools.
     */
    public G2DLightManager(G2DDrawingTools drawingTools) {
        this.drawingTools = drawingTools;
        lights = new G2DLight[getLightNumber()];
    }

    @Override
    public int getLightNumber() {
        return 16;
    }

    @Override
    public Light getLight(int i) {
        if (i < 0 || i >= getLightNumber()) {
            return null;
        } else {
            if (lights[i] == null) {
                lights[i] = new G2DLight(i);
            }
            return lights[i];
        }
    }

    @Override
    public void setLightningEnable(boolean isLightningEnable) {
        this.isLightningEnable = isLightningEnable;
    }

    @Override
    public boolean isLightningEnable() {
        return isLightningEnable;
    }

    @Override
    public void setMaterial(Material material) {
        this.material = material;
    }
	
	public Material getMaterial() {
        return material;
    }
}
