/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.rowheader;

import java.awt.Component;

import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.ListCellRenderer;

/**
 * @author Bruno JOFRET
 */
public class RowHeaderRenderer extends JButton implements ListCellRenderer {

    /**
     * Constructor
     */
    public RowHeaderRenderer() {
        super();
    }

    /**
     * {@inheritDoc}
     */
    public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
        if (value == null) {
            setText("");
        } else {
            setText(value.toString());
        }
        return this;
    }
}
