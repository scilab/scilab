/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2012 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.forge.scirenderer.implementation.g2d.clipping;

import java.util.ArrayList;
import java.util.List;

import org.scilab.forge.scirenderer.clipping.ClippingManager;
import org.scilab.forge.scirenderer.clipping.ClippingPlane;
import org.scilab.forge.scirenderer.implementation.g2d.G2DDrawingTools;

/**
 * @author Calixte DENIZET
 */
public class G2DClippingManager implements ClippingManager {

    /**
     * Used drawing tools.
     */
    private final G2DDrawingTools drawingTools;

    /**
     * Clipping planes array.
     */
    private final List<ClippingPlane> clippingPlanes;

    /**
     * Default constructor.
     * @param drawingTools used drawing tools.
     */
    public G2DClippingManager(G2DDrawingTools drawingTools) {
        this.drawingTools = drawingTools;
        this.clippingPlanes = new ArrayList<ClippingPlane>(6);
    }

    public List<ClippingPlane> getClippingPlanes() {
        return clippingPlanes;
    }

    @Override
    public int getClippingPlaneNumber() {
        return Integer.MAX_VALUE;
    }

    @Override
    public ClippingPlane getClippingPlane(int i) {
        if (i < 0 || i >= getClippingPlaneNumber()) {
            return null;
        } else {
            if (i >= clippingPlanes.size() || clippingPlanes.get(i) == null) {
                clippingPlanes.add(i, new G2DClippingPlane(i, drawingTools));
            }
            return clippingPlanes.get(i);
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
}
