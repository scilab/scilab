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

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.Graphics;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.table.JTableHeader;
import javax.swing.table.TableCellRenderer;

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
