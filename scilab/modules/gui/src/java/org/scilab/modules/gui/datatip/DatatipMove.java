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
import java.util.ArrayList;

import org.scilab.modules.gui.datatip.DatatipCreate;

/**
 * Move a datatip along the curve
 * @author Gustavo Barbosa Libotte
 */
public class DatatipMove {

    private static Integer[] datatipBounds = new Integer[2];
    public static double[] coordsToMove = new double[2];
    static double rightX;
    static double leftX;
    static double rightY;
    static double leftY;

    /**
    * Move a datatip to the right
    *
    * @param figureUid Figure unique identifier.
    * @param polylinesUid ArrayList containing all polylines unique identifiers.
    * @param indexToMove Integer referring to the selected datatip.
    * @param datatipsCoord Arraylist containing all created datatip's user coordinates.
    * @param datatipsUid Arraylist containing all created datatip's unique identifier.
    * @param markersUid Arraylist containing all created merker's unique identifier.
    * @return Arraylist containing all created datatip's user coordinates updated with new position
    */
    public static ArrayList<Double> moveRight (String figureUid, ArrayList<String> polylinesUid, Integer indexToMove, ArrayList<Double> datatipsCoord, ArrayList<String> datatipsUid, ArrayList<String> markersUid) {

        double[] polylineDataX = getPolylineDataX (indexToMove, polylinesUid);
        double[] polylineDataY = getPolylineDataY (indexToMove, polylinesUid);
        coordsToMove = getCoordsToMove (indexToMove, datatipsCoord);

        for (int i = 1 ; i < (polylineDataX.length - 2) ; i++) {
            if (coordsToMove[0] >= polylineDataX[i] & coordsToMove[0] <= polylineDataX[i + 1]) {
                rightX = polylineDataX[i + 1];
                rightY = polylineDataY[i + 1];
            }
        }

        String datatipUid = datatipsUid.get (indexToMove / 2);
        String markerUid = markersUid.get (indexToMove / 2);

        double[] posRight = { rightX , rightY , 0.0 };

        String[] datatipLabel = DatatipCreate.setDatatipLabel (posRight);
        datatipBounds = DatatipCreate.getDatatipBounds (datatipLabel);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, datatipBounds);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_TEXT_STRINGS__, datatipLabel);

        Double[] datatipPositionRight = DatatipCreate.setDatatipPosition (posRight);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_POSITION__, datatipPositionRight);
        GraphicController.getController().setProperty(markerUid, GraphicObjectProperties.__GO_POSITION__, datatipPositionRight);

        datatipsCoord.set (indexToMove, rightX);
        datatipsCoord.set (indexToMove + 1, rightY);

        return datatipsCoord;
    }

    /**
    * Move a datatip to the left
    *
    * @param figureUid Figure unique identifier.
    * @param polylinesUid ArrayList containing all polylines unique identifiers.
    * @param indexToMove Integer referring to the selected datatip.
    * @param datatipsCoord Arraylist containing all created datatip's user coordinates.
    * @param datatipsUid Arraylist containing all created datatip's unique identifier.
    * @param markersUid Arraylist containing all created merker's unique identifier.
    * @return Arraylist containing all created datatip's user coordinates updated with new position
    */
    public static ArrayList<Double> moveLeft (String figureUid, ArrayList<String> polylinesUid, Integer indexToMove, ArrayList<Double> datatipsCoord, ArrayList<String> datatipsUid, ArrayList<String> markersUid) {

        double[] polylineDataX = getPolylineDataX (indexToMove, polylinesUid);
        double[] polylineDataY = getPolylineDataY (indexToMove, polylinesUid);
        coordsToMove = getCoordsToMove (indexToMove, datatipsCoord);

        for (int i = 2 ; i < (polylineDataX.length - 1) ; i++) {
            if (coordsToMove[0] >= polylineDataX[i] & coordsToMove[0] <= polylineDataX[i + 1]) {
                leftX = polylineDataX[i - 1];
                leftY = polylineDataY[i - 1];
            }
        }

        String datatipUid = datatipsUid.get (indexToMove / 2);
        String markerUid = markersUid.get (indexToMove / 2);

        double[] posLeft = { leftX , leftY , 0.0 };

        String[] datatipLabel = DatatipCreate.setDatatipLabel (posLeft);
        datatipBounds = DatatipCreate.getDatatipBounds (datatipLabel);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, datatipBounds);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_TEXT_STRINGS__, datatipLabel);

        Double[] datatipPositionLeft = DatatipCreate.setDatatipPosition (posLeft);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_POSITION__, datatipPositionLeft);
        GraphicController.getController().setProperty(markerUid, GraphicObjectProperties.__GO_POSITION__, datatipPositionLeft);

        datatipsCoord.set (indexToMove, leftX);
        datatipsCoord.set (indexToMove + 1, leftY);

        return datatipsCoord;
    }

    /**
    * Get all values of x in a specific polyline
    *
    * @param indexToMove Integer referring to the selected datatip.
    * @param polylinesUid ArrayList containing all polylines unique identifiers.
    * @return Array containing all values of x in double precision
    */
    public static double[] getPolylineDataX (Integer indexToMove, ArrayList<String> polylinesUid) {
        String polylineUid = polylinesUid.get (indexToMove / 2);
        double[] DataX = (double[]) PolylineData.getDataX (polylineUid);
        return DataX;
    }

    /**
    * Get all values of y in a specific polyline
    *
    * @param indexToMove Integer referring to the selected datatip.
    * @param polylinesUid ArrayList containing all polylines unique identifiers.
    * @return Array containing all values of y in double precision
    */
    public static double[] getPolylineDataY (Integer indexToMove, ArrayList<String> polylinesUid) {
        String polylineUid = polylinesUid.get (indexToMove / 2);
        double[] DataY = (double[]) PolylineData.getDataY (polylineUid);
        return DataY;
    }

    /**
    * Get x , y to be moved
    *
    * @param indexToMove Integer referring to the selected datatip.
    * @param datatipsCoord Arraylist containing all created datatip's user coordinates.
    * @return Array containing the values of x , y to be moved
    */
    private static double[] getCoordsToMove (Integer indexToMove, ArrayList<Double> datatipsCoord) {
        coordsToMove[0] = datatipsCoord.get (indexToMove);
        coordsToMove[1] = datatipsCoord.get (indexToMove + 1);
        return coordsToMove;
    }
}
