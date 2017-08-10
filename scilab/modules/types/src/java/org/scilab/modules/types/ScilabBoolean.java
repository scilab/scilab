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
 * This class provides a representation on the Scilab boolean datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * Example:<BR>
 * <code>
 * boolean [][]a={{true,false,true}, {true,true,true}};<BR>
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
    protected boolean[][] data;
    protected String varName;
    protected boolean swaped;
    transient protected boolean byref;

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
     * {@inheritDoc}
     */
    public boolean isReference() {
        return byref;
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
     * @return the data
     */
    public Object getRawData() {
        return data;
    }

    /**
     * Get the element at position (i, j)
     * @param i the first coordinate
     * @param j the second coordinate
     * @return the corresponding boolean
     */
    public boolean getElement(final int i, final int j) {
        return data[i][j];
    }

    /**
     * Set the element at position (i, j)
     * @param i the first coordinate
     * @param j the second coordinate
     * @param x the new value
     */
    public void setElement(final int i, final int j, final boolean x) {
        data[i][j] = x;
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
        return data == null;
    }

    @Override
    public int hashCode() {
        return Arrays.deepHashCode(data);
    }

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabBoolean) {
            ScilabBoolean sciBool = (ScilabBoolean) obj;
            if (isEmpty() && sciBool.isEmpty()) {
                return true;
            }

            if (this.getWidth() != sciBool.getWidth() || this.getHeight() != sciBool.getHeight()) {
                return false;
            }

            return ScilabTypeUtils.equalsBoolean(this.getRawData(), this.isSwaped(), sciBool.getRawData(), sciBool.isSwaped());
        }

        return false;
    }

    /**
     * {@inheritDoc}
     */
    public Object getSerializedObject() {
        return getData();
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
        out.writeObject(getData());
        out.writeObject(varName);
        out.writeBoolean(swaped);
    }

    /**
     * Display the representation in the Scilab language of the type<BR>
     * Note that the representation can be copied/pasted straight into Scilab
     *
     * @return the pretty print
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
                if (getElement(i, j)) {
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
