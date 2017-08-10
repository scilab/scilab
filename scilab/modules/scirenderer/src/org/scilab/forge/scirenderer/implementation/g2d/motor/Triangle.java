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

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.Shape;
import java.awt.Stroke;
import java.awt.geom.Area;
import java.awt.geom.Path2D;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;

import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.tranformations.Vector4d;

/**
 * @author Calixte DENIZET
 */
public class Triangle extends ConvexObject {

    private static final Color[] COLORS = new Color[] {Color.BLACK, Color.BLACK, Color.BLACK};
    private static final Stroke stroke = new BasicStroke(1f, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL);
    private static final Stroke EMPTYSTROKE = new BasicStroke(0f, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL);

    private SpritedRectangle sprite;
    private BufferedImage image;
    private Vector3d[] textureCoords;
    private Texture.Filter filter;

    protected List<Segment> segments;

    public Triangle(Vector3d[] vertices, Color[] colors, Vector3d normal) throws InvalidPolygonException {
        super(vertices, colors);
        if (vertices.length != 3) {
            throw new InvalidPolygonException("Invalid triangle: must have 3 vertices.");
        }
    }

    public Triangle(Vector3d[] vertices, Color[] colors) throws InvalidPolygonException {
        this(vertices, colors, null);
    }

    public Triangle(Vector3d[] vertices, Vector3d[] textureCoords, BufferedImage image, Texture.Filter filter) throws InvalidPolygonException {
        this(vertices, COLORS, null);
        this.textureCoords = textureCoords;
        this.image = image;
        this.filter = filter;
    }

    @Override
    public int isBehind(ConvexObject o) {
        if (o instanceof Segment && isSegmentAcross((Segment) o)) {
            return -1;
        }

        return super.isBehind(o);
    }

    public boolean isIn2D() {
        return vertices[0].getZ() == 0 && vertices[1].getZ() == 0 && vertices[2].getZ() == 0;
    }

    public boolean addSegment(Segment s) {
        if (isSegmentInside(s)) {
            if (segments == null) {
                segments = new ArrayList<Segment>(3);
            }
            if (segments.contains(s)) {
                segments.remove(s);
                s.removeConvexObject(this);
            }
            segments.add(s);
            s.addConvexObject(this);

            return true;
        }

        return false;
    }

    public boolean pointOnVertices(Vector3d p) {
        return p.equals(vertices[0]) || p.equals(vertices[1]) || p.equals(vertices[2]);
    }

    public void removeSegment(Segment s) {
        if (segments != null) {
            segments.remove(s);
            s.removeConvexObject(this);
        }
    }

    public void replaceSegment(Segment s, List<Segment> segs) {
        segments.remove(s);
        //s.removeConvexObject(this);
        for (Segment ss : segs) {
            if (segments.contains(ss)) {
                segments.remove(ss);
                ss.removeConvexObject(this);
            }
            segments.add(ss);
            ss.addConvexObject(this);
        }
    }

    @Override
    public List<ConvexObject> breakObject(ConvexObject o) {
        if (o instanceof Triangle) {
            return breakObject((Triangle) o);
        } else if (o instanceof Segment) {
            return breakObject((Segment) o);
        } else if (o instanceof SpritedRectangle) {
            return ((SpritedRectangle) o).breakObject(this);
        }

        return null;
    }

    public List<ConvexObject> breakObject(Triangle o) {
        Vector3d n = Vector3d.product(getNormal(), o.getNormal());
        n = n.times(1 / n.getNorm2());
        Vector3d u = Vector3d.product(o.v0v1, n);
        Vector3d v = Vector3d.product(n, this.v0v1);
        Vector3d p = Vector3d.getBarycenter(u, v, this.v0v1.scalar(this.vertices[0]), o.v0v1.scalar(o.vertices[0]));

        List<ConvexObject> list1 = breakTriangleOnLine(o, p, this.v0v1);
        List<ConvexObject> list2 = breakTriangleOnLine(this, p, o.v0v1);

        list1.addAll(list2);

        return list1;
    }

    public List<ConvexObject> breakObject(Segment o) {
        double c = this.getSegmentIntersection(o);
        if (Double.isNaN(c)) {
            return null;
        }

        List<ConvexObject> list = new ArrayList<ConvexObject>(5);
        Vector3d p = Vector3d.getBarycenter(o.vertices[0], o.vertices[1], c, 1 - c);
        Color cc = getColorsBarycenter(o.getColor(0), o.getColor(1), c, 1 - c);

        try {
            list.add(new Segment(new Vector3d[] {o.vertices[0], p}, new Color[] {o.getColor(0), cc}, o.stroke, o.is2D));
            list.add(new Segment(new Vector3d[] {p, o.vertices[1]}, new Color[] {cc, o.getColor(1)}, o.stroke, o.is2D));
        } catch (InvalidPolygonException e) { }

        List<ConvexObject> list1 = breakTriangleOnLine(this, p, Vector3d.product(getNormal(), vertices[0].minus(p)));
        list.addAll(list1);

        return list;
    }

