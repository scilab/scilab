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
package org.scilab.modules.renderer.JoGLView.util;

import org.scilab.forge.scirenderer.shapes.appearance.Material;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.lightning.Light;
import org.scilab.forge.scirenderer.lightning.LightManager;

/**
 * Utility functions to setup lighting.
 * @author Pedro SOUZA
 */
public class LightingUtils {

    /**
     * Convert the given material to a scirenderer material.
     * @param m the material.
     */
    public static Material getMaterial(org.scilab.modules.graphic_objects.lighting.Material m) {
        Material mtl = new Material();

        Double[] color = m.getAmbientColor();
        mtl.setAmbientColor(new Color(color[0].floatValue(), color[1].floatValue(), color[2].floatValue()));
        color = m.getDiffuseColor();
        mtl.setDiffuseColor(new Color(color[0].floatValue(), color[1].floatValue(), color[2].floatValue()));
        color = m.getSpecularColor();
        mtl.setSpecularColor(new Color(color[0].floatValue(), color[1].floatValue(), color[2].floatValue()));
        mtl.setColorMaterialEnable(m.getColorMaterialMode().booleanValue());
        mtl.setShinines(m.getShinines().floatValue());

        return mtl;
    }

    /**
     * Setup the give light.
     * @param manager the light manager.
     * @param light the light.
     */
    public static void setLight(LightManager manager, org.scilab.modules.graphic_objects.lighting.Light light) {

        setLightingEnable(manager, light.isEnable());

        Light sciLight = manager.getLight(0);
        Double[] color = light.getAmbientColor();
        sciLight.setAmbientColor(new Color(color[0].floatValue(), color[1].floatValue(), color[2].floatValue()));
        color = light.getDiffuseColor();
        sciLight.setDiffuseColor(new Color(color[0].floatValue(), color[1].floatValue(), color[2].floatValue()));
        color = light.getSpecularColor();
        sciLight.setSpecularColor(new Color(color[0].floatValue(), color[1].floatValue(), color[2].floatValue()));

        switch (light.getTypeAsInteger()) {
            case 0: //directional
                sciLight.setDirection(new Vector3d(light.getDirection()));
                break;
            case 1: //point
                sciLight.setPosition(new Vector3d(light.getPosition()));
                break;
            default:
                break;
        }
    }

    /**
     * Enables/disables lighting.
     * @param manager the light manager.
     * @param status the status.
     */
    public static void setLightingEnable(LightManager manager, Boolean status) {
        manager.setLightningEnable(status.booleanValue());
        Light light = manager.getLight(0);
        light.setEnable(status.booleanValue());
    }
}
