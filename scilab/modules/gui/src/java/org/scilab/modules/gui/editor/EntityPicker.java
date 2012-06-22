/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.editor;

import java.util.*;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.*;
import org.scilab.modules.graphic_objects.CallGraphicController;
import org.scilab.modules.renderer.CallRenderer;


import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.gui.editor.PolylineHandler;
import org.scilab.modules.gui.editor.ObjectSearcher;

import java.lang.Math;


/**
* Given a (x, y) window coord checks
* if it is closer or belongs to a polyline.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/


public class EntityPicker {

    private Double dy;

    /**
    * Picks a polyline at the given position.
    *
    * @param figureUid 	Figure uid to be check.
    * @param posX 		Position on x axis in pixels.
    * @param posY 		Position on y axis in pixels.
    * @return			Picked polyline uid or null if there isn't any polyline at the given position.
    */
    public String pick(String figureUid, Integer posX, Integer posY) {

        Integer[] position = {posX, posY};
        String axes = AxesHandler.clickedAxes(figureUid, position);
        if (axes == null) {
            return null;
        }

        double[] pos = {1.0 * posX, 1.0 * posY, 1.0};
        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);

        pos[1] += 15.0;
        double[] c2d2 = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);

        dy = c2d[1] - c2d2[1];

        /* Checks if the click is outside canvas drawable area*/
        if (AxesHandler.isZoomBoxEnabled(axes)) {
            if (!AxesHandler.isInZoomBoxBounds(axes, c2d[0], c2d[1])) {
                return null;
            }
        }

        String polylines[] = (new ObjectSearcher()).search(axes, GraphicObjectProperties.__GO_POLYLINE__);

        if (polylines != null) {
            for (int i = 0; i < polylines.length; ++i) {
                if (PolylineHandler.getInstance().isVisible(polylines[i])) {

                    if (PolylineHandler.getInstance().isLineEnabled(polylines[i])) {
                        if (isOverLine(polylines[i], c2d[0], c2d[1])) {
                            return polylines[i];
                        }
                    }
                    if (PolylineHandler.getInstance().isMarkEnabled(polylines[i])) {
                        if (isOverMark(polylines[i], c2d[0], c2d[1])) {
                            return polylines[i];
                        }
                    }
                }
            }
        }

        return null;
    }

    /**
    * Check algorithm linear interpolation for each pair of points.
    * @param uid	Polyline uid to be checked.
    * @param x		position on x axis in view coordinates.
    * @param y		position on y axis in view coordinates.
    * @return		true if x,y belongs or is closest to the polyline.
    */
    private boolean isOverLine(String uid, Double x, Double y) {

        double[] datax = (double[])PolylineData.getDataX(uid);
        double[] datay = (double[])PolylineData.getDataY(uid);
        int size = datax.length;


        for (Integer i = 0; i < (size - 1); ++i) {
            if (isInRange(datax[i], datax[i + 1], datay[i], datay[i + 1], x, y)) {
                return true;
            }
        }
        return false;
    }

    private boolean isInRange(Double x0, Double x1, Double y0, Double y1, Double x, Double y) {
		/* Fast bound check*/
        double m = (x1 + x0)/2;
        double dx = m - x0;

        Double ca = (y1 - y0) / (x1 - x0);

        Double yy = y0 + ca * (x - x0);

        if (y >= (yy - dy)) {
            if (y <= (yy + dy)) {
                return (Math.abs(m - x) <= Math.abs(dx));
            }
        }
        return false;
    }

   /**
    * Checks if the given point belongs the polyline mark.
    * @param uid	Polyline uid to be checked.
    * @param x		position on x axis in view coordinates.
    * @param y		position on y axis in view coordinates.
    * @return		True if x,y belongs to the polyline mark.
    */
    private boolean isOverMark(String uid, Double x, Double y) {

        double[] datax = (double[])PolylineData.getDataX(uid);
        double[] datay = (double[])PolylineData.getDataY(uid);
        Integer size = PolylineHandler.getInstance().getMarkSize(uid);
        Integer unit = PolylineHandler.getInstance().getMarkSizeUnit(uid);
        /*dy/15 = 1px unit = 0 -> point, unit = 1 -> tabulated*/
        double delta = (dy/15)*(size*(0.75 + 0.7*unit) + 1.0 + unit*4.0);
        
        for (int i = 0; i < datax.length; ++i) {
            if ((Math.abs(datax[i] - x) <= delta) && (Math.abs(datay[i] - y) <= delta)) {
                return true;
            }
        }
        return false;
    }

}

