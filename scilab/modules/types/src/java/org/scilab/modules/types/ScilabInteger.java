/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
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
 * This class provides a representation on the Scilab Integer datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * Example:<br />
 * <code>
 * byte [][]a={{32,42,41}, {12,13,32}};<br />
 * ScilabInteger aMatrix = new ScilabInteger(a, true); // true = unsigned
 * </code>
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabInteger implements ScilabType {

    private static final long serialVersionUID = 1759633801332932450L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_ints;

    private static final int VERSION = 0;

    private long[][] longData = null;
    private short[][] shortData = null;
    private int[][] intData = null;
    private byte[][] byteData = null;
    private ScilabIntegerTypeEnum precision;
    private String varName;
    private boolean swaped;

    /**
     * Default constructor
     */
    public ScilabInteger() {

    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(byte[][] data, boolean bUnsigned) {
        this.setData(data, bUnsigned);
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(short[][] data, boolean bUnsigned) {
        this.setData(data, bUnsigned);
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(int[][] data, boolean bUnsigned) {
        this.setData(data, bUnsigned);
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(long[][] data, boolean bUnsigned) {
        this.setData(data, bUnsigned);
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(String varName, byte[][] data, boolean bUnsigned, boolean swaped) {
        this.setData(data, bUnsigned);
        this.varName = varName;
        this.swaped = swaped;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(String varName, short[][] data, boolean bUnsigned, boolean swaped) {
        this.setData(data, bUnsigned);
        this.varName = varName;
        this.swaped = swaped;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(String varName, int[][] data, boolean bUnsigned, boolean swaped) {
        this.setData(data, bUnsigned);
        this.varName = varName;
        this.swaped = swaped;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(String varName, long[][] data, boolean bUnsigned, boolean swaped) {
        this.setData(data, bUnsigned);
        this.varName = varName;
        this.swaped = swaped;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabInteger(byte value) {
        this.byteData = new byte[1][1];
        this.byteData[0][0] = value;
        this.precision = ScilabIntegerTypeEnum.sci_int8;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabInteger(short value) {
        this.shortData = new short[1][1];
        this.shortData[0][0] = value;
        this.precision = ScilabIntegerTypeEnum.sci_int16;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabInteger(int value) {
        this.intData = new int[1][1];
        this.intData[0][0] = value;
        this.precision = ScilabIntegerTypeEnum.sci_int32;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabInteger(long value) {
        this.longData = new long[1][1];
        this.longData[0][0] = value;
        this.precision = ScilabIntegerTypeEnum.sci_int64;
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
        this.byteData = data;
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
        this.shortData = data;
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
        this.intData = data;
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
        this.longData = data;
        if (bUnsigned) {
            this.precision = ScilabIntegerTypeEnum.sci_uint64;
        } else {
            this.precision = ScilabIntegerTypeEnum.sci_int64;
        }
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
     * If the precision is not 64, all values will be converted to long
     * (attention, the convertion can be long) if precision is 64, just return
     * the data
     *
     * @return the values
     */
    public long[][] getData() {
        long[][] convertedMatrix = new long[this.getHeight()][this.getWidth()];
        switch (this.getPrec()) {

            case sci_int8:
            case sci_uint8:
                for (int i = 0; i < this.getHeight(); i++) {
                    for (int j = 0; j < this.getWidth(); j++) {
                        convertedMatrix[i][j] = Long.valueOf(byteData[i][j]);
                    }
                }
                return convertedMatrix;
            case sci_int16:
            case sci_uint16:
                for (int i = 0; i < this.getHeight(); i++) {
                    for (int j = 0; j < this.getWidth(); j++) {
                        convertedMatrix[i][j] = Long.valueOf(shortData[i][j]);
                    }
                }
                return convertedMatrix;
            case sci_int32:
            case sci_uint32:
                for (int i = 0; i < this.getHeight(); i++) {
                    for (int j = 0; j < this.getWidth(); j++) {
                        convertedMatrix[i][j] = Long.valueOf(intData[i][j]);
                    }
                }
                return convertedMatrix;
            case sci_int64:
            case sci_uint64:
                return longData;
        }
        return null;
    }

    /**
     * Returns the value as the form of short
     *
     * @return the values as short
     */
    public short[][] getDataAsShort() {
        return shortData;
    }

    /**
     * Returns the value as the form of byte
     *
     * @return the values as byte
     */
    public byte[][] getDataAsByte() {
        return byteData;
    }

    /**
     * Returns the value as the form of int
     *
     * @return the values as int
     */
    public int[][] getDataAsInt() {
        return intData;
    }

    /**
     * Returns the value as the form of long Only for Scilab 6.X
     *
     * @return the values as long
     */
    public long[][] getDataAsLong() {
        return longData;
    }

    /**
     * @return the precision of the values
     */
    public ScilabIntegerTypeEnum getPrec() {
        return precision;
    }

    /**
     * @return true, if the values are signed, false otherwise.
     */
    public boolean isUnsigned() {
        switch (precision) {
            case sci_int8:
            case sci_int16:
            case sci_int32:
            case sci_int64:
                return false;
            case sci_uint8:
            case sci_uint16:
            case sci_uint32:
            case sci_uint64:
                return true;
        }
        return false;
    }

    /**
     * Manage the old representation of IntegerType
     *
     * @param typeName
     *            the typeName (TYPE8, TYPE16, TYPE32, TYPE64)
     * @param unsigned
     *            unsigned or not
     * @return the converted type to ScilabIntegerTypeEnum. null is cannot
     *         convert
     */
    public static ScilabIntegerTypeEnum convertOldType(String typeName, boolean unsigned) {
        if (typeName.equals("TYPE8")) {
            if (unsigned) {
                return ScilabIntegerTypeEnum.sci_uint8;
            } else {
                return ScilabIntegerTypeEnum.sci_int8;
            }
        }
        if (typeName.equals("TYPE16")) {
            if (unsigned) {
                return ScilabIntegerTypeEnum.sci_uint16;
            } else {
                return ScilabIntegerTypeEnum.sci_int16;
            }
        }
        if (typeName.equals("TYPE32")) {
            if (unsigned) {
                return ScilabIntegerTypeEnum.sci_uint32;
            } else {
                return ScilabIntegerTypeEnum.sci_int32;
            }
        }
        if (typeName.equals("TYPE64")) {
            if (unsigned) {
                return ScilabIntegerTypeEnum.sci_uint64;
            } else {
                return ScilabIntegerTypeEnum.sci_int64;
            }
        }
        return null;

    }

    /**
     * @return the height of the value matrix
     * @see org.scilab.modules.types.ScilabType#getHeight()
     */
    @Override
    public int getHeight() {
        if (this.getPrec() == null) {
            return 0;
        }
        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                if (byteData == null) {
                    return 0;
                }
                return byteData.length;
            case sci_int16:
            case sci_uint16:
                if (shortData == null) {
                    return 0;
                }
                return shortData.length;
            case sci_int32:
            case sci_uint32:
                if (intData == null) {
                    return 0;
                }
                return intData.length;
            case sci_int64:
            case sci_uint64:
                if (longData == null) {
                    return 0;
                }
                return longData.length;
            default:
                return 0;
        }
    }

    /**
     * @return the width of the value matrix
     * @see org.scilab.modules.types.ScilabType#getWidth()
     */
    @Override
    public int getWidth() {
        if (this.getPrec() == null) {
            return 0;
        }
        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                if (byteData == null) {
                    return 0;
                }
                return byteData[0].length;
            case sci_int16:
            case sci_uint16:
                if (shortData == null) {
                    return 0;
                }
                return shortData[0].length;
            case sci_int32:
            case sci_uint32:
                if (intData == null) {
                    return 0;
                }
                return intData[0].length;
            case sci_int64:
            case sci_uint64:
                if (longData == null) {
                    return 0;
                }
                return longData[0].length;
            default:
                return 0;
        }
    }

    // int32(X), int8(x) , int16([x,x,x;x,x,x])
    // uint32(X), uint8(x) , uint16([x,x,x;x,x,x])

    /**
     * @return true, if there is no values; false otherwise.
     */
    @Override
    public boolean isEmpty() {
        if (this.getPrec() == null) {
            return true;
        }
        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                return byteData == null;
            case sci_int16:
            case sci_uint16:
                return shortData == null;
            case sci_int32:
            case sci_uint32:
                return intData == null;
            case sci_int64:
            case sci_uint64:
                return longData == null;
            default:
                return true;
        }
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
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabInteger) {
            return Arrays.deepEquals(this.getData(), ((ScilabInteger) obj).getData());
        } else {
            return false;
        }
    }

    private Object getCorrectData() {
        switch (this.getPrec()) {
            case sci_int8:
                return byteData;
            case sci_uint8:
                return byteData;
            case sci_int16:
                return shortData;
            case sci_uint16:
                return shortData;
            case sci_int32:
                return intData;
            case sci_uint32:
                return intData;
            case sci_int64:
                return longData;
            case sci_uint64:
                return longData;
        }
        return null;
    }

    /**
     * {@inheritDoc}
     */
    public Object getSerializedObject() {
        return new Object[] { new int[] { this.getPrec().swigValue() }, getCorrectData() };
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        int version = in.readInt();
        switch (version) {
            case 0:
                precision = ScilabIntegerTypeEnum.swigToEnum(in.readInt());
                Object data = in.readObject();
                switch (precision) {
                    case sci_int8:
                    case sci_uint8:
                        byteData = (byte[][]) data;
                        break;
                    case sci_int16:
                    case sci_uint16:
                        shortData = (short[][]) data;
                        break;
                    case sci_int32:
                    case sci_uint32:
                        intData = (int[][]) data;
                        break;
                    case sci_int64:
                    case sci_uint64:
                        longData = (long[][]) data;
                        break;
                }
                varName = (String) in.readObject();
                swaped = in.readBoolean();
                break;
            default:
                throw new ClassNotFoundException("A class ScilabInteger with a version " + version + " does not exists");
        }
    }

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeInt(VERSION);
        out.writeInt(getPrec().swigValue());
        out.writeObject(getCorrectData());
        out.writeObject(varName);
        out.writeBoolean(swaped);
    }

    /**
     * Display the representation in the Scilab language of the type<br />
     * Note that the representation can be copied/pasted straight into Scilab
     *
     * @return the pretty-printed values
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();

        if (isEmpty()) {
            result.append("int([])");
            return result.toString();
        }

        if (isUnsigned()) {
            result.append("u");
        }
        result.append("int");

        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                result.append("8");
                break;

            case sci_int16:
            case sci_uint16:
                result.append("16");
                break;

            case sci_int32:
            case sci_uint32:
                result.append("32");
                break;

            case sci_int64:
            case sci_uint64:
                result.append("64");
                break;

            default:
                break;
        }

        result.append("([");
        appendData(result);
        result.append("])");

        return result.toString();
    }

    /**
     * Put each value on the buffer.
     *
     * @param result
     *            the current buffer
     */
    private void appendData(StringBuilder result) {
        for (int i = 0; i < getHeight(); ++i) {
            for (int j = 0; j < getWidth(); ++j) {

                result.append(getData()[i][j]);

                if (j != getWidth() - 1) {
                    result.append(", ");
                }
            }
            if (i != getHeight() - 1) {
                result.append(" ; ");
            }
        }
    }

}
