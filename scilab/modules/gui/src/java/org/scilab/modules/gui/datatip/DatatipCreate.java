/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
import org.scilab.modules.graphic_objects.datatip.Datatip;
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
        EntityPicker.PickedPoint picked = ep.pickPoint(polylineUid, coordIntX, coordIntY);
        if (picked.isSegment) {
            double[] info = EntityPicker.getSegment(figureUid, polylineUid, picked.point, coordIntX, coordIntY);
            return datatipProperties(info, polylineUid);
        } else {
            return datatipProperties(new double[] {picked.point, 0.}, polylineUid);
        }
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
            double[] info;
            if (PolylineData.isZCoordSet(polylineUid) == 0) {
                info = EntityPicker.getNearestSegmentIndex(polylineUid, coordDoubleXY[0], coordDoubleXY[1]);
            } else {
                info = EntityPicker.getNearestSegmentIndex(polylineUid, coordDoubleXY[0], coordDoubleXY[1], coordDoubleXY[2]);
            }
            return datatipProperties(info, polylineUid);
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

        Integer newDatatip = datatipProperties(new double[] {indexPoint - 1, 0}, polylineUid);
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
    private static int datatipProperties(double[] coord, Integer polyline) {
        GraphicController controller = GraphicController.getController();
        Integer newDatatip = controller.askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_DATATIP__));
        Double[] indexes = null;
        Integer axesUid = (Integer) controller.getProperty(polyline, GraphicObjectProperties.__GO_PARENT_AXES__);
        Integer viewInfo = (Integer) controller.getProperty(axesUid, __GO_VIEW__);
        Integer fg = (Integer)controller.getProperty(polyline, GraphicObjectProperties.__GO_LINE_COLOR__);
        Integer bg = (Integer)controller.getProperty(polyline, GraphicObjectProperties.__GO_BACKGROUND__);
        Integer style = (Integer)controller.getProperty(polyline, GraphicObjectProperties.__GO_DATATIP_MARK__);

        //do not set relationship, only set parent to be able to go up in hierarchy
        controller.setProperty(newDatatip, GraphicObjectProperties.__GO_PARENT__, polyline);

        //set dataIndex and ratio from polyline
        controller.setProperty(newDatatip, GraphicObjectProperties.__GO_DATATIP_INDEXES__, new Double[] {coord[0], coord[1]});

        //set colors
        controller.setProperty(newDatatip, GraphicObjectProperties.__GO_MARK_FOREGROUND__, fg);
        controller.setProperty(newDatatip, GraphicObjectProperties.__GO_MARK_BACKGROUND__, bg);

        //set style
        controller.setProperty(newDatatip, GraphicObjectProperties.__GO_MARK_STYLE__, style);

        //get current polyline datatips
        Integer[] tips = (Integer[]) controller.getProperty(polyline, GraphicObjectProperties.__GO_DATATIPS__);

        //insert new tip in children array
        List<Integer> l = new LinkedList<Integer>(Arrays.asList(tips));
        l.add(0, newDatatip);

        //set new datatips arry in polyline
        Integer[] var = new Integer[l.size()];
        l.toArray(var);
        controller.setProperty(polyline, GraphicObjectProperties.__GO_DATATIPS__, var);

        if (viewInfo == 1 && PolylineData.isZCoordSet(polyline) == 1) {
            controller.setProperty(newDatatip, GraphicObjectProperties.__GO_DATATIP_DISPLAY_COMPONENTS__, "xyz");
            controller.setProperty(newDatatip, GraphicObjectProperties.__GO_CLIP_STATE__, 0);
        }

        if (DatatipManagerMode.getInstance() != null) {
            DatatipManagerMode.getInstance().setLastTip(axesUid, newDatatip);
        }

        return newDatatip;
    }

    /**
     * Set the datatip interpolation mode.
     *
     * @param datatipUid datatip unique identifier.
     * @param interpMode boolean for the interpolation mode.
     */
    public static void datatipSetInterp(int uid, boolean interpMode) {
        GraphicController controller = GraphicController.getController();
        Object o = controller.getObjectFromId(uid);
        if (o instanceof Datatip) {
            controller.setProperty(uid, GraphicObjectProperties.__GO_DATATIP_INTERP_MODE__, interpMode);
        } else {
            Integer[] tips = (Integer[]) controller.getProperty(uid, GraphicObjectProperties.__GO_DATATIPS__);
            Boolean b = new Boolean(interpMode);
            for (int i = 0; i < tips.length; i++) {
                controller.setProperty(tips[i], GraphicObjectProperties.__GO_DATATIP_INTERP_MODE__, b);
            }
        }
    }
}
