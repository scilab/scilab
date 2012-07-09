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
import org.scilab.modules.graphic_objects.graphicObject.*;

import org.scilab.modules.renderer.CallRenderer;

import org.scilab.modules.gui.datatip.DatatipCreate;

/**
 * Create a datatip marker and set its properties
 * @author Gustavo Barbosa Libotte
 */
public class MarkerCreate {

    public static String axesUid;
    public static double[] pixelMouseCoordDouble = { 0.0 , 0.0 };
    public static double[] graphicCoord = { 0.0 , 0.0 };
    public static String newMarker;
    public static String[] markerLabel;
    public static Integer[] markerBounds = { 0 , 0 };
    public static Double[] markerPosition = { 0.0 , 0.0 , 0.0 };

    /**
    * Given a mouse coordinate point x, y in pixels
    * create a datatip marker.
    *
    * @param figureUid Figure unique identifier.
    * @param pixelMouseCoordInt Vector with pixel mouse position x and y.
    * @return Marker handler string.
    */
    public static String createMarker (String figureUid, Integer[] pixelMouseCoordInt) {

        axesUid = DatatipCreate.datatipAxesHandler (figureUid, pixelMouseCoordInt);
        pixelMouseCoordDouble = DatatipCreate.transformPixelCoordToDouble (pixelMouseCoordInt);
        graphicCoord = DatatipCreate.transformPixelCoordToGraphic (axesUid, pixelMouseCoordDouble);
        newMarker = DatatipCreate.askToCreateObject ();
        markerLabel = setMarkerLabel ();
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

    /**
    * Set an empty datatip marker label
    *
    * @return String vector with datatip marker label.
    */
    public static String[] setMarkerLabel ()
    {
        String markerLabelUp = "  ";
        String markerLabelDown = "  ";
        String[] markerLabel = { markerLabelUp , markerLabelDown };
        return markerLabel;
    }

}
