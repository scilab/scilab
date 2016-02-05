/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *  Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.nio.ShortBuffer;

/**
 * This class provides a representation on the Scilab Integer datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * Example:<BR>
 * <code>
 * byte [][]a={{32,42,41}, {12,13,32}};<BR>
 * ScilabInteger aMatrix = new ScilabInteger(a, true); // true = unsigned
 * </code>
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabIntegerReference extends ScilabInteger {

    private LongBuffer longBuffer;
    private IntBuffer intBuffer;
    private ShortBuffer shortBuffer;
    private ByteBuffer byteBuffer;
    private final int nbRows;
    private final int nbCols;

    /**
     * Default constructor
     */
    public ScilabIntegerReference() {
        super();
        nbRows = 0;
        nbCols = 0;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    ScilabIntegerReference(String varName, ByteBuffer data, int nbRows, int nbCols, boolean bUnsigned) {
        this.varName = varName;
        this.byteBuffer = data;
        this.nbRows = nbRows;
        this.nbCols = nbCols;
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint8 : ScilabIntegerTypeEnum.sci_int8;
        this.byref = true;
        this.swaped = false;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    ScilabIntegerReference(String varName, ShortBuffer data, int nbRows, int nbCols, boolean bUnsigned) {
        this.varName = varName;
        this.shortBuffer = data;
        this.nbRows = nbRows;
        this.nbCols = nbCols;
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint16 : ScilabIntegerTypeEnum.sci_int16;
        this.byref = true;
        this.swaped = false;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    ScilabIntegerReference(String varName, IntBuffer data, int nbRows, int nbCols, boolean bUnsigned) {
        this.varName = varName;
        this.intBuffer = data;
        this.nbRows = nbRows;
        this.nbCols = nbCols;
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint32 : ScilabIntegerTypeEnum.sci_int32;
        this.byref = true;
        this.swaped = false;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    ScilabIntegerReference(String varName, LongBuffer data, int nbRows, int nbCols, boolean bUnsigned) {
        this.varName = varName;
        this.longBuffer = data;
        this.nbRows = nbRows;
        this.nbCols = nbCols;
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint64 : ScilabIntegerTypeEnum.sci_int64;
        this.byref = true;
        this.swaped = false;
    }

    /**
     * {@inheritDoc}
     */
    public byte getByteElement(final int i, final int j) {
        return byteBuffer.get(i + nbRows * j);
    }

    /**
     * {@inheritDoc}
     */
    public short getShortElement(final int i, final int j) {
        return shortBuffer.get(i + nbRows * j);
    }

    /**
     * {@inheritDoc}
     */
    public int getIntElement(final int i, final int j) {
        return intBuffer.get(i + nbRows * j);
    }

    /**
     * {@inheritDoc}
     */
    public long getLongElement(final int i, final int j) {
        return longBuffer.get(i + nbRows * j);
    }

    /**
     * {@inheritDoc}
     */
    public void setByteElement(final int i, final int j, final byte x) {
        byteBuffer.put(i + nbRows * j, x);
    }

    /**
     * {@inheritDoc}
     */
    public void setShortElement(final int i, final int j, final short x) {
        shortBuffer.put(i + nbRows * j, x);
    }

    /**
     * {@inheritDoc}
     */
    public void setIntElement(final int i, final int j, final int x) {
        intBuffer.put(i + nbRows * j, x);
    }

    /**
     * {@inheritDoc}
     */
    public void setLongElement(final int i, final int j, final long x) {
        longBuffer.put(i + nbRows * j, x);
    }

    /**
     * Set the current values
     *
     * @param data
     *            the values to set
     * @param bUnsigned
     *            true, if these values are unsigned; false otherwise.
     */
    public void setData(byte[][] data, boolean bUnsigned) {
        ScilabTypeUtils.setPart(byteBuffer, data);
        if (bUnsigned) {
            this.precision = ScilabIntegerTypeEnum.sci_uint8;
        } else {
            this.precision = ScilabIntegerTypeEnum.sci_int8;
        }
    }

    /**
     * Set the current values
     *
     * @param data
     *            the values to set
     * @param bUnsigned
     *            true, if these values are unsigned; false otherwise.
     */
    public void setData(short[][] data, boolean bUnsigned) {
        ScilabTypeUtils.setPart(shortBuffer, data);
        if (bUnsigned) {
            this.precision = ScilabIntegerTypeEnum.sci_uint16;
        } else {
            this.precision = ScilabIntegerTypeEnum.sci_int16;
        }
    }

    /**
     * Set the current values
     *
     * @param data
     *            the values to set
     * @param bUnsigned
     *            true, if these values are unsigned; false otherwise.
     */
    public void setData(int[][] data, boolean bUnsigned) {
        ScilabTypeUtils.setPart(intBuffer, data);
        if (bUnsigned) {
            this.precision = ScilabIntegerTypeEnum.sci_uint32;
        } else {
            this.precision = ScilabIntegerTypeEnum.sci_int32;
        }
    }

    /**
     * Set the current values
     *
     * @param data
     *            the values to set
     * @param bUnsigned
     *            true, if these values are unsigned; false otherwise.
     */
    public void setData(long[][] data, boolean bUnsigned) {
        ScilabTypeUtils.setPart(longBuffer, data);
        if (bUnsigned) {
            this.precision = ScilabIntegerTypeEnum.sci_uint64;
        } else {
            this.precision = ScilabIntegerTypeEnum.sci_int64;
        }
    }

    /**
     * Returns the value as the form of short
     *
     * @return the values as short
     */
    public short[][] getDataAsShort() {
        short[][] d = new short[nbRows][nbCols];
        ScilabTypeUtils.setBuffer(d, shortBuffer);

        return d;
    }

    /**
     * Returns the value as the form of byte
     *
     * @return the values as byte
     */
    public byte[][] getDataAsByte() {
        byte[][] d = new byte[nbRows][nbCols];
        ScilabTypeUtils.setBuffer(d, byteBuffer);

        return d;
    }

    /**
     * Returns the value as the form of int
     *
     * @return the values as int
     */
    public int[][] getDataAsInt() {
        int[][] d = new int[nbRows][nbCols];
        ScilabTypeUtils.setBuffer(d, intBuffer);

        return d;
    }

    /**
     * Returns the value as the form of long Only for Scilab 6.X
     *
     * @return the values as long
     */
    public long[][] getDataAsLong() {
        long[][] d = new long[nbRows][nbCols];
        ScilabTypeUtils.setBuffer(d, longBuffer);

        return d;
    }

    /**
     * @return the height of the value matrix
     * @see org.scilab.modules.types.ScilabType#getHeight()
     */
    @Override
    public int getHeight() {
        return nbRows;
    }

    /**
     * @return the width of the value matrix
     * @see org.scilab.modules.types.ScilabType#getWidth()
     */
    @Override
    public int getWidth() {
        return nbCols;
    }

    /**
     * @return true, if there is no values; false otherwise.
     */
    @Override
    public boolean isEmpty() {
        return nbRows == 0 || nbCols == 0;
    }

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabInteger) {
            ScilabInteger sciInt = (ScilabInteger) obj;
            if (isEmpty() && sciInt.isEmpty()) {
                return true;
            }

            if (this.getWidth() != sciInt.getWidth() || this.getHeight() != sciInt.getHeight()) {
                return false;
            }

            return ScilabTypeUtils.equalsInteger(this.getRawData(), this.isSwaped(), sciInt.getRawData(), sciInt.isSwaped());
        } else {
            return false;
        }
    }

    /**
     * {@inheritDoc}
     */
    public Object getRawData() {
        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                return byteBuffer;
            case sci_int16:
            case sci_uint16:
                return shortBuffer;
            case sci_int32:
            case sci_uint32:
                return intBuffer;
            case sci_int64:
            case sci_uint64:
                return longBuffer;
        }

        return null;
    }

    /**
     * {@inheritDoc}
     */
    public Object getCorrectData() {
        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                return getDataAsByte();
            case sci_int16:
            case sci_uint16:
                return getDataAsShort();
            case sci_int32:
            case sci_uint32:
                return getDataAsInt();
            case sci_int64:
            case sci_uint64:
                return getDataAsLong();
        }

        return null;
    }

    /**
     * When this object is deserialized we want a ScilabInteger, not a ScilabIntegerReference.
     * @return a ScilabInteger
     */
    private Object readResolve() throws ObjectStreamException {
        switch (precision) {
            case sci_int8:
                return new ScilabInteger(varName, byteData, false, swaped);
            case sci_uint8:
                return new ScilabInteger(varName, byteData, true, swaped);
            case sci_int16:
                return new ScilabInteger(varName, shortData, false, swaped);
            case sci_uint16:
                return new ScilabInteger(varName, shortData, true, swaped);
            case sci_int32:
                return new ScilabInteger(varName, intData, false, swaped);
            case sci_uint32:
                return new ScilabInteger(varName, intData, true, swaped);
            case sci_int64:
                return new ScilabInteger(varName, longData, false, swaped);
            case sci_uint64:
                return new ScilabInteger(varName, longData, true, swaped);
        }

        return this;
    }
}
