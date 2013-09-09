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

import javax.swing.JCheckBox;

import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

public class Checkbox extends ScilabWidget {

    public Checkbox(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        JCheckBox checkbox = new JCheckBox();
        checkbox.setContentAreaFilled(false);
        checkbox.setOpaque(true);

        return checkbox;
    }

    public void setHorizontalAlignment(String halign) {
        super.setHorizontalAlignment(halign);
        JCheckBox checkbox = (JCheckBox) getModifiableComponent();
        checkbox.setHorizontalAlignment(ScilabAlignment.toSwingAlignment(halign));
    }

    public void setString(String[][] strs) {
        JCheckBox checkbox = (JCheckBox) getModifiableComponent();
        String s = getSingleString(strs);
        if (s != null) {
            checkbox.setText(s);
            super.setString(strs);
        }
    }

    public double[] getValue() {
        if (((JCheckBox) getModifiableComponent()).isSelected()) {
            return new double[] {getMin()};
        } else {
            return new double[] {getMax()};
        }
    }

    public void setCallback(String callback) {
        JCheckBox checkbox = (JCheckBox) getModifiableComponent();
        if (commonCallBack != null) {
            checkbox.removeActionListener(commonCallBack);
        }

        super.setCallback(callback);

        if (commonCallBack != null) {
            checkbox.addActionListener(commonCallBack);
        }
    }
}
