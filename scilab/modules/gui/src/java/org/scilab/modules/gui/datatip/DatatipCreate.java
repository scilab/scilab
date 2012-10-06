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
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.graphic_objects.PolylineData;

import java.lang.String;
import java.lang.Math;
import java.text.DecimalFormat;

import org.scilab.modules.gui.editor.AxesHandler;
import org.scilab.modules.gui.editor.EntityPicker;
import org.scilab.modules.gui.datatip.DatatipDrag;

/**
 * Create a datatip and set its properties
 * @author Gustavo Barbosa Libotte
 */
public class DatatipCreate {

    static EntityPicker ep = new EntityPicker();



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
        String polylineUid = ep.pick (figureUid, coordIntX, coordIntY);
        String axesUid = datatipAxesHandler(figureUid, pixelMouseCoordInt);
        double[] pixelMouseCoordDouble = transformPixelCoordToDouble(pixelMouseCoordInt);
        double[] graphicCoord = transformPixelCoordToGraphic(axesUid, pixelMouseCoordDouble);
        String newDatatip = datatipProperties (graphicCoord, polylineUid);
        return newDatatip;
    }

    /**
    * Given a double graphic coordinate point x, y
    * create a datatip.
    *
    * @param polylineUid Polyline unique identifier.
    * @param coordDoubleXY double array with graphic position x and y.
    * @return Datatip handler string.
    */
    public static String createDatatipProgramCoord(String polylineUid, double[] coordDoubleXY) {

        if (polylineUid != null) {
            String polylineInterp = null;
            polylineInterp = (String) GraphicController.getController().getProperty(polylineUid, GraphicObjectProperties.__GO_TAG__);

            DatatipCommon.Segment seg = DatatipCommon.getSegment(coordDoubleXY[0], polylineUid);

            double[] position = null;

            if (polylineInterp.equals("d_i")) {
                Double[] pos = DatatipCommon.Interpolate(coordDoubleXY[0], seg);
                position = new double[] {pos[0], pos[1], 0.0};
            } else {
                position = new double[] {seg.x0, seg.y0, 0.0};
            }

            String newDatatip = datatipProperties (position, polylineUid);
            return newDatatip;

        }

        return null;
    }


    /**
    * Given an integer index that belongs to the polyline create a datatip.
    *
    * @param polylineUid Polyline unique identifier.
    * @param indexPoint integer related to one of the indexed points of the polyline
    * @return Datatip handler string.
    */
    public static String createDatatipProgramIndex(String polylineUid, int indexPoint) {

        double[] DataX = (double[]) PolylineData.getDataX(polylineUid);
        double[] DataY = (double[]) PolylineData.getDataY(polylineUid);
        if (indexPoint > DataX.length) {
            indexPoint = DataX.length;
        } else if (indexPoint <= 0) {
            indexPoint = 1;
        }
        double[] coordDoubleXY = new double[2];
        coordDoubleXY[0] = DataX[indexPoint - 1];
        coordDoubleXY[1] = DataY[indexPoint - 1];
        String newDatatip = datatipProperties(coordDoubleXY, polylineUid);
        return newDatatip;
    }

    /**
    * Get integer datatip position on a specific axes
    *
    * @param coordinates Datatip coordinates x, y in double precision
    * @param axesUid Axes unique identifier
    * @return integer datatip position
    */
    public static Integer[] getDatatipPositionInteger (double[] coordinates, String axesUid) {

        double[] graphCoordDouble = new double[] {0.0, 0.0, 0.0};
        graphCoordDouble[0] = coordinates[0];
        graphCoordDouble[1] = coordinates[1];
        graphCoordDouble[2] = 0.0;

        double[] pixelCoordinates = new double[] {0.0, 0.0};
        pixelCoordinates = CallRenderer.getPixelFrom2dViewCoordinates(axesUid, graphCoordDouble);

        int xInt = (int) pixelCoordinates[0];
        int yInt = (int) pixelCoordinates[1];

        Integer[] coordInteger = new Integer[2];
        coordInteger[0] = (Integer) xInt;
        coordInteger[1] = (Integer) yInt;

        return coordInteger;
    }

    /**
    * Get the axes handler of the figure which mouse click belongs
    *
    * @param figureUid Figure unique identifier.
    * @param pixelMouseCoordInt Vector with pixel mouse position x and y.
    * @return Axes handler string.
    */
    public static String datatipAxesHandler(String figureUid, Integer[] pixelMouseCoordInt) {

        return AxesHandler.clickedAxes(figureUid, pixelMouseCoordInt);
    }

    /**
    * Transform integer pixel coordinate to double precision
    *
    * @param pixelMouseCoordInt Vector with pixel mouse position x and y.
    * @return Pixel coordinates in double precision.
    */
    public static double[] transformPixelCoordToDouble(Integer[] pixelMouseCoordInt) {

        double[] pixelMouseCoordDouble = new double[3];
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
        return CallRenderer.get2dViewFromPixelCoordinates(axesUid, pixelMouseCoordDouble);
    }

    /**
    * Ask to create an object in graphic window
    *
    * @return Datatip object handle
    */
    public static String askToCreateObject() {

        String newDatatip = GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_DATATIP__));
        return newDatatip;
    }


    /**
    * Set the position which datatip will be shown in graphic
    *
    * @param graphicCoord Vector containing the graphic coordinates in double precision.
    * @return Graphic coordinates x, y that datatip will be shown.
    */
    public static Double[] setDatatipPosition (double[] graphicCoord) {

        Double[] datatipPosition = new Double[3];
        datatipPosition[0] = graphicCoord[0];
        datatipPosition[1] = graphicCoord[1];
        datatipPosition[2] = 0.0;
        return datatipPosition;
    }

    /**
    * Set all default datatip properties;
    *
    * @param graphicCoord double array with graphic position x and y.
    * @param polyline the polyline handler string.
    * @return Datatip handler string.
    */
    private static String datatipProperties (double[] graphicCoord, String polyline) {

        String newDatatip = askToCreateObject();
        Double[] datatipPosition = setDatatipPosition(graphicCoord);

        GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_DATATIP_DATA__, datatipPosition);
        GraphicController.getController().setGraphicObjectRelationship(polyline, newDatatip);
        return newDatatip;
    }

    /**
     * Set interpolation mode of a polyline;
     *
     * @param polylineUid Polyline handler string.
     * @param interpMode Boolean to control interpolation mode
     */
    public static void datatipSetInterp (String polylineUid, boolean interpMode) {

        if (interpMode) {
            String interpString = "d_i";
            GraphicController.getController().setProperty(polylineUid, GraphicObjectProperties.__GO_TAG__, interpString);
        } else {
            String interpString = "";
            GraphicController.getController().setProperty(polylineUid, GraphicObjectProperties.__GO_TAG__, interpString);
        }
    }

}
