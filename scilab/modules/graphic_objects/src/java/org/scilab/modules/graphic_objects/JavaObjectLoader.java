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

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.arc.ArcDecomposer;
import org.scilab.modules.graphic_objects.vectfield.ChampDecomposer;
import org.scilab.modules.graphic_objects.rectangle.RectangleDecomposer;
import org.scilab.modules.graphic_objects.vectfield.SegsDecomposer;

/**
 * Class JavaObjectLoader
 * A set of methods used to load the data of purely Java-based objects.
 * @author Manuel JULIACHS
 */
public class JavaObjectLoader {

    /**
     * Returns the number of data elements for the given object.
     * @param the id of the given object.
     * @return the number of data elements.
     */
    public static int getDataSize(Integer id) {
        int type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);
        switch (type) {
            case GraphicObjectProperties.__GO_ARC__ :
                return ArcDecomposer.getDataSize();
            case GraphicObjectProperties.__GO_CHAMP__ :
                return ChampDecomposer.getDataSize(id);
            case GraphicObjectProperties.__GO_RECTANGLE__ :
                return RectangleDecomposer.getDataSize();
            case GraphicObjectProperties.__GO_SEGS__ :
                return SegsDecomposer.getDataSize(id);
            default :
                return 0;
        }
    }

    /**
     * Fills the given buffer with vertex data from the given object.
     * @param the id of the given object.
     * @param the buffer to fill.
     * @param the number of coordinates taken by one element in the buffer.
     * @param the bit mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param the conversion scale factor to apply to data.
     * @param the conversion translation value to apply to data.
     * @param the bit mask specifying whether logarithmic coordinates are used.
     */
    public static void fillVertices(Integer id, FloatBuffer buffer, int elementsSize,
                                    int coordinateMask, double[] scale, double[] translation, int logMask) {
        int type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        switch (type) {
            case GraphicObjectProperties.__GO_ARC__ :
                ArcDecomposer.fillVertices(buffer, id, elementsSize, coordinateMask, scale, translation, logMask);
                break;
            case GraphicObjectProperties.__GO_CHAMP__ :
                ChampDecomposer.fillVertices(buffer, id, elementsSize, coordinateMask, scale, translation, logMask);
                break;
            case GraphicObjectProperties.__GO_RECTANGLE__ :
                RectangleDecomposer.fillVertices(buffer, id, elementsSize, coordinateMask, scale, translation, logMask);
                break;
            case GraphicObjectProperties.__GO_SEGS__ :
                SegsDecomposer.fillVertices(buffer, id, elementsSize, coordinateMask, scale, translation, logMask);
                break;
        }
    }

    /**
     * Fills the given buffer with color data from the given object.
     * Does nothing at the moment (no color-outputting Java objects implemented so far).
     * @param the id of the given object.
     * @param the buffer to fill.
     * @param the number of components taken by one element in the buffer (3 or 4).
     */
    public static void fillColors(Integer id, FloatBuffer buffer, int elementsSize) {
        int type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        if (type == GraphicObjectProperties.__GO_CHAMP__) {
            ChampDecomposer.fillColors(buffer, id, elementsSize);
        } else if (type == GraphicObjectProperties.__GO_SEGS__) {
            SegsDecomposer.fillColors(buffer, id, elementsSize);
        }
    }

    /**
     * Returns the number of indices for the given object.
     * @param the id of the given object.
     * @return the object's number of indices.
     */
    public static int getIndicesSize(Integer id) {
        int type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        switch (type) {
            case GraphicObjectProperties.__GO_ARC__ :
                return ArcDecomposer.getIndicesSize();
            case GraphicObjectProperties.__GO_CHAMP__ :
                return ChampDecomposer.getIndicesSize(id);
            case GraphicObjectProperties.__GO_RECTANGLE__ :
                return RectangleDecomposer.getIndicesSize();
            case GraphicObjectProperties.__GO_SEGS__ :
                return SegsDecomposer.getIndicesSize(id);
            default :
                return 0;
        }
    }

    /**
     * Fills the given buffer with indices data of the given object.
     * @param the id of the given object.
     * @param the buffer to fill.
     * @param the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    public static int fillIndices(Integer id, IntBuffer buffer, int logMask) {
        int type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        switch (type) {
            case GraphicObjectProperties.__GO_ARC__ :
                return ArcDecomposer.fillIndices(buffer, id, logMask);
            case GraphicObjectProperties.__GO_CHAMP__ :
                return ChampDecomposer.fillIndices(buffer, id, logMask);
            case GraphicObjectProperties.__GO_RECTANGLE__ :
                return RectangleDecomposer.fillIndices(buffer, id, logMask);
            case GraphicObjectProperties.__GO_SEGS__ :
                return SegsDecomposer.fillIndices(buffer, id, logMask);
            default :
                return 0;
        }
    }

    /**
     * Returns the number of wireframe indices of the given object.
     * @param the id of the given object.
     * @return the object's number of indices.
     */
    public static int getWireIndicesSize(Integer id) {
        int type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        switch (type) {
            case GraphicObjectProperties.__GO_ARC__ :
                return ArcDecomposer.getWireIndicesSize();
            case GraphicObjectProperties.__GO_CHAMP__ :
                return ChampDecomposer.getWireIndicesSize(id);
            case GraphicObjectProperties.__GO_RECTANGLE__ :
                return RectangleDecomposer.getWireIndicesSize();
            case GraphicObjectProperties.__GO_SEGS__ :
                return SegsDecomposer.getWireIndicesSize(id);
            default :
                return 0;
        }
    }

    /**
     * Fills the given buffer with wireframe index data of the given object.
     * @param the id of the given object.
     * @param the buffer to fill.
     * @param the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    public static int fillWireIndices(Integer id, IntBuffer buffer, int logMask) {
        int type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        switch (type) {
            case GraphicObjectProperties.__GO_ARC__ :
                return ArcDecomposer.fillWireIndices(buffer, id, logMask);
            case GraphicObjectProperties.__GO_CHAMP__ :
                return ChampDecomposer.fillWireIndices(buffer, id, logMask);
            case GraphicObjectProperties.__GO_RECTANGLE__ :
                return RectangleDecomposer.fillWireIndices(buffer, id, logMask);
            case GraphicObjectProperties.__GO_SEGS__ :
                return SegsDecomposer.fillWireIndices(buffer, id, logMask);
            default :
                return 0;
        }
    }
}
