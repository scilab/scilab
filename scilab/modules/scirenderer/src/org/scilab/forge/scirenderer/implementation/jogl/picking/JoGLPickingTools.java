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

package org.scilab.forge.scirenderer.implementation.jogl.picking;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLDrawingTools;
import org.scilab.forge.scirenderer.picking.PickingTools;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import javax.media.opengl.GL2;
import java.awt.Point;
import java.nio.FloatBuffer;

/**
 *
 * JoGL implementation of {@link PickingTools}
 *
 * @author Pierre Lando
 */
public class JoGLPickingTools implements PickingTools {

    private final JoGLDrawingTools drawingTools;

    /**
     * Default constructor.
     * @param drawingTools the drawing tools to use.
     */
    public JoGLPickingTools(JoGLDrawingTools drawingTools) {
        this.drawingTools = drawingTools;
    }

    @Override
    public Vector3d getUnderlyingPoint(Point windowPosition) {
        GL2 gl = drawingTools.getGl().getGL2();

        int x = windowPosition.x;
        int y = drawingTools.getCanvas().getHeight() - windowPosition.y - 1;
        FloatBuffer buffer = FloatBuffer.allocate(1);

        buffer.rewind();
        gl.glReadPixels(x, y, 1, 1, GL2.GL_DEPTH_COMPONENT, GL2.GL_FLOAT, buffer);
        buffer.rewind();
        float vz = 2f * buffer.get() - 1;

        float vx = 2f * x / drawingTools.getCanvas().getWidth() - 1f;
        float vy = 2f * y / drawingTools.getCanvas().getHeight() - 1f;

        return new Vector3d(vx, vy, vz);
    }

    @Override
    public Canvas getCanvas() {
        return drawingTools.getCanvas();
    }

}
