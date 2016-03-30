/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
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

package org.scilab.modules.renderer.JoGLView.axes;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.utils.shapes.geometry.CubeFactory;

/**
 * @author Pierre Lando
 */
class Geometries {
    private static final int ELEMENT_SIZE = 4;

    private final Geometry cubeGeometry;
    private final DefaultGeometry boxBorderGeometry;
    private final DefaultGeometry frontBoxBorderGeometry;
    private final DefaultGeometry hiddenBoxBorderGeometry;

    private final ElementsBuffer boxBorderVertices;
    private static final float[] boxBorderVerticesData = new float[] {
        -1, -1, +1, +1,
        -1, +1, +1, +1,
        -1, +1, -1, +1,
        +1, +1, -1, +1,
        +1, -1, -1, +1,
        +1, -1, +1, +1
    };

    private final ElementsBuffer hiddenBoxBorderVertices;
    private static final float[] hiddenBoxBorderVerticesData = new float[] {
        -1, -1, -1, +1,
        +1, -1, -1, +1,
        -1, -1, -1, +1,
        -1, +1, -1, +1,
        -1, -1, -1, +1,
        -1, -1, +1, +1
    };

    private final ElementsBuffer frontBoxBorderVertices;
    private static final float[] frontBoxBorderVerticesData = new float[] {
        +1, +1, +1, +1,
        -1, +1, +1, +1,
        +1, +1, +1, +1,
        +1, -1, +1, +1,
        +1, +1, +1, +1,
        +1, +1, -1, +1
    };

    public Geometries(Canvas canvas) {

        boxBorderVertices = canvas.getBuffersManager().createElementsBuffer();
        boxBorderVertices.setData(boxBorderVerticesData, ELEMENT_SIZE);

        hiddenBoxBorderVertices = canvas.getBuffersManager().createElementsBuffer();
        hiddenBoxBorderVertices.setData(hiddenBoxBorderVerticesData, ELEMENT_SIZE);

        frontBoxBorderVertices = canvas.getBuffersManager().createElementsBuffer();
        frontBoxBorderVertices.setData(frontBoxBorderVerticesData, ELEMENT_SIZE);

        cubeGeometry = CubeFactory.createCube(canvas);

        hiddenBoxBorderGeometry = new DefaultGeometry();
        hiddenBoxBorderGeometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
        hiddenBoxBorderGeometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
        hiddenBoxBorderGeometry.setVertices(hiddenBoxBorderVertices);

        frontBoxBorderGeometry = new DefaultGeometry();
        frontBoxBorderGeometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
        frontBoxBorderGeometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
        frontBoxBorderGeometry.setVertices(frontBoxBorderVertices);

        boxBorderGeometry = new DefaultGeometry();
        boxBorderGeometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
        boxBorderGeometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS_LOOP);
        boxBorderGeometry.setVertices(boxBorderVertices);
    }

    /* TODO !
        canvas.getBuffersManager().dispose(boxBorderVertices);
        canvas.getBuffersManager().dispose(hiddenBoxBorderVertices);
        canvas.getBuffersManager().dispose(frontBoxBorderVertices);
        canvas.getBuffersManager().dispose(cubeGeometry);
        */


    public Geometry getCubeGeometry() {
        return cubeGeometry;
    }

    public Geometry getBoxBorderGeometry() {
        return boxBorderGeometry;
    }

    public Geometry getFrontBoxBorderGeometry() {
        return frontBoxBorderGeometry;
    }

    public Geometry getHiddenBoxBorderGeometry() {
        return hiddenBoxBorderGeometry;
    }
}