    protected void setSprite(SpritedRectangle sprite) {
        this.sprite = sprite;
    }

    protected SpritedRectangle getSprite() {
        return sprite;
    }

    @Override
    public void draw(Graphics2D g2d) {
        if (sprite != null) {
            Shape oldClip = g2d.getClip();
            Path2D contour = getProjectedContour();
            Area area = new Area(contour);
            // Trick to paint the triangle and its outline
            //area.add(new Area(stroke.createStrokedShape(contour)));
            g2d.clip(area);//getProjectedContour());
            sprite.draw(g2d);
            g2d.setClip(oldClip);
        } else if (image != null) {
            Object key = filter == Texture.Filter.LINEAR ? RenderingHints.VALUE_INTERPOLATION_BILINEAR : RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR;
            DrawTools.drawTriangleTexture(g2d, image, new double[] {textureCoords[0].getX(), textureCoords[1].getX(), textureCoords[2].getX()}, new double[] {textureCoords[0].getY(), textureCoords[1].getY(), textureCoords[2].getY()}, new double[] {vertices[0].getX(), vertices[1].getX(), vertices[2].getX()}, new double[] {vertices[0].getY(), vertices[1].getY(), vertices[2].getY()}, key);
        } else if (isMonochromatic()) {
            // AA generates artifacts on the triangles borders whent they slightlyt overlap
            // So we disable it just ot fill the triangle
            Object prevAA = g2d.getRenderingHint(RenderingHints.KEY_ANTIALIASING);
            g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
            Path2D contour = getProjectedContour();
            //Area area = new Area(contour);
            // Trick to paint the triangle and its outline
            // TODO: the newly created Area contains in fact two areas
            // it should be better to have one area where its border
            // is the external outline of the contour...
            // (it would reduce eps/ps/pdf/svg file size)
            // fill(area) is very very slow... (for a grayplot 500x500, fill(area) is 10 times slower than fill(contour))...
            //area.add(new Area(stroke.createStrokedShape(contour)));
            if (g2d.getStroke() != EMPTYSTROKE) {
                g2d.setStroke(EMPTYSTROKE);
            }
            g2d.setColor(getColor(0));
            g2d.fill(contour);
            g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, prevAA);
        } else {
            DrawTools.fillGouraud(g2d, this);
        }

        if (segments != null) {
            for (Segment s : segments) {
                s.removeConvexObject(this);
                s.draw(g2d);
            }
        }

