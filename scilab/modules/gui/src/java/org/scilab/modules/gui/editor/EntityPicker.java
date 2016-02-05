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
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.CallGraphicController;
import org.scilab.modules.renderer.CallRenderer;

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.AxesContainer;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.graphic_objects.SurfaceData;
import org.scilab.modules.gui.datatip.DatatipCommon;
import org.scilab.modules.gui.editor.CommonHandler;
import org.scilab.modules.gui.editor.ObjectSearcher;

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

    private static final int EPS = 2;

    /**
     * Picks a polyline at the given position.
     *
     * @param figureUid     Figure uid to be check.
     * @param posX         Position on x axis in pixels.
     * @param posY         Position on y axis in pixels.
     * @return            Picked polyline uid or null if there isn't any polyline at the given position.
     */
    public Integer pick(Integer figureUid, Integer posX, Integer posY) {
        Integer[] position = {posX, posY};
        Integer axes = AxesHandler.clickedAxes(figureUid, position);
        if (axes == null) {
            return null;
        }

        double[] pos = {posX.doubleValue(), posY.doubleValue(), 1.0};
        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);

        /* Checks if the click is outside canvas drawable area*/
        if (AxesHandler.isZoomBoxEnabled(axes) && !AxesHandler.isInZoomBoxBounds(axes, c2d[0], c2d[1])) {
            return null;
        }

        Integer polylines[] = (new ObjectSearcher()).search(axes, GraphicObjectProperties.__GO_POLYLINE__);
        if (polylines != null) {
            for (int i = 0; i < polylines.length; ++i) {
                if (CommonHandler.isVisible(polylines[i])) {
                    if (CommonHandler.isLineEnabled(polylines[i])) {
                        if (isOverLine(axes, polylines[i], posX, posY) != -1) {
                            return polylines[i];
                        }
                    }

                    if (CommonHandler.isMarkEnabled(polylines[i])) {
                        if (isOverMark(axes, polylines[i], posX, posY) != -1) {
                            return polylines[i];
                        }
                    }

                    if (!CommonHandler.isLineEnabled(polylines[i])) {
                        if (isOverDot(axes, polylines[i], posX, posY) != -1) {
                            return polylines[i];
                        }
                    }
                }
            }
        }

        return null;
    }

    public static DatatipCommon.Segment getSegment(Integer uid, int index) {
        double[] datax = (double[])PolylineData.getDataX(uid);
        double[] datay = (double[])PolylineData.getDataY(uid);
        double[] dataz = (double[])PolylineData.getDataZ(uid);

        if (datax.length == 1) {
            return new DatatipCommon.Segment(0, datax[0], datax[0], datay[0], datay[0], dataz[0], dataz[0]);
        }

        if (index <= 0) {
            return new DatatipCommon.Segment(0, datax[0], datax[1], datay[0], datay[1], dataz[0], dataz[1]);
        } else if (index >= datax.length - 1) {
            return new DatatipCommon.Segment(datax.length - 2, datax[datax.length - 2], datax[datax.length - 1], datay[datay.length - 2], datay[datay.length - 1], dataz[dataz.length - 2], dataz[dataz.length - 1]);
        } else {
            return new DatatipCommon.Segment(index, datax[index], datax[index + 1], datay[index], datay[index + 1], datay[index], datay[index + 1]);
        }
    }

    public static double[] getSegment(Integer figureUid, Integer uid, int index, int x, int y) {
        final Integer axes = AxesHandler.clickedAxes(figureUid, new Integer[] {x, y});
        final double[][] pixs = EntityPicker.getPixelsData(axes, uid);
        final double[] p1 = pixs[index];
        final double[] p2 = pixs[index + 1];
        final double px = p2[0] - p1[0];
        final double py = p2[1] - p1[1];
        final double ps = (x - p1[0]) * px + (y - p1[1]) * py;
        final double[] info = new double[] {index, 0};

        if (0 <= ps) {
            final double n2 = px * px + py * py;
            // the projected point is in the segment [p1,p2]
            if (ps <= n2) {
                info[1] = ps / n2;
            } else {
                info[1] = 1;
            }
        }

        return info;
    }

    public static double[][] getPixelsData(Integer axes, Integer uid) {
        double[] datax = (double[])PolylineData.getDataX(uid);
        double[] datay = (double[])PolylineData.getDataY(uid);
        double[] dataz = (double[])PolylineData.getDataZ(uid);
        int size = datax.length;

        if (PolylineData.isXShiftSet(uid) != 0) {
            double[] x_shift = (double[])PolylineData.getShiftX(uid);
            for (int i = 0; i < size; i++) {
                datax[i] += x_shift[i];
            }
        }

        if (PolylineData.isYShiftSet(uid) != 0) {
            double[] y_shift = (double[])PolylineData.getShiftY(uid);
            for (int i = 0; i < size; i++) {
                datay[i] += y_shift[i];
            }
        }

        if (PolylineData.isZShiftSet(uid) != 0) {
            double[] z_shift = (double[])PolylineData.getShiftZ(uid);
            for (int i = 0; i < size; i++) {
                dataz[i] += z_shift[i];
            }
        }

        return CallRenderer.getPixelFrom3dCoordinates(axes, datax, datay, dataz);
    }

    public static double[] getNearestSegmentIndex(double[][] data, double x, double y) {
        if (data != null && data.length >= 1) {
            int minIndex = 0;
            double minD2 = Double.MAX_VALUE;

            for (int i = 0; i < data.length - 1; i++) {
                final double[] p1 = data[i];
                final double[] p2 = data[i + 1];
                final double px = p2[0] - p1[0];
                final double py = p2[1] - p1[1];
                final double mpx = x - p1[0];
                final double mpy = y - p1[1];
                final double ps = mpx * px + mpy * py;
                final double d2;

                if (0 <= ps) {
                    final double n2 = px * px + py * py;
                    // the projected point is in the segment [p1,p2]
                    if (ps <= n2) {
                        d2 = mpx * mpx + mpy * mpy - ps * ps / n2;
                    } else {
                        d2 = (x - p2[0]) * (x - p2[0]) + (y - p2[1]) * (y - p2[1]);
                    }
                } else {
                    d2 = mpx * mpx + mpy * mpy;
                }

                if (d2 < minD2) {
                    minD2 = d2;
                    minIndex = i;
                }
            }

            final double[] p1 = data[minIndex];
            final double[] p2 = data[minIndex + 1];
            final double px = p2[0] - p1[0];
            final double py = p2[1] - p1[1];
            final double ps = (x - p1[0]) * px + (y - p1[1]) * py;
            double[] info = new double[] {minIndex, 0};

            if (0 <= ps) {
                final double n2 = px * px + py * py;
                // the projected point is in the segment [p1,p2]
                if (ps <= n2) {
                    info[1] = ps / n2;
                } else {
                    info[1] = 1;
                }
            }

            return info;
        }

        return null;
    }

    public static double[] getNearestSegmentIndex(double[][] data, double x, double y, double z) {
        if (data != null && data.length >= 1) {
            int minIndex = 0;
            double minD2 = Double.MAX_VALUE;

            for (int i = 0; i < data.length - 1; i++) {
                final double[] p1 = data[i];
                final double[] p2 = data[i + 1];
                final double px = p2[0] - p1[0];
                final double py = p2[1] - p1[1];
                final double pz = p2[2] - p1[2];
                final double mpx = x - p1[0];
                final double mpy = y - p1[1];
                final double mpz = z - p1[2];
                final double ps = mpx * px + mpy * py + mpz * pz;
                final double d2;

                if (0 <= ps) {
                    final double n2 = px * px + py * py + pz * pz;
                    // the projected point is in the segment [p1,p2]
                    if (ps <= n2) {
                        d2 = mpx * mpx + mpy * mpy + mpz * mpz - ps * ps / n2;
                    } else {
                        d2 = (x - p2[0]) * (x - p2[0]) + (y - p2[1]) * (y - p2[1]) + (z - p2[2]) * (z - p2[2]);
                    }
                } else {
                    d2 = mpx * mpx + mpy * mpy + mpz * mpz;
                }

                if (d2 < minD2) {
                    minD2 = d2;
                    minIndex = i;
                }
            }

            final double[] p1 = data[minIndex];
            final double[] p2 = data[minIndex + 1];
            final double px = p2[0] - p1[0];
            final double py = p2[1] - p1[1];
            final double pz = p2[2] - p1[2];
            final double ps = (x - p1[0]) * px + (y - p1[1]) * py + (z - p1[2]) * pz;
            double[] info = new double[] {minIndex, 0};

            if (0 <= ps) {
                final double n2 = px * px + py * py + pz * pz;
                // the projected point is in the segment [p1,p2]
                if (ps <= n2) {
                    info[1] = ps / n2;
                } else {
                    info[1] = 1;
                }
            }

            return info;
        }

        return null;
    }

    public static double[] getNearestSegmentIndex(Integer axes, Integer uid, int x, int y) {
        return getNearestSegmentIndex(EntityPicker.getPixelsData(axes, uid), (double) x, (double) y);
    }

    public static double[] getNearestSegmentIndex(Integer uid, double x, double y, double z) {
        return getNearestSegmentIndex(DatatipCommon.getPolylineDataMatrix(uid, false), x, y, z);
    }

    public static double[] getNearestSegmentIndex(Integer uid, double x, double y) {
        return getNearestSegmentIndex(DatatipCommon.getPolylineDataMatrix(uid, true), x, y);
    }

    /**
     * Check algorithm linear interpolation for each pair of points.
     * @param uid    Polyline uid to be checked.
     * @param x        position on x axis in view coordinates.
     * @param y        position on y axis in view coordinates.
     * @return        true if x,y belongs or is closest to the polyline.
     */
    private int isOverLine(Integer axes, Integer uid, int x, int y) {
        double[][] pixs = EntityPicker.getPixelsData(axes, uid);
        for (int i = 0; i < pixs.length - 1; i++) {
            // We could probably make something more efficient
            // but in the case of datatip it is probably useless...
            // So wait and see the user feedback.
            final double[] p1 = pixs[i];
            final double[] p2 = pixs[i + 1];
            final double px = p2[0] - p1[0];
            final double py = p2[1] - p1[1];
            final double mpx = x - p1[0];
            final double mpy = y - p1[1];
            final double ps = mpx * px + mpy * py;

            // the projected point is in the segment [p1,p2]
            if (0 <= ps) {
                final double n2 = px * px + py * py;
                if (ps <= n2) {
                    final double d = mpx * mpx + mpy * mpy - ps * ps / n2;
                    // the square of the distance is lower than EPS^2
                    if (d <= EPS * EPS) {
                        return i;
                    }
                }
            }
        }

        return -1;
    }

    /**
     * Checks if the given point belongs the polyline dot.
     * @param uid    Polyline uid to be checked.
     * @param x        position on x axis in view coordinates.
     * @param y        position on y axis in view coordinates.
     * @return        True if x,y belongs to the polyline mark.
     */
    private int isOverDot(Integer axes, Integer uid, int x, int y) {
        double[][] pixs = EntityPicker.getPixelsData(axes, uid);
        for (int i = 0; i < pixs.length; ++i) {
            if ((Math.abs(x - pixs[i][0]) <= EPS + 1) && (Math.abs(y - pixs[i][1]) <= EPS + 1)) {
                return i;
            }
        }

        return -1;
    }

    /**
     * Checks if the given point belongs the polyline dot.
     * @param uid    Polyline uid to be checked.
     * @param x        position on x axis in view coordinates.
     * @param y        position on y axis in view coordinates.
     * @return        True if x,y belongs to the polyline mark.
     */
    private int isOverBar(Integer axes, Integer uid, int x, int y, int width) {
        double[][] pixs = EntityPicker.getPixelsData(axes, uid);
        for (int i = 0; i < pixs.length; ++i) {
            if ((Math.abs(x - pixs[i][0]) <= width) && (Math.abs(y - pixs[i][1]) <= EPS)) {
                return i;
            }
        }

        return -1;
    }

    /**
     * Checks if the given point belongs the polyline mark.
     * @param uid    Polyline uid to be checked.
     * @param x        position on x axis in view coordinates.
     * @param y        position on y axis in view coordinates.
     * @return        True if x,y belongs to the polyline mark.
     */
    private int isOverMark(Integer axes, Integer uid, int x, int y) {
        double[][] pixs = EntityPicker.getPixelsData(axes, uid);
        Integer size = CommonHandler.getMarkSize(uid);
        Integer unit = CommonHandler.getMarkSizeUnit(uid);

        int finalSize = (unit == 1) ? (8 + 2 * size) : size;
        finalSize /= 2;

        for (int i = 0; i < pixs.length; ++i) {
            if ((Math.abs(x - pixs[i][0]) <= finalSize) && (Math.abs(y - pixs[i][1]) <= finalSize)) {
                return i;
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

        public String toString() {
            return "Point: " + point + ", isSegment=" + isSegment;
        }
    }

    /**
     * Given a polyline uid checks if the given point (px,py)
     * belongs or is closer to any polyline point.
     *
     * @return The picked point or PickedPoint.point = -1 otherwise.
     */
    public PickedPoint pickPoint(Integer uid, int px, int py) {
        PickedPoint point = new PickedPoint(-1, false);
        Integer[] position = new Integer[] {px, py};
        Integer figUid = (new ObjectSearcher()).searchParent(uid, GraphicObjectProperties.__GO_FIGURE__);
        Integer axes = AxesHandler.clickedAxes(figUid, position);
        if (axes == null) {
            return point;
        }

        if (CommonHandler.isMarkEnabled(uid)) {
            point.point = isOverMark(axes, uid, px, py);
            if (point.point != -1) {
                return point;
            }
        }

        if (CommonHandler.isLineEnabled(uid)) {
            point.point = isOverLine(axes, uid, px, py);
            point.isSegment = true;
        } else {
            point.point = isOverDot(axes, uid, px, py);
        }

        return point;
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
        public Integer legend = null;
        public Integer polyline = null;
        LegendInfo(Integer legend, Integer polyline) {
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
    public LegendInfo pickLegend(Integer figure, Integer[] position) {
        Integer axes = AxesHandler.clickedAxes(figure, position);
        if (axes == null) {
            return null;
        }
        Integer legend = LegendHandler.searchLegend(axes);
        if (legend == null) {
            return null;
        }

        Integer[] links;
        Integer[] axesSize = {0, 0};
        Double delta;
        Double[] axesBounds = { 0., 0. }, dPosition = { 0., 0. }, legendPos = { 0., 0. }, legendBounds = { 0., 0., 0., 0. }, dimension = { 0., 0. };

        AxesContainer container = (AxesContainer) GraphicController.getController().getObjectFromId(figure);
        axesSize = container.getAxesSize();

        axesBounds = (Double[])GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_AXES_BOUNDS__);
        legendPos = (Double[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_POSITION__);
        links = (Integer[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_LINKS__);
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
    public static AxesHandler.axisTo pickLabel(Integer figure, Integer[] pos) {

        Integer axes = AxesHandler.clickedAxes(figure, pos);
        if (axes == null) {
            return null;
        }
        Double[] corners;
        Double radAngle = 0.;
        int rotate = 0;
        double[] point = new double[3];
        double[] coord;
        Integer[] label = {  (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_X_AXIS_LABEL__),
                             (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__),
                             (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__),
                             (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_TITLE__)
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
    Integer pickSurface(Integer figure, Integer[] pos) {
        Integer uid = AxesHandler.clickedAxes(figure, pos);
        Axes curAxes = (Axes)GraphicController.getController().getObjectFromId(uid);
        if (curAxes == null) {
            return null;
        }

        double[][] factors = curAxes.getScaleTranslateFactors();
        double[] mat = DrawerVisitor.getVisitor(figure).getAxesDrawer().getProjection(uid).getMatrix();

        Vector3d v0 = AxesDrawer.unProject(curAxes, new Vector3d(1.0f * pos[0], 1.0f * pos[1], 0.0));
        Vector3d v1 = AxesDrawer.unProject(curAxes, new Vector3d(1.0f * pos[0], 1.0f * pos[1], 1.0));
        v0 = new Vector3d((v0.getX() - factors[1][0]) / factors[0][0], (v0.getY() - factors[1][1]) / factors[0][1], (v0.getZ() - factors[1][2]) / factors[0][2]);
        v1 = new Vector3d((v1.getX() - factors[1][0]) / factors[0][0], (v1.getY() - factors[1][1]) / factors[0][1], (v1.getZ() - factors[1][2]) / factors[0][2]);

        Vector3d Dir = v0.minus(v1).getNormalized();


        Integer[] types = {GraphicObjectProperties.__GO_PLOT3D__, GraphicObjectProperties.__GO_FAC3D__, GraphicObjectProperties.__GO_GRAYPLOT__};
        Integer[] objs = (new ObjectSearcher()).searchMultiple(figure, types);
        double Z = 2.0;
        Integer picked = null;
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


    public Integer pickDatatip(Integer figure, Integer[] pos) {
        Integer axes = AxesHandler.clickedAxes(figure, pos);
        if (axes == null) {
            return null;
        }

        Integer[] datatips = (new ObjectSearcher()).search(axes, GraphicObjectProperties.__GO_DATATIP__, true);
        if (datatips != null) {
            int x = pos[0];
            int y = pos[1];

            for (int i = 0; i < datatips.length; ++i) {
                Double[] tip_pos = (Double[])GraphicController.getController().getProperty(datatips[i], GraphicObjectProperties.__GO_DATATIP_DATA__);
                double[] point = new double[] {tip_pos[0], tip_pos[1], tip_pos[2]};
                double[] pix = CallRenderer.getPixelFrom3dCoordinates(axes, point);

                Integer size = CommonHandler.getMarkSize(datatips[i]);
                Integer unit = CommonHandler.getMarkSizeUnit(datatips[i]);

                int finalSize = (unit == 1) ? (8 + 2 * size) : size;
                finalSize /= 2;

                if ((Math.abs(x - pix[0]) <= finalSize) && (Math.abs(y - pix[1]) <= finalSize)) {
                    return datatips[i];
                }
            }
        }

        return null;
    }
}

