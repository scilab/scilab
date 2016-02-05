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
import java.util.ArrayList;
import java.util.List;

import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.tranformations.Vector4d;

/**
 * @author Calixte DENIZET
 *
 * Class to represent a convex object.
 * Collision and relative positions of convexs object are relatively easy to determinate.
 * About the method isBehind, it could be interesting to use the algorithm of Chung-Wang.
 */
public abstract class ConvexObject extends AbstractDrawable3DObject implements Clippable {

    private List<ConvexObject> areas;

    /**
     * Default constructor
     * @param vertices the vertices
     * @param colors the colors
     */
    public ConvexObject(Vector3d[] vertices, Color[] colors) throws InvalidPolygonException {
        super(vertices, colors);
    }

    /**
     * Abstract method
     * Break this ConvexObject against the ConvexObject o
     * @param o a ConvexObject
     * @return a list of ConvexObject.
     */
    public abstract List<ConvexObject> breakObject(ConvexObject o);

    /**
     * {@inheritDoc}
     */
    public abstract List<ConvexObject> breakObject(Vector4d v);

    public void addArea(ConvexObject co) {
        if (areas == null) {
            areas = new ArrayList<ConvexObject>();
        }
        areas.add(co);
    }

    protected void drawAreas(Graphics2D g2d) {
        if (areas != null) {
            for (ConvexObject co : areas) {
                Shape oldClip = g2d.getClip();
                g2d.clip(this.getProjectedContour());
                co.draw(g2d);
                g2d.setClip(oldClip);
            }
        }
    }

    /**
     * Test the coplanarity of two objects
     * @param o a ConvexObject
     * @return true if the two objects are coplanar
     */
    public boolean areCoplanar(ConvexObject o) {
        if (!(this instanceof Segment)) {
            double sc = vertices[0].scalar(getNormal());
            if (o instanceof Segment) {
                return isEqual(sc, o.vertices[0].scalar(v0v1)) && isEqual(sc, o.vertices[1].scalar(v0v1));
            }
            return isEqual(sc, o.vertices[0].scalar(v0v1)) && isEqual(sc, o.vertices[1].scalar(v0v1)) && isEqual(sc, o.vertices[2].scalar(v0v1));
        }

        if (!(o instanceof Segment)) {
            return o.areCoplanar(this);
        }

        if (o.vertices[0].equals(vertices[0]) || o.vertices[1].equals(vertices[0]) || o.vertices[0].equals(vertices[1]) || o.vertices[1].equals(vertices[1])) {
            return true;
        }

        getNormal();
        o.getNormal();
        Vector3d v = Vector3d.product(v0, o.v0);
        return isNull(v.scalar(vertices[0].minus(o.vertices[0])));
    }

    /**
     * Check if o is behind this.
     * Take care: the algorithms used are for convex objects (typically tri-tri, seg-seg or tri-seg)
     * @return true if o is behind this
     */
    public int isBehind(ConvexObject o) {
        BoundingBox bbox = getBBox();
        BoundingBox obbox = o.getBBox();
        // Quick test in using bounding boxes
        if (bbox.isNonZOverlapping(obbox)) {
            return 0;
        }

        // Check if the two objects intersect in projection plane or not
        if (check2DIntersection(o)) {
            // We have a strict intersection or an intersection on an edge
            if (areCoplanar(o)) {
                return getPrecedence() > o.getPrecedence() ? 1 : -1;
            }

            // Quick test with bounding-box along z-axis
            int ret = bbox.zCompare(obbox);
            if (ret != 0) {
                return ret;
            }

            // In the most of the cases, one of the two following test are sufficient to determinate
            // if one object is behind or on front of the plane containing this or o
            ret = check(o, getNormal());
            if (ret != 0) {
                return ret;
            }

            ret = check(o, o.getNormal());
            if (ret != 0) {
                return ret;
            }

            // Check against the cross product of one edge of this and one edge of o
            int M = vertices.length == 2 ? 1 : vertices.length;
            int N = o.vertices.length == 2 ? 1 : o.vertices.length;
            for (int j = 0; j < M; j++) {
                int l = (j + 1 < vertices.length) ? j + 1 : 0;
                Vector3d e = vertices[l].minus(vertices[j]);
                for (int k = 0; k < N; k++) {
                    int m = (k + 1 < o.vertices.length) ? k + 1 : 0;
                    Vector3d oe = o.vertices[m].minus(o.vertices[k]);
                    ret = check(o, Vector3d.product(e, oe).getNormalized());
                    if (ret != 0) {
                        return ret;
                    }
                }
            }

            // At this point: there is a collision between the two objects
            return 2;
        } else {
            return 0;
        }
    }

