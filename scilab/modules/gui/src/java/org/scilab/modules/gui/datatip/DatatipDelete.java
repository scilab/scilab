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
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.editor.ObjectSearcher;
import org.scilab.modules.gui.datatip.DatatipCommon;

/**
 * Delete a datatip
 * @author Gustavo Barbosa Libotte
 */
public class DatatipDelete {

    /**
    * Delete a specific datatip using the right mouse click;
    *
    * @param datatipUid Datatip unique identifier.
    */
    public static void deleteDatatip(Integer datatipUid) {

        Double[] datatipPosition = (Double[]) GraphicController.getController().getProperty(datatipUid, GraphicObjectProperties.__GO_DATATIP_DATA__);

        double[] graphCoordDouble = new double[] {0.0, 0.0, 0.0};
        graphCoordDouble[0] = datatipPosition[0];
        graphCoordDouble[1] = datatipPosition[1];
        graphCoordDouble[2] = datatipPosition[2];


        GraphicController.getController().removeRelationShipAndDelete(datatipUid);
    }

    /**
    * Delete a specific datatip using the command "datatipRemove(polyline_handle,ind)";
    *
    * @param polylineUid Polyline unique identifier.
    * @param indexRemove Index of the datatip to be removed.
    */
    public static void datatipRemoveProgramIndex(Integer polylineUid, int indexRemove) {

        Integer[] datatips = (new ObjectSearcher()).search(polylineUid, GraphicObjectProperties.__GO_DATATIP__);

        if (datatips != null) {
            /* use index from 1 .. lenght (like scilab vectors)*/
            if (indexRemove >= 1 && indexRemove <= datatips.length) {
                Double[] datatipPosition = (Double[]) GraphicController.getController().getProperty(datatips[indexRemove - 1], GraphicObjectProperties.__GO_DATATIP_DATA__);
                GraphicController.getController().removeRelationShipAndDelete(datatips[indexRemove - 1]);
            }
        }


    }

    /**
    * Delete a specific datatip using the command "datatipRemove(datatip_handle)";
    *
    * @param datatipUid Datatip unique identifier.
    * @param figureUid Figure unique identifier.
    */
    public static void datatipRemoveProgramHandler(Integer datatipUid, Integer figureUid) {
        deleteDatatip(datatipUid);
    }

}
