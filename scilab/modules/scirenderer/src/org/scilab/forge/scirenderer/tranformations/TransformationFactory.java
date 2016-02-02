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

import java.awt.Dimension;

/**
 * @author Pierre Lando
 */
public final class TransformationFactory {

    /**
     * The identity transformation.
     */
    static final Transformation IDENTITY_TRANSFORMATION = new IdentityTransformation();

    /**
     * Default constructor.
     * The constructor is private : this is an utility class.
     */
    private TransformationFactory() {
    }


    /**
     * Return the identity transformation.
     * @return the identity transformation.
     */
    public static Transformation getIdentity() {
        return IDENTITY_TRANSFORMATION;
    }

    /**
     * Return a translate transformation.
     * @param x translation in x coordinate.
     * @param y translation in y coordinate.
     * @param z translation in z coordinate.
     * @return a translate transformation.
     */
    public static Transformation getTranslateTransformation(double x, double y, double z) {
        if ((x == 0) && (y == 0) && (z == 0)) {
            return IDENTITY_TRANSFORMATION;
        } else {
            return new TranslateTransformation(x, y, z);
        }
    }

    private static Transformation getTranslateTransformation(Vector3d t) {
        return getTranslateTransformation(t.getX(), t.getY(), t.getZ());
    }

    /**
     * Return a scale transformation.
     * @param x scale in x.
     * @param y scale in y.
     * @param z scale in z.
     * @return a scale transformation.
     * @throws DegenerateMatrixException - A DegenerateMatrixException is thrown if one of the scale factor is zero.
     */
    public static Transformation getScaleTransformation(double x, double y, double z) throws DegenerateMatrixException {
        if ((x == 1) && (y == 1) && (z == 1)) {
            return IDENTITY_TRANSFORMATION;
        } else if ((x == 0) || (y == 0) || (z == 0)) {
            throw new DegenerateMatrixException("Scale matrix with 0 factor.");
        } else {
            return new ScaleTransformation(x, y, z);
        }
    }

    /**
     * Return a scale transformation.
     * @param s scale vector.
     * @return a scale transformation.
     * @throws DegenerateMatrixException - A DegenerateMatrixException is thrown if one coordinate of the scale vector is zero.
     */
    private static Transformation getScaleTransformation(Vector3d s) throws DegenerateMatrixException {
        return getScaleTransformation(s.getX(), s.getY(), s.getZ());
    }

    /**
     * Return a scale transformation.
     * @param s scale value.
     * @return a scale transformation.
     * @throws DegenerateMatrixException - A DegenerateMatrixException is thrown if one coordinate of the scale vector is zero.
     */
    public static Transformation getScaleTransformation(double s) throws DegenerateMatrixException {
        return getScaleTransformation(s, s, s);
    }

    /**
     * Return a rotation transformation.
     * @param angle the rotation angle in degree.
     * @param x the x coordinate of the rotation axes.
     * @param y the y coordinate of the rotation axes.
     * @param z the z coordinate of the rotation axes.
     * @return a rotation transformation.
     * @throws DegenerateMatrixException - A DegenerateMatrixException is thrown if the rotation axes is zero.
     */
    public static Transformation getRotationTransformation(double angle, double x, double y, double z) throws DegenerateMatrixException {
        if ((x == 0) && (y == 0) && (z == 0)) {
            throw new DegenerateMatrixException("Rotation axes should not be nul.");
        } else if (angle == 0) {
            return IDENTITY_TRANSFORMATION;
        } else {
            return new RotationTransformation(Rotation.getDegreeRotation(angle, new Vector3d(x, y, z)));
        }
    }

    /**
     * Return a rotation transformation.
     * @param q the quaternion.
     * @return a rotation transformation.
     */
    public static Transformation getRotationTransformation(Rotation q) {
        if (q.isIdentity()) {
            return IDENTITY_TRANSFORMATION;
        } else {
            return new RotationTransformation(q);
        }
    }

    /**
     * Return the 'preferred aspect ratio transformation'
     * @param dimension the canvas dimension.
     * @param preferredRatio the preferred aspect ratio.
     * @return the 'preferred aspect ratio transformation'
     * @throws DegenerateMatrixException - A DegenerateMatrixException is thrown if the given value are not reasonable.
     */
    public static Transformation getPreferredAspectRatioTransformation(Dimension dimension, double preferredRatio) throws DegenerateMatrixException {
        double ratio = dimension.getWidth() / dimension.getHeight();
        if (ratio > preferredRatio) {
            return getScaleTransformation(1 / ratio, 1, 1);
        } else {
            return getScaleTransformation(1, ratio, 1);
        }
    }

