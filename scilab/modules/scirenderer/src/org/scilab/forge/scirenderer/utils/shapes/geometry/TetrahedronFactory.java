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

package org.scilab.forge.scirenderer.utils.shapes.geometry;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 * An utility class to create a tetrahedron.
 * @author Pierre Lando
 */
public final class TetrahedronFactory {
    private static final int ELEMENTS_SIZE = 4;

    private static final Vector3d A = new Vector3d(+1, +1, +1); // Point #0
    private static final Vector3d B = new Vector3d(+1, -1, -1); // Point #1
    private static final Vector3d C = new Vector3d(-1, +1, -1); // Point #2
    private static final Vector3d D = new Vector3d(-1, -1, +1); // Point #3

    /**
     * Private constructor.
     * This is an utility class.
     */
    private TetrahedronFactory() {
    }

    /**
     * @param canvas the canvas where the buffers are created.
     * @return a Tetrahedron geometry.
     */
    public static DefaultGeometry createTetrahedron(Canvas canvas) {
        Vector3d mab = A.plus(B).times(.5); // Point #4
        Vector3d mac = A.plus(C).times(.5); // Point #5
        Vector3d mad = A.plus(D).times(.5); // Point #6

        Vector3d mbc = B.plus(C).times(.5); // Point #7
        Vector3d mbd = B.plus(D).times(.5); // Point #8

        Vector3d mcd = C.plus(D).times(.5); // Point #9

        // Create vertices buffer.
        FloatBuffer vertices = FloatBuffer.allocate(10 * ELEMENTS_SIZE);
        vertices.rewind();
        vertices.put(A.getDataAsFloatArray(ELEMENTS_SIZE));
        vertices.put(B.getDataAsFloatArray(ELEMENTS_SIZE));
        vertices.put(C.getDataAsFloatArray(ELEMENTS_SIZE));
        vertices.put(D.getDataAsFloatArray(ELEMENTS_SIZE));

        vertices.put(mab.getDataAsFloatArray(ELEMENTS_SIZE));
        vertices.put(mac.getDataAsFloatArray(ELEMENTS_SIZE));
        vertices.put(mad.getDataAsFloatArray(ELEMENTS_SIZE));

        vertices.put(mbc.getDataAsFloatArray(ELEMENTS_SIZE));
        vertices.put(mbd.getDataAsFloatArray(ELEMENTS_SIZE));

        vertices.put(mcd.getDataAsFloatArray(ELEMENTS_SIZE));
        vertices.rewind();

        // 4 triangles per faces.
        // 4 faces.
        // 3 indices by triangles.
        IntBuffer indices = IntBuffer.allocate(48);
        indices.rewind();
        indices.put(new int[] {
                        0, 4, 5,
                        4, 1, 7,
                        5, 7, 2,
                        7, 5, 4,

                        0, 5, 6,
                        5, 2, 9,
                        6, 9, 3,
                        9, 6, 5,

                        0, 6, 4,
                        6, 3, 8,
                        4, 8, 1,
                        8, 4, 6,

                        8, 9, 7,
                        9, 8, 3,
                        7, 1, 8,
                        2, 7, 9
                    });
        indices.rewind();

        IntBuffer edgesIndices = IntBuffer.allocate(48);
        edgesIndices.rewind();
        edgesIndices.put(new int[] {
                             0, 4,   4, 1,
                             0, 5,   5, 2,
                             0, 6,   6, 3,
                             4, 5,   5, 6,   6, 4,
                             1, 7,   7, 2,
                             2, 9,   9, 3,
                             3, 8,   8, 1,
                             4, 7,   7, 5,   5, 9,   9, 6,   6, 8,   8, 4,
                             7, 9,   9, 8,   8, 7
                         });

        edgesIndices.rewind();

        canvas.getBuffersManager().createElementsBuffer();
        ElementsBuffer vertexBuffer = canvas.getBuffersManager().createElementsBuffer();
        IndicesBuffer indicesBuffer = canvas.getBuffersManager().createIndicesBuffer();
        IndicesBuffer edgesIndicesBuffer = canvas.getBuffersManager().createIndicesBuffer();

        vertexBuffer.setData(vertices, ELEMENTS_SIZE);
        indicesBuffer.setData(indices);
        edgesIndicesBuffer.setData(edgesIndices);

        DefaultGeometry geometry = new DefaultGeometry();
        geometry.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
        geometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
        geometry.setPolygonOffsetMode(true);
        geometry.setWireIndices(edgesIndicesBuffer);
        geometry.setIndices(indicesBuffer);
        geometry.setVertices(vertexBuffer);

        return geometry;
    }
}
