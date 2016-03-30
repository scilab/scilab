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
 */
package org.scilab.modules.renderer.JoGLView.util;

import org.scilab.forge.scirenderer.shapes.appearance.Material;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.lightning.Light;
import org.scilab.forge.scirenderer.lightning.LightManager;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;

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
        mtl.setShininess(m.getShininess().floatValue());

        return mtl;
    }

    /**
     * Enables/disables lighting.
     * @param manager the light manager.
     * @param status the status.
     */
    public static void setLightingEnable(LightManager manager, Boolean status) {
        if (manager == null) {
            return;
        }
        manager.setLightningEnable(status.booleanValue());
        Light light = manager.getLight(0);
        light.setEnable(status.booleanValue());
    }


    public static void setupLights(LightManager manager, org.scilab.modules.graphic_objects.axes.Axes axes) {

        if (manager == null) {
            return;
        }

        boolean hasLight = false;
        int index = 0;

        for (Integer child : axes.getChildren()) {
            GraphicObject object = GraphicController.getController().getObjectFromId(child);
            if (object instanceof org.scilab.modules.graphic_objects.lighting.Light) {
                //setup only visible lights
                if (((org.scilab.modules.graphic_objects.lighting.Light)object).getVisible()) {
                    setLight(manager, (org.scilab.modules.graphic_objects.lighting.Light)object, index++, axes);
                    hasLight = true;
                }
            }
            if (index >= manager.getLightNumber()) {
                break;
            }
        }
        for (int i = index; i < manager.getLightNumber(); ++i) {
            //manager.getLight(i).setEnable(false);
        }
        manager.setLightningEnable(hasLight);
    }

    /**
     * Setup the give light.
     * @param manager the light manager.
     * @param light the light.
     * @param index the light index.
     */
    public static void setLight(LightManager manager, org.scilab.modules.graphic_objects.lighting.Light light, int index, org.scilab.modules.graphic_objects.axes.Axes axes) {
        Light sciLight = manager.getLight(index);
        double[][] factors = axes.getScaleTranslateFactors();
        Double[] coords = light.getLightTypeAsInteger() == 0 ? light.getDirection() : light.getPosition();
        coords[0] = coords[0] * factors[0][0] + factors[1][0];
        coords[1] = coords[1] * factors[0][1] + factors[1][1];
        coords[2] = coords[2] * factors[0][2] + factors[1][2];

        Double[] color = light.getAmbientColor();
        sciLight.setAmbientColor(new Color(color[0].floatValue(), color[1].floatValue(), color[2].floatValue()));
        color = light.getDiffuseColor();
        sciLight.setDiffuseColor(new Color(color[0].floatValue(), color[1].floatValue(), color[2].floatValue()));
        color = light.getSpecularColor();
        sciLight.setSpecularColor(new Color(color[0].floatValue(), color[1].floatValue(), color[2].floatValue()));

        if (light.getLightTypeAsInteger() == 0) {
            sciLight.setDirection(new Vector3d(coords));
        } else {
            sciLight.setPosition(new Vector3d(coords));
        }

        sciLight.setEnable(true);
    }
}
