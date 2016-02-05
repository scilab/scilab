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

package org.scilab.modules.ui_data.datatable;

import java.awt.Cursor;
import java.util.Vector;

import javax.swing.event.TableModelEvent;
import javax.swing.table.DefaultTableModel;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.EditVar;
import org.scilab.modules.ui_data.UI_data;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;
import org.scilab.modules.ui_data.variableeditor.celleditor.ScilabGenericCellEditor;
import org.scilab.modules.ui_data.variableeditor.undo.CellsUndoManager;
import org.scilab.modules.ui_data.variableeditor.undo.CellsUndoableEdit;

/**
 * Swing implementation of table model.
 * @author Allan SIMON
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class SwingEditvarTableModel extends DefaultTableModel {

    private static final long serialVersionUID = -4255704246347716837L;
    private static final String GUILL = "\"";
    private static final String EQUAL = "=";
    private static final String COMMA = ",";
    private static final String SEMI = ";";

    private int scilabMatrixRowCount;
    private int scilabMatrixColCount;
    private int currentRow = -1;
    private int currentCol = -1;
    private String type;
    private String varName;
    private ScilabGenericCellEditor cellEditor;
    private SwingScilabVariableEditor editor;
    private CellsUndoManager undoManager;

    private Vector copyVector;

    /**
     * Default construction setting table data.
     * @param editor the editor
     * @param varName the variable name
     * @param type the variable type
     * @param data : the data to store.
     * @param cellEditor the cellEditor
     */
    public SwingEditvarTableModel(SwingScilabVariableEditor editor, String varName, String type, Object[][] data, ScilabGenericCellEditor cellEditor, int rows, int cols) {
        this.editor = editor;
        this.varName = varName;
        scilabMatrixRowCount = data.length;
        scilabMatrixColCount = 0;
        if (scilabMatrixRowCount != 0) { // Not an empty matrix
            scilabMatrixColCount =  data[0].length;
        }
        this.type = type;
        this.setDataVector(data, rows, cols);
        this.cellEditor = cellEditor;
        this.undoManager = new CellsUndoManager(editor);
    }

    /**
     * Change the data
     * @param type the data type
     * @param data the datas
     * @param cellEditor the cell editor
     * @param rows the number of rows
     * @param cols the number of columns
     */
    public void changeData(String type, Object[][] data, ScilabGenericCellEditor cellEditor, int rows, int cols) {
        this.type = type;
        this.cellEditor = cellEditor;
        scilabMatrixRowCount = data.length;
        scilabMatrixColCount = 0;
        if (scilabMatrixRowCount != 0) { // Not an empty matrix
            scilabMatrixColCount =  data[0].length;
        }
        setDataVector(data, rows, cols);
    }

    /**
     * Change all the data
     * @param v the data vector
     * @param row the number of rows
     * @param row the number of columns
     */
    public void changeData(Vector v, int row, int col) {
        dataVector = v;
        scilabMatrixRowCount = row;
        scilabMatrixColCount = col;
        updateFullMatrix(null, 0, 0);
    }

    /**
     * {@inheritDoc}
     */
    public void setDataVector(Object[][] data, int rows, int cols) {
        int colsData = 0;
        if (data.length != 0) { // Not an empty matrix
            colsData = data[0].length;
        }

        int c = Math.max(cols, colsData);

        Vector identifiers = new Vector(c);
        for (int i = 0; i < c; ++i) {
            identifiers.addElement(i + 1);
        }
        super.setDataVector(convertData(data, rows, cols), identifiers);
    }

    /**
     * Convert datas as arrays in a Vector object
     * @param data the datas
     * @param rows the number of rows
     * @param columns the number of columns
     * @return the corresponding Vector
     */
    protected Vector convertData(Object[][] data, int rows, int cols) {
        int r = Math.max(data.length, rows);
        int c = 0;
        if (data.length != 0) {
            c = Math.max(data[0].length, cols);
        }
        Vector rv = new Vector(r);
        for (int i = 0; i < data.length; i++) {
            Vector cv = new Vector(c);
            for (int j = 0; j < data[i].length; j++) {
                cv.addElement(data[i][j]);
            }
            for (int j = 0; j < cols - data[i].length; j++) {
                cv.addElement(null);
            }
            rv.addElement(cv);
        }
        for (int i = 0; i < rows - data.length; i++) {
            Vector cv = new Vector(c);
            for (int j = 0; j < c; j++) {
                cv.addElement(null);
            }
            rv.addElement(cv);
        }

        return rv;
    }

    /**
     * Enlarge the model
     * @param rows the number of rows
     * @param columns the number of columns
     * @return true if the model has been enlarged
     */
    public boolean enlarge(int rows, int cols) {
        boolean isEnlarged = false;

        if (cols > getColumnCount()) {
            addColumns(cols - getColumnCount());
            isEnlarged = true;
        }
        if (rows > getRowCount()) {
            int r = rows - getRowCount();
            for (int i = 0; i < r; i++) {
                Vector v = new Vector(getColumnCount());
                for (int j = 0; j < getColumnCount(); j++) {
                    v.addElement(null);
                }
                dataVector.addElement(v);
            }
            isEnlarged = true;
        }

        return isEnlarged;
    }

    /**
     * Clone the datas
     * @return the cloned datas
     */
    public Vector cloneDatas() {
        Vector v = new Vector(dataVector.size());
        for (int i = 0; i < dataVector.size(); i++) {
            Vector vv = (Vector) dataVector.get(i);
            if (vv != null) {
                v.addElement(vv.clone());
            } else {
                v.addElement(null);
            }
        }
        return v;
    }

    /**
     * Add columns to the model
     * @param number the number of columns to add
     */
    public void addColumns(int number) {
        columnIdentifiers.setSize(getColumnCount() + number);
        for (int i = 0; i < getRowCount(); i++) {
            ((Vector) dataVector.elementAt(i)).setSize(getColumnCount());
        }
    }

    /**
     * Add rows
     * @param number the number of rows to add
     */
    public void addRows(int number) {
        enlarge(getRowCount() + number, getColumnCount());
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
     * @return the type of this tableModel
     */
    public String getType() {
        return type;
    }

    /**
     * @return the type of this tableModel
     */
    public void setType(String type) {
        this.type = type;
    }

    /**
     * @return the Scilab matrix rows number
     */
    public int getScilabMatrixRowCount() {
        return scilabMatrixRowCount;
    }

    /**
     * @return the Scilab matrix columns number
     */
    public int getScilabMatrixColCount() {
        return scilabMatrixColCount;
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
            str = GUILL + getDataAsScilabString(str) + GUILL;
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
    public void setValueAtAndUpdate(boolean mustUpdate, boolean addUndoableEdit, Object value, int row, int col) {
        Object oldValue = addUndoableEdit ? getValueAt(row, col) : null;
        int oldScilabMatrixRowCount = scilabMatrixRowCount;
        int oldScilabMatrixColCount = scilabMatrixColCount;

        copyVector = null;
        boolean defaultAdded = setValue(value, row, col);

        if (defaultAdded) {
            fireTableChanged(new TableModelEvent(this));
        } else {
            fireTableCellUpdated(row, col);
        }

        if (mustUpdate) {
            if (defaultAdded) {
                updateFullMatrix(copyVector, oldScilabMatrixRowCount, oldScilabMatrixColCount);
            } else {
                updateMatrix(oldValue, row, col);
            }
        }
    }

    /**
     * Set a value at the given row and column
     * @param value the value to set
     * @param row the row
     * @param col the column
     * @return true if the matrix has been enlarged (typically when the value to set is outside the actual matrix, zeros are added)
     */
    public boolean setValue(Object value, int row, int col) {
        if (value == null || (value instanceof String && ((String) value).length() == 0)) {
            return false;
        }

        Object defaultValue = getDefaultValue();
        boolean defaultAdded = false;

        if (row >= scilabMatrixRowCount) {
            for (int i = scilabMatrixRowCount; i <= row; i++) {
                Vector rowVector = (Vector) dataVector.elementAt(i);
                for (int j = 0; j <= Math.max(col, scilabMatrixColCount - 1); j++) {
                    rowVector.setElementAt(defaultValue, j);
                    if (copyVector == null) {
                        copyVector = (Vector) dataVector.clone();
                    }
                    if (!defaultAdded) {
                        defaultAdded = true;
                    }
                }
            }
        }

        if (col >= scilabMatrixColCount) {
            for (int i = 0; i <= Math.max(row, scilabMatrixRowCount - 1); i++) {
                Vector rowVector = (Vector) dataVector.elementAt(i);
                for (int j = scilabMatrixColCount; j <= col; j++) {
                    rowVector.setElementAt(defaultValue, j);
                    if (copyVector == null) {
                        copyVector = (Vector) dataVector.clone();
                    }
                    if (!defaultAdded) {
                        defaultAdded = true;
                    }
                }
            }
        }

        ((Vector) dataVector.elementAt(row)).setElementAt(value, col);

        if (col >= scilabMatrixColCount || row >= scilabMatrixRowCount) {
            if (col >= scilabMatrixColCount) {
                scilabMatrixColCount = col + 1;
            }
            if (row >= scilabMatrixRowCount) {
                scilabMatrixRowCount = row + 1;
            }
        }

        return defaultAdded;
    }

    /**
     * Set values at the given row and column
     * @param data the datas
     * @param row the row index
     * @param col the column index
     * @return true if the matrix has been enlarged
     */
    public boolean setValues(Vector data, int row, int col) {
        int countCols = 0;
        for (int i = 0; i < data.size(); i++) {
            countCols = Math.max(countCols, ((Vector) data.get(i)).size());
        }
        boolean isModified = enlarge(row + data.size(), col + countCols);

        for (int i = 0; i < data.size(); i++) {
            for (int j = 0; j < ((Vector) data.get(i)).size(); j++) {
                isModified = setValue(((Vector) data.get(i)).get(j), row + i, col + j) || isModified;
            }
        }

        return isModified;
    }

    /**
     * {@inheritDoc}
     */
    public void setValueAt(Object value, int row, int col) {
        setValueAtAndUpdate(true, true, value, row, col);
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
            Vector v = (Vector) dataVector.get(row);
            for (int i = 0; i < scilabMatrixColCount; i++) {
                v.setElementAt(null, i);
            }
        }
    }

    /**
     * @param table where to remove the col
     * @param col the col
     * @param rowB the first row
     * @param rowE the last row
     */
    public void removeCol(int col, int rowB, int rowE) {
        if (col == scilabMatrixColCount - 1 && rowB == 0 && rowE >= scilabMatrixRowCount - 1) {
            scilabMatrixColCount--;
            for (int i = 0; i < scilabMatrixRowCount; i++) {
                ((Vector) dataVector.get(i)).setElementAt(null, col);
            }
        }
    }

    /**
     * @param cols the column indexes to remove, the array is supposed to be ordered
     */
    public void removeColumns(int[] cols) {
        Vector copy = null;
        int oldRowCount = scilabMatrixRowCount;
        int oldColCount = scilabMatrixColCount;
        int c = 0;
        for (Integer col : cols) {
            if (col - c < scilabMatrixColCount) {
                if (copy == null) {
                    copy = cloneDatas();
                }
                if (col - c == scilabMatrixColCount - 1) {
                    for (int i = 0; i < scilabMatrixRowCount; i++) {
                        ((Vector) dataVector.get(i)).setElementAt(null, col - c);
                    }
                } else {
                    for (int i = 0; i < scilabMatrixRowCount; i++) {
                        ((Vector) dataVector.get(i)).removeElementAt(col - c);
                    }
                }
                scilabMatrixColCount--;
                c++;
            }
        }
        if (copy != null) {
            fireTableStructureChanged();
            updateFullMatrix(copy, oldRowCount, oldColCount);
        }
    }

    /**
     * @param rows the row indexes to remove, the array is supposed to be ordered
     */
    public void removeRows(int[] rows) {
        Vector copy = null;
        int oldRowCount = scilabMatrixRowCount;
        int oldColCount = scilabMatrixColCount;
        int r = 0;
        for (Integer row : rows) {
            if (row - r < scilabMatrixRowCount) {
                if (copy == null) {
                    copy = cloneDatas();
                }
                if (row - r == scilabMatrixRowCount - 1) {
                    Vector rv = (Vector) dataVector.get(row - r);
                    for (int i = 0; i < scilabMatrixColCount; i++) {
                        rv.setElementAt(null, i);
                    }
                } else {
                    dataVector.removeElementAt(row - r);
                }
                scilabMatrixRowCount--;
                fireTableRowsDeleted(row - r, row - r);
                r++;
            }
        }
        if (copy != null) {
            updateFullMatrix(copy, oldRowCount, oldColCount);
        }
    }

    /**
     * @param col the column index to insert
     */
    public void insertColumn(int col) {
        if (col < scilabMatrixColCount - 1) {
            Vector copy = cloneDatas();
            int oldRowCount = scilabMatrixRowCount;
            int oldColCount = scilabMatrixColCount;
            Object o = getDefaultValue();
            for (int i = 0; i < scilabMatrixRowCount; i++) {
                ((Vector) dataVector.get(i)).insertElementAt(o, col);
            }
            scilabMatrixColCount++;
            updateFullMatrix(copy, oldRowCount, oldColCount);
        }
    }

    /**
     * @param row the row index to insert
     */
    public void insertRow(int row) {
        if (row < scilabMatrixRowCount - 1) {
            Vector copy = cloneDatas();
            int oldRowCount = scilabMatrixRowCount;
            int oldColCount = scilabMatrixColCount;
            Object o = getDefaultValue();
            Vector v = new Vector(getColumnCount());
            for (int i = 0; i < scilabMatrixColCount; i++) {
                v.addElement(o);
            }
            for (int i = scilabMatrixColCount; i < getColumnCount(); i++) {
                v.addElement(null);
            }
            dataVector.insertElementAt(v, row);
            scilabMatrixRowCount++;
            updateFullMatrix(copy, oldRowCount, oldColCount);
        }
    }

    /**
     * Update all the matrix on the Scilab's side.
     */
    public void updateFullMatrix(Object oldValue, int oldRow, int oldCol) {
        String[] mat = getSubMatrix(0, scilabMatrixRowCount - 1, 0, scilabMatrixColCount - 1);
        UI_data.putScilabVariable("L?8625083632641564278", mat, scilabMatrixRowCount, scilabMatrixColCount);
        String command;
        if (type.equals(EditVar.SPARSE) || type.equals(EditVar.COMPLEXSPARSE) || type.equals(EditVar.BOOLEANSPARSE)) {
            command = buildScilabRequest(varName + "=sparse(evstr(L?8625083632641564278))", "editvar(\"" + varName + "\"),clear(\"L?8625083632641564278\")");
        } else {
            command = buildScilabRequest(varName + "=evstr(L?8625083632641564278)", "editvar(\"" + varName + "\"),clear(\"L?8625083632641564278\")");
        }
        execCommand(command, oldValue, oldRow, oldCol);
    }

    /**
     * Get a submatrix
     * @param rowB first row index
     * @param rowE last row index
     * @param colB first col index
     * @param colE last col index
     * @return the corresponding submatrix
     */
    private String[] getSubMatrix(int rowB, int rowE, int colB, int colE) {
        int row = rowE - rowB + 1;
        int col = colE - colB + 1;
        String[] mat = new String[row * col];
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                mat[j * row + i] = getScilabValueAt(i + rowB, j + colB);
            }
        }

        return mat;
    }

    /**
     * Get a Scilab submatrix
     * @param varName the variable name
     * @param rowB first row index
     * @param rowE last row index
     * @param colB first col index
     * @param colE last col index
     */
    public String getScilabSubMatrix(int[] rows, int[] cols, int[] rowSize, int[] colSize) {
        if (rows.length == 0 || cols.length == 0) {
            return null;
        }

        int rowC = getScilabMatrixRowCount() - 1;
        int colC = getScilabMatrixColCount() - 1;
        String rowsLim = getLimits(rows, rowC, rowSize);
        if (rowsLim == null) {
            return null;
        }

        String colsLim = getLimits(cols, colC, colSize);
        if (colsLim == null) {
            return null;
        }

        if (rowSize[0] == rowC + 1 && colSize[0] == colC + 1) {
            return getVarName();
        } else {
            return getVarName() + "(" + rowsLim + "," + colsLim + ")";
        }
    }

    public String getScilabSubMatrix(int[] rows, int[] cols) {
        int[] csize = new int[1];
        int[] rsize = new int[1];

        return getScilabSubMatrix(rows, cols, csize, rsize);
    }

    /**
     * Build the Scilab indexes to extract the corresponding matrix.
     * For example, if index={1,2,3,5,6,10} then the string "1:3,5:6,10" will be returned.
     * @param index the indexes
     * @param max the maximum value that an index can have
     * @param size an array of length 1 which will contain the total size (e.g. with the previous example size[0] will be 6)
     * @return the corresponding Scilab string
     */
    private static String getLimits(int[] index, int max, int[] size) {
        if (index.length == 0 || index[0] > max) {
            return null;
        }

        Vector<Integer> lim = new Vector<Integer>(2 * index.length);
        int i = 1;

        lim.addElement(index[0]);
        for (; i < index.length && index[i] <= max; i++) {
            if (index[i] != index[i - 1] + 1) {
                lim.addElement(index[i - 1]);
                lim.addElement(index[i]);
            }
        }

        if (i == index.length) {
            lim.addElement(index[index.length - 1]);
        } else {
            lim.addElement(max);
        }

        if (lim.size() == 2) {
            size[0] = lim.get(1) - lim.get(0) + 1;
            if (lim.get(0) == 0 && lim.get(1) == max) {
                return ":";
            } else {
                if (lim.get(0) == lim.get(1)) {
                    return Integer.toString(lim.get(0) + 1);
                } else {
                    return (lim.get(0) + 1) + ":" + (lim.get(1) + 1);
                }
            }
        }

        StringBuilder buf = new StringBuilder("[");
        size[0] = 0;
        for (i = 0; i < lim.size(); i += 2) {
            size[0] += lim.get(i + 1) - lim.get(i) + 1;
            buf.append(lim.get(i) + 1);
            if (lim.get(i) != lim.get(i + 1)) {
                buf.append(":");
                buf.append(lim.get(i + 1) + 1);
            }
            if (i == lim.size() - 2) {
                buf.append("]");
            } else {
                buf.append(",");
            }
        }

        return buf.toString();
    }

    /**
     * Refresh the current matrix
     */
    public void refreshMatrix() {
        execCommand("editvar(\"" + varName + "\");");
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
        if (type.equals(EditVar.DOUBLE)) {
            ret = new Double(0.0);
        } else if (type.equals(EditVar.STRING)) {
            ret = "";
        } else if (type.equals(EditVar.INTEGER)) {
            ret = new Integer(0);
        } else if (type.equals(EditVar.COMPLEX)) {
            ret = new Double[] {0.0, 0.0};
        } else if (type.equals(EditVar.BOOLEAN)) {
            ret = Boolean.FALSE;
        } else if (type.equals(EditVar.SPARSE) || type.equals(EditVar.COMPLEXSPARSE) || type.equals(EditVar.BOOLEANSPARSE)) {
            ret = "";
        }

        return ret;
    }

    /**
     * @return the default representation according to the type
     */
    private String getDefaultStringValue() {
        if (type.equals(EditVar.DOUBLE) || type.equals(EditVar.COMPLEX) || type.equals(EditVar.INTEGER) || type.equals(EditVar.SPARSE) || type.equals(EditVar.COMPLEXSPARSE)) {
            return "0";
        }

        if (type.equals(EditVar.STRING)) {
            return "";
        }

        if (type.equals(EditVar.BOOLEAN) || type.equals(EditVar.BOOLEANSPARSE)) {
            return "%f";
        }

        return null;
    }

    /**
     * @param oldValue the value to update on the Scilab's side
     * @param row the row coord
     * @param col the col coord
     */
    public void updateMatrix(Object oldValue, int row, int col) {
        String val = getScilabValueAt(row, col);
        String coords = "";
        if (scilabMatrixRowCount != 1 || scilabMatrixColCount != 1) {
            coords = "(" + (row + 1) + COMMA + (col + 1) + ")";
        }
        String expr = varName + coords + EQUAL + getDataAsScilabString(val);
        String update = "editvar(\"" + varName + "\"," + varName + "(" + (row + 1) + "," + (col + 1) + ")," + (row + 1) + "," + (col + 1) + ");";
        String command = buildScilabRequest(expr, update);
        execCommand(command, oldValue, row, col);
    }

    /**
     * Execute a command
     * @param com the command
     */
    public void execCommand(String com) {
        execCommand(com, null, 0 , 0);
    }

    /**
     * @param com the command to execute.
     */
    private void execCommand(final String com, final Object oldValue, final int row, final int col) {
        try {
            CommonCallBack callback = new CommonCallBack("") {
                public void callBack() {
                    editor.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
                    if (oldValue != null) {
                        Object newValue;
                        if (oldValue instanceof Vector) {
                            newValue = (Vector) dataVector.clone();
                        } else {
                            newValue = getValueAt(row, col);
                        }
                        if (!oldValue.equals(newValue)) {
                            undoManager.addEdit(new CellsUndoableEdit(SwingEditvarTableModel.this, newValue, oldValue, row, col));
                        }
                    }
                }
            };
            editor.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
            ScilabInterpreterManagement.asynchronousScilabExec(callback, com);
        } catch (InterpreterException e1) {
            System.err.println(e1);
        }
    }

    /**
     * @param expr the expr to put in the request.
     * @return the request
     */
    public static String buildScilabRequest(String expr, String update) {
        StringBuilder command = new StringBuilder();
        command.append("L$8625083632641564277=warning(\"query\");warning(\"off\");");
        command.append("if execstr(\"");
        command.append(expr);
        command.append("\",\"errcatch\") <> 0 then messagebox(\"Could not edit variable: \" + lasterror() + \"\"");
        command.append(",\"Variable editor\",\"error\",\"modal\");");
        command.append("end;");
        command.append("warning(L$8625083632641564277);clear(\"L$8625083632641564277\");");
        command.append(update);
        command.append(";");
        command.append("updatebrowsevar()");
        return command.toString();
    }

    /**
     * @return the Scilab matrix for this JTable
     */
    private String getMatrix() {
        StringBuilder str = new StringBuilder("[");
        for (int i = 0; i < scilabMatrixRowCount; i++) {
            for (int j = 0; j < scilabMatrixColCount; j++) {
                str.append(getScilabValueAt(i, j));
                if (j < scilabMatrixColCount - 1) {
                    str.append(COMMA);
                } else if (i < scilabMatrixRowCount - 1) {
                    str.append(SEMI);
                }
            }
        }
        str.append("]");

        return str.toString();
    }

    /**
     * Refresh the model
     */
    public void refresh() {
        try {
            ScilabInterpreterManagement.asynchronousScilabExec(null, "editvar(\"" + varName + "\")");
        } catch (InterpreterException e1) {
            System.err.println(e1);
        }
    }

    /**
     * @param str the string where replace " and '
     * @return the well-formed string
     */
    public static String getDataAsScilabString(String str) {
        return str.replace(GUILL, "\"\"").replace("'", "''");
    }
}
