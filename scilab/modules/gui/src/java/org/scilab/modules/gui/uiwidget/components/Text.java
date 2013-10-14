/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.components;

import javax.swing.SwingUtilities;

import org.scilab.modules.gui.bridge.label.SwingScilabLabel;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

public class Text extends ScilabWidget {

    public Text(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        SwingScilabLabel label = new SwingScilabLabel();

        return label;
    }

    public void setHorizontalAlignment(String halign) {
        super.setHorizontalAlignment(halign);
        SwingScilabLabel label = (SwingScilabLabel) getModifiableComponent();
        label.setHorizontalAlignment(halign);
    }

    public void setVerticalAlignment(String valign) {
        super.setVerticalAlignment(valign);
        SwingScilabLabel label = (SwingScilabLabel) getModifiableComponent();
        label.setVerticalAlignment(valign);
    }

    public void setString(String[][] strs) {
        final SwingScilabLabel label = (SwingScilabLabel) getModifiableComponent();
        String[] s = getRowString(strs);
        if (s != null) {
            StringBuffer buffer = new StringBuffer();
            for (int i = 0; i < s.length - 1; i++) {
                buffer.append(s[i]).append("\n");
            }
            buffer.append(s[s.length - 1]);
            label.setText(buffer.toString());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    label.getVerticalScrollBar().setValue(0);
                    label.getVerticalScrollBar().setUnitIncrement(label.getFont().getSize());
                }
            });
            super.setString(strs);
        }
    }
}
