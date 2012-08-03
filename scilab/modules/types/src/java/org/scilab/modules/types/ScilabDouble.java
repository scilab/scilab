/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2010-2010 - DIGITEO - Clement DAVID
 *  Copyright (C) 2011-2011 - DIGITEO - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
 * Example (real):<br />
 * <code>
 * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<br />
 * ScilabDouble aMatrix = new ScilabDouble(a);<br />
 * </code> <br>
 * Example (complex):<br />
 * <code>
 * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<br />
 * double [][]aImg={{210.2, 220.0, 420.0, 390.0},{230.2, 240.0, 440.0, 400.0}};<br />
 * ScilabDouble aMatrix = new ScilabDouble(a, aImg);
 * </code>
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabDouble implements ScilabType {

    private static final long serialVersionUID = 879624048944109684L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_matrix;

    private static final int VERSION = 0;

    private double[][] realPart;
    private double[][] imaginaryPart;
    private String varName;
    private boolean swaped;

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
     * Get complex matrix as a serialized form
     *
     * @return the serialized matrix with complex values
     */
    public double[] getSerializedComplexMatrix() {
        int size = this.getHeight() * this.getWidth();
        double[] serializedComplexMatrix = new double[size * 2];
        for (int i = 0; i < this.getHeight(); i++) {
            for (int j = 0; j < this.getWidth(); j++) {
                serializedComplexMatrix[j * this.getHeight() + i] = realPart[i][j];
                serializedComplexMatrix[size + j * this.getHeight() + i] = imaginaryPart[i][j];
            }
        }

        return serializedComplexMatrix;
    }

    /**
     * {@inheritDoc}
     */
    public Object getSerializedObject() {
        if (isReal()) {
            return new Object[] { realPart };
        } else {
            return new Object[] { realPart, imaginaryPart };
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

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabDouble) {
            ScilabDouble sciDouble = ((ScilabDouble) obj);
            if (isEmpty() && sciDouble.isEmpty()) {
                return true;
            }

            if (this.isReal() && sciDouble.isReal()) {
                return Arrays.deepEquals(this.getRealPart(), sciDouble.getRealPart());
            } else {
                /* Complex */
                return Arrays.deepEquals(this.getRealPart(), sciDouble.getRealPart())
                       && Arrays.deepEquals(this.getImaginaryPart(), sciDouble.getImaginaryPart());
            }
        } else {
            return false;
        }
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
        out.writeObject(realPart);
        out.writeObject(imaginaryPart);
        out.writeObject(varName);
        out.writeBoolean(swaped);
    }

    /**
     * Display the representation in the Scilab language of the type<br />
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
        for (int i = 0; i < getHeight(); ++i) {
            for (int j = 0; j < getWidth(); ++j) {
                if (isReal()) {
                    result.append(Double.toString(realPart[i][j]));
                } else {
                    result.append(Double.toString(realPart[i][j]));
                    result.append(" + ");
                    result.append(Double.toString(imaginaryPart[i][j]));
                    result.append(" * %i");
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
