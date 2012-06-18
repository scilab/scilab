/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.editor;


import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.*;

import org.scilab.modules.gui.editor.ObjectSearcher;
import org.scilab.modules.gui.editor.PolylineHandler;


/**
* Implements all axes manipulation functions for the editor.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class AxesHandler {

    private static String[] searchAxes(String uid) {
        return (new ObjectSearcher()).search(uid, GraphicObjectProperties.__GO_AXES__);
    }

    /**
    * Given a mouse coordinate point (x, y) in pixels
    * retrieves the axes wich the point belongs.
    *
    * @param figure Figure unique identifier.
    * @param position Vector with mouse position x and y.
    * @return Retrieved axes or null if there isn't an axes.
    */
    public static String clickedAxes(String figure, Integer[] position) {

        String[] axes = searchAxes(figure);
        if (axes == null) {
            return null;
        }
        Integer[] figureSize = (Integer[])GraphicController.getController().getProperty(figure, GraphicObjectProperties.__GO_AXES_SIZE__);

        for (Integer i = 0; i < axes.length; i++) {
            Double[] axesBound = (Double[])GraphicController.getController().getProperty(axes[i], GraphicObjectProperties.__GO_AXES_BOUNDS__);
            Double xi, xf, yi, yf;

            xi = (figureSize[0] * axesBound[0]);
            xf = (figureSize[0] * (axesBound[0] + axesBound[2]));
            yi = (figureSize[1] * axesBound[1]);
            yf = (figureSize[1] * (axesBound[1] + axesBound[3]));

            if (position[0] > xi && position[0] < xf && position[1] > yi && position[1] < yf) {
                return axes[i];
            }
        }
        return null;
    }



    public static void setAxesVisible(String axes) {

        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_X_AXIS_VISIBLE__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_Y_AXIS_VISIBLE__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_Z_AXIS_VISIBLE__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_VISIBLE__, true);
    }

    public static String duplicateAxes(String figureUid, String axes) {

        String newAxes = (String)GraphicController.getController().cloneObject(axes);
        setAxesVisible(newAxes);
        PolylineHandler.getInstance().deleteAll(newAxes);
        return newAxes;
    }

    public static void axesBound(String axesFrom, String axesTo) {

        Double[] axesFB = (Double[])GraphicController.getController().getProperty(axesFrom, GraphicObjectProperties.__GO_DATA_BOUNDS__);
        Double[] axesTB = (Double[])GraphicController.getController().getProperty(axesTo, GraphicObjectProperties.__GO_DATA_BOUNDS__);
        Double[] newBounds = {0., 0., 0., 0., 0., 0.};

        if (axesFB[0] < axesTB[0]) {
            newBounds[0] = axesFB[0];
        } else {
            newBounds[0] = axesTB[0];
        }

        if (axesFB[1] > axesTB[1]) {
            newBounds[1] = axesFB[1];
        } else {
            newBounds[1] = axesTB[1];
        }

        if (axesFB[2] < axesTB[2]) {
            newBounds[2] = axesFB[2];
        } else {
            newBounds[2] = axesTB[2];
        }

        if (axesFB[3] > axesTB[3]) {
            newBounds[3] = axesFB[3];
        } else {
            newBounds[3] = axesTB[3];
        }
        if (axesFB[4] < axesTB[4]) {
            newBounds[4] = axesFB[4];
        } else {
            newBounds[4] = axesTB[4];
        }

        if (axesFB[5] > axesTB[5]) {
            newBounds[5] = axesFB[5];
        } else {
            newBounds[5] = axesTB[5];
        }

        GraphicController.getController().setProperty(axesTo, GraphicObjectProperties.__GO_DATA_BOUNDS__, newBounds);
    }
}

