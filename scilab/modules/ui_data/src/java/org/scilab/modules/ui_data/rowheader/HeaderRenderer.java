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
import java.awt.Insets;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.border.AbstractBorder;
import javax.swing.table.JTableHeader;
import javax.swing.table.TableCellRenderer;

/**
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class HeaderRenderer extends JLabel implements TableCellRenderer {

    private static final Insets INSETS = new Insets(1, 1, 1, 0);

    private int index;
    private JTable table;
    private boolean rowModel;

    private Font selectedFont;
    private Font unselectedFont;
    private Color selectedFgColor;
    private Color unselectedFgColor;
    private Color selectedBgColor;
    private Color unselectedBgColor;

    /**
     * Constructor
     */
    public HeaderRenderer(JTable table, boolean rowModel) {
        this.table = table;
        this.rowModel = rowModel;
        JTableHeader header = table.getTableHeader();
        this.selectedFont = header.getFont().deriveFont(Font.BOLD);
        this.unselectedFont = header.getFont();
        this.selectedFgColor = UIManager.getColor("textHighlightText");
        this.unselectedFgColor = header.getForeground();
        this.unselectedBgColor = header.getBackground().darker();

        Color c = table.getSelectionBackground();
        float[] hsb = Color.RGBtoHSB(c.getRed(), c.getGreen(), c.getBlue(), null);
        this.selectedBgColor = Color.getHSBColor(hsb[0], hsb[1] * 2f, hsb[2]);

        setOpaque(true);
        setBorder(BorderFactory.createEmptyBorder());
        setHorizontalAlignment(CENTER);
        setForeground(unselectedFgColor);
        setBackground(unselectedBgColor);
        setFont(unselectedFont);
        if (!rowModel) {
            setBorder(new AbstractBorder() {
                public Insets getBorderInsets(Component c) {
                    return INSETS;
                }

                public Insets getBorderInsets(Component c, Insets insets) {
                    return INSETS;
                }

                public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
                    g.setColor(HeaderRenderer.this.table.getGridColor().darker());
                    g.drawLine(x, y, x + width, y);
                    g.drawLine(x, y, x, y + height);
                }
            });
        }
    }

    public void paintComponent(Graphics g) {
        ListSelectionModel lsm;
        if (rowModel) {
            lsm = table.getSelectionModel();
        } else {
            lsm = table.getColumnModel().getSelectionModel();
        }

        if (lsm.isSelectedIndex(index)) {
            setForeground(selectedFgColor);
            setBackground(selectedBgColor);
            setFont(selectedFont);
        } else {
            setForeground(unselectedFgColor);
            setBackground(unselectedBgColor);
            setFont(unselectedFont);
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
