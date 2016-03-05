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

/**
 * @author Pierre Lando
 */
public class Vector3f {
    private final float x;
    private final float y;
    private final float z;

    public Vector3f(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public float getX() {
        return x;
    }

    public float getY() {
        return y;
    }

    public float getZ() {
        return z;
    }

    @Override
    public String toString() {
        return "[" + x + ", " + y + ", " + z + "]";
    }

    public Vector3d asDouble() {
        return new Vector3d(x, y, z);
    }

    public Vector3f plus(Vector3f v) {
        return new Vector3f(x + v.x, y + v.y, z + v.z);
    }

    public Vector3f minus(Vector3f v) {
        return new Vector3f(x - v.x, y - v.y, z - v.z);
    }

    public Vector3f negate() {
        return new Vector3f(-x, -y, -z);
    }

    public Vector3f times(float d) {
        return new Vector3f(x * d, y * d, z * d);
    }

    public Vector3f getNormalized() {
        float norm = getNorm();
        if (norm == 0) {
            return new Vector3f(0.0f, 0.0f, 0.0f);
        }
        return this.times(1.0f / getNorm());
    }

    public float getNorm() {
        return (float)Math.sqrt(getNorm2());
    }

    public float getNorm2() {
        return scalar(this);
    }

    public float scalar(Vector3f v) {
        return x * v.x + y * v.y + z * v.z;
    }

    /**
     * Create a new vector cross-product of the given vectors.
     * @param v1 the first given vector.
     * @param v2 the second given vector.
     * @return a new vector cross-product of the given vectors.
     */
    public static Vector3f product(Vector3f v1, Vector3f v2) {
        return new Vector3f(
                   v1.y * v2.z - v1.z * v2.y,
                   v1.z * v2.x - v1.x * v2.z,
                   v1.x * v2.y - v1.y * v2.x
               );
    }

    public final static float det(final Vector3f v0, final Vector3f v1, final Vector3f v2) {
        return v0.x * (v1.y * v2.z - v1.z * v2.y) + v0.y * (v1.z * v2.x - v1.x * v2.z) + v0.z * (v1.x * v2.y - v1.y * v2.x);
    }

    public final static Vector3f getBarycenter(Vector3f v0, Vector3f v1, float w0, float w1) {
        return new Vector3f(v0.x * w0 + v1.x * w1, v0.y * w0 + v1.y * w1, v0.z * w0 + v1.z * w1);
    }
}
