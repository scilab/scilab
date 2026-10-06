/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2024 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.completion;

import com.artenum.rosetta.interfaces.core.CompletionItem;
import java.awt.Color;
import java.awt.Component;
import javax.swing.JList;

import com.artenum.rosetta.ui.CompletionItemListCellRenderer;

public class SciCompletionItemListCellRenderer extends CompletionItemListCellRenderer {
    private static final long serialVersionUID = 1L;

    @Override
    public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
        this.setText(((CompletionItem)value).getMethodProfile());
        this.setBackground(isSelected ? new Color(0, 120, 214) : Color.white);
        this.setForeground(isSelected ? Color.white : Color.black);
        return this;
     }

}
