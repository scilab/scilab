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
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.graphic_objects.PolylineData;

import java.lang.String;
import java.lang.Math;
import java.text.DecimalFormat;

import org.scilab.modules.gui.editor.AxesHandler;
import org.scilab.modules.gui.editor.EntityPicker;
import org.scilab.modules.gui.editor.CommonHandler;
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

        String polylineUid = ep.pick(figureUid, coordIntX, coordIntY);
        double[] graphicCoord = DatatipCommon.getTransformedPosition(figureUid, new Integer[] {coordIntX, coordIntY});
        EntityPicker.PickedPoint picked = ep.pickPoint(polylineUid, coordIntX, coordIntY);
        double[] point = CommonHandler.computeIntersection(polylineUid, picked.point, graphicCoord);
        String newDatatip = datatipProperties(point, polylineUid);
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

            if (PolylineData.isZCoordSet(polylineUid) == 0) {

                DatatipCommon.Segment seg = DatatipCommon.getSegment(coordDoubleXY[0], polylineUid);


                Double[] pos = DatatipCommon.Interpolate(coordDoubleXY[0], seg);
                double[] position = new double[] {pos[0], pos[1], 0.0};

                String newDatatip = datatipProperties(position, polylineUid);
                return newDatatip;

            } else {

                DatatipCommon.Segment seg3d = DatatipCommon.getSegment3dView(coordDoubleXY[0], coordDoubleXY[1], polylineUid);

                Double[] pos = DatatipCommon.Interpolate3dViewProgCoord(coordDoubleXY[0], coordDoubleXY[1], seg3d, polylineUid);
                double[] position = new double[] {pos[0], pos[1], pos[2]};

                String newDatatip = datatipProperties(position, polylineUid);
                return newDatatip;

            }

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

        double[] coordDoubleXY = new double[3];
        if (PolylineData.isZCoordSet(polylineUid) == 1) {
            double[] DataZ = (double[]) PolylineData.getDataZ(polylineUid);
            coordDoubleXY[0] = DataX[indexPoint - 1];
            coordDoubleXY[1] = DataY[indexPoint - 1];
            coordDoubleXY[2] = DataZ[indexPoint - 1];
        } else {
            coordDoubleXY[0] = DataX[indexPoint - 1];
            coordDoubleXY[1] = DataY[indexPoint - 1];
        }

        String newDatatip = datatipProperties(coordDoubleXY, polylineUid);
        return newDatatip;
    }

    /**
    * Get the datatip position in pixels on a specific axes
    *
    * @param coordinates Datatip coordinates x, y in double precision
    * @param axesUid Axes unique identifier
    * @return datatip position in pixels
    */
    public static Integer[] getDatatipPositionInteger(double[] coordinates, String axesUid) {

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
    * Creates and setup the datatip.
    *
    * @param coord double array with graphic position x and y.
    * @param polyline the polyline uid string.
    * @return Datatip uid string.
    */
    private static String datatipProperties(double[] coord, String polyline) {

        String newDatatip = GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_DATATIP__));
        Double[] datatipPosition = new Double[] {coord[0], coord[1], coord[2]};

        GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_DATATIP_DATA__, datatipPosition);

        String axesUid = (String)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_PARENT_AXES__);
        Integer viewInfo = (Integer) GraphicController.getController().getProperty(axesUid, __GO_VIEW__);
        if (viewInfo == 1 && PolylineData.isZCoordSet(polyline) == 1) {
            GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_DATATIP_3COMPONENT__, true);
            GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_CLIP_STATE__, 0);
        }

        GraphicController.getController().setGraphicObjectRelationship(polyline, newDatatip);
        return newDatatip;
    }

    /**
    * Set the datatip interpolation mode.
    *
    * @param datatipUid datatip unique identifier.
    * @param interpMode boolean for the interpolation mode.
    */
    private static void datatipSetInterp(String datatipUid, boolean interpMode) {

        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_DATATIP_INTERP_MODE__, interpMode);

    }

}
