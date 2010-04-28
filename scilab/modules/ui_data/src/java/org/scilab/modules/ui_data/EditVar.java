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
	 */
	public static void openVariableEditor(double[][] data) {
		int rows = data.length;
		int cols = data[0].length;
		
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
		
		VariableEditor editvar = ScilabVariableEditor.getVariableEditor(dataDouble);
		editvar.setVisible(true);
	}	

	/**
	 * Close Variable Editor
	 */
	public static void closeVariableEditor() {
		ScilabVariableEditor.getVariableEditor().close();
	}

}
