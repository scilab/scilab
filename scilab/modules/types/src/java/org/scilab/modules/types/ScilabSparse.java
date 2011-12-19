/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * This class provides a representation on the Scilab Sparse datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could
 * impact load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * //TODO Sly : ajouter de la doc
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabSparse implements ScilabType {

    private static final long serialVersionUID = 879625048944109684L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_sparse;

    private int rows;
    private int cols;
    private int nbItem;
    private int[] nbItemRow;
    private int[] colPos;
    private double[] realPart;
    private double[] imaginaryPart;
    private String varName;

    /**
     * Default constructor
     */
    public ScilabSparse() { }

    /**
     * Constructor with a unique value.
     * @param data the unique value
     */
    public ScilabSparse(double data) {
        if (data != 0) {
            nbItem = 1;
            rows = cols = 1;
            nbItemRow = new int[]{1};
            colPos = new int[]{0};
            realPart = new double[]{ data };
        }
    }

    /**
     * Constructor with a unique complex value.
     *
     * @param realData the real part
     * @param imagData the complex part
     */
    public ScilabSparse(double realData, double imagData) {
        if (realData != 0 || imagData != 0) {
            nbItem = 1;
            rows = cols = 1;
            nbItemRow = new int[]{1};
            colPos = new int[]{0};
            realPart = new double[]{ realData };
            imaginaryPart = new double[]{ imagData };
        }
    }

    /**
     * Constructor
     *
     * @param rows the number of rows
     * @param cols the number of cols
     * @param nbItem the number of non null items
     * @param nbItemRow contains the number of true in each rows
     * @param colPos the column position of each non null item
     * @param data the non null data
     */
    public ScilabSparse(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] data) {
        this.rows = rows;
        this.cols = cols;
        this.nbItem = nbItem;
        this.nbItemRow = nbItemRow;
        this.colPos = colPos;
        this.realPart = data;
    }

    /**
     * Constructor
     *
     * @param rows the number of rows
     * @param cols the number of cols
     * @param nbItem the number of non null items
     * @param nbItemRow contains the number of true in each rows
     * @param colPos the column position of each non null item
     * @param real the non null real data
     * @param imag the non null imaginary data
     */
    public ScilabSparse(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] real, double[] imag) {
        this(rows, cols, nbItem, nbItemRow, colPos, real);
        this.imaginaryPart = imag;
    }

    /**
     * Constructor with a matrix of real data.
     *
     * @param data the data
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
            for (Double x : listVal) {
                realPart[i++] = x;
            }
        }
    }

    /**
     * Constructor with a matrix of complex numbers
     *
     * @param realData the real part of the data
     * @param imagData the imaginary part of the data
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
     * @param varName the variable name
     * @param rows the number of rows
     * @param cols the number of cols
     * @param nbItem the number of non null items
     * @param nbItemRow contains the number of true in each rows
     * @param colPos the column position of each non null item
     * @param real the non null real data
     * @param imag the non null imaginary data
     */
    public ScilabSparse(String varName, int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] real, double[] imag) {
        this(rows, cols, nbItem, nbItemRow, colPos, real, imag);
        this.varName = varName;
    }

    /**
     * Return the type of Scilab
     * @return the type of Scilab
     * @since 5.4.0
     */
    @Override
    public ScilabTypeEnum getType() {
        return type;
    }

    /**
     * Check the emptiness of the associated data.
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
        return (imaginaryPart == null);
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
     * @param realPart the real part.
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
     * @param imaginaryPart the imaginary part.
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
     * @param the number of non null items.
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
     * @param an integer array.
     */
    public void setNbItemRow(int[] nbItemRow) {
        this.nbItemRow = nbItemRow;
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
     * @param an integer array.
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
        for (int i = 0; i < rows; i++) {
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
        for (int i = 0; i < rows; i++) {
            for (; j < prev + nbItemRow[i]; j++) {
                d[i][colPos[j]] = imaginaryPart[j];
            }
            prev += nbItemRow[i];
        }

        return d;
    }

    /**
     * Get the full sparse matrix representation as an array 2 x rows x cols
     * If d = getFullMatrix(), then d[0] contains realpart and d[1] the imaginary one.
     *
     * @return the full matrix components
     */
    public double[][][] getFullMatrix() {
        int prev = 0;
        int j = 0;
        double[][][] d = new double[2][rows][cols];
        for (int i = 0; i < rows; i++) {
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

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabSparse) {
            ScilabSparse sciSparse = (ScilabSparse) obj;
            if (this.getNbNonNullItems() == sciSparse.getNbNonNullItems() &&
                Arrays.equals(this.getNbItemRow(), sciSparse.getNbItemRow()) &&
                Arrays.equals(this.getColPos(), sciSparse.getColPos())) {
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
            result.append("[]");
            return result.toString();
        }

        result.append("sparse([");
        int j = 0;
        int prev = 0;
        for (int i = 0; i < rows; i++) {
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
            if (isReal()) {
                result.append(Double.toString(realPart[i]));
            } else {
                result.append(Double.toString(realPart[i]));
                if (imaginaryPart[i] != 0) {
                    result.append("+");
                    result.append(Double.toString(imaginaryPart[i]));
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
