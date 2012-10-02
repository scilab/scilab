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
    public static void deleteDatatip (String datatipUid) {

        Double[] datatipPosition = (Double[]) GraphicController.getController().getProperty(datatipUid, GraphicObjectProperties.__GO_DATATIP_DATA__);

        double[] graphCoordDouble = new double[] {0.0, 0.0, 0.0};
        graphCoordDouble[0] = datatipPosition[0];
        graphCoordDouble[1] = datatipPosition[1];
        graphCoordDouble[2] = datatipPosition[2];

        String polylineUid = DatatipCommon.getParentPolyline(datatipUid);

        GraphicController.getController().removeRelationShipAndDelete(datatipUid);
        removePoint(polylineUid, graphCoordDouble[0], graphCoordDouble[1]);
    }

    /**
    * Delete a specific datatip using the command "datatipRemove(polyline_handle,ind)";
    *
    * @param polylineUid Polyline unique identifier.
    * @param indexRemove Index of the datatip to be removed.
    */
    public static void datatipRemoveProgramIndex (String polylineUid, int indexRemove) {

        String[] datatips = (new ObjectSearcher()).search(polylineUid, GraphicObjectProperties.__GO_DATATIPS__);

        if (datatips != null) {
            /* use index from 1 .. lenght (like scilab vectors)*/
            if (indexRemove >= 1 && indexRemove <= datatips.length) {
                Double[] datatipPosition = (Double[]) GraphicController.getController().getProperty(datatips[indexRemove - 1], GraphicObjectProperties.__GO_DATATIP_DATA__);
                GraphicController.getController().removeRelationShipAndDelete(datatips[indexRemove - 1]);
                removePoint (polylineUid, datatipPosition[0], datatipPosition[1]);
            }
        }


    }

    /**
    * Delete a specific datatip using the command "datatipRemove(datatip_handle)";
    *
    * @param datatipUid Datatip unique identifier.
    * @param figureUid Figure unique identifier.
    */
    public static void datatipRemoveProgramHandler (String datatipUid, String figureUid) {
        deleteDatatip(datatipUid);
    }


    /**
    * Remove the point of the deleted datatip from the datatips field;
    *
    * @param polylineUid Polyline unique identifier.
    * @param coordX Coordinate x to be removed.
    * @param coordY Coordinate y to be removed.
    */
    private static void removePoint (String polylineUid, double coordX, double coordY) {

        Double[] currentDatatips = (Double[]) GraphicController.getController().getProperty(polylineUid, GraphicObjectProperties.__GO_DATATIPS__);
        if (currentDatatips == null) {
            return;
        }
        Integer numDatatips = (Integer) GraphicController.getController().getProperty(polylineUid, GraphicObjectProperties.__GO_DATATIPS_SIZE__);
        int xSkip = -1;
        int ySkip = -1;
        if (numDatatips > 0) {
            int allocArray = (2 * numDatatips) - 2;
            Double[] newDatatips = new Double[allocArray];
            if (allocArray == 0) {
                GraphicController.getController().setProperty(polylineUid, GraphicObjectProperties.__GO_DATATIPS__, newDatatips);
            } else {
                for (int i = 0 ; i < currentDatatips.length ; i++) {
                    if (currentDatatips[i] == coordX && currentDatatips[i + numDatatips] == coordY) {
                        xSkip = i;
                        ySkip = i + numDatatips;
                        break;
                    }
                }
                int j = 0;
                for (int i = 0 ; i < currentDatatips.length ; i++) {
                    if (i != xSkip & i != ySkip) {
                        newDatatips[j] = currentDatatips[i];
                        j++;
                    }
                }
                GraphicController.getController().setProperty(polylineUid, GraphicObjectProperties.__GO_DATATIPS__, newDatatips);
            }
        }
    }
}
