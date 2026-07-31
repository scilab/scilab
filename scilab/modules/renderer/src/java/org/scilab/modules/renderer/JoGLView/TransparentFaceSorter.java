/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E.
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.modules.renderer.JoGLView;

import org.scilab.forge.scirenderer.buffers.BuffersManager;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

/**
 * Computes and caches back-to-front depth-sorted fill index buffers for transparent
 * 3D surfaces (fac3d, plot3d).
 *
 * When a surface is transparent (face_alpha &lt; 1), the depth buffer is not written
 * (glDepthMask(false) in JoGLShapeDrawer), so the blending order is determined solely
 * by the order of the fill index buffer. The grid-order index buffer then blends facets
 * in an arbitrary depth order, making near facets disappear behind far ones. This class
 * reorders the triangles from the farthest to the nearest (painter's algorithm) so that
 * alpha blending is correct.
 *
 * Results are cached per object identifier and only recomputed when the view
 * transformation (rotation/zoom) or the number of indices changes. Data changes are
 * signalled by callers through {@link #invalidate(Integer)}.
 */
class TransparentFaceSorter {

    /** Vertex stride in the vertex buffer (x, y, z, w). */
    private static final int VERTEX_SIZE = 4;

    /** Depth-sorted fill index buffer, by object identifier. */
    private final Map<Integer, IndicesBuffer> sortedIndexMap = new HashMap<Integer, IndicesBuffer>();

    /** Model-view-projection matrix used for the last sort, by object identifier. */
    private final Map<Integer, double[]> sortedMatrixMap = new HashMap<Integer, double[]>();

    /** Number of indices used for the last sort, by object identifier (detects data changes). */
    private final Map<Integer, Integer> sortedSizeMap = new HashMap<Integer, Integer>();

    /**
     * Return a depth-sorted (back-to-front) copy of the given triangle index buffer.
     *
     * @param id the surface identifier (used as cache key).
     * @param baseBuffer the grid-order fill index buffer (triangle list).
     * @param vertexBuffer the surface vertex buffer (4 floats per vertex).
     * @param transformation the current model-view-projection transformation.
     * @param buffersManager the buffers manager used to allocate the sorted buffer.
     * @return the depth-sorted fill index buffer.
     */
    IndicesBuffer getSortedIndexBuffer(Integer id, IndicesBuffer baseBuffer, ElementsBuffer vertexBuffer,
                                       Transformation transformation, BuffersManager buffersManager) {
        IntBuffer baseData = baseBuffer.getData();
        final int n = baseData.limit();
        double[] matrix = transformation.getMatrix();

        /* Reuse the cached sorted buffer if neither the view nor the index count changed. */
        IndicesBuffer cached = sortedIndexMap.get(id);
        Integer cachedSize = sortedSizeMap.get(id);
        double[] cachedMatrix = sortedMatrixMap.get(id);
        if (cached != null && cachedSize != null && cachedSize == n
                && cachedMatrix != null && Arrays.equals(cachedMatrix, matrix)) {
            return cached;
        }

        FloatBuffer vertices = vertexBuffer.getData();
        final int triangleCount = n / 3;

        /* Compute each triangle centroid depth (projected z) once. */
        final double[] depth = new double[triangleCount];
        Integer[] order = new Integer[triangleCount];
        for (int tri = 0; tri < triangleCount; tri++) {
            int base = tri * 3;
            int i0 = baseData.get(base) * VERTEX_SIZE;
            int i1 = baseData.get(base + 1) * VERTEX_SIZE;
            int i2 = baseData.get(base + 2) * VERTEX_SIZE;
            double cx = (vertices.get(i0)     + vertices.get(i1)     + vertices.get(i2))     / 3.0;
            double cy = (vertices.get(i0 + 1) + vertices.get(i1 + 1) + vertices.get(i2 + 1)) / 3.0;
            double cz = (vertices.get(i0 + 2) + vertices.get(i1 + 2) + vertices.get(i2 + 2)) / 3.0;
            depth[tri] = transformation.project(new Vector3d(cx, cy, cz)).getZ();
            order[tri] = tri;
        }

        /* Sort triangles from the farthest (greatest projected z) to the nearest. */
        Arrays.sort(order, (a, b) -> Double.compare(depth[b], depth[a]));

        int[] sorted = new int[n];
        for (int k = 0; k < triangleCount; k++) {
            int base = order[k] * 3;
            sorted[k * 3]     = baseData.get(base);
            sorted[k * 3 + 1] = baseData.get(base + 1);
            sorted[k * 3 + 2] = baseData.get(base + 2);
        }

        if (cached == null) {
            cached = buffersManager.createIndicesBuffer();
            sortedIndexMap.put(id, cached);
        }
        cached.setData(sorted);
        sortedMatrixMap.put(id, matrix);
        sortedSizeMap.put(id, n);
        return cached;
    }

    /**
     * Force the sorted buffer of the given object to be recomputed on next draw.
     * To be called when the object geometry (vertices/indices) may have changed.
     * @param id the object identifier.
     */
    void invalidate(Integer id) {
        sortedMatrixMap.remove(id);
    }

    /**
     * Release the cached data for the given object.
     * @param id the object identifier.
     */
    void dispose(Integer id) {
        sortedIndexMap.remove(id);
        sortedMatrixMap.remove(id);
        sortedSizeMap.remove(id);
    }
}
