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

package org.scilab.modules.gui.datatip;

import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.gui.editor.AxesHandler;
import org.scilab.modules.gui.editor.CommonHandler;

/** Common datatip functions*/
public class DatatipCommon {

    public static class Segment {
        public int pointIndex;
        public double x0, y0, z0, x1, y1, z1;
        public Segment() {}
        public Segment(int index, double x0, double x1, double y0, double y1, double z0, double z1) {
            pointIndex = index;
            this.x0 = x0;
            this.x1 = x1;
            this.y0 = y0;
            this.y1 = y1;
            this.z0 = z0;
            this.z1 = z1;
        }

        public final double norm2() {
            return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + (z1 - z0) * (z1 - z0);
        }

        public final double norm() {
            return Math.sqrt(norm2());
        }
    }

    /*
     * Given a datatip return its parent polyline
     */
    public static Integer getParentPolyline(Integer datatip) {
        Integer parent = (Integer) GraphicController.getController().getProperty(datatip, __GO_PARENT__);
        Integer parentType = (Integer) GraphicController.getController().getProperty(parent, __GO_TYPE__);
        if (parentType.equals(__GO_POLYLINE__)) {
            return parent;
        } else {
            return null;
        }
    }

    /*
     * Given a pixel coordinate return the transformed axis coordinate
     */
    public static double[] getTransformedPosition(Integer figure, Integer[] pos) {
        Integer axes = AxesHandler.clickedAxes(figure, pos);
        double[] position = new double[] {pos[0].doubleValue(), pos[1].doubleValue(), 0.0};
        return CallRenderer.get2dViewFromPixelCoordinates(axes, position);
    }

    /*
     * Given a pixel coordinate return the transformed axis coordinate, in the view scale
     * (don't transfor it back if log scale is used)
     */
    public static double[] getTransformedPositionInViewScale(Integer figure, Integer[] pos) {
        Integer axes = AxesHandler.clickedAxes(figure, pos);
        double[] position = {pos[0].doubleValue(), pos[1].doubleValue(), 0.0};
        return CallRenderer.get2dViewFromPixelCoordinates(axes, position);
    }

    public static double[][] getPolylineDataMatrix(Integer polyline) {
        return getPolylineDataMatrix(polyline, false);
    }

    public static double[][] getPolylineDataMatrix(Integer polyline, final boolean two_d) {
        double[] DataX = (double[]) PolylineData.getDataX(polyline);
        double[] DataY = (double[]) PolylineData.getDataY(polyline);
        double[][] polylineDataMatrix;

        if (two_d) {
            polylineDataMatrix = new double[DataX.length][2];
            for (int i = 0 ; i < polylineDataMatrix.length ; i++) {
                polylineDataMatrix[i][0] = DataX[i];
                polylineDataMatrix[i][1] = DataY[i];
            }
        } else {
            double[] DataZ = (double[]) PolylineData.getDataZ(polyline);
            polylineDataMatrix = new double[DataX.length][3];
            for (int i = 0 ; i < polylineDataMatrix.length ; i++) {
                polylineDataMatrix[i][0] = DataX[i];
                polylineDataMatrix[i][1] = DataY[i];
                polylineDataMatrix[i][2] = DataZ[i];
            }
        }

        return polylineDataMatrix;
    }
}
