/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.components;

import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

public class UIInfoBar extends UIComponent {

    protected TextBox info;

    public UIInfoBar(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        info = ScilabTextBox.createTextBox();

        return info.getAsSimpleTextBox();
    }

    public void setString(String[][] strs) {
        SwingScilabTextBox info = (SwingScilabTextBox) getModifiableComponent();
        String s = ScilabWidget.getSingleString(strs);
        if (s != null) {
            info.setText(s);
        }
    }

    public String[][] getString() {
        SwingScilabTextBox info = (SwingScilabTextBox) getModifiableComponent();
        String text = info.getText();
        if (text != null) {
            return new String[][] {new String[]{text}};
        } else {
            return null;
        }
    }
}
