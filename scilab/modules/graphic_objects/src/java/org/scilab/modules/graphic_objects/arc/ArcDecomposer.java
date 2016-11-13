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

package org.scilab.modules.graphic_objects.arc;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.utils.Utils;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Class ArcDecomposer
 *
 * A set of static methods decomposing an arc into a set of triangles (when filled)
 * and a set of line segments. Decomposition uses a fixed number of sectors, NB_SECTORS+2
 * vertices are output, the first vertex (at index 0) being the arc's center, whereas vertices
 * 1 and NB_SECTORS+1 respectively correspond to the arc's first (theta=START_ANGLE)
 * and last (theta=START_ANGLE+END_ANGLE) vertices.
 *
 * To do: implement the Nurbs drawing mode.
 *
 * @author Manuel JULIACHS
 */
public class ArcDecomposer {

    /** The number of sectors */
    private static final int NB_SECTORS = 64;

    /**
     * @return the number of data elements.
     */
    public static int getDataSize() {
        return (NB_SECTORS + 2);
    }

    /**
     * @return the number of triangle indices.
     */
    public static int getIndicesSize() {
        return 3 * NB_SECTORS;
    }

    /**
     * @return the number of line segment indices.
     */
    public static int getWireIndicesSize() {
        return 2 * NB_SECTORS;
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

        double [] center;
        double [] currentPoint;
        double semiMajorAxis;
        double semiMinorAxis;

        float [] coords;

        int offset = 0;

        coords = new float[3];

        Double [] leftUpperPoint = (Double []) GraphicController.getController().getProperty(id, __GO_UPPER_LEFT_POINT__);
        Double width = (Double) GraphicController.getController().getProperty(id, __GO_WIDTH__);
        Double height = (Double) GraphicController.getController().getProperty(id, __GO_HEIGHT__);

        /* Radians */
        Double startAngle = (Double) GraphicController.getController().getProperty(id, __GO_START_ANGLE__);
        Double endAngle = (Double) GraphicController.getController().getProperty(id, __GO_END_ANGLE__);

        center = new double[3];
        currentPoint = new double[3];

        semiMajorAxis = width / 2.0;
        semiMinorAxis = height / 2.0;

        center[0] = leftUpperPoint[0] + semiMajorAxis;
        center[1] = leftUpperPoint[1] - semiMinorAxis;
        center[2] = leftUpperPoint[2];

        /* First vertex: center */
        if ((coordinateMask & 0x1) != 0) {
            currentPoint[0] = center[0];

            if ((logMask & 0x1) != 0) {
                currentPoint[0] = Math.log10(currentPoint[0]);
            }

            coords[0] = (float) (currentPoint[0] * scale[0] + translation[0]);
            buffer.put(offset, coords[0]);
        }

        if ((coordinateMask & 0x2) != 0) {
            currentPoint[1] = center[1];

            if ((logMask & 0x2) != 0) {
                currentPoint[1] = Math.log10(currentPoint[1]);
            }

            coords[1] = (float) (currentPoint[1] * scale[1] + translation[1]);
            buffer.put(offset + 1, coords[1]);
        }

        if ((coordinateMask & 0x4) != 0) {
            currentPoint[2] = center[2];

            if ((logMask & 0x4) != 0) {
                currentPoint[2] = Math.log10(currentPoint[2]);
            }

            coords[2] = (float) (currentPoint[2] * scale[2] + translation[2]);
            buffer.put(offset + 2, coords[2]);
        }

        if (elementsSize == 4 && (coordinateMask & 0x8) != 0) {
            buffer.put(offset + 3, 1.0f);
        }

        offset += elementsSize;

        if (endAngle > 2 * Math.PI) {
            endAngle = 2 * Math.PI;
        } else if (endAngle < -2 * Math.PI) {
            endAngle = -2 * Math.PI;
        }

        startAngle = startAngle % (2.0 * Math.PI);


        /* Arc vertices */
        for (int i = 0; i < NB_SECTORS + 1; i++) {
            double theta = startAngle + endAngle * (double) i / (double) NB_SECTORS;

            if ((coordinateMask & 0x1) != 0) {
                currentPoint[0] = center[0] + semiMajorAxis * Math.cos(theta);

                if ((logMask & 0x1) != 0) {
                    currentPoint[0] = Math.log10(currentPoint[0]);
                }

                coords[0] = (float) (currentPoint[0] * scale[0] + translation[0]);
                buffer.put(offset, coords[0]);
            }

            if ((coordinateMask & 0x2) != 0) {
                currentPoint[1] = center[1] + semiMinorAxis * Math.sin(theta);

                if ((logMask & 0x2) != 0) {
                    currentPoint[1] = Math.log10(currentPoint[1]);
                }

                coords[1] = (float) (currentPoint[1] * scale[1] + translation[1]);
                buffer.put(offset + 1, coords[1]);
            }

            if ((coordinateMask & 0x4) != 0) {
                currentPoint[2] = center[2];

                if ((logMask & 0x4) != 0) {
                    currentPoint[2] = Math.log10(currentPoint[2]);
                }

                coords[2] = (float) (currentPoint[2] * scale[2] + translation[2]);
                buffer.put(offset + 2, coords[2]);
            }

            if (elementsSize == 4 && (coordinateMask & 0x8) != 0) {
                buffer.put(offset + 3, 1.0f);
            }

            offset +=  elementsSize;
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
        boolean valid;

        valid = isValid(id, logMask);

        if (valid) {
            for (int i = 0; i < NB_SECTORS; i++) {
                buffer.put(0);
                buffer.put(i + 1);
                buffer.put(i + 2);
            }

            return 3 * NB_SECTORS;
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
        boolean valid;

        valid = isValid(id, logMask);

        if (valid) {
            for (int i = 0; i < NB_SECTORS; i++) {
                buffer.put(i + 1);
                buffer.put(i + 2);
            }

            return 2 * NB_SECTORS;
        } else {
            return 0;
        }
    }

    /**
     * Determines whether an Arc is valid or not, depending on its data values.
     * @param the Arc id.
     * @param the bit mask specifying whether logarithmic coordinates are used.
     * @return true if the Arc is valid, false if it is not.
     */
    private static boolean isValid(Integer id, int logMask) {
        boolean valid = false;

        Double [] leftUpperPoint = (Double []) GraphicController.getController().getProperty(id, __GO_UPPER_LEFT_POINT__);
        Double width = (Double) GraphicController.getController().getProperty(id, __GO_WIDTH__);
        Double height = (Double) GraphicController.getController().getProperty(id, __GO_HEIGHT__);
        Double startAngle = (Double) GraphicController.getController().getProperty(id, __GO_START_ANGLE__);
        Double endAngle = (Double) GraphicController.getController().getProperty(id, __GO_END_ANGLE__);

        if (Utils.isValid(leftUpperPoint[0], leftUpperPoint[1], leftUpperPoint[2])
                && Utils.isValid(width) && Utils.isValid(height)
                && Utils.isValid(startAngle) && Utils.isValid(endAngle)) {
            valid = true;
        }

        /*
         * Test log validity using the minimum x and y coordinates,
         * as width and height can be less than 0 .
         */
        if (logMask != 0) {
            double [] minPoint = new double[2];

            minPoint[0] = Math.min(leftUpperPoint[0], leftUpperPoint[0] + width);
            minPoint[1] = Math.min(leftUpperPoint[1], leftUpperPoint[1] - height);

            valid = valid && Utils.isLogValid(minPoint[0], minPoint[1], leftUpperPoint[2], logMask);
        }

        return valid;
    }
}
