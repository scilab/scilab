/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView;

import com.sun.opengl.util.BufferUtil;
import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.modules.graphic_objects.DataLoader;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * @author Pierre Lando
 */
public class DataManager {

    /**
     * Set of properties that affect polyline data.
     */
    private static final Set<String> POLYLINE_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__,
            GraphicObjectProperties.__GO_POLYLINE_STYLE__,
            GraphicObjectProperties.__GO_LINE_MODE__,
            GraphicObjectProperties.__GO_BAR_WIDTH__,
            GraphicObjectProperties.__GO_CLOSED__
    ));


    private static final double[] DEFAULT_SCALE     = new double[] {1, 1, 1};
    private static final double[] DEFAULT_TRANSLATE = new double[] {0, 0, 0};


    private final Map<String, ElementsBuffer> vertexBufferMap = new HashMap<String, ElementsBuffer>();
    private final Map<String, IndicesBuffer> indexBufferMap = new HashMap<String, IndicesBuffer>();
    private final Canvas canvas;


    public DataManager(Canvas canvas) {
        this.canvas = canvas;
    }

    /**
     * Return the vertex buffer of the given object.
     * @param id the given object Id.
     * @return the vertex buffer of the given object.
     */
    public ElementsBuffer getVertexBuffer(String id) {
        if (vertexBufferMap.containsKey(id)) {
            return vertexBufferMap.get(id);
        } else {
            ElementsBuffer vertexBuffer = canvas.getBuffersManager().createElementsBuffer();
            fillVertexBuffer(vertexBuffer, id);
            vertexBufferMap.put(id, vertexBuffer);
            return vertexBuffer;
        }
    }

    /**
     * Return the index buffer of the given object.
     * @param id the given object Id.
     * @return the index buffer of the given object.
     */
    public IndicesBuffer getIndexBuffer(String id) {
        if (indexBufferMap.containsKey(id)) {
            return indexBufferMap.get(id);
        } else {
            IndicesBuffer indexBuffer = canvas.getBuffersManager().createIndicesBuffer();
            fillIndexBuffer(indexBuffer, id);
            indexBufferMap.put(id, indexBuffer);
            return indexBuffer;
        }
    }

    /**
     * Update the data if needed.
     * @param id the modified object.
     * @param property the changed property.
     */
    public void update(String id, String property) {
        if (POLYLINE_DATA_PROPERTIES.contains(property) && vertexBufferMap.containsKey(id)) {
            ElementsBuffer vertexBuffer = vertexBufferMap.get(id);
            if (vertexBuffer != null) {
                fillVertexBuffer(vertexBuffer, id);
            }

            IndicesBuffer indexBuffer = indexBufferMap.get(id);
            if (indexBuffer != null) {
                fillIndexBuffer(indexBuffer, id);
            }
        }
    }

    /**
     * Clear the buffer corresponding to the given object.
     * @param id object id.
     */
    public void dispose(String id) {
        if (vertexBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(vertexBufferMap.get(id));
            vertexBufferMap.remove(id);
        }

        if (indexBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(indexBufferMap.get(id));
            indexBufferMap.remove(id);
        }
    }


    private void fillVertexBuffer(ElementsBuffer vertexBuffer, String id) {
            int length = DataLoader.getDataSize(id);
            FloatBuffer data = BufferUtil.newFloatBuffer(length * 4);
            DataLoader.fillVertices(id, data, length, 4, 0x1 | 0x2 | 0x4 | 0x8, DEFAULT_SCALE, DEFAULT_TRANSLATE);
            vertexBuffer.setData(data, 4);
    }

    private void fillIndexBuffer(IndicesBuffer indexBuffer, String id) {
        int length = DataLoader.getIndicesSize(id);
        IntBuffer data = BufferUtil.newIntBuffer(length);

        int actualLength = DataLoader.fillIndices(id, data, length);

        /* Set the buffer size to the actual number of indices */
        data.limit(actualLength);

        indexBuffer.setData(data);
    }
}
