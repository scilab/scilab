/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data;

import org.scilab.modules.ui_data.variableeditor.ScilabVariableEditor;
import org.scilab.modules.ui_data.variableeditor.VariableEditor;

/**
 * 
 * Static class to open/close Scilab Variable Editor
 *
 */
public class EditVar {

	/**
	 * Default private constructor for utility class
	 */
	private EditVar() { }

	/**
	 * Open Variable editor
	 */
	public static void openVariableEditor() {

	}

	/**
	 * Open variable Editor with information given by Scilab
	 * @param data : scilab double matrix
	 * @param variableName : name of the variable being edited.
	 */
	public static void openVariableEditorDouble(double[][] data, String variableName) {
		int rows = data.length;
		int cols = 0;
		if (rows != 0) { // Not an empty matrix
			cols = data[0].length;
		}

		// we need to transpose the matrix as the way to store elements is different in scilab
		// otherwise 
		//  1  2  3    would be rendered   1  4  2 (for example)
		//  4  5  6                        5  3  6 
		Double[][] dataDouble = new Double[rows][cols];
		int k = 0;
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				dataDouble[k % rows][k / rows] = data[i][j];
				k++;
			}
		}

		VariableEditor editvar = ScilabVariableEditor.getVariableEditor(dataDouble, variableName);
	}

	/**
	 * Open variable Editor with information given by Scilab
	 * @param data : scilab double matrix
	 * @param variableName : name of the variable being edited.
	 */
	public static void openVariableEditorString(String[][] data, String variableName) {
		int rows = data.length;
		int cols = data[0].length;

		// we need to transpose the matrix as the way to store elements is different in scilab
		// otherwise 
		//  1  2  3    would be rendered   1  4  2 (for example)
		//  4  5  6                        5  3  6 
		String[][] dataString = new String[rows][cols];
		int k = 0;
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				dataString[k % rows][k / rows] = data[i][j];
				k++;
			}
		}

		VariableEditor editvar = ScilabVariableEditor.getVariableEditor(dataString, variableName);
	}
	
	/**
	 * Open variable Editor with information given by Scilab
	 * @param realData : real part of scilab double matrix
	 * @param complexData : complex part of scilab double matrix
	 * @param variableName : name of the variable being edited.
	 */
	public static void openVariableEditorComplex(double[][] realData, double[][] complexData, String variableName) {
		int rows = realData.length;
		int cols = realData[0].length;

		// we need to transpose the matrix as the way to store elements is different in scilab
		// otherwise 
		//  1  2  3    would be rendered   1  4  2 (for example)
		//  4  5  6                        5  3  6 
		Double[][][] dataDoubleComplex = new Double[rows][cols][2];
		int k = 0;
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				dataDoubleComplex[k % rows][k / rows][0] = realData[i][j];
				dataDoubleComplex[k % rows][k / rows][1] = complexData[i][j];	
				k++;
			}
		}

		VariableEditor editvar = ScilabVariableEditor.getVariableEditor(dataDoubleComplex, variableName);
	}
	
	
	/**
	 * Open variable Editor with information given by Scilab
	 * @param data : scilab double matrix
	 * @param variableName : name of the variable being edited.
	 */
	public static void openVariableEditorBoolean(int[][] data, String variableName) {
		int rows = data.length;
		int cols = data[0].length;

		// we need to transpose the matrix as the way to store elements is different in scilab
		// otherwise 
		//  1  2  3    would be rendered   1  4  2 (for example)
		//  4  5  6                        5  3  6 
		Boolean[][] dataBool = new Boolean[rows][cols];
		int k = 0;
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				dataBool[k % rows][k / rows] = (data[i][j] == 1);
				k++;
			}
		}

		VariableEditor editvar = ScilabVariableEditor.getVariableEditor(dataBool, variableName);
	}
	
	/**
	 * 	
	 * Update the cell at coordinate (row,col) to new double value or keep the old one if errCode is set
	 * @param variableName : The name of the variable that has been updated
	 * @param row : the row whose value is to be changed
	 * @param col : the column whose value is to be changed
	 * @param newValue : the new double value to set.
	 * @param errCode : the errCode given by Scilab, 0 if no error.
	 */
	

	
	public static void updateVariableEditorDouble(String variableName, int row, int col, double newValue, int errCode) {
		updateVariableEditor(variableName, row, col, (Double) newValue, errCode);
	}
	
	public static void updateVariableEditorComplex(String variableName, int row, int col, double realValue, double imgValue, int errCode) {
		Double[] newValue = new Double[]{realValue, imgValue};
		updateVariableEditor(variableName, row, col, newValue, errCode);
	}
	
	public static void updateVariableEditorBoolean(String variableName, int row, int col, int newValue, int errCode) {
		Boolean newBool = (newValue == 1);
		updateVariableEditor(variableName, row, col, newBool, errCode);
	}
	
	public static void updateVariableEditorString(String variableName, int row, int col, String newValue, int errCode) {
		updateVariableEditor(variableName, row, col, newValue, errCode);
	}

	/**
	 * 	
	 * Update the cell at coordinate (row,col) to new generic value or keep the old one if errCode is set
	 * @param variableName : The name of the variable that has been updated
	 * @param row : the row whose value is to be changed
	 * @param col : the column whose value is to be changed
	 * @param newValue : the new value to set.
	 * @param errCode : the errCode given by Scilab, 0 if no error.
	 */
	public static void updateVariableEditor(String variableName, int row, int col, Object newValue, int errCode) {
		ScilabVariableEditor.getVariableEditor().setValueAt(newValue, row - 1, col - 1);
	}

	/**
	 * Close Variable Editor
	 */
	public static void closeVariableEditor() {
		ScilabVariableEditor.getVariableEditor().close();
	}

}
