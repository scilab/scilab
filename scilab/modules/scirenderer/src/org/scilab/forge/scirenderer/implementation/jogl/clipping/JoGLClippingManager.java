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

package org.scilab.forge.scirenderer.implementation.jogl.clipping;

import org.scilab.forge.scirenderer.clipping.ClippingManager;
import org.scilab.forge.scirenderer.clipping.ClippingPlane;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLDrawingTools;

import javax.media.opengl.GL2;

/**
 * @author Pierre Lando
 */
public class JoGLClippingManager implements ClippingManager {

    /**
     * Used drawing tools.
     */
    private final JoGLDrawingTools drawingTools;

    /**
     * Clipping planes array.
     */
    private final JoGLClippingPlane[] clippingPlanes;

    /**
     * Default constructor.
     * @param drawingTools used drawing tools.
     */
    public JoGLClippingManager(JoGLDrawingTools drawingTools) {
        this.drawingTools = drawingTools;
        this.clippingPlanes = new JoGLClippingPlane[getClippingPlaneNumber()];
    }

    @Override
    public int getClippingPlaneNumber() {
        return drawingTools.getGLCapacity().getClippingPlaneNumber();
    }

    @Override
    public JoGLClippingPlane getClippingPlane(int i) {
        if (i < 0 || i >= getClippingPlaneNumber()) {
            return null;
        } else {
            if (clippingPlanes[i] == null) {
                clippingPlanes[i] = new JoGLClippingPlane(drawingTools.getGl().getGL2(), i);
            }
            return clippingPlanes[i];
        }
    }

    @Override
    public void disableClipping() {
        for (ClippingPlane clippingPlane : clippingPlanes) {
            if (clippingPlane != null) {
                clippingPlane.setEnable(false);
            }
        }
    }

    public void reload() {
        GL2 gl = drawingTools.getGl().getGL2();
        for (JoGLClippingPlane clippingPlane : clippingPlanes) {
            if (clippingPlane != null) {
                clippingPlane.reload(gl);
            }
        }
    }
}
