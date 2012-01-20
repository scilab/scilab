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
import org.scilab.modules.graphic_objects.MainDataLoader;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;

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
     * Set of properties that affect Fac3d data.
     */
    private static final Set<String> FAC3D_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__,
            GraphicObjectProperties.__GO_COLOR_FLAG__,
            GraphicObjectProperties.__GO_COLOR_MODE__,
            GraphicObjectProperties.__GO_DATA_MAPPING__
    ));

    /**
     * Set of properties that affect Fec data.
     */
    private static final Set<String> FEC_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__,
            GraphicObjectProperties.__GO_Z_BOUNDS__,
            GraphicObjectProperties.__GO_COLOR_RANGE__,
            GraphicObjectProperties.__GO_OUTSIDE_COLOR__
    ));

    /**
     * Set of properties that affect Grayplot data.
     */
    private static final Set<String> GRAYPLOT_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__,
            GraphicObjectProperties.__GO_DATA_MAPPING__
    ));

    /**
     * Set of properties that affect Matplot data.
     */
    private static final Set<String> MATPLOT_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__
    ));

    /**
     * Set of properties that affect polyline data.
     */
    private static final Set<String> POLYLINE_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__,
            GraphicObjectProperties.__GO_POLYLINE_STYLE__,
            GraphicObjectProperties.__GO_LINE_MODE__,
            GraphicObjectProperties.__GO_BAR_WIDTH__,
            GraphicObjectProperties.__GO_CLOSED__,
            GraphicObjectProperties.__GO_FILL_MODE__,
            GraphicObjectProperties.__GO_INTERP_COLOR_VECTOR__,
            GraphicObjectProperties.__GO_INTERP_COLOR_MODE__
    ));

    /**
     * Set of properties that affect Plot3d data.
     */
    private static final Set<String> PLOT3D_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__
    ));

    /**
     * Set of properties that affect Arc data.
     */
    private static final Set<String> ARC_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_UPPER_LEFT_POINT__,
            GraphicObjectProperties.__GO_WIDTH__,
            GraphicObjectProperties.__GO_HEIGHT__,
            GraphicObjectProperties.__GO_START_ANGLE__,
            GraphicObjectProperties.__GO_END_ANGLE__
    ));

    /**
     * Set of properties that affect Rectangle data.
     */
    private static final Set<String> RECTANGLE_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_UPPER_LEFT_POINT__,
            GraphicObjectProperties.__GO_WIDTH__,
            GraphicObjectProperties.__GO_HEIGHT__
    ));

    /**
     * Set of properties that affect Segs data.
     */
    private static final Set<String> SEGS_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_BASE__,
            GraphicObjectProperties.__GO_DIRECTION__,
            GraphicObjectProperties.__GO_SEGS_COLORS__
    ));

    private static final double[] DEFAULT_SCALE     = new double[] {1, 1, 1};
    private static final double[] DEFAULT_TRANSLATE = new double[] {0, 0, 0};

    /*
     * Bit mask specifying whether logarithmic coordinates are used.
     * Temporarily defined as a constant for now and set to 0 (linear x, y and z coordinates).
     * To do: pass it as an argument of fillVertexBuffer and fillWireIndexBuffer, when updating data.
     */
    private static final int DEFAULT_LOG_MASK = 0;


    private final Map<String, ElementsBuffer> vertexBufferMap = new HashMap<String, ElementsBuffer>();
    private final Map<String, ElementsBuffer> colorBufferMap = new HashMap<String, ElementsBuffer>();
    private final Map<String, IndicesBuffer> indexBufferMap = new HashMap<String, IndicesBuffer>();
    private final Map<String, IndicesBuffer> wireIndexBufferMap = new HashMap<String, IndicesBuffer>();
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
     * Return the color buffer of the given object.
     * @param id the given object Id.
     * @return the color buffer of the given object.
     */
    public ElementsBuffer getColorBuffer(String id) {
        if (colorBufferMap.containsKey(id)) {
            return colorBufferMap.get(id);
        } else {
            ElementsBuffer colorBuffer = canvas.getBuffersManager().createElementsBuffer();
            fillColorBuffer(colorBuffer, id);
            colorBufferMap.put(id, colorBuffer);
            return colorBuffer;
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
     * Return the wire index buffer of the given object.
     * @param id the given object Id.
     * @return the wire index buffer of the given object.
     */
    public IndicesBuffer getWireIndexBuffer(String id) {
        if (wireIndexBufferMap.containsKey(id)) {
            return wireIndexBufferMap.get(id);
        } else {
            IndicesBuffer indexBuffer = canvas.getBuffersManager().createIndicesBuffer();
            fillWireIndexBuffer(indexBuffer, id);
            wireIndexBufferMap.put(id, indexBuffer);
            return indexBuffer;
        }
    }

    /**
     * Update the data if needed.
     * @param id the modified object.
     * @param property the changed property.
     */
    public void update(String id, String property) {
        String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        if (vertexBufferMap.containsKey(id)) {
            if ((type.equals(GraphicObjectProperties.__GO_FAC3D__) && FAC3D_DATA_PROPERTIES.contains(property))
               || (type.equals(GraphicObjectProperties.__GO_FEC__) && FEC_DATA_PROPERTIES.contains(property))
               || (type.equals(GraphicObjectProperties.__GO_GRAYPLOT__) && GRAYPLOT_DATA_PROPERTIES.contains(property))
               || (type.equals(GraphicObjectProperties.__GO_MATPLOT__) && MATPLOT_DATA_PROPERTIES.contains(property))
               || (type.equals(GraphicObjectProperties.__GO_POLYLINE__) && POLYLINE_DATA_PROPERTIES.contains(property))
               || (type.equals(GraphicObjectProperties.__GO_PLOT3D__) && PLOT3D_DATA_PROPERTIES.contains(property))
               || (type.equals(GraphicObjectProperties.__GO_ARC__) && ARC_DATA_PROPERTIES.contains(property))
               || (type.equals(GraphicObjectProperties.__GO_RECTANGLE__) && RECTANGLE_DATA_PROPERTIES.contains(property))
               || (type.equals(GraphicObjectProperties.__GO_SEGS__) && SEGS_DATA_PROPERTIES.contains(property))) {
                fillBuffers(id);
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

        if (colorBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(colorBufferMap.get(id));
            colorBufferMap.remove(id);
        }

        if (indexBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(indexBufferMap.get(id));
            indexBufferMap.remove(id);
        }

        if (wireIndexBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(wireIndexBufferMap.get(id));
            wireIndexBufferMap.remove(id);
        }
    }

    /**
     * Clears all the color buffers.
     */
    public void disposeAllColorBuffers() {
        canvas.getBuffersManager().dispose(colorBufferMap.values());
        colorBufferMap.clear();
    }

    /**
     * Fill the vertex, color, index and wire index buffers
     * of a given object.
     * @param id the object id.
     */
    private void fillBuffers(String id) {
        ElementsBuffer vertexBuffer = vertexBufferMap.get(id);
        if (vertexBuffer != null) {
            fillVertexBuffer(vertexBuffer, id);
        }

        ElementsBuffer colorBuffer = colorBufferMap.get(id);
        if (colorBuffer != null) {
            fillColorBuffer(colorBuffer, id);
        }

        IndicesBuffer indexBuffer = indexBufferMap.get(id);
        if (indexBuffer != null) {
            fillIndexBuffer(indexBuffer, id);
        }

        IndicesBuffer wireIndexBuffer = wireIndexBufferMap.get(id);
        if (wireIndexBuffer != null) {
            fillWireIndexBuffer(wireIndexBuffer, id);
        }
    }

    private void fillVertexBuffer(ElementsBuffer vertexBuffer, String id) {
            int length = MainDataLoader.getDataSize(id);
            FloatBuffer data = BufferUtil.newFloatBuffer(length * 4);
            MainDataLoader.fillVertices(id, data, 4, 0x1 | 0x2 | 0x4 | 0x8, DEFAULT_SCALE, DEFAULT_TRANSLATE, DEFAULT_LOG_MASK);
            vertexBuffer.setData(data, 4);
    }

    private void fillColorBuffer(ElementsBuffer colorBuffer, String id) {
            int length = MainDataLoader.getDataSize(id);
            FloatBuffer data = BufferUtil.newFloatBuffer(length * 4);
            MainDataLoader.fillColors(id, data, 4);
            colorBuffer.setData(data, 4);
    }

    private void fillIndexBuffer(IndicesBuffer indexBuffer, String id) {
        int length = MainDataLoader.getIndicesSize(id);
        IntBuffer data = BufferUtil.newIntBuffer(length);

        int actualLength = 0;
        if (length != 0) {
            /* Do not call JNI when the buffer is empty */
            /* Because under Mac OS X, GetDirectBufferAddress returns a NULL pointer in this case */
            /* This generates an exception in DataLoader_wrap.c */
            actualLength = MainDataLoader.fillIndices(id, data, DEFAULT_LOG_MASK);
        }

        /* Set the buffer size to the actual number of indices */
        data.limit(actualLength);

        indexBuffer.setData(data);
    }

    private void fillWireIndexBuffer(IndicesBuffer indexBuffer, String id) {
        int length = MainDataLoader.getWireIndicesSize(id);
        IntBuffer data = BufferUtil.newIntBuffer(length);

        int actualLength = 0;
        if (length != 0) {
            /* Do not call JNI when the buffer is empty */
            /* Because under Mac OS X, GetDirectBufferAddress returns a NULL pointer in this case */
            /* This generates an exception in DataLoader_wrap.c */
            actualLength = MainDataLoader.fillWireIndices(id, data, DEFAULT_LOG_MASK);
        }

        /* Set the buffer size to the actual number of indices */
        data.limit(actualLength);

        indexBuffer.setData(data);
    }
}
