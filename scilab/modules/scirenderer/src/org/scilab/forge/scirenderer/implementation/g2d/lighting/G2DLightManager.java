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

package org.scilab.forge.scirenderer.implementation.g2d.lighting;

import org.scilab.forge.scirenderer.lightning.Light;
import org.scilab.forge.scirenderer.lightning.LightManager;
import org.scilab.forge.scirenderer.shapes.appearance.Material;
import org.scilab.forge.scirenderer.implementation.g2d.G2DDrawingTools;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.Vector3f;



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

    /**
     * Returns the camera position used to perform the lighting.
     */
    public Vector3f getCameraPosition() {
        double[] m = drawingTools.getTransformationManager().getTransformation().getMatrix();
        //extract the translation of the matrix
        return new Vector3f((float)m[12], (float)m[13], (float)m[14]);
    }

    /**
     * Returns the vertex transformation as a float array.
     */
    public float[] getVertexTransform() {
        float[] ret = new float[16];
        double[] m = drawingTools.getTransformationManager().getTransformation().getMatrix();
        for (int i = 0; i < 16; ++i) {
            ret[i] = (float)m[i];
        }
        return ret;
    }

    /**
     * Returns the normal transformation as a float array.
     * The normal transformation is defined as the inverse transpose
     * of the vertex transformation.
     */
    public float[] getNormalTransform() {
        float[] ret = new float[16];
        double[] m = drawingTools.getTransformationManager().getTransformation().getInverseTransformation().getMatrix();

        //only the top 3x3 matrix is used.
        ret[0] = (float)m[0];
        ret[4] = (float)m[1];
        ret[8] =  (float)m[2];
        ret[12] = 0.f;
        ret[1] = (float)m[4];
        ret[5] = (float)m[5];
        ret[9] =  (float)m[6];
        ret[13] = 0.f;
        ret[2] = (float)m[8];
        ret[6] = (float)m[9];
        ret[10] = (float)m[10];
        ret[14] = 0.f;
        ret[3] = 0.f;
        ret[7] = 0.f;
        ret[11] = 0.f;
        ret[15] = 1.f;

        return ret;
    }
}
