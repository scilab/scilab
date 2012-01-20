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
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Class SegsDecomposer
 * A set of static methods decomposing a Segs object into a set of line segments.
 * To do: -output arrow tips as triangles.
 * @author Manuel JULIACHS
 */
public class SegsDecomposer extends VectFieldDecomposer {

    /**
     * Fills the given buffer with color data from the given object.
     * @param the buffer to fill.
     * @param the id of the given object.
     * @param the number of components taken by one element in the buffer (3 or 4).
     */
    public static void fillColors(FloatBuffer buffer, String id, int elementsSize) {
        int bufferOffset = 0;
        int numberArrows = (Integer) GraphicController.getController().getProperty(id, __GO_NUMBER_ARROWS__);

        Integer[] segmentColors = (Integer[]) GraphicController.getController().getProperty(id, __GO_SEGS_COLORS__);

        String parentFigureId = (String) GraphicController.getController().getProperty(id, __GO_PARENT_FIGURE__);

        /* To be sure that the object has a parent Figure, though it should not occur. */
        if (parentFigureId.equals("")) {
            return;
        }

        Figure parentFigure = (Figure) GraphicController.getController().getObjectFromId(parentFigureId);
        ColorMap colorMap = parentFigure.getColorMap();

        /* Colors are output in the following order: first the base's color and then the tip's */
        for (int i = 0; i < numberArrows; i++) {
            float[] color = colorMap.getScilabColor(segmentColors[i]);

            bufferOffset = 2*elementsSize*i;

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
        }
    }
}
