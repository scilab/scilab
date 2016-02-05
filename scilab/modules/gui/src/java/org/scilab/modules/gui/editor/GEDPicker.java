/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro Arthur dos S. Souza
 * Copyright (C) 2013 - Caio Lucas dos S. Souza
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
import org.scilab.modules.graphic_objects.CallGraphicController;
import org.scilab.modules.renderer.CallRenderer;

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.graphic_objects.SurfaceData;
import org.scilab.modules.graphic_objects.ObjectData;
import org.scilab.modules.gui.editor.CommonHandler;
import org.scilab.modules.gui.editor.AxesHandler;

import java.lang.Math;
import java.util.List;
import java.util.LinkedList;
/**
 *
 * GED Object picker for all Graphics Objects
 *
 */
public class GEDPicker {

    private final Double delta = 7.0;
    private Integer axesUID;
    private Axes axes;
    private double Z;

    /**
     * Picker, given a figure and a click position in pixel coordinates
     * it returns an array of objects picked
     *
     * @param figureUID Unique id from the figure object
     * @param position Mouse click position in pixels
     * @return Array of picked objects
     */
    public Integer[] pick(Integer figureUID, Integer[] position) {

        Integer[] AllObjs;
        boolean curObj = false;

        axesUID = AxesHandler.clickedAxes(figureUID, position);
        axes = AxesHandler.getAxesFromUid(axesUID);

        if (axes == null) {
            return new Integer[] { figureUID };
        }
        List<Integer> ret = new LinkedList<Integer>();
        AllObjs = getAllObjAsArray(axesUID);

        for (int i = 0; i < AllObjs.length; i++) {

            Integer type = (Integer)GraphicController.getController().getProperty(AllObjs[i], GraphicObjectProperties.__GO_TYPE__);
            if (!CommonHandler.isVisible(AllObjs[i])) {
                continue;
            }
            switch (type) {

                case GraphicObjectProperties.__GO_POLYLINE__:
                    curObj = getPolyline(AllObjs[i], position);
                    break;
                case GraphicObjectProperties.__GO_PLOT3D__:
                case GraphicObjectProperties.__GO_GRAYPLOT__:
                case GraphicObjectProperties.__GO_FAC3D__:
                case GraphicObjectProperties.__GO_MATPLOT__:
                    curObj = getSurface(AllObjs[i], position);
                    break;
                case GraphicObjectProperties.__GO_LEGEND__:
                    curObj = getLegend(AllObjs[i], position);
                    break;
                case GraphicObjectProperties.__GO_LABEL__:
                    curObj = getLabel(AllObjs[i], position);
                    break;
                case GraphicObjectProperties.__GO_ARC__:
                    curObj = getArc(AllObjs[i], position);
                    break;
                case GraphicObjectProperties.__GO_CHAMP__:
                    curObj = getChamp(AllObjs[i], position);
                    break;
                case GraphicObjectProperties.__GO_FEC__:
                    curObj = getFec(AllObjs[i], position);
                    break;
                case GraphicObjectProperties.__GO_RECTANGLE__:
                    curObj = getRectangle(AllObjs[i], position);
                    break;
                case GraphicObjectProperties.__GO_SEGS__:
                    curObj = getSegs(AllObjs[i], position);
                    break;
                case GraphicObjectProperties.__GO_DATATIP__:
                    curObj = getDatatip(AllObjs[i], position);
                    break;
                case GraphicObjectProperties.__GO_COMPOUND__:
                    curObj = false;
                    break;
            }
            if (curObj) {
                ret.add(AllObjs[i]);
            }
        }
        if (ret.size() > 0) {
            return ret.toArray(new Integer[ret.size()]);
        }

        if (isInsideAxes(figureUID, position)) {
            return new Integer[] { axesUID };
        } else {
            return new Integer[] { figureUID };
        }
    }