        drawAreas(g2d);
    }

    @Override
    public List<ConvexObject> breakObject(Vector4d v) {
        double[] vv = v.getData();
        Vector3d np = new Vector3d(vv);
        int ret = isBehind(np, vv[3]);
        if (ret == 1) {
            List<ConvexObject> list = new ArrayList<ConvexObject>(1);
            list.add(this);
            return list;
        } else if (ret == -1) {
            return null;
        }

        Vector3d n = Vector3d.product(getNormal(), np);
        n = n.times(1 / n.getNorm2());
        Vector3d u = Vector3d.product(np, n);
        Vector3d w = Vector3d.product(n, this.v0v1);
        Vector3d p = Vector3d.getBarycenter(u, w, this.v0v1.scalar(this.vertices[0]), -vv[3]);

        List<ConvexObject> list1 = breakTriangleOnLine(this, p, np);
        List<ConvexObject> list = new ArrayList<ConvexObject>(3);
        for (ConvexObject o : list1) {
            if (o.isBehind(np, vv[3]) == 1) {
                list.add(o);
            }
        }

        return list;
    }

    protected boolean isPointInside(final Vector3d v) {
        return isPointInside(v, true);
    }

    protected boolean isPointInside(final Vector3d v, final boolean checkCoplanarity) {
        Vector3d v2 = v.minus(vertices[0]);
        getNormal();
        if (checkCoplanarity && !isNull(v2.scalar(v0v1))) {
            return false;
        }

        Vector3d v0v1v2 = Vector3d.product(v0v1, v2);
        double a = -v0v1v2.scalar(v1);
        if (a < 0 || isNull(a)) {
            return false;
        }

        double b = v0v1v2.scalar(v0);
        if (b < 0 || isNull(b)) {
            return false;
        }

        return a + b < nv0v1 || isEqual(a + b, nv0v1);
    }

    protected boolean isCoplanar(final Segment s) {
        double sc = vertices[0].scalar(getNormal());
        return isEqual(sc, s.vertices[0].scalar(v0v1)) && isEqual(sc, s.vertices[1].scalar(v0v1));
    }

    protected boolean isCoplanar(final Triangle t) {
        double sc = vertices[0].scalar(getNormal());
        return isEqual(sc, t.vertices[0].scalar(v0v1)) && isEqual(sc, t.vertices[1].scalar(v0v1)) && isEqual(sc, t.vertices[2].scalar(v0v1));
    }

    protected boolean isSegmentAcross(final Segment s) {
        if (!isCoplanar(s)) {
            // the segment and the triangle are not coplanar
            return false;
        }

        return check2DTrueIntersection(s);
    }

    protected boolean isSegmentInside(final Segment s) {
        if (!isCoplanar(s)) {
            // the segment and the triangle are not coplanar
            return false;
        }

        // Since there is a good probability that a segment is triangle border we check that
        if ((s.vertices[0].equals(vertices[0]) || s.vertices[0].equals(vertices[1]) || s.vertices[0].equals(vertices[2]))
                && (s.vertices[1].equals(vertices[0]) || s.vertices[1].equals(vertices[1]) || s.vertices[1].equals(vertices[2]))) {
            return true;
        }

        return isPointInside(s.vertices[0], false) || isPointInside(s.vertices[1], false) || check2DIntersection(s);
    }

    protected boolean isSegmentIntersects(final Segment s) {
        Vector3d v3 = s.vertices[0].minus(vertices[0]);
        Vector3d v4 = s.vertices[1].minus(vertices[0]);
        double c = v3.scalar(getNormal());

        if (Math.signum(c) == Math.signum(v4.scalar(v0v1))) {
            return false;
        }

        Vector3d v2 = s.vertices[0].minus(s.vertices[1]);
        Vector3d v1v2 = Vector3d.product(v1, v2);
        double a = v3.scalar(v1v2);
        double b = Vector3d.det(v3, v2, v0);
        double detv0v1v2 = v0.scalar(v1v2);
        double sign = Math.signum(detv0v1v2);

        return Math.signum(a) == sign && Math.signum(b) == sign && Math.signum(c) == sign && a + b <= detv0v1v2 && c <= detv0v1v2;
    }

    protected double getSegmentIntersection(final Segment s) {
        Vector3d v = s.vertices[1].minus(vertices[0]);
        double c = v.scalar(getNormal()) / s.v0.scalar(getNormal());

        if (isNegativeOrNull(c) || isGreaterOrEqual(c, 1)) {
            return Double.NaN;
        }

        Vector3d p = Vector3d.getBarycenter(s.vertices[0], s.vertices[1], c, 1 - c);
        if (isPointInside(p, false)) {
            return c;
        }

        return Double.NaN;
    }

    protected static List<ConvexObject> breakSegmentOnTriangle(final Triangle t, final Segment s) {
        double c = t.getSegmentIntersection(s);
        if (Double.isNaN(c)) {
            return null;
        }

        List<ConvexObject> list = new ArrayList<ConvexObject>(2);
        Vector3d p = Vector3d.getBarycenter(s.vertices[0], s.vertices[1], c, 1 - c);
        Color cc = getColorsBarycenter(s.getColor(0), s.getColor(1), c, 1 - c);

        try {
            list.add(new Segment(new Vector3d[] {s.vertices[0], p}, new Color[] {s.getColor(0), cc}, s.stroke, s.is2D));
            list.add(new Segment(new Vector3d[] {p, s.vertices[1]}, new Color[] {cc, s.getColor(1)}, s.stroke, s.is2D));
        } catch (InvalidPolygonException e) { }


        return list;
    }

    /**
     * Break a triangle according to its intersection with a line containing p in the plane of the triangle and orthogonal to n
     * The triangle and the line are supposed to be coplanar.
     * @param t the triangle to break
     * @param p a point of the line
     * @param n a vector
     * @return a list of triangles
     */
    protected static List<ConvexObject> breakTriangleOnLine(Triangle t, Vector3d p, Vector3d n) {
        t.getNormal();
        // aP0+(1-a)P1
        double a = t.vertices[1].minus(p).scalar(n) / t.v0.scalar(n);
        // bP0+(1-b)P2
        double b = t.vertices[2].minus(p).scalar(n) / t.v1.scalar(n);
        // cP1+(1-c)P2
        Vector3d v2 = t.vertices[2].minus(t.vertices[1]);
        double c = t.vertices[2].minus(p).scalar(n) / v2.scalar(n);

        List<ConvexObject> list = new ArrayList<ConvexObject>(3);
        int i = -1;
        int j = -1;
        int k = -1;
        double weight = -1;
        if (isNull(a)) {
            // We are on P1
            i = 1;
            j = 2;
            k = 0;
            weight = b;
        }

        if (isNull(a - 1)) {
            // We are on P0
            if (weight != -1) {
                list.add(t);
                return list;
            } else {
                i = 0;
                j = 1;
                k = 2;
                weight = c;
            }
        }

        if (isNull(b)) {
            // We are on P2
            if (weight != -1) {
                list.add(t);
                return list;
            } else {
                i = 2;
                j = 0;
                k = 1;
                weight = a;
            }
        }

        if (i != -1) {
            if (weight >= 0 && weight <= 1) {
                // We break into two triangles
                Vector3d vb = Vector3d.getBarycenter(t.vertices[j], t.vertices[k], weight, 1 - weight);
                Color cb = getColorsBarycenter(t.getColor(j), t.getColor(k), weight, 1 - weight);
                Vector3d[] vertices1 = new Vector3d[] {t.vertices[i], t.vertices[j], vb};
                Vector3d[] vertices2 = new Vector3d[] {t.vertices[i], vb, t.vertices[k]};
                Color[] colors1 = new Color[] {t.getColor(i), t.getColor(j), cb};
                Color[] colors2 = new Color[] {t.getColor(i), cb, t.getColor(k)};

                Vector3d[] tvertices1 = null;
                Vector3d[] tvertices2 = null;
                if (t.textureCoords != null) {
                    Vector3d tvb = Vector3d.getBarycenter(t.textureCoords[j], t.textureCoords[k], weight, 1 - weight);
                    tvertices1 = new Vector3d[] {t.textureCoords[i], t.textureCoords[j], tvb};
                    tvertices2 = new Vector3d[] {t.textureCoords[i], tvb, t.textureCoords[k]};
                }

                try {
                    Triangle t1 = new Triangle(vertices1, colors1, null);
                    t1.setSprite(t.getSprite());
                    list.add(t1);
                    Triangle t2 = new Triangle(vertices2, colors2, null);
                    t2.setSprite(t.getSprite());
                    list.add(t2);
                    if (t.textureCoords != null) {
                        t1.textureCoords = tvertices1;
                        t2.textureCoords = tvertices2;
                        t1.image = t2.image = t.image;
                        t1.filter = t2.filter = t.filter;
                    }
                } catch (InvalidPolygonException e) { }

                addSegments(list, t, p, Vector3d.product(t.v0v1, n), n);

                return list;
            } else {
                list.add(t);
                return list;
            }
        }

        Color cu, cv;
        Vector3d u, v;
        Vector3d tu = null, tv = null;
        // t has not been broken, so continue...
        if (a < 0 || a > 1) {
            i = 2;
            j = 0;
            k = 1;
            u = Vector3d.getBarycenter(t.vertices[1], t.vertices[2], c, 1 - c);
            v = Vector3d.getBarycenter(t.vertices[0], t.vertices[2], b, 1 - b);
            cu = getColorsBarycenter(t.getColor(1), t.getColor(2), c, 1 - c);
            cv = getColorsBarycenter(t.getColor(0), t.getColor(2), b, 1 - b);
            if (t.textureCoords != null) {
                tu = Vector3d.getBarycenter(t.textureCoords[1], t.textureCoords[2], c, 1 - c);
                tv = Vector3d.getBarycenter(t.textureCoords[0], t.textureCoords[2], b, 1 - b);
            }
        } else if (b < 0 || b > 1) {
            i = 1;
            j = 2;
            k = 0;
            u = Vector3d.getBarycenter(t.vertices[0], t.vertices[1], a, 1 - a);
            v = Vector3d.getBarycenter(t.vertices[1], t.vertices[2], c, 1 - c);
            cu = getColorsBarycenter(t.getColor(0), t.getColor(1), a, 1 - a);
            cv = getColorsBarycenter(t.getColor(1), t.getColor(2), c, 1 - c);
            if (t.textureCoords != null) {
                tu = Vector3d.getBarycenter(t.textureCoords[0], t.textureCoords[1], a, 1 - a);
                tv = Vector3d.getBarycenter(t.textureCoords[1], t.textureCoords[2], c, 1 - c);
            }
        } else {
            i = 0;
            j = 1;
            k = 2;
            u = Vector3d.getBarycenter(t.vertices[0], t.vertices[2], b, 1 - b);
            v = Vector3d.getBarycenter(t.vertices[0], t.vertices[1], a, 1 - a);
            cu = getColorsBarycenter(t.getColor(0), t.getColor(2), b, 1 - b);
            cv = getColorsBarycenter(t.getColor(0), t.getColor(1), a, 1 - a);
            if (t.textureCoords != null) {
                tu = Vector3d.getBarycenter(t.textureCoords[0], t.textureCoords[2], b, 1 - b);
                tv = Vector3d.getBarycenter(t.textureCoords[0], t.textureCoords[1], a, 1 - a);
            }
        }

        Vector3d[] vertices1 = new Vector3d[] {u, t.vertices[i], v};
        Color[] colors1 = new Color[] {cu, t.getColor(i), cv};
        Vector3d[] vertices2 = new Vector3d[] {u, v, t.vertices[j]};
        Color[] colors2 = new Color[] {cu, cv, t.getColor(j)};
        Vector3d[] vertices3 = new Vector3d[] {u, t.vertices[j], t.vertices[k]};
        Color[] colors3 = new Color[] {cu, t.getColor(j), t.getColor(k)};

        Vector3d[] tvertices1 = null;
        Vector3d[] tvertices2 = null;
        Vector3d[] tvertices3 = null;
        if (t.textureCoords != null) {
            tvertices1 = new Vector3d[] {tu, t.textureCoords[i], tv};
            tvertices2 = new Vector3d[] {tu, tv, t.textureCoords[j]};
            tvertices3 = new Vector3d[] {tu, t.textureCoords[j], t.textureCoords[k]};
        }

        try {
            Triangle t1 = new Triangle(vertices1, colors1, null);
            t1.setSprite(t.getSprite());
            list.add(t1);
            Triangle t2 = new Triangle(vertices2, colors2, null);
            t2.setSprite(t.getSprite());
            list.add(t2);
            Triangle t3 = new Triangle(vertices3, colors3, null);
            t3.setSprite(t.getSprite());
            list.add(t3);
            if (t.textureCoords != null) {
                t1.textureCoords = tvertices1;
                t2.textureCoords = tvertices2;
                t3.textureCoords = tvertices3;
                t1.image = t2.image = t3.image = t.image;
                t1.filter = t2.filter = t3.filter = t.filter;
            }
        } catch (InvalidPolygonException e) { }

        addSegments(list, t, p, Vector3d.product(t.v0v1, n), n);

        return list;
    }

    private static final void addSegments(List<ConvexObject> list, Triangle t, Vector3d p, Vector3d u, Vector3d n) {
        if (t.segments != null) {
            List<Segment> segs = new ArrayList<Segment>();
            for (Segment s : t.segments) {
                s.removeConvexObject(t);
                List<Segment> l = s.breakObject(p, u, n);
                if (l != null && !l.isEmpty()) {
                    segs.addAll(l);
                    s.replaceSegment(l);
                }
            }
            t.textureCoords = null;

            for (Segment s : segs) {
                for (ConvexObject tri : list) {
                    ((Triangle) tri).addSegment(s);
                }
            }
        }
    }

    /**
     * Get the broken triangles in following the intersection of the planes containing t1 and t2.
     * The planes containing t1 and t2 are supposed to be secant.
     * @param t1 the first triangle
     * @param t2 the second triangle
     * @return an array of length 2 containing the resulting triangles for t1 and t2.
     */
    protected static List<ConvexObject> breakIntersectingTriangles(Triangle t1, Triangle t2) {
        Vector3d n = Vector3d.product(t1.getNormal(), t2.getNormal());
        n = n.times(1 / n.getNorm2());
        Vector3d u = Vector3d.product(t2.v0v1, n);
        Vector3d v = Vector3d.product(n, t1.v0v1);
        Vector3d p = Vector3d.getBarycenter(u, v, t1.v0v1.scalar(t1.vertices[0]), t2.v0v1.scalar(t2.vertices[0]));

        List<ConvexObject> list1 = breakTriangleOnLine(t1, p, t2.v0v1);
        List<ConvexObject> list2 = breakTriangleOnLine(t2, p, t1.v0v1);
        list1.addAll(list2);

        return list1;
    }

    public String toString() {
        return "Triangle: " + vertices[0].toString() + " " + vertices[1].toString() + " " + vertices[2].toString() + "\nColor: " + getColor(0) + "\nPrecedence: " + precedence;
    }
}
