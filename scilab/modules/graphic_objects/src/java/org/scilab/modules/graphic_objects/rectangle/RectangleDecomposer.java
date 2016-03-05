/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Manuel JULIACHS
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

package org.scilab.modules.graphic_objects.rectangle;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.utils.Utils;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Class RectangleDecomposer
 * A set of static methods decomposing a rectangle into two triangles.
 * Vertex indices 0 to 3 respectively correspond to the lower-left, lower-right,
 * upper-left and upper-right corners.
 * @author Manuel JULIACHS
 */
public class RectangleDecomposer {

    /** The number of vertices of a rectangle. */
    private static final int NUMBER_VERTICES = 4;

    /** The triangle indices of a decomposed rectangle */
    private static final int [] triangleIndices = {0, 1, 3, 0, 3, 2};

    /** The line segment indices of a decomposed rectangle */
    private static final int [] lineIndices = {0, 1, 1, 3, 3, 2, 2, 0};

    /**
     * @return the number of data elements.
     */
    public static int getDataSize() {
        return NUMBER_VERTICES;
    }

    /**
     * @return the number of triangle indices.
     */
    public static int getIndicesSize() {
        return triangleIndices.length;
    }

    /**
     * @return the number of line segment indices.
     */
    public static int getWireIndicesSize() {
        return lineIndices.length;
    }

    /**
     * Fills the input buffer with vertex data from the given object.
     * @param the buffer to fill.
     * @param the given object id.
     * @param the number of coordinates taken by one element in the buffer.
     * @param the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param the conversion scale factor to apply to data.
     * @param the conversion translation value to apply to data.
     * @param the bit mask specifying whether logarithmic coordinates are used.
     */
    public static void fillVertices(FloatBuffer buffer, Integer id, int elementsSize,
                                    int coordinateMask, double[] scale, double[] translation, int logMask) {

        /* The x coordinates of the lower-left and upper-right corners */
        double [] xcoords;
        /* The y coordinates of the lower-left and upper-right corners */
        double [] ycoords;
        /* The rectangle's z coordinate */
        double zcoord;

        xcoords = new double[2];
        ycoords = new double[2];

        Double [] upperLeftPoint = (Double []) GraphicController.getController().getProperty(id, __GO_UPPER_LEFT_POINT__);
        Double width = (Double) GraphicController.getController().getProperty(id, __GO_WIDTH__);
        Double height = (Double) GraphicController.getController().getProperty(id, __GO_HEIGHT__);

        xcoords[0] = upperLeftPoint[0];
        xcoords[1] = upperLeftPoint[0] + width;

        ycoords[0] = upperLeftPoint[1] - height;
        ycoords[1] = upperLeftPoint[1];

        zcoord = upperLeftPoint[2];

        /* Vertices are output in the following order: lower-left, lower-right, upper-left and upper-right. */
        if ((coordinateMask & 0x1) != 0) {
            if ((logMask & 0x1) != 0) {
                xcoords[0] = Math.log10(xcoords[0]);
                xcoords[1] = Math.log10(xcoords[1]);
            }

            xcoords[0] = xcoords[0] * scale[0] + translation[0];
            xcoords[1] = xcoords[1] * scale[0] + translation[0];

            buffer.put(0, (float) xcoords[0]);
            buffer.put(elementsSize, (float) xcoords[1]);
            buffer.put(2 * elementsSize, (float) xcoords[0]);
            buffer.put(3 * elementsSize, (float) xcoords[1]);
        }

        if ((coordinateMask & 0x2) != 0) {
            if ((logMask & 0x2) != 0) {
                ycoords[0] = Math.log10(ycoords[0]);
                ycoords[1] = Math.log10(ycoords[1]);
            }

            ycoords[0] = ycoords[0] * scale[1] + translation[1];
            ycoords[1] = ycoords[1] * scale[1] + translation[1];

            buffer.put(1, (float) ycoords[0]);
            buffer.put(elementsSize + 1, (float) ycoords[0]);
            buffer.put(2 * elementsSize + 1, (float) ycoords[1]);
            buffer.put(3 * elementsSize + 1, (float) ycoords[1]);
        }

        if ((coordinateMask & 0x4) != 0) {
            if ((logMask & 0x4) != 0) {
                zcoord = Math.log10(zcoord);
            }

            zcoord = zcoord * scale[2] + translation[2];

            buffer.put(2, (float) zcoord);
            buffer.put(elementsSize + 2, (float) zcoord);
            buffer.put(2 * elementsSize + 2, (float) zcoord);
            buffer.put(3 * elementsSize + 2, (float) zcoord);
        }

        if (elementsSize == 4 && (coordinateMask & 0x8) != 0) {
            buffer.put(3, 1.0f);
            buffer.put(elementsSize + 3, 1.0f);
            buffer.put(2 * elementsSize + 3, 1.0f);
            buffer.put(3 * elementsSize + 3, 1.0f);
        }
    }

    /**
     * Fills the input buffer with triangle index data from the given object.
     * @param the buffer to fill.
     * @param the given object id.
     * @param the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    public static int fillIndices(IntBuffer buffer, Integer id, int logMask) {
        boolean valid = false;

        Double [] upperLeftPoint = (Double []) GraphicController.getController().getProperty(id, __GO_UPPER_LEFT_POINT__);
        Double width = (Double) GraphicController.getController().getProperty(id, __GO_WIDTH__);
        Double height = (Double) GraphicController.getController().getProperty(id, __GO_HEIGHT__);

        /* Indices are output only if all the rectangle's values are valid. */
        if (Utils.isValid(upperLeftPoint[0], upperLeftPoint[1], upperLeftPoint[2])
                && Utils.isValid(width) && Utils.isValid(height)) {
            valid = true;
        }

        /*
         * Only the lower-left corner's log validity is tested, since width and height are supposed
         * to be greater than or equal to 0 .
         */
        if (logMask != 0) {
            valid = valid && Utils.isLogValid(upperLeftPoint[0], upperLeftPoint[1] - height, upperLeftPoint[2], logMask);
        }

        if (valid) {
            buffer.put(triangleIndices);
            return triangleIndices.length;
        } else {
            return 0;
        }
    }

    /**
     * Fills the input buffer with segment index data from the given object.
     * @param the buffer to fill.
     * @param the given object id.
     * @param the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    public static int fillWireIndices(IntBuffer buffer, Integer id, int logMask) {
        boolean valid = false;

        Double [] upperLeftPoint = (Double []) GraphicController.getController().getProperty(id, __GO_UPPER_LEFT_POINT__);
        Double width = (Double) GraphicController.getController().getProperty(id, __GO_WIDTH__);
        Double height = (Double) GraphicController.getController().getProperty(id, __GO_HEIGHT__);

        /* Segment indices are output only if all the rectangle's values are valid. */
        if (Utils.isValid(upperLeftPoint[0], upperLeftPoint[1], upperLeftPoint[2])
                && Utils.isValid(width) && Utils.isValid(height)) {
            valid = true;
        }

        /*
         * Only the lower-left corner's log validity is tested, since width and height are supposed
         * to be greater than or equal to 0 .
         */
        if (logMask != 0) {
            valid = valid && Utils.isLogValid(upperLeftPoint[0], upperLeftPoint[1] - height, upperLeftPoint[2], logMask);
        }

        if (valid) {
            buffer.put(lineIndices);
            return lineIndices.length;
        } else {
            return 0;
        }
    }

}
