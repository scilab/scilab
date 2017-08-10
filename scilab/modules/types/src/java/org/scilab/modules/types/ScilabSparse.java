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
 * This class provides a representation on the Scilab Sparse datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * //TODO Sly : ajouter de la doc
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabSparse implements ScilabType {

    private static final long serialVersionUID = 879625048944109684L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_sparse;

    private static final int VERSION = 0;

    protected int rows;
    protected int cols;
    protected int nbItem;
    protected int[] nbItemRow;
    protected int[] colPos;
    protected double[] realPart;
    protected double[] imaginaryPart;
    protected String varName;
    protected boolean byref;

    /**
     * Default constructor
     */
    public ScilabSparse() {
    }

    /**
     * Constructor with a unique value.
     *
     * @param data
     *            the unique value
     */
    public ScilabSparse(double data) {
        if (data != 0) {
            nbItem = 1;
            rows = cols = 1;
            nbItemRow = new int[] { 1 };
            colPos = new int[] { 0 };
            realPart = new double[] { data };
        }
    }

    /**
     * Constructor with a unique complex value.
     *
     * @param realData
     *            the real part
     * @param imagData
     *            the complex part
     */
    public ScilabSparse(double realData, double imagData) {
        if (realData != 0 || imagData != 0) {
            nbItem = 1;
            rows = cols = 1;
            nbItemRow = new int[] { 1 };
            colPos = new int[] { 0 };
            realPart = new double[] { realData };
            imaginaryPart = new double[] { imagData };
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
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     * @param data
     *            the non null data
     * @param check
     *            if true the parameters validity is checked
     */
    public ScilabSparse(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] data, boolean check) throws ScilabSparseException {
        this(rows, cols, nbItem, nbItemRow, colPos, data);
        if (check) {
            checkValidity(rows, cols, nbItem, nbItemRow, colPos);

            if (realPart.length != nbItem) {
                throw new ScilabSparseException("Invalid length for the array realPart: its length must be equal to the number of non-null items.");
            }
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
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     * @param real
     *            the non null real data
     */
    public ScilabSparse(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] real) {
        this.rows = rows;
        this.cols = cols;
        this.nbItem = nbItem;
        this.nbItemRow = nbItemRow;
        this.colPos = colPos;
        this.realPart = real;
    }

    /**
     * Constructor
     *
     * @param rows
     *            the number of rows
     * @param cols
     *            the number of cols
     * @param nbItem
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     * @param real
     *            the non null real data
     * @param imag
     *            the non null imaginary data
     * @param check
     *            if true the parameters validity is checked
     */
    public ScilabSparse(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] real, double[] imag, boolean check)
    throws ScilabSparseException {
        this(rows, cols, nbItem, nbItemRow, colPos, real, imag);
        if (check) {
            checkValidity(rows, cols, nbItem, nbItemRow, colPos);

            if (realPart.length != nbItem) {
                throw new ScilabSparseException("Invalid length for the array realPart: its length must be equal to the number of non-null items.");
            }

            if (imaginaryPart.length != nbItem) {
                throw new ScilabSparseException("Invalid length for the array imaginaryPart: its length must be equal to the number of non-null items.");
            }
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
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     * @param real
     *            the non null real data
     * @param imag
     *            the non null imaginary data
     */
    public ScilabSparse(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] real, double[] imag) {
        this(rows, cols, nbItem, nbItemRow, colPos, real);
        this.imaginaryPart = imag;
    }

    /**
     * Constructor
     *
     * @param rows
     *            the number of rows
     * @param cols
     *            the number of cols
     * @param nbItem
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     */
    static final void checkValidity(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos) throws ScilabSparseException {
        if (nbItem > rows * cols || nbItem < 0) {
            throw new ScilabSparseException("Invalid number of items: between 0 and " + rows * cols + " expected.");
        }

        if (nbItemRow.length > rows) {
            throw new ScilabSparseException("Invalid length for the array nbItemRow: a length between 0 and " + rows + " expected.");
        }

        int s = 0;
        for (int i = 0; i < nbItemRow.length; i++) {
            if (nbItemRow[i] > cols) {
                throw new ScilabSparseException("Invalid number of non-null items in nbItemRow at position " + i + ".");
            }
            s += nbItemRow[i];
        }

        if (s != nbItem) {
            throw new ScilabSparseException("Invalid array nbItemRow: the total sum is not equal to the number of non-null items.");
        }

        if (colPos.length != nbItem) {
            throw new ScilabSparseException("Invalid length for the array colPos: its length must be equal to the number of non-null items.");
        }

        for (int i = 0; i < nbItem; i++) {
            if (colPos[i] >= cols || colPos[i] < 0) {
                throw new ScilabSparseException("Invalid column position at position " + i + ".");
            }
        }
    }

    /**
     * Constructor with a matrix of real data.
     *
     * @param data
     *            the data
     */
    public ScilabSparse(double[][] data) {
        if (data.length != 0 && data[0].length != 0) {
            rows = data.length;
            cols = data[0].length;
            nbItemRow = new int[rows];
            List<Integer> listCol = new ArrayList<Integer>();
            List<Double> listVal = new ArrayList<Double>();

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (data[i][j] != 0) {
                        ++nbItem;
                        ++nbItemRow[i];
                        listCol.add(j);
                        listVal.add(data[i][j]);
                    }
                }
            }

            colPos = new int[listCol.size()];
            realPart = new double[colPos.length];
            int i = 0;
            for (Integer c : listCol) {
                colPos[i++] = c;
            }
            i = 0;
            for (Double x : listVal) {
                realPart[i++] = x;
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
    public ScilabSparse(double[][] realData, double[][] imagData) {
        if (realData.length != 0 && realData[0].length != 0) {
            rows = realData.length;
            cols = realData[0].length;
            nbItemRow = new int[rows];
            List<Integer> listCol = new ArrayList<Integer>();
            List<Double> listReal = new ArrayList<Double>();
            List<Double> listImag = new ArrayList<Double>();

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (realData[i][j] != 0 || imagData[i][j] != 0) {
                        ++nbItem;
                        ++nbItemRow[i];
                        listCol.add(j);
                        listReal.add(realData[i][j]);
                        listImag.add(imagData[i][j]);
                    }
                }
            }

            colPos = new int[listCol.size()];
            this.realPart = new double[colPos.length];
            this.imaginaryPart = new double[colPos.length];
            int i = 0;
            for (Integer c : listCol) {
                colPos[i++] = c;
            }
            i = 0;
            for (Double x : listReal) {
                this.realPart[i++] = x;
            }
            i = 0;
            for (Double x : listImag) {
                this.imaginaryPart[i++] = x;
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
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     * @param real
     *            the non null real data
     * @param imag
     *            the non null imaginary data
     */
    public ScilabSparse(String varName, int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] real, double[] imag) {
        this(rows, cols, nbItem, nbItemRow, colPos, real, imag);
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
     * Check if the current data doesn't have an imaginary part.
     *
     * @return true, if the data are real only.
     */
    public boolean isReal() {
        return imaginaryPart == null;
    }

    /**
     * Get the element in column position
     * @param i the position
     * @return the column position
     */
    public int getColPosElement(final int i) {
        return colPos[i];
    }

    /**
     * Set the element in column position
     * @param i the position
     * @param x the column position
     */
    public void setColPosElement(final int i, final int x) {
        colPos[i] = x;
    }

    /**
     * Get the number of non-null item in row i
     * @param i the row
     * @return the number of non-null items
     */
    public int getNbItemElement(final int i) {
        return nbItemRow[i];
    }

    /**
     * Set the number of non-null item in row i
     * @param i the row
     * @param x the number of non-null items
     */
    public void setNbItemElement(final int i, final int x) {
        nbItemRow[i] = x;
    }

    /**
     * Get the real element at position i
     * @param i the position
     * @return a real
     */
    public double getRealElement(final int i) {
        return realPart[i];
    }

    /**
     * Get the real element at position (i, j) in the sparse matrix
     * @param i the row index
     * @param j the column index
     * @return a real
     */
    public double getRealElement(final int i, final int j) {
        if (getNbItemElement(i) == 0) {
            return 0;
        }

        int prev = 0;
        for (int k = 0; k < i; k++) {
            prev += getNbItemElement(k);
        }

        for (int k = prev; k < prev + getNbItemElement(i); k++) {
            if (getColPosElement(k) == j) {
                return getRealElement(k);
            }
        }

        return 0;
    }

    /**
     * Get the imaginary element at position i
     * @param i the position
     * @return a real
     */
    public double getImaginaryElement(final int i) {
        return imaginaryPart[i];
    }

    /**
     * Get the imanginary element at position (i, j) in the sparse matrix
     * @param i the row index
     * @param j the column index
     * @return a real
     */
    public double getImaginaryElement(final int i, final int j) {
        if (getNbItemElement(i) == 0) {
            return 0;
        }

        int prev = 0;
        for (int k = 0; k < i; k++) {
            prev += getNbItemElement(k);
        }

        for (int k = prev; k < prev + getNbItemElement(i); k++) {
            if (getColPosElement(k) == j) {
                return getImaginaryElement(k);
            }
        }

        return 0;
    }

    /**
     * Get the real and imaginary elements at position i
     * @param i the position
     * @return a 2-array containing real and imaginary part
     */
    public double[] getElement(final int i) {
        if (isReal()) {
            return new double[] {realPart[i], 0};
        } else {
            return new double[] {realPart[i], imaginaryPart[i]};
        }
    }

    /**
     * Get the real and imanginary element at position (i, j) in the sparse matrix
     * @param i the row index
     * @param j the column index
     * @return a 2-array containing real and imaginary parts
     */
    public double[] getElement(final int i, final int j) {
        if (isReal()) {
            return new double[] {getRealElement(i, j), 0};
        } else {
            if (getNbItemElement(i) == 0) {
                return new double[] {0, 0};
            }

            int prev = 0;
            for (int k = 0; k < i; k++) {
                prev += getNbItemElement(k);
            }

            for (int k = prev; k < prev + getNbItemElement(i); k++) {
                if (getColPosElement(k) == j) {
                    return new double[] {getRealElement(k), getImaginaryElement(k)};
                }
            }

            return new double[] {0, 0};
        }
    }

    /**
     * Set the real element at position i
     * @param i the position
     * @param x the value to set
     */
    public void setRealElement(final int i, final double x) {
        realPart[i] = x;
    }

    /**
     * Set the real element at position (i, j) in sparse matrix
     * Take care only an already non-null element can be set
     * @param i the row index
     * @param j the column index
     * @param x the value to set
     */
    public void setRealElement(final int i, final int j, final double x) {
        if (getNbItemElement(i) == 0) {
            return;
        }

        int prev = 0;
        for (int k = 0; k < i; k++) {
            prev += getNbItemElement(k);
        }

        for (int k = prev; k < prev + getNbItemElement(i); k++) {
            if (getColPosElement(k) == j) {
                setRealElement(k, x);
                break;
            }
        }
    }

    /**
     * Set the imaginary element at position i
     * @param i the position
     * @param x the value to set
     */
    public void setImaginaryElement(final int i, final double x) {
        imaginaryPart[i] = x;
    }

    /**
     * Set the imaginary element at position (i, j) in sparse matrix
     * Take care only an already non-null element can be set
     * @param i the row index
     * @param j the column index
     * @param x the value to set
     */
    public void setImaginaryElement(final int i, final int j, final double x) {
        if (getNbItemElement(i) == 0) {
            return;
        }

        int prev = 0;
        for (int k = 0; k < i; k++) {
            prev += getNbItemElement(k);
        }

        for (int k = prev; k < prev + getNbItemElement(i); k++) {
            if (getColPosElement(k) == j) {
                setImaginaryElement(k, x);
                break;
            }
        }
    }

    /**
     * Set the real and imaginary elements at position i
     * @param i the position
     * @param x the real part to set
     * @param y the imaginary part to set
     */
    public void setElement(final int i, final double x, final double y) {
        setRealElement(i, x);
        setImaginaryElement(i, y);
    }

    /**
     * Get the real part of the data.
     *
     * @return the real part.
     */
    public double[] getRealPart() {
        return realPart;
    }

    /**
     * Set the real part of the data.
     *
     * @param realPart
     *            the real part.
     */
    public void setRealPart(double[] realPart) {
        this.realPart = realPart;
    }

    /**
     * Get the imaginary part of the data.
     *
     * @return the imaginary part.
     */
    public double[] getImaginaryPart() {
        return imaginaryPart;
    }

    /**
     * Set the imaginary part of the data.
     *
     * @param imaginaryPart
     *            the imaginary part.
     */
    public void setImaginaryPart(double[] imaginaryPart) {
        this.imaginaryPart = imaginaryPart;
    }

    /**
     * Get the number of non null items in the matrix.
     *
     * @return the number of non null items.
     */
    public int getNbNonNullItems() {
        return nbItem;
    }

    /**
     * Set the number of non null items in the matrix.
     *
     * @param nbItem
     *            the number of non null items.
     */
    public void setNbNonNullItems(int nbItem) {
        this.nbItem = nbItem;
    }

    /**
     * Get the number of non null items by row.
     *
     * @return an integer array.
     */
    public int[] getNbItemRow() {
        return nbItemRow;
    }

    /**
     * Set the number of non null items by row.
     *
     * @param nbItemRow
     *            an integer array.
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
     * Get the column positions of the non null items.
     *
     * @return an integer array.
     */
    public int[] getColPos() {
        return colPos;
    }

    /**
     * Set the column positions of the non null items.
     *
     * @param colPos
     *            an integer array.
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
    public double[][] getFullRealPart() {
        int prev = 0;
        int j = 0;
        double[][] d = new double[rows][cols];
        for (int i = 0; i < nbItemRow.length; i++) {
            for (; j < prev + nbItemRow[i]; j++) {
                d[i][colPos[j]] = realPart[j];
            }
            prev += nbItemRow[i];
        }

        return d;
    }

    /**
     * Get the imaginary part of the full sparse matrix
     *
     * @return the full imaginary matrix
     */
    public double[][] getFullImaginaryPart() {
        int prev = 0;
        int j = 0;
        double[][] d = new double[rows][cols];
        for (int i = 0; i < nbItemRow.length; i++) {
            for (; j < prev + nbItemRow[i]; j++) {
                d[i][colPos[j]] = imaginaryPart[j];
            }
            prev += nbItemRow[i];
        }

        return d;
    }

    /**
     * Get the full sparse matrix representation as an array 2 x rows x cols If
     * d = getFullMatrix(), then d[0] contains realpart and d[1] the imaginary
     * one.
     *
     * @return the full matrix components
     */
    public double[][][] getFullMatrix() {
        int prev = 0;
        int j = 0;
        double[][][] d = new double[2][rows][cols];
        for (int i = 0; i < nbItemRow.length; i++) {
            for (; j < prev + nbItemRow[i]; j++) {
                d[0][i][colPos[j]] = realPart[j];
                d[1][i][colPos[j]] = imaginaryPart[j];
            }
            prev += nbItemRow[i];
        }

        return d;
    }

    /**
     * Get complex matrix as a serialized form
     *
     * @return the serialized matrix with complex values
     */
    // TODO Sly : faire qque chose ici...
    public double[] getSerializedSparseMatrix() {
        return new double[0];
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
        result = prime * result + Arrays.hashCode(imaginaryPart);
        result = prime * result + nbItem;
        result = prime * result + Arrays.hashCode(nbItemRow);
        result = prime * result + Arrays.hashCode(realPart);
        result = prime * result + rows;
        return result;
    }

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabSparse) {
            ScilabSparse sciSparse = (ScilabSparse) obj;
            if (isEmpty() && sciSparse.isEmpty()) {
                return true;
            }

            if (this.getWidth() != sciSparse.getWidth() || this.getHeight() != sciSparse.getHeight()) {
                return false;
            }

            if (this.getNbNonNullItems() == sciSparse.getNbNonNullItems() && compareNbItemRow(this.getNbItemRow(), sciSparse.getNbItemRow())
                    && Arrays.equals(this.getColPos(), sciSparse.getColPos())) {
                if (this.isReal() && sciSparse.isReal()) {
                    return Arrays.equals(this.getRealPart(), sciSparse.getRealPart());
                } else {
                    /* Complex */
                    return Arrays.equals(this.getRealPart(), sciSparse.getRealPart()) && Arrays.equals(this.getImaginaryPart(), sciSparse.getImaginaryPart());
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    /**
     * Compare two arrays containing the number of items by row. For example {1,
     * 2, 3, 4} is equal to {1, 2, 3, 4, 0, 0, 0, 0}/
     *
     * @param a
     *            an array
     * @param b
     *            an other array
     * @return true if the arrays are equal
     */
    static final boolean compareNbItemRow(final int[] a, final int[] b) {
        if (Arrays.equals(a, b)) {
            return true;
        }

        if (a.length == b.length) {
            return false;
        }

        int[] c, d;
        if (a.length < b.length) {
            c = a;
            d = b;
        } else {
            c = b;
            d = a;
        }

        int i = 0;
        for (; i < c.length; i++) {
            if (c[i] != d[i]) {
                return false;
            }
        }

        for (; i < d.length; i++) {
            if (d[i] != 0) {
                return false;
            }
        }

        return true;
    }

    /**
     * {@inheritDoc}
     */
    public Object getSerializedObject() {
        if (isReal()) {
            return new Object[] { new int[] { getHeight(), getWidth() }, getNbItemRow(), getScilabColPos(), getRealPart() };
        } else {
            return new Object[] { new int[] { getHeight(), getWidth() }, getNbItemRow(), getScilabColPos(), getRealPart(), getImaginaryPart() };
        }
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
                realPart = (double[]) in.readObject();
                imaginaryPart = (double[]) in.readObject();
                varName = (String) in.readObject();
                break;
            default:
                throw new ClassNotFoundException("A class ScilabSparse with a version " + version + " does not exists");
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
        out.writeObject(realPart);
        out.writeObject(imaginaryPart);
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
            return "[]";
        }

        result.append("sparse([");
        int j = 0;
        int prev = 0;
        int[] nbItemRow = getNbItemRow();
        int[] colPos = getColPos();
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
        boolean real = isReal();
        for (int i = 0; i < nbItem; i++) {
            result.append(Double.toString(getRealElement(i)));
            if (!real) {
                final double y = getImaginaryElement(i);
                if (y != 0) {
                    if (y > 0) {
                        result.append("+");
                    }
                    result.append(Double.toString(y));
                    result.append("*%i");
                }
            }
            if (i < nbItem - 1) {
                result.append(" ; ");
            } else {
                result.append("]");
            }
        }

        result.append(", [");
        result.append(Integer.toString(rows));
        result.append(", ");
        result.append(Integer.toString(cols));

        result.append("])");

        return result.toString();
    }
}
