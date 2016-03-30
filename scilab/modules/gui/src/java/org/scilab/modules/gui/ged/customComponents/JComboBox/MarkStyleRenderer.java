/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
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

package org.scilab.modules.gui.ged.customComponents.JComboBox;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.ListCellRenderer;
import org.scilab.modules.gui.ged.graphic_objects.contouredObject.MarkStyle;

/**
* Mark Style Renderer - JComboBox
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class MarkStyleRenderer extends JPanel implements ListCellRenderer {
    private MarkStyle value;
    private static Color background = Color.WHITE;
    private static Color foreground = Color.BLACK;

    @Override
    public Component getListCellRendererComponent(JList list,
            Object value,
            int index,
            boolean isSelected,
            boolean cellHasFocus) {
        if (isSelected) {
            setBackground(list.getSelectionBackground());
            setForeground(list.getSelectionForeground());
        } else {
            setBackground(list.getBackground());
            setForeground(list.getForeground());
        }

        if (value instanceof MarkStyle) {
            setMarkType((MarkStyle) value);
        } else {
            setMarkType(null);
        }

        return this;
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;
        if (value != null) {
            value.setGraphics2D(g2d);
            value.setBackground(background);
            value.setForeground(foreground);
            g2d = value.getMarkStyle();
        }
    }

    private void setMarkType(MarkStyle value) {
        this.value = value;
    }

    public void setMarkBackground(Color c) {
        background = c;
    }

    public void setMarkForeground(Color c) {
        foreground = c;
    }

    @Override
    public Dimension getPreferredSize() {
        return new Dimension(5, 20);
    }
}
