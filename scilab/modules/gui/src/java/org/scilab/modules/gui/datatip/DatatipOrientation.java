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

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.editor.AxesHandler;

import org.scilab.modules.renderer.CallRenderer;

import org.scilab.modules.gui.datatip.DatatipCreate;

/**
 * Orientate the datatip for "upper left", "upper right", "lower left" or "lower right" according to the quadrant position.
 * @author Gustavo Barbosa Libotte
 */
public class DatatipOrientation {

    public static Double[] datatipCorners = new Double[12];
    public static Double[] markerPosition = new Double[3];
    public static double[] newPosition = new double[3];
    public static Double[] newDatatipPosition = new Double[3];
    public static double xSize;
    public static double ySize;
    private static String datatipUid;

    /**
    * Set the orientation of the datatip accordind to its position
    *
    * @param markerUid datatip marker unique identifier
    */
    public static void setOrientation (String markerUid) {

        String axesUid = (String) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_PARENT__);
        String figureUid = (String) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_PARENT__);
        String[] axesChildrenUid = (String[]) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_CHILDREN__);

        for (int i = 0 ; i < axesChildrenUid.length ; i++) {
            if (axesChildrenUid[i] == markerUid) {
                datatipUid = axesChildrenUid[i+1];
                break;
            }
        }

        datatipCorners = (Double[]) GraphicController.getController().getProperty(datatipUid, GraphicObjectProperties.__GO_CORNERS__);
        markerPosition = (Double[]) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_POSITION__);

        xSize = datatipCorners[9] - datatipCorners[0];
        ySize = datatipCorners[4] - datatipCorners[1];

        Double[] axesDataBounds = (Double[])GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_DATA_BOUNDS__);
        Double[] axesZoomBox = (Double[])GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_ZOOM_BOX__);

        boolean zoomEnabled = AxesHandler.isZoomBoxEnabled(axesUid);

        autoOrientation (zoomEnabled, axesDataBounds, axesZoomBox, markerPosition, xSize, ySize, datatipUid);
    }

    /**
    * Get the datatip quadrant position and set the orientation
    *
    * @param zoomEnabled Boolean to check if zoom is enabled.
    * @param axesDataBounds Bounds of clicked axes when zoom is disable.
    * @param axesZoomBox Bounds of clicked axes when zoom is enable.
    * @param markerPosition Position of selected datatip.
    * @param xSize Length of x axis.
    * @param ySize Length of y axis.
    * @param datatipid selected datatip unique identifier.
    */
    public static void autoOrientation (boolean zoomEnabled, Double[] axesDataBounds, Double[] axesZoomBox, Double[] markerPosition, double xSize, double ySize, String datatipid) {

        if (!zoomEnabled) {
            Double middleAxesX = axesDataBounds[0] + ((axesDataBounds[1] - axesDataBounds[0]) / 2);
            Double middleAxesY = axesDataBounds[2] + ((axesDataBounds[3] - axesDataBounds[2]) / 2);
            if ( markerPosition[0] > middleAxesX & markerPosition[1] > middleAxesY ) {
                newPosition[0] = markerPosition[0] - xSize;
                newPosition[1] = markerPosition[1] - ySize;
                newPosition[2] = 0;
                newDatatipPosition = DatatipCreate.setDatatipPosition (newPosition);
                GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_POSITION__, newDatatipPosition);
            } else if ( markerPosition[0] <= middleAxesX & markerPosition[1] > middleAxesY ) {
                newPosition[0] = markerPosition[0];
                newPosition[1] = markerPosition[1] - ySize;
                newPosition[2] = 0;
                newDatatipPosition = DatatipCreate.setDatatipPosition (newPosition);
                GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_POSITION__, newDatatipPosition);
            } else if ( markerPosition[0] <= middleAxesX & markerPosition[1] <= middleAxesY ) {
                newPosition[0] = markerPosition[0];
                newPosition[1] = markerPosition[1];
                newPosition[2] = 0;
                newDatatipPosition = DatatipCreate.setDatatipPosition (newPosition);
                GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_POSITION__, newDatatipPosition);
            } else if ( markerPosition[0] > middleAxesX & markerPosition[1] <= middleAxesY ) {
                newPosition[0] = markerPosition[0] - xSize;
                newPosition[1] = markerPosition[1];
                newPosition[2] = 0;
                newDatatipPosition = DatatipCreate.setDatatipPosition (newPosition);
                GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_POSITION__, newDatatipPosition);
            }
        } else {
            Double middleAxesX = axesZoomBox[0] + ((axesZoomBox[1] - axesZoomBox[0]) / 2);
            Double middleAxesY = axesZoomBox[2] + ((axesZoomBox[3] - axesZoomBox[2]) / 2);
            if ( markerPosition[0] > middleAxesX & markerPosition[1] > middleAxesY ) {
                newPosition[0] = markerPosition[0] - xSize;
                newPosition[1] = markerPosition[1] - ySize;
                newPosition[2] = 0;
                newDatatipPosition = DatatipCreate.setDatatipPosition (newPosition);
                GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_POSITION__, newDatatipPosition);
            } else if ( markerPosition[0] <= middleAxesX & markerPosition[1] > middleAxesY ) {
                newPosition[0] = markerPosition[0];
                newPosition[1] = markerPosition[1] - ySize;
                newPosition[2] = 0;
                newDatatipPosition = DatatipCreate.setDatatipPosition (newPosition);
                GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_POSITION__, newDatatipPosition);
            } else if ( markerPosition[0] <= middleAxesX & markerPosition[1] <= middleAxesY ) {
                newPosition[0] = markerPosition[0];
                newPosition[1] = markerPosition[1];
                newPosition[2] = 0;
                newDatatipPosition = DatatipCreate.setDatatipPosition (newPosition);
                GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_POSITION__, newDatatipPosition);
            } else if ( markerPosition[0] > middleAxesX & markerPosition[1] <= middleAxesY ) {
                newPosition[0] = markerPosition[0] - xSize;
                newPosition[1] = markerPosition[1];
                newPosition[2] = 0;
                newDatatipPosition = DatatipCreate.setDatatipPosition (newPosition);
                GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_POSITION__, newDatatipPosition);
            }
        }
    }
}
