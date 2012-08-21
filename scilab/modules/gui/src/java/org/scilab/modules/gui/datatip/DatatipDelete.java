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
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.renderer.CallRenderer;

import org.scilab.modules.gui.editor.EntityPicker;

/**
 * Delete a datatip
 * @author Gustavo Barbosa Libotte
 */
public class DatatipDelete {

    static EntityPicker ep = new EntityPicker();

    /**
    * Delete a specific datatip using the right mouse click;
    *
    * @param markerUid Datatip marker unique identifier.
    */
    public static void deleteDatatip (String markerUid) {

        String axesUid = (String) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_PARENT__);
        String figureUid = (String) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_PARENT__);

        Double[] datatipPosition = (Double[]) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_POSITION__);

        double[] graphCoordDouble = new double[]{0.0, 0.0, 0.0};
        graphCoordDouble[0] = datatipPosition[0];
        graphCoordDouble[1] = datatipPosition[1];
        graphCoordDouble[2] = datatipPosition[2];

        String polylineUid = getPolylineOfDatatip (graphCoordDouble, markerUid);

        String[] childrenUid = (String[]) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_CHILDREN__);
        for (int i = 0 ; i < childrenUid.length ; i++) {
            if (markerUid == childrenUid[i]) {
                GraphicController.getController().removeRelationShipAndDelete(childrenUid[i]);
                GraphicController.getController().removeRelationShipAndDelete(childrenUid[i + 1]);
                removePoint (polylineUid, graphCoordDouble[0], graphCoordDouble[1]);
            }
        }
    }

    /**
    * Delete a specific datatip using the command "datatipRemove(polyline_handle,ind)";
    *
    * @param polylineUid Polyline unique identifier.
    * @param indexRemove Index of the datatip to be removed.
    */
    public static void datatipRemoveProgramIndex (String polylineUid, int indexRemove) {

        int numDatatips = 0;
        String compoundUid = (String) GraphicController.getController().getProperty(polylineUid, GraphicObjectProperties.__GO_PARENT__);
        String axesUid = (String) GraphicController.getController().getProperty(compoundUid, GraphicObjectProperties.__GO_PARENT__);
        String[] childrenUid = (String[]) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_CHILDREN__);
        for (int i = 0 ; i < childrenUid.length ; i++) {
            String objType = (String) GraphicController.getController().getProperty(childrenUid[i], GraphicObjectProperties.__GO_TYPE__);
            if (objType == "Text") {
                numDatatips++;
            }
        }
        numDatatips = (numDatatips / 2);
        if (indexRemove <= numDatatips & indexRemove > 0) {
            int indexDel = (2 * indexRemove) - 1;
            String datatipUid = childrenUid[indexDel];
            String markerUid = childrenUid[indexDel - 1];
            Double[] datatipPosition = (Double[]) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_POSITION__);
            removePoint (polylineUid, datatipPosition[0], datatipPosition[1]);
            GraphicController.getController().removeRelationShipAndDelete(datatipUid);
            GraphicController.getController().removeRelationShipAndDelete(markerUid);
            
        }

    }

    /**
    * Delete a specific datatip using the command "datatipRemove(datatip_handle)";
    *
    * @param datatipUid Datatip unique identifier.
    * @param figureUid Figure unique identifier.
    */
    public static void datatipRemoveProgramHandler (String datatipUid, String figureUid) {

        int posDelete = -1;
        String axesUid = null;
        String[] childrenUid = (String[]) GraphicController.getController().getProperty(figureUid, GraphicObjectProperties.__GO_CHILDREN__);
        for (int i = 0 ; i < childrenUid.length ; i++) {
            String objType = (String) GraphicController.getController().getProperty(childrenUid[i], GraphicObjectProperties.__GO_TYPE__);
            if (objType == "Axes") {
                axesUid = childrenUid[i];
            }
        }
        if (axesUid != null){
            childrenUid = (String[]) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_CHILDREN__);
            String objType = (String) GraphicController.getController().getProperty(datatipUid, GraphicObjectProperties.__GO_TYPE__);
            if (objType == "Text") {
                for (int i = 0 ; i < childrenUid.length ; i++) {
                    if (datatipUid.equals(childrenUid[i])) {
                        posDelete = i;
                    }
                }
            }
            if (posDelete >= 0) {
                Double[] datatipPosition = (Double[]) GraphicController.getController().getProperty(datatipUid, GraphicObjectProperties.__GO_POSITION__);
                double[] graphCoordDouble = new double[]{0.0, 0.0, 0.0};
                graphCoordDouble[0] = datatipPosition[0];
                graphCoordDouble[1] = datatipPosition[1];
                graphCoordDouble[2] = datatipPosition[2];
                String polylineUid = getPolylineOfDatatip (graphCoordDouble, datatipUid);
                removePoint (polylineUid, datatipPosition[0], datatipPosition[1]);
                GraphicController.getController().removeRelationShipAndDelete(childrenUid[posDelete]);
                if (posDelete % 2 == 0) {
                    GraphicController.getController().removeRelationShipAndDelete(childrenUid[posDelete + 1]);
                } else {
                    GraphicController.getController().removeRelationShipAndDelete(childrenUid[posDelete - 1]);
                }
            }
        }
    }

    /**
    * Get the polyline which the datatip belongs
    *
    * @param graphCoordDouble double array with graphic position x and y of the datatip.
    * @param datatipUid Datatip unique identifier.
    * @return Polyline unique identifier string.
    */
    private static String getPolylineOfDatatip (double[] graphCoordDouble, String datatipUid) {

        String axesUid = (String) GraphicController.getController().getProperty(datatipUid, GraphicObjectProperties.__GO_PARENT__);
        String figureUid = (String) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_PARENT__);

        double[] pixelCoordinates = new double[]{0.0, 0.0};
        pixelCoordinates = CallRenderer.getPixelFrom2dViewCoordinates(axesUid, graphCoordDouble);

        int xInt = (int) pixelCoordinates[0];
        int yInt = (int) pixelCoordinates[1];

        Integer[] coordInteger = new Integer[]{0, 0};
        coordInteger[0] = (Integer) xInt;
        coordInteger[1] = (Integer) yInt;

        String polylineUid = ep.pick (figureUid, coordInteger[0], coordInteger[1]);

        return polylineUid;
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
        Integer numDatatips = (Integer) GraphicController.getController().getProperty(polylineUid, GraphicObjectProperties.__GO_DATATIPS_SIZE__);
        int xSkip = -1;
        int ySkip = -1;
        if (numDatatips > 0) {
            int allocArray = (2*numDatatips) - 2;
            Double[] newDatatips = new Double[allocArray];
            if (allocArray == 0) {
                GraphicController.getController().setProperty(polylineUid, GraphicObjectProperties.__GO_DATATIPS__, newDatatips);
            } else {
                for (int i = 0 ; i < currentDatatips.length ; i++) {
                    if (currentDatatips[i] == coordX & currentDatatips[i+numDatatips] == coordY) {
                        xSkip = i;
                        ySkip = i+numDatatips;
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