    /**
     * Polyline picker, given a polyline object it checks if the click
     * was over it
     *
     * @param obj The given polyline object uid
     * @param position Mouse click position in pixels
     * @return true if picked the polyline otherwise returns false
     */
    boolean getPolyline(Integer obj, Integer[] position) {

        double[] datax = (double[])PolylineData.getDataX(obj);
        double[] datay = (double[])PolylineData.getDataY(obj);
        double[] dataz = (double[])PolylineData.getDataZ(obj);

        if (PolylineData.isXShiftSet(obj) != 0) {
            double[] x_shift = (double[])PolylineData.getShiftX(obj);
            for (int i = 0; i < datax.length; ++i) {
                datax[i] += x_shift[i];
            }
        }

        if (PolylineData.isYShiftSet(obj) != 0) {
            double[] y_shift = (double[])PolylineData.getShiftY(obj);
            for (int i = 0; i < datay.length; ++i) {
                datay[i] += y_shift[i];
            }
        }

        if (PolylineData.isZShiftSet(obj) != 0) {
            double[] z_shift = (double[])PolylineData.getShiftZ(obj);
            for (int i = 0; i < dataz.length; ++i) {
                dataz[i] += z_shift[i];
            }
        }

        datax = CommonHandler.toLogScale(datax, axes.getXAxisLogFlag());
        datay = CommonHandler.toLogScale(datay, axes.getYAxisLogFlag());
        dataz = CommonHandler.toLogScale(dataz, axes.getZAxisLogFlag());


        double[] pos = {1.0 * position[0], 1.0 * position[1], 1.0};
        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axesUID, pos);

        pos[0] += delta;
        pos[1] += delta;
        double[] c2d2 = CallRenderer.get2dViewFromPixelCoordinates(axesUID, pos);

        double dx = Math.abs(c2d[0] - c2d2[0]);
        double dy = Math.abs(c2d[1] - c2d2[1]);

        if (AxesHandler.isZoomBoxEnabled(axesUID)) {
            if (!AxesHandler.isInZoomBoxBounds(axesUID, c2d[0], c2d[1])) {
                return false;
            }
        }

