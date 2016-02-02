/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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

package org.scilab.modules.renderer.JoGLView.interaction.util;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.buffers.BuffersManager;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.renderer.JoGLView.interaction.RubberBox;

import java.nio.FloatBuffer;

/**
 * @author Pierre Lando
 */
public class HelpersGeometry extends DefaultGeometry {

    /** The helper size */
    private static final double HELPERS_SIZE = .05;

    /**
     * There are:
     *  - 4 marks per axis.
     *  - 2 segments per marks.
     *  - 2 vertex per segments.
     *  - 4 coordinate per vertex.
     */
    private static final int VERTEX_BUFFER_SIZE = 64;

    private final BuffersManager bufferManager;
    private final ElementsBuffer vertexBuffer;

    public HelpersGeometry(DrawingTools drawingTools) {
        bufferManager = drawingTools.getCanvas().getBuffersManager();
        vertexBuffer = bufferManager.createElementsBuffer();

        setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
        setFillDrawingMode(Geometry.FillDrawingMode.NONE);
        setVertices(vertexBuffer);
    }

    @Override
    public void finalize() throws Throwable {
        super.finalize();
        bufferManager.dispose(vertexBuffer);
    }

    public void updateVertex(Axes axes, PointComputer pointAComputer, Vector3d secondPoint, RubberBox.Status status) {
        if ((pointAComputer != null) && (pointAComputer.getFirstAxisIndex() != -1)) {
            FloatBuffer data;
            boolean oneAxis;
            if ((status == RubberBox.Status.WAIT_POINT_A) || (status == RubberBox.Status.WAIT_POINT_B)) {
                oneAxis = false;
                data = FloatBuffer.allocate(VERTEX_BUFFER_SIZE * 2);
            } else {
                oneAxis = true;
                data = FloatBuffer.allocate(VERTEX_BUFFER_SIZE);
            }

            Double[] bounds = axes.getCorrectedBounds();
            int mainAxisIndex = (pointAComputer.getFirstAxisIndex() + 1) % 3;
            for (int axisIndex = 0; axisIndex < 3; axisIndex++) {
                if ((mainAxisIndex != axisIndex) ^ oneAxis) {
                    for (int u = 0; u < 2; u++) {
                        for (int v = 0; v < 2; v++) {
                            double base[] = secondPoint.getData();
                            base[axisIndex] = bounds[axisIndex * 2 + u];
                            int nextAxisIndex = (axisIndex + 1) % 3;
                            base[nextAxisIndex] = bounds[nextAxisIndex * 2 + v];
                            float[] buffer = asFloatArray(base);
                            data.put(buffer);

                            buffer[axisIndex] = buffer[axisIndex] + (float) (HELPERS_SIZE * (bounds[axisIndex * 2 + (1 - u)] - bounds[axisIndex * 2 + u]));
                            data.put(buffer);

                            buffer[axisIndex] = bounds[axisIndex * 2 + u].floatValue();
                            data.put(buffer);

                            buffer[nextAxisIndex] = buffer[nextAxisIndex] + (float) (HELPERS_SIZE * (bounds[nextAxisIndex * 2 + (1 - v)] - bounds[nextAxisIndex * 2 + v]));
                            data.put(buffer);
                        }
                    }
                }
            }

            data.rewind();
            getVertices().setData(data, 3);
        } else {
            getVertices().setData(new Float[0], 4);
        }
    }

    private float[] asFloatArray(double[] base) {
        float data[] = new float[base.length];
        for (int i = 0; i < base.length; i++) {
            data[i] = (float) base[i];
        }
        return data;
    }
}
