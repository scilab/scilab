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

package org.scilab.modules.gui.datatip;


import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.gui.editor.AxesHandler;
import java.lang.Math;

/** Common datatip functions*/
public class DatatipCommon {

    public static class Segment {
        public int pointIndex;
        public double x0, y0, x1, y1;
        public Segment() {}
        public Segment(int index, double x0, double x1, double y0, double y1) {
            pointIndex = index;
            this.x0 = x0;
            this.x1 = x1;
            this.y0 = y0;
            this.y1 = y1;
        }
    }

    /*
     * Given a polyline and a position x in X axis return
     * the segment from the polyline that x belongs
     */
    public static Segment getSegment(double x, String polyline) {

        double dataX[] = (double[])PolylineData.getDataX(polyline);
        double dataY[] = (double[])PolylineData.getDataY(polyline);
        int index = -1;

        if (dataX != null) {
            for (int i = 0; i < dataX.length - 1; ++i) {
                double min = Math.min(dataX[i], dataX[i + 1]);
                double max = Math.max(dataX[i], dataX[i + 1]);

                if (x >= min && x <= max) {
                    index = i;
                    break;
                }
            }
            if (index == -1) {
                /* x out of polyline bounds, return the closest segment bound*/

                double min = Math.min(dataX[0], dataX[dataX.length - 1]);
                double max = Math.max(dataX[0], dataX[dataX.length - 1]);

                if (x < min) {
                    index = (min == dataX[0]) ? 0 : (dataX.length - 2);
                }
                if (x > max) {
                    index = (max == dataX[0]) ? 0 : (dataX.length - 2);
                }
            }
            return new Segment(index, dataX[index], dataX[index + 1], dataY[index], dataY[index + 1]);
        }
        return null;
    }

    /*
     * Given a segment and a position x, return the
     * interpolated (x, y) that belongs to the segment
     */
    public static Double[] Interpolate(double x, Segment seg) {
        double m = (seg.x1 + seg.x0) / 2;
        double dx = Math.abs(m - seg.x0);
        /* is in bounds?*/
        if (Math.abs(x - m) <= dx) {
            double xDiff = (seg.x1 - seg.x0);
            if (xDiff != 0.0) {
                double ca = (seg.y1 - seg.y0) / xDiff;
                double y = seg.y0 + ca * (x - seg.x0);
                return new Double[] {x, y, 0.0};
            } else {
                /*angular coeficent -> inf, return the first point*/
                return new Double[] {seg.x0, seg.y0, 0.0};
            }
        } else {
            if (seg.pointIndex == 0) {
                return new Double[] {seg.x0, seg.y0, 0.0};
            } else {
                return new Double[] {seg.x1, seg.y1, 0.0};
            }
        }
    }

    /*
     * Given a datatip return its parent polyline
     */
    public static String getParentPolyline(String datatip) {
        String parent = (String)GraphicController.getController().getProperty(datatip, __GO_PARENT__);
        Integer parentType = (Integer)GraphicController.getController().getProperty(parent, __GO_TYPE__);
        if (parentType.equals(__GO_POLYLINE__)) {
            return parent;
        } else {
            return null;
        }
    }

    /*
     * Given a pixel coordinate return the transformed axis coordinate
     */
    public static double[] getTransformedPosition(String figure, Integer[] pos) {

        String axes = AxesHandler.clickedAxes(figure, pos);
        double[] position = {1.0 * pos[0], 1.0 * pos[1], 0.0};
        return CallRenderer.get2dViewFromPixelCoordinates(axes, position);
    }
}
