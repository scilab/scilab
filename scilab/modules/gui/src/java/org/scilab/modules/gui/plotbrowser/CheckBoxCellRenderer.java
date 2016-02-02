/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
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
package org.scilab.modules.gui.plotbrowser;

import java.awt.Color;
import java.awt.Component;
import javax.swing.JCheckBox;
import javax.swing.JList;
import javax.swing.ListCellRenderer;
import javax.swing.UIManager;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;

/**
 * Allows the list items are displayed as JCheckBoxes.
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class CheckBoxCellRenderer implements ListCellRenderer {
    Border noFocusBorder = new EmptyBorder(1, 1, 1, 1);

    public Component getListCellRendererComponent(
        JList list,
        Object value,
        int index,
        boolean isSelected,
        boolean cellHasFocus) {
        JCheckBox checkbox = (JCheckBox) value;
        checkbox.setBackground(isSelected
                               ? list.getSelectionBackground() : list.getBackground());
        checkbox.setForeground(isSelected
                               ? Color.ORANGE : list.getForeground());

        checkbox.setEnabled(list.isEnabled());
        checkbox.setFont(list.getFont());
        checkbox.setFocusPainted(false);

        checkbox.setBorderPainted(true);
        checkbox.setBorder(isSelected ? UIManager.getBorder(
                               "List.focusCellHighlightBorder") : noFocusBorder);

        return checkbox;
    }
}
