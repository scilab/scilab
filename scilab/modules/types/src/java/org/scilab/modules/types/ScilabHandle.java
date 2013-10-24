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
 * This class provides a representation on the Scilab Handle datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * Example:<br />
 * <code>
 * byte [][]a={{32,42,41}, {12,13,32}};<br />
 * ScilabHandle aMatrix = new ScilabHandle(a, true); // true = unsigned
 * </code>
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabHandle implements ScilabType {

    private static final long serialVersionUID = 1759633801332932451L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_handles;

    private static final int VERSION = 0;

    private long[][] data = null;
    private String varName;
    private boolean swaped;

    /**
     * Default constructor
     */
    public ScilabHandle() {

    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabHandle(long[][] data) {
        this.data = data;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabHandle(String varName, long[][] data, boolean swaped) {
        this.data = data;
        this.varName = varName;
        this.swaped = swaped;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabHandle(long value) {
        this.data = new long[1][1];
        this.data[0][0] = value;
    }

    /**
     * Set the current values
     *
     * @param data
     *            the values to set
     * @param bUnsigned
     *            true, if these values are unsigned; false otherwise.
     */
    public void setData(long[][] data) {
        this.data = data;
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
     * If the precision is not 64, all values will be converted to long
     * (attention, the convertion can be long) if precision is 64, just return
     * the data
     *
     * @return the values
     */
    public long[][] getData() {
        return data;
    }

    /**
     * @return the height of the value matrix
     * @see org.scilab.modules.types.ScilabType#getHeight()
     */
    @Override
    public int getHeight() {
        if (data == null) {
            return 0;
        }
        return data.length;
    }

    /**
     * @return the width of the value matrix
     * @see org.scilab.modules.types.ScilabType#getWidth()
     */
    @Override
    public int getWidth() {
        if (data == null) {
            return 0;
        }
        return data[0].length;
    }

    /**
     * @return true, if there is no values; false otherwise.
     */
    @Override
    public boolean isEmpty() {
        return data == null;
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
        if (obj instanceof ScilabHandle) {
            return Arrays.deepEquals(this.getData(), ((ScilabHandle) obj).getData());
        } else {
            return false;
        }
    }

    /**
     * {@inheritDoc}
     */
    public Object getSerializedObject() {
        return data;
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        int version = in.readInt();
        switch (version) {
            case 0:
                data = (long[][]) in.readObject();
                varName = (String) in.readObject();
                swaped = in.readBoolean();
                break;
            default:
                throw new ClassNotFoundException("A class ScilabString with a version " + version + " does not exists");
        }
    }

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeInt(VERSION);
        out.writeObject(data);
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
        return "";
    }
}
