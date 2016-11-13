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

import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * @author Calixte DENIZET
 *
 * Bounding box of a 3D object, useful to speed-up intersection detection.
 */
public class BoundingBox {

    private double minX = Double.POSITIVE_INFINITY;
    private double maxX = Double.NEGATIVE_INFINITY;
    private double minY = Double.POSITIVE_INFINITY;
    private double maxY = Double.NEGATIVE_INFINITY;
    private double minZ = Double.POSITIVE_INFINITY;
    private double maxZ = Double.NEGATIVE_INFINITY;

    /**
     * Default constructor
     * @param minX the minimal X
     * @param maxX the maximal X
     * @param minY the minimal Y
     * @param maxY the maximal Y
     * @param minZ the minimal Z
     * @param maxZ the maximal Z
     */
    public BoundingBox(double minX, double maxX, double minY, double maxY, double minZ, double maxZ) {
        this.minX = minX;
        this.maxX = maxX;
        this.minY = minY;
        this.maxY = maxY;
        this.minZ = minZ;
        this.maxZ = maxZ;
    }

    /**
     * Get the relative x-position of this bounding-box and of the box
     * @param box a BoundingBox
     * @return 1 if box is on the right, -1 if on the left and 0 if nothing.
     */
    public int xCompare(BoundingBox box) {
        if (box.minX >= maxX) {
            return -1;
        }

        if (minX >= box.maxX) {
            return 1;
        }

        return 0;
    }

    /**
     * Get the relative y-position of this bounding-box and of the box
     * @param box a BoundingBox
     * @return 1 if box is on the bottom, -1 if on the top and 0 if nothing.
     */
    public int yCompare(BoundingBox box) {
        if (box.minY >= maxY) {
            return -1;
        }

        if (minY >= box.maxY) {
            return 1;
        }

        return 0;
    }

    /**
     * Get the relative z-position of this bounding-box and of the box
     * @param box a BoundingBox
     * @return 1 if box is on the front, -1 if behind and 0 if nothing.
     */
    public int zCompare(BoundingBox box) {
        if (box.minZ >= maxZ) {
            return 1;
        }

        if (minZ >= box.maxZ) {
            return -1;
        }

        return 0;
    }

    /**
     * @param box a BoundingBox
     * @return true if this BoundingBox and the box have an intersection
     */
    public boolean isIntersecting(BoundingBox box) {
        return box.maxX >= minX && maxX >= box.minX
               && box.maxY >= minY && maxY >= box.minY
               && box.maxZ >= minZ && maxZ >= box.minZ;
    }

    /**
     * @return true if the bbox are non-intersecting and no z-overlapping
     */
    public boolean isNonZOverlapping(BoundingBox box) {
        return (box.maxX < minX || maxX < box.minX || box.maxY < minY || maxY < box.minY) || ((box.maxZ < minZ || maxZ < box.minZ) && (maxX == box.minX || box.maxX == minX || box.minY == maxY || minY == box.maxY));
    }

    /**
     * @param box a BoundingBox
     * @return true if this BoundingBox and the box have a strict intersection
     */
    public boolean isStrictlyIntersecting(BoundingBox box) {
        return box.maxX > minX && maxX > box.minX
               && box.maxY > minY && maxY > box.minY
               && box.maxZ > minZ && maxZ > box.minZ;
    }

    /**
     * Get the bounding box of an object
     * @param object a 3D object
     * @return the corresponding bounding-box
     */
    public static BoundingBox getBoundingBox(AbstractDrawable3DObject object) {
        Vector3d[] vertices = object.vertices;
        double minX = Double.POSITIVE_INFINITY;
        double maxX = Double.NEGATIVE_INFINITY;
        double minY = Double.POSITIVE_INFINITY;
        double maxY = Double.NEGATIVE_INFINITY;
        double minZ = Double.POSITIVE_INFINITY;
        double maxZ = Double.NEGATIVE_INFINITY;

        for (int i = 0; i < vertices.length; i++) {
            double x = vertices[i].getX();
            double y = vertices[i].getY();
            double z = vertices[i].getZ();
            if (x < minX) {
                minX = x;
            }

            if (x > maxX) {
                maxX = x;
            }

            if (y < minY) {
                minY = y;
            }

            if (y > maxY) {
                maxY = y;
            }

            if (z < minZ) {
                minZ = z;
            }

            if (z > maxZ) {
                maxZ = z;
            }
        }

        return new BoundingBox(minX, maxX, minY, maxY, minZ, maxZ);
    }

    @Override
    public String toString() {
        return "[" + minX + ";" + maxX + "]x" + "[" + minY + ";" + maxY + "]x" + "[" + minZ + ";" + maxZ + "]";
    }
}
