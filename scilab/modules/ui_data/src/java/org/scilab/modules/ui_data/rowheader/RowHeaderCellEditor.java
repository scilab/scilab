/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.ui_data.rowheader;

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;

import javax.swing.AbstractCellEditor;
import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.UIManager;
import javax.swing.table.JTableHeader;
import javax.swing.table.TableCellEditor;

/**
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class RowHeaderCellEditor extends AbstractCellEditor implements TableCellEditor {

    private JLabel label;
    private int row;

    /**
     * Constructor
     * @param table the table where to put the rowHeader
     */
    public RowHeaderCellEditor(final JTable table) {
        label = new JLabel();
        JTableHeader header = table.getTableHeader();
        label.setOpaque(true);
        label.setBorder(BorderFactory.createEmptyBorder());
        label.setHorizontalAlignment(JLabel.CENTER);
        label.setForeground(UIManager.getColor("textHighlightText"));
        label.setFont(header.getFont().deriveFont(Font.BOLD));

        Color selected = UIManager.getColor("Table.selectionBackground");
        float[] hsb = Color.RGBtoHSB(selected.getRed(), selected.getGreen(), selected.getBlue(), null);
        label.setBackground(Color.getHSBColor(hsb[0], 1, hsb[2]));
    }

    /**
     * {@inheritDoc}
     */
    public Object getCellEditorValue() {
        return row;
    }

    /**
     * {@inheritDoc}
     */
    public Component getTableCellEditorComponent(JTable table, Object value, boolean isSelected, int row, int col) {
        this.row = row;
        label.setText(Integer.toString(row + 1));

        return label;
    }
}
