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
 * This class represent a Rotation.
 * The data are stored as an unitary quaternion [a; b; c; d].
 * A rotation of an angle alpha along the vector [x; y; z] is stored thus :
 *  a = cos (alpha / 2)
 *  b = sin (alpha / 2) * nx
 *  c = sin (alpha / 2) * ny
 *  d = sin (alpha / 2) * nz
 *
 *  Where [nx; ny; nz] is [x; y; z] / norm([x; y; z]).
 *
 * @author Pierre Lando
 */
public class Rotation {

    private static final int MAX_SELF_OPERATION = 16;
    private int opCount;
    private double a;
    private double b;
    private double c;
    private double d;
    private double[] rotationMatrix;

    /**
     * Default constructor.
     * The created object represent identity rotation.
     */
    public Rotation() {
        a = 1;
        b = 0;
        c = 0;
        d = 0;
        normalize();
    }

    /**
     * Copy constructor.
     * The created object is a copy of the given rotation r.
     * @param r the given rotation.
     */
    public Rotation(Rotation r) {
        a = r.a;
        b = r.b;
        c = r.c;
        d = r.d;
        normalize();
    }

    /**
     * The created object represent a rotation of an angle of 'alpha' radians along the vector 'v'.
     * @param alpha the rotation angle in radians.
     * @param v     the vector carrying the rotation.
     */
    public Rotation(double alpha, Vector3d v) {
        double t = Math.cos(alpha / 2);
        Vector3d nv = v.getNormalized().times(Math.sin(alpha / 2));

        a = t;
        b = nv.getX();
        c = nv.getY();
        d = nv.getZ();
        normalize();
    }

    /**
     * The created object represent a rotation of an angle of 'alpha' radians along the vector 'v'.
     * @param alpha the rotation angle in radians.
     * @param v     the vector carrying the rotation.
     */
    public Rotation(double alpha, Vector3f v) {
        this(alpha, v.asDouble());
    }

    /**
     * Get a rotation from an angle alpha given in degree and an axis v
     * @param alpha angle of rotation in degree
     * @param v the axis of the rotation
     * @return a Rotation object
     *
     * For information, the function has been written to fix bug 11399.
     * For angle (in radians) close to Pi the Rotation has a 'a' close to 0
     * and that conflicts with small axes where length is closed to a: rounding errors
     * led to a "bad" transformation matrix (AxesDrawer::computeBoxTransformation).
     */
    public static Rotation getDegreeRotation(double alpha, Vector3d v) {
        int a = (int) alpha;
        if (alpha == a) {
            a = a % 360;
            if (a == 0) {
                return new Rotation(1, 0, 0, 0);
            }
            if (a == 180) {
                Vector3d nv = v.getNormalized();
                return new Rotation(0, nv.getX(), nv.getY(), nv.getZ());
            }

            return new Rotation(((double) a) / 180.0 * Math.PI, v);
        }

        alpha = (alpha / 180.0 - 2 * Math.floor(alpha / 360.0)) * Math.PI;

        return new Rotation(alpha, v);
    }


    private Rotation(double a, double b, double c, double d) {
        this.a = a;
        this.b = b;
        this.c = c;
        this.d = d;
        normalize();
    }

    public Rotation(float[] v) {
        if ((v != null) && (v.length == 4)) {
            a = v[0];
            b = v[1];
            c = v[2];
            d = v[3];
        } else { // TODO throw invalid data exception
            a = 1;
            b = 0;
            c = 0;
            d = 0;
        }
        normalize();
    }

    public Rotation(double[] v) {
        if ((v != null) && (v.length == 4)) {
            a = v[0];
            b = v[1];
            c = v[2];
            d = v[3];
        } else { // TODO throw invalid data exception
            a = 1;
            b = 0;
            c = 0;
            d = 0;
        }
        normalize();
    }

    /**
     * Return the inverse rotation.
     * @return the inverse rotation.
     */
    public Rotation getInverse() {
        return new Rotation(a, -b, -c, -d);
    }

    /**
     * Return true if this object represents an identity transformation.
     * @return true if this object represents an identity transformation.
     */
    public boolean isIdentity() {
        return (a == 1);
    }

