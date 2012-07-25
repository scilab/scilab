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

    private static Integer indexToSelect = null;

    /**
    * Select a datatip when click on it
    *
    * @param figureUid Figure unique identifier.
    * @param coordIntX Integer with pixel mouse position x.
    * @param coordIntY Integer with pixel mouse position y.
    * @param datatipsCoord Arraylist containing all created datatip's user coordinates.
    * @param datatipsUid Arraylist containing all created datatip's unique identifier.
    * @param markersUid Arraylist containing all created merker's unique identifier.
    * @return Integer index of selected datatip.
    */
    public static Integer selectDatatip (String figureUid, Integer coordIntX, Integer coordIntY, ArrayList<Double> datatipsCoord, ArrayList<String> datatipsUid, ArrayList<String> markersUid) {

        Integer[] pixelMouseCoordInt = { coordIntX , coordIntY };
        String axesUid = DatatipCreate.datatipAxesHandler(figureUid, pixelMouseCoordInt);
        double[] clickPositionDouble = DatatipCreate.transformPixelCoordToDouble(pixelMouseCoordInt);
        double[] clickGraphPos = DatatipCreate.transformPixelCoordToGraphic(axesUid, clickPositionDouble);

        for (int i = 0 ; i < datatipsCoord.size() ; i = i + 2) {

            String markerid = markersUid.get (i / 2);
            Double[] markerCorners = (Double[]) GraphicController.getController().getProperty(markerid, GraphicObjectProperties.__GO_CORNERS__);
            if (clickGraphPos[0] >= markerCorners[0] & clickGraphPos[0] <= markerCorners[6]) {
                if (clickGraphPos[1] >= markerCorners[1] & clickGraphPos[1] <= markerCorners[7]) {
                    indexToSelect = i;
                    DatatipHighlight.highlightSelected (indexToSelect, markersUid);
                    return indexToSelect;
                }
            }
        }
        indexToSelect = null;
        DatatipHighlight.highlightSelected (indexToSelect, markersUid);
        return indexToSelect;
    }
}
