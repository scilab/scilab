/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.graphic_objects.SurfaceData;


/**
 * Implements all object common manipulation functions for the editor.
 *
 * @author Caio Souza <caioc2bolado@gmail.com>
 * @author Pedro Souza <bygrandao@gmail.com>
 *
 * @since 2012-07-26
 */
public class CommonHandler {
    /**
     * Checks if the object exists.
     *
     * @param uid object unique identifier.
     * @return True if exists, false otherwise.
     */
    public static Boolean objectExists(Integer uid) {
        if (uid != null) {
            if (GraphicController.getController().getObjectFromId(uid) != null) {
                return true;
            }
        }
        return false;
    }

    /**
     * Checks if the object uses line mode.
     *
     * @param uid object unique identifier.
     * @return True if line mode is enabled, false otherwise.
     */
    public static Boolean isLineEnabled(Integer uid) {
        return (Boolean)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_LINE_MODE__);
    }

    /**
     * Checks if the object uses mark mode.
     *
     * @param uid object unique identifier.
     * @return True if mark mode is enabled, false otherwise.
     */
    public static Boolean isMarkEnabled(Integer uid) {
        return (Boolean)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_MARK_MODE__);
    }

    /**
     * Checks if the object uses mark mode.
     *
     * @param uid object unique identifier.
     * @return the polyline style.
     */
    public static int getStyle(Integer uid) {
        return (Integer) GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_POLYLINE_STYLE__);
    }

    /**
     * Checks if the object uses mark mode.
     *
     * @param uid object unique identifier.
     * @return the bar width.
     */
    public static int getBarWidth(Integer uid) {
        return (Integer) GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_BAR_WIDTH__);
    }

    /**
     * Get the mark style from the given object.
     *
     * @param uid object unique identifier.
     * @return Style number.
     */
    public static Integer getMarkStyle(Integer uid) {
        return (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_MARK_STYLE__);
    }

    /**
     * Get the mark size from the given object.
     *
     * @param uid object unique identifier.
     * @return Mark size.
     */
    public static Integer getMarkSize(Integer uid) {
        return (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_MARK_SIZE__);
    }

    /**
     * Get the mark size from the given object.
     *
     * @param uid object unique identifier.
     * @return Mark size.
     */
    public static Integer getMarkSizeUnit(Integer uid) {
        return (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__);
    }

    /**
     * Change object line/mark color.
     *
     * @param uid object unique identifier.
     * @param newColor Color to be used.
     * @return Returns the old color of the object.
     */
    public static void setSelected(Integer uid, boolean selected) {
        if (uid == null) {
            return;
        }
        GraphicController.getController().setProperty(uid, GraphicObjectProperties.__GO_SELECTED__, selected);
    }

    /**
     * Duplicate the object and its data.
     *
     * @param uid object unique identifier.
     * @return New duplicated object uid or null if it fails.
     */
    public static Integer duplicate(Integer uid) {
        Integer dup = null;
        Integer ret = null;
        dup = GraphicController.getController().cloneObject(uid);
        GraphicController.getController().setGraphicObjectRelationship(0, dup);
        Integer typeName = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_TYPE__);

        if (typeName == GraphicObjectProperties.__GO_POLYLINE__) {
            ret = PolylineData.createPolylineData(uid, dup);
        } else if (typeName == GraphicObjectProperties.__GO_PLOT3D__ ||
                   typeName == GraphicObjectProperties.__GO_FAC3D__  ||
                   typeName == GraphicObjectProperties.__GO_GRAYPLOT__) {
            ret = SurfaceData.createObject3dData(uid, dup, typeName);
        }
        if (ret == null) {
            delete(dup);
            return null;
        }
        return dup;
    }

    /**
     * Inserts the given object in the given axes.
     *
     * @param axes Axes unique identifier.
     * @param uid object unique identifier.
     */
    public static void insert(Integer axes, Integer uid) {
        Integer typeName = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_TYPE__);
        if (typeName == GraphicObjectProperties.__GO_POLYLINE__) {
            Integer newCompound = GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_COMPOUND__));
            GraphicController.getController().setGraphicObjectRelationship(axes, newCompound);
            GraphicController.getController().setGraphicObjectRelationship(newCompound, uid);
        } else if (typeName == GraphicObjectProperties.__GO_PLOT3D__ ||
                   typeName == GraphicObjectProperties.__GO_FAC3D__  ||
                   typeName == GraphicObjectProperties.__GO_GRAYPLOT__) {
            GraphicController.getController().setGraphicObjectRelationship(axes, uid);
        }
    }

    /**
     * Remove the relationship from the given object
     * from its parent.
     *
     * @param uid object unique object.
     */
    public static void cut(Integer uid) {
        GraphicController.getController().setGraphicObjectRelationship(0, uid);
    }

    /**
     * Remove the relationship from the given object
     * from its parent and deletes it.
     *
     * @param uid object unique identifier.
     */
    public static void delete(Integer uid) {
        GraphicController.getController().removeRelationShipAndDelete(uid);
    }

    /**
     * Retrieve if the object is visible.
     *
     * @param uid object unique identifier.
     * @return True if is visible, false otherwise.
     */
    public static Boolean isVisible(Integer uid) {
        return (Boolean)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_VISIBLE__);
    }

    /**
     * Change the visible status of
     * the given object.
     *
     * @param uid object unique identifier.
     * @param status true set object vidible false hide it.
     */
    public static void setVisible(Integer uid, boolean status) {
        GraphicController.getController().setProperty(uid, GraphicObjectProperties.__GO_VISIBLE__, status);
    }

    /**
     * Set all polylines and plot3d's status to visible.
     *
     * @param figure figure unique identifier.
     */
    public static void unhideAll(Integer figure) {
        Integer[] types = {GraphicObjectProperties.__GO_POLYLINE__, GraphicObjectProperties.__GO_PLOT3D__,
                           GraphicObjectProperties.__GO_FAC3D__, GraphicObjectProperties.__GO_GRAYPLOT__
                          };
        Integer[] objs = (new ObjectSearcher()).searchMultiple(figure, types);
        if (objs != null) {
            for (int i = 0; i < objs.length; ++i) {
                setVisible(objs[i], true);
            }
        }
    }

    /**
     * Get The parent UID from an object
     *
     * @param object The object
     * @return the parent UID
     */
    public static Integer getParent(Integer object) {

        return (Integer)GraphicController.getController().getProperty(object, GraphicObjectProperties.__GO_PARENT__);
    }

    /**
     * Get The parent figure UID from an object
     *
     * @param object The object
     * @return the parent igure UID
     */
    public static Integer getParentFigure(Integer object) {
        GraphicObject go = GraphicController.getController().getObjectFromId(object);
        return go.getParentFrameOrFigure();
    }

    /**
     * Check if the color map of the given figures
     * are equals
     * @param figure1 First figure uid.
     * @param figure2 Second figure uid.
     * @return True if equal false otherwise.
     */
    public static boolean cmpColorMap(Integer figure1, Integer figure2) {

        Double[] cm1 = getColorMap(figure1);
        Double[] cm2 = getColorMap(figure2);

        if (cm1.length != cm2.length) {
            return false;
        }
        for (int i = 0; i < cm1.length; ++i) {
            if ((cm1[i] - cm2[i]) != 0) {
                return false;
            }
        }
        return true;
    }

    /**
     * Clone the color map from ane figure to another.
     *
     * @param from Source figure.
     * @param to Destination figure.
     */
    public static void cloneColorMap(Integer from, Integer to) {
        Double[] cm1 = getColorMap(from);
        GraphicController.getController().setProperty(to, GraphicObjectProperties.__GO_COLORMAP__, cm1);

    }

    /**
     * Clone the object background color(if colormap from both figures are different this won't work)
     *
     * @param objectFrom The object to get the backgound color
     * @param objectTo The object to set the background color
     */
    public static void cloneBackgroundColor(Integer objectFrom, Integer objectTo) {

        Integer color = getBackground(objectFrom);
        GraphicController.getController().setProperty(objectTo, GraphicObjectProperties.__GO_BACKGROUND__, color);
    }

    public static Double[] getColorMap(Integer figure) {

        return (Double[])GraphicController.getController().getProperty(figure, GraphicObjectProperties.__GO_COLORMAP__);
    }

    public static Integer getBackground(Integer object) {

        return (Integer)GraphicController.getController().getProperty(object, GraphicObjectProperties.__GO_BACKGROUND__);
    }

    public  static double logScale(Double value, boolean logScale) {

        if (logScale) {
            return Math.log10(value);
        }
        return value;
    }

    public static double InverseLogScale(Double value, boolean logScale) {

        if (logScale) {
            return Math.pow(10., value);
        }
        return value;
    }

    public static double[] toLogScale(double[] data, boolean logScale) {

        if (logScale) {
            double[] temp = new double[data.length];
            for (int i = 0; i < data.length; i++) {
                temp[i] = Math.log10(data[i]);
            }
            return temp;
        }
        return data;
    }

    public static double[] toInverseLogScale(double[] data, boolean logScale) {

        if (logScale) {
            double[] temp = new double[data.length];
            for (int i = 0; i < data.length; i++) {
                temp[i] = Math.pow(10., data[i]);
            }
            return temp;
        }
        return data;
    }

    public static void toLogScale(double[] data, boolean[] logScale) {
        if (logScale[0] || logScale[1] || logScale[2]) {
            for (int i = 0; i < data.length; i += 3) {
                if (logScale[0]) {
                    data[i] = Math.log10(data[i]);
                }
                if (logScale[1]) {
                    data[i + 1] = Math.log10(data[i + 1]);
                }
                if (logScale[2]) {
                    data[i + 2] = Math.log10(data[i + 2]);
                }
            }
        }
    }

    public static void toInverseLogScale(double[] data, boolean[] logScale) {
        if (logScale[0] || logScale[1] || logScale[2]) {
            for (int i = 0; i < data.length; i += 3) {
                if (logScale[0]) {
                    data[i] = Math.pow(10., data[i]);
                }
                if (logScale[1]) {
                    data[i + 1] = Math.pow(10., data[i + 1]);
                }
                if (logScale[2]) {
                    data[i + 2] = Math.pow(10., data[i + 2]);
                }
            }
        }
    }

    /**
     * Used to compute the correct position to insert a point
     * R1: Given index i, it calculates the straight line between the points i and i+1 from curPolyline
     * R2: Given the 2d point it calculates the 3d straight line from this point and current 3d view
     * With this two lines we can compute the closest point to R2 belonging to R1
     * This point is the position to insert the point
     *
     * @param polyline The polyline to find the point
     * @param i The index from the first point of R1
     * @param point2d The 2d point in default view
     * @return The position in 3d coordinates (x, y ,z) and ratio between polyline data and interpoled data ( dx, dy )
     */
    public static double[] computeIntersection(Integer polyline, int i, double[] point2d) {
        double[] datax = (double[])PolylineData.getDataX(polyline);
        double[] datay = (double[])PolylineData.getDataY(polyline);
        double[] dataz = (double[])PolylineData.getDataZ(polyline);
        Integer axes = (Integer)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_PARENT_AXES__);
        boolean[] logFlags = new boolean[] {(Boolean)GraphicController.getController().getProperty(axes, __GO_X_AXIS_LOG_FLAG__),
                                            (Boolean)GraphicController.getController().getProperty(axes, __GO_Y_AXIS_LOG_FLAG__),
                                            (Boolean)GraphicController.getController().getProperty(axes, __GO_Z_AXIS_LOG_FLAG__)
                                           };
        Axes obj = (Axes)GraphicController.getController().getObjectFromId(axes);
        //Points of R1
        double[] c3d1 = AxesDrawer.compute3dViewCoordinates(obj, new double[] { point2d[0], point2d[1], 1.0});
        double[] c3d2 = AxesDrawer.compute3dViewCoordinates(obj, new double[] { point2d[0], point2d[1], 0.0});
        //Points of R2
        double[] p3d1 = {datax[i], datay[i], dataz[i]};
        double[] p3d2 = {datax[i + 1], datay[i + 1], dataz[i + 1]};

        toLogScale(c3d1, logFlags);
        toLogScale(c3d2, logFlags);
        toLogScale(p3d1, logFlags);
        toLogScale(p3d2, logFlags);

        /*
         * Computes the intersection between the lines
         * or the closest point belonging to the curPolyline
         */
        double[] p0, p1, q0, q1, nl;
        p0 = p3d1;
        p1 = minus(p3d2, p3d1);
        q0 = c3d1;
        q1 = minus(c3d2, c3d1);
        nl = cross(q1, cross(p1, q1));
        double u = (dot(nl, q0) - dot(nl, p0)) / dot(nl, p1);
        double[] point = plus(p0, scalar(p1, u));

        toInverseLogScale(point, logFlags);

        return new double[] {point[0], point[1], point[2], u, u};
    }

    private static double dot(double[] p, double[] q) {
        return p[0] * q[0] + p[1] * q[1] + p[2] * q[2];
    }

    private static double[] cross(double[] p, double[] q) {
        return new double[] { p[1] * q[2] - p[2] * q[1], p[2] * q[0] - p[0] * q[2], p[0] * q[1] - p[1] * q[0]};
    }

    private static double[] minus(double[] p, double[] q) {
        return new double[] { p[0] - q[0], p[1] - q[1], p[2] - q[2]};
    }

    private static double[] plus(double[] p, double[] q) {
        return new double[] { p[0] + q[0], p[1] + q[1], p[2] + q[2]};
    }

    private static double[] scalar(double[] p, double v) {
        p[0] *= v;
        p[1] *= v;
        p[2] *= v;
        return p;
    }
}
