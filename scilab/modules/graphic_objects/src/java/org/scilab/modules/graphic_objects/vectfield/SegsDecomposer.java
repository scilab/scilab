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

import java.nio.IntBuffer;
import java.nio.FloatBuffer;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.utils.Utils;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Class SegsDecomposer
 * A set of static methods decomposing a Segs object into a set of line segments.
 * It takes into account the additional buffer space needed for arrow tip vertices and colors,
 * fills tip colors, and outputs their triangle indices. Arrow tip coordinates are, however,
 * computed at drawing time as they are projection-dependent.
 * @author Manuel JULIACHS
 */
public class SegsDecomposer extends VectFieldDecomposer {

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
     * Fills the given buffer with color data from the given object.
     * @param buffer the buffer to fill.
     * @param id the id of the given object.
     * @param elementsSize the number of components taken by one element in the buffer (3 or 4).
     */
    public static void fillColors(FloatBuffer buffer, Integer id, int elementsSize) {
        int bufferOffset = 0;
        int numberArrows = (Integer) GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

        Integer[] segmentColors = (Integer[]) GraphicController.getController().getProperty(id, __GO_SEGS_COLORS__);

        Integer parentFigureId = (Integer) GraphicController.getController().getProperty(id, __GO_PARENT_FIGURE__);

        /* To be sure that the object has a parent Figure, though it should not occur. */
        if (parentFigureId == 0) {
            return;
        }

        Figure parentFigure = (Figure) GraphicController.getController().getObjectFromId(parentFigureId);
        ColorMap colorMap = parentFigure.getColorMap();

        GraphicController.getController().getProperty(id, __GO_ARROW_SIZE__);

        /* Colors are output in the following order: first the base's color and then the tip's */
        for (int i = 0; i < numberArrows; i++) {
            float[] color = colorMap.getScilabColor(segmentColors[i]);

            bufferOffset = 2 * elementsSize * i;

            writeSegmentColors(buffer, elementsSize, bufferOffset, color);
        }

        /* Arrow colors */
        for (int i = 0; i < numberArrows; i++) {
            float[] color = colorMap.getScilabColor(segmentColors[i]);

            bufferOffset = 2 * elementsSize * numberArrows + 3 * elementsSize * i;

            writeArrowColors(buffer, elementsSize, bufferOffset, color);
        }
    }

    /**
     * Returns the number of triangle indices for the given object.
     * Triangle indices correspond to arrow tips.
     * @param id the object id.
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

        double[] base = new double[3];
        double[] direction = new double[3];

        for (int i = 0; i < numberArrows; i++) {
            int tipIndex = segmentOffset + 3 * i;
            int leftIndex = tipIndex + 1;
            int rightIndex = tipIndex + 2;

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
                buffer.put(bufferOffset, tipIndex);
                buffer.put(bufferOffset + 1, leftIndex);
                buffer.put(bufferOffset + 2, rightIndex);

                bufferOffset += 3;
            }
        }

        if (bufferOffset > 0) {
            return bufferOffset;
        } else {
            return 0;
        }
    }

}
