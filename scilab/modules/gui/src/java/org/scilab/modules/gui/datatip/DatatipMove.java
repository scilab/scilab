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

import org.scilab.modules.graphic_objects.PolylineData;

import org.scilab.modules.renderer.CallRenderer;

import org.scilab.modules.gui.editor.EntityPicker;

import org.scilab.modules.gui.datatip.DatatipCreate;

/**
 * Move a datatip along the curve
 * @author Gustavo Barbosa Libotte
 */
public class DatatipMove {

    public static double[] graphCoordDouble = new double[3];
    public static EntityPicker ep = new EntityPicker();
    private static Integer[] datatipBounds = new Integer[2];
    public static Double[] coordsToMove = new Double[3];
    public static Integer[] coordInteger = new Integer[2];
    public static String datatipUid;
    public static String polylineUid;
    static double newX;
    static double newY;

    /**
    * Move a datatip to the right using keyboard
    *
    * @param markerUid datatip marker unique identifier
    */
    public static void moveRight (String markerUid) {

        String axesUid = (String) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_PARENT__);
        String figureUid = (String) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_PARENT__);
        String[] axesChildrenUid = (String[]) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_CHILDREN__);
        coordInteger = getCoordInteger (markerUid, axesUid);
        String polylineUid = ep.pick(figureUid, coordInteger[0], coordInteger[1]);
        double[] polylineDataX = (double[]) PolylineData.getDataX (polylineUid);
        double[] polylineDataY = (double[]) PolylineData.getDataY (polylineUid);
        coordsToMove = (Double[]) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_POSITION__);
        for (int i = 1 ; i < (polylineDataX.length - 2) ; i++) {
            if (coordsToMove[0] >= polylineDataX[i] & coordsToMove[0] <= polylineDataX[i + 1]) {
                newX = polylineDataX[i + 1];
                newY = polylineDataY[i + 1];
            }
        }
        for (int i = 0 ; i < axesChildrenUid.length ; i++) {
            if (axesChildrenUid[i] == markerUid) {
                datatipUid = axesChildrenUid[i + 1];
                break;
            }
        }
        setNewPosition (datatipUid, markerUid, newX, newY);
    }

    /**
    * Move a datatip to the left using keyboard
    *
    * @param markerUid datatip marker unique identifier
    */
    public static void moveLeft (String markerUid) {

        String axesUid = (String) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_PARENT__);
        String figureUid = (String) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_PARENT__);
        String[] axesChildrenUid = (String[]) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_CHILDREN__);
        coordInteger = getCoordInteger (markerUid, axesUid);
        String polylineUid = ep.pick(figureUid, coordInteger[0], coordInteger[1]);
        double[] polylineDataX = (double[]) PolylineData.getDataX (polylineUid);
        double[] polylineDataY = (double[]) PolylineData.getDataY (polylineUid);
        coordsToMove = (Double[]) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_POSITION__);
        for (int i = 2 ; i < (polylineDataX.length - 1) ; i++) {
            if (coordsToMove[0] >= polylineDataX[i] & coordsToMove[0] <= polylineDataX[i + 1]) {
                newX = polylineDataX[i - 1];
                newY = polylineDataY[i - 1];
            }
        }
        for (int i = 0 ; i < axesChildrenUid.length ; i++) {
            if (axesChildrenUid[i] == markerUid) {
                datatipUid = axesChildrenUid[i + 1];
                break;
            }
        }
        setNewPosition (datatipUid, markerUid, newX, newY);
    }

    public static Integer[] getCoordInteger (String markerUid, String axesUid) {

        Double[] markerPosition = (Double[]) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_POSITION__);
        for (int i = 0 ; i < graphCoordDouble.length ; i++) {
            graphCoordDouble[i] = markerPosition[i];
        }
        double[] pixelCoordinates = CallRenderer.getPixelFrom2dViewCoordinates(axesUid, graphCoordDouble);
        int xInt = (int) pixelCoordinates[0];
        int yInt = (int) pixelCoordinates[1];
        coordInteger[0] = (Integer) xInt;
        coordInteger[1] = (Integer) yInt;
        return coordInteger;
    }

    private static void setNewPosition (String datatipUid, String markerUid, double newX, double newY) {

        double[] newPosition = { newX , newY , 0.0 };
        String[] datatipLabel = DatatipCreate.setDatatipLabel (newPosition);
        datatipBounds = DatatipCreate.getDatatipBounds (datatipLabel);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, datatipBounds);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_TEXT_STRINGS__, datatipLabel);
        Double[] datatipPosition = DatatipCreate.setDatatipPosition (newPosition);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_POSITION__, datatipPosition);
        GraphicController.getController().setProperty(markerUid, GraphicObjectProperties.__GO_POSITION__, datatipPosition);
    }

}