    /**
     * Return a perspective transformation.
     * @param near the distance from the viewer to the near clipping plane.
     * @param far  the distance from the viewer to the far clipping plane.
     * @param fov  the field of view angle in degree.
     * @return a perspective transformation.
     */
    public static Transformation getPerspectiveTransformation(double near, double far, double fov) {
        return new PerspectiveTransformation(near, far, fov);
    }

    /**
     * Return an orthographic transformation.
     * @param left   the left plane distance to origin.
     * @param right  the right plane distance to origin.
     * @param bottom the bottom plane distance to origin.
     * @param top    the top plane distance to origin.
     * @param near   the near plane distance to origin.
     * @param far    the far plane distance to origin.
     * @return an orthographic transformation.
     */
    public static Transformation getOrthographic(double left, double right, double bottom, double top, double near, double far) {
        return new OrthographicTransformation(left, right, bottom, top, near, far);
    }

    /**
     * Return an affine transformation
     * aX + b
     * @param s the scale parameter.
     * @param t the translate parameter.
     * @return an affine transformation
     * @throws DegenerateMatrixException - A DegenerateMatrixException is thrown if one coordinate of the scale vector is zero.
     */
    public static Transformation getAffineTransformation(Vector3d s, Vector3d t) throws DegenerateMatrixException {
        return getTranslateTransformation(t).rightTimes(getScaleTransformation(s));
    }

    /**
     * Abstract transformation.
     * Implement common methods of all transformation.
     */
    private abstract static class AbstractTransformation implements Transformation {

        @Override
        public Transformation rightTimes(Transformation transformation) {
            return getProductTransformation(this, transformation);
        }

        @Override
        public Transformation leftTimes(Transformation transformation) {
            return getProductTransformation(transformation, this);
        }

        @Override
        public Vector3d project(Vector3d vector) {
            double[] matrix = getMatrix();
            double x = matrix[0] * vector.getX() + matrix[4] * vector.getY() +  matrix[8] * vector.getZ() + matrix[12];
            double y = matrix[1] * vector.getX() + matrix[5] * vector.getY() +  matrix[9] * vector.getZ() + matrix[13];
            double z = matrix[2] * vector.getX() + matrix[6] * vector.getY() +  matrix[10] * vector.getZ() + matrix[14];
            double w = matrix[3] * vector.getX() + matrix[7] * vector.getY() +  matrix[11] * vector.getZ() + matrix[15];
            return new Vector3d(x / w, y / w, z / w);
        }

        @Override
        public Vector3d projectDirection(Vector3d direction) {
            double[] matrix = getMatrix();
            double x = matrix[0] * direction.getX() + matrix[4] * direction.getY() +  matrix[8] * direction.getZ();
            double y = matrix[1] * direction.getX() + matrix[5] * direction.getY() +  matrix[9] * direction.getZ();
            double z = matrix[2] * direction.getX() + matrix[6] * direction.getY() +  matrix[10] * direction.getZ();
            double w = matrix[3] * direction.getX() + matrix[7] * direction.getY() +  matrix[11] * direction.getZ() + matrix[15];
            return new Vector3d(x / w, y / w, z / w);
        }

        @Override
        public Vector3d unproject(Vector3d vector) {
            return getInverseTransformation().project(vector);
        }

        @Override
        public Transformation getInverseTransformation() {
            return new AbstractTransformation() {
                @Override
                public boolean isIdentity() {
                    return AbstractTransformation.this.isIdentity();
                }

                @Override
                public double[] getMatrix() {
                    return AbstractTransformation.this.getInverseMatrix();
                }

                @Override
                public double[] getInverseMatrix() {
                    return AbstractTransformation.this.getMatrix();
                }
            };
        }

        @Override
        public String toString() {
            double[] matrix = getMatrix();
            String r = "";
            for (int i = 0; i < 16; i++) {
                r += matrix[i];
                if ((i % 4) == 3) {
                    r += "\n";
                } else {
                    r += ", ";
                }
            }
            return r;
        }

        /**
         * Return a product transformation.
         * @param t1 the first transformation.
         * @param t2 the second transformation.
         * @return t1 x t2
         */
        private Transformation getProductTransformation(Transformation t1, Transformation t2) {
            if (t1.isIdentity()) {
                return t2;
            } else if (t2.isIdentity()) {
                return t1;
            } else {
                return new ProductTransformation(t1, t2);
            }
        }
    }


