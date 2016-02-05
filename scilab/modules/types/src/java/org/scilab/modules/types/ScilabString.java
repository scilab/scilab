/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
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
 * This class provides a representation on the Scilab String datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * Example:<BR>
 * <code>
 * String [][]a={{"This","is","my","string"},{"and","I want to", "compare"," them"}};<BR>
 * ScilabString aMatrix = new ScilabString(a);
 * </code>
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabString implements ScilabType {

    private static final long serialVersionUID = 359802519980180085L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_strings;

    private static final int VERSION = 0;

    private String[][] data;
    private String varName;
    private boolean swaped;

    /**
     * Default constructor
     */
    public ScilabString() {
        data = null;
    }

    /**
     * Constructor with data.
     *
     * @param data
     *            the associated data.
     */
    public ScilabString(String[][] data) {
        this.data = data;
    }

    /**
     * Constructor with data.
     *
     * @param data
     *            the associated data.
     */
    public ScilabString(String varName, String[][] data, boolean swaped) {
        this.varName = varName;
        this.data = data;
        this.swaped = swaped;
    }

    /**
     * Constructor with vector data.
     *
     * @param data
     *            the column vector data
     */
    public ScilabString(String[] data) {
        if (data == null || data.length == 0) {
            this.data = new String[1][1];
            this.data[0][0] = "";
        } else {
            this.data = new String[1][data.length];
            for (int i = 0; i < data.length; i++) {
                this.data[0][i] = data[i];
            }
        }
    }

    /**
     * Constructor with a unique value
     *
     * @param string
     *            the value
     */
    public ScilabString(String string) {
        if (string == null) {
            throw new IllegalArgumentException("string == null");
        }
        this.data = new String[1][1];
        this.data[0][0] = string;
    }

    /**
     * Set the values.
     *
     * @param data
     *            the values
     */
    public void setData(String[][] data) {
        this.data = data;
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
     * @return the associated values
     */
    public String[][] getData() {
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
     * {@inheritDoc}
     */
    public boolean isReference() {
        return false;
    }

    /**
     * @return the height of the data matrix
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
     * @return the width of the data matrix
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
     * Check the emptiness of the associated data.
     *
     * @return true, if the associated data array is empty.
     */
    @Override
    public boolean isEmpty() {
        return (data == null);
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + Arrays.deepHashCode(data);
        return result;
    }

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabString) {
            return Arrays.deepEquals(this.getData(), ((ScilabString) obj).getData());
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
                data = (String[][]) in.readObject();
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
        for (int i = 0; i < getHeight(); ++i) {
            for (int j = 0; j < getWidth(); ++j) {
                result.append('"');
                result.append(data[i][j].replaceAll("[\"\']", "\"\""));
                result.append('"');

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
