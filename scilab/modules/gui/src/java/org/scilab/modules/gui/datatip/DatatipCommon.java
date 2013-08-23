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
import org.scilab.modules.gui.editor.CommonHandler;

import org.scilab.modules.gui.datatip.OrthogonalProjection;

import java.lang.Math;

/** Common datatip functions*/
public class DatatipCommon {

    static int i, j;

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
    }

    /*
     * Given a polyline and a position x in X axis return
     * the segment from the polyline that x belongs
     */
    public static Segment getSegment(double x, String polyline) {
        return getSegment(x, polyline, 0);
    }


    /*
     * Given a polyline and a position (x , y) in 2d view
     * return the segment from the polyline that this point belongs
     */
    public static Segment getSegment3dView(double x, double y, String polyline) {

        double[][] polylineData = getPolylineDataMatrix (polyline);

        Double[][] polylineDataBackup = backupPolylineData(polylineData);

        String axesUid = (String)GraphicController.getController().getProperty(polyline, __GO_PARENT_AXES__);
        double[][] geom3dCoords = getGeom3dCoords (axesUid, polylineData);
        polylineData = geom3dCoords;

        double[][] tempPoints = new double[polylineData.length][2];
        for (i = 0 ; i < tempPoints.length ; i++) {
            tempPoints[i][0] = x;
            tempPoints[i][1] = y;
        }

        double[][] dataDiff = new double[tempPoints.length][2];
        for (i = 0 ; i < dataDiff.length ; i++) {
            dataDiff[i][0] = Math.pow((polylineData[i][0] - tempPoints[i][0]), 2);
            dataDiff[i][1] = Math.pow((polylineData[i][1] - tempPoints[i][1]), 2);
        }

        double[] sumDataDiff = new double[dataDiff.length];
        for (i = 0 ; i < sumDataDiff.length ; i++) {
            sumDataDiff[i] = dataDiff[i][0] + dataDiff[i][1];
        }

        int indMin = 0;
        double minElem = sumDataDiff[0];
        for (i = 1 ; i < sumDataDiff.length ; i++) {
            if (sumDataDiff[i] < minElem) {
                minElem = sumDataDiff[i];
                indMin = i;
            }
        }

        Double[] pointCalc = new Double[] {polylineDataBackup[indMin][0], polylineDataBackup[indMin][1], polylineDataBackup[indMin][2]};

        if (indMin < (polylineDataBackup.length - 1)) {

            Double[] diffPointsXY = new Double[3];
            if (indMin < polylineDataBackup.length) {
                diffPointsXY[0] = polylineDataBackup[indMin + 1][0] - pointCalc[0];
                diffPointsXY[1] = polylineDataBackup[indMin + 1][1] - pointCalc[1];
                diffPointsXY[2] = polylineDataBackup[indMin + 1][2] - pointCalc[2];
            } else {
                diffPointsXY[0] = pointCalc[0] - polylineDataBackup[indMin - 1][0];
                diffPointsXY[1] = pointCalc[1] - polylineDataBackup[indMin - 1][1];
                diffPointsXY[2] = pointCalc[2] - polylineDataBackup[indMin - 1][2];
            }

            double norm_diffPointsXY = Math.sqrt((Math.pow(diffPointsXY[0], 2)) + (Math.pow(diffPointsXY[1], 2)) + (Math.pow(diffPointsXY[2], 2)));

            for (i = 0; i < diffPointsXY.length ; i++) {
                diffPointsXY[i] = (diffPointsXY[i] / norm_diffPointsXY) / Math.pow(10, 4);
            }

            Double[] datatipNewPos = new Double[3];
            datatipNewPos[0] = pointCalc[0] + diffPointsXY[0];
            datatipNewPos[1] = pointCalc[1] + diffPointsXY[1];
            datatipNewPos[2] = pointCalc[2] + diffPointsXY[2];

            return new Segment(0, datatipNewPos[0], 0.0, datatipNewPos[1], 0.0, datatipNewPos[2], 0.0);

        } else {

            return new Segment(0, pointCalc[0], 0.0, pointCalc[1], 0.0, pointCalc[2], 0.0);

        }
    }


    /*
     * Given a polyline and a position x in X axis return
     * the segment+offset from the polyline that x belongs
     */
    public static Segment getSegment(double x, String polyline, int offset) {

        double dataX[] = (double[])PolylineData.getDataX(polyline);
        double dataY[] = (double[])PolylineData.getDataY(polyline);
        int index = -1;

        String axes = (String)GraphicController.getController().getProperty(polyline, __GO_PARENT_AXES__);
        boolean[] logFlags = new boolean[] {(Boolean)GraphicController.getController().getProperty(axes, __GO_X_AXIS_LOG_FLAG__),
                                            (Boolean)GraphicController.getController().getProperty(axes, __GO_Y_AXIS_LOG_FLAG__)
                                           };

        dataX = toLogScale(dataX, logFlags[0]);
        dataY = toLogScale(dataY, logFlags[1]);
        if (dataX != null) {
            for (int i = 0; i < dataX.length - 1; ++i) {
                double min = Math.min(dataX[i], dataX[i + 1]);
                double max = Math.max(dataX[i], dataX[i + 1]);

                // using "if (x >= min && x < max)" seems to have no difference
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
            //check upper bound
            index = (index + offset + 1) < dataX.length ? (index + offset) : dataX.length - 2;
            //check lower bound
            index = (index + offset) >= 0 ? index : 0;
            return new Segment(index, dataX[index], dataX[index + 1], dataY[index], dataY[index + 1], 0.0, 0.0);
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
     * Return the interpolated position (x, y, z)
     * that the segment (x, y) belongs in the polyline
     */
    public static Double[] Interpolate3dView(double x, double y, Segment seg, String polyline) {

        double[][] polylineData = getPolylineDataMatrix (polyline);

        Double[][] polylineDataBackup = backupPolylineData(polylineData);

        String axesUid = (String)GraphicController.getController().getProperty(polyline, __GO_PARENT_AXES__);
        double[][] geom3dCoords = getGeom3dCoords (axesUid, polylineData);
        polylineData = geom3dCoords;

        double[] coefProj = OrthogonalProjection.orthogonalProj(polylineData, new double[] {x, y});

        int ind = (int) coefProj[0];
        double coef = coefProj[1];

        Double[] datatipNewPos = new Double[3];

        datatipNewPos[0] = polylineDataBackup[ind][0] + ((polylineDataBackup[ind + 1][0] - polylineDataBackup[ind][0]) * coef);
        datatipNewPos[1] = polylineDataBackup[ind][1] + ((polylineDataBackup[ind + 1][1] - polylineDataBackup[ind][1]) * coef);
        datatipNewPos[2] = polylineDataBackup[ind][2] + ((polylineDataBackup[ind + 1][2] - polylineDataBackup[ind][2]) * coef);

        return datatipNewPos;

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
        double[] position = new double[] {1.0 * pos[0], 1.0 * pos[1], 0.0};
        position = CallRenderer.get2dViewFromPixelCoordinates(axes, position);
        boolean[] logFlags = new boolean[] {(Boolean)GraphicController.getController().getProperty(axes, __GO_X_AXIS_LOG_FLAG__),
                                            (Boolean)GraphicController.getController().getProperty(axes, __GO_Y_AXIS_LOG_FLAG__),
                                            (Boolean)GraphicController.getController().getProperty(axes, __GO_Z_AXIS_LOG_FLAG__)
                                           };

        for (int i = 0; i < logFlags.length; i++) {
            position[i] = CommonHandler.InverseLogScale(position[i], logFlags[i]);
        }
        return position;
    }

    /*
     * Given a pixel coordinate return the transformed axis coordinate, in the view scale
     * (don't transfor it back if log scale is used)
     */
    public static double[] getTransformedPositionInViewScale(String figure, Integer[] pos) {

        String axes = AxesHandler.clickedAxes(figure, pos);
        double[] position = {1.0 * pos[0], 1.0 * pos[1], 0.0};
        return CallRenderer.get2dViewFromPixelCoordinates(axes, position);
    }

    static double[] toLogScale(double[] data, boolean logScale) {

        if (logScale) {
            double[] temp = new double[data.length];
            for (int i = 0; i < data.length; i++) {
                temp[i] = Math.log10(data[i]);
            }
            return temp;
        }
        return data;
    }

    static double[] toInverseLogScale(double[] data, boolean logScale) {

        if (logScale) {
            double[] temp = new double[data.length];
            for (int i = 0; i < data.length; i++) {
                temp[i] = Math.pow(10., data[i]);
            }
            return temp;
        }
        return data;
    }

    private static double[][] getPolylineDataMatrix (String polyline) {
        double[] DataX = (double[]) PolylineData.getDataX(polyline);
        double[] DataY = (double[]) PolylineData.getDataY(polyline);
        double[] DataZ = (double[]) PolylineData.getDataZ(polyline);

        double[][] polylineDataMatrix = new double[DataX.length][3];

        for (i = 0 ; i < polylineDataMatrix.length ; i++) {
            for (j = 0 ; j < polylineDataMatrix[0].length ; j++) {
                if (j == 0) {
                    polylineDataMatrix[i][j] = DataX[i];
                } else if (j == 1) {
                    polylineDataMatrix[i][j] = DataY[i];
                } else {
                    polylineDataMatrix[i][j] = DataZ[i];
                }
            }
        }

        return polylineDataMatrix;
    }

    private static Double[][] backupPolylineData(double[][] polylineData) {
        Double[][] polylineBackup = new Double[polylineData.length][3];
        for (i = 0 ; i < polylineData.length ; i++) {
            for (j = 0 ; j < polylineData[0].length ; j++) {
                polylineBackup[i][j] = (Double) polylineData[i][j];
            }
        }

        return polylineBackup;
    }

    private static double[][] getGeom3dCoords (String axesUid, double[][] polylineData) {
        double[] tempCoords = new double[3];
        double[][] geom3d = new double[polylineData.length][3];

        for (i = 0 ; i < polylineData.length ; i++) {
            for (j = 0 ; j < polylineData[0].length ; j++) {
                tempCoords[j] = polylineData[i][j];
            }
            double[] view2dCoords = CallRenderer.get2dViewCoordinates(axesUid, tempCoords);
            geom3d[i][0] = view2dCoords[0];
            geom3d[i][1] = view2dCoords[1];
            geom3d[i][2] = view2dCoords[2];
        }

        return geom3d;
    }
}
