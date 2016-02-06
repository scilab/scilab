/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Manuel JULIACHS
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
 * It takes into account the additional buffer space needed for arrow tip vertices and colors,
 * fills tip colors, and outputs their triangle indices. Arrow tip coordinates are, however,
 * computed at drawing time as they are projection-dependent.
 * @author Manuel JULIACHS
 */
public class ChampDecomposer extends VectFieldDecomposer {

    /**
     * Returns the number of data elements for the given object.
     * @param id the id of the given object.
     * @return the number of data elements.
     */
    public static int getDataSize(Integer id) {
        int numberArrows = (Integer) GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

        /* 2 vertices per segment plus 3 vertices for the arrow tip */
        return 5 * numberArrows;
    }

    /**
     * Fills the input buffer with vertex data from the given object.
     * To do: optimize, as the test on colored's value is performed for each segment.
     * @param buffer the buffer to fill.
     * @param id the id of the given object.
     * @param elementsSize the number of coordinates taken by one element in the buffer.
     * @param coordinateMask the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param scale the conversion scale factor to apply to data.
     * @param translation the conversion translation value to apply to data.
     * @param logMask the bit mask specifying whether logarithmic coordinates are used.
     */
    public static void fillVertices(FloatBuffer buffer, Integer id, int elementsSize,
                                    int coordinateMask, double[] scale, double[] translation, int logMask) {
        int bufferOffset = 0;

        /* The coordinates of a segment's base */
        double[] base;

        /* The coordinates of a segment's tip */
        double[] tip;

        base = new double[3];
        tip = new double[3];

        /* The direction vector */
        double[] direction = new double[3];

        GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

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
                base[0] = xBase[i];
                base[1] = yBase[j];
                base[2] = zBase[j * xBase.length + i];

                direction[0] = xDirection[j * xBase.length + i];
                direction[1] = yDirection[j * xBase.length + i];
                direction[2] = zDirection[j * xBase.length + i];

                /* Compute the segment tip */
                tip = computeSegmentTip(base, direction, colored, maxUsableLength, maxLength);

                /* Vertices are output in the following order: the segment's base and then its tip */
                if ((coordinateMask & 0x1) != 0) {
                    if ((logMask & 0x1) != 0) {
                        base[0] = Math.log10(base[0]);
                        tip[0] = Math.log10(tip[0]);
                    }

                    base[0] = base[0] * scale[0] + translation[0];
                    tip[0] = tip[0] * scale[0] + translation[0];

                    buffer.put(bufferOffset, (float) base[0]);
                    buffer.put(bufferOffset + elementsSize, (float) tip[0]);
                }

                if ((coordinateMask & 0x2) != 0) {
                    if ((logMask & 0x2) != 0) {
                        base[1] = Math.log10(base[1]);
                        tip[1] = Math.log10(tip[1]);
                    }

                    base[1] = base[1] * scale[1] + translation[1];
                    tip[1] = tip[1] * scale[1] + translation[1];

                    buffer.put(bufferOffset + 1, (float) base[1]);
                    buffer.put(bufferOffset + elementsSize + 1, (float) tip[1]);
                }

                if ((coordinateMask & 0x4) != 0) {
                    if ((logMask & 0x4) != 0) {
                        base[2] = Math.log10(base[2] + DEFAULT_LOG_COORD_Z);
                        tip[2] = Math.log10(tip[2] + DEFAULT_LOG_COORD_Z);
                    }

                    base[2] = base[2] * scale[2] + translation[2];
                    tip[2] = tip[2] * scale[2] + translation[2];

                    buffer.put(bufferOffset + 2, (float) base[2]);
                    buffer.put(bufferOffset + elementsSize + 2, (float) tip[2]);
                }

                if (elementsSize == 4 && (coordinateMask & 0x8) != 0) {
                    buffer.put(bufferOffset + 3, 1.0f);
                    buffer.put(bufferOffset + elementsSize + 3, 1.0f);
                }

                bufferOffset += 2 * elementsSize;
            }
        }
    }

    /**
     * Fills the given buffer with color data from the given object.
     * @param buffer the buffer to fill.
     * @param id the id of the given object.
     * @param elementsSize the number of components taken by one element in the buffer (3 or 4).
     */
    public static void fillColors(FloatBuffer buffer, Integer id, int elementsSize) {
        int bufferOffset = 0;

        Integer parentFigureId = (Integer) GraphicController.getController().getProperty(id, __GO_PARENT_FIGURE__);

        /* To be sure that the object has a parent Figure, though it should not occur. */
        if (parentFigureId == 0) {
            return;
        }

        Figure parentFigure = (Figure) GraphicController.getController().getObjectFromId(parentFigureId);
        ColorMap colorMap = parentFigure.getColorMap();
        int colorMapSize = colorMap.getSize();

        GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

        Double[] xBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_X__);
        Double[] yBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_Y__);
        GraphicController.getController().getProperty(id, __GO_BASE_Z__);

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

        GraphicController.getController().getProperty(id, __GO_ARROW_SIZE__);

        double[] direction = new double[3];

        /* Colors are output in the same order as vertices: first the base's color and then the tip's */
        for (int j = 0; j < yBase.length; j++) {
            for (int i = 0; i < xBase.length; i++) {
                direction[0] = xDirection[j * xBase.length + i];
                direction[1] = yDirection[j * xBase.length + i];
                direction[2] = zDirection[j * xBase.length + i];

                double norm = Math.sqrt(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]);

                int colorIndex  = (int) Math.floor((((double) colorMapSize - 1.0) * norm / maxLength) + 0.5);

                float[] color = colorMap.getScilabColor(colorIndex);

                writeSegmentColors(buffer, elementsSize, bufferOffset, color);

                bufferOffset += 2 * elementsSize;
            }
        }

        /* Arrow colors */
        for (int j = 0; j < yBase.length; j++) {
            for (int i = 0; i < xBase.length; i++) {
                direction[0] = xDirection[j * xBase.length + i];
                direction[1] = yDirection[j * xBase.length + i];
                direction[2] = zDirection[j * xBase.length + i];

                double norm = Math.sqrt(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]);

                int colorIndex  = (int) Math.floor((((double) colorMapSize - 1.0) * norm / maxLength) + 0.5);

                float[] color = colorMap.getScilabColor(colorIndex);

                writeArrowColors(buffer, elementsSize, bufferOffset, color);

                bufferOffset += 3 * elementsSize;
            }
        }

    }

    /**
     * Returns the number of triangle indices for the given object.
     * Triangle indices correspond to arrow tips.
     * @param id the id of the given object.
     * @return the number of triangle indices.
     */
    public static int getIndicesSize(Integer id) {
        int numberArrows = (Integer) GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

        /* 3 indices per arrow */
        return 3 * numberArrows;
    }

    /**
     * Fills the input buffer with triangle index data from the given object.
     * Triangle indices are the indices of arrow tips.
     * @param buffer the buffer to fill.
     * @param id the id of the given object.
     * @param logMask the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    public static int fillIndices(IntBuffer buffer, Integer id, int logMask) {
        boolean valid;
        int bufferOffset = 0;
        int segmentOffset;

        int numberArrows = (Integer) GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

        if (numberArrows == 0) {
            return 0;
        }

        segmentOffset = 2 * numberArrows;

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
                int tipIndex = 3 * (xBase.length * j + i) + segmentOffset;
                int leftIndex = tipIndex + 1;
                int rightIndex = tipIndex + 2;

                base[0] = xBase[i];
                base[1] = yBase[j];
                base[2] = zBase[j * xBase.length + i];

                direction[0] = xDirection[j * xBase.length + i];
                direction[1] = yDirection[j * xBase.length + i];
                direction[2] = zDirection[j * xBase.length + i];

                /* Compute the segment tip */
                direction = computeSegmentTip(base, direction, colored, maxUsableLength, maxLength);

                if (Utils.isValid(base[0], base[1], base[2])
                        && Utils.isValid(direction[0], direction[1], direction[2])) {
                    valid = true;
                } else {
                    valid = false;
                }

                if (logMask != 0) {
                    valid = valid && Utils.isLogValid(base[0], base[1], base[2] + DEFAULT_LOG_COORD_Z, logMask);
                    valid = valid && Utils.isLogValid(direction[0], direction[1], direction[2] + DEFAULT_LOG_COORD_Z, logMask);
                }

                if (valid) {
                    buffer.put(bufferOffset, tipIndex);
                    buffer.put(bufferOffset + 1, leftIndex);
                    buffer.put(bufferOffset + 2, rightIndex);
                    bufferOffset += 3;
                }

            }
        }

        if (bufferOffset > 0) {
            return bufferOffset;
        } else {
            return 0;
        }
    }

    /**
     * Computes and returns the coordinates of a vector segment's tip.
     * @param base the vector base's (x,y,z) coordinates (3-element array).
     * @param direction the vector's (x,y,z) direction (3-element array).
     * @param colored specifies whether the vector is colored or not.
     * @param maxUsableLength the maximum usable vector length.
     * @param maxLength the maximum vector length.
     * @return the (x,y,z) coordinates of the tip (3-element array).
     */
    private static double[] computeSegmentTip(double[] base, double[] direction, boolean colored, double maxUsableLength, double maxLength) {
        double[] tip = new double[3];

        /* The current vector's norm is used to normalize for colored Champ objects instead of the maximum length. */
        if (colored) {
            double norm = Math.sqrt(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]);

            if (norm <= Double.MIN_VALUE) {
                norm = 1.0;
            }

            tip[0] = base[0] + direction[0] * maxUsableLength / norm;
            tip[1] = base[1] + direction[1] * maxUsableLength / norm;
            tip[2] = base[2] + direction[2] * maxUsableLength / norm;
        } else {
            tip[0] = base[0] + direction[0] * maxUsableLength / maxLength;
            tip[1] = base[1] + direction[1] * maxUsableLength / maxLength;
            tip[2] = base[2] + direction[2] * maxUsableLength / maxLength;
        }

        return tip;
    }

    /**
     * Fills the input buffer with segment index data from the given object.
     * To do: optimize, as the test on colored's value is performed for each segment.
     * @param buffer the buffer to fill.
     * @param id the id of the given object.
     * @param logMask the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    public static int fillWireIndices(IntBuffer buffer, Integer id, int logMask) {
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
                int baseIndex = 2 * (xBase.length * j + i);
                int tipIndex = baseIndex + 1;

                base[0] = xBase[i];
                base[1] = yBase[j];
                base[2] = zBase[j * xBase.length + i];

                direction[0] = xDirection[j * xBase.length + i];
                direction[1] = yDirection[j * xBase.length + i];
                direction[2] = zDirection[j * xBase.length + i];

                /* Compute the segment tip */
                direction = computeSegmentTip(base, direction, colored, maxUsableLength, maxLength);

                if (Utils.isValid(base[0], base[1], base[2])
                        && Utils.isValid(direction[0], direction[1], direction[2])) {
                    valid = true;
                } else {
                    valid = false;
                }

                if (logMask != 0) {
                    valid = valid && Utils.isLogValid(base[0], base[1], base[2] + DEFAULT_LOG_COORD_Z, logMask);
                    valid = valid && Utils.isLogValid(direction[0], direction[1], direction[2] + DEFAULT_LOG_COORD_Z, logMask);
                }

                if (valid) {
                    buffer.put(bufferOffset, baseIndex);
                    buffer.put(bufferOffset + 1, tipIndex);
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
