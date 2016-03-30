/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.gui.editor;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.CallRenderer;

import org.scilab.modules.gui.editor.ObjectSearcher;
import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.graphic_objects.axes.Axes;

/**
 * Implements all polyline manipulation functions for the editor.
 *
 * @author Caio Souza <caioc2bolado@gmail.com>
 * @author Pedro Souza <bygrandao@gmail.com>
 *
 * @since 2012-06-01
 */

public class PolylineHandler {

    static PolylineHandler instance = null;

    public PolylineHandler() {
    }

    public static PolylineHandler getInstance() {
        if (instance == null) {
            instance = new PolylineHandler();
        }
        return instance;
    }

    /**
     * Deletes all child entities from the given object.
     *
     * @param uid Object unique identifier.
     */
    public void deleteAll(Integer uid) {

        Integer type = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_TYPE__);
        Integer childCount = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN_COUNT__);
        Integer[] child = (Integer[])GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN__);
        if (type == GraphicObjectProperties.__GO_AXES__) {
            for (Integer i = 0; i < childCount; ++i) {
                Integer cType = (Integer)GraphicController.getController().getProperty(child[i], GraphicObjectProperties.__GO_TYPE__);
                if (cType != GraphicObjectProperties.__GO_LABEL__) {
                    CommonHandler.delete(child[i]);
                } else {
                    String[] text = { "" };
                    GraphicController.getController().setProperty(child[i], GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
                }
            }
        } else {

            for (Integer i = 0; i < childCount; ++i) {
                deleteAll(child[i]);
            }
        }

    }

    private Integer[] searchCompound(Integer uid) {
        return (new ObjectSearcher()).search(uid, GraphicObjectProperties.__GO_COMPOUND__);
    }

    /**
     * Drag the given polyline, add the difference of the click position to the polyline data
     *
     * @param polyline The uid of the polyline
     * @param position The click position
     * @param nextPosition The drag position
     */
    public void dragPolyline(Integer polyline, Integer[] position, Integer[] nextPosition) {

        Integer axes = (Integer) GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_PARENT_AXES__);
        if (polyline != null && axes != null) {
            double[] datax = (double[])PolylineData.getDataX(polyline);
            double[] datay = (double[])PolylineData.getDataY(polyline);
            double[] dataz = (double[])PolylineData.getDataZ(polyline);

            //Current point
            Vector3d planePoint = new Vector3d(datax[0], datay[0], dataz[0]);
            Vector3d planeNorm = new Vector3d(0.0, 0.0, 1.0);
            //2d coords for current click
            double[] pos = {nextPosition[0].doubleValue(), nextPosition[1].doubleValue(), 1.0};
            double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);
            //2d position for last click
            double[] pos2 = {position[0].doubleValue(), position[1].doubleValue(), 1.0};
            double[] c2d2 = CallRenderer.get2dViewFromPixelCoordinates(axes, pos2);

            Axes axesObj = AxesHandler.getAxesFromUid(axes);
            //3d ray for the current click
            double[] c3d1 = AxesDrawer.compute3dViewCoordinates(axesObj, c2d);
            c2d[2] += 1.0;
            double[] c3d2 = AxesDrawer.compute3dViewCoordinates(axesObj, c2d);
            //3d ray for last click
            double[] c3d3 = AxesDrawer.compute3dViewCoordinates(axesObj, c2d2);
            c2d2[2] += 1.0;
            double[] c3d4 = AxesDrawer.compute3dViewCoordinates(axesObj, c2d2);

            //3d current click point and dir
            Vector3d v0 = new Vector3d(c3d1);
            Vector3d v1 = new Vector3d(c3d2);
            Vector3d vdir = v1.minus(v0);
            //3d old click point and dir
            Vector3d w0 = new Vector3d(c3d3);
            Vector3d w1 = new Vector3d(c3d4);
            Vector3d wdir = v1.minus(v0);

            //if the dir is parallel to the plane there isn't intersection
            if (wdir.getZ() == 0 || vdir.getZ() == 0) {
                return;
            }
            //General plane intersection
            double v = (planePoint.getZ() - v0.getZ()) / vdir.getZ();
            Vector3d pointNew = vdir.times(v);
            pointNew = pointNew.plus(v0);

            double w = (planePoint.getZ() - w0.getZ()) / wdir.getZ();
            Vector3d pointOld = wdir.times(w);
            pointOld = pointOld.plus(w0);

            PolylineData.translatePolyline(polyline, pointNew.getX() - pointOld.getX(), pointNew.getY() - pointOld.getY(), 0.0, 0, 0, 0);
            GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_DATA_MODEL__, polyline);
        }
    }
}
