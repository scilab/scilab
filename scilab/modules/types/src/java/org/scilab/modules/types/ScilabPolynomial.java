/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.types;

import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;
import java.util.Arrays;

/**
 * This class provides a representation on the Scilab Polynomial datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * TODO Sly: faire de la doc
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabPolynomial implements ScilabType {

    private static final long serialVersionUID = 870624048944109684L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_poly;

    private static final int VERSION = 0;

    private double[][][] realPart;
    private double[][][] imaginaryPart;

    private String varName;
    private String polyVarName = "x";
    private boolean swaped;

    /**
     * Default constructor
     */
    public ScilabPolynomial() {
        realPart = null;
        imaginaryPart = null;
    }

    /**
     * Constructor with a unique polynomial.
     *
     * @param data
     *            the unique polynomial
     */
    public ScilabPolynomial(double[] data) {
        realPart = new double[1][1][];
        realPart[0][0] = data;
        imaginaryPart = null;
    }

    /**
     * Constructor with a unique polynomial.
     *
     * @param data
     *            the unique polynomial
     */
    public ScilabPolynomial(double[] data, String polyVarName) {
        this(data);
        setPolyVarName(polyVarName);
    }

    /**
     * Constructor with a unique complex polynomial.
     *
     * @param realData
     *            the real part
     * @param imagData
     *            the complex part
     */
    public ScilabPolynomial(double[] realData, double[] imagData) {
        realPart = new double[1][1][];
        realPart[0][0] = realData;
        imaginaryPart = new double[1][1][];
        imaginaryPart[0][0] = imagData;
    }

    /**
     * Constructor with a unique complex polynomial.
     *
     * @param realData
     *            the real part
     * @param imagData
     *            the complex part
     */
    public ScilabPolynomial(double[] realData, double[] imagData, String polyVarName) {
        this(realData, imagData);
        setPolyVarName(polyVarName);
    }

    /**
     * Constructor with a matrix of real polynomials.
     *
     * @param data
     *            the data
     */
    public ScilabPolynomial(double[][][] data) {
        realPart = data;
        imaginaryPart = null;
    }

    /**
     * Constructor with a matrix of real polynomials.
     *
     * @param data
     *            the data
     */
    public ScilabPolynomial(double[][][] data, String polyVarName) {
        this(data);
        setPolyVarName(polyVarName);
    }

    /**
     * Constructor with a matrix of complex polynomials
     *
     * @param realData
     *            the real part of the data
     * @param imagData
     *            the imaginary part of the data
     */
    public ScilabPolynomial(double[][][] realData, double[][][] imagData) {
        realPart = realData;
        imaginaryPart = imagData;
    }

    /**
     * Constructor with a matrix of complex polynomials
     *
     * @param realData
     *            the real part of the data
     * @param imagData
     *            the imaginary part of the data
     */
    public ScilabPolynomial(double[][][] realData, double[][][] imagData, String polyVarName) {
        this(realData, imagData);
        setPolyVarName(polyVarName);
    }

    /**
     * Constructor with a matrix of complex polynomials
     *
     * @param varName
     *            the variable name
     * @param polyVarName
     *            the polynomial variable name
     * @param realData
     *            the real part of the data
     * @param imagData
     *            the imaginary part of the data
     * @param swaped
     *            if true the matrix is stored row by row
     */
    public ScilabPolynomial(String varName, String polyVarName, double[][][] realData, double[][][] imagData, boolean swaped) {
        this.varName = varName;
        this.polyVarName = polyVarName;
        this.swaped = swaped;
        realPart = realData;
        imaginaryPart = imagData;
    }

    /**
     * {@inheritDoc}
     */
    public boolean isReference() {
        return false;
    }

    /**
     * Return the type of Scilab
     *
     * @return the type of Scilab
     * @since 5.4.0
     */
    @Override
    public ScilabTypeEnum getType() {
        return type;
    }

    /**
     * Check the emptiness of the associated data.
     *
     * @return true, if the associated data array is empty.
     */
    @Override
    public boolean isEmpty() {
        return (realPart == null && imaginaryPart == null);
    }

    /**
     * Check if the current data doesn't have an imaginary part.
     *
     * @return true, if the data are real only.
     */
    public boolean isReal() {
        return (imaginaryPart == null);
    }

    /**
     * Get the real part of the data.
     *
     * @return the real part.
     */
    public double[][][] getRealPart() {
        return realPart;
    }

    /**
     * Set the real part of the data.
     *
     * @param realPart
     *            the real part.
     */
    public void setRealPart(double[][][] realPart) {
        this.realPart = realPart;
    }

    /**
     * Get the imaginary part of the data.
     *
     * @return the imaginary part.
     */
    public double[][][] getImaginaryPart() {
        return imaginaryPart;
    }

    /**
     * Set the imaginary part of the data.
     *
     * @param imaginaryPart
     *            the imaginary part.
     */
    public void setImaginaryPart(double[][][] imaginaryPart) {
        this.imaginaryPart = imaginaryPart;
    }

    /**
     * Get the polynomila variable name
     *
     * @return the name
     */
    public String getPolyVarName() {
        return polyVarName;
    }

    public void setPolyVarName(String polyVarName) {
        this.polyVarName = polyVarName;
    }

    /**
     * {@inheritDoc}
     */
    public String getVarName() {
        return varName;
    }

    /**
     * {@inheritDoc}
     */
    public boolean isSwaped() {
        return swaped;
    }

    /**
     * Get complex matrix as a serialized form
     *
     * @return the serialized matrix with complex values
     */
    // TODO Sly : serializer ce machin
    public double[][] getSerializedComplexPolynomial() {
        return new double[0][0];
    }

    /**
     * @return the height of the data matrix
     * @see org.scilab.modules.types.ScilabType#getHeight()
     */
    @Override
    public int getHeight() {
        if (isEmpty()) {
            return 0;
        }
        return realPart.length;
    }

    /**
     * @return the width of the data matrix
     * @see org.scilab.modules.types.ScilabType#getWidth()
     */
    @Override
    public int getWidth() {
        if (isEmpty() || realPart.length == 0) {
            return 0;
        }

        return realPart[0].length;
    }


    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + Arrays.deepHashCode(imaginaryPart);
        result = prime * result + ((polyVarName == null) ? 0 : polyVarName.hashCode());
        result = prime * result + Arrays.deepHashCode(realPart);
        return result;
    }

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabPolynomial) {
            ScilabPolynomial sciPoly = ((ScilabPolynomial) obj);
            if (sciPoly.getPolyVarName().equals(getPolyVarName())) {
                if (this.isReal() && sciPoly.isReal()) {
                    return Arrays.deepEquals(this.getRealPart(), sciPoly.getRealPart());
                } else {
                    /* Complex */
                    return Arrays.deepEquals(this.getRealPart(), sciPoly.getRealPart())
                           && Arrays.deepEquals(this.getImaginaryPart(), sciPoly.getImaginaryPart());
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    /**
     * {@inheritDoc}
     */
    public Object getSerializedObject() {
        if (isReal()) {
            return new Object[] { polyVarName, realPart };
        } else {
            return new Object[] { polyVarName, realPart, imaginaryPart };
        }
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        int version = in.readInt();
        switch (version) {
            case 0:
                realPart = (double[][][]) in.readObject();
                imaginaryPart = (double[][][]) in.readObject();
                varName = (String) in.readObject();
                polyVarName = in.readUTF();
                swaped = in.readBoolean();
                break;
            default:
                throw new ClassNotFoundException("A class ScilabPolynomial with a version " + version + " does not exists");
        }
    }

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeInt(VERSION);
        out.writeObject(realPart);
        out.writeObject(imaginaryPart);
        out.writeObject(varName);
        out.writeUTF(polyVarName);
        out.writeBoolean(swaped);
    }

    /**
     * Display the representation in the Scilab language of the type<BR>
     * Note that the representation can be copied/pasted straight into Scilab
     *
     * @return a Scilab-like String representation of the data.
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();

        if (isEmpty()) {
            result.append("[]");
            return result.toString();
        }

        result.append("[");

        for (int i = 0; i < getHeight(); ++i) {
            for (int j = 0; j < getWidth(); ++j) {
                result.append("poly([");
                for (int k = 0; k < realPart[i][j].length; k++) {
                    if (isReal()) {
                        result.append(Double.toString(realPart[i][j][k]));
                    } else {
                        result.append(Double.toString(realPart[i][j][k]));
                        if (imaginaryPart[i][j][k] != 0) {
                            result.append("+");
                            result.append(Double.toString(imaginaryPart[i][j][k]));
                            result.append("*%i");
                        }
                    }
                    if (k < realPart[i][j].length - 1) {
                        result.append(", ");
                    }
                }
                result.append("], \"");
                result.append(getPolyVarName());
                result.append("\", \"coeff\")");

                if (j < getWidth() - 1) {
                    result.append(", ");
                }
            }
            if (i < getHeight() - 1) {
                result.append(" ; ");
            }
        }
        result.append("]");

        return result.toString();
    }
}