    /**
     * Check the intersections of the projection on the xOy-plane of this and o
     * The algorithm is the following: for each edge, determinate the normal vector and project all the points
     * of this and o on the normal. If the intersection of [this.min,this.max] and [o.min, o.max] is empty, then
     * we have a separating line so the two objects are separated.
     * @param o the object to test with this
     * @return true if there is a collision
     */
    public boolean check2DIntersection(final ConvexObject o) {
        int ret = check2D(this, o);
        if (ret != -1) {
            return false;
        }

        ret = check2D(o, this);
        if (ret != -1) {
            return false;
        }

        return true;
    }

    /**
     * Check the intersections of the projection on the xOy-plane of this and o
     * The algorithm is the following: for each edge, determinate the normal vector and project all the points
     * of this and o on the normal. If the intersection of [this.min,this.max] and [o.min, o.max] is empty, then
     * we have a separating line so the two objects are separated.
     * @param o the object to test with this
     * @return true if there is a collision
     */
    public boolean check2DTrueIntersection(final ConvexObject o) {
        int ret = check2D2(this, o);
        if (ret == 1) {
            return true;
        } else if (ret == 0) {
            return false;
        }

        ret = check2D2(o, this);
        if (ret == 1) {
            return true;
        } else if (ret == 0) {
            return false;
        }

        return true;
    }

    /**
     * Check 2D intersection of two convex objects
     * @param o1 first object
     * @param o2 second object
     * @return -1 if strict intersection, 1 if intersection on an edge and 0 if no intersection
     */
    private static final int check2D(final ConvexObject o1, final ConvexObject o2) {
        // When o1 is a Segment (i.e. o1.vertices;length == 2) it is mandatory to check against ortho(v1-v0) and ortho(v0-v1)
        int M = o1.vertices.length == 2 ? 1 : o1.vertices.length;
        for (int i = 0; i < M; i++) {
            int j = (i + 1 < o1.vertices.length) ? i + 1 : 0;
            double xN = o1.vertices[i].getY() - o1.vertices[j].getY();
            double yN = o1.vertices[j].getX() - o1.vertices[i].getX();
            double n = Math.hypot(xN, yN);
            xN /= n;
            yN /= n;
            double[] mM = minmax2D(o1, xN, yN);
            double min = mM[0];
            double max = mM[1];

            mM = minmax2D(o2, xN, yN);
            double omin = mM[0];
            double omax = mM[1];

            if (max < omin || omax < min) {
                return 0;
            }

            if (isEqual(max, omin) || isEqual(omax, min)) {
                return 1;
            }
        }

        return -1;
    }

    /**
     * Check 2D intersection of two convex objects
     * @param o1 first object
     * @param o2 second object
     * @return -1 if strict intersection, 1 if intersection on an edge and 0 if no intersection
     */
    private static final int check2D2(final ConvexObject o1, final ConvexObject o2) {
        // When o1 is a Segment (i.e. o1.vertices;length == 2) it is mandatory to check against ortho(v1-v0) and ortho(v0-v1)
        int M = o1.vertices.length == 2 ? 1 : o1.vertices.length;
        boolean bool = false;
        for (int i = 0; i < M; i++) {
            int j = (i + 1 < o1.vertices.length) ? i + 1 : 0;
            double xN = o1.vertices[i].getY() - o1.vertices[j].getY();
            double yN = o1.vertices[j].getX() - o1.vertices[i].getX();
            double n = Math.hypot(xN, yN);
            xN /= n;
            yN /= n;
            double[] mM = minmax2D(o1, xN, yN);
            double min = mM[0];
            double max = mM[1];

            mM = minmax2D(o2, xN, yN);
            double omin = mM[0];
            double omax = mM[1];

            if (max < omin || omax < min) {
                return 0;
            }

            if (!bool && (isEqual(max, omin) || isEqual(omax, min))) {
                bool = true;
            }
        }

        if (bool) {
            return 1;
        }

        return -1;
    }

    /**
     * Check the intersection this and o against vector v.
     * The algorithm is just to project this and o on the vector v and to check if the two projected sets
     * can be separated.
     * @param v the vector where to project
     * @return 1 if o is behind this, 0 if it is undeterminated and -1 if this is behind o.
     */
    protected int check(final ConvexObject o, final Vector3d v) {
        if (!v.isNearZero()) {
            double[] mM = minmax3D(this, v);
            double min = mM[0];
            double max = mM[1];

            mM = minmax3D(o, v);
            double omin = mM[0];
            double omax = mM[1];
            double z = v.getZ();

            if (Math.signum(z) == 0) {
                return 0;
            }

            if (isLowerOrEqual(max, omin)) {
                return (int) Math.signum(z);
            }

            if (isLowerOrEqual(omax, min)) {
                return (int) - Math.signum(z);
            }
        }

        return 0;
    }
}
