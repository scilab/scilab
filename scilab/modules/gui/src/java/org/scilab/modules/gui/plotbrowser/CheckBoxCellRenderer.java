/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
