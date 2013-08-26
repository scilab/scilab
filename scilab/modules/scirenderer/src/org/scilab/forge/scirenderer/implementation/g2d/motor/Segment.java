/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.implementation.g2d.motor;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Stroke;
import java.awt.geom.Path2D;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.HashSet;
import java.util.Set;

import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.tranformations.Vector4d;

/**
 * @author Calixte DENIZET
 */
public class Segment extends ConvexObject implements Comparable<Segment> {

    private Integer hash;
    protected Stroke stroke;
    protected List<ConvexObject> segmentOn;

    public Segment(Vector3d[] vertices, Color[] colors, Stroke stroke) throws InvalidPolygonException {
        super(vertices, colors);
        if (vertices.length != 2) {
            throw new InvalidPolygonException("Invalid segment: must have 2 vertices.");
        }
        this.stroke = stroke;
    }

    public Segment(Vector3d[] vertices, Color[] colors) throws InvalidPolygonException {
        this(vertices, colors, null);
    }

    public void setStroke(Stroke stroke) {
        this.stroke = stroke;
    }

    public double getLength() {
        return vertices[0].minus(vertices[1]).getNorm();
    }

    public static double getLength(Vector3d[] vertices) {
        return vertices[0].minus(vertices[1]).getNorm();
    }

    public void addConvexObject(ConvexObject co) {
        if (segmentOn == null) {
            segmentOn = new ArrayList<ConvexObject>(2);
        }
        segmentOn.add(co);
    }

    public void removeConvexObject(ConvexObject co) {
        if (segmentOn != null) {
            segmentOn.remove(co);
        }
    }

    public void replaceSegment(List<Segment> segs) {
        if (segmentOn != null) {
            for (ConvexObject co : segmentOn) {
                Triangle t = (Triangle) co;
                t.replaceSegment(this, segs);
            }
        }
    }

    public boolean isIn2D() {
        return isNull(vertices[0].getZ()) && isNull(vertices[1].getZ());
    }

    public boolean isInFront() {
        return isEqual(vertices[0].getZ(), -0.5) && isEqual(vertices[1].getZ(), -0.5);
    }

    @Override
    public int compareTo(Segment o) {
        if (equals(o)) {
            return 0;
        }

        return getPrecedence() - o.getPrecedence();
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Segment) {
            Segment s = (Segment) o;
            return (s.vertices[0].equals(vertices[0]) && s.vertices[1].equals(vertices[1]) && s.colors[0].equals(colors[0]) && s.colors[1].equals(colors[1])) || (s.vertices[1].equals(vertices[0]) && s.vertices[0].equals(vertices[1]) && s.colors[1].equals(colors[0]) && s.colors[0].equals(colors[1]));
        }

        return false;
    }

    @Override
    public int isBehind(ConvexObject o) {
        if (o instanceof Triangle && ((Triangle) o).isSegmentAcross(this)) {
            return 1;
        }

        return super.isBehind(o);
    }

    @Override
    public List<ConvexObject> breakObject(ConvexObject o) {
        if (o instanceof Triangle) {
            return ((Triangle) o).breakObject(this);
        } else if (o instanceof SpritedRectangle) {
            return ((SpritedRectangle) o).breakObject(this);
        }

        return null;
    }

    @Override
    public List<ConvexObject> breakObject(Vector4d v) {
        double[] vv = v.getData();
        Vector3d np = new Vector3d(vv);
        boolean a = isBehind(vertices[0], np, vv[3]);
        boolean b = isBehind(vertices[1], np, vv[3]);

        if (a && b) {
            List<ConvexObject> list = new ArrayList<ConvexObject>(1);
            list.add(this);
            return list;
        }

        if (!a && !b) {
            return null;
        }

        double c = (vv[3] + vertices[1].scalar(np)) / v0.scalar(np);
        Vector3d p = Vector3d.getBarycenter(vertices[0], vertices[1], c, 1 - c);
        Color color = getColorsBarycenter(colors[0], colors[1], c, 1 - c);
        Vector3d[] vs = null;
        Color[] cs = null;

        if (a) {
            vs = new Vector3d[] {vertices[0], p};
            cs = new Color[] {colors[0], color};
        }

        if (b) {
            vs = new Vector3d[] {p, vertices[1]};
            cs = new Color[] {color, colors[1]};
        }

        try {
            List<ConvexObject> list = new ArrayList<ConvexObject>(1);
            list.add(new Segment(vs, cs, this.stroke));

            return list;
        } catch (InvalidPolygonException e) { }

        return null;
    }

    public List<Segment> breakObject(Vector3d p, Vector3d u, Vector3d n) {
        double c = vertices[1].minus(p).scalar(n) / v0.scalar(n);
        if (c > 0 && !isNull(c) && c < 1 && !isEqual(c, 1)) {
            List<Segment> list = new ArrayList<Segment>(2);
            Vector3d q = Vector3d.getBarycenter(vertices[0], vertices[1], c, 1 - c);
            Color color = getColorsBarycenter(colors[0], colors[1], c, 1 - c);
            try {
                list.add(new Segment(new Vector3d[] {vertices[0], q}, new Color[] {colors[0], color}, stroke));
                list.add(new Segment(new Vector3d[] {q, vertices[1]}, new Color[] {color, colors[1]}, stroke));

                return list;
            } catch (InvalidPolygonException e) { }
        } else {
            List<Segment> list = new ArrayList<Segment>(1);
            try {
                list.add(new Segment(new Vector3d[] {vertices[0], vertices[1]}, new Color[] {colors[0], colors[1]}, stroke));

                return list;
            } catch (InvalidPolygonException e) { }
        }

        return null;
    }

    @Override
    public void draw(Graphics2D g2d) {
        if (segmentOn == null || segmentOn.isEmpty()) {
            Path2D polyline = getProjectedPolyLine();
            g2d.setColor(colors[0]);
            Stroke oldStroke = g2d.getStroke();
            if (oldStroke != stroke) {
                g2d.setStroke(stroke);
            }

            g2d.draw(polyline);

            if (oldStroke != stroke) {
                g2d.setStroke(oldStroke);
            }

            drawAreas(g2d);
        }
    }

    @Override
    public int hashCode() {
        if (hash == null) {
            hash = Arrays.hashCode(vertices) + 19 * Arrays.hashCode(colors);
        }
        return hash;
    }

    @Override
    public String toString() {
        return "Segment " + vertices[0].toString() + " " + vertices[1].toString() + " Precedence: " + getPrecedence();
    }
}
