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
import org.scilab.modules.graphic_objects.graphicObject.*;

import org.scilab.modules.gui.editor.AxesHandler;

import java.util.ArrayList;

import org.scilab.modules.gui.datatip.DatatipCreate;

/**
 * Select a datatip
 * @author Gustavo Barbosa Libotte
 */
public class DatatipSelect {

    public static Double[] axesSizeGraph = { 0.0 , 0.0 };
    public static Double[] axesOrigin = { 0.0 , 0.0 };
    public static Double[] datatipPosAxes = { 0.0 , 0.0 };
    public static double[] deltaDatatip = { 0.0 , 0.0 };
    public static double[] deltaClick = { 0.0 , 0.0 };
    public static Double[] clickPosAxes = { 0.0 , 0.0 };
    public static boolean zoomEnabled = false;
    public static Integer datatip_index = 0;

    /**
    * Select a datatip when click on it
    *
    * @param figureUid Figure unique identifier.
    * @param axesUid Axes unique identifier.
    * @param clickPosition Vector with pixel mouse position x and y.
    * @param datatipsCoord Arraylist containing all created datatip's user coordinates.
    * @param datatipsUid Arraylist containing all created datatip's unique identifier.
    * @param markersUid Arraylist containing all created merker's unique identifier.
    * @return Integer index of selected datatip.
    */
    public static Integer selectDatatip(String figureUid, String axesUid, Integer[] clickPosition, ArrayList<Double> datatipsCoord, ArrayList<String> datatipsUid, ArrayList<String> markersUid) {

        double[] clickPositionDouble = DatatipCreate.transformPixelCoordToDouble(clickPosition);
        double[] graphicPosition = DatatipCreate.transformPixelCoordToGraphic(axesUid, clickPositionDouble);

        Double[] axesDataBounds = (Double[])GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_DATA_BOUNDS__);
        Double[] axesZoomBox = (Double[])GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_ZOOM_BOX__);
        zoomEnabled = AxesHandler.isZoomBoxEnabled(axesUid);
        
        Integer[] axesDimension = (Integer[])GraphicController.getController().getProperty(figureUid, GraphicObjectProperties.__GO_AXES_SIZE__);
        Double[] axesDimensionDouble = {1.0 * axesDimension[0], 1.0 * axesDimension[1]};

        axesSizeGraph = getAxesSize (axesDataBounds, axesZoomBox, zoomEnabled);
        axesOrigin = getAxesOriginPoint (axesDataBounds, axesZoomBox, zoomEnabled);

        clickPosAxes = getAxesClickPosition(graphicPosition, axesOrigin, axesSizeGraph, axesDimensionDouble);
    
        for (int i = 0 ; i < datatipsCoord.size() ; i = i + 2) {

            datatipPosAxes = getAxesDatatipPosition(i, datatipsCoord, axesOrigin, axesSizeGraph, axesDimensionDouble);

            if (clickPosAxes[0] >= datatipPosAxes[0] & clickPosAxes[0] <= (datatipPosAxes[0] + 74)) {
                if (clickPosAxes[1] >= datatipPosAxes[1] & clickPosAxes[1] <= (datatipPosAxes[1] + 30)) {
                    datatip_index = i;
                    return datatip_index;
                }
            }
        }
        return null;
    }

    /**
    * Calculate current axes size
    *
    * @param axesDataBounds Bounds of current axes.
    * @param axesZoomBox Bounds of current axes when zoom is enabled.
    * @param zoomEnabled Boolean which checks if zoom is enabled in current axes.
    * @return Currrent axes length.
    */
    public static Double[] getAxesSize (Double[] axesDataBounds, Double[] axesZoomBox, boolean zoomEnabled) {

        if (!zoomEnabled) {
            axesSizeGraph[0] = axesDataBounds[1] - axesDataBounds[0];
            axesSizeGraph[1] = axesDataBounds[3] - axesDataBounds[2];
        } else {
            axesSizeGraph[0] = axesZoomBox[1] - axesZoomBox[0];
            axesSizeGraph[1] = axesZoomBox[3] - axesZoomBox[2];
        }
        return axesSizeGraph;
    }

    /**
    * Get current axes origin point
    *
    * @param axesDataBounds Bounds of current axes.
    * @param axesZoomBox Bounds of current axes when zoom is enabled.
    * @param zoomEnabled Boolean which checks if zoom is enabled in current axes.
    * @return Origin point for x, y in relation to axes length.
    */
    public static Double[] getAxesOriginPoint (Double[] axesDataBounds, Double[] axesZoomBox, boolean zoomEnabled) {

        if (!zoomEnabled) {
            axesOrigin[0] = axesDataBounds[0];
            axesOrigin[1] = axesDataBounds[2];
        } else {
            axesOrigin[0] = axesZoomBox[0];
            axesOrigin[1] = axesZoomBox[2];
        }
        return axesOrigin;
    }

    /**
    * Calculate click position in relation to axes dimension
    *
    * @param graphicPosition Graphic coordinates related to inserted pixel coordinates.
    * @param axesOrigin Origin point for x, y in relation to axes length.
    * @param axesSizeGraph Currrent axes length.
    * @param axesDimensionDouble Axes size of current figure in double precision.
    * @return Click position in relation to current axes length.
    */
    public static Double[] getAxesClickPosition(double[] graphicPosition, Double[] axesOrigin, Double[] axesSizeGraph, Double[] axesDimensionDouble) {
    
        deltaClick[0] = ( (graphicPosition[0] - axesOrigin[0]) * 100) / axesSizeGraph[0];
        clickPosAxes[0] = (deltaClick[0] / 100) * axesDimensionDouble[0];
        deltaClick[1] = ( (graphicPosition[1] - axesOrigin[1]) * 100) / axesSizeGraph[1];
        clickPosAxes[1] = (deltaClick[1] / 100) * axesDimensionDouble[1];
        return clickPosAxes;
    }

    /**
    * Calculate datatip position in relation to axes dimension
    *
    * @param index Position to be verified
    * @param datatipsCoord Arraylist containing all created datatip's user coordinates.
    * @param axesOrigin Origin point for x, y in relation to axes length.
    * @param axesSizeGraph Currrent axes length.
    * @param axesDimensionDouble Axes size of current figure in double precision.
    * @return Datatip position in relation to current axes length.
    */
    public static Double[] getAxesDatatipPosition(int index, ArrayList<Double> datatipsCoord, Double[] axesOrigin, Double[] axesSizeGraph, Double[] axesDimensionDouble) {
        
        deltaDatatip[0] = ( (datatipsCoord.get(index) - axesOrigin[0]) * 100) / axesSizeGraph[0];
        datatipPosAxes[0] = (deltaDatatip[0] / 100) * axesDimensionDouble[0];
        deltaDatatip[1] = ( (datatipsCoord.get(index+1) - axesOrigin[1]) * 100) / axesSizeGraph[1];
        datatipPosAxes[1] = (deltaDatatip[1] / 100) * axesDimensionDouble[1];
        return datatipPosAxes;
    }
}
