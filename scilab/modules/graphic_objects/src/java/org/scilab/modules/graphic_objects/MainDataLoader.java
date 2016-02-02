/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2012 - DIGITEO - Manuel JULIACHS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.graphic_objects;

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

/**
 * Class MainDataLoader
 * A utility class used to switch to the appropriate data loading class
 * depending on the type of the object whose data is loaded. A class is used
 * for purely Java-based objects and another one is used to load the data
 * of data model (i.e. C++-based) objects.
 * @author Manuel JULIACHS
 */
public class MainDataLoader {

    /**
     * The names of Java-based objects.
     */
    private static final Set<Integer> JAVA_OBJECTS = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_ARC__,
                GraphicObjectProperties.__GO_CHAMP__,
                GraphicObjectProperties.__GO_RECTANGLE__,
                GraphicObjectProperties.__GO_SEGS__
            ));

    /**
     * Returns the data width for the given object.
     * @param id of the given object.
     * @return the data width.
     */
    public static int getTextureWidth(Integer id) {
        return DataLoader.getTextureWidth(id);
    }

    /**
     * Returns the data height for the given object.
     * @param id of the given object.
     * @return the data height.
     */
    public static int getTextureHeight(Integer id) {
        return DataLoader.getTextureHeight(id);
    }

    /**
     * Returns the data for the given object.
     * @param id of the given object.
     * @return the data.
     */
    public static ByteBuffer getTextureData(Integer id) {
        return DataLoader.getTextureData(id);
    }

    /**
     * Returns the data for the given object.
     * @param id of the given object.
     * @return the data.
     */
    public static boolean isTextureRowOrder(Integer id) {
        return DataLoader.isTextureRowOrder(id) != 0;
    }

    /**
     * Returns the data for the given object.
     * @param id of the given object.
     * @return the data.
     */
    public static void disposeTextureData(Integer id, ByteBuffer buffer) {
        DataLoader.disposeTextureData(id, buffer);
    }

    /**
     * Returns the image type of the texture data.
     * 0 for RGB, 1 for RGBA, 2 for GRAY.
     * @param id of the given object.
     * @return the image type.
     */
    public static int getTextureImageType(Integer id) {
        return DataLoader.getTextureImageType(id);
    }

    /**
     * Returns the image type of the texture data.
     * 0 for RGB, 1 for RGBA, 2 for GRAY.
     * @param id of the given object.
     * @return the image type.
     */
    public static int getTextureDataType(Integer id) {
        return DataLoader.getTextureDataType(id);
    }

    /**
     * Returns the image type of the texture data.
     * 0 for RGB, 1 for RGBA, 2 for GRAY.
     * @param id of the given object.
     * @return the image type.
     */
    public static int getTextureGLType(Integer id) {
        return DataLoader.getTextureGLType(id);
    }

    public static void fillTextureCoordinates(Integer id, FloatBuffer buffer, int bufferLength) {
        DataLoader.fillTextureCoordinates(id, buffer, bufferLength);
    }

    public static void fillTextureData(Integer id, ByteBuffer buffer, int bufferLength) {
        DataLoader.fillTextureData(id, buffer, bufferLength);
    }

    public static void fillTextureData(Integer identifier, ByteBuffer buffer, int bufferLength, int x, int y, int width, int height) {
        DataLoader.fillSubTextureData(identifier, buffer, bufferLength, x, y, width, height);
    }

    /**
     * Returns the number of data elements for the given object.
     * @param id id of the given object.
     * @return the number of data elements.
     * @throws ObjectRemovedException if the object no longer exist.
     */
    public static int getDataSize(Integer id) throws ObjectRemovedException {
        Integer type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        if (type == null) {
            throw (new ObjectRemovedException(id));
        }

        if (JAVA_OBJECTS.contains(type)) {
            return JavaObjectLoader.getDataSize(id);
        } else {
            return DataLoader.getDataSize(id);
        }
    }

    /**
     * Fills the given buffer with vertex data from the given object.
     * @param id id of the given object.
     * @param buffer buffer to fill.
     * @param elementsSize number of coordinates taken by one element in the buffer.
     * @param coordinateMask bit mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param scale conversion scale factor to apply to data.
     * @param translation conversion translation value to apply to data.
     * @param logMask bit mask specifying whether logarithmic coordinates are used.
     * @throws ObjectRemovedException if the object no longer exist.
     */
    public static void fillVertices(Integer id, FloatBuffer buffer, int elementsSize,
                                    int coordinateMask, double[] scale, double[] translation, int logMask) throws ObjectRemovedException {
        Integer type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        if (type == null) {
            throw (new ObjectRemovedException(id));
        }

        if (JAVA_OBJECTS.contains(type)) {
            JavaObjectLoader.fillVertices(id, buffer, elementsSize, coordinateMask, scale, translation, logMask);
        } else {
            DataLoader.fillVertices(id, buffer, buffer.capacity(), elementsSize, coordinateMask, scale, translation, logMask);
        }
    }

    /**
     * Fills the given buffer with normal data from the given object.
     * @param id id of the given object.
     * @param position buffer with vertex position data.
     * @param buffer buffer to fill.
     * @param elementsSize number of coordinates taken by one element in the buffer.
     * @throws ObjectRemovedException if the object no longer exist.
     */
    public static void fillNormals(Integer id, FloatBuffer position, FloatBuffer buffer, int elementsSize) throws ObjectRemovedException {
        Integer type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        if (type == null) {
            throw (new ObjectRemovedException(id));
        }
        DataLoader.fillNormals(id, position, buffer, buffer.capacity(), elementsSize);
    }

    /**
     * Fills the given buffer with color data from the given object.
     * @param id id of the given object.
     * @param buffer buffer to fill.
     * @param elementsSize number of components taken by one element in the buffer (3 or 4).
     * @throws ObjectRemovedException if the object no longer exist.
     */
    public static void fillColors(Integer id, FloatBuffer buffer, int elementsSize) throws ObjectRemovedException {
        Integer type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        if (type == null) {
            throw (new ObjectRemovedException(id));
        }

        if (JAVA_OBJECTS.contains(type)) {
            JavaObjectLoader.fillColors(id, buffer, elementsSize);
        } else {
            DataLoader.fillColors(id, buffer, buffer.capacity(), elementsSize);
        }
    }

    /**
     * Returns the number of indices for the given object.
     * @param id id of the given object.
     * @return the object's number of indices.
     * @throws ObjectRemovedException if the object no longer exist.
     */
    public static int getIndicesSize(Integer id) throws ObjectRemovedException {
        Integer type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        if (type == null) {
            throw (new ObjectRemovedException(id));
        }

        if (JAVA_OBJECTS.contains(type)) {
            return JavaObjectLoader.getIndicesSize(id);
        } else {
            return DataLoader.getIndicesSize(id);
        }
    }

    /**
     * Fills the given buffer with indices data of the given object.
     * @param id id of the given object.
     * @param buffer buffer to fill.
     * @param logMask bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     * @throws ObjectRemovedException if the object no longer exist.
     */
    public static int fillIndices(Integer id, IntBuffer buffer, int logMask) throws ObjectRemovedException {
        Integer type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        if (type == null) {
            throw (new ObjectRemovedException(id));
        }

        if (JAVA_OBJECTS.contains(type)) {
            return JavaObjectLoader.fillIndices(id, buffer, logMask);
        } else {
            return DataLoader.fillIndices(id, buffer, buffer.capacity(), logMask);
        }
    }

    /**
     * Returns the number of wire-frame indices of the given object.
     * @param id id of the given object.
     * @return the object's number of indices.
     * @throws ObjectRemovedException if the object no longer exist.
     */
    public static int getWireIndicesSize(Integer id) throws ObjectRemovedException {
        Integer type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);


        if (type == null) {
            throw (new ObjectRemovedException(id));
        }

        if (JAVA_OBJECTS.contains(type)) {
            return JavaObjectLoader.getWireIndicesSize(id);
        } else {
            return DataLoader.getWireIndicesSize(id);
        }
    }

    /**
     * Fills the given buffer with wire-frame index data of the given object.
     * @param id id of the given object.
     * @param buffer buffer to fill.
     * @param logMask bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     * @throws ObjectRemovedException if the object no longer exist.
     */
    public static int fillWireIndices(Integer id, IntBuffer buffer, int logMask) throws ObjectRemovedException {
        Integer type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        if (type == null) {
            throw (new ObjectRemovedException(id));
        }

        if (JAVA_OBJECTS.contains(type)) {
            return JavaObjectLoader.fillWireIndices(id, buffer, logMask);
        } else {
            return DataLoader.fillWireIndices(id, buffer, buffer.capacity(), logMask);
        }
    }

    /**
     * Returns the number of mark indices of the given object.
     * To implement.
     * @param id id of the given object.
     * @return the number of mark indices.
     */
    public static int getMarkIndicesSize(String id) {
        return 0;
    }

    /**
     * Fills the given buffer with mark index data of the given object.
     * To implement.
     * @param id id of the given object.
     * @param buffer buffer to fill.
     * @return the number of indices actually written.
     */
    public static int fillMarkIndices(String id, IntBuffer buffer) {
        return 0;
    }

    public static int getLogMask(Integer id) {
        try {
            GraphicObject object = GraphicController.getController().getObjectFromId(id);
            Integer parentAxesId = object.getParentAxes();
            Axes axes = (Axes) GraphicController.getController().getObjectFromId(parentAxesId);
            int mask = 0;
            if (axes.getXAxisLogFlag()) {
                mask += 1;
            }
            if (axes.getYAxisLogFlag()) {
                mask += 2;
            }
            if (axes.getZAxisLogFlag()) {
                mask += 4;
            }
            return mask;
        } catch (Exception ignored) {
            return 0;
        }
    }
}