    /**
     * The identity transformation.
     */
    private static class IdentityTransformation extends AbstractTransformation {

        /**
         * The identity matrix data.
         */
        private static final double[] IDENTITY_MATRIX = new double[] {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };

        /**
         * Default constructor.
         */
        IdentityTransformation() {
        }

        @Override
        public Transformation rightTimes(Transformation transformation) {
            return transformation;
        }

        @Override
        public Transformation leftTimes(Transformation transformation) {
            return transformation;
        }

        @Override
        public Vector3d project(Vector3d vector) {
            return vector;
        }

        @Override
        public Vector3d projectDirection(Vector3d direction) {
            return direction;
        }

        @Override
        public Vector3d unproject(Vector3d vector) {
            return vector;
        }

        @Override
        public boolean isIdentity() {
            return true;
        }

        @Override
        public double[] getMatrix() {
            return IDENTITY_MATRIX.clone();
        }

        @Override
        public double[] getInverseMatrix() {
            return IDENTITY_MATRIX.clone();
        }
    }

    /**
     * A translate transformation.
     */
    private static class TranslateTransformation extends AbstractTransformation {

        /**
         * translation in x coordinate.
         */
        private final double x;

        /**
         * translation in y coordinate.
         */
        private final double y;

        /**
         * translation in z coordinate.
         */
        private final double z;

        /**
         * Default constructor.
         *
         * (x, y, z) != (0, 0, 0).
         *
         * @param x translation in x coordinate.
         * @param y translation in y coordinate.
         * @param z translation in z coordinate.
         */
        public TranslateTransformation(double x, double y, double z) {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        @Override
        public Vector3d project(Vector3d vector) {
            return new Vector3d(vector.getX() + x, vector.getY() + y, vector.getZ() + z);
        }

        @Override
        public Vector3d unproject(Vector3d vector) {
            return new Vector3d(vector.getX() - x, vector.getY() - y, vector.getZ() - z);
        }

        @Override
        public boolean isIdentity() {
            return false;
        }

        @Override
        public double[] getMatrix() {
            return new double[] {
                       1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       x, y, z, 1
                   };
        }

        @Override
        public double[] getInverseMatrix() {
            return new double[] {
                       1,  0,  0,  0,
                       0,  1,  0,  0,
                       0,  0,  1,  0,
                       -x, -y, -z, 1
                   };
        }
    }


    /**
     * A scale transformation.
     */
    private static class ScaleTransformation extends AbstractTransformation {

        /**
         * scale in x coordinate.
         */
        private final double x;

        /**
         * scale in y coordinate.
         */
        private final double y;

        /**
         * scale in z coordinate.
         */
        private final double z;


        /**
         * Default constructor.
         *
         * x, y and z not 0 or 1.
         *
         * @param x scale in x coordinate.
         * @param y scale in y coordinate.
         * @param z scale in z coordinate.
         */
        public ScaleTransformation(double x, double y, double z) {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        @Override
        public Vector3d project(Vector3d vector) {
            return new Vector3d(vector.getX() * x, vector.getY() * y, vector.getZ() * z);
        }

        @Override
        public Vector3d unproject(Vector3d vector) {
            return new Vector3d(vector.getX() / x, vector.getY() / y, vector.getZ() / z);
        }

        @Override
        public boolean isIdentity() {
            return false;
        }

        @Override
        public double[] getMatrix() {
            return new double[] {
                       x, 0, 0, 0,
                       0, y, 0, 0,
                       0, 0, z, 0,
                       0, 0, 0, 1
                   };
        }

        @Override
        public double[] getInverseMatrix() {
            return new double[] {
                       1 / x, 0,     0,     0,
                       0,     1 / y, 0,     0,
                       0,     0,     1 / z, 0,
                       0,     0,     0,     1
                   };
        }
    }

    /**
     * A perspective transformation.
     */
    private static class PerspectiveTransformation extends AbstractTransformation {

        private final double[] matrix;
        private final double[] inverseMatrix;

        /**
         * Default constructor.
         * @param near the distance to the near plane.
         * @param far  the distance to the far plane.
         * @param fov  the field of view in degree.
         */
        PerspectiveTransformation(double near, double far, double fov) {
            double fInv = Math.tan(Math.toRadians(fov / 2));
            double f    = 1 / fInv;

            matrix = new double[] {
                f, 0, 0,                             0,
                0, f, 0,                             0,
                0, 0, (far + near) / (near - far),   -1,
                0, 0, 2 * far * near / (near - far), 0
            };

            inverseMatrix = new double[] {
                fInv, 0,    0,     0,
                0,    fInv, 0,     0,
                0,    0,    0,     (near - far) / (2 * far * near),
                0,    0,    -1,    (near + far) / (2 * far * near)
            };
        }

