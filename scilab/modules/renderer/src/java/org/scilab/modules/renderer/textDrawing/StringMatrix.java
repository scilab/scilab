/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Matrix of string 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.textDrawing;

/**
 * Matrix of string
 * @author Jean-Baptiste Silvy
 */
public class StringMatrix {

	private int nbRow;
	private int nbCol;
	private String[][] data;
	
	
	/**
	 * Default constructor
	 */
	public StringMatrix() {
		nbRow = 0;
		nbCol = 0;
		data = null;
	}
	
	/**
	 * Construct a matrix of size nbRow x nbCol.
	 * @param nbRow number of row the matrix
	 * @param nbCol number of column of the matrix
	 */
	public StringMatrix(int nbRow, int nbCol) {
		data = new String[nbRow][nbCol];
		this.nbCol = nbCol;
		this.nbRow = nbRow;
	}
	
	/**
	 * Construct a new matrix from already given data.
	 * @param text array of strings conataining the data.
	 * @param nbRow number of row of the matrix.
	 * @param nbCol number of column of the matrix.
	 */
	public StringMatrix(String[] text, int nbRow, int nbCol) {
		setData(text, nbRow, nbCol);
	}
	
	/**
	 * Set the matrix data from an array of string
	 * @param text array of strings conataining the data.
	 * @param nbRow number of row of the matrix.
	 * @param nbCol number of column of the matrix.
	 */
	public void setData(String[] text, int nbRow, int nbCol) {
		if (data == null || this.nbRow != nbRow || this.nbCol != nbCol) {
			data = new String[nbRow][nbCol];
			this.nbCol = nbCol;
			this.nbRow = nbRow;
		}
		
		// scilab matrices are stored column wise
		for (int i = 0; i < nbRow; i++) {
			for (int j = 0; j < nbCol; j++) {
				data[i][j] = text[j * nbRow + i];
			}
		}
	}
	
	/**
	 * Change an element of the matrix.
	 * @param numRow row index
	 * @param numCol column index
	 * @param value new value to set
	 */
	public void setMatrixElement(int numRow, int numCol, String value) {
		data[numRow][numCol] = value;
	}
	
	/**
	 * @param numRow row index
	 * @param numCol column index
	 * @return element (numRow, numCol).
	 */
	public String getMatrixElement(int numRow, int numCol) {
		return data[numRow][numCol];
	}
	
	/**
	 * @return number of column of the matrix
	 */
	public int getNbCol() {
		return nbCol;
	}
	
	/**
	 * @return number of row of the matrix.
	 */
	public int getNbRow() {
		return nbRow;
	}
	
	/**
	 * @return data of the matrix
	 */
	protected String[][] getData() {
		return data;
	}
	
	/**
	 * @return String representation of the object.
	 */
	@Override
	public String toString() {
		String res = "";
		for (int i = 0; i < nbRow; i++) {
			for (int j = 0; j < nbCol; j++) {
				res += data[i][j];
			}
			res += "\n";
		}
		return res;
	}
	
}
