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
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.utils.Utils;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Class VectFieldDecomposer
 * A set of static methods decomposing a vector field into a set of line segments.
 * @author Manuel JULIACHS
 */
public class VectFieldDecomposer {

    /** The default z-coordinate value when the logarithmic scale is used. */
    protected static final double DEFAULT_LOG_COORD_Z = 1.0;

    /**
     * Returns the number of data elements for the given object.
     * @param id the id of the given object.
     * @return the number of data elements.
     */
    public static int getDataSize(Integer id) {
        int numberArrows = (Integer) GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

        /* 2 vertices per arrow */
        return 2 * numberArrows;
    }

    /**
     * Fills the input buffer with vertex data from the given object.
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

        /* The x coordinates of a segment's base and direction */
        double[] xcoords;
        /* The y coordinates of a segment's base and direction */
        double[] ycoords;
        /* The z coordinates of a segment's base and direction */
        double[] zcoords;

        xcoords = new double[2];
        ycoords = new double[2];
        zcoords = new double[2];

        int numberArrows = (Integer) GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

        Double[] xBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_X__);
        Double[] yBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_Y__);
        Double[] zBase = (Double[]) GraphicController.getController().getProperty(id, __GO_BASE_Z__);

        Double[] xDirection = (Double[]) GraphicController.getController().getProperty(id, __GO_DIRECTION_X__);
        Double[] yDirection = (Double[]) GraphicController.getController().getProperty(id, __GO_DIRECTION_Y__);
        Double[] zDirection = (Double[]) GraphicController.getController().getProperty(id, __GO_DIRECTION_Z__);

        for (int i = 0; i < numberArrows; i++) {
            xcoords[0] = xBase[i];
            xcoords[1] = xDirection[i];
            ycoords[0] = yBase[i];
            ycoords[1] = yDirection[i];
            zcoords[0] = zBase[i];
            zcoords[1] = zDirection[i];

            /* Vertices are output in the following order: the segment's base and then its tip */
            if ((coordinateMask & 0x1) != 0) {
                if ((logMask & 0x1) != 0) {
                    xcoords[0] = Math.log10(xcoords[0]);
                    xcoords[1] = Math.log10(xcoords[1]);
                }

                xcoords[0] = xcoords[0] * scale[0] + translation[0];
                xcoords[1] = xcoords[1] * scale[0] + translation[0];

                buffer.put(bufferOffset, (float) xcoords[0]);
                buffer.put(bufferOffset + elementsSize, (float) xcoords[1]);
            }

            if ((coordinateMask & 0x2) != 0) {
                if ((logMask & 0x2) != 0) {
                    ycoords[0] = Math.log10(ycoords[0]);
                    ycoords[1] = Math.log10(ycoords[1]);
                }

                ycoords[0] = ycoords[0] * scale[1] + translation[1];
                ycoords[1] = ycoords[1] * scale[1] + translation[1];

                buffer.put(bufferOffset + 1, (float) ycoords[0]);
                buffer.put(bufferOffset + elementsSize + 1, (float) ycoords[1]);
            }

            if ((coordinateMask & 0x4) != 0) {
                if ((logMask & 0x4) != 0) {
                    zcoords[0] = Math.log10(zcoords[0]);
                    zcoords[1] = Math.log10(zcoords[1]);
                }

                zcoords[0] = zcoords[0] * scale[2] + translation[2];
                zcoords[1] = zcoords[1] * scale[2] + translation[2];

                buffer.put(bufferOffset + 2, (float) zcoords[0]);
                buffer.put(bufferOffset + elementsSize + 2, (float) zcoords[1]);
            }

            if (elementsSize == 4 && (coordinateMask & 0x8) != 0) {
                buffer.put(bufferOffset + 3, 1.0f);
                buffer.put(bufferOffset + elementsSize + 3, 1.0f);
            }

            bufferOffset += 2 * elementsSize;
        }
    }

    /**
     * Fills the given buffer with color data from the given object.
     * @param the buffer to fill.
     * @param the id of the given object.
     * @param the number of components taken by one element in the buffer (3 or 4).
     */
    public static void fillColors(FloatBuffer buffer, String id, int elementsSize) {
        return;
    }

    /**
     * Writes the colors of a segment's vertices into a buffer.
     * The colors of the 2 vertices are written consecutively starting from the specified offset.
     * @param buffer the buffer written to.
     * @param elementsSize the number of components taken by a color (3 or 4).
     * @param bufferOffset the offset of the first segment vertex's color.
     * @color the arrow color (3 elements: r, g, b components).
     */
    protected static void writeSegmentColors(FloatBuffer buffer, int elementsSize, int bufferOffset, float[] color) {
        buffer.put(bufferOffset, color[0]);
        buffer.put(bufferOffset + 1, color[1]);
        buffer.put(bufferOffset + 2, color[2]);

        buffer.put(bufferOffset + elementsSize, color[0]);
        buffer.put(bufferOffset + elementsSize + 1, color[1]);
        buffer.put(bufferOffset + elementsSize + 2, color[2]);

        if (elementsSize == 4) {
            buffer.put(bufferOffset + 3, 1.0f);
            buffer.put(bufferOffset + elementsSize + 3, 1.0f);
        }
    }

    /**
     * Writes the colors of an arrow's vertices into a buffer.
     * The colors of the 3 vertices are written consecutively starting from the specified offset.
     * @param buffer the buffer written to.
     * @param elementsSize the number of components taken by a color (3 or 4).
     * @param bufferOffset the offset of the first arrow vertex's color.
     * @color the arrow color (3 elements: r, g, b components).
     */
    protected static void writeArrowColors(FloatBuffer buffer, int elementsSize, int bufferOffset, float[] color) {
        buffer.put(bufferOffset, color[0]);
        buffer.put(bufferOffset + 1, color[1]);
        buffer.put(bufferOffset + 2, color[2]);

        buffer.put(bufferOffset + elementsSize, color[0]);
        buffer.put(bufferOffset + elementsSize + 1, color[1]);
        buffer.put(bufferOffset + elementsSize + 2, color[2]);

        buffer.put(bufferOffset + 2 * elementsSize, color[0]);
        buffer.put(bufferOffset + 2 * elementsSize + 1, color[1]);
        buffer.put(bufferOffset + 2 * elementsSize + 2, color[2]);

        if (elementsSize == 4) {
            buffer.put(bufferOffset + 3, 1.0f);
            buffer.put(bufferOffset + elementsSize + 3, 1.0f);
            buffer.put(bufferOffset + 2 * elementsSize + 3, 1.0f);
        }
    }

    /**
     * Returns the number of triangle indices.
     * @return the number of triangle indices.
     */
    public static int getIndicesSize() {
        return 0;
    }

    /**
     * Fills the input buffer with triangle index data from the given object.
     * @param buffer the buffer to fill.
     * @param id the id of the given object.
     * @param logMask the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    public static int fillIndices(IntBuffer buffer, String id, int logMask) {
        return 0;
    }

    /**
     * Returns the number of wireframe indices for the given object.
     * @param id the id of the given object.
     * @return the number of line segment indices.
     */
    public static int getWireIndicesSize(Integer id) {
        int numberArrows = (Integer) GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

        /* 2 segment indices per arrow */
        return 2 * numberArrows;
    }

    /**
     * Fills the input buffer with segment index data from the given object.
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

        double[] base = new double[3];
        double[] direction = new double[3];

        for (int i = 0; i < numberArrows; i++) {
            base[0] = xBase[i];
            base[1] = yBase[i];
            base[2] = zBase[i];

            direction[0] = xDirection[i];
            direction[1] = yDirection[i];
            direction[2] = zDirection[i];

            if (Utils.isValid(base[0], base[1], base[2])
                    && Utils.isValid(direction[0], direction[1], direction[2])) {
                valid = true;
            } else {
                valid = false;
            }

            if (logMask != 0) {
                valid = valid && Utils.isLogValid(base[0], base[1], base[2], logMask);
                valid = valid && Utils.isLogValid(direction[0], direction[1], direction[2], logMask);
            }

            if (valid) {
                buffer.put(bufferOffset, 2 * i);
                buffer.put(bufferOffset + 1, 2 * i + 1);

                bufferOffset += 2;
            }

        }

        if (bufferOffset > 0) {
            return bufferOffset;
        } else {
            return 0;
        }
    }

}
