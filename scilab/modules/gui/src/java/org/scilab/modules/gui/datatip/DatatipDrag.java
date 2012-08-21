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

import org.scilab.modules.gui.editor.EntityPicker;

import org.scilab.modules.renderer.CallRenderer;

import org.scilab.modules.gui.datatip.DatatipCreate;
import org.scilab.modules.gui.datatip.DatatipMove;

/**
 * Drag a datatip along a polyline
 * @author Gustavo Barbosa Libotte
 */
public class DatatipDrag {

    public static Double[] datatipPosition = new Double[3];
    static EntityPicker ep = new EntityPicker();
    public static String polylineUidTest = null;
    public static String polylineUidInit = null;
    public static String polylineUidEnd = null;
    public static int middleLineFactor = 0;
    public static double[] graphicCoord = new double[3];
    public static double[] newCoord = new double[3];
    public static Integer[] coordInteger = new Integer[2];
    public static String datatipUid;
    public static Integer newX;
    public static Integer newY;

    /**
    * Drag a datatip over a polyline using mouse
    *
    * @param markerUid Datatip marker unique identifier.
    * @param endX Integer with pixel mouse position x after mouse drag.
    * @param endY Integer with pixel mouse position y after mouse drag.
    */
    public static void dragDatatip (String markerUid, Integer endX, Integer endY) {

        String axesUid = (String) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_PARENT__);
        String figureUid = (String) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_PARENT__);
        String[] axesChildrenUid = (String[]) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_CHILDREN__);
        coordInteger = DatatipMove.getCoordInteger (markerUid, axesUid);
        polylineUidInit = ep.pick(figureUid, coordInteger[0], coordInteger[1]);
        Boolean isLineModeOn = (Boolean) GraphicController.getController().getProperty(polylineUidInit, GraphicObjectProperties.__GO_LINE_MODE__);
        Integer[] axesDimension = (Integer[])GraphicController.getController().getProperty(figureUid, GraphicObjectProperties.__GO_AXES_SIZE__);
        for (int yVerify = 0 ; yVerify < axesDimension[1] ; yVerify++) {
            polylineUidEnd = ep.pick (figureUid, endX, yVerify);
            if (polylineUidEnd == polylineUidInit) {
                newX = endX;
                newY = yVerify;
                newY = getMiddleLine (figureUid, newX, newY, yVerify);
                if (isLineModeOn) {
                    Integer[] pixelMouseCoordInt = { newX , newY };
                    double[] pixelMouseCoordDouble = DatatipCreate.transformPixelCoordToDouble(pixelMouseCoordInt);
                    double[] graphicCoord = DatatipCreate.transformPixelCoordToGraphic (axesUid, pixelMouseCoordDouble);
                    setNewPosition (graphicCoord, markerUid, axesUid, polylineUidInit);
                } else {
                    Integer[] pixelMouseCoordInt = { newX , newY };
                    double[] pixelMouseCoordDouble = DatatipCreate.transformPixelCoordToDouble(pixelMouseCoordInt);
                    double[] graphicCoord = DatatipCreate.transformPixelCoordToGraphic(axesUid, pixelMouseCoordDouble);
                    double[] polylineDataX = (double[]) PolylineData.getDataX (polylineUidEnd);
                    double[] polylineDataY = (double[]) PolylineData.getDataY (polylineUidEnd);
                    for (int i = 1 ; i < (polylineDataX.length - 1) ; i++) {
                        if (graphicCoord[0] > polylineDataX[i - 1] & graphicCoord[0] < polylineDataX[i + 1]) {
                            newCoord[0] = polylineDataX[i];
                            newCoord[1] = polylineDataY[i];
                            setNewPosition (newCoord, markerUid, axesUid, polylineUidInit);
                        }
                    }
                }
            }
        }
    }

    /**
    * Set the best y pixel position for a x pixel when it's over a polyline.
    *
    * @param figureUid Figure unique identifier.
    * @param newX Integer with new pixel mouse position x after drag when find polyline.
    * @param newY Integer with new pixel mouse position y after drag when find polyline.
    * @param yVerify Pixel coordinate y to be verified.
    * @return Integer with new y position in pixels
    */
    public static Integer getMiddleLine (String figureUid, Integer newX, Integer newY, int yVerify) {

        middleLineFactor = 0;
        int testPixel = newY;
        polylineUidTest = ep.pick (figureUid, newX, testPixel);
        while (polylineUidTest == polylineUidEnd) {
            middleLineFactor++;
            testPixel--;
            polylineUidTest = ep.pick (figureUid, newX, testPixel);
        }
        if (middleLineFactor % 2 == 0) {
            newY = yVerify - (middleLineFactor / 2);
        } else {
            middleLineFactor++;
            newY = yVerify - (middleLineFactor / 2);
        }
        return newY;
    }

    /**
    * Set the new position after the datatip is dragged.
    *
    * @param graphicCoord the new calculated position of the datatip
    * @param datatipsUid Arraylist containing all created datatip's unique identifier.
    * @param markersUid Arraylist containing all created merker's unique identifier.
    * @param indexToMove Integer referring to the selected datatip.
    * @return New position of the datatip to be saved
    */
    private static void setNewPosition (double[] graphicCoord, String markerUid, String axesUid, String polylineUid) {

        String[] axesChildrenUid = (String[]) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_CHILDREN__);
        for (int i = 0 ; i < axesChildrenUid.length ; i++) {
            if (axesChildrenUid[i] == markerUid) {
                datatipUid = axesChildrenUid[i + 1];
                break;
            }
        }
        Double[] datatipPosition = (Double[]) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_POSITION__);
        DatatipMove.updateDatatipsField (polylineUid, datatipPosition[0], datatipPosition[1], graphicCoord[0], graphicCoord[1]);
        datatipPosition = DatatipCreate.setDatatipPosition(graphicCoord);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_POSITION__, datatipPosition);
        GraphicController.getController().setProperty(markerUid, GraphicObjectProperties.__GO_POSITION__, datatipPosition);
        String[] datatipLabel = DatatipCreate.setDatatipLabel(graphicCoord);
        Integer[] datatipBounds = DatatipCreate.getDatatipBounds(datatipLabel);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, datatipBounds);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_TEXT_STRINGS__, datatipLabel);
    }
}
