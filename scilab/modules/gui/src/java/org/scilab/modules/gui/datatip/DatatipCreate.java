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

import java.lang.String;
import java.text.DecimalFormat;
import java.util.ArrayList;

import org.scilab.modules.gui.editor.AxesHandler;

/**
 * Create a datatip and set its properties
 * @author Gustavo Barbosa Libotte
 */
public class DatatipCreate {

    private static Integer[] datatipBounds = new Integer[2];
    public static Double[] datatipPosition = new Double[3];
    private static double[] pixelMouseCoordDouble = new double[2];

    /**
    * Given a mouse coordinate point x, y in pixels
    * create a datatip.
    *
    * @param figureUid Figure unique identifier.
    * @param coordIntX Integer with pixel mouse position x.
    * @param coordIntY Integer with pixel mouse position y.
    * @return Datatip handler string.
    */
    public static String createDatatip(String figureUid, Integer coordIntX, Integer coordIntY) {

        Integer[] pixelMouseCoordInt = { coordIntX , coordIntY };
        String axesUid = datatipAxesHandler(figureUid, pixelMouseCoordInt);
        double[] pixelMouseCoordDouble = transformPixelCoordToDouble(pixelMouseCoordInt);
        double[] graphicCoord = transformPixelCoordToGraphic(axesUid, pixelMouseCoordDouble);
        String newDatatip = askToCreateObject();
        String[] datatipLabel = setDatatipLabel(graphicCoord);
        datatipBounds = getDatatipBounds(datatipLabel);
        datatipPosition = setDatatipPosition(graphicCoord);
        GraphicController.getController().setGraphicObjectRelationship(axesUid, newDatatip);
        GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, datatipBounds);
        GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_TEXT_STRINGS__, datatipLabel);
        GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_BOX__, true);
        GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_CLIP_STATE__, 1);
        GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_POSITION__, datatipPosition);
        GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_LINE_MODE__, true);
        GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_FILL_MODE__, true);
        GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_BACKGROUND__, 8);
        return newDatatip;
    }

    /**
    * Get the axes handler of the figure which mouse click belongs
    *
    * @param figureUid Figure unique identifier.
    * @param pixelMouseCoordInt Vector with pixel mouse position x and y.
    * @return Axes handler string.
    */
    public static String datatipAxesHandler(String figureUid, Integer[] pixelMouseCoordInt) {

        String axesUid = AxesHandler.clickedAxes(figureUid, pixelMouseCoordInt);
        return axesUid;
    }

    /**
    * Transform integer pixel coordinate to double precision
    *
    * @param pixelMouseCoordInt Vector with pixel mouse position x and y.
    * @return Pixel coordinates in double precision.
    */
    public static double[] transformPixelCoordToDouble(Integer[] pixelMouseCoordInt) {

        for (int i = 0 ; i < pixelMouseCoordInt.length ; i++) {
            pixelMouseCoordDouble[i] = (double) pixelMouseCoordInt[i];
        }
        return pixelMouseCoordDouble;
    }

    /**
    * Transform pixel screen coordinates in graphic coordinates
    *
    * @param axesUid Axes handler of clicked figure.
    * @param pixelMouseCoordDouble Mouse click pixel coordinates in double precision.
    * @return Graphic coordinates related to inserted pixel coordinates.
    */
    public static double[] transformPixelCoordToGraphic(String axesUid, double[] pixelMouseCoordDouble) {

        double[] graphicCoord = CallRenderer.get2dViewFromPixelCoordinates(axesUid, pixelMouseCoordDouble);
        return graphicCoord;
    }

    /**
    * Ask to create an object in graphic window
    *
    * @return Datatip object handle
    */
    public static String askToCreateObject() {

        String newDatatip = GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_TEXT__));
        return newDatatip;
    }

    /**
    * Set the datatip label according to graphic coordinates
    *
    * @param graphicCoord Vector containing the graphic coordinates in double precision.
    * @return String vector with datatip label.
    */
    public static String[] setDatatipLabel(double[] graphicCoord) {

        DecimalFormat numDecimal = new DecimalFormat("#.#####");
        String datatipLabelX = numDecimal.format(graphicCoord[0]);
        datatipLabelX = " X:" + datatipLabelX + "  ";
        String datatipLabelY = numDecimal.format(graphicCoord[1]);
        datatipLabelY = " Y:" + datatipLabelY + "  ";
        String[] datatipLabel = { datatipLabelX , datatipLabelY };
        return datatipLabel;
    }

    /**
    * Calculate datatip box bounds accordind to length of its label
    *
    * @param datatipLabel Vector containing the graphic coordinates in double precision.
    * @return String vector with datatip label.
    */
    public static Integer[] getDatatipBounds(String[] datatipLabel) {

        datatipBounds[0] = datatipLabel.length;
        datatipBounds[1] = 1;
        return datatipBounds;
    }

    /**
    * Set the position which datatip will be shown in graphic
    *
    * @param graphicCoord Vector containing the graphic coordinates in double precision.
    * @return Graphic coordinates x, y that datatip will be shown.
    */
    public static Double[] setDatatipPosition (double[] graphicCoord) {

        datatipPosition[0] = graphicCoord[0];
        datatipPosition[1] = graphicCoord[1];
        datatipPosition[2] = 0.0;
        return datatipPosition;
    }

    /**
    * Save all created datatips coordinates into an ArrayList
    *
    * @param datatipsCoord ArrayList containing all created datatips coordinates
    * @param datatipid datatip unique identifier to get its coordinates
    * @return ArrayList containing all created datatips coordinates updated
    */
    public static ArrayList<Double> getAllDatatipsCoord (ArrayList<Double> datatipsCoord, String markerUid) {

        Double[] graphicCoord = (Double[]) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_POSITION__);
        datatipsCoord.add(graphicCoord[0]);
        datatipsCoord.add(graphicCoord[1]);
        return datatipsCoord;
    }

    /**
    * Save all created datatips unique identifiers into an ArrayList
    *
    * @param datatipsUid ArrayList containing all created datatips unique identifiers
    * @param datatipid datatip unique identifier
    * @return ArrayList containing all created datatips unique identifiers updated
    */
    public static ArrayList<String> getAllDatatipsUid (ArrayList<String> datatipsUid, String datatipUid) {

        datatipsUid.add(datatipUid);
        return datatipsUid;
    }

    /**
    * Save all polylines unique identifiers on which datatips were created
    *
    * @param polylinesUid ArrayList containing all polylines unique identifiers
    * @param polylineid polyline unique identifier string
    * @return ArrayList containing all polylines unique identifiers updated
    */
    public static ArrayList<String> getAllPolylinesUid (ArrayList<String> polylinesUid, String uid) {

        polylinesUid.add(uid);
        return polylinesUid;
    }
}
