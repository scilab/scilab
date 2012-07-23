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

import java.util.ArrayList;

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
    public static Integer newX;
    public static Integer newY;

    /**
    * Given a mouse coordinate point x, y in pixels
    * create a datatip.
    *
    * @param figureUid Figure unique identifier.
    * @param endX Integer with pixel mouse position x after mouse drag.
    * @param endY Integer with pixel mouse position y after mouse drag.
    * @param polylineUidInit String containing the polyline unique identifier of the initial click.
    * @param datatipsUid Arraylist containing all created datatip's unique identifier.
    * @param markersUid Arraylist containing all created merker's unique identifier.
    * @param indexToMove Integer referring to the selected datatip.
    * @param datatipsCoord Arraylist containing all created datatip's user coordinates.
    * @param polylinesUid ArrayList containing all polylines unique identifiers.
    * @return Arraylist containing all created datatip's user coordinates updated with new position
    */
    public static ArrayList<Double> dragDatatip (String figureUid, Integer endX, Integer endY, String polylineUidInit, ArrayList<String> datatipsUid, ArrayList<String> markersUid, Integer indexToMove, ArrayList<Double> datatipsCoord, ArrayList<String> polylinesUid) {

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
                    String axesUid = DatatipCreate.datatipAxesHandler(figureUid, pixelMouseCoordInt);
                    double[] pixelMouseCoordDouble = DatatipCreate.transformPixelCoordToDouble(pixelMouseCoordInt);
                    double[] graphicCoord = DatatipCreate.transformPixelCoordToGraphic (axesUid, pixelMouseCoordDouble);
                    graphicCoord = setNewPosition (graphicCoord, datatipsUid, markersUid, indexToMove);
                    datatipsCoord.set (indexToMove, graphicCoord[0]);
                    datatipsCoord.set (indexToMove + 1, graphicCoord[1]);
                } else {
                    Integer[] pixelMouseCoordInt = { newX , newY };
                    String axesUid = DatatipCreate.datatipAxesHandler(figureUid, pixelMouseCoordInt);
                    double[] pixelMouseCoordDouble = DatatipCreate.transformPixelCoordToDouble(pixelMouseCoordInt);
                    double[] graphicCoord = DatatipCreate.transformPixelCoordToGraphic(axesUid, pixelMouseCoordDouble);
                    double[] polylineDataX = DatatipMove.getPolylineDataX (indexToMove, polylinesUid);
                    double[] polylineDataY = DatatipMove.getPolylineDataY (indexToMove, polylinesUid);
                    for (int i = 1 ; i < (polylineDataX.length - 1) ; i++) {
                        if (graphicCoord[0] > polylineDataX[i - 1] & graphicCoord[0] < polylineDataX[i + 1]) {
                            newCoord[0] = polylineDataX[i];
                            newCoord[1] = polylineDataY[i];
                            newCoord = setNewPosition (newCoord, datatipsUid, markersUid, indexToMove);
                            datatipsCoord.set (indexToMove, newCoord[0]);
                            datatipsCoord.set (indexToMove + 1, newCoord[1]);
                        }
                    }
                }
            }
        }
        return datatipsCoord;
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
    private static Integer getMiddleLine (String figureUid, Integer newX, Integer newY, int yVerify) {

        middleLineFactor = 0;
        int testPixel = newY;
        polylineUidTest = ep.pick (figureUid, newX, testPixel);
        while (polylineUidTest == polylineUidEnd) {
            middleLineFactor++;
            testPixel--;
            polylineUidTest = ep.pick (figureUid, newX, testPixel);
        }
        if (middleLineFactor%2 == 0) {
            newY = yVerify - (middleLineFactor/2);
        } else {
            middleLineFactor++;
            newY = yVerify - (middleLineFactor/2);
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
    private static double[] setNewPosition (double[] graphicCoord, ArrayList<String> datatipsUid, ArrayList<String> markersUid, Integer indexToMove) {

        datatipPosition = DatatipCreate.setDatatipPosition(graphicCoord);
        String datatipid = datatipsUid.get (indexToMove/2);
        GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_POSITION__, datatipPosition);
        String markerid = markersUid.get (indexToMove/2);
        GraphicController.getController().setProperty(markerid, GraphicObjectProperties.__GO_POSITION__, datatipPosition);
        String[] datatipLabel = DatatipCreate.setDatatipLabel(graphicCoord);
        Integer[] datatipBounds = DatatipCreate.getDatatipBounds(datatipLabel);
        GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, datatipBounds);
        GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_TEXT_STRINGS__, datatipLabel);
        return graphicCoord;
    }

    /**
    * Get the initial click to drag
    *
    * @param figureUid Figure unique identifier.
    * @param indexToMove Integer referring to the selected datatip.
    * @param startX pixel coordinate x of the initial click. 
    * @param startY pixel coordinate y of the initial click. 
    * @return Polyline unique identifier 
    */
    public static String getInitialInfo (String figureUid, Integer indexToMove, Integer startX, Integer startY) {

        if (indexToMove != null) {
            polylineUidInit = ep.pick (figureUid, startX, startY);
            return polylineUidInit;
        }
        polylineUidInit = null;
        return polylineUidInit;
    }
   
}
