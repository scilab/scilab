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
    * Picks a polyline at the given position
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
        String polylines[] = (new ObjectSearcher()).search(axes, GraphicObjectProperties.__GO_POLYLINE__);


        double[] pos = new double[3];
        pos[0] = 1.0 * posX;
        pos[1] = 1.0 * posY;
        pos[2] = 1.0;
        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);

        pos[1] += 15.0;

        double[] c2d2 = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);

        dy = c2d[1] - c2d2[1];

        long Time = 0;
        if (polylines != null) {
            for (int i = 0; i < polylines.length; ++i) {
                if (PolylineHandler.getInstance().isVisible(polylines[i])) {
                    Time = System.currentTimeMillis();
                    if (isOverLine(polylines[i], c2d[0], c2d[1])) {
                        //if (binarySearch(polylines[i], c2d[0], c2d[1])) {
                        Time = System.currentTimeMillis() - Time;
                        System.out.println("You picked a entity! ->" + polylines[i] + "\nTime: " + Time);
                        return polylines[i];
                    } else {
                        Time = System.currentTimeMillis() - Time;
                        System.out.println("\nTime: " + Time);
                    }
                }
            }
        }

        //
        return null;
    }

    /**
    * Check algorithm linear interpolation for each pair of points
    * @param uid	Polyline uid to be checked
    * @param x		position on x axis in view coordinates
    * @param y		position on y axis in view coordinates
    * @return		true if x,y belongs or is closest to the polyline
    */
    private boolean isOverLine(String uid, Double x, Double y) {

        System.out.println("IsOverLine\nGetting data!");
        double[] datax = (double[])PolylineData.getDataX(uid);//DataController.getController().getProperty(uid, GraphicObjectProperties.__GO_DATA_MODEL_X__);
        double[] datay = (double[])PolylineData.getDataY(uid);//DataController.getController().getProperty(uid, GraphicObjectProperties.__GO_DATA_MODEL_Y__);
        int size = datax.length;


        for (Integer i = 0; i < (size - 1); ++i) {
            if (isInRange(datax[i], datax[i + 1], datay[i], datay[i + 1], x, y)) {
                return true;
            }
        }
        return false;
    }

    private boolean isInRange(Double x0, Double x1, Double y0, Double y1, Double x, Double y) {

        /*Double xmin, xmax, ymin, ymax;
        if (x0 < x1) {
        	xmin = x0; xmax = x1;
        }
        else {
        	xmin = x1; xmax = x0;
        }

        if (x < xmin || x > xmax) {
        	return false;
        }*/


        double dx = x1 - x0;
        double p1 = x - x0;
        double p2 = x1 - x;

        Double ca = (y1 - y0) / (x1 - x0);

        Double yy = y0 + ca * (x - x0);

        if (y >= (yy - dy)) {
            if (y <= (yy + dy)) {
                return (true && (Math.abs(p1) + Math.abs(p2) ) <= Math.abs(dx)) ;
            }
        }
        return false;
    }

    /**
    * Check algorithm using binary search and linear intepolation
    * @param uid	Polyline uid to be checked
    * @param x		position on x axis in view coordinates
    * @param y		position on y axis in view coordinates
    * @return		true if x,y belongs or is closest to the polyline
    */
    private boolean binarySearch(String uid, Double x, Double y) {

        System.out.println("BinarySearch");

        double[] datax = (double[])PolylineData.getDataX(uid);
        double[] datay = (double[])PolylineData.getDataY(uid);
        int size = datax.length;

        int begin = 0, end = size - 1;

        while ((end - begin) > 1) {
            int m = (int)((begin + end) / 2);
            if (x <= datax[m]) {
                end = m;
            } else {
                begin = m;
            }
        }

        if (begin == end) {
            return isOverLerp(datax[begin - 1], datax[end], datay[begin - 1], datay[end], x, y)
                   || isOverLerp(datax[begin], datax[end + 1], datay[begin], datay[end + 1], x, y);
        } else {
            return isOverLerp(datax[begin], datax[end], datay[begin], datay[end], x, y);
        }
    }

    private boolean isOverLerp(Double x0, Double x1, Double y0, Double y1, Double x, Double y) {

        Double ca = (y1 - y0) / (x1 - x0);

        Double yy = y0 + ca * (x - x0);

        if (y >= (yy - dy)) {
            if (y <= (yy + dy)) {
                return true;
            }
        }
        return false;
    }

}

