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

import javax.swing.JRadioButton;

import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

public class Radiobutton extends ScilabWidget {

    public Radiobutton(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        JRadioButton radio = new JRadioButton();
        radio.setContentAreaFilled(false);
        radio.setOpaque(true);

        return radio;
    }

    public void setHorizontalAlignment(String halign) {
        super.setHorizontalAlignment(halign);
        JRadioButton radio = (JRadioButton) getModifiableComponent();
        radio.setHorizontalAlignment(ScilabAlignment.toSwingAlignment(halign));
    }

    public void setString(String[][] strs) {
        JRadioButton radio = (JRadioButton) getModifiableComponent();
        String s = getSingleString(strs);
        if (s != null) {
            radio.setText(s);
            super.setString(strs);
        }
    }

    public double[] getValue() {
        if (((JRadioButton) getModifiableComponent()).isSelected()) {
            return new double[] {getMin()};
        } else {
            return new double[] {getMax()};
        }
    }

    public void setCallback(String callback) {
        JRadioButton radio = (JRadioButton) getModifiableComponent();
        if (commonCallBack != null) {
            radio.removeActionListener(commonCallBack);
        }

        super.setCallback(callback);

        if (commonCallBack != null) {
            radio.addActionListener(commonCallBack);
        }
    }
}
