/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.vectfield;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.utils.Utils;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Class ChampDecomposer
 * A set of static methods decomposing a Champ object (vector field) into a set of line segments.
 * To do: -output arrow tips as triangles.
 * @author Manuel JULIACHS
 */
public class ChampDecomposer extends VectFieldDecomposer {

    /**
     * Fills the input buffer with vertex data from the given object.
     * To do: optimize, as the test on colored's value is performed for each segment.
     * @param the buffer to fill.
     * @param the given object id.
     * @param the number of coordinates taken by one element in the buffer.
     * @param the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param the conversion scale factor to apply to data.
     * @param the conversion translation value to apply to data.
     * @param the bit mask specifying whether logarithmic coordinates are used.
     */
    public static void fillVertices(FloatBuffer buffer, String id, int elementsSize,
        int coordinateMask, double[] scale, double[] translation, int logMask) {
        int bufferOffset = 0;

        /* The x coordinates of a segment's base and tip */
        double[] xcoords;
        /* The y coordinates of a segment's base and tip */
        double[] ycoords;
        /* The z coordinates of a segment's base and tip */
        double[] zcoords;

        xcoords = new double[2];
        ycoords = new double[2];
        zcoords = new double[2];

        /* The direction vector */
        double[] direction = new double[3];

        int numberArrows = (Integer) GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

        Double[] xBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_X__);
        Double[] yBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_Y__);
        Double[] zBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_Z__);

        Double[] xDirection = (Double[]) GraphicController.getController().getProperty(id, __GO_DIRECTION_X__);
        Double[] yDirection = (Double[]) GraphicController.getController().getProperty(id, __GO_DIRECTION_Y__);
        Double[] zDirection = (Double[]) GraphicController.getController().getProperty(id, __GO_DIRECTION_Z__);

        double maxLength = (Double) GraphicController.getController().getProperty(id, __GO_MAX_LENGTH__);
        double maxUsableLength = (Double) GraphicController.getController().getProperty(id, __GO_MAX_USABLE_LENGTH__);

        boolean colored = (Boolean) GraphicController.getController().getProperty(id, __GO_COLORED__);

        if (maxLength <= Double.MIN_VALUE) {
            maxLength = 1.0;
        }

        for (int j = 0; j < yBase.length; j++) {
            for (int i = 0; i < xBase.length; i++) {
                xcoords[0] = xBase[i];
                ycoords[0] = yBase[j];
                zcoords[0] = zBase[j*xBase.length+i];

                direction[0] = xDirection[j*xBase.length+i];
                direction[1] = yDirection[j*xBase.length+i];
                direction[2] = zDirection[j*xBase.length+i];

                /* The current vector's norm is used to normalize for colored Champ objects instead of the maximum length. */
                if (colored) {
                    double norm = Math.sqrt(direction[0]*direction[0] + direction[1]*direction[1] + direction[2]*direction[2]);

                    if (norm <= Double.MIN_VALUE) {
                        norm = 1.0;
                    }

                    xcoords[1] = xcoords[0] + direction[0] * maxUsableLength / norm;
                    ycoords[1] = ycoords[0] + direction[1] * maxUsableLength / norm;
                    zcoords[1] = zcoords[0] + direction[2] * maxUsableLength / norm;
                } else {
                    xcoords[1] = xcoords[0] + direction[0] * maxUsableLength / maxLength;
                    ycoords[1] = ycoords[0] + direction[1] * maxUsableLength / maxLength;
                    zcoords[1] = zcoords[0] + direction[2] * maxUsableLength / maxLength;
                }

                /* Vertices are output in the following order: the segment's base and then its tip */
                if ((coordinateMask & 0x1) != 0) {
                    if ((logMask & 0x1) != 0) {
                        xcoords[0] = Math.log10(xcoords[0]);
                        xcoords[1] = Math.log10(xcoords[1]);
                    }

                    xcoords[0] = xcoords[0] * scale[0] + translation[0];
                    xcoords[1] = xcoords[1] * scale[0] + translation[0];

                    buffer.put(bufferOffset, (float) xcoords[0]);
                    buffer.put(bufferOffset+elementsSize, (float) xcoords[1]);
                }

                if ((coordinateMask & 0x2) != 0) {
                    if ((logMask & 0x2) != 0) {
                        ycoords[0] = Math.log10(ycoords[0]);
                        ycoords[1] = Math.log10(ycoords[1]);
                    }

                    ycoords[0] = ycoords[0] * scale[1] + translation[1];
                    ycoords[1] = ycoords[1] * scale[1] + translation[1];

                    buffer.put(bufferOffset+1, (float) ycoords[0]);
                    buffer.put(bufferOffset+elementsSize+1, (float) ycoords[1]);
                }

                if ((coordinateMask & 0x4) != 0) {
                    if ((logMask & 0x4) != 0) {
                        zcoords[0] = Math.log10(zcoords[0] + DEFAULT_LOG_COORD_Z);
                        zcoords[1] = Math.log10(zcoords[1] + DEFAULT_LOG_COORD_Z);
                    }

                    zcoords[0] = zcoords[0] * scale[2] + translation[2];
                    zcoords[1] = zcoords[1] * scale[2] + translation[2];

                    buffer.put(bufferOffset+2, (float) zcoords[0]);
                    buffer.put(bufferOffset+elementsSize+2, (float) zcoords[1]);
                }

                if (elementsSize == 4 && (coordinateMask & 0x8) != 0) {
                    buffer.put(bufferOffset+3, 1.0f);
                    buffer.put(bufferOffset+elementsSize +3, 1.0f);
                }

                bufferOffset += 2*elementsSize;
            }
        }
    }

    /**
     * Fills the given buffer with color data from the given object.
     * @param the buffer to fill.
     * @param the id of the given object.
     * @param the number of components taken by one element in the buffer (3 or 4).
     */
    public static void fillColors(FloatBuffer buffer, String id, int elementsSize) {
        int bufferOffset = 0;

        String parentFigureId = (String) GraphicController.getController().getProperty(id, __GO_PARENT_FIGURE__);

        /* To be sure that the object has a parent Figure, though it should not occur. */
        if (parentFigureId.equals("")) {
            return;
        }

        Figure parentFigure = (Figure) GraphicController.getController().getObjectFromId(parentFigureId);
        ColorMap colorMap = parentFigure.getColorMap();
        int colorMapSize = colorMap.getSize();

        int numberArrows = (Integer) GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

        Double[] xBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_X__);
        Double[] yBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_Y__);
        Double[] zBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_Z__);

        Double[] xDirection = (Double[]) GraphicController.getController().getProperty(id, __GO_DIRECTION_X__);
        Double[] yDirection = (Double[]) GraphicController.getController().getProperty(id, __GO_DIRECTION_Y__);
        Double[] zDirection = (Double[]) GraphicController.getController().getProperty(id, __GO_DIRECTION_Z__);

        double maxLength = (Double) GraphicController.getController().getProperty(id, __GO_MAX_LENGTH__);

        boolean colored = (Boolean) GraphicController.getController().getProperty(id, __GO_COLORED__);

        if (!colored) {
            return;
        }

        if (maxLength <= Double.MIN_VALUE) {
            maxLength = 1.0;
        }

        double[] direction = new double[3];

        /* Colors are output in the same order as vertices: first the base's color and then the tip's */
        for (int j = 0; j < yBase.length; j++) {
            for (int i = 0; i < xBase.length; i++) {
                direction[0] = xDirection[j*xBase.length+i];
                direction[1] = yDirection[j*xBase.length+i];
                direction[2] = zDirection[j*xBase.length+i];

                double norm = Math.sqrt(direction[0]*direction[0] + direction[1]*direction[1] + direction[2]*direction[2]);

                int colorIndex  = (int) Math.floor((((double) colorMapSize -1.0) * norm / maxLength) + 0.5);

                float[] color = colorMap.getScilabColor(colorIndex);

                buffer.put(bufferOffset, color[0]);
                buffer.put(bufferOffset+1, color[1]);
                buffer.put(bufferOffset+2, color[2]);

                buffer.put(bufferOffset+elementsSize, color[0]);
                buffer.put(bufferOffset+elementsSize+1, color[1]);
                buffer.put(bufferOffset+elementsSize+2, color[2]);

                if (elementsSize == 4) {
                    buffer.put(bufferOffset+3, 1.0f);
                    buffer.put(bufferOffset+elementsSize+3, 1.0f);
                }

                bufferOffset += 2*elementsSize;
            }
        }
    }

    /**
     * Fills the input buffer with segment index data from the given object.
     * To do: optimize, as the test on colored's value is performed for each segment.
     * @param the buffer to fill.
     * @param the given object id.
     * @param the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    public static int fillWireIndices(IntBuffer buffer, String id, int logMask) {
        boolean valid;
        int bufferOffset = 0;

        int numberArrows = (Integer) GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

        if (numberArrows == 0) {
            return 0;
        }

        Double[] xBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_X__);
        Double[] yBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_Y__);
        Double[] zBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_Z__);

        Double[] xDirection = (Double[]) GraphicController.getController().getProperty(id, __GO_DIRECTION_X__);
        Double[] yDirection = (Double[]) GraphicController.getController().getProperty(id, __GO_DIRECTION_Y__);
        Double[] zDirection = (Double[]) GraphicController.getController().getProperty(id, __GO_DIRECTION_Z__);

        double maxLength = (Double) GraphicController.getController().getProperty(id, __GO_MAX_LENGTH__);
        double maxUsableLength = (Double) GraphicController.getController().getProperty(id, __GO_MAX_USABLE_LENGTH__);

        boolean colored = (Boolean) GraphicController.getController().getProperty(id, __GO_COLORED__);

        /* The base and tip of a segment */
        double[] base = new double[3];
        double[] direction = new double[3];

        for (int j = 0; j < yBase.length; j++) {
            for (int i = 0; i < xBase.length; i++) {
                int baseIndex = 2*(xBase.length*j+i);
                int tipIndex = baseIndex +1;

                base[0] = xBase[i];
                base[1] = yBase[j];
                base[2] = zBase[i*xBase.length+j];

                direction[0] = xDirection[j*xBase.length+i];
                direction[1] = yDirection[j*xBase.length+i];
                direction[2] = zDirection[j*xBase.length+i];

                /* The current vector's norm is used to normalize for colored Champ objects instead of the maximum length. */
                if (colored) {
                    double norm = Math.sqrt(direction[0]*direction[0] + direction[1]*direction[1] + direction[2]*direction[2]);

                    if (norm <= Double.MIN_VALUE) {
                        norm = 1.0;
                    }

                    direction[0] = base[0] + direction[0] * maxUsableLength / norm;
                    direction[1] = base[1] + direction[1] * maxUsableLength / norm;
                    direction[2] = base[2] + direction[2] * maxUsableLength / norm;
                } else {
                    direction[0] = base[0] + direction[0] * maxUsableLength / maxLength;
                    direction[1] = base[1] + direction[1] * maxUsableLength / maxLength;
                    direction[2] = base[2] + direction[2] * maxUsableLength / maxLength;
                }

                if (Utils.isValid(base[0], base[1], base[2])
                    && Utils.isValid(direction[0], direction[1], direction[2])) {
                    valid = true;
                } else {
                    valid = false;
                }

                if (logMask != 0) {
                    valid = valid && Utils.isLogValid(base[0], base[1], base[2]+DEFAULT_LOG_COORD_Z, logMask);
                    valid = valid && Utils.isLogValid(direction[0], direction[1], direction[2]+DEFAULT_LOG_COORD_Z, logMask);
                }

                if (valid) {
                    buffer.put(bufferOffset, baseIndex);
                    buffer.put(bufferOffset+1, tipIndex);
                    bufferOffset += 2;
                }

            }
        }

        if (bufferOffset > 0) {
            return bufferOffset;
        } else {
            return 0;
        }
    }
}
