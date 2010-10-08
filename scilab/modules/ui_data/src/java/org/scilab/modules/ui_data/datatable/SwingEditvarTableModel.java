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

package org.scilab.modules.ui_data.datatable;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.ui_data.EditVar;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;
import org.scilab.modules.ui_data.variableeditor.celleditor.ScilabGenericCellEditor;
import org.scilab.modules.ui_data.variableeditor.undo.CellsUndoManager;
import org.scilab.modules.ui_data.variableeditor.undo.CellsUndoableEdit;

/**
 * Swing implementation of table model.
 * @author Allan SIMON
 * @author Calixte DENIZET
 */
public class SwingEditvarTableModel extends DefaultTableModel {

    private static final long serialVersionUID = -4255704246347716837L;
    private static final String GUILL = "\"";
    private static final String EQUAL = "=";
    private static final String COMMA = ",";
    private static final String SEMI = ";";

    private static final ActionListener EMPTYACTION = new ActionListener() {
            public void actionPerformed(ActionEvent e) { }
        };

    private int scilabMatrixRowCount;
    private int scilabMatrixColCount;
    private int currentRow = -1;
    private int currentCol = -1;
    private String type;
    private String varName;
    private ScilabGenericCellEditor cellEditor;
    private SwingScilabVariableEditor editor;
    private CellsUndoManager undoManager;

    /**
     * Default construction setting table data.
     * @param editor the editor
     * @param varName the variable name
     * @param type the variable type
     * @param data : the data to store.
     * @param cellEditor the cellEditor
     */
    public SwingEditvarTableModel(SwingScilabVariableEditor editor, String varName, String type, Object[][] data, ScilabGenericCellEditor cellEditor) {
        this.editor = editor;
        this.varName = varName;
        scilabMatrixRowCount = data.length;
        scilabMatrixColCount = 0;
        if (scilabMatrixRowCount != 0) { // Not an empty matrix
            scilabMatrixColCount =  data[0].length;
        }
        this.type = type;
        this.setDataVector(data);
        this.cellEditor = cellEditor;
        this.undoManager = new CellsUndoManager(editor, getCommandFromMatrix());
        expandTable();
    }

    /**
     * {@inheritDoc}
     */
    public void setDataVector(Object[][] data) {
        int cols = 0;
        if (data.length != 0) { // Not an empty matrix
            cols = data[0].length;
        }
        Integer[] identifiers = new Integer[cols];
        for (int i = 0; i < cols; ++i) {
            identifiers[i] = i + 1;
        }
        super.setDataVector(data, identifiers);

    }

    /**
     * @return the CellEditor used in this tableModel
     */
    public ScilabGenericCellEditor getCellEditor() {
        return cellEditor;
    }

    /**
     * @return the undoManager used in this tableModel
     */
    public CellsUndoManager getUndoManager() {
        return undoManager;
    }

    /**
     * @param manager the undoManager to set in this tableModel
     */
    public void setUndoManager(CellsUndoManager manager) {
        this.undoManager = manager;
    }

    /**
     * @return the tyype of this tableModel
     */
    public String getType() {
        return type;
    }

    /**
     * @param row the row
     * @param col the col
     * @param useDefault if true, null is replaced by the default value
     * @return the scilab expression corresponding to the value
     */
    public String getScilabValueAt(int row, int col, boolean useDefault) {
        String str = cellEditor.getDataAsScilabString(getValueAt(row, col));
        if (!useDefault && getValueAt(row, col) == null) {
            return null;
        }

        if (str.length() == 0) {
            str = getDefaultStringValue();
        }

        if (type.equals(EditVar.STRING)) {
            str = GUILL + str + GUILL;
        }

        return str;
    }

    /**
     * @param row the row
     * @param col the col
     * @return the scilab expression corresponding to the value
     */
    public String getScilabValueAt(int row, int col) {
        return getScilabValueAt(row, col, true);
    }

    /**
     * @param row the row
     * @param col the col
     */
    public void emptyValueAt(int row, int col) {
        super.setValueAt(null, row, col);
    }

