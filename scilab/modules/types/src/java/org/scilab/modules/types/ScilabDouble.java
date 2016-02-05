/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2010-2010 - DIGITEO - Clement DAVID
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
 * This class provides a representation on the Scilab Double datatype<br>
 * Note that double is the default datatype in Scilab.<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * Example (real):<BR>
 * <code>
 * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<BR>
 * ScilabDouble aMatrix = new ScilabDouble(a);<BR>
 * </code> <br>
 * Example (complex):<BR>
 * <code>
 * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<BR>
 * double [][]aImg={{210.2, 220.0, 420.0, 390.0},{230.2, 240.0, 440.0, 400.0}};<BR>
 * ScilabDouble aMatrix = new ScilabDouble(a, aImg);
 * </code>
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabDouble implements ScilabType {

    private static final long serialVersionUID = 879624048944109684L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_matrix;

    private static final int VERSION = 0;

    protected double[][] realPart;
    protected double[][] imaginaryPart;
    protected String varName;
    protected boolean swaped;
    transient protected boolean byref;

    /**
     * Default constructor
     */
    public ScilabDouble() {
        realPart = new double[0][];
        imaginaryPart = new double[0][];
    }

    /**
     * Constructor with a unique value.
     *
     * @param data
     *            the unique value
     */
    public ScilabDouble(double data) {
        realPart = new double[1][1];
        realPart[0][0] = data;
        imaginaryPart = new double[0][];
    }

    /**
     * Constructor with a unique complex value.
     *
     * @param realData
     *            the real part
     * @param imagData
     *            the complex part
     */
    public ScilabDouble(double realData, double imagData) {
        realPart = new double[1][1];
        realPart[0][0] = realData;
        imaginaryPart = new double[1][1];
        imaginaryPart[0][0] = imagData;
    }

    /**
     * Constructor with a matrix of real data.
     *
     * @param data
     *            the data
     */
    public ScilabDouble(double[][] data) {
        this(data, new double[0][]);
    }

    /**
     * Constructor with a matrix of complex numbers
     *
     * @param realData
     *            the real part of the data
     * @param imagData
     *            the imaginary part of the data
     */
    public ScilabDouble(double[][] realData, double[][] imagData) {
        if (realData == null) {
            realPart = new double[0][];
            imaginaryPart = new double[0][];
        } else {
            realPart = realData;
            if (imagData == null) {
                imaginaryPart = new double[0][];
            } else {
                imaginaryPart = imagData;
            }
        }
    }

    /**
     * Constructor with a matrix of complex numbers
     *
     * @param realData
     *            the real part of the data
     * @param imagData
     *            the imaginary part of the data
     */
    public ScilabDouble(String varName, double[][] realData, double[][] imagData, boolean swaped) {
        this(realData, imagData);
        this.varName = varName;
        this.swaped = swaped;
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
        return realPart == null || realPart.length == 0;
    }

    /**
     * {@inheritDoc}
     */
    public boolean isReference() {
        return byref;
    }

    /**
     * Check if the current data doesn't have an imaginary part.
     *
     * @return true, if the data are real only.
     */
    public boolean isReal() {
        return imaginaryPart == null || imaginaryPart.length == 0;
    }

    /**
     * Get the real part of the data.
     *
     * @return the real part.
     */
    public double[][] getRealPart() {
        return realPart;
    }

    /**
     * Set the real part of the data.
     *
     * @param realPart
     *            the real part.
     */
    public void setRealPart(double[][] realPart) {
        this.realPart = realPart;
    }

    /**
     * Get the imaginary part of the data.
     *
     * @return the imaginary part.
     */
    public double[][] getImaginaryPart() {
        return imaginaryPart;
    }

    /**
     * Set the imaginary part of the data.
     *
     * @param imaginaryPart
     *            the imaginary part.
     */
    public void setImaginaryPart(double[][] imaginaryPart) {
        this.imaginaryPart = imaginaryPart;
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
     * Get the element at position (i, j) in the real part
     * @param i the first coordinate
     * @param j the second coordinate
     * @return the corresponding double
     */
    public double getRealElement(final int i, final int j) {
        return realPart[i][j];
    }

    /**
     * Get the element at position (i, j) in the imaginary part
     * @param i the first coordinate
     * @param j the second coordinate
     * @return the corresponding double
     */
    public double getImaginaryElement(final int i, final int j) {
        return imaginaryPart[i][j];
    }

    /**
     * Get the complex at position (i, j) as a 2-length row-vector
     * @param i the first coordinate
     * @param j the second coordinate
     * @return the corresponding complex
     */
    public double[] getElement(final int i, final int j) {
        return new double[] {getRealElement(i, j), getImaginaryElement(i, j)};
    }

    /**
     * Set the element at position (i, j) in the real part
     * @param i the first coordinate
     * @param j the second coordinate
     * @param x the new value
     */
    public void setRealElement(final int i, final int j, final double x) {
        realPart[i][j] = x;
    }

    /**
     * Set the element at position (i, j) in the imaginary part
     * @param i the first coordinate
     * @param j the second coordinate
     * @param x the new value
     */
    public void setImaginaryElement(final int i, final int j, final double x) {
        imaginaryPart[i][j] = x;
    }

    /**
     * Set the element at position (i, j)
     * @param i the first coordinate
     * @param j the second coordinate
     * @param x the new value for real part
     * @param y the new value for imaginary part
     */
    public void setElement(final int i, final int j, final double x, final double y) {
        realPart[i][j] = x;
        imaginaryPart[i][j] = y;
    }

    /**
     * Get the raw data
     * @return the real part
     */
    public Object getRawRealPart() {
        return realPart;
    }

    /**
     * Get the raw data
     * @return the imaginary part
     */
    public Object getRawImaginaryPart() {
        return imaginaryPart;
    }

    /**
     * Get complex matrix as a serialized form
     *
     * @return the serialized matrix with complex values
     */
    public double[] getSerializedComplexMatrix() {
        int size = this.getHeight() * this.getWidth();
        double[] serializedComplexMatrix = new double[size * 2];
        for (int i = 0; i < this.getHeight(); i++) {
            for (int j = 0; j < this.getWidth(); j++) {
                serializedComplexMatrix[j * this.getHeight() + i] = getRealElement(i, j);
                serializedComplexMatrix[size + j * this.getHeight() + i] = getImaginaryElement(i, j);
            }
        }

        return serializedComplexMatrix;
    }

    /**
     * {@inheritDoc}
     */
    public Object getSerializedObject() {
        if (isReal()) {
            return new Object[] { getRealPart() };
        } else {
            return new Object[] { getRealPart(), getImaginaryPart() };
        }
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
        if (isEmpty()) {
            return 0;
        }

        return realPart[0].length;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + Arrays.deepHashCode(imaginaryPart);
        result = prime * result + Arrays.deepHashCode(realPart);
        return result;
    }

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabDouble) {
            ScilabDouble sciDouble = (ScilabDouble) obj;
            if (isEmpty() && sciDouble.isEmpty()) {
                return true;
            }

            if (this.getWidth() != sciDouble.getWidth() || this.getHeight() != sciDouble.getHeight()) {
                return false;
            }

            if (this.isReal() && sciDouble.isReal()) {
                return ScilabTypeUtils.equalsDouble(this.getRawRealPart(), this.isSwaped(), sciDouble.getRawRealPart(), sciDouble.isSwaped());
            } else if (!this.isReal() && !sciDouble.isReal()) {
                /* Complex */
                return ScilabTypeUtils.equalsDouble(this.getRawRealPart(), this.isSwaped(), sciDouble.getRawRealPart(), sciDouble.isSwaped())
                       && ScilabTypeUtils.equalsDouble(this.getRawImaginaryPart(), this.isSwaped(), sciDouble.getRawImaginaryPart(), sciDouble.isSwaped());
            }
        }

        return false;
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        int version = in.readInt();
        switch (version) {
            case 0:
                realPart = (double[][]) in.readObject();
                imaginaryPart = (double[][]) in.readObject();
                varName = (String) in.readObject();
                swaped = in.readBoolean();
                break;
            default:
                throw new ClassNotFoundException("A class ScilabDouble with a version " + version + " does not exists");
        }
    }

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeInt(VERSION);
        out.writeObject(getRealPart());
        out.writeObject(getImaginaryPart());
        out.writeObject(varName);
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
            return "[]";
        }

        result.append("[");
        final boolean real = isReal();
        for (int i = 0; i < getHeight(); ++i) {
            for (int j = 0; j < getWidth(); ++j) {
                result.append(Double.toString(getRealElement(i, j)));
                if (!real) {
                    final double y = getImaginaryElement(i, j);
                    if (y != 0) {
                        if (y > 0) {
                            result.append(" + ");
                        }
                        result.append(Double.toString(y));
                        result.append(" * %i");
                    }
                }
                if (j != getWidth() - 1) {
                    result.append(", ");
                }
            }
            if (i != getHeight() - 1) {
                result.append(" ; ");
            }
        }
        result.append("]");

        return result.toString();
    }
}
