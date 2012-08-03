/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2012 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
        public static int getDataSize(String id) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (type.equals(GraphicObjectProperties.__GO_ARC__)) {
                        return ArcDecomposer.getDataSize();
                } else if (type.equals(GraphicObjectProperties.__GO_CHAMP__)) {
                        return ChampDecomposer.getDataSize(id);
                } else if (type.equals(GraphicObjectProperties.__GO_RECTANGLE__)) {
                        return RectangleDecomposer.getDataSize();
                } else if (type.equals(GraphicObjectProperties.__GO_SEGS__)) {
                        return SegsDecomposer.getDataSize(id);
                }

                return 0;
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
        public static void fillVertices(String id, FloatBuffer buffer, int elementsSize,
                int coordinateMask, double[] scale, double[] translation, int logMask) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (type.equals(GraphicObjectProperties.__GO_ARC__)) {
                        ArcDecomposer.fillVertices(buffer, id, elementsSize, coordinateMask, scale, translation, logMask);
                } else if (type.equals(GraphicObjectProperties.__GO_CHAMP__)) {
                        ChampDecomposer.fillVertices(buffer, id, elementsSize, coordinateMask, scale, translation, logMask);
                } else if (type.equals(GraphicObjectProperties.__GO_RECTANGLE__)) {
                        RectangleDecomposer.fillVertices(buffer, id, elementsSize, coordinateMask, scale, translation, logMask);
                } else if (type.equals(GraphicObjectProperties.__GO_SEGS__)) {
                        SegsDecomposer.fillVertices(buffer, id, elementsSize, coordinateMask, scale, translation, logMask);
                }
        }

        /**
         * Fills the given buffer with color data from the given object.
         * Does nothing at the moment (no color-outputting Java objects implemented so far).
         * @param the id of the given object.
         * @param the buffer to fill.
         * @param the number of components taken by one element in the buffer (3 or 4).
         */
        public static void fillColors(String id, FloatBuffer buffer, int elementsSize) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (type.equals(GraphicObjectProperties.__GO_CHAMP__)) {
                        ChampDecomposer.fillColors(buffer, id, elementsSize);
                } else if (type.equals(GraphicObjectProperties.__GO_SEGS__)) {
                        SegsDecomposer.fillColors(buffer, id, elementsSize);
                }
        }

        /**
         * Returns the number of indices for the given object.
         * @param the id of the given object.
         * @return the object's number of indices.
         */
        public static int getIndicesSize(String id) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (type.equals(GraphicObjectProperties.__GO_ARC__)) {
                        return ArcDecomposer.getIndicesSize();
                } else if (type.equals(GraphicObjectProperties.__GO_CHAMP__)) {
                        return ChampDecomposer.getIndicesSize(id);
                } else if (type.equals(GraphicObjectProperties.__GO_RECTANGLE__)) {
                        return RectangleDecomposer.getIndicesSize();
                } else if (type.equals(GraphicObjectProperties.__GO_SEGS__)) {
                        return SegsDecomposer.getIndicesSize(id);
                }

                return 0;
        }

        /**
         * Fills the given buffer with indices data of the given object.
         * @param the id of the given object.
         * @param the buffer to fill.
         * @param the bit mask specifying whether logarithmic coordinates are used.
         * @return the number of indices actually written.
         */
        public static int fillIndices(String id, IntBuffer buffer, int logMask) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (type.equals(GraphicObjectProperties.__GO_ARC__)) {
                        return ArcDecomposer.fillIndices(buffer, id, logMask);
                } else if (type.equals(GraphicObjectProperties.__GO_CHAMP__)) {
                        return ChampDecomposer.fillIndices(buffer, id, logMask);
                } else if (type.equals(GraphicObjectProperties.__GO_RECTANGLE__)) {
                        return RectangleDecomposer.fillIndices(buffer, id, logMask);
                } else if (type.equals(GraphicObjectProperties.__GO_SEGS__)) {
                        return SegsDecomposer.fillIndices(buffer, id, logMask);
                }

                return 0;
        }

        /**
         * Returns the number of wireframe indices of the given object.
         * @param the id of the given object.
         * @return the object's number of indices.
         */
        public static int getWireIndicesSize(String id) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (type.equals(GraphicObjectProperties.__GO_ARC__)) {
                        return ArcDecomposer.getWireIndicesSize();
                } else if (type.equals(GraphicObjectProperties.__GO_CHAMP__)) {
                        return ChampDecomposer.getWireIndicesSize(id);
                } else if (type.equals(GraphicObjectProperties.__GO_RECTANGLE__)) {
                        return RectangleDecomposer.getWireIndicesSize();
                } else if (type.equals(GraphicObjectProperties.__GO_SEGS__)) {
                        return SegsDecomposer.getWireIndicesSize(id);
                }

                return 0;
        }

        /**
         * Fills the given buffer with wireframe index data of the given object.
         * @param the id of the given object.
         * @param the buffer to fill.
         * @param the bit mask specifying whether logarithmic coordinates are used.
         * @return the number of indices actually written.
         */
        public static int fillWireIndices(String id, IntBuffer buffer, int logMask) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (type.equals(GraphicObjectProperties.__GO_ARC__)) {
                        return ArcDecomposer.fillWireIndices(buffer, id, logMask);
                } else if (type.equals(GraphicObjectProperties.__GO_CHAMP__)) {
                        return ChampDecomposer.fillWireIndices(buffer, id, logMask);
                } else if (type.equals(GraphicObjectProperties.__GO_RECTANGLE__)) {
                        return RectangleDecomposer.fillWireIndices(buffer, id, logMask);
                } else if (type.equals(GraphicObjectProperties.__GO_SEGS__)) {
                        return SegsDecomposer.fillWireIndices(buffer, id, logMask);
                }

                return 0;
        }
}