        Double[] rotAngles = (Double[])GraphicController.getController().getProperty(axesUID, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
        boolean default2dView = (rotAngles[0] == 0.0 && rotAngles[1] == 270.0);
        double[] oldPoint = new double[3];

        if (CommonHandler.isLineEnabled(obj)) {
            //System.out.println("lineEnable");
            if (!default2dView) {
                oldPoint = AxesDrawer.compute2dViewCoordinates(axes, new double[] { datax[0], datay[0], dataz[0] });
                //System.out.println("NotDefault");
            }

            for (Integer i = 0; i < (datax.length - 1); ++i) {
                if (!default2dView) {
                    double[] newPoint = AxesDrawer.compute2dViewCoordinates(axes, new double[] {datax[i + 1], datay[i + 1], dataz[i + 1]});
                    if (isInRange(oldPoint[0], newPoint[0], oldPoint[1], newPoint[1], c2d[0], c2d[1], dx, dy)) {
                        //System.out.println("in");
                        return true;
                    }
                    oldPoint = newPoint;
                } else {
                    if (isInRange(datax[i], datax[i + 1], datay[i], datay[i + 1], c2d[0], c2d[1], dx, dy)) {
                        return true;
                    }
                }
            }
        }

        if (CommonHandler.isMarkEnabled(obj)) {

            Integer markSize = CommonHandler.getMarkSize(obj);
            Integer unit = CommonHandler.getMarkSizeUnit(obj);

            int finalSize = (unit == 1) ? (8 + 2 * markSize) : markSize;
            finalSize /= 2;
            double deltax = Math.abs((dx / delta) * finalSize);
            double deltay = Math.abs((dy / delta) * finalSize);


            for (int i = 0; i < datax.length; ++i) {
                if (!default2dView) {
                    double[] point = AxesDrawer.compute2dViewCoordinates(axes, new double[] {datax[i], datay[i], dataz[i]});
                    if ((Math.abs(point[0] - c2d[0]) <= deltax) && (Math.abs(point[1] - c2d[1]) <= deltay)) {
                        return true;
                    }
                } else {
                    if ((Math.abs(datax[i] - c2d[0]) <= deltax) && (Math.abs(datay[i] - c2d[1]) <= deltay)) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    /**
     * Surface picker, given a plot3d/fac3d/grayplot/matplot object it checks if the click
     * was over it
     *
     * @param obj The given surface object uid
     * @param position Mouse click position in pixels
     * @return true if picked the surface otherwise returns false
     */
    boolean getSurface(Integer obj, Integer[] position) {
        GraphicObject go = GraphicController.getController().getObjectFromId(obj);
        Integer figure = go.getParentFrameOrFigure();
        double[] mat = DrawerVisitor.getVisitor(figure).getAxesDrawer().getProjection(axesUID).getMatrix();
        double[][] factors = axes.getScaleTranslateFactors();

        Vector3d v0 = AxesDrawer.unProject(axes, new Vector3d(1.0f * position[0], 1.0f * position[1], 0.0));
        Vector3d v1 = AxesDrawer.unProject(axes, new Vector3d(1.0f * position[0], 1.0f * position[1], 1.0));
        v0 = new Vector3d((v0.getX() - factors[1][0]) / factors[0][0], (v0.getY() - factors[1][1]) / factors[0][1], (v0.getZ() - factors[1][2]) / factors[0][2]);
        v1 = new Vector3d((v1.getX() - factors[1][0]) / factors[0][0], (v1.getY() - factors[1][1]) / factors[0][1], (v1.getZ() - factors[1][2]) / factors[0][2]);

        Vector3d Dir = v0.minus(v1).getNormalized();
        Z = 2.0;
        double curZ = SurfaceData.pickSurface(obj, v0.getX(), v0.getY(), v0.getZ(),
                                              Dir.getX(), Dir.getY(), Dir.getZ(), mat[2], mat[6], mat[10], mat[14]);
        if (curZ < Z) {
            return true;
        }
        return false;
    }

    /**
     * Legend picker, given a legend object it checks if the click
     * was over it
     *
     * @param obj The given legend object uid
     * @param position Mouse click position in pixels
     * @return true if picked the legend otherwise returns false
     */
    boolean getLegend(Integer obj, Integer[] position) {
        Integer[] axesSize = {0, 0};
        Double delta;
        Double[] axesBounds = { 0., 0. }, dPosition = { 0., 0. }, legendPos = { 0., 0. }, legendBounds = { 0., 0., 0., 0. }, dimension = { 0., 0. };

        GraphicObject go = GraphicController.getController().getObjectFromId(obj);
        Integer figure = go.getParentFrameOrFigure();
        axesSize = (Integer[])GraphicController.getController().getProperty(figure, GraphicObjectProperties.__GO_AXES_SIZE__);
        axesBounds = (Double[])GraphicController.getController().getProperty(axesUID, GraphicObjectProperties.__GO_AXES_BOUNDS__);
        legendPos = (Double[])GraphicController.getController().getProperty(obj, GraphicObjectProperties.__GO_POSITION__);
        dPosition[0] = (position[0] - (axesBounds[0] * axesSize[0])) / (axesBounds[2] * axesSize[0]);
        dPosition[1] = (position[1] - (axesBounds[1] * axesSize[1])) / (axesBounds[3] * axesSize[1]);
        dimension = (Double[])GraphicController.getController().getProperty(obj, GraphicObjectProperties.__GO_SIZE__);
        legendBounds[0] =  legendPos[0];
        legendBounds[1] = legendPos[1];
        legendBounds[2] = legendPos[0] + dimension[0];
        legendBounds[3] = legendPos[1] + dimension[1];

        if (dPosition[0] >= legendBounds[0] && dPosition[0] <= legendBounds[2] && dPosition[1] >= legendBounds[1] && dPosition[1] <= legendBounds[3]) {
            return true;
        }
        return false;
    }

    /**
     * Label picker, given a label object it checks if the click
     * was over it
     *
     * @param obj The given surface object uid
     * @param position Mouse click position in pixels
     * @return true if picked the surface otherwise returns false
     */
    boolean getLabel(Integer obj, Integer[] position) {

        Double[] corners = (Double[])GraphicController.getController().getProperty(obj, GraphicObjectProperties.__GO_CORNERS__);
        Double radAngle = (Double)GraphicController.getController().getProperty(obj, GraphicObjectProperties.__GO_FONT_ANGLE__);
        int rotate = ((int)((radAngle * 2) / Math.PI)) % 2;
        if (rotate == 1) {

            Double[] temp = new Double[corners.length];
            for (Integer i = 0; i < corners.length - 3; i++) {
                temp[i + 3] = corners[i];
            }
            temp[0] = corners[corners.length - 3];
            temp[1] = corners[corners.length - 2];
            temp[2] = corners[corners.length - 1];
            corners = temp;
        }
        double[] point = new double[] { 1.0 * position[0], 1.0 * position[1], 1.0};
        double[] coord = CallRenderer.get2dViewFromPixelCoordinates(axesUID, point);
        boolean[] logScale = { 	(Boolean)GraphicController.getController().getProperty(axesUID, GraphicObjectProperties.__GO_X_AXIS_LOG_FLAG__),
                                (Boolean)GraphicController.getController().getProperty(axesUID, GraphicObjectProperties.__GO_Y_AXIS_LOG_FLAG__)
                             };
        coord[0] = CommonHandler.InverseLogScale(coord[0], logScale[0]);
        coord[1] = CommonHandler.InverseLogScale(coord[1], logScale[1]);

        if ((coord[0] >= corners[0] && coord[0] <= corners[6]) || (coord[0] <= corners[0] && coord[0] >= corners[6])) {
            if ((coord[1] >= corners[1] && coord[1] <= corners[4]) || (coord[1] <= corners[1] && coord[1] >= corners[4])) {
                return true;
            }
        }
        return false;
    }

    /**
    * Arc picker, given a arc object it checks if the click
    * was over it
    *
    * @param obj The given arc object uid
    * @param position Mouse click position in pixels
    * @return true if picked the arc otherwise returns false
    */
    boolean getArc(Integer obj, Integer[] position) {

        double[] upperLeft = (double[])ObjectData.getArcUpperLeftPoint(obj);
        double[] data = (double[])ObjectData.getArcData(obj);
        double[] pos = { position[0] * 1.0, position[1] * 1.0, 0.0 };
        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axesUID, pos);
        pos[0] += delta;
        pos[1] += delta;
        double[] c2d2 = CallRenderer.get2dViewFromPixelCoordinates(axesUID, pos);
        Boolean fill_mode = (Boolean)GraphicController.getController().getProperty(obj, GraphicObjectProperties.__GO_FILL_MODE__);

        //Calculates the selection delta based on ellipse & axes size
        double dt = Math.sqrt(Math.pow(c2d[0] - c2d2[0], 2.0) + Math.pow(c2d[1] - c2d2[1], 2.0)) / Math.sqrt((data[0] * data[0] / 4.) / 2. + (data[1] * data[1] / 4.) / 2.);

        double[] c3d1 = AxesDrawer.compute3dViewCoordinates(axes, c2d);
        c2d[2] += 1.0;
        double[] c3d2 = AxesDrawer.compute3dViewCoordinates(axes, c2d);

        //Calculates the intersection of the click ray with the ellipse plane
        Vector3d v0 = new Vector3d(c3d1);
        Vector3d v1 = new Vector3d(c3d2);
        Vector3d dir = v1.minus(v0);

        if (dir.getZ() == 0) {
            return false;
        }

        double u = (upperLeft[2] - v0.getZ()) / dir.getZ();
        Vector3d point = v0.plus(dir.times(u));

        double xr = data[1] / 2.;
        double yr = data[0] / 2.;

        Vector3d center =  new Vector3d(upperLeft[0] + xr , upperLeft[1] - yr , upperLeft[2]);

        //checks if the point lies within the ellipse
        double x = point.getX() - center.getX();
        double y = point.getY() - center.getY();
        double v = (x * x) / (xr * xr) + (y * y) / (yr * yr);

        if (v > 1.0 + dt) {
            return false;
        }

        double angle0 = getAngle360(data[2]);
        double angle1 = getAngle360(data[3]) + angle0;

        /**
         * Checks if the point is closer to the drawn arc
         * and inside de given angles. If fill_mode = false
         * it it accept 1-dt < value < 1+dt, if fill_mode = true
         * it accept value < 1+dt;
         */

        v0 = new Vector3d(1.0, 0.0, 0.0);
        v1 = point.minus(center);
        double angle = Math.acos(v0.scalar(v1) / (v0.getNorm() * v1.getNorm()));
        angle += v1.getY() < 0.0 ? Math.PI : 0.0;

        if (!(angle0 == angle1) && !(angle >= angle0 && angle <= angle1)) {
            return false;
        }

        if (fill_mode) {
            return true;
        } else if (v > 1.0 - dt) {
            return true;
        }

        return false;
    }

    double getAngle360(double a) {
        while (a > 2 * Math.PI) {
            a -= 2 * Math.PI;
        }
        return a;
    }

    /**
     * Champ picker, given a champ object it checks if the click
     * was over it
     *
     * @param obj The given champ object uid
     * @param position Mouse click position in pixels
     * @return true if picked the champ otherwise returns false
     */
    boolean getChamp(Integer obj, Integer[] position) {

        double[] datax = (double[])ObjectData.getChampX(obj);
        double[] datay = (double[])ObjectData.getChampY(obj);
        double[] data = (double[])ObjectData.getArrows(obj);

        double scale = 0;
        for (int i = 0; i < data.length; i++) {
            scale = data[i] > scale ? data[i] : scale;
        }

        double[][] arrows = getAsStdMatrix(data, scale);
        if (arrows == null) {
            return false;
        }
        double[] pos = {1.0 * position[0], 1.0 * position[1], 1.0};

        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axesUID, pos);
        pos[0] += delta;
        pos[1] += delta;
        double[] c2d2 = CallRenderer.get2dViewFromPixelCoordinates(axesUID, pos);

        double dx = Math.abs(c2d[0] - c2d2[0]);
        double dy = Math.abs(c2d[1] - c2d2[1]);
        int xSize = datax.length;
        int ySize = datay.length;

        for (int i = 0; i < xSize; i++) {
            for (int j = 0; j < ySize; j++) {

                double[] ch2d = AxesDrawer.compute2dViewCoordinates(axes, new double[] { datax[i], datay[j], 0.0});
                double[] temp = { datax[i] + arrows[i * xSize + j][0], datay[j] + arrows[i * xSize + j][1], arrows[i * xSize + j][2]};
                double[] ch2d2 = AxesDrawer.compute2dViewCoordinates(axes, temp);
                if (isInRange(ch2d[0], ch2d2[0], ch2d[1], ch2d2[1], c2d[0], c2d[1], dx, dy)) {
                    return true;
                }
                if ((Math.abs(c2d[0] - ch2d[0]) <= dx) && (Math.abs(c2d[1] - ch2d[1]) <= dy)) {
                    return true;
                }
                if ((Math.abs(c2d[0] - ch2d2[0]) <= dx) && (Math.abs(c2d[1] - ch2d2[1]) <= dy)) {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * Fec picker, given a fec object it checks if the click
     * was over it
     *
     * @param obj The given fec object uid
     * @param position Mouse click position in pixels
     * @return true if picked the fec otherwise returns false
     */
    boolean getFec(Integer obj, Integer[] position) {
        int numVerticesByElem = ObjectData.getFecNumVerticesByElement(obj);
        double[] elements = (double[])ObjectData.getFecElements(obj);
        double[] data = (double[])ObjectData.getFecData(obj);

        double[] pos = { position[0] * 1.0, position[1] * 1.0, 0.0 };
        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axesUID, pos);
        double[] c3d1 = AxesDrawer.compute3dViewCoordinates(axes, c2d);
        c2d[2] += 1.0;

        double[] c3d2 = AxesDrawer.compute3dViewCoordinates(axes, c2d);
        Vector3d l0 = new Vector3d(c3d1);
        Vector3d l1 = new Vector3d(c3d2);
        Vector3d dir = l1.minus(l0);
        int idx1, idx2, idx3;
        Vector3d p1, p2, p3;

        int tSize = elements.length / (numVerticesByElem + 2);
        for (int i = 0; i < tSize; i++) {
            idx1 = (int)elements[tSize + i];
            for (int j = 2; j < numVerticesByElem; ++j) {
                idx2 = (int)elements[j * tSize + i];
                idx3 = (int)elements[(j + 1) * tSize + i];

                p1 = new Vector3d(data[(idx1 - 1) * 3], data[(idx1 - 1) * 3 + 1], data[(idx1 - 1) * 3 + 2]);
                p2 = new Vector3d(data[(idx2 - 1) * 3], data[(idx2 - 1) * 3 + 1], data[(idx2 - 1) * 3 + 2]);
                p3 = new Vector3d(data[(idx3 - 1) * 3], data[(idx3 - 1) * 3 + 1], data[(idx3 - 1) * 3 + 2]);

                if (testTri(p1, p2, p3, l0, dir)) {
                    return true;
                }
            }
        }
        return false;
    }
    /**
     * Möller–Trumbore intersection algorithm
     * Test if a line intersect a triangle
     *
     * @param p1 The vertex 1 of the triangle
     * @param p2 The vertex 2 of the triangle
     * @param p3 The vertex 3 of the triangle
     * @param l0 origin point
     * @param direction The direction vector
     * @return true if it intersect the triangle, false otherwise
     */
    private boolean testTri(Vector3d p1, Vector3d p2, Vector3d p3, Vector3d l0, Vector3d direction) {

        Vector3d e1 = p2.minus(p1);
        Vector3d e2 = p3.minus(p1);

        Vector3d h = Vector3d.product(direction, e2);
        double det = e1.scalar(h);

        if (det > -0.00001 && det < 0.00001) {
            return false;
        }

        double inv = 1.0 / det;
        Vector3d s = l0.minus(p1);

        double u = s.scalar(h) * inv;
        if (u < 0.0 || u > 1.0) {
            return false;
        }

        Vector3d q = Vector3d.product(s, e1);
        double v = direction.scalar(q) * inv;
        if (v < 0.0 || (u + v) > 1.0) {
            return false;
        }

        return true;
    }

    /**
     * Rectangle picker, given a rectangle object it checks if the click
     * was over it
     *
     * @param obj The given rectangle object uid
     * @param position Mouse click position in pixels
     * @return true if picked the rectangle otherwise returns false
     */
    boolean getRectangle(Integer obj, Integer[] position) {

        Double[] upperLeft = (Double[])GraphicController.getController().getProperty(obj, GraphicObjectProperties.__GO_UPPER_LEFT_POINT__);
        Double height = (Double)GraphicController.getController().getProperty(obj, GraphicObjectProperties.__GO_HEIGHT__);
        Double width = (Double)GraphicController.getController().getProperty(obj, GraphicObjectProperties.__GO_WIDTH__);
        double[] pos = { position[0] * 1.0, position[1] * 1.0, 0.0 };
        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axesUID, pos);

        double[] c3d1 = AxesDrawer.compute3dViewCoordinates(axes, c2d);
        c2d[2] += 1.0;
        double[] c3d2 = AxesDrawer.compute3dViewCoordinates(axes, c2d);

        Vector3d v0 = new Vector3d(c3d1);
        Vector3d v1 = new Vector3d(c3d2);
        Vector3d c = v1.minus(v0);

        if (c.getZ() == 0) {
            return false;
        }

        double u = (upperLeft[2] - v0.getZ()) / c.getZ();
        Vector3d point = v0.plus(c.times(u));

        if (point.getX() >= upperLeft[0] && point.getX() <= (upperLeft[0] + width)
                && point.getY() <= upperLeft[1] && point.getY() >= (upperLeft[1] - height)) {
            return true;
        }
        return false;
    }

    /**
     * Segs picker, given a segs object it checks if the click
     * was over it
     *
     * @param obj The given segs object uid
     * @param position Mouse click position in pixels
     * @return true if picked the segs otherwise returns false
     */
    boolean getSegs(Integer obj, Integer[] position) {

        double[][] base = getAsStdMatrix((double[])ObjectData.getSegsData(obj), 1.);
        double[][] arrows = getAsStdMatrix((double[])ObjectData.getArrows(obj), 1.);

        double[] pos = {1.0 * position[0], 1.0 * position[1], 1.0};
        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axesUID, pos);
        pos[0] += delta;
        pos[1] += delta;
        double[] c2d2 = CallRenderer.get2dViewFromPixelCoordinates(axesUID, pos);

        double dx = Math.abs(c2d[0] - c2d2[0]);
        double dy = Math.abs(c2d[1] - c2d2[1]);
        //System.out.println("Click: " + c2d[0] + "," + c2d[1]);
        for (int i = 0; i < base.length; i++) {
            double[] ch2d = AxesDrawer.compute2dViewCoordinates(axes, base[i]);
            double[] ch2d2 = AxesDrawer.compute2dViewCoordinates(axes, arrows[i]);
            //System.out.println("P1: " + ch2d[0] + "," + ch2d[1]);
            //System.out.println("P2: " + ch2d2[0] + "," + ch2d2[1]);
            if (isInRange(ch2d[0], ch2d2[0], ch2d[1], ch2d2[1], c2d[0], c2d[1], dx, dy)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Datatip picker, given a datatip object it checks if the click
     * was over it
     *
     * @param obj The given datatip object uid
     * @param position Mouse click position in pixels
     * @return true if picked the datatip otherwise returns false
     */
    boolean getDatatip(Integer obj, Integer[] position) {

        boolean[] logFlags = { axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
        double[] pix_pos = {1.0 * position[0], 1.0 * position[1], 1.0};
        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axesUID, pix_pos);
        pix_pos[0] += 1.0;
        pix_pos[1] += 1.0;
        double[] c2d2 = CallRenderer.get2dViewFromPixelCoordinates(axesUID, pix_pos);
        double dx = Math.abs(c2d[0] - c2d2[0]);
        double dy = Math.abs(c2d[1] - c2d2[1]);

        Double[] tip_pos = (Double[])GraphicController.getController().getProperty(obj, GraphicObjectProperties.__GO_DATATIP_DATA__);
        double point[] = new double[3];
        point[0] = CommonHandler.logScale(tip_pos[0], logFlags[0]);
        point[1] = CommonHandler.logScale(tip_pos[1], logFlags[1]);
        point[2] = CommonHandler.logScale(tip_pos[2], logFlags[2]);

        Double[] rotAngles = (Double[])GraphicController.getController().getProperty(axesUID, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
        boolean default2dView = (rotAngles[0] == 0.0 && rotAngles[1] == 270.0);

        if (!default2dView) {
            point = AxesDrawer.compute2dViewCoordinates(axes, point);
        }

        Integer size = CommonHandler.getMarkSize(obj);
        Integer unit = CommonHandler.getMarkSizeUnit(obj);
        int finalSize = (unit == 1) ? (8 + 2 * size) : size;
        finalSize /= 2;

        if ((Math.abs(point[0] - c2d[0]) <= dx * finalSize) && (Math.abs(point[1] - c2d[1]) <= dy * finalSize)) {
            return true;
        }

        //TODO: Add selection when the click is inside datatip textbox

        return false;
    }

    boolean isInsideAxes(Integer figureUID, Integer[] position) {
        Double[] rotAngles = (Double[])GraphicController.getController().getProperty(axesUID, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
        boolean default2dView = (rotAngles[0] == 0.0 && rotAngles[1] == 270.0);
        //        if (default2dView) {
        Double[] dataBounds = (Double[])GraphicController.getController().getProperty(axesUID, GraphicObjectProperties.__GO_DATA_BOUNDS__);
        double[] c2d = AxesDrawer.compute2dViewFromPixelCoordinates(axes, new double[] { 1.0 * position[0], 1.0 * position[1], 0.0 });
        if (c2d[0] >= dataBounds[0] && c2d[0] <= dataBounds[1] && c2d[1] >= dataBounds[2] && c2d[1] <= dataBounds[3]) {
            return true;
        }
        //      } else {
        //TODO: Add check when it is in 3d view
        //      }
        return false;
    }


    void getObjects(Integer root, List<Integer> putObjs) {

        Integer count = (Integer)GraphicController.getController().getProperty(root, GraphicObjectProperties.__GO_CHILDREN_COUNT__);
        if (count > 0) {
            Integer[] children = (Integer[])GraphicController.getController().getProperty(root, GraphicObjectProperties.__GO_CHILDREN__);
            for (int i = 0; i < count; i++) {
                putObjs.add(children[i]);
                getObjects(children[i], putObjs);
            }
        }
    }

    /**
     * Given an root object return an array with all children objects
     *
     * @param root The root object uid
     * @return Array with all objects
     */
    Integer[] getAllObjAsArray(Integer root) {

        List<Integer> objs = new LinkedList<Integer>();
        getObjects(root, objs);
        return objs.toArray(new Integer[objs.size()]);
    }

    private boolean isInRange(Double x0, Double x1, Double y0, Double y1, Double x, Double y, Double xRange, Double yRange) {
        /* Fast bound check*/
        double m = (x1 + x0) / 2;
        double dx = m - x0;

        double ca = (y1 - y0) / (x1 - x0);

        double yy = y0 + ca * (x - x0);

        double pix = xRange / delta;
        double m_y = (y1 + y0) / 2;
        double dy = m_y - y0;

        boolean ca_inf = (Math.abs(x1 - x0) < Math.abs(pix * 2));
        boolean in_bounds = (Math.abs(m - x) <= Math.abs(pix * 2)) && (Math.abs(m_y - y) <= Math.abs(dy));

        /*
         * test if (x, y) belongs or is closer to the line
         * if the angular coeficent -> inf(ca_inf), the interpolation fails
         * then we use "in_bunds" test.
         */
        return (Math.abs(m - x) <= Math.abs(dx)) && (y >= (yy - yRange)) && (y <= (yy + yRange)) || (ca_inf && in_bounds);
    }

    private double[][] getAsStdMatrix(double[] data, double scale) {

        double[][] ret = null;
        if (data.length % 3 != 0) {
            return ret;
        }
        ret = new double[data.length / 3][3];
        for (int i = 0; i < data.length / 3; i++) {
            for (int j = 0; j < 3; j++) {
                ret[i][j] = data[i * 3 + j] / scale;
            }
        }
        return ret;
    }
}
