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

import java.awt.Color;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

public class Pushbutton extends ScilabWidget {

    public Pushbutton(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        return new SwingScilabPushButton();
    }

    public void initialize() {
        super.initialize();
        try {
            setBackgroundColor(new Color(0.6f, 0.6f, 0.6f));
            setRelief(ScilabRelief.RAISED);
        } catch (UIWidgetException e) { }
    }

    public void setHorizontalAlignment(String halign) {
        super.setHorizontalAlignment(halign);
        SwingScilabPushButton button = (SwingScilabPushButton) getModifiableComponent();
        button.setHorizontalAlignment(halign);
    }

    public void setString(String[][] strs) {
        SwingScilabPushButton button = (SwingScilabPushButton) getModifiableComponent();
        String s = getSingleString(strs);
        if (s != null) {
            button.setText(s);
            super.setString(strs);
        }
    }

    public void setCallback(String callback) {
        SwingScilabPushButton button = (SwingScilabPushButton) getModifiableComponent();
        if (commonCallBack != null) {
            button.removeActionListener(commonCallBack);
        }

        super.setCallback(callback);

        if (commonCallBack != null) {
            button.addActionListener(commonCallBack);
        }
    }

    public String getStyle() {
        return "pushbutton";
    }
}
