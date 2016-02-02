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
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * This class provides a representation on the Scilab Boolean Sparse datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * //TODO Sly : ajouter de la doc
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabBooleanSparse implements ScilabType {

    private static final long serialVersionUID = 879625048944109684L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_boolean_sparse;

    private static final int VERSION = 0;

    private int rows;
    private int cols;
    private int nbItem;
    private int[] nbItemRow;
    private int[] colPos;
    private String varName;

    /**
     * Default constructor
     */
    public ScilabBooleanSparse() {
    }

    /**
     * Constructor with a unique value.
     *
     * @param data
     *            the unique value
     */
    public ScilabBooleanSparse(boolean data) {
        if (data) {
            nbItem = 1;
            rows = cols = 1;
            nbItemRow = new int[] { 1 };
            colPos = new int[] { 0 };
        }
    }

    /**
     * Constructor
     *
     * @param rows
     *            the number of rows
     * @param cols
     *            the number of cols
     * @param nbItem
     *            the number of true items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each true
     * @param check
     *            if true the parameters validity is checked
     */
    public ScilabBooleanSparse(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, boolean check) throws ScilabSparseException {
        this(rows, cols, nbItem, nbItemRow, colPos);
        if (check) {
            ScilabSparse.checkValidity(rows, cols, nbItem, nbItemRow, colPos);
        }
    }

    /**
     * Constructor
     *
     * @param rows
     *            the number of rows
     * @param cols
     *            the number of cols
     * @param nbItem
     *            the number of true items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each true
     */
    public ScilabBooleanSparse(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos) {
        this.rows = rows;
        this.cols = cols;
        this.nbItem = nbItem;
        this.nbItemRow = nbItemRow;
        this.colPos = colPos;
    }

    /**
     * Constructor with a matrix of real data.
     *
     * @param data
     *            the data
     */
    public ScilabBooleanSparse(boolean[][] data) {
        if (data.length != 0 && data[0].length != 0) {
            rows = data.length;
            cols = data[0].length;
            nbItemRow = new int[rows];
            List<Integer> listCol = new ArrayList<Integer>();

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (data[i][j]) {
                        ++nbItem;
                        ++nbItemRow[i];
                        listCol.add(j);
                    }
                }
            }

            colPos = new int[listCol.size()];
            int i = 0;
            for (Integer c : listCol) {
                colPos[i++] = c;
            }
        }
    }

    /**
     * Constructor
     *
     * @param varName
     *            the variable name
     * @param rows
     *            the number of rows
     * @param cols
     *            the number of cols
     * @param nbItem
     *            the number of true items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each true
     */
    public ScilabBooleanSparse(String varName, int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos) {
        this(rows, cols, nbItem, nbItemRow, colPos);
        this.varName = varName;
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
        return rows == 0 && cols == 0;
    }

    /**
     * Get the real part of the data.
     *
     * @return the real part.
     */
    public int getNbNonNullItems() {
        return nbItem;
    }

    /**
     * Set the real part of the data.
     *
     * @param nbItem
     *            the real part.
     */
    public void setNbNonNullItems(int nbItem) {
        this.nbItem = nbItem;
    }

    /**
     * Get the real part of the data.
     *
     * @return the real part.
     */
    public int[] getNbItemRow() {
        return nbItemRow;
    }

    /**
     * Set the real part of the data.
     *
     * @param nbItemRow
     *            the real part.
     */
    public void setNbItemRow(int[] nbItemRow) {
        this.nbItemRow = nbItemRow;
    }

    /**
     * Get the column positions of the non null items.
     *
     * @return an integer array.
     */
    public int[] getScilabColPos() {
        int[] cp = new int[colPos.length];
        for (int i = 0; i < colPos.length; i++) {
            cp[i] = colPos[i] + 1;
        }
        return cp;
    }

    /**
     * Get the real part of the data.
     *
     * @return the real part.
     */
    public int[] getColPos() {
        return colPos;
    }

    /**
     * Set the real part of the data.
     *
     * @param colPos
     *            the real part.
     */
    public void setColPos(int[] colPos) {
        this.colPos = colPos;
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
        return false;
    }

    /**
     * Get the real part of the full sparse matrix
     *
     * @return the full real matrix
     */
    public boolean[][] getFullMatrix() {
        int prev = 0;
        int j = 0;
        boolean[][] b = new boolean[rows][cols];
        for (int i = 0; i < nbItemRow.length; i++) {
            for (; j < prev + nbItemRow[i]; j++) {
                b[i][colPos[j]] = true;
            }
            prev += nbItemRow[i];
        }

        return b;
    }

    /**
     * Get complex matrix as a serialized form
     *
     * @return the serialized matrix with complex values
     */
    // TODO Sly : faire qque chose ici...
    public boolean[] getSerializedBooleanSparseMatrix() {
        return new boolean[0];
    }

    /**
     * @return the height of the data matrix
     * @see org.scilab.modules.types.ScilabType#getHeight()
     */
    @Override
    public int getHeight() {
        return rows;
    }

    /**
     * @return the width of the data matrix
     * @see org.scilab.modules.types.ScilabType#getWidth()
     */
    @Override
    public int getWidth() {
        return cols;
    }




    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + Arrays.hashCode(colPos);
        result = prime * result + cols;
        result = prime * result + nbItem;
        result = prime * result + Arrays.hashCode(nbItemRow);
        result = prime * result + rows;
        return result;
    }

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabBooleanSparse) {
            ScilabBooleanSparse sciSparse = (ScilabBooleanSparse) obj;
            return this.getNbNonNullItems() == sciSparse.getNbNonNullItems() && ScilabSparse.compareNbItemRow(this.getNbItemRow(), sciSparse.getNbItemRow())
                   && Arrays.equals(this.getColPos(), sciSparse.getColPos());
        } else {
            return false;
        }
    }

    /**
     * {@inheritDoc}
     */
    public Object getSerializedObject() {
        return new Object[] { new int[] { rows, cols }, nbItemRow, getScilabColPos() };
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        int version = in.readInt();
        switch (version) {
            case 0:
                rows = in.readInt();
                cols = in.readInt();
                nbItem = in.readInt();
                nbItemRow = (int[]) in.readObject();
                colPos = (int[]) in.readObject();
                varName = (String) in.readObject();
                break;
            default:
                throw new ClassNotFoundException("A class ScilabBooleanSparse with a version " + version + " does not exists");
        }
    }

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeInt(VERSION);
        out.writeInt(rows);
        out.writeInt(cols);
        out.writeInt(nbItem);
        out.writeObject(nbItemRow);
        out.writeObject(colPos);
        out.writeObject(varName);
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

        result.append("sparse([");
        int j = 0;
        int prev = 0;
        for (int i = 0; i < nbItemRow.length; i++) {
            for (; j < prev + nbItemRow[i]; j++) {
                result.append(Integer.toString(i + 1));
                result.append(", ");
                result.append(Integer.toString(colPos[j] + 1));
                if (j < nbItem - 1) {
                    result.append(" ; ");
                }
            }
            prev += nbItemRow[i];
        }

        result.append("], [");
        for (int i = 0; i < nbItem; i++) {
            result.append("%t");
            if (i < nbItem - 1) {
                result.append(", ");
            }
        }

        result.append("], [");
        result.append(Integer.toString(rows));
        result.append(", ");
        result.append(Integer.toString(cols));

        result.append("])");

        return result.toString();
    }
}
