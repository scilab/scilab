/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

package org.scilab.modules.ui_data.rowheader;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;

import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.border.AbstractBorder;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.DefaultTableModel;

import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.variableeditor.TableVariableEditor;
import org.scilab.modules.ui_data.variableeditor.actions.InsertRowAction;
import org.scilab.modules.ui_data.variableeditor.actions.RemoveRowAction;

/**
 * Class for the RowHeader
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class RowHeader extends JPanel implements TableModelListener {

    private static final Insets INSETS = new Insets(1, 0, 0, 0);

    private JTable rowTable;
    private JTable table;
    private DefaultTableModel model;
    private int clickedRow;

    /**
     * Constructor
     * @param table the table
     */
    public RowHeader(final JTable table) {
        super(new BorderLayout());
        this.table = table;
        table.getModel().addTableModelListener(this);
        model = new DefaultTableModel(0, 1) {
            public boolean isCellEditable(int iRowIndex, int iColumnIndex) {
                return false;
            }
        };
        rowTable = new JTable(model);
        setBorder(new AbstractBorder() {
            public Insets getBorderInsets(Component c) {
                return INSETS;
            }

            public Insets getBorderInsets(Component c, Insets insets) {
                return INSETS;
            }

            public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
                g.setColor(table.getGridColor().darker());
                g.drawLine(x, y, x + width, y);
            }
        });
        rowTable.setDoubleBuffered(true);
        rowTable.setDragEnabled(false);
        rowTable.setGridColor(table.getGridColor().darker());
        rowTable.setShowVerticalLines(true);
        rowTable.setModel(model);
        rowTable.setRowHeight(table.getRowHeight());
        rowTable.setRowMargin(table.getRowMargin());
        rowTable.setIntercellSpacing(table.getIntercellSpacing());
        rowTable.setDefaultEditor(Object.class, null);
        rowTable.setCellEditor(null);
        rowTable.setDefaultRenderer(Object.class, new HeaderRenderer(table, true));
        rowTable.setComponentPopupMenu(createPopupMenu());
        rowTable.doLayout();
        add(rowTable);
        doLayout();
        rowTable.addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                if (SwingUtilities.isLeftMouseButton(e)) {
                    int row  = rowTable.rowAtPoint(e.getPoint());
                    ListSelectionModel rsm = table.getSelectionModel();
                    int colC = ((SwingEditvarTableModel) ((TableVariableEditor) table).getModel()).getScilabMatrixColCount();
                    if (colC == 0) {
                        colC = 1;
                    }

                    if (e.isShiftDown()) {
                        rsm.setSelectionInterval(row, clickedRow);
                        table.setColumnSelectionInterval(0, colC - 1);
                    } else {
                        if ((e.getModifiers() & Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()) != 0) {
                            if (table.isRowSelected(row)) {
                                rsm.removeSelectionInterval(row, row);
                            } else {
                                rsm.addSelectionInterval(row, row);
                                table.setColumnSelectionInterval(0, colC - 1);
                            }
                        } else {
                            rsm.setSelectionInterval(row, row);
                            table.setColumnSelectionInterval(0, colC - 1);
                        }
                        clickedRow = row;
                    }
                    table.requestFocus();
                }
            }
        });

        rowTable.addMouseMotionListener(new MouseMotionAdapter() {
            public void mouseDragged(MouseEvent e) {
                if (SwingUtilities.isLeftMouseButton(e)) {
                    Point p = e.getPoint();
                    int row;
                    if (p.y >= rowTable.getCellRect(rowTable.getRowCount() - 1, 0, false).y) {
                        row = rowTable.getRowCount() - 1;
                    } else {
                        row = rowTable.rowAtPoint(p);
                    }

                    ListSelectionModel rsm = table.getSelectionModel();
                    int lead = rsm.getLeadSelectionIndex();
                    if (lead != row) {
                        if (row == rowTable.getRowCount() - 1) {
                            ((TableVariableEditor) table).addTenRows();
                        }
                        table.scrollRectToVisible(table.getCellRect(row, 0, false));
                        if (e.isShiftDown()) {
                            rsm.setSelectionInterval(clickedRow, row);
                        } else {
                            if ((e.getModifiers() & Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()) != 0) {
                                if (rsm.isSelectedIndex(row)) {
                                    rsm.removeSelectionInterval(lead, row);
                                    rsm.addSelectionInterval(clickedRow, row);
                                } else {
                                    rsm.addSelectionInterval(clickedRow, row);
                                }
                            } else {
                                rsm.setSelectionInterval(clickedRow, row);
                            }
                        }

                        int colC = ((SwingEditvarTableModel) ((TableVariableEditor) table).getModel()).getScilabMatrixColCount();
                        if (colC == 0) {
                            colC = 1;
                        }
                        table.setColumnSelectionInterval(0, colC - 1);

                        table.requestFocus();
                    }
                }
            }
        });
    }

    /**
     * @return a popup menu for the table
     */
    public JPopupMenu createPopupMenu() {
        JPopupMenu popup = new JPopupMenu() {
            public void show(Component invoker, int x, int y) {
                ((TableVariableEditor) table).setPopupRow(rowTable.rowAtPoint(new Point(x, y)));
                super.show(invoker, x, y);
            }
        };
        popup.setBorderPainted(true);
        popup.add(InsertRowAction.createMenuItem(table, UiDataMessages.INSERTR));
        popup.add(RemoveRowAction.createMenuItem(table, UiDataMessages.REMOVER));

        return popup;
    }

    /**
     * {@inheritDoc}
     */
    public void tableChanged(TableModelEvent e) {
        DefaultTableModel dtm = (DefaultTableModel) e.getSource();
        int N = dtm.getRowCount() - model.getRowCount();
        for (int i = 0; i < N; i++) {
            model.addRow(new Object[1]);
        }
    }

    /**
     * @return the JTable used as row header
     */
    public JTable getTable() {
        return rowTable;
    }
}
