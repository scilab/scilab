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