        @Override
        public boolean isIdentity() {
            return false;
        }

        @Override
        public double[] getMatrix() {
            return matrix.clone();
        }

        @Override
        public double[] getInverseMatrix() {
            return inverseMatrix.clone();
        }
    }

    /**
     * An orthographic transformation.
     */
    private static class OrthographicTransformation extends AbstractTransformation {

        private final double[] matrix;
        private final double[] inverseMatrix;
        private final boolean isIdentity;

        /**
         * Default constructor.
         * @param left   the left plane distance to origin.
         * @param right  the right plane distance to origin.
         * @param bottom the bottom plane distance to origin.
         * @param top    the top plane distance to origin.
         * @param near   the near plane distance to origin.
         * @param far    the far plane distance to origin.
         */
        public OrthographicTransformation(double left, double right, double bottom, double top, double near, double far) {
            double tx = (right + left) / (left - right);
            double ty = (top + bottom) / (bottom - top);
            double tz = (far + near)   / (near - far);

            matrix = new double[] {
                2 / (right - left), 0,                  0,                  0,
                0,                  2 / (top - bottom), 0,                  0,
                0,                  0,                  2 / (near - far),   0,
                tx,                 ty,                 tz,                 1
            };

            inverseMatrix = new double[] {
                (right - left) / 2, 0,                  0,                  0,
                0,                  (top - bottom) / 2, 0,                  0,
                0,                  0,                  (near - far) / 2,   0,
                (right + left) / 2, (top + bottom) / 2, -(near + far) / 2,  1
            };

            isIdentity = ((left == -1) && (right == 1) && (bottom == -1) && (top == 1) && (far == -1) && (near == 1));

        }

        @Override
        public boolean isIdentity() {
            return isIdentity;
        }

        @Override
        public double[] getMatrix() {
            return matrix.clone();
        }

        @Override
        public double[] getInverseMatrix() {
            return inverseMatrix.clone();
        }
    }

    /**
     * A rotation transformation.
     */
    private static class RotationTransformation extends AbstractTransformation {

        private final double[] matrix;
        private final double[] inverseMatrix;

        /**
         * Default constructor.
         * @param q the quaternion.
         */
        public RotationTransformation(Rotation q) {
            matrix = q.getRotationMatrix();
            inverseMatrix = q.getUnRotateMatrix();
        }

        @Override
        public boolean isIdentity() {
            return false;
        }

        @Override
        public double[] getMatrix() {
            return matrix.clone();
        }

        @Override
        public double[] getInverseMatrix() {
            return inverseMatrix.clone();
        }
    }

    /**
     * A product transformation.
     */
    private static class ProductTransformation extends AbstractTransformation {

        private final double[] matrix;
        private final double[] inverseMatrix;
        private final boolean isIdentity;

        /**
         * Default constructor.
         * @param t1 first transformation.
         * @param t2 first transformation.
         */
        public ProductTransformation(Transformation t1, Transformation t2) {
            isIdentity = t1.isIdentity() && t2.isIdentity();
            matrix = multiply(t1.getMatrix(), t2.getMatrix());

            // TODO : inverse 'matrix' instead.
            inverseMatrix = multiply(t2.getInverseMatrix(), t1.getInverseMatrix());
        }

        @Override
        public boolean isIdentity() {
            return isIdentity;
        }

        @Override
        public double[] getMatrix() {
            return matrix.clone();
        }

        @Override
        public double[] getInverseMatrix() {
            return inverseMatrix.clone();
        }
    }

    /**
     * Multiply two matrix.
     * @param matrix1 first matrix.
     * @param matrix2 second matrix.
     * @return matrix1 x matrix2
     */
    private static double[] multiply(double[] matrix1, double[] matrix2) {
        double[] result = new double[16];
        for (int l = 0; l < 4; l++) {
            for (int c = 0; c < 4; c++) {
                double r = 0;
                for (int i = 0; i < 4; i++) {
                    r += matrix1[(4 * i) + c] * matrix2[(4 * l) + i];
                }
                result[(4 * l) + c] = r;
            }
        }
        return result;
    }
}
