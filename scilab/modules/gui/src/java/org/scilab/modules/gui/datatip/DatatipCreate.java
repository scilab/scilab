/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VIEW__;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.editor.CommonHandler;
import org.scilab.modules.gui.editor.EntityPicker;
import org.scilab.modules.renderer.CallRenderer;

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
    public static Integer createDatatip(int figureUid, Integer coordIntX, Integer coordIntY) {

        Integer polylineUid = ep.pick(figureUid, coordIntX, coordIntY);
        double[] graphicCoord = DatatipCommon.getTransformedPosition(figureUid, new Integer[] {coordIntX, coordIntY});
        EntityPicker.PickedPoint picked = ep.pickPoint(polylineUid, coordIntX, coordIntY);
        double[] point = CommonHandler.computeIntersection(polylineUid, picked.point, graphicCoord);
        Integer newDatatip = datatipProperties(point, polylineUid, picked.point);
        return newDatatip;
    }

    /**
    * Given a double graphic coordinate point x, y
    * create a datatip.
    *
    * @param polylineUid Polyline unique identifier.
    * @param coordDoubleXY double array with graphic position x and y.
    * @return Datatip.
    */
    public static int createDatatipProgramCoord(int polylineUid, double[] coordDoubleXY) {
        if (polylineUid != 0) {
            if (PolylineData.isZCoordSet(polylineUid) == 0) {
                DatatipCommon.Segment seg = DatatipCommon.getSegment(coordDoubleXY[0], polylineUid);
                Double[] pos = DatatipCommon.Interpolate(coordDoubleXY[0], seg);
                double dx = (coordDoubleXY[0] - seg.x0) / (seg.x1 - seg.x0);
                double dy = (coordDoubleXY[1] - seg.y0) / (seg.y1 - seg.y0);
                return datatipProperties(new double[] {pos[0], pos[1], 0.0, dx, dy}, polylineUid, seg.pointIndex);
            } else {
                DatatipCommon.Segment seg3d = DatatipCommon.getSegment3dView(coordDoubleXY[0], coordDoubleXY[1], polylineUid);
                Double[] pos = DatatipCommon.Interpolate3dViewProgCoord(coordDoubleXY[0], coordDoubleXY[1], seg3d, polylineUid);
                double dx = (coordDoubleXY[0] - seg3d.x0) / (seg3d.x1 - seg3d.x0);
                double dy = (coordDoubleXY[1] - seg3d.y0) / (seg3d.y1 - seg3d.y0);
                return datatipProperties(new double[] {pos[0], pos[1], pos[2], dx, dy}, polylineUid, seg3d.pointIndex);
            }
        }
        return 0;
    }


    /**
    * Given an integer index that belongs to the polyline create a datatip.
    *
    * @param polylineUid Polyline unique identifier.
    * @param indexPoint integer related to one of the indexed points of the polyline
    * @return Datatip handler string.
    */
    public static int createDatatipProgramIndex(int polylineUid, int indexPoint) {

        double[] DataX = (double[]) PolylineData.getDataX(polylineUid);
        double[] DataY = (double[]) PolylineData.getDataY(polylineUid);

        if (indexPoint > DataX.length) {
            indexPoint = DataX.length;
        } else if (indexPoint <= 0) {
            indexPoint = 1;
        }

        double[] coordDoubleXY = new double[] {0.0, 0.0, 0.0, 0.0, 0.0};
        if (PolylineData.isZCoordSet(polylineUid) == 1) {
            double[] DataZ = (double[]) PolylineData.getDataZ(polylineUid);
            coordDoubleXY[0] = DataX[indexPoint - 1];
            coordDoubleXY[1] = DataY[indexPoint - 1];
            coordDoubleXY[2] = DataZ[indexPoint - 1];
        } else {
            coordDoubleXY[0] = DataX[indexPoint - 1];
            coordDoubleXY[1] = DataY[indexPoint - 1];
        }

        Integer newDatatip = datatipProperties(coordDoubleXY, polylineUid, indexPoint - 1);
        return newDatatip;
    }

    /**
    * Get the datatip position in pixels on a specific axes
    *
    * @param coordinates Datatip coordinates x, y in double precision
    * @param axesUid Axes unique identifier
    * @return datatip position in pixels
    */
    public static int[] getDatatipPositionInteger(double[] coordinates, int axesUid) {

        double[] graphCoordDouble = new double[] {0.0, 0.0, 0.0};
        graphCoordDouble[0] = coordinates[0];
        graphCoordDouble[1] = coordinates[1];
        graphCoordDouble[2] = 0.0;

        double[] pixelCoordinates = new double[] {0.0, 0.0};
        pixelCoordinates = CallRenderer.getPixelFrom2dViewCoordinates(axesUid, graphCoordDouble);

        int xInt = (int) pixelCoordinates[0];
        int yInt = (int) pixelCoordinates[1];

        int[] coordInteger = new int[2];
        coordInteger[0] = xInt;
        coordInteger[1] = yInt;

        return coordInteger;
    }


    /**
    * Creates and setup the datatip.
    *
    * @param coord double array with graphic position x and y.
    * @param polyline the polyline uid string.
    * @return Datatip uid string.
    */
    private static int datatipProperties(double[] coord, Integer polyline, int dataIndex) {
        GraphicController controller = GraphicController.getController();

        Integer newDatatip = controller.askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_DATATIP__));

        //Double[] datatipPosition = new Double[] {coord[0], coord[1], coord[2]};
        //controller.setProperty(newDatatip, GraphicObjectProperties.__GO_DATATIP_DATA__, datatipPosition);

        Double[] indexes = null;
        Integer axesUid = (Integer)controller.getProperty(polyline, GraphicObjectProperties.__GO_PARENT_AXES__);
        Integer viewInfo = (Integer) controller.getProperty(axesUid, __GO_VIEW__);
        if (viewInfo == 1 && PolylineData.isZCoordSet(polyline) == 1) {
            controller.setProperty(newDatatip, GraphicObjectProperties.__GO_DATATIP_3COMPONENT__, true);
            controller.setProperty(newDatatip, GraphicObjectProperties.__GO_CLIP_STATE__, 0);
            indexes = new Double[] {new Double(dataIndex), new Double(dataIndex), coord[3], coord[4]};
        } else {
            indexes = new Double[] {new Double(dataIndex), coord[3], coord[4]};
        }

        //do not set relationship, only set parent to be able to go up in hierarchy
        controller.setProperty(newDatatip, GraphicObjectProperties.__GO_PARENT__, polyline);

        //set dataIndex and ratio from polyline
        controller.setProperty(newDatatip, GraphicObjectProperties.__GO_DATATIP_INDEXES__, indexes);

        //get current polyline datatips
        Integer[] tips = (Integer[])controller.getProperty(polyline, GraphicObjectProperties.__GO_DATATIPS__);

        //insert new tip in children array
        List<Integer> l = new LinkedList<Integer>(Arrays.asList(tips));
        l.add(0, newDatatip);

        //set new datatips arry in polyline
        Integer[] var = new Integer[l.size()];
        l.toArray(var);
        controller.setProperty(polyline, GraphicObjectProperties.__GO_DATATIPS__, var);

        return newDatatip;
    }

    /**
    * Set the datatip interpolation mode.
    *
    * @param datatipUid datatip unique identifier.
    * @param interpMode boolean for the interpolation mode.
    */
    private static void datatipSetInterp(int datatipUid, boolean interpMode) {
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_DATATIP_INTERP_MODE__, interpMode);
    }

}
