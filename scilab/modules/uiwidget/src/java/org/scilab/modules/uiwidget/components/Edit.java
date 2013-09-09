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

package org.scilab.modules.uiwidget.components;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.JTextField;

import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

public class Edit extends ScilabWidget {

    public Edit(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        return new JTextField();
    }

    public void initialize() {
        super.initialize();
        try {
            setRelief(ScilabRelief.SUNKEN);
        } catch (UIWidgetException e) { }
    }

    public void setHorizontalAlignment(String halign) {
        super.setHorizontalAlignment(halign);
        JTextField edit = (JTextField) getModifiableComponent();
        edit.setHorizontalAlignment(ScilabAlignment.toSwingAlignment(halign));
    }

    public void setString(String[][] strs) {
        JTextField edit = (JTextField) getModifiableComponent();
        String s = getSingleString(strs);
        if (s != null) {
            edit.setText(s);
        }
    }

    public String[][] getString() {
        JTextField edit = (JTextField) getModifiableComponent();
        if (edit.getText() != null) {
            return new String[][] {new String[]{edit.getText()}};
        } else {
            return null;
        }
    }

    public void setCallback(String callback) {
        JTextField edit = (JTextField) getModifiableComponent();
        if (commonCallBack != null) {
            edit.setAction(null);
        }

        super.setCallback(callback);

        if (commonCallBack != null) {
            edit.setAction(new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    commonCallBack.actionPerformed(e);
                }
            });
        }
    }
}
