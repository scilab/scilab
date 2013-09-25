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
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.CallRenderer;

import org.scilab.modules.gui.editor.ObjectSearcher;
import org.scilab.modules.graphic_objects.PolylineData;


/**
* Implements all polyline manipulation functions for the editor.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/

public class PolylineHandler {

    static PolylineHandler instance = null;

    public PolylineHandler() {
    }

    public static PolylineHandler getInstance() {
        if (instance == null) {
            instance = new PolylineHandler();
        }
        return instance;
    }

    /**
    * Deletes all child entities from the given object.
    *
    * @param uid Object unique identifier.
    */
    public void deleteAll(String uid) {

        Integer type = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_TYPE__);
        Integer childCount = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN_COUNT__);
        String[] child = (String[])GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN__);
        if (type == GraphicObjectProperties.__GO_AXES__) {
            for (Integer i = 0; i < childCount; ++i) {
                Integer cType = (Integer)GraphicController.getController().getProperty(child[i], GraphicObjectProperties.__GO_TYPE__);
                if (cType != GraphicObjectProperties.__GO_LABEL__) {
                    CommonHandler.delete(child[i]);
                } else {
                    String[] text = { "" };
                    GraphicController.getController().setProperty(child[i], GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
                }
            }
        } else {

            for (Integer i = 0; i < childCount; ++i) {
                deleteAll(child[i]);
            }
        }

    }


    private String[] searchCompound(String uid) {
        return (new ObjectSearcher()).search(uid, GraphicObjectProperties.__GO_COMPOUND__);
    }


    /**
    * Drag the given polyline, add the difference of the click position to the polyline data
    *
    * @param polyline The uid of the polyline
    * @param position The click position
    * @param nextPosition The drag position
    */
    public void dragPolyline(String polyline, Integer[] position, Integer[] nextPosition) {

        String axes = (String) GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_PARENT_AXES__);
        if (polyline != null && axes != null) {
            double[] pos0 = { position[0] * 1.0, position[1] * 1.0};
            double[] pos1 = { nextPosition[0] * 1.0, nextPosition[1] * 1.0 };
            double[] coord0 = CallRenderer.get2dViewFromPixelCoordinates(axes, pos0);
            double[] coord1 = CallRenderer.get2dViewFromPixelCoordinates(axes, pos1);
            double[] coordDiff = {coord1[0] - coord0[0], coord1[1] - coord0[1]};
            boolean[] logFlags = new boolean[] {(Boolean)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_X_AXIS_LOG_FLAG__),
                                                (Boolean)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Y_AXIS_LOG_FLAG__),
                                                (Boolean)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Z_AXIS_LOG_FLAG__)
                                               };
            PolylineData.translatePolyline(polyline, coordDiff[0], coordDiff[1], 0.0, logFlags[0] ? 1 : 0, logFlags[1] ? 1 : 0, logFlags[2] ? 1 : 0);
            GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_DATA_MODEL__, polyline);
        }
    }


}

