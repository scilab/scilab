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

    private static Integer[] datatipBounds = new Integer[2];
    public static Double[] datatipPosition = new Double[3];
    private static double[] pixelMouseCoordDouble = new double[2];
    public static double[] coordDoubleXY = new double[] {0.0, 0.0};
    public static Integer[] coordInteger = new Integer[] {0, 0};
    public static double[] graphicCoord = new double[3];
    public static double[] datatipCoordinates = new double[2];
    static EntityPicker ep = new EntityPicker();
    public static String newDatatip = null;


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
        graphicCoord = transformPixelCoordToGraphic(axesUid, pixelMouseCoordDouble);
        insertPoint (polylineUid, graphicCoord[0], graphicCoord[1]);
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

        String compoundUid = (String) GraphicController.getController().getProperty(polylineUid, GraphicObjectProperties.__GO_PARENT__);
        String axesUid = (String) GraphicController.getController().getProperty(compoundUid, GraphicObjectProperties.__GO_PARENT__);
        String figureUid = (String) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_PARENT__);

        String polylineInterp = null;
        polylineInterp = (String) GraphicController.getController().getProperty(polylineUid, GraphicObjectProperties.__GO_TAG__);
        if (polylineInterp.equals("d_i")) {
            double[] DataX = (double[]) PolylineData.getDataX (polylineUid);
            double[] DataY = (double[]) PolylineData.getDataY (polylineUid);
            double[] distAllPoints = new double[DataX.length];
            for (int i = 0 ; i < DataX.length ; i++) {
                double distPoint = Math.sqrt(Math.pow((coordDoubleXY[0] - DataX[i]), 2.0) + Math.pow((coordDoubleXY[1] - DataY[i]), 2.0));
                distAllPoints[i] = distPoint;
            }
            double minValue = distAllPoints[0];
            int posMin = 0;
            for (int j = 1 ; j < distAllPoints.length ; j++) {
                if (distAllPoints[j] < minValue) {
                    minValue = distAllPoints[j];
                    posMin = j;
                }
            }
            String newDatatip = createDatatipProgramIndex(polylineUid, posMin + 1);
            return newDatatip;

        } else {

            int middleLineFactor = 0;
            coordInteger = getDatatipPositionInteger (coordDoubleXY, axesUid);
            int yVerify = 0;
            Integer[] axesDimension = new Integer[] {0, 0};
            axesDimension = (Integer[])GraphicController.getController().getProperty(figureUid, GraphicObjectProperties.__GO_AXES_SIZE__);

            String polylineUidEnd = null;
            for (yVerify = 0 ; yVerify < axesDimension[1] ; yVerify++) {
                polylineUidEnd = ep.pick (figureUid, coordInteger[0], yVerify);
                if (polylineUidEnd != null) {
                    if (polylineUidEnd.equals(polylineUid)) {
                        middleLineFactor++;
                    }
                }
            }

            if (middleLineFactor < 20) {

                for (yVerify = 0 ; yVerify < axesDimension[1] ; yVerify++) {
                    polylineUidEnd = ep.pick (figureUid, coordInteger[0], yVerify);
                    if (polylineUidEnd != null) {
                        if (polylineUidEnd.equals(polylineUid)) {
                            break;
                        }
                    }
                }

                Integer newX = 0;
                Integer newY = 0;
                newX = coordInteger[0];
                newY = yVerify + (middleLineFactor / 2);

                Integer[] pixelMouseCoordInt = new Integer[] {0, 0};
                pixelMouseCoordInt[0] = newX;
                pixelMouseCoordInt[1] = newY;

                pixelMouseCoordDouble = transformPixelCoordToDouble(pixelMouseCoordInt);

                graphicCoord = transformPixelCoordToGraphic (axesUid, pixelMouseCoordDouble);

                insertPoint (polylineUid, graphicCoord[0], graphicCoord[1]);
                newDatatip = datatipProperties (graphicCoord, polylineUid);
                return newDatatip;

            } else {
                insertPoint (polylineUid, coordDoubleXY[0], coordDoubleXY[1]);
                newDatatip = datatipProperties (coordDoubleXY, polylineUid);
                return newDatatip;

            }

        }

    }

    public static double[] createDatatipField (String polylineUid, double[] coordDoubleXY) {

        double[] fieldArray = new double[coordDoubleXY.length];

        String compoundUid = (String) GraphicController.getController().getProperty(polylineUid, GraphicObjectProperties.__GO_PARENT__);
        String axesUid = (String) GraphicController.getController().getProperty(compoundUid, GraphicObjectProperties.__GO_PARENT__);
        String figureUid = (String) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_PARENT__);

        String[] childrenUid = (String[]) GraphicController.getController().getProperty(polylineUid, GraphicObjectProperties.__GO_CHILDREN__);
        for (int i = 0 ; i < childrenUid.length ; i++) {
            String objType = (String) GraphicController.getController().getProperty(childrenUid[i], GraphicObjectProperties.__GO_TYPE__);
            if (objType == GraphicObjectProperties.__GO_DATATIP__) {
                GraphicController.getController().removeRelationShipAndDelete(childrenUid[i]);
            }
        }

        for (int i = 0 ; i < (coordDoubleXY.length / 2) ; i++) {
            datatipCoordinates[0] = coordDoubleXY[2 * i];
            datatipCoordinates[1] = coordDoubleXY[2 * i + 1];

            int middleLineFactor = 0;
            coordInteger = getDatatipPositionInteger (datatipCoordinates, axesUid);
            int yVerify = 0;
            Integer[] axesDimension = new Integer[] {0, 0};
            axesDimension = (Integer[])GraphicController.getController().getProperty(figureUid, GraphicObjectProperties.__GO_AXES_SIZE__);

            String polylineUidEnd = null;
            for (yVerify = 0 ; yVerify < axesDimension[1] ; yVerify++) {
                polylineUidEnd = ep.pick (figureUid, coordInteger[0], yVerify);
                if (polylineUidEnd != null) {
                    if (polylineUidEnd.equals(polylineUid)) {
                        middleLineFactor++;
                    }
                }
            }

            if (middleLineFactor < 20) {

                for (yVerify = 0 ; yVerify < axesDimension[1] ; yVerify++) {
                    polylineUidEnd = ep.pick (figureUid, coordInteger[0], yVerify);
                    if (polylineUidEnd != null) {
                        if (polylineUidEnd.equals(polylineUid)) {
                            break;
                        }
                    }
                }

                Integer newX = 0;
                Integer newY = 0;
                newX = coordInteger[0];
                newY = yVerify + (middleLineFactor / 2);

                Integer[] pixelMouseCoordInt = new Integer[] {0, 0};
                pixelMouseCoordInt[0] = newX;
                pixelMouseCoordInt[1] = newY;

                pixelMouseCoordDouble = transformPixelCoordToDouble(pixelMouseCoordInt);

                graphicCoord = transformPixelCoordToGraphic (axesUid, pixelMouseCoordDouble);

                newDatatip = datatipProperties (graphicCoord, polylineUid);
                fieldArray[i] = graphicCoord[0];
                fieldArray[i + (coordDoubleXY.length / 2)] = graphicCoord[1];

            } else {
                graphicCoord[0] = datatipCoordinates[0];
                graphicCoord[1] = datatipCoordinates[1];
                newDatatip = datatipProperties (graphicCoord, polylineUid);
                fieldArray[i] = datatipCoordinates[0];
                fieldArray[i + (coordDoubleXY.length / 2)] = datatipCoordinates[1];
            }
        }

        return fieldArray;
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
        coordDoubleXY[0] = DataX[indexPoint - 1];
        coordDoubleXY[1] = DataY[indexPoint - 1];
        insertPoint (polylineUid, coordDoubleXY[0], coordDoubleXY[1]);
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

        graphicCoord = CallRenderer.get2dViewFromPixelCoordinates(axesUid, pixelMouseCoordDouble);
        return graphicCoord;
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

        datatipPosition[0] = graphicCoord[0];
        datatipPosition[1] = graphicCoord[1];
        datatipPosition[2] = 0.0;
        return datatipPosition;
    }

    /**
    * Set all default datatip properties;
    *
    * @param graphicCoord double array with graphic position x and y.
    * @param axesUid Axes handler string.
    * @return Datatip handler string.
    */
    private static String datatipProperties (double[] graphicCoord, String axesUid) {

        String newDatatip = askToCreateObject();
        datatipPosition = setDatatipPosition(graphicCoord);

        GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_DATATIP_DATA__, datatipPosition);
        GraphicController.getController().setGraphicObjectRelationship(axesUid, newDatatip);
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

    /**
    * Insert the coordinates of a created datatip in the datatips field in the polyline
    *
    * @param polylineUid Polyline handler string.
    * @param coordX Coordinate x of the datatip.
    * @param coordY Coordinate y of the datatip.
    */
    private static void insertPoint (String polylineUid, double coordX, double coordY) {

        Double[] currentDatatips = (Double[]) GraphicController.getController().getProperty(polylineUid, GraphicObjectProperties.__GO_DATATIPS__);
        Integer numDatatips = (Integer) GraphicController.getController().getProperty(polylineUid, GraphicObjectProperties.__GO_DATATIPS_SIZE__);
        int allocArray = (2 * numDatatips) + 2;
        Double[] newDatatips = new Double[allocArray];
        int j = 0;
        for (int i = 0 ; i < newDatatips.length ; i++) {
            if (i == numDatatips) {
                newDatatips[i] = coordX;
            } else if (i == (newDatatips.length - 1)) {
                newDatatips[i] = coordY;
            } else {
                newDatatips[i] = currentDatatips[j];
                j++;
            }
        }
        GraphicController.getController().setProperty(polylineUid, GraphicObjectProperties.__GO_DATATIPS__, newDatatips);
    }
}
