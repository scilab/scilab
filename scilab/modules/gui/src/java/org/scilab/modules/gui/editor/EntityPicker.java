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


import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.CallGraphicController;
import org.scilab.modules.renderer.CallRenderer;

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.graphic_objects.SurfaceData;
import org.scilab.modules.gui.editor.CommonHandler;
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
    private Double dx;
    private boolean needTransform = false;
    private Axes curAxes = null;
    private final Double selectionDelta = 7.0;

    /**
    * Picks a polyline at the given position.
    *
    * @param figureUid     Figure uid to be check.
    * @param posX         Position on x axis in pixels.
    * @param posY         Position on y axis in pixels.
    * @return            Picked polyline uid or null if there isn't any polyline at the given position.
    */
    public String pick(String figureUid, Integer posX, Integer posY) {

        Integer[] position = {posX, posY};
        String axes = AxesHandler.clickedAxes(figureUid, position);
        if (axes == null) {
            return null;
        }

        curAxes = AxesHandler.getAxesFromUid(axes);

        double[] pos = {1.0 * posX, 1.0 * posY, 1.0};
        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);

        pos[0] += selectionDelta;
        pos[1] += selectionDelta;
        double[] c2d2 = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);

        dx = Math.abs(c2d[0] - c2d2[0]);
        dy = Math.abs(c2d[1] - c2d2[1]);

        needTransform = !isInDefaultView(curAxes);

        /* Checks if the click is outside canvas drawable area*/
        if (AxesHandler.isZoomBoxEnabled(axes)) {
            if (!AxesHandler.isInZoomBoxBounds(axes, c2d[0], c2d[1])) {
                return null;
            }
        }

        String polylines[] = (new ObjectSearcher()).search(axes, GraphicObjectProperties.__GO_POLYLINE__);

        if (polylines != null) {
            for (int i = 0; i < polylines.length; ++i) {
                if (CommonHandler.isVisible(polylines[i])) {

                    if (CommonHandler.isLineEnabled(polylines[i])) {
                        if (isOverLine(polylines[i], c2d[0], c2d[1]) != -1) {
                            return polylines[i];
                        }
                    }
                    if (CommonHandler.isMarkEnabled(polylines[i])) {
                        if (isOverMark(polylines[i], c2d[0], c2d[1]) != -1) {
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
    * @param uid    Polyline uid to be checked.
    * @param x        position on x axis in view coordinates.
    * @param y        position on y axis in view coordinates.
    * @return        true if x,y belongs or is closest to the polyline.
    */
    private int isOverLine(String uid, Double x, Double y) {

        double[] datax = (double[])PolylineData.getDataX(uid);
        double[] datay = (double[])PolylineData.getDataY(uid);
        double[] dataz = (double[])PolylineData.getDataZ(uid);
        int size = datax.length;

        if (PolylineData.isXShiftSet(uid) != 0) {
            double[] x_shift = (double[])PolylineData.getShiftX(uid);
            for (int i = 0; i < size; ++i) {
                datax[i] += x_shift[i];
            }
        }

        if (PolylineData.isYShiftSet(uid) != 0) {
            double[] y_shift = (double[])PolylineData.getShiftY(uid);
            for (int i = 0; i < size; ++i) {
                datay[i] += y_shift[i];
            }
        }

        if (PolylineData.isZShiftSet(uid) != 0) {
            double[] z_shift = (double[])PolylineData.getShiftZ(uid);
            for (int i = 0; i < size; ++i) {
                dataz[i] += z_shift[i];
            }
        }
        datax = CommonHandler.toLogScale(datax, curAxes.getXAxisLogFlag());
        datay = CommonHandler.toLogScale(datay, curAxes.getYAxisLogFlag());
        dataz = CommonHandler.toLogScale(dataz, curAxes.getZAxisLogFlag());

        double[] oldPoint = null;
        if (needTransform) {
            oldPoint = transformPoint(curAxes, datax[0], datay[0], dataz[0]);
        }

        for (Integer i = 0; i < (size - 1); ++i) {
            if (needTransform) {
                double[] newPoint = transformPoint(curAxes, datax[i + 1], datay[i + 1], dataz[i + 1]);
                if (isInRange(oldPoint[0], newPoint[0], oldPoint[1], newPoint[1], x, y)) {
                    return i;
                }
                oldPoint = newPoint;
            } else {
                if (isInRange(datax[i], datax[i + 1], datay[i], datay[i + 1], x, y)) {
                    return i;
                }
            }
        }
        return -1;
    }

    private boolean isInRange(Double x0, Double x1, Double y0, Double y1, Double x, Double y) {
        /* Fast bound check*/
        double m = (x1 + x0) / 2;
        double dx = m - x0;

        double ca = (y1 - y0) / (x1 - x0);

        double yy = y0 + ca * (x - x0);

        double pix = this.dx / selectionDelta;
        double m_y = (y1 + y0) / 2;
        double dy = m_y - y0;

        boolean ca_inf = (Math.abs(x1 - x0) < Math.abs(pix * 2));
        boolean in_bounds = (Math.abs(m - x) <= Math.abs(pix * 2)) && (Math.abs(m_y - y) <= Math.abs(dy));

        /*
         * test if (x, y) belongs or is closer to the line
         * if the angular coeficent -> inf(ca_inf), the interpolation fails
         * then we use "in_bunds" test.
         */
        return (Math.abs(m - x) <= Math.abs(dx)) && (y >= (yy - this.dy)) && (y <= (yy + this.dy)) || (ca_inf && in_bounds);
    }

    /**
     * Checks if the given point belongs the polyline mark.
     * @param uid    Polyline uid to be checked.
     * @param x        position on x axis in view coordinates.
     * @param y        position on y axis in view coordinates.
     * @return        True if x,y belongs to the polyline mark.
     */
    private int isOverMark(String uid, Double x, Double y) {

        double[] datax = (double[])PolylineData.getDataX(uid);
        double[] datay = (double[])PolylineData.getDataY(uid);
        double[] dataz = (double[])PolylineData.getDataZ(uid);

        if (PolylineData.isXShiftSet(uid) != 0) {
            double[] x_shift = (double[])PolylineData.getShiftX(uid);
            for (int i = 0; i < datax.length; ++i) {
                datax[i] += x_shift[i];
            }
        }

        if (PolylineData.isYShiftSet(uid) != 0) {
            double[] y_shift = (double[])PolylineData.getShiftY(uid);
            for (int i = 0; i < datay.length; ++i) {
                datay[i] += y_shift[i];
            }
        }

        if (PolylineData.isZShiftSet(uid) != 0) {
            double[] z_shift = (double[])PolylineData.getShiftZ(uid);
            for (int i = 0; i < dataz.length; ++i) {
                dataz[i] += z_shift[i];
            }
        }

        datax = CommonHandler.toLogScale(datax, curAxes.getXAxisLogFlag());
        datay = CommonHandler.toLogScale(datay, curAxes.getYAxisLogFlag());
        dataz = CommonHandler.toLogScale(dataz, curAxes.getZAxisLogFlag());

        Integer size = CommonHandler.getMarkSize(uid);
        Integer unit = CommonHandler.getMarkSizeUnit(uid);

        int finalSize = (unit == 1) ? (8 + 2 * size) : size;
        finalSize /= 2;
        double deltax = Math.abs((dx / selectionDelta) * finalSize);
        double deltay = Math.abs((dy / selectionDelta) * finalSize);


        for (int i = 0; i < datax.length; ++i) {
            if (needTransform) {
                double[] point = transformPoint(curAxes, datax[i], datay[i], dataz[i]);
                if ((Math.abs(point[0] - x) <= deltax) && (Math.abs(point[1] - y) <= deltay)) {
                    return i;
                }
            } else {
                if ((Math.abs(datax[i] - x) <= deltax) && (Math.abs(datay[i] - y) <= deltay)) {
                    return i;
                }
            }
        }
        return -1;
    }


    public class PickedPoint {
        public int point;
        public boolean isSegment;
        PickedPoint(int p, boolean segment) {
            point = p;
            isSegment = segment;
        }
    }

    /**
     * Given a polyline uid checks if the given point (px,py)
     * belongs or is closer to any polyline point.
     *
     * @return The picked point or PickedPoint.point = -1 otherwise.
     */
    public PickedPoint pickPoint(String uid, int px, int py) {

        PickedPoint point = new PickedPoint(-1, false);
        Integer[] position = {px, py};
        String figUid = (new ObjectSearcher()).searchParent(uid, GraphicObjectProperties.__GO_FIGURE__);
        String axes = AxesHandler.clickedAxes(figUid, position);
        if (axes == null) {
            return point;
        }

        curAxes = AxesHandler.getAxesFromUid(axes);

        double[] pos = {1.0 * px, 1.0 * py, 1.0};
        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);

        pos[0] += selectionDelta;
        pos[1] += selectionDelta;
        double[] c2d2 = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);

        dx = Math.abs(c2d[0] - c2d2[0]);
        dy = Math.abs(c2d[1] - c2d2[1]);

        needTransform = !isInDefaultView(curAxes);

        /*try pick a point*/
        point.point = isOverMark(uid, c2d[0], c2d[1]);
        if (point.point != -1) {
            return point;
        } else {
            /*try pick a segment*/
            point.point = isOverLine(uid, c2d[0], c2d[1]);
            point.isSegment = true;
            return point;
        }
    }

    /**
     * Checks if the axes is in default view (2d view).
     *
     * @return true if is in default view, false otherwise.
     */
    private boolean isInDefaultView(Axes axes) {
        Double rot[] = axes.getRotationAngles();
        return (rot[0] == 0.0 && rot[1] == 270.0);
    }

    /**
     * Project the given point in view plane.
     *
     * @return the projected point.
     */
    private double[] transformPoint(Axes axes, double x, double y, double z) {
        double point[] = {x, y, z};
        return AxesDrawer.compute2dViewCoordinates(axes, point);
    }


    public class LegendInfo {
        public String legend = null;
        public String polyline = null;
        LegendInfo(String legend, String polyline) {
            this.legend = legend;
            this.polyline = polyline;
        }
    }

    /**
    * Check if the given position is over a legend object.
    *
    * @param axes The uid of axes that was clicked.
    * @param position The mouse position (x, y).
    * @return The LegendInfo if picked a legend null otherwise.
    */
    public LegendInfo pickLegend(String figure, Integer[] position) {

        String axes = AxesHandler.clickedAxes(figure, position);
        if (axes == null) {
            return null;
        }
        String legend = LegendHandler.searchLegend(axes);
        if (legend == null) {
            return null;
        }

        String[] links;
        Integer[] axesSize = {0, 0};
        Double delta;
        Double[] axesBounds = { 0., 0. }, dPosition = { 0., 0. }, legendPos = { 0., 0. }, legendBounds = { 0., 0., 0., 0. }, dimension = { 0., 0. };

        axesSize = (Integer[])GraphicController.getController().getProperty(figure, GraphicObjectProperties.__GO_AXES_SIZE__);
        axesBounds = (Double[])GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_AXES_BOUNDS__);
        legendPos = (Double[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_POSITION__);
        links = (String[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_LINKS__);
        dPosition[0] = (position[0] - (axesBounds[0] * axesSize[0])) / (axesBounds[2] * axesSize[0]);
        dPosition[1] = (position[1] - (axesBounds[1] * axesSize[1])) / (axesBounds[3] * axesSize[1]);
        dimension = (Double[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_SIZE__);
        legendBounds[0] =  legendPos[0];
        legendBounds[1] = legendPos[1];
        legendBounds[2] = legendPos[0] + dimension[0];
        legendBounds[3] = legendPos[1] + dimension[1];
        delta = dimension[1] / (1.0 * links.length);

        if (dPosition[0] >= legendBounds[0] && dPosition[0] <= legendBounds[2] && dPosition[1] >= legendBounds[1] && dPosition[1] <= legendBounds[3]) {
            for (Integer i = 0; i < links.length; i++) {
                if (dPosition[1] >= (legendBounds[1] + i * delta) && dPosition[1] <= (legendBounds[1] + (i + 1) * delta)) {
                    return new LegendInfo(legend, links[links.length - 1 - i]);
                }
            }
        }
        return null;
    }

    /**
     * Try pick an axis label at the given position.
     * @param figure The figure uid.
     * @param pos The position (x, y).
     * @return The picked axis label or null.
     */
    public static AxesHandler.axisTo pickLabel(String figure, Integer[] pos) {

        String axes = AxesHandler.clickedAxes(figure, pos);
        if (axes == null) {
            return null;
        }
        Double[] corners;
        Double radAngle = 0.;
        int rotate = 0;
        double[] point = new double[3];
        double[] coord;
        String[] label = {  (String)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_X_AXIS_LABEL__),
                            (String)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__),
                            (String)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__),
                            (String)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_TITLE__)
                         };

        boolean[] logScale = {  (Boolean)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_X_AXIS_LOG_FLAG__),
                                (Boolean)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Y_AXIS_LOG_FLAG__)
                             };

        for (Integer i = 0; i < 4; i++) {
            corners = (Double[])GraphicController.getController().getProperty(label[i], GraphicObjectProperties.__GO_CORNERS__);
            radAngle = (Double)GraphicController.getController().getProperty(label[i], GraphicObjectProperties.__GO_FONT_ANGLE__);
            rotate = ((int)((radAngle * 2) / Math.PI)) % 2;
            if (rotate == 1) {
                corners = rotateCorners(corners);
            }
            point[0] = pos[0];
            point[1] = pos[1];
            point[2] = 1.0;
            coord = CallRenderer.get2dViewFromPixelCoordinates(axes, point);

            coord[0] = CommonHandler.InverseLogScale(coord[0], logScale[0]);
            coord[1] = CommonHandler.InverseLogScale(coord[1], logScale[1]);

            if ((coord[0] >= corners[0] && coord[0] <= corners[6]) || (coord[0] <= corners[0] && coord[0] >= corners[6])) {
                if ((coord[1] >= corners[1] && coord[1] <= corners[4]) || (coord[1] <= corners[1] && coord[1] >= corners[4])) {
                    switch (i) {
                        case 0:
                            return AxesHandler.axisTo.__X__;
                        case 1:
                            return AxesHandler.axisTo.__Y__;
                        case 2:
                            return AxesHandler.axisTo.__Z__;
                        case 3:
                            return AxesHandler.axisTo.__TITLE__;
                    }
                }
            }
        }
        return null;
    }

    /**
    * Rotate the Corners points 90 degrees
    *
    * @param vec The corners points vector(should be 4 points x 3 coord, arranged sequentially)
    */
    private static Double[] rotateCorners(Double[] vec) {

        Integer length = vec.length;
        Double[] newVec = new Double[length];
        for (Integer i = 0; i < length - 3; i++) {
            newVec[i + 3] = vec[i];
        }
        newVec[0] = vec[length - 3];
        newVec[1] = vec[length - 2];
        newVec[2] = vec[length - 1];
        return newVec;
    }

    /*
     * Given a figure search for all plot3d's and for each surface
     * test if the ray given by the mouse position intersects any surface
     * @param figure Figure unique identifier.
     * @param pos Mouse position (x, y).
     * @return The nearest surface intersected or null otherwise.
     */
    String pickSurface(String figure, Integer[] pos) {
        String uid = AxesHandler.clickedAxes(figure, pos);
        curAxes = (Axes)GraphicController.getController().getObjectFromId(uid);
        double[][] factors = curAxes.getScaleTranslateFactors();
        double[] mat = DrawerVisitor.getVisitor(figure).getAxesDrawer().getProjection(uid).getMatrix();

        Vector3d v0 = AxesDrawer.unProject(curAxes, new Vector3d(1.0f * pos[0], 1.0f * pos[1], 0.0));
        Vector3d v1 = AxesDrawer.unProject(curAxes, new Vector3d(1.0f * pos[0], 1.0f * pos[1], 1.0));
        v0 = new Vector3d((v0.getX() - factors[1][0]) / factors[0][0], (v0.getY() - factors[1][1]) / factors[0][1], (v0.getZ() - factors[1][2]) / factors[0][2]);
        v1 = new Vector3d((v1.getX() - factors[1][0]) / factors[0][0], (v1.getY() - factors[1][1]) / factors[0][1], (v1.getZ() - factors[1][2]) / factors[0][2]);

        Vector3d Dir = v0.minus(v1).getNormalized();


        Integer[] types = {GraphicObjectProperties.__GO_PLOT3D__, GraphicObjectProperties.__GO_FAC3D__, GraphicObjectProperties.__GO_GRAYPLOT__};
        String[] objs = (new ObjectSearcher()).searchMultiple(figure, types);
        double Z = 2.0;
        String picked = null;
        if (objs != null) {
            for (int i = 0; i < objs.length; ++i) {
                double curZ = SurfaceData.pickSurface(objs[i], v0.getX(), v0.getY(), v0.getZ(),
                                                      Dir.getX(), Dir.getY(), Dir.getZ(), mat[2], mat[6], mat[10], mat[14]);
                if (curZ < Z) {
                    picked = objs[i];
                    Z = curZ;
                }
            }
        }

        return picked;
    }


    public String pickDatatip(String figure, Integer[] pos) {

        String axes = AxesHandler.clickedAxes(figure, pos);
        if (axes == null) {
            return null;
        }

        curAxes = AxesHandler.getAxesFromUid(axes);
        String[] datatips = (new ObjectSearcher()).search(axes, GraphicObjectProperties.__GO_DATATIP__);
        boolean[] logFlags = {  curAxes.getXAxisLogFlag(), curAxes.getYAxisLogFlag(), curAxes.getZAxisLogFlag()};

        if (datatips != null) {

            needTransform = !isInDefaultView(curAxes);
            double[] pix_pos = {1.0 * pos[0], 1.0 * pos[1], 1.0};
            double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axes, pix_pos);
            pix_pos[0] += 1.0;
            pix_pos[1] += 1.0;
            double[] c2d2 = CallRenderer.get2dViewFromPixelCoordinates(axes, pix_pos);
            double dx = Math.abs(c2d[0] - c2d2[0]);
            double dy = Math.abs(c2d[1] - c2d2[1]);

            for (int i = 0; i < datatips.length; ++i) {

                Double[] tip_pos = (Double[])GraphicController.getController().getProperty(datatips[i], GraphicObjectProperties.__GO_DATATIP_DATA__);
                double point[] = new double[3];
                point[0] = CommonHandler.logScale(tip_pos[0], logFlags[0]);
                point[1] = CommonHandler.logScale(tip_pos[1], logFlags[1]);
                point[2] = CommonHandler.logScale(tip_pos[2], logFlags[2]);


                if (needTransform) {
                    point = transformPoint(curAxes, tip_pos[0], tip_pos[1], tip_pos[2]);
                }

                Integer size = CommonHandler.getMarkSize(datatips[i]);
                Integer unit = CommonHandler.getMarkSizeUnit(datatips[i]);

                int finalSize = (unit == 1) ? (8 + 2 * size) : size;
                finalSize /= 2;

                if ((Math.abs(point[0] - c2d[0]) <= dx * finalSize) && (Math.abs(point[1] - c2d[1]) <= dy * finalSize)) {
                    return datatips[i];
                }

            }
        }
        return null;
    }
}

