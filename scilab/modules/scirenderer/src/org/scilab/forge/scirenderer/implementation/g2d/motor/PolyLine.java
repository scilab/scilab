/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte Denizet
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
import java.awt.Shape;
import java.awt.Stroke;
import java.awt.geom.Line2D;
import java.awt.geom.Path2D;
import java.awt.Graphics2D;
import java.util.ArrayList;
import java.util.List;

import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.tranformations.Vector4d;

/**
 * Class to represent a polyline object. This is used only in 2D (in 3D, a line could begin
 * on front of a face and could end behind it).
 * This allows to draw a polyline like a polyline ! and not as a set of segments.
 * Notice that a PolyLine is NOT a convex object... but in 2D it does not matter, algorithms
 * which use the convexity are not applyed.
 *
 * @author Calixte DENIZET
 */
public class PolyLine extends ConvexObject {

    private boolean monochromatic;
    private G2DStroke stroke;
    protected double[] clip = new double[] {Double.NaN, Double.NaN, Double.NaN, Double.NaN};

    /**
     * Default constructor
     * @param vertices the polyline vertices
     * @param colors the vertices color
     * @param stroke the stroke to used
     */
    public PolyLine(Vector3d[] vertices, Color[] colors, G2DStroke stroke) throws InvalidPolygonException {
        super(vertices, colors);
        if (vertices.length <= 1) {
            throw new InvalidPolygonException("A polyline cannot have one or zero point");
        }

        this.monochromatic = isMonochromatic(colors);
        this.stroke = stroke;
    }

    /**
     * Get a set of polylines. The Nan of Inf vectors are removed and so the polyline is splitted.
     * @param vertices the polyline vertices
     * @param colors the vertices color
     * @param stroke the stroke to used
     * @param loop if true a looping polyline is created
     * @return a set of polylines
     */
    public static List<PolyLine> getPolyLines(Vector3d[] vertices, Color[] colors, G2DStroke stroke, boolean loop) {
        if (loop) {
            Vector3d[] v = new Vector3d[vertices.length + 1];
            Color[] c = new Color[vertices.length + 1];
            for (int i = 0; i < vertices.length; i++) {
                v[i] = vertices[i];
                c[i] = colors[i];
            }
            v[vertices.length] = v[0];
            c[vertices.length] = c[0];
            vertices = v;
            colors = c;
        }

        int pos = 0;
        List<PolyLine> list = new ArrayList<PolyLine>(1);
        while ((pos = trimLeft(vertices, pos)) != -1) {
            final int second = findNanOrInf(vertices, pos + 1);
            final int len = second - pos;
            final Vector3d[] newVertices = new Vector3d[len];
            final Color[] newColors = new Color[len];
            for (int i = 0; i < len; i++) {
                newVertices[i] = vertices[pos + i];
                newColors[i] = colors[pos + i];
            }
            pos = second + 1;
            try {
                list.add(new PolyLine(newVertices, newColors, stroke));
            } catch (InvalidPolygonException e) { }
        }

        return list;
    }

    /**
     * {@inheritDoc}
     */
    public List<ConvexObject> breakObject(Vector4d v) {
        final double[] vv = v.getData();
        final List<ConvexObject> list = new ArrayList<ConvexObject>(1);

        // Since PolyLine are only used in 2D it is useless to check when z != 0
        if (vv[2] == 0) {
            final Vector3d np = new Vector3d(vv);
            ConvexObject.makeClip(clip, vv);

            int pos = 0;
            boolean prev = false;

            for (int i = 0; i < vertices.length; i++) {
                final boolean b = isBehind(vertices[i], np, vv[3]);
                if (b && !prev) {
                    pos = i;
                    prev = true;
                } else if (!b && prev) {
                    prev = false;
                    try {
                        list.add(cut(pos, i, np, vv[3]));
                    } catch (InvalidPolygonException e) { }
                }
            }

            if (prev) {
                try {
                    list.add(cut(pos, vertices.length, np, vv[3]));
                } catch (InvalidPolygonException e) { }
            }
        } else {
            list.add(this);
        }

        return list;
    }

    /**
     * {@inheritDoc}
     */
    public List<ConvexObject> breakObject(ConvexObject o) {
        return null;
    }

