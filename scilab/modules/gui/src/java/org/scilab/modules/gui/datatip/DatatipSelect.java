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

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.datatip.DatatipCreate;
import org.scilab.modules.gui.datatip.DatatipHighlight;

/**
 * Select a datatip
 * @author Gustavo Barbosa Libotte
 */
public class DatatipSelect {

    /**
    * Select a datatip when click on it
    *
    * @param figureUid Figure unique identifier.
    * @param coordIntX Integer with pixel mouse position x.
    * @param coordIntY Integer with pixel mouse position y.
    * @return String containing the datatip marker unique identifier
    */
    public static String selectDatatip (String figureUid, Integer coordIntX, Integer coordIntY) {

        Integer[] pixelMouseCoordInt = { coordIntX , coordIntY };
        String axesUid = DatatipCreate.datatipAxesHandler(figureUid, pixelMouseCoordInt);
        double[] clickPositionDouble = DatatipCreate.transformPixelCoordToDouble(pixelMouseCoordInt);
        double[] clickGraphPos = DatatipCreate.transformPixelCoordToGraphic(axesUid, clickPositionDouble);

        String[] axesChildrenUid = (String[]) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_CHILDREN__);

        for (int i = 0 ; i < axesChildrenUid.length ; i++) {

            String objType = (String) GraphicController.getController().getProperty(axesChildrenUid[i], GraphicObjectProperties.__GO_TYPE__);
            if (objType == "Text") {
                Integer textBoxMode = (Integer) GraphicController.getController().getProperty(axesChildrenUid[i], GraphicObjectProperties.__GO_TEXT_BOX_MODE__);
                if (textBoxMode == 1) {
                    Double[] markerCorners = (Double[]) GraphicController.getController().getProperty(axesChildrenUid[i], GraphicObjectProperties.__GO_CORNERS__);
                    if (clickGraphPos[0] >= markerCorners[0] & clickGraphPos[0] <= markerCorners[6]) {
                        if (clickGraphPos[1] >= markerCorners[1] & clickGraphPos[1] <= markerCorners[7]) {
                            DatatipHighlight.highlightSelected (axesChildrenUid[i]);
                            return axesChildrenUid[i];
                        }
                    }
                }
            }
        }
        DatatipHighlight.highlightSelected (null);
        return null;
    }
}
