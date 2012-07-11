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
 * This class provides a representation on the Scilab boolean datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * Example:<br />
 * <code>
 * boolean [][]a={{true,false,true}, {true,true,true}};<br />
 * ScilabBoolean aMatrix = new ScilabBoolean(a);
 * </code>
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabBoolean implements ScilabType {

    private static final long serialVersionUID = 6511497080095473901L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_boolean;

    private static final int VERSION = 0;

    /* the boolean data */
    private boolean[][] data;
    private String varName;
    private boolean swaped;

    /**
     * Create an empty object
     */
    public ScilabBoolean() {
        data = null;
    }

    /**
     * Create an object from an array of array of boolean
     *
     * @param data
     *            the array of boolean
     */
    public ScilabBoolean(boolean[][] data) {
        this.data = data;
    }

    /**
     * Create an object from an array of array of boolean
     *
     * @param data
     *            the array of boolean
     */
    public ScilabBoolean(String varName, boolean[][] data, boolean swaped) {
        this.varName = varName;
        this.data = data;
        this.swaped = swaped;
    }

    /**
     * Create a scalar boolean from a boolean
     *
     * @param value
     *            the boolean
     */
    public ScilabBoolean(boolean value) {
        this.data = new boolean[1][1];
        this.data[0][0] = value;
    }

    /**
     * Change the value with the provided data
     *
     * @param data
     *            array of boolean
     */
    public void setData(boolean[][] data) {
        this.data = data;
    }

    /**
     * Return the data
     *
     * @return the array of array of boolean
     */
    public boolean[][] getData() {
        return data;
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
     * Return the height (number of element) of the stored data
     *
     * @return the height
     */
    @Override
    public int getHeight() {
        if (data == null) {
            return 0;
        }
        return data.length;
    }

    /**
     * Return the width (number of element) of the stored data
     *
     * @return the width
     */
    @Override
    public int getWidth() {
        if (data == null) {
            return 0;
        }
        return data[0].length;
    }

    /**
     * If it is empty / uninitialized
     *
     * @return true if empty
     */
    @Override
    public boolean isEmpty() {
        return (data == null);
    }

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabBoolean) {
            return Arrays.deepEquals(this.getData(), ((ScilabBoolean) obj).getData());
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
                data = (boolean[][]) in.readObject();
                varName = (String) in.readObject();
                swaped = in.readBoolean();
                break;
            default:
                throw new ClassNotFoundException("A class ScilabBoolean with a version " + version + " does not exists");
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
     * @return the pretty print
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
                if (getData()[i][j]) {
                    result.append("%t");
                } else {
                    result.append("%f");
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
