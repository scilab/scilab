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

package org.scilab.modules.renderer.JoGLView;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.modules.graphic_objects.MainDataLoader;
import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.JoGLView.util.BufferAllocation;
import org.scilab.modules.renderer.JoGLView.util.OutOfMemoryException;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/**
 * @author Pierre Lando
 */
public class DataManager {

    /**
     * Set of properties that affect Fac3d data.
     */
    private static final Set<Integer> FAC3D_DATA_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_DATA_MODEL__,
                GraphicObjectProperties.__GO_COLOR_FLAG__,
                GraphicObjectProperties.__GO_COLOR_MODE__,
                GraphicObjectProperties.__GO_DATA_MAPPING__
            ));

    /**
     * Set of properties that affect Fec data.
     */
    private static final Set<Integer> FEC_DATA_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_DATA_MODEL__,
                GraphicObjectProperties.__GO_Z_BOUNDS__,
                GraphicObjectProperties.__GO_COLOR_RANGE__,
                GraphicObjectProperties.__GO_OUTSIDE_COLOR__
            ));

    /**
     * Set of properties that affect Grayplot data.
     */
    private static final Set<Integer> GRAYPLOT_DATA_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_DATA_MODEL__,
                GraphicObjectProperties.__GO_DATA_MAPPING__
            ));

    /**
     * Set of properties that affect Matplot data.
     */
    private static final Set<Integer> MATPLOT_DATA_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_DATA_MODEL__
            ));

    /**
     * Set of properties that affect polyline data.
     */
    private static final Set<Integer> POLYLINE_DATA_PROPERTIES = new HashSet<Integer>(Arrays.asList(
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
    private static final Set<Integer> PLOT3D_DATA_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_DATA_MODEL__
            ));

    /**
     * Set of properties that affect Arc data.
     */
    private static final Set<Integer> ARC_DATA_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_UPPER_LEFT_POINT__,
                GraphicObjectProperties.__GO_WIDTH__,
                GraphicObjectProperties.__GO_HEIGHT__,
                GraphicObjectProperties.__GO_START_ANGLE__,
                GraphicObjectProperties.__GO_END_ANGLE__
            ));

    /**
     * Set of properties that affect Champ data.
     */
    private static final Set<Integer> CHAMP_DATA_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_BASE_X__,
                GraphicObjectProperties.__GO_BASE_Y__,
                GraphicObjectProperties.__GO_BASE_Z__,
                GraphicObjectProperties.__GO_DIRECTION_X__,
                GraphicObjectProperties.__GO_DIRECTION_Y__,
                GraphicObjectProperties.__GO_DIRECTION_Z__,
                GraphicObjectProperties.__GO_COLORED__
            ));

    /**
     * Set of properties that affect Rectangle data.
     */
    private static final Set<Integer> RECTANGLE_DATA_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_UPPER_LEFT_POINT__,
                GraphicObjectProperties.__GO_WIDTH__,
                GraphicObjectProperties.__GO_HEIGHT__
            ));

    /**
     * Set of properties that affect Segs data.
     */
    private static final Set<Integer> SEGS_DATA_PROPERTIES = new HashSet<Integer>(Arrays.asList(
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

    private final Map<Integer, TransformedElementsBuffer> vertexBufferMap = new HashMap<Integer, TransformedElementsBuffer>();
    private final Map<Integer, ElementsBuffer> normalBufferMap = new HashMap<Integer, ElementsBuffer>();
    private final Map<Integer, ElementsBuffer> colorBufferMap = new ConcurrentHashMap<Integer, ElementsBuffer>();
    private final Map<Integer, ElementsBuffer> texturesCoordinatesBufferMap = new HashMap<Integer, ElementsBuffer>();
    private final Map<Integer, IndicesBuffer> indexBufferMap = new HashMap<Integer, IndicesBuffer>();
    private final Map<Integer, IndicesBuffer> wireIndexBufferMap = new HashMap<Integer, IndicesBuffer>();
    private final Canvas canvas;


    /**
     * Default constructor.
     * @param canvas the canvas where managed data live.
     */
    public DataManager(Canvas canvas) {
        this.canvas = canvas;
    }

    /**
     * Return the vertex buffer of the given object.
     * @param id the given object Id.
     * @return the vertex buffer of the given object.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public ElementsBuffer getVertexBuffer(Integer id) throws ObjectRemovedException, OutOfMemoryException {
        GraphicObject currentObject = GraphicController.getController().getObjectFromId(id);
        Axes axes = (Axes) GraphicController.getController().getObjectFromId(currentObject.getParentAxes());
        double[][] factors = axes.getScaleTranslateFactors();

        if (vertexBufferMap.containsKey(id)) {
            TransformedElementsBuffer buf = vertexBufferMap.get(id);
            if (buf.isSameFactors(factors)) {
                return buf.getBuffer();
            }
        }

        ElementsBuffer vertexBuffer = canvas.getBuffersManager().createElementsBuffer();
        fillVertexBuffer(vertexBuffer, id, factors[0], factors[1]);
        vertexBufferMap.put(id, new TransformedElementsBuffer(vertexBuffer, factors));

        return vertexBuffer;
    }

    /**
     * Return the normal buffer of the given object.
     * @param id the given object Id.
     * @return the vertex buffer of the given object.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public ElementsBuffer getNormalBuffer(Integer id) throws ObjectRemovedException, OutOfMemoryException {
        if (normalBufferMap.containsKey(id)) {
            return normalBufferMap.get(id);
        } else {
            ElementsBuffer normalBuffer = canvas.getBuffersManager().createElementsBuffer();
            fillNormalBuffer(normalBuffer, id);
            normalBufferMap.put(id, normalBuffer);
            return normalBuffer;
        }
    }

    /**
     * Texture coordinates getter.
     * @param identifier the graphic object id.
     * @return the texture coordinates corresponding to the given graphic object.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public ElementsBuffer getTextureCoordinatesBuffer(Integer identifier) throws ObjectRemovedException, OutOfMemoryException {
        if (texturesCoordinatesBufferMap.containsKey(identifier)) {
            return texturesCoordinatesBufferMap.get(identifier);
        } else {
            ElementsBuffer texturesCoordinatesBuffer = canvas.getBuffersManager().createElementsBuffer();
            fillTextureCoordinatesBuffer(texturesCoordinatesBuffer, identifier);
            texturesCoordinatesBufferMap.put(identifier, texturesCoordinatesBuffer);
            return texturesCoordinatesBuffer;
        }
    }

    /**
     * Return the color buffer of the given object.
     * @param id the given object Id.
     * @return the color buffer of the given object.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public ElementsBuffer getColorBuffer(Integer id) throws ObjectRemovedException, OutOfMemoryException {
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
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public IndicesBuffer getIndexBuffer(Integer id) throws ObjectRemovedException, OutOfMemoryException {
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
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public IndicesBuffer getWireIndexBuffer(Integer id) throws ObjectRemovedException, OutOfMemoryException {
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
     * Update texture coordinate buffer for the given object.
     * @param id given object id.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public void updateTextureCoordinatesBuffer(Integer id) throws ObjectRemovedException, OutOfMemoryException {
        ElementsBuffer textureCoordinatesBuffer = texturesCoordinatesBufferMap.get(id);
        if (textureCoordinatesBuffer != null) {
            fillTextureCoordinatesBuffer(textureCoordinatesBuffer, id);
        }
    }

    /**
     * Update the data if needed.
     * @param id the modified object.
     * @param property the changed property.
     */
    public void update(Integer id, int property) throws OutOfMemoryException {
        Integer type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        try {
            if (vertexBufferMap.containsKey(id)) {
                if ((type.equals(GraphicObjectProperties.__GO_FAC3D__) && FAC3D_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_FEC__) && FEC_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_GRAYPLOT__) && GRAYPLOT_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_MATPLOT__) && MATPLOT_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_POLYLINE__) && POLYLINE_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_PLOT3D__) && PLOT3D_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_ARC__) && ARC_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_CHAMP__) && CHAMP_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_RECTANGLE__) && RECTANGLE_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_SEGS__) && SEGS_DATA_PROPERTIES.contains(property))) {
                    fillBuffers(id);
                }
            }
            if (property == GraphicObjectProperties.__GO_X_AXIS_LOG_FLAG__) {
                updateChildrenVertexIndex(id, 0x01);
            }

            if (property == GraphicObjectProperties.__GO_Y_AXIS_LOG_FLAG__) {
                updateChildrenVertexIndex(id, 0x02);
            }

            if (property == GraphicObjectProperties.__GO_Z_AXIS_LOG_FLAG__) {
                updateChildrenVertexIndex(id, 0x04);
            }
        } catch (ObjectRemovedException e) {
            // Object has been removed before drawing : do nothing.
        }
    }

    /**
     * Update the vertex buffer and index buffers of the given object and its descendants.
     * @param id the id of the object.
     * @param coordinateMask the coordinateMask to use.
     * @throws ObjectRemovedException if the object is no longer present.
     * @throws OutOfMemoryException if there was not enough memory.
     */
    private void updateChildrenVertexIndex(Integer id, int coordinateMask) throws ObjectRemovedException, OutOfMemoryException {
        GraphicObject currentObject = GraphicController.getController().getObjectFromId(id);
        Axes axes = (Axes) GraphicController.getController().getObjectFromId(currentObject.getParentAxes());
        double[][] factors = axes.getScaleTranslateFactors();

        TransformedElementsBuffer buf = vertexBufferMap.get(id);
        if (buf != null) {
            updateVertexBuffer(buf.getBuffer(), id, coordinateMask, factors[0], factors[1]);
        }

        ElementsBuffer normalBuffer = normalBufferMap.get(id);
        if (normalBuffer != null) {
            fillNormalBuffer(normalBuffer, id);
        }

        /*
         * To update the index and wire index buffers, on the contrary to updateVertexBuffer, we must perform a complete fill.
         * That is because IndicesBuffer's getData method returns a read-only buffer, which cannot be written to, as is
         * done by updateVertexBuffer, whereas the fill methods allocate new buffers (see the implementations of getData in
         * SciRenderer's ElementsBuffer and IndicesBuffer). To allow an allocation-free update would require modifying
         * IndicesBuffer's getData method.
         */
        IndicesBuffer indexBuffer = indexBufferMap.get(id);
        if (indexBuffer != null) {
            fillIndexBuffer(indexBuffer, id);
        }

        IndicesBuffer wireIndexBuffer = wireIndexBufferMap.get(id);
        if (wireIndexBuffer != null) {
            fillWireIndexBuffer(wireIndexBuffer, id);
        }

        for (Integer childId : (Integer []) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CHILDREN__)) {
            updateChildrenVertexIndex(childId, coordinateMask);
        }
    }

    /**
     * Clear the buffer corresponding to the given object.
     * @param id object id.
     */
    public void dispose(Integer id) {
        if (vertexBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(vertexBufferMap.get(id).getBuffer());
            vertexBufferMap.remove(id);
        }

        if (normalBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(normalBufferMap.get(id));
            normalBufferMap.remove(id);
        }

        if (colorBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(colorBufferMap.get(id));
            colorBufferMap.remove(id);
        }

        if (texturesCoordinatesBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(texturesCoordinatesBufferMap.get(id));
            texturesCoordinatesBufferMap.remove(id);
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
     * Clears all the texture coordinates buffers.
     */
    public void disposeAllTextureCoordinatesBuffers() {
        canvas.getBuffersManager().dispose(texturesCoordinatesBufferMap.values());
        texturesCoordinatesBufferMap.clear();
    }

    /**
     * Fill the vertex, color, index and wire index buffers
     * of a given object.
     * @param id the object id.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    private void fillBuffers(Integer id) throws ObjectRemovedException, OutOfMemoryException {
        TransformedElementsBuffer buf = vertexBufferMap.get(id);
        if (buf != null) {
            fillVertexBuffer(buf.getBuffer(), id, buf.getScale(), buf.getTranslate());
        }

        ElementsBuffer normalBuffer = normalBufferMap.get(id);
        if (normalBuffer != null) {
            fillNormalBuffer(normalBuffer, id);
        }

        ElementsBuffer colorBuffer = colorBufferMap.get(id);
        if (colorBuffer != null) {
            fillColorBuffer(colorBuffer, id);
        }

        ElementsBuffer textureCoordinatesBuffer = texturesCoordinatesBufferMap.get(id);
        if (textureCoordinatesBuffer != null) {
            fillTextureCoordinatesBuffer(textureCoordinatesBuffer, id);
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

    private void fillVertexBuffer(ElementsBuffer vertexBuffer, Integer id, double[] scale, double[] translate) throws ObjectRemovedException, OutOfMemoryException {
        fillVertexBuffer(vertexBuffer, id, 0x01 | 0x02 | 0x04 | 0x08, scale, translate);
    }

    private void fillVertexBuffer(ElementsBuffer vertexBuffer, Integer id, int coordinateMask, double[] scale, double[] translate) throws ObjectRemovedException, OutOfMemoryException {
        int logMask = MainDataLoader.getLogMask(id);
        int length = MainDataLoader.getDataSize(id);
        FloatBuffer data = BufferAllocation.newFloatBuffer(length * 4);
        MainDataLoader.fillVertices(id, data, 4, coordinateMask, scale, translate, logMask);
        vertexBuffer.setData(data, 4);
    }

    private void fillNormalBuffer(ElementsBuffer normalBuffer, Integer id) throws ObjectRemovedException, OutOfMemoryException {
        int length = MainDataLoader.getDataSize(id);
        FloatBuffer data = BufferAllocation.newFloatBuffer(length * 4);
        MainDataLoader.fillNormals(id, getVertexBuffer(id).getData(), data, 4);
        normalBuffer.setData(data, 4);
    }

    private void updateVertexBuffer(ElementsBuffer vertexBuffer, Integer id, int coordinateMask, double[] scale, double[] translate) throws ObjectRemovedException {
        int logMask = MainDataLoader.getLogMask(id);
        FloatBuffer data = vertexBuffer.getData();
        MainDataLoader.fillVertices(id, data, 4, coordinateMask, scale, translate, logMask);
        vertexBuffer.setData(data, 4);
    }

    private void fillTextureCoordinatesBuffer(ElementsBuffer colorBuffer, Integer id) throws ObjectRemovedException, OutOfMemoryException {
        int length = MainDataLoader.getDataSize(id);
        FloatBuffer data = BufferAllocation.newFloatBuffer(length * 4);
        MainDataLoader.fillTextureCoordinates(id, data, length);
        colorBuffer.setData(data, 4);
    }

    private void fillColorBuffer(ElementsBuffer colorBuffer, Integer id) throws ObjectRemovedException, OutOfMemoryException {
        int length = MainDataLoader.getDataSize(id);
        FloatBuffer data = BufferAllocation.newFloatBuffer(length * 4);
        MainDataLoader.fillColors(id, data, 4);
        colorBuffer.setData(data, 4);
    }

    private void fillIndexBuffer(IndicesBuffer indexBuffer, Integer id) throws ObjectRemovedException, OutOfMemoryException {
        int length = MainDataLoader.getIndicesSize(id);
        IntBuffer data = BufferAllocation.newIntBuffer(length);

        int actualLength = 0;
        if (length != 0) {
            /* Do not call JNI when the buffer is empty */
            /* Because under Mac OS X, GetDirectBufferAddress returns a NULL pointer in this case */
            /* This generates an exception in DataLoader_wrap.c */
            int logMask = MainDataLoader.getLogMask(id);
            actualLength = MainDataLoader.fillIndices(id, data, logMask);
        }

        /* Set the buffer size to the actual number of indices */
        data.limit(actualLength);

        indexBuffer.setData(data);
    }

    private void fillWireIndexBuffer(IndicesBuffer indexBuffer, Integer id) throws ObjectRemovedException, OutOfMemoryException {
        int length = MainDataLoader.getWireIndicesSize(id);
        IntBuffer data = BufferAllocation.newIntBuffer(length);

        int actualLength = 0;
        if (length != 0) {
            /* Do not call JNI when the buffer is empty */
            /* Because under Mac OS X, GetDirectBufferAddress returns a NULL pointer in this case */
            /* This generates an exception in DataLoader_wrap.c */
            int logMask = MainDataLoader.getLogMask(id);
            actualLength = MainDataLoader.fillWireIndices(id, data, logMask);
        }

        /* Set the buffer size to the actual number of indices */
        data.limit(actualLength);

        indexBuffer.setData(data);
    }

    private static class TransformedElementsBuffer {

        ElementsBuffer buffer;
        double[][] factors;

        TransformedElementsBuffer(ElementsBuffer buffer, double[][] factors) {
            this.buffer = buffer;
            this.factors = factors;
        }

        ElementsBuffer getBuffer() {
            return buffer;
        }

        double[] getScale() {
            return factors[0];
        }

        double[] getTranslate() {
            return factors[1];
        }

        boolean isSameFactors(final double[][] factors) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 3; j++) {
                    if (this.factors[i][j] != factors[i][j]) {
                        return false;
                    }
                }
            }

            return true;
        }
    }
}
