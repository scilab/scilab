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

import java.io.ObjectStreamException;
import java.nio.DoubleBuffer;

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
public class ScilabDoubleReference extends ScilabDouble {

    private DoubleBuffer realBuffer;
    private DoubleBuffer imaginaryBuffer;
    private final int nbRows;
    private final int nbCols;

    /**
     * Constructor with a matrix of complex numbers
     *
     * @param realData
     *            the real part of the data
     * @param imagData
     *            the imaginary part of the data
     */
    ScilabDoubleReference(String varName, DoubleBuffer realBuffer, DoubleBuffer imagBuffer, int nbRows, int nbCols) {
        this.realBuffer = realBuffer;
        this.imaginaryBuffer = imagBuffer;
        this.nbRows = nbRows;
        this.nbCols = nbCols;
        this.varName = varName;
        this.byref = true;
        this.swaped = false;
    }

    /**
     * Check the emptiness of the associated data.
     *
     * @return true, if the associated data array is empty.
     */
    @Override
    public boolean isEmpty() {
        return nbRows == 0 || nbCols == 0;
    }

    /**
     * Check if the current data doesn't have an imaginary part.
     *
     * @return true, if the data are real only.
     */
    public boolean isReal() {
        return imaginaryBuffer == null || imaginaryBuffer.capacity() == 0;
    }

    /**
     * {@inheritDoc}
     */
    public Object getRawRealPart() {
        return realBuffer;
    }

    /**
     * {@inheritDoc}
     */
    public Object getRawImaginaryPart() {
        return imaginaryBuffer;
    }

    /**
     * Get the real part as DoubleBuffer
     * @return the real part
     */
    public DoubleBuffer getRealBuffer() {
        return realBuffer;
    }

    /**
     * Get the imaginary part as DoubleBuffer
     * @return the imaginary part
     */
    public DoubleBuffer getImaginaryBuffer() {
        return imaginaryBuffer;
    }

    /**
     * Get the real part of the data.
     *
     * @return the real part.
     */
    public double[][] getRealPart() {
        double[][] d = new double[nbRows][nbCols];
        ScilabTypeUtils.setBuffer(d, realBuffer);
        return d;
    }

    /**
     * Set the real part of the data.
     *
     * @param realPart
     *            the real part.
     */
    public void setRealPart(double[][] realPart) {
        ScilabTypeUtils.setPart(realBuffer, realPart);
    }

    /**
     * Get the imaginary part of the data.
     *
     * @return the imaginary part.
     */
    public double[][] getImaginaryPart() {
        double[][] d = new double[nbRows][nbCols];
        ScilabTypeUtils.setBuffer(d, imaginaryBuffer);
        return d;
    }

    /**
     * Set the imaginary part of the data.
     *
     * @param imaginaryPart
     *            the imaginary part.
     */
    public void setImaginaryPart(double[][] imaginaryPart) {
        ScilabTypeUtils.setPart(imaginaryBuffer, imaginaryPart);
    }

    /**
     * {@inheritDoc}
     */
    public double getRealElement(final int i, final int j) {
        return realBuffer.get(i + nbRows * j);
    }

    /**
     * {@inheritDoc}
     */
    public double getImaginaryElement(final int i, final int j) {
        return imaginaryBuffer.get(i + nbRows * j);
    }

    /**
     * {@inheritDoc}
     */
    public void setRealElement(final int i, final int j, final double x) {
        realBuffer.put(i + nbRows * j, x);
    }

    /**
     * {@inheritDoc}
     */
    public void setImaginaryElement(final int i, final int j, final double x) {
        imaginaryBuffer.put(i + nbRows * j, x);
    }

    /**
     * {@inheritDoc}
     */
    public void setElement(final int i, final int j, final double x, final double y) {
        realBuffer.put(i + nbRows * j, x);
        imaginaryBuffer.put(i + nbRows * j, x);
    }

    /**
     * When this object is deserialized we want a ScilabDouble, not a ScilabDoubleReference.
     * @return a ScilabDouble
     */
    private Object readResolve() throws ObjectStreamException {
        return new ScilabDouble(varName, realPart, imaginaryPart, swaped);
    }

    @Override
    public int getHeight() {
        return nbRows;
    }

    @Override
    public int getWidth() {
        return nbCols;
    }
}
