/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.rowheader;

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.table.JTableHeader;
import javax.swing.table.TableCellRenderer;

import org.scilab.modules.ui_data.rowheader.HeaderRenderer;
import org.scilab.modules.ui_data.rowheader.RowHeader;

/**
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class RowHeaderRenderer extends JLabel implements TableCellRenderer {

    private int index;
    private JTable table;
    private boolean rowModel;

    /**
     * Constructor
     */
    public RowHeaderRenderer(JTable table, boolean rowModel) {
        this.table = table;
        this.rowModel = rowModel;
        JTableHeader header = table.getTableHeader();
        setOpaque(true);
        setBorder(BorderFactory.createEmptyBorder());
        setHorizontalAlignment(CENTER);
        setForeground(header.getForeground());
        setBackground(header.getBackground());
        setFont(header.getFont());
        if (!rowModel) {
            setBorder(BorderFactory.createLineBorder(table.getGridColor().darker(), 1));
        }
    }

    public void paintComponent(Graphics g) {
        JTableHeader header = table.getTableHeader();
        ListSelectionModel lsm;
        if (rowModel) {
            lsm = table.getSelectionModel();
        } else {
            lsm = table.getColumnModel().getSelectionModel();
        }

        if (lsm.isSelectedIndex(index)) {
            Color c = UIManager.getColor("Table.selectionBackground");
            float[] hsb = Color.RGBtoHSB(c.getRed(), c.getGreen(), c.getBlue(), null);
            setBackground(Color.getHSBColor(hsb[0], 1, hsb[2]));
            setFont(header.getFont().deriveFont(Font.BOLD));
            setForeground(UIManager.getColor("textHighlightText"));
        } else {
            setForeground(header.getForeground());
            setBackground(header.getBackground());
            setFont(header.getFont());
        }
        super.paintComponent(g);
    }

    /**
     * {@inheritDoc}
     */
    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        if (rowModel) {
            this.index = row;
        } else {
            this.index = column;
        }

        setText(Integer.toString(index + 1));

        return this;
    }
}