    /**
     * @param mustUpdate if true, update in Scilab (useful with paste action)
     * @param value the value
     * @param row the row
     * @param col the col
     */
    public void setValueAtAndUpdate(boolean mustUpdate, Object value, int row, int col) {
        if (value == null || (value instanceof String && ((String) value).length() == 0)) {
            return;
        }

        boolean defaultAdded = false;
        for (int i = scilabMatrixRowCount; i <= Math.max(row, scilabMatrixRowCount - 1); i++) {
            for (int j = 0; j <= Math.max(col, scilabMatrixColCount - 1); j++) {
                super.setValueAt(getDefaultValue(), i, j);
                if (!defaultAdded) {
                    defaultAdded = true;
                }
            }
        }

        for (int i = scilabMatrixColCount; i <= Math.max(col, scilabMatrixColCount - 1); i++) {
            for (int j = 0; j <= Math.max(row, scilabMatrixRowCount - 1); j++) {
                super.setValueAt(getDefaultValue(), j, i);
                if (!defaultAdded) {
                    defaultAdded = true;
                }
            }
        }

        String exp = value.toString();

        if (exp.startsWith(EQUAL) && !exp.matches("=[ ]*")) {
            exp = exp.substring(1);
            super.setValueAt(exp, row, col);
            // keep the expression
            if (exp.length() > 0) {
                cellEditor.addExpression(exp, row, col);
            }
        } else {
            cellEditor.removeExpression(row, col);
            super.setValueAt(value, row, col);
        }

        if (col >= scilabMatrixColCount || row >= scilabMatrixRowCount) {
            if (col >= scilabMatrixColCount) {
                scilabMatrixColCount = col + 1;
            }
            if (row >= scilabMatrixRowCount) {
                scilabMatrixRowCount = row + 1;
            }
            expandTable();
        }

        if (mustUpdate) {
            if (defaultAdded) {
                updateMatrix();
            } else {
                updateMatrix(exp, row, col);
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void setValueAt(Object value, int row, int col) {
        setValueAtAndUpdate(true, value, row, col);
        currentRow = row;
        currentCol = col;
    }

    /**
     * @return the current row
     */
    public int getCurrentRow() {
        return currentRow;
    }

    /**
     * @return the current col
     */
    public int getCurrentCol() {
        return currentCol;
    }

    /**
     * @param row the row
     * @param colB the first column
     * @param colE the last column
     */
    public void removeRow(int row, int colB, int colE) {
        if (row == scilabMatrixRowCount - 1 && colB == 0 && colE >= scilabMatrixColCount - 1) {
            scilabMatrixRowCount--;
            removeRow(row);
        }
    }

    /**
     * @param table where to remove the col
     * @param col the col
     * @param rowB the first row
     * @param rowE the last row
     */
    public void removeCol(JTable table, int col, int rowB, int rowE) {
        if (col == scilabMatrixColCount - 1 && rowB == 0 && rowE >= scilabMatrixRowCount - 1) {
            scilabMatrixColCount--;
            table.removeColumn(table.getColumnModel().getColumn(col));
        }
    }

    /**
     * Update all the matrix on the Scilab's side.
     */
    public void updateMatrix() {
        execCommand(getCommandFromMatrix());
    }

    /**
     * Expand the table
     */
    public void expandTable() {
        int newTableColCount = 42;
        int newTableRowCount = 42;
        // row
        if (getColumnCount() > 32) {
            newTableColCount = scilabMatrixColCount + 10;
        }
        for (int i = getColumnCount(); i < newTableColCount; i++) {
            addColumn(i + 1);
        }
        // col
        if (getRowCount() > 32) {
            newTableRowCount = scilabMatrixRowCount + 10;
        }
        for (Integer i = getRowCount(); i < newTableRowCount; i++) {
            Object[] newRow = new Object[newTableColCount];
            addRow(newRow);
        }
    }

    /**
     * Refresh the current matrix
     */
    public void refreshMatrix() {
        execCommand("editvar(\"" + varName + "\");");
    }

    /**
     * @param command the command to execute.
     */
    public void updateCommand(String command) {
        try {
            ScilabInterpreterManagement.asynchronousScilabExec(EMPTYACTION, command);
        } catch (InterpreterException e1) {
            System.err.println(e1);
        }
    }

    /**
     * Update all the matrix on the Scilab's side.
     * @return the command to create the matrix
     */
    public String getCommandFromMatrix() {
        String matrix = getDataAsScilabString(getMatrix());
        String expr = varName + EQUAL + matrix;
        return buildScilabRequest(expr);
    }

    /**
     * @return the var name in this model
     */
    public String getVarName() {
        return varName;
    }

    /**
     * @return the default value according to this type
     */
    private Object getDefaultValue() {
        Object ret = null;
        if (type.equals(EditVar.STRING)) {
            ret = "";
        } else if (type.equals(EditVar.DOUBLE)) {
            ret = new Double(0.0);
        } else if (type.equals(EditVar.INTEGER)) {
            ret = new Integer(0);
        } else if (type.equals(EditVar.COMPLEX)) {
            ret = new Double[]{0.0, 0.0};
        } else if (type.equals(EditVar.BOOLEAN)) {
            ret = Boolean.FALSE;
        }

        return ret;
    }

    /**
     * @return the default representation according to the type
     */
    private String getDefaultStringValue() {
        if (type.equals(EditVar.STRING)) {
            return "";
        }

        if (type.equals(EditVar.DOUBLE) || type.equals(EditVar.COMPLEX) || type.equals(EditVar.INTEGER)) {
            return "0";
        }

        if (type.equals(EditVar.BOOLEAN)) {
            return "%f";
        }

        return null;
    }

    /**
     * @param value the value to update on the Scilab's side
     * @param row the row coord
     * @param col the col coord
     */
    private void updateMatrix(Object value, int row, int col) {
        String val = getScilabValueAt(row, col);
        String coords = "";
        if (scilabMatrixRowCount != 1 || scilabMatrixColCount != 1) {
            coords = "(" + (row + 1) + COMMA + (col + 1) + ")";
        }
        String expr = varName + coords + EQUAL + getDataAsScilabString(val);
        String command = buildScilabRequest(expr);
        execCommand(command);
    }

    /**
     * @param com the command to execute.
     */
    private void execCommand(final String com) {
        try {
            CallBack callback = new CallBack("") {
                    public void callBack() {
                        undoManager.addEdit(new CellsUndoableEdit(SwingEditvarTableModel.this));
                    }
                };

            ScilabInterpreterManagement.asynchronousScilabExec(callback, com);
        } catch (InterpreterException e1) {
            System.err.println(e1);
        }
    }

    /**
     * @param expr the expr to put in the request.
     * @return the request
     */
    private String buildScilabRequest(String expr) {
        StringBuilder command = new StringBuilder();
        command.append("L8625083632641564277=warning(\"query\");warning(\"off\");");
        command.append("if execstr(\"" + expr + SEMI + cellEditor.getAllExpressions(varName) + "\", \"errcatch\") <> 0 then ");
        command.append("messagebox(\"Could not edit variable: \" + lasterror() + \"\"");
        command.append(",\"Variable editor\", \"error\", \"modal\");");
        command.append("end;");
        command.append("warning(L8625083632641564277);clear(L8625083632641564277);");
        command.append("editvar(\"" + varName + "\");");
        return command.toString();
    }

    /**
     * @return the Scilab matrix for this JTable
     */
    private String getMatrix() {
        String str = "[";
        for (int i = 0; i < scilabMatrixRowCount; i++) {
            for (int j = 0; j < scilabMatrixColCount; j++) {
                if (j < scilabMatrixColCount - 1) {
                    str += getScilabValueAt(i, j) + COMMA;
                } else if (i < scilabMatrixRowCount - 1) {
                    str += getScilabValueAt(i, j) + SEMI;
                } else {
                    str += getScilabValueAt(i, j);
                }
            }
        }

        str += "]";

        return str;
    }

    /**
     * @param str the string where replace " and '
     * @return the well-formed string
     */
    private String getDataAsScilabString(String str) {
        String data = str.replace(GUILL, "\"\""); // Change " to "" because added in an execstr command
        data = data.replace("'", "''"); // Change ' to '' because added in an execstr command

        return data;
    }
}
