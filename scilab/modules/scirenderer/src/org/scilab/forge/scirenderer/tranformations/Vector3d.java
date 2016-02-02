/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
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

package org.scilab.forge.scirenderer.tranformations;

import java.util.Arrays;

/**
 * @author Pierre Lando
 */
public class Vector3d {

    private final double x;
    private final double y;
    private final double z;
    private int hash = -1;

    public Vector3d(Vector3d v) {
        this.x = v.x;
        this.y = v.y;
        this.z = v.z;
    }

    public Vector3d(double x, double y, double z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public Vector3d(float[] position) {
        this.x = position[0];
        this.y = position[1];
        this.z = position[2];
    }

    public Vector3d(double[] position) {
        this.x = position[0];
        this.y = position[1];
        this.z = position[2];
    }

    public Vector3d(Double[] position) {
        this.x = position[0];
        this.y = position[1];
        this.z = position[2];
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public double getZ() {
        return z;
    }

    @Override
    public String toString() {
        return "[" + x + ", " + y + ", " + z + "]";
    }

    public double[] getData() {
        return new double[] {x, y, z};
    }

    public float[] getDataAsFloatArray() {
        return new float[] {(float) x, (float) y, (float) z};
    }

    public float[] getDataAsFloatArray(int size) {
        if (size == 4) {
            return new float[] {(float) x, (float) y, (float) z, 1};
        } else {
            return new float[] {(float) x, (float) y, (float) z};
        }
    }

    public Vector3d plus(Vector3d v) {
        return new Vector3d(x + v.x, y + v.y, z + v.z);
    }

    public Vector3d minus(Vector3d v) {
        return new Vector3d(x - v.x, y - v.y, z - v.z);
    }

    public Vector3d times(double d) {
        return new Vector3d(x * d, y * d, z * d);
    }

    public Vector3d getNormalized() {
        double norm = getNorm();
        if (norm == 0) {
            return new Vector3d(0, 0, 0);
        }
        return this.times(1 / getNorm());
    }

    public double getNorm() {
        return Math.sqrt(getNorm2());
    }

    public double getNorm2() {
        return scalar(this);
    }

    public double scalar(Vector3d v) {
        return x * v.x + y * v.y + z * v.z;
    }

    /**
     * Create a new vector cross-product of the given vectors.
     * @param v1 the first given vector.
     * @param v2 the second given vector.
     * @return a new vector cross-product of the given vectors.
     */
    public static Vector3d product(Vector3d v1, Vector3d v2) {
        return new Vector3d(
                   v1.y * v2.z - v1.z * v2.y,
                   v1.z * v2.x - v1.x * v2.z,
                   v1.x * v2.y - v1.y * v2.x
               );
    }

    public final static double det(final Vector3d v0, final Vector3d v1, final Vector3d v2) {
        return v0.x * (v1.y * v2.z - v1.z * v2.y) + v0.y * (v1.z * v2.x - v1.x * v2.z) + v0.z * (v1.x * v2.y - v1.y * v2.x);
    }

    public final static Vector3d getBarycenter(Vector3d v0, Vector3d v1, double w0, double w1) {
        return new Vector3d(v0.x * w0 + v1.x * w1, v0.y * w0 + v1.y * w1, v0.z * w0 + v1.z * w1);
    }

    /**
     * Create a new vector, copy of this one, with a new X value.
     * @param x the new X value.
     * @return a new vector, copy of this one, with a new X value.
     */
    public Vector3d setX(double x) {
        return new Vector3d(x, y, z);
    }


    /**
     * Create a new vector, copy of this one, with a new Y value.
     * @param y the new Y value.
     * @return a new vector, copy of this one, with a new Y value.
     */
    public Vector3d setY(double y) {
        return new Vector3d(x, y, z);
    }


    /**
     * Create a new vector, copy of this one, with a new Z value.
     * @param z the new Z value.
     * @return a new vector, copy of this one, with a new Z value.
     */
    public Vector3d setZ(double z) {
        return new Vector3d(x, y, z);
    }

    /**
     * Return true if this vector is (0, 0, 0).
     * @return true if this vector is (0, 0, 0).
     */
    public boolean isZero() {
        return (x == 0) && (y == 0) && (z == 0);
    }

    /**
     * Return true if this vector is (0, 0, 0).
     * @return true if this vector is (0, 0, 0).
     */
    public boolean isNearZero() {
        final double eps = 1e-9;
        return Math.abs(x) <= eps && Math.abs(y) <= eps && Math.abs(z) <= eps;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Vector3d) {
            Vector3d v = (Vector3d) obj;
            final double eps = 1e-9;
            return Math.abs(v.x - x) <= eps && Math.abs(v.y - y) <= eps && Math.abs(v.z - z) <= eps;
        }
        return false;
    }

    @Override
    public int hashCode() {
        if (hash == -1) {
            hash = Arrays.hashCode(new double[] {x, y, z});
        }
        return hash;
    }
}
