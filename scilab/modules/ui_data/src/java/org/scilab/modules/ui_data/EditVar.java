/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - Calixte DENIZET
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
     * Boolean sparse type
     */
    public static final String BOOLEANSPARSE = "Boolean Sparse";

    /**
     * Sparse type
     */
    public static final String SPARSE = "Sparse";

    /**
     * Complex sparse type
     */
    public static final String COMPLEXSPARSE = "Complex Sparse";

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
        int c = data.length == 0 ? 0 : data[0].length;
        Double[][] dataDouble = new Double[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataDouble[i][j] = data[i][j];
            }
        }
        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(DOUBLE, dataDouble, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorBooleanSparse(int[][] data, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Boolean[][] dataBool = new Boolean[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataBool[i][j] = data[i][j] == 1;
            }
        }
        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(BOOLEANSPARSE, dataBool, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorSparse(double[][] data, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Double[][] dataDouble = new Double[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataDouble[i][j] = data[i][j];
            }
        }
        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(SPARSE, dataDouble, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorComplexSparse(double[][] realData, double[][] complexData, String variableName) {
        int c = realData.length == 0 ? 0 : realData[0].length;
        Double[][][] dataDoubleComplex = new Double[realData.length][c][2];
        for (int i = 0; i < realData.length; i++) {
            for (int j = 0; j < c; j++) {
                dataDoubleComplex[i][j][0] = realData[i][j];
                dataDoubleComplex[i][j][1] = complexData[i][j];
            }
        }
        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(COMPLEXSPARSE, dataDoubleComplex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorInteger8(byte[][] data, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Byte[][] dataInteger = new Byte[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataInteger[i][j] = data[i][j];
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
        int c = data.length == 0 ? 0 : data[0].length;
        Short[][] dataInteger = new Short[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataInteger[i][j] = data[i][j];
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
        int c = data.length == 0 ? 0 : data[0].length;
        Short[][] dataInteger = new Short[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataInteger[i][j] = data[i][j];
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
        int c = data.length == 0 ? 0 : data[0].length;
        Integer[][] dataInteger = new Integer[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataInteger[i][j] = data[i][j];
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
        int c = data.length == 0 ? 0 : data[0].length;
        Integer[][] dataInteger = new Integer[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataInteger[i][j] = data[i][j];
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
        int c = data.length == 0 ? 0 : data[0].length;
        Long[][] dataInteger = new Long[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataInteger[i][j] = data[i][j];
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
        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(STRING, data, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param realData : real part of scilab double matrix
     * @param complexData : complex part of scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void openVariableEditorComplex(double[][] realData, double[][] complexData, String variableName) {
        int c = realData.length == 0 ? 0 : realData[0].length;
        Double[][][] dataDoubleComplex = new Double[realData.length][c][2];
        for (int i = 0; i < realData.length; i++) {
            for (int j = 0; j < c; j++) {
                dataDoubleComplex[i][j][0] = realData[i][j];
                dataDoubleComplex[i][j][1] = complexData[i][j];
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
        int c = data.length == 0 ? 0 : data[0].length;
        Boolean[][] dataBool = new Boolean[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataBool[i][j] = data[i][j] == 1;
            }
        }
        VariableEditor editvar = ScilabVariableEditor.getVariableEditor(BOOLEAN, dataBool, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorDouble(double[][] data, double[] rowsIndex, double[] colsIndex, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Double[][] dataDouble = new Double[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataDouble[i][j] = data[i][j];
            }
        }
        ScilabVariableEditor.refreshVariableEditor(DOUBLE, dataDouble, rowsIndex, colsIndex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorBooleanSparse(int[][] data, double[] rowsIndex, double[] colsIndex, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Boolean[][] dataBool = new Boolean[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataBool[i][j] = data[i][j] == 1;
            }
        }
        ScilabVariableEditor.refreshVariableEditor(BOOLEANSPARSE, dataBool, rowsIndex, colsIndex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorSparse(double[][] data, double[] rowsIndex, double[] colsIndex, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Double[][] dataDouble = new Double[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataDouble[i][j] = data[i][j];
            }
        }
        ScilabVariableEditor.refreshVariableEditor(SPARSE, dataDouble, rowsIndex, colsIndex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param realData : real part of scilab double matrix
     * @param complexData : complex part of scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorComplexSparse(double[][] realData, double[][] complexData, double[] rowsIndex, double[] colsIndex, String variableName) {
        int c = realData.length == 0 ? 0 : realData[0].length;
        Double[][][] dataDoubleComplex = new Double[realData.length][c][2];
        for (int i = 0; i < realData.length; i++) {
            for (int j = 0; j < c; j++) {
                dataDoubleComplex[i][j][0] = realData[i][j];
                dataDoubleComplex[i][j][1] = complexData[i][j];
            }
        }
        ScilabVariableEditor.refreshVariableEditor(COMPLEXSPARSE, dataDoubleComplex, rowsIndex, colsIndex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorInteger8(byte[][] data, double[] rowsIndex, double[] colsIndex, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Byte[][] dataInteger = new Byte[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataInteger[i][j] = data[i][j];
            }
        }
        ScilabVariableEditor.refreshVariableEditor(INTEGER, dataInteger, rowsIndex, colsIndex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorUInteger8(short[][] data, double[] rowsIndex, double[] colsIndex, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Short[][] dataInteger = new Short[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataInteger[i][j] = data[i][j];
            }
        }
        ScilabVariableEditor.refreshVariableEditor(INTEGER, dataInteger, rowsIndex, colsIndex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorInteger16(short[][] data, double[] rowsIndex, double[] colsIndex, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Short[][] dataInteger = new Short[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataInteger[i][j] = data[i][j];
            }
        }
        ScilabVariableEditor.refreshVariableEditor(INTEGER, dataInteger, rowsIndex, colsIndex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorUInteger16(int[][] data, double[] rowsIndex, double[] colsIndex, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Integer[][] dataInteger = new Integer[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataInteger[i][j] = data[i][j];
            }
        }
        ScilabVariableEditor.refreshVariableEditor(INTEGER, dataInteger, rowsIndex, colsIndex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorInteger32(int[][] data, double[] rowsIndex, double[] colsIndex, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Integer[][] dataInteger = new Integer[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataInteger[i][j] = data[i][j];
            }
        }
        ScilabVariableEditor.refreshVariableEditor(INTEGER, dataInteger, rowsIndex, colsIndex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorUInteger32(long[][] data, double[] rowsIndex, double[] colsIndex, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Long[][] dataInteger = new Long[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataInteger[i][j] = data[i][j];
            }
        }
        ScilabVariableEditor.refreshVariableEditor(INTEGER, dataInteger, rowsIndex, colsIndex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorString(String[][] data, double[] rowsIndex, double[] colsIndex, String variableName) {
        ScilabVariableEditor.refreshVariableEditor(STRING, data, rowsIndex, colsIndex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param realData : real part of scilab double matrix
     * @param complexData : complex part of scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorComplex(double[][] realData, double[][] complexData, double[] rowsIndex, double[] colsIndex, String variableName) {
        int c = realData.length == 0 ? 0 : realData[0].length;
        Double[][][] dataDoubleComplex = new Double[realData.length][c][2];
        for (int i = 0; i < realData.length; i++) {
            for (int j = 0; j < c; j++) {
                dataDoubleComplex[i][j][0] = realData[i][j];
                dataDoubleComplex[i][j][1] = complexData[i][j];
            }
        }
        ScilabVariableEditor.refreshVariableEditor(COMPLEX, dataDoubleComplex, rowsIndex, colsIndex, variableName);
    }

    /**
     * Open variable Editor with information given by Scilab
     * @param data : scilab double matrix
     * @param variableName : name of the variable being edited.
     */
    public static void refreshVariableEditorBoolean(int[][] data, double[] rowsIndex, double[] colsIndex, String variableName) {
        int c = data.length == 0 ? 0 : data[0].length;
        Boolean[][] dataBool = new Boolean[data.length][c];
        for (int i = 0; i < data.length; i++) {
            for (int j = 0; j < c; j++) {
                dataBool[i][j] = data[i][j] == 1;
            }
        }
        ScilabVariableEditor.refreshVariableEditor(BOOLEAN, dataBool, rowsIndex, colsIndex, variableName);
    }

    /**
     * Close Variable Editor
     */
    public static void closeVariableEditor() {
        ScilabVariableEditor.closeEditVar();
    }
}
