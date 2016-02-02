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
import org.scilab.forge.scirenderer.buffers.BuffersManager;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 * An utility class to create cube geometry.
 * @author Pierre Lando
 */
public final class CubeFactory {

    /**
     * The vertex size.
     */
    private static final int VERTEX_SIZE = 4;

    /**
     * The constructor is private : this is an utility class.
     */
    private CubeFactory() {
    }

    /**
     * Return a geometry representing a simple cube.
     * @param canvas the canvas where the buffers are created.
     * @return a geometry representing a simple cube.
     */
    public static Geometry createCube(Canvas canvas) {
        return createCube(canvas, 1);
    }

    /**
     * Return a geometry representing a cube.
     * Each cube face are made of density^2 square.
     *
     * @param canvas the canvas where the geometry buffers are created.
     * @param density the square density on cube faces.
     * @return a geometry representing a cube.
     */
    public static Geometry createCube(Canvas canvas, int density) {
        return createCube(canvas, density, false);
    }

    /**
     * Return a geometry representing a cube.
     * Each cube face are made of density^2 square.
     *
     * @param canvas the canvas where the geometry buffers are created.
     * @param density the square density on cube faces.
     * @param wired true if the wire is visible.
     * @return a geometry representing a cube.
     */
    public static Geometry createCube(Canvas canvas, int density, boolean wired) {
        if (density < 1) {
            return null;
        } else {
            int borderLength = density + 1;
            FloatBuffer vertices = FloatBuffer.allocate(6 * borderLength * borderLength * 4);
            FloatBuffer normals = FloatBuffer.allocate(6 * borderLength * borderLength * 4);
            IntBuffer indices = IntBuffer.allocate(6 * 6 * density * density);
            IntBuffer wireIndices = IntBuffer.allocate(8 * 6 * density * density);

            int shift = 0;
            for (int axes = 0; axes < 3; axes++) {
                for (int orientation : new int[] { -1, 1}) {
                    float[] normal = getVectorPermutation(axes, 0, 0, orientation);
                    for (int i = 0; i < borderLength; i++) {
                        for (int j = 0; j < borderLength; j++) {
                            float s = (2f * i) / density - 1f;
                            float t = (2f * j) / density - 1f;
                            vertices.put(getVectorPermutation(axes, s, t, orientation));
                            normals.put(normal);
                        }
                    }
                    for (int i = 0; i < density; i++) {
                        for (int j = 0; j < density; j++) {
                            int index = shift + j + i * borderLength;
                            indices.put(index);
                            indices.put(index + 1);
                            indices.put(index + borderLength);

                            indices.put(index + 1);
                            indices.put(index + borderLength + 1);
                            indices.put(index + borderLength);

                            wireIndices.put(index);
                            wireIndices.put(index + 1);
                            wireIndices.put(index + 1);
                            wireIndices.put(index + borderLength + 1);
                            wireIndices.put(index + borderLength + 1);
                            wireIndices.put(index + borderLength);
                            wireIndices.put(index + borderLength);
                            wireIndices.put(index);
                        }
                    }
                    shift += borderLength * borderLength;
                }
            }

            vertices.rewind();
            normals.rewind();
            indices.rewind();

            BuffersManager bm = canvas.getBuffersManager();
            final ElementsBuffer verticesBuffer = bm.createElementsBuffer();
            final ElementsBuffer normalsBuffer = bm.createElementsBuffer();
            final IndicesBuffer indicesBuffer = bm.createIndicesBuffer();

            verticesBuffer.setData(vertices, VERTEX_SIZE);
            normalsBuffer.setData(normals, VERTEX_SIZE);
            indicesBuffer.setData(indices);

            DefaultGeometry geometry = new DefaultGeometry();
            geometry.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
            geometry.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
            geometry.setVertices(verticesBuffer);
            geometry.setNormals(normalsBuffer);
            geometry.setIndices(indicesBuffer);

            if (wired) {
                final IndicesBuffer wireIndicesBuffer = bm.createIndicesBuffer();
                wireIndicesBuffer.setData(wireIndices);
                geometry.setWireIndices(wireIndicesBuffer);
                geometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                geometry.setPolygonOffsetMode(true);
            }

            return geometry;
        }
    }

    /**
     * Return a permutation of the [s; t; u] vector.
     * @param permutation the permutation factor.
     * @param s x coordinate of the given vector.
     * @param t y coordinate of the given vector.
     * @param u z coordinate of the given vector.
     * @return a permutation of the [s; t; u] vector.
     */
    private static float[] getVectorPermutation(int permutation, float s, float t, float u) {
        switch (permutation % 3) {
            case 0:
                return new float[] {s, t, u, 1};
            case 1:
                return new float[] {u, s, t, 1};
            case 2:
                return new float[] {t, u, s, 1};
            default:
                return null;
        }
    }
}
