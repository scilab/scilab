/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - Calixte DENIZET
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
 * Static class to open/close Scilab Variable Editor
 * @author Allan SIMON
 * @author Calixte DENIZET
 */
public final class EditVar {

    /**
     * Integer type
     */
    public static final String INTEGER = "Integer";

    /**
     * Double type
     */
    public static final String DOUBLE = "Double";

    /**
     * Complex type
     */
    public static final String COMPLEX = "Complex";

    /**
     * String type
     */
    public static final String STRING = "String";

    /**
     * Boolean type
     */
    public static final String BOOLEAN = "Boolean";

    /**
     * Default private constructor for utility class
     */
    private EditVar() { }

    /**
     * Open Variable editor
     */
    public static void openVariableEditor() { }

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

        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(DOUBLE, dataDouble, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorInteger8(byte[][] data, String variableName) {
        int rows = data.length;
        int cols = data[0].length;
        Byte[][] dataInteger = new Byte[rows][cols];
        int k = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                dataInteger[k % rows][k / rows] = data[i][j];
                k++;
            }
        }

        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(INTEGER, dataInteger, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorUInteger8(short[][] data, String variableName) {
        int rows = data.length;
        int cols = data[0].length;
        Short[][] dataInteger = new Short[rows][cols];
        int k = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                dataInteger[k % rows][k / rows] = data[i][j];
                k++;
            }
        }

        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(INTEGER, dataInteger, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorInteger16(short[][] data, String variableName) {
        int rows = data.length;
        int cols = data[0].length;
        Short[][] dataInteger = new Short[rows][cols];
        int k = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                dataInteger[k % rows][k / rows] = data[i][j];
                k++;
            }
        }

        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(INTEGER, dataInteger, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorUInteger16(int[][] data, String variableName) {
        int rows = data.length;
        int cols = data[0].length;
        Integer[][] dataInteger = new Integer[rows][cols];
        int k = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                dataInteger[k % rows][k / rows] = data[i][j];
                k++;
            }
        }

        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(INTEGER, dataInteger, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorInteger32(int[][] data, String variableName) {
        int rows = data.length;
        int cols = data[0].length;
        Integer[][] dataInteger = new Integer[rows][cols];
        int k = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                dataInteger[k % rows][k / rows] = data[i][j];
                k++;
            }
        }

        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(INTEGER, dataInteger, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorUInteger32(long[][] data, String variableName) {
        int rows = data.length;
        int cols = data[0].length;
        Long[][] dataInteger = new Long[rows][cols];
        int k = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                dataInteger[k % rows][k / rows] = data[i][j];
                k++;
            }
        }

        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(INTEGER, dataInteger, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorString(String[][] data, String variableName) {
        int rows = data.length;
        int cols = data[0].length;
        String[][] dataString = new String[rows][cols];
        int k = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                dataString[k % rows][k / rows] = data[i][j];
                k++;
            }
        }

        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(STRING, dataString, variableName);
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
        Double[][][] dataDoubleComplex = new Double[rows][cols][2];
        int k = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                dataDoubleComplex[k % rows][k / rows][0] = realData[i][j];
                dataDoubleComplex[k % rows][k / rows][1] = complexData[i][j];
                k++;
            }
        }

        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(COMPLEX, dataDoubleComplex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorBoolean(int[][] data, String variableName) {
        int rows = data.length;
        int cols = data[0].length;
        Boolean[][] dataBool = new Boolean[rows][cols];
        int k = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                dataBool[k % rows][k / rows] = data[i][j] == 1;
                k++;
            }
        }

        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(BOOLEAN, dataBool, variableName);
    }

    /**
     * Close Variable Editor
     */
    public static void closeVariableEditor() {
        ScilabVariableEditor.getVariableEditor().close();
    }
}