    public double[] getRotationMatrix() {
        return rotationMatrix.clone();
    }
    public double[] getUnRotateMatrix() {
        return new double[] {
                   rotationMatrix[0], rotationMatrix[4], rotationMatrix[8], rotationMatrix[12],
                   rotationMatrix[1], rotationMatrix[5], rotationMatrix[9], rotationMatrix[13],
                   rotationMatrix[2], rotationMatrix[6], rotationMatrix[10], rotationMatrix[14],
                   rotationMatrix[3], rotationMatrix[7], rotationMatrix[11], rotationMatrix[15]
               };
    }

    public Rotation multiply(Rotation q) {
        double ar = a * q.a - b * q.b - c * q.c - d * q.d;
        double br = a * q.b + b * q.a + c * q.d - d * q.c;
        double cr = a * q.c - b * q.d + c * q.a + d * q.b;
        double dr = a * q.d + b * q.c - c * q.b + d * q.a;
        a = ar;
        b = br;
        c = cr;
        d = dr;
        selfCheck();
        return this;
    }

    private void selfCheck() {
        if (opCount++ > MAX_SELF_OPERATION) {
            normalize();
        } else {
            computeRotationMatrix();
        }
    }

    private void normalize() {
        opCount = 0;
        double f = 1f / Math.sqrt((a * a) + (b * b) + (c * c) + (d * d));
        a *= f;
        b *= f;
        c *= f;
        d *= f;
        computeRotationMatrix();
    }


    public Rotation times(Rotation q) {
        return new Rotation(this).multiply(q);
    }

    public Vector3d conjugate(Vector3d v) {
        return new Vector3d(
                   rotationMatrix[0] * v.getX() + rotationMatrix[4] * v.getY() + rotationMatrix[8] * v.getZ(),
                   rotationMatrix[1] * v.getX() + rotationMatrix[5] * v.getY() + rotationMatrix[9] * v.getZ(),
                   rotationMatrix[2] * v.getX() + rotationMatrix[6] * v.getY() + rotationMatrix[10] * v.getZ()
               );
    }
    public Vector3d conjugateInverse(Vector3d v) {
        return new Vector3d(
                   rotationMatrix[0] * v.getX() + rotationMatrix[1] * v.getY() + rotationMatrix[2] * v.getZ(),
                   rotationMatrix[4] * v.getX() + rotationMatrix[5] * v.getY() + rotationMatrix[6] * v.getZ(),
                   rotationMatrix[8] * v.getX() + rotationMatrix[9] * v.getY() + rotationMatrix[10] * v.getZ()
               );
    }

    private void computeRotationMatrix() {
        double a2 = a * a;
        double b2 = b * b;
        double c2 = c * c;
        double d2 = d * d;
        this.rotationMatrix = new double[] {
            a2 + b2 - c2 - d2,       2 * ((b * c) - (a * d)), 2 * ((a * c) + (b * d)), 0,
            2 * ((a * d) + (b * c)), a2 - b2 + c2 - d2,       2 * ((c * d) - (a * b)), 0,
            2 * ((b * d) - (a * c)), 2 * ((a * b) + (c * d)), a2 - b2 - c2 + d2,       0,
            0,                       0,                       0,                       1
        };
    }

    public String toString() {
        return "(" + a + ", " + b + ", " + c + ", " + d + ")";
    }

    public Vector3d getVectorY() {
        return new Vector3d(rotationMatrix[1], rotationMatrix[5], rotationMatrix[9]);
    }

    public Rotation power(double p) {
        // TODO : do a better S.L.E.R.P.
        double cos = a;
        double sin = Math.sqrt((b * b) + (c * c) + (d * d));

        if (sin < 0.001) {
            return new Rotation();
        }

        double t = (p - 1)  *  Math.atan2(sin, cos);
        double na = cos * Math.cos(t) - sin * Math.sin(t);

        double beta = Math.cos(t) + Math.sin(t) * (cos / sin);
        return new Rotation(na, beta * b, beta * c, beta * d);
    }

    public double[] getData() {
        return new double[] {a, b, c, d};
    }

    public float[] getDataAsFloatArray() {
        return new float[] {(float) a, (float) b, (float) c, (float) d};
    }

    @Override
    public int hashCode() {
        int hashCode = 0;
        hashCode += 7 * (new Double(a).hashCode());
        hashCode += 17 * (new Double(b).hashCode());
        hashCode += 29 * (new Double(c).hashCode());
        hashCode += 37 * (new Double(d).hashCode());
        return hashCode;
    }

    public boolean equals(Rotation r2) {
        return  (r2 != null) && (a == r2.a)  && (b == r2.b)  && (c == r2.c)  && (d == r2.d);
    }
}
