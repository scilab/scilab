/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.forge.scirenderer.implementation.g2d.motor;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Shape;
import java.awt.geom.Path2D;
import java.awt.geom.Rectangle2D;
import java.util.HashSet;
import java.util.Set;

import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * @author Calixte DENIZET
 */
public abstract class AbstractDrawable3DObject {

    public static final double PRECISION = 1e-8;

    private static int defaultPrecedence = 0;

    protected final Vector3d[] vertices;
    protected final Color[] colors;
    protected final Color color;

    protected int precedence;
    protected Vector3d v0;
    protected Vector3d v1;
    protected Vector3d v0v1;
    protected double nv0v1;
    protected Vector3d normal;
    protected BoundingBox bbox;

    /**
     * Default constructor
     * @param vertices the vertices
     */
    public AbstractDrawable3DObject(Vector3d[] vertices, Color[] colors) throws InvalidPolygonException {
        if (vertices == null || vertices.length == 0) {
            throw new InvalidPolygonException("Invalid 3D Object: no vertices was given");
        }
        this.vertices = vertices;
        if (colors != null && AbstractDrawable3DObject.isMonochromatic(colors)) {
            this.color = colors[0];
            this.colors = null;
        } else {
            this.colors = colors;
            this.color = null;
        }
        if (isDegenerate()) {
            throw new InvalidPolygonException("Invalid 3D Object: two vertices are the same");
        }
        if (isNanOrInf()) {
            throw new InvalidPolygonException("Invalid 3D Object: contains NaN or Inf coordinates");
        }
        setPrecedence(defaultPrecedence++);
    }

    final Color getColor(final int i) {
        if (colors != null) {
            return colors[i];
        }

        return color;
    }

    final boolean isMonochromatic() {
        return color != null;
    }

    /**
     * Get the bounding box
     * @return the bounding box
     */
    final BoundingBox getBBox() {
        if (bbox == null) {
            bbox = BoundingBox.getBoundingBox(this);
        }

        return bbox;
    }

