/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.ui_data.variableeditor;

import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.util.Arrays;
import java.util.Vector;

import javax.swing.BoundedRangeModel;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.TableModelEvent;
import javax.swing.table.JTableHeader;
import javax.swing.table.TableColumn;
import javax.swing.table.TableCellEditor;

import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.rowheader.HeaderRenderer;
import org.scilab.modules.ui_data.rowheader.RowHeader;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.variableeditor.actions.InsertColumnAction;
import org.scilab.modules.ui_data.variableeditor.actions.RemoveColumnAction;

/**
 * The table
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class TableVariableEditor extends JTable {

    private JScrollPane scrollPane;
    private int clickedColumn;
    private int[] selectedRows = new int[0];
    private int[] selectedCols = new int[0];
    private int popupRow;
    private int popupCol;
    private SwingScilabVariableEditor editor;

    /**
     * Default constructor
     */
    public TableVariableEditor(SwingScilabVariableEditor editor) {
        super();
        this.editor = editor;
        scrollPane = new JScrollPane(this);
        /* Modify the table size when the knob is at the bottom of the vertical scrollbar */
        scrollPane.getVerticalScrollBar().addAdjustmentListener(new AdjustmentListener() {
            public void adjustmentValueChanged(AdjustmentEvent e) {
                BoundedRangeModel brm = scrollPane.getVerticalScrollBar().getModel();
                if (brm.getMaximum() - brm.getExtent() - e.getValue() <= 1) {
                    addTenRows();
                }
            }
        });

        /* Modify the table size when the knob is at the right of the horizontal scrollbar */
        scrollPane.getHorizontalScrollBar().addAdjustmentListener(new AdjustmentListener() {
            public void adjustmentValueChanged(AdjustmentEvent e) {
                BoundedRangeModel brm = scrollPane.getHorizontalScrollBar().getModel();
                if (brm.getMaximum() - brm.getExtent() - e.getValue() <= 1) {
                    addTenCols();
                }
            }
        });

        /* Modify the table size when the scrollpane is resized */
        scrollPane.getViewport().addComponentListener(new ComponentAdapter() {
            public void componentResized(ComponentEvent e) {
                if (((SwingEditvarTableModel) getModel()).enlarge(getMinimalRowNumber() + 1, getMinimalColumnNumber() + 1)) {
                    ((SwingEditvarTableModel) getModel()).fireTableChanged(new TableModelEvent(getModel()));
                }
            }
        });

        getTableHeader().setDefaultRenderer(new HeaderRenderer(this, false));

        /* Handle left click on the column header */
        getTableHeader().addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                if (getTableHeader().getCursor().getType() == Cursor.DEFAULT_CURSOR && SwingUtilities.isLeftMouseButton(e)) {
                    JTable table = TableVariableEditor.this;
                    int column = table.getColumnModel().getColumnIndexAtX(e.getX());
                    ListSelectionModel csm = table.getColumnModel().getSelectionModel();
                    int rowC = ((SwingEditvarTableModel) getModel()).getScilabMatrixRowCount();
                    if (rowC == 0) {
                        rowC = 1;
                    }

                    if (e.isShiftDown()) {
                        csm.setSelectionInterval(column, clickedColumn);
                        table.setRowSelectionInterval(0, rowC - 1);
                    } else {
                        if ((e.getModifiers() & Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()) != 0) {
                            if (table.isColumnSelected(column)) {
                                csm.removeSelectionInterval(column, column);
                            } else {
                                csm.addSelectionInterval(column, column);
                                table.setRowSelectionInterval(0, rowC - 1);
                            }
                        } else {
                            csm.setSelectionInterval(column, column);
                            table.setRowSelectionInterval(0, rowC - 1);
                        }
                        clickedColumn = column;
                    }
                    table.requestFocus();
                }
            }
        });

        /* Handle left-click drag on the column header */
        getTableHeader().addMouseMotionListener(new MouseMotionAdapter() {
            public void mouseDragged(MouseEvent e) {
                if (getTableHeader().getCursor().getType() == Cursor.DEFAULT_CURSOR && SwingUtilities.isLeftMouseButton(e)) {
                    JTable table = TableVariableEditor.this;
                    Point p =  e.getPoint();
                    int column;
                    if (p.x >= table.getTableHeader().getHeaderRect(table.getColumnCount() - 1).x) {
                        column = table.getColumnCount() - 1;
                    } else {
                        column = table.getColumnModel().getColumnIndexAtX(p.x);
                    }

                    ListSelectionModel csm = table.getColumnModel().getSelectionModel();
                    int lead = csm.getLeadSelectionIndex();
                    if (lead != column) {
                        if (column == getColumnCount() - 1) {
                            ((TableVariableEditor) table).addTenCols();
                        }
                        table.scrollRectToVisible(table.getTableHeader().getHeaderRect(column));
                        if (e.isShiftDown()) {
                            csm.setSelectionInterval(clickedColumn, column);
                        } else {
                            if ((e.getModifiers() & Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()) != 0) {
                                if (csm.isSelectedIndex(column)) {
                                    csm.removeSelectionInterval(lead, column);
                                    csm.addSelectionInterval(clickedColumn, column);
                                } else {
                                    csm.addSelectionInterval(clickedColumn, column);
                                }
                            } else {
                                csm.setSelectionInterval(clickedColumn, column);
                            }
                        }
                        int rowC = ((SwingEditvarTableModel) getModel()).getScilabMatrixRowCount();
                        if (rowC == 0) {
                            rowC = 1;
                        }

                        table.setRowSelectionInterval(0, rowC - 1);
                        table.requestFocus();
                    }
                }
            }
        });

        getTableHeader().setReorderingAllowed(false);
        getTableHeader().setComponentPopupMenu(createPopupMenu());

        /* Click on the upper left corner selects all */
        scrollPane.setCorner(JScrollPane.UPPER_LEFT_CORNER, new JPanel());
        scrollPane.getCorner(JScrollPane.UPPER_LEFT_CORNER).addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                selectAll();
            }
        });

        /* Click on the lower left corner clear the selection */
        scrollPane.setCorner(JScrollPane.LOWER_LEFT_CORNER, new JPanel());
        scrollPane.getCorner(JScrollPane.LOWER_LEFT_CORNER).addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                clearSelection();
            }
        });

        setFillsViewportHeight(true);
        setRowHeight(18);
        scrollPane.setRowHeaderView(new RowHeader(this));
        setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        setCellSelectionEnabled(true);
        setBackground(Color.WHITE);
        setAutoResizeMode(AUTO_RESIZE_OFF);

        if (getGridColor().equals(Color.WHITE)) {
            setGridColor(new Color(128, 128, 128));
        }
        setShowHorizontalLines(true);
        setShowVerticalLines(true);
    }

    /**
     * {@inheritDoc}
     */
    public void valueChanged(ListSelectionEvent e) {
        int[] rows = getSelectedRows();
        Vector<Integer> rowDiff = getDifference(rows, selectedRows);
        JTable rowHeader = ((RowHeader) scrollPane.getRowHeader().getView()).getTable();
        for (int i : rowDiff) {
            rowHeader.repaint(rowHeader.getCellRect(i, 0, false));
        }
        selectedRows = rows;
        SwingEditvarTableModel model = (SwingEditvarTableModel) getModel();
        editor.getInfoBar().getAsSimpleTextBox().setText(model.getScilabSubMatrix(rows, getSelectedColumns()));

        super.valueChanged(e);
    }

    /**
     * {@inheritDoc}
     */
    public void columnSelectionChanged(ListSelectionEvent e) {
        int[] cols = getSelectedColumns();
        Vector<Integer> colDiff = getDifference(cols, selectedCols);
        JTableHeader colHeader = getTableHeader();
        for (int i : colDiff) {
            colHeader.repaint(colHeader.getHeaderRect(i));
        }
        selectedCols = cols;
        SwingEditvarTableModel model = (SwingEditvarTableModel) getModel();
        editor.getInfoBar().getAsSimpleTextBox().setText(model.getScilabSubMatrix(getSelectedRows(), cols));

        super.columnSelectionChanged(e);
    }

    /**
     * {@inheritDoc}
     */
    public void editingStopped(ChangeEvent e) {
        TableCellEditor editor = getCellEditor();
        if (editor != null) {
            Object value = editor.getCellEditorValue();
            if (value != null && !value.toString().isEmpty()) {
                super.editingStopped(e);
            } else {
                editingCanceled(e);
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void selectAll() {
        /* Just select the matrix not all the table */
        SwingEditvarTableModel model = (SwingEditvarTableModel) getModel();
        int rowC = model.getScilabMatrixRowCount();
        if (rowC == 0) {
            rowC = 1;
        }

        int colC = model.getScilabMatrixColCount();
        if (colC == 0) {
            colC = 1;
        }

        setColumnSelectionInterval(0, colC - 1);
        setRowSelectionInterval(0, rowC - 1);
    }

    /**
     * @return a popup menu for the column header
     */
    public JPopupMenu createPopupMenu() {
        JPopupMenu popup = new JPopupMenu() {
            public void show(Component invoker, int x, int y) {
                setPopupColumn(getColumnModel().getColumnIndexAtX(x));
                super.show(invoker, x, y);
            }
        };
        popup.setBorderPainted(true);
        popup.add(InsertColumnAction.createMenuItem(this, UiDataMessages.INSERTC));
        popup.add(RemoveColumnAction.createMenuItem(this, UiDataMessages.REMOVEC));

        return popup;
    }

    /**
     * @return the scrollpane used to display this table
     */
    public JScrollPane getScrollPane() {
        return scrollPane;
    }

    /**
     * @return the minimal row number according to the table size
     */
    public int getMinimalRowNumber() {
        Dimension dim = scrollPane.getViewport().getSize();
        return (int) ((double) dim.height / getRowHeight()) + 1;
    }

    /**
     * @return the minimal column number according to the table size
     */
    public int getMinimalColumnNumber() {
        Dimension dim = scrollPane.getViewport().getSize();
        return (int) ((double) dim.width / 75) + 1;
    }

    /**
     * Add ten rows
     */
    public void addTenRows() {
        int rowCount = getRowCount();
        ((SwingEditvarTableModel) getModel()).addRows(10);
        ((SwingEditvarTableModel) getModel()).fireTableRowsInserted(rowCount, rowCount + 10);
    }

    /**
     * Add ten columns
     */
    public void addTenCols() {
        ((SwingEditvarTableModel) getModel()).addColumns(10);
        int count = getColumnCount();
        for (int i = count; i < count + 10; i++) {
            addColumn(new TableColumn(i));
        }
    }

    /**
     * Resize the columns to fit contents
     */
    public void sizeColumnsToFit() {
        SwingEditvarTableModel model = (SwingEditvarTableModel) getModel();
        for (int i = 0; i < model.getScilabMatrixColCount(); i++) {
            TableColumn col = getColumnModel().getColumn(i);
            int width = 75;
            for (int j = 0; j < model.getScilabMatrixRowCount(); j++) {
                Component c = getCellRenderer(j, i).getTableCellRendererComponent(this, getValueAt(j, i), false, false, j, i);
                if (c instanceof JLabel) {
                    JLabel label = (JLabel) c;
                    String text = label.getText();
                    if (text != null && !text.isEmpty()) {
                        int trueWidth = SwingUtilities.computeStringWidth(label.getFontMetrics(label.getFont()), label.getText());
                        width = Math.max(width, trueWidth);
                    }
                }
            }
            if (width != 75) {
                col.setPreferredWidth(width + 20);
            } else {
                col.setPreferredWidth(75);
            }
        }
        invalidate();
        doLayout();
        repaint();
    }

    /**
     * Set the row where a jpopupmenu has been shown
     * @param r the row index
     */
    public void setPopupRow(int r) {
        popupRow = r;
    }

    /**
     * Get the row index where a jpopupmenu has been shown
     * @return the row index
     */
    public int getPopupRow() {
        return popupRow;
    }

    /**
     * Set the column where a jpopupmenu has been shown
     * @param r the column index
     */
    public void setPopupColumn(int c) {
        popupCol = c;
    }

    /**
     * Get the column index where a jpopupmenu has been shown
     * @return the column index
     */
    public int getPopupColumn() {
        return popupCol;
    }

    /**
     * Computes the symmetric difference between the sorted arrays a and b
     */
    private Vector<Integer> getDifference(int[] a, int[] b) {
        Vector<Integer> v = new Vector<Integer>(a.length + b.length);
        for (int i : a) {
            if (Arrays.binarySearch(b, i) < 0) {
                v.add(i);
            }
        }
        for (int i : b) {
            if (Arrays.binarySearch(a, i) < 0) {
                v.add(i);
            }
        }

        return v;
    }
}
