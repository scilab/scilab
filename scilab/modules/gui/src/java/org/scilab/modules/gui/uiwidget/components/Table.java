/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.components;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Point;
import java.awt.Rectangle;
import java.util.Vector;

import javax.swing.DefaultListModel;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListCellRenderer;
import javax.swing.UIManager;
import javax.swing.table.DefaultTableColumnModel;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.JTableHeader;

import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

public class Table extends ScilabWidget {

    protected JTable table;
    protected JList rowHeader;
    protected RowHeaderRenderer rowHeaderRenderer;
    protected DefaultTableCellRenderer cellRenderer;
    protected String corner;

    public Table(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        JScrollPane scroll = new JScrollPane();
        cellRenderer = new DefaultTableCellRenderer();
        table = new JTable() {
            public void setFont(Font f) {
                super.setFont(f);
                if (getTableHeader() != null) {
                    getTableHeader().setFont(f);
                }
                if (rowHeaderRenderer != null) {
                    rowHeaderRenderer.setFont(f);
                }
            }
        };
        table.setDefaultRenderer(Object.class, cellRenderer);
        table.setFillsViewportHeight(true);
        if (table.getGridColor().equals(Color.WHITE)) {
            table.setGridColor(new Color(128, 128, 128));
        }
        table.setShowHorizontalLines(true);
        table.setShowVerticalLines(true);
        scroll.getViewport().add(table);
        rowHeader = new JList();
        rowHeader.setFixedCellHeight(table.getRowHeight());
        rowHeaderRenderer = new RowHeaderRenderer(table);
        rowHeader.setCellRenderer(rowHeaderRenderer);
        scroll.setRowHeaderView(rowHeader);
        modifiableComponent = table;

        return scroll;
    }

    public void setHorizontalAlignment(String halign) {
        super.setHorizontalAlignment(halign);
        cellRenderer.setHorizontalAlignment(ScilabAlignment.toSwingAlignment(halign));
        table.repaint();
    }

    public void setString(String[][] str) {
        JScrollPane scroll = (JScrollPane) getComponent();
        if (str == null) {
            return;
        }

        corner = str[0][0];

        if (getMin() != 0) {
            DefaultListModel model = new DefaultListModel();
            if (str.length > 0) {
                model.ensureCapacity(str.length - 1);
                for (int i = 1; i < str.length; i++) {
                    model.addElement(str[i][0]);
                }
            }
            rowHeader.setModel(model);
            if (scroll.getRowHeader().getView() == null) {
                scroll.setRowHeaderView(rowHeader);
            }
        } else {
            scroll.setRowHeaderView(null);
        }

        final int r = getMax() != 0 ? 1 : 0;
        final int c = getMin() != 0 ? 1 : 0;
        Vector<Vector<String>> rows = new Vector<Vector<String>>(str.length - r);
        for (int i = r; i < str.length; i++) {
            Vector<String> v = new Vector<String>(str[i].length - c);
            for (int j = c; j < str[i].length; j++) {
                v.add(str[i][j]);
            }
            rows.add(v);
        }

        DefaultTableModel tableModel;
        if (getMax() != 0) {
            Vector<String> columnsName = new Vector<String>(str[0].length - 1);
            for (int i = c; i < str[0].length; i++) {
                columnsName.add(str[0][i]);
            }
            tableModel = new DefaultTableModel(rows, columnsName);
            table.getTableHeader().setColumnModel(table.getColumnModel());
        } else {
            Vector<String> columnsName = new Vector<String>(str[0].length - c);
            for (int i = c; i < str[0].length; i++) {
                columnsName.add("");
            }
            tableModel = new DefaultTableModel(rows, columnsName);
            table.getTableHeader().setColumnModel(new DefaultTableColumnModel());
        }

        table.setModel(tableModel);
    }

    public void setMin(double min) {
        if ((getMin() != 0 && min == 0) || (getMin() == 0 && min != 0)) {
            String[][] strs = getString();
            super.setMin(min);
            setString(strs);
        } else {
            super.setMin(min);
        }
    }

    public void setMax(double max) {
        if ((getMax() != 0 && max == 0) || (getMax() == 0 && max != 0)) {
            String[][] strs = getString();
            super.setMax(max);
            setString(strs);
        } else {
            super.setMax(max);
        }
    }

    public String[][] getString() {
        final int r = getMax() != 0 ? 1 : 0;
        final int c = getMin() != 0 ? 1 : 0;
        DefaultTableModel tableModel = (DefaultTableModel) table.getModel();
        Vector data = tableModel.getDataVector();
        int nbRow = data.size();
        if (nbRow == 0) {
            return null;
        }
        int nbCol = ((Vector) data.get(0)).size();
        String[][] str = new String[nbRow + r][nbCol + c];
        str[0][0] = corner;

        if (getMin() != 0 && rowHeader.getModel() instanceof DefaultListModel) {
            DefaultListModel model = (DefaultListModel) rowHeader.getModel();
            for (int i = 0; i < model.getSize(); i++) {
                str[i + r][0] = (String) model.get(i);
            }
        }

        if (getMax() != 0) {
            for (int i = 0; i < nbCol; i++) {
                str[0][i + c] = tableModel.getColumnName(i);
            }
        }

        for (int i = 0; i < data.size(); i++) {
            Vector<String> v = (Vector<String>) data.get(i);
            for (int j = 0; j < v.size(); j++) {
                str[i + r][j + c] = v.get(j);
            }
        }

        return str;
    }

    public void setListboxTop(int top) {
        super.setListboxTop(top);
        JScrollPane scroll = (JScrollPane) getComponent();
        if (top > 0 & top != getListboxTop()) {
            Rectangle r = table.getCellRect(top - 1, 0, true);
            if (r != null) {
                r.y = Math.min(r.y, table.getHeight() - scroll.getViewport().getExtentSize().height - 1);
                scroll.getViewport().setViewPosition(new Point(r.x, r.y));
                scroll.doLayout();
            }
        }
    }

    public int getListboxTop() {
        JScrollPane scroll = (JScrollPane) getComponent();
        int listboxtopValue = table.rowAtPoint(scroll.getViewport().getViewPosition()) + 1;

        return listboxtopValue;
    }

    public static class RowHeaderRenderer extends JLabel implements ListCellRenderer {

        public RowHeaderRenderer(JTable table) {
            JTableHeader header = table.getTableHeader();
            setOpaque(true);
            setBorder(UIManager.getBorder("TableHeader.cellBorder"));
            setHorizontalAlignment(CENTER);
            if (header != null) {
                setForeground(header.getForeground());
                setBackground(header.getBackground());
                setFont(header.getFont());
            }
        }

        public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
            setText((value == null) ? "" : value.toString());
            return this;
        }
    }
}
