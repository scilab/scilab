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
import java.awt.Stroke;
import java.awt.geom.Path2D;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.tranformations.Vector4d;

/**
 * Segment object (for info, when modify rendering check for PolyLine too).
 *
 * @author Calixte DENIZET
 */
public class Segment extends ConvexObject implements Comparable<Segment> {

    private int hash = -1;
    protected G2DStroke stroke;
    protected List<ConvexObject> segmentOn;
    protected boolean is2D;
    protected double[] clip = new double[] {Double.NaN, Double.NaN, Double.NaN, Double.NaN};

    public Segment(Vector3d[] vertices, Color[] colors, G2DStroke stroke, boolean is2D) throws InvalidPolygonException {
        super(vertices, colors);
        if (vertices.length != 2) {
            throw new InvalidPolygonException("Invalid segment: must have 2 vertices.");
        }
        this.stroke = stroke;
        this.is2D = is2D;
    }

    public Segment(Vector3d[] vertices, Color[] colors) throws InvalidPolygonException {
        this(vertices, colors, null, false);
    }

    public void setStroke(G2DStroke stroke) {
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
            return (s.vertices[0].equals(vertices[0]) && s.vertices[1].equals(vertices[1]) && s.getColor(0).equals(getColor(0)) && s.getColor(1).equals(getColor(1))) || (s.vertices[1].equals(vertices[0]) && s.vertices[0].equals(vertices[1]) && s.getColor(1).equals(getColor(0)) && s.getColor(0).equals(getColor(1)));
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

        if (is2D && vv[2] == 0) {
            ConvexObject.makeClip(clip, vv);
        }
        getNormal();
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
        Color color = getColorsBarycenter(getColor(0), getColor(1), c, 1 - c);
        Segment s;

        try {
            if (a) {
                s = new Segment(new Vector3d[] {vertices[0], p}, new Color[] {getColor(0), color}, this.stroke, this.is2D);
            } else {
                s = new Segment(new Vector3d[] {p, vertices[1]}, new Color[] {color, getColor(1)}, this.stroke, this.is2D);
            }

            List<ConvexObject> list = new ArrayList<ConvexObject>(1);
            list.add(s);

            return list;
        } catch (InvalidPolygonException e) { }

        return null;
    }

    public List<Segment> breakObject(Vector3d p, Vector3d u, Vector3d n) {
        getNormal();
        double c = vertices[1].minus(p).scalar(n) / v0.scalar(n);
        if (c > 0 && !isNull(c) && c < 1 && !isEqual(c, 1)) {
            List<Segment> list = new ArrayList<Segment>(2);
            Vector3d q = Vector3d.getBarycenter(vertices[0], vertices[1], c, 1 - c);
            Color color = getColorsBarycenter(getColor(0), getColor(1), c, 1 - c);
            try {
                list.add(new Segment(new Vector3d[] {vertices[0], q}, new Color[] {getColor(0), color}, stroke, this.is2D));
                list.add(new Segment(new Vector3d[] {q, vertices[1]}, new Color[] {color, getColor(1)}, stroke, this.is2D));

                return list;
            } catch (InvalidPolygonException e) { }
        } else {
            List<Segment> list = new ArrayList<Segment>(1);
            try {
                list.add(new Segment(new Vector3d[] {vertices[0], vertices[1]}, new Color[] {getColor(0), getColor(1)}, stroke, this.is2D));

                return list;
            } catch (InvalidPolygonException e) { }
        }

        return null;
    }

    @Override
    public void draw(Graphics2D g2d) {
        if (segmentOn == null || segmentOn.isEmpty()) {
            Path2D polyline = getProjectedPolyLine();
            g2d.setColor(getColor(0));
            Stroke oldStroke = g2d.getStroke();
            if (oldStroke != stroke) {
                g2d.setStroke(stroke);
            }

            Shape oldClip = g2d.getClip();
            Shape newClip = ConvexObject.getClip(clip);
            if (newClip != null) {
                g2d.clip(newClip);
            }

            g2d.draw(polyline);

            if (oldStroke != stroke) {
                g2d.setStroke(oldStroke);
            }

            if (newClip != null) {
                g2d.setClip(oldClip);
            }

            drawAreas(g2d);
        }
    }

    @Override
    public int hashCode() {
        if (hash == -1) {
            if (colors != null) {
                hash = Arrays.hashCode(vertices) + 19 * Arrays.hashCode(colors);
            } else {
                hash = Arrays.hashCode(vertices) + 19 * getColor(0).hashCode();
            }
        }
        return hash;
    }

    @Override
    public String toString() {
        return "Segment " + vertices[0].toString() + " " + vertices[1].toString() + " Precedence: " + getPrecedence();
    }
}