    /**
     * {@inheritDoc}
     */
    protected boolean isDegenerate() {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    protected boolean isNanOrInf() {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    public Vector3d getNormal() {
        // Never used
        return null;
    }

    /**
     * {@inheritDoc}
     */
    protected boolean isPlanar() {
        // Never used
        return true;
    }

    /**
     * Cut a polyline on a clipping plane
     * @param first the first vertex position
     * @param second the second vertex position
     * @param np the normal vector of the clipping plane
     * @param C the constant part of the hyperplane
     * @return a cutted PolyLine
     */
    private PolyLine cut(int first, int second, Vector3d np, double C) throws InvalidPolygonException {
        final boolean cutAtBegin = first != 0;
        final boolean cutAtEnd = second != vertices.length;

        if (!cutAtBegin && !cutAtEnd) {
            return this;
        }

        if (cutAtBegin && cutAtEnd) {
            final int len = second - first + 2;

            Vector3d[] newVertices = new Vector3d[len];
            Color[] newColors = new Color[len];

            for (int i = 1; i < len - 1; i++) {
                newVertices[i] = vertices[first + i - 1];
                newColors[i] = getColor(first + i - 1);
            }

            double c = (C + vertices[first].scalar(np)) / vertices[first].minus(vertices[first - 1]).scalar(np);
            newVertices[0] = Vector3d.getBarycenter(vertices[first - 1], vertices[first], c, 1 - c);
            newColors[0] = getColorsBarycenter(getColor(first - 1), getColor(first), c, 1 - c);

            c = (C + vertices[second].scalar(np)) / vertices[second].minus(vertices[second - 1]).scalar(np);
            newVertices[len - 1] = Vector3d.getBarycenter(vertices[second - 1], vertices[second], c, 1 - c);
            newColors[len - 1] = getColorsBarycenter(getColor(second - 1), getColor(second), c, 1 - c);

            return new PolyLine(newVertices, newColors, this.stroke);
        }

        if (cutAtBegin) {
            final double c = (C + vertices[first].scalar(np)) / vertices[first].minus(vertices[first - 1]).scalar(np);
            final int len = second - first + 1;

            Vector3d[] newVertices = new Vector3d[len];
            Color[] newColors = new Color[len];

            for (int i = 1; i < len; i++) {
                newVertices[i] = vertices[first + i - 1];
                newColors[i] = getColor(first + i - 1);
            }

            newVertices[0] = Vector3d.getBarycenter(vertices[first - 1], vertices[first], c, 1 - c);
            newColors[0] = getColorsBarycenter(getColor(first - 1), getColor(first), c, 1 - c);

            return new PolyLine(newVertices, newColors, this.stroke);
        } else {
            final double c = (C + vertices[second].scalar(np)) / vertices[second].minus(vertices[second - 1]).scalar(np);
            final int len = second - first + 1;

            Vector3d[] newVertices = new Vector3d[len];
            Color[] newColors = new Color[len];

            for (int i = 0; i < len - 1; i++) {
                newVertices[i] = vertices[first + i];
                newColors[i] = getColor(first + i);
            }

            newVertices[len - 1] = Vector3d.getBarycenter(vertices[second - 1], vertices[second], c, 1 - c);
            newColors[len - 1] = getColorsBarycenter(getColor(second - 1), getColor(second), c, 1 - c);
            return new PolyLine(newVertices, newColors, this.stroke);
        }
    }

    @Override
    public void draw(Graphics2D g2d) {
        Stroke oldStroke = g2d.getStroke();
        Shape oldClip = g2d.getClip();

        Shape newClip = ConvexObject.getClip(clip);
        if (newClip != null) {
            g2d.clip(newClip);
        }

        if (monochromatic) {
            g2d.setColor(getColor(0));
            g2d.setStroke(stroke);
            g2d.draw(getProjectedPolyLine());
        } else {
            // on peut surement faire mieux ici
            // avec un LinearGradientPaint
            Vector3d start = vertices[0];
            Color color = getColor(0);
            double cumLen = 0;
            float[] dashArray = stroke.getDashArray();
            float lwidth = stroke.getLineWidth();
            for (int i = 1; i < vertices.length; i++) {
                Stroke nstroke = new G2DStroke(lwidth, dashArray, (float) cumLen);
                g2d.setStroke(nstroke);
                g2d.setColor(color);
                g2d.draw(new Line2D.Double(start.getX(), start.getY(), vertices[i].getX(), vertices[i].getY()));
                cumLen += Math.hypot(start.getX() - vertices[i].getX(), start.getY() - vertices[i].getY());
            }
        }

        if (newClip != null) {
            g2d.setClip(oldClip);
        }

        g2d.setStroke(oldStroke);
    }

    /**
     * Get first non Nan (or Inf) vector position
     * @param v the array to trim
     * @param start the starting position
     * @return index of the first non Nan vector or -1 if not found
     */
    private static int trimLeft(Vector3d[] v, int start) {
        for (int i = start; i < v.length; i++) {
            if (!isNanOrInf(v[i])) {
                return i;
            }
        }

        return -1;
    }

    /**
     * Get first Nan (or Inf) vector position
     * @param v the array to trim
     * @param start the starting position
     * @return index of the first Nan vector
     */
    private static int findNanOrInf(Vector3d[] v, int start) {
        for (int i = start; i < v.length; i++) {
            if (isNanOrInf(v[i])) {
                return i;
            }
        }

        return v.length;
    }
}