    /**
     * Test if an array of colors contains only one color or not
     * @param colors the colors array
     * @return true if the array is monochromatic
     */
    public static boolean isMonochromatic(Color[] colors) {
        if (colors != null && colors.length > 0) {
            Color c = colors[0];
            for (int i = 1; i < colors.length; i++) {
                if (!c.equals(colors[i])) {
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * Draw this object on a Graphics2D object
     * @param g2d the Graphics2d object where to draw
     */
    public abstract void draw(Graphics2D g2d);

    /**
     * Reset the default precedence
     */
    public static void resetDefaultPrecedence() {
        defaultPrecedence = 0;
    }

    /**
     * Set the precedence of this object.
     * @param precedence the precedence of this object
     */
    public void setPrecedence(int precedence) {
        this.precedence = precedence;
    }

    /**
     * Get the precedence of this object, i.e. its position in the list of the draw objects.
     * The first object has a precedence of 0, the second has a precedence of 1, ...
     * @param the precedence
     */
    public int getPrecedence() {
        return precedence;
    }

    /**
     * Get the normal vector.
     * If no normal vector has been set then it is calculated in using the cross product of the first two vectors.
     * @return the normal vector.
     */
    public Vector3d getProvidedNormal() {
        return normal;
    }

    /**
     * Get the normal vector.
     * If no normal vector has been set then it is calculated in using the cross product of the first two vectors.
     * @return the normal vector.
     */
    public Vector3d getNormal() {
        if (v0v1 == null) {
            setNormal();
        }

        return v0v1;
    }

    /**
     * Set the normal vector
     */
    protected void setNormal() {
        v0 = vertices[1].minus(vertices[0]);
        if (vertices.length >= 3) {
            v1 = vertices[2].minus(vertices[0]);
            v0v1 = Vector3d.product(v0, v1);
            nv0v1 = v0v1.getNorm();
            v0v1 = v0v1.times(1 / nv0v1);;
        } else {
            v0v1 = new Vector3d(0, 0, 0);
        }
    }

    /**
     * Determinates if the object is contained into a plane
     * @return true if the object is planar
     */
    protected boolean isPlanar() {
        Vector3d n = getNormal();
        if (n.isZero() || vertices.length == 3) {
            return true;
        }

        for (int i = 1; i < vertices.length; i++) {
            if (!isNull(n.scalar(vertices[0].minus(vertices[i])))) {
                return false;
            }
        }

        return true;
    }

    public int isBehind(Vector3d v, double a) {
        double[] mM = minmax3D(this, v);

        if (isPositiveOrNull(mM[0] + a)) {
            return 1;
        }
        if (mM[1] + a < 0) {
            return -1;
        }

        return 0;
    }

    public static boolean isBehind(Vector3d M, Vector3d v, double a) {
        return isPositiveOrNull(M.scalar(v) + a);
    }

    /**
     * Get the projected polyline of this object
     * @return a path 2D
     */
    protected Path2D getProjectedPolyLine() {
        Path2D.Double path = new Path2D.Double();
        path.moveTo(vertices[0].getX(), vertices[0].getY());
        for (int i = 1; i < vertices.length; i++) {
            path.lineTo(vertices[i].getX(), vertices[i].getY());
        }

        return path;
    }

    /**
     * Get the projected contour (i.e. a closed polyline) of this object
     * @return a path 2D
     */
    protected Path2D getProjectedContour() {
        Path2D path = getProjectedPolyLine();
        path.closePath();

        return path;
    }

    /**
     * @param d a number
     * @return true if d is near zero
     */
    protected final static boolean isNull(final double d) {
        return Math.abs(d) <= PRECISION;
    }

    /**
     * @param d a number
     * @return true if d is greater than zero
     */
    protected final static boolean isPositiveOrNull(final double d) {
        return d >= 0 || isNull(d);
    }

    /**
     * @param d a number
     * @return true if d is greater than zero
     */
    protected final static boolean isNegativeOrNull(final double d) {
        return d <= 0 || isNull(d);
    }

    /**
     * @param d1 a number
     * @param d2 a number
     * @return true if d1 is greater than d2
     */
    protected final static boolean isGreaterOrEqual(final double d1, final double d2) {
        return isPositiveOrNull(d1 - d2);
    }

    /**
     * @param d1 a number
     * @param d2 a number
     * @return true if d1 is lower than d2
     */
    protected final static boolean isLowerOrEqual(final double d1, final double d2) {
        return isPositiveOrNull(d2 - d1);
    }

    /**
     * @param d1 a number
     * @param d2 a number
     * @return true if d1 is equal to d2
     */
    protected final static boolean isEqual(final double d1, final double d2) {
        return isNull(d1 - d2);
    }

    /**
     * Get min-max of the projections of the vertices of o on v
     * @param o an object
     * @param v a vector
     * @return an array of size 2 containing min-max.
     */
    protected static final double[] minmax3D(final AbstractDrawable3DObject o, final Vector3d v) {
        double min = v.scalar(o.vertices[0]);
        double max = min;

        for (int i = 1; i < o.vertices.length; i++) {
            double s = v.scalar(o.vertices[i]);
            if (s < min) {
                min = s;
            } else if (s > max) {
                max = s;
            }
        }

        return new double[] {min, max};
    }

    /**
     * Get min-max of the projections of the vertices of o on v
     * @param o an object
     * @param v a vector
     * @return an array of size 2 containing min-max.
     */
    protected static final double[] minmax2D(final AbstractDrawable3DObject o, final double x, final double y) {
        double min = x * o.vertices[0].getX() + y * o.vertices[0].getY();
        double max = min;

        for (int i = 1; i < o.vertices.length; i++) {
            double s = x * o.vertices[i].getX() + y * o.vertices[i].getY();
            if (s < min) {
                min = s;
            } else if (s > max) {
                max = s;
            }
        }

        return new double[] {min, max};
    }

    protected static final Color getColorsBarycenter(final Color c1, final Color c2, final double w1, final double w2) {
        if (c1 != null && c2 != null && !c1.equals(c2)) {
            float[] comp1 = c1.getComponents(null);
            float[] comp2 = c2.getComponents(null);

            return new Color((float) (comp1[0] * w1 + comp2[0] * w2),
                             (float) (comp1[1] * w1 + comp2[1] * w2),
                             (float) (comp1[2] * w1 + comp2[2] * w2),
                             (float) (comp1[3] * w1 + comp2[3] * w2));
        }

        return c1;
    }

    /**
     * @return true if there are two vertices which are indentical
     */
    protected boolean isDegenerate() {
        Set<Vector3d> set = new HashSet<Vector3d>();
        for (Vector3d v : vertices) {
            set.add(v);
        }

        return set.size() != vertices.length;
    }

    protected boolean isNanOrInf() {
        for (Vector3d v : vertices) {
            if (isNanOrInf(v)) {
                return true;
            }
        }

        return false;
    }

    public static final boolean isNanOrInf(final Vector3d v) {
        final double x = v.getX();
        if (Double.isNaN(x) || Double.isInfinite(x)) {
            return true;
        }
        final double y = v.getY();
        if (Double.isNaN(y) || Double.isInfinite(y)) {
            return true;
        }
        final double z = v.getZ();
        if (Double.isNaN(z) || Double.isInfinite(z)) {
            return true;
        }
        return false;
    }

    /**
     * Create a clip rect for 2D view according to clipping planes passed as arguments
     * @param v the clipping plane equation
     */
    public final static void makeClip(final double[] clip, final double[] v) {
        if (v[1] == 0) {
            double x = -v[3] / v[0];
            if (Double.isNaN(clip[0])) {
                clip[0] = x;
            } else if (clip[0] > x) {
                clip[1] = clip[0];
                clip[0] = x;
            } else {
                clip[1] = x;
            }
        } else {
            double y = -v[3] / v[1];
            if (Double.isNaN(clip[2])) {
                clip[2] = y;
            } else if (clip[2] > y) {
                clip[3] = clip[2];
                clip[2] = y;
            } else {
                clip[3] = y;
            }
        }
    }

    /**
     * Get the clipping shape (for 2D view)
     * @return the clipping shape
     */
    public final static Shape getClip(final double[] clip) {
        if (!Double.isNaN(clip[0]) && !Double.isNaN(clip[1]) && !Double.isNaN(clip[2]) && !Double.isNaN(clip[3])) {
            return new Rectangle2D.Double(clip[0], clip[2], clip[1] - clip[0], clip[3] - clip[2]);
        }

        return null;
    }
}
