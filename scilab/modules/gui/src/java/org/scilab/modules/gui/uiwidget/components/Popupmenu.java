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

import java.awt.event.ActionListener;

import javax.swing.JComboBox;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

public class Popupmenu extends ScilabWidget {

    private ActionListener listener;

    public Popupmenu(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        JComboBox combo = new JComboBox();
        combo.putClientProperty("JComboBox.isTableCellEditor", Boolean.TRUE);

        return combo;
    }

    public void setString(String[][] str) {
        String[] s = getRowString(str);

        if (s == null) {
            return;
        }

        if (s.length == 1) {
            s = s[0].split("\\|");
        }

        JComboBox combo = (JComboBox) getModifiableComponent();
        String[] prev = getRowString(getString());
        boolean needUpdate = false;
        if (prev.length != s.length) {
            needUpdate = true;
        } else {
            for (int i = 0; i < prev.length; i++) {
                if (!prev[i].equals(s[i])) {
                    needUpdate = true;
                    break;
                }
            }
        }

        if (!needUpdate) {
            return;
        }

        if (commonCallBack != null) {
            combo.removeActionListener(commonCallBack);
        }

        combo.removeAllItems();

        for (int i = 0; i < s.length; i++) {
            combo.addItem(new MyString(s[i]));
        }

        if (commonCallBack != null) {
            combo.addActionListener(commonCallBack);
        }

        super.setString(str);
    }

    public void setValue(double[] value) {
        super.setValue(value);

        JComboBox combo = (JComboBox) getModifiableComponent();
        if (value != null && value.length > 0) {
            int index = (int) (value[0] - 1);
            if (index >= -1 && index < combo.getItemCount()) {
                combo.setSelectedIndex(index);
            }
        } else {
            combo.setSelectedIndex(-1);
        }
    }

    public double[] getValue() {
        JComboBox combo = (JComboBox) getModifiableComponent();
        double index = (double) (combo.getSelectedIndex() + 1);

        if (index == 0) {
            return null;
        }

        return new double[] {index};
    }

    public void setCallback(String callback) {
        JComboBox combo = (JComboBox) getModifiableComponent();
        if (commonCallBack != null) {
            combo.removeActionListener(commonCallBack);
            commonCallBack = null;
        }

        super.setCallback(callback);

        if (commonCallBack != null) {
            combo.addActionListener(commonCallBack);
        }
    }

    private static class MyString {

        private String s;

        private MyString(String s) {
            this.s = s;
        }

        public String toString() {
            return s;
        }
    }
}
