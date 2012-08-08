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

import org.scilab.modules.gui.editor.AxesHandler;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import java.util.ArrayList;

import org.scilab.modules.gui.datatip.DatatipCreate;

/**
 * Create a datatip marker and set its properties
 * @author Gustavo Barbosa Libotte
 */
public class MarkerCreate {

    private static Integer[] markerBounds = { 0 , 0 };
    public static Double[] markerPosition = { 0.0 , 0.0 , 0.0 };
    private static double[] pixelMouseCoordDouble = { 0.0 , 0.0 };

    /**
    * Given a mouse coordinate point x, y in pixels
    * create a datatip marker.
    *
    * @param figureUid Figure unique identifier.
    * @param coordIntX Integer with pixel mouse position x.
    * @param coordIntY Integer with pixel mouse position y.
    * @return Marker handler string.
    */
    public static String createMarker (String figureUid, Integer coordIntX, Integer coordIntY) {

        Integer[] pixelMouseCoordInt = { coordIntX , coordIntY };
        String axesUid = DatatipCreate.datatipAxesHandler (figureUid, pixelMouseCoordInt);
        double[] pixelMouseCoordDouble = DatatipCreate.transformPixelCoordToDouble (pixelMouseCoordInt);
        double[] graphicCoord = DatatipCreate.transformPixelCoordToGraphic (axesUid, pixelMouseCoordDouble);
        String markerUid = markerProperties (graphicCoord, axesUid);
        return markerUid;
    }

    /**
    * Set an empty datatip marker label
    *
    * @return String vector with datatip marker label.
    */
    private static String[] setMarkerLabel () {

        String markerLabelUp = "  ";
        String markerLabelDown = "  ";
        String[] markerLabel = { markerLabelUp , markerLabelDown };
        return markerLabel;
    }

    /**
    * Save all created markers unique identifiers into an ArrayList
    *
    * @param markersUid ArrayList containing all created markers unique identifiers
    * @param markerid marker unique identifier string
    * @return ArrayList containing all created markers unique identifiers updated
    */
    public static ArrayList<String> getAllMarkersUid (ArrayList<String> markersUid, String markerUid) {

        markersUid.add(markerUid);
        return markersUid;
    }

    /**
    * Set all default marker properties;
    *
    * @param graphicCoord double array with graphic position x and y.
    * @param axesUid Axes handler string.
    * @return Marker handler string.
    */
    public static String markerProperties (double[] graphicCoord, String axesUid) {

        String newMarker = DatatipCreate.askToCreateObject ();
        String[] markerLabel = setMarkerLabel ();
        markerBounds = DatatipCreate.getDatatipBounds (markerLabel);
        markerPosition = DatatipCreate.setDatatipPosition (graphicCoord);
        GraphicController.getController().setGraphicObjectRelationship(axesUid, newMarker);
        GraphicController.getController().setProperty(newMarker, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, markerBounds);
        GraphicController.getController().setProperty(newMarker, GraphicObjectProperties.__GO_TEXT_STRINGS__, markerLabel);
        GraphicController.getController().setProperty(newMarker, GraphicObjectProperties.__GO_BOX__, true);
        GraphicController.getController().setProperty(newMarker, GraphicObjectProperties.__GO_CLIP_STATE__, 1);
        GraphicController.getController().setProperty(newMarker, GraphicObjectProperties.__GO_POSITION__, markerPosition);
        GraphicController.getController().setProperty(newMarker, GraphicObjectProperties.__GO_LINE_MODE__, true);
        GraphicController.getController().setProperty(newMarker, GraphicObjectProperties.__GO_TEXT_BOX_MODE__, 1);
        GraphicController.getController().setProperty(newMarker, GraphicObjectProperties.__GO_FILL_MODE__, true);
        GraphicController.getController().setProperty(newMarker, GraphicObjectProperties.__GO_BACKGROUND__, 1);
        return newMarker;
    }
}
