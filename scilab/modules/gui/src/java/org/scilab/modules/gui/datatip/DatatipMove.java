/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.renderer.CallRenderer;


/**
 * Move a datatip along the curve
 * @author Gustavo Barbosa Libotte
 */
public class DatatipMove {

    public static double[] graphCoordDouble = new double[3];
    public static Integer[] coordInteger = new Integer[2];

    /**
    * Move a datatip to the right using keyboard
    *
    * @param markerUid datatip marker unique identifier
    */
    public static void moveRight (String datatipUid) {
        Integer[] pix_pos = getCoordInteger(datatipUid);
        pix_pos[0] += 1;
        DatatipDrag.dragDatatip(datatipUid, pix_pos[0], pix_pos[1]);
    }

    /**
    * Move a datatip to the left using keyboard
    *
    * @param datatipUid datatip unique identifier
    */
    public static void moveLeft (String datatipUid) {
        Integer[] pix_pos = getCoordInteger(datatipUid);
        pix_pos[0] -= 1;
        DatatipDrag.dragDatatip(datatipUid, pix_pos[0], pix_pos[1]);
    }

    /**
    * Get the pixel integer coordinates of a datatip
    *
    * @param datatipUid Datatip unique identifier
    * @return Array with x, y coordinates
    */
    public static Integer[] getCoordInteger(String datatipUid) {

        Double[] markerPosition = (Double[]) GraphicController.getController().getProperty(datatipUid, __GO_DATATIP_DATA__);
        for (int i = 0 ; i < graphCoordDouble.length ; i++) {
            graphCoordDouble[i] = markerPosition[i];
        }
        String axes = (String)GraphicController.getController().getProperty(datatipUid, __GO_PARENT_AXES__);
        if (axes != null) {
            double[] pixelCoordinates = CallRenderer.getPixelFrom2dViewCoordinates(axes, graphCoordDouble);
            int xInt = (int) pixelCoordinates[0];
            int yInt = (int) pixelCoordinates[1];
            coordInteger[0] = (Integer) xInt;
            coordInteger[1] = (Integer) yInt;
            return coordInteger;
        }
        return null;
    }

}
