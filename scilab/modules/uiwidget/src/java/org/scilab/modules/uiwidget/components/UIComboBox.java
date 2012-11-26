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
import java.awt.Component;
import java.awt.Font;
import java.util.Map;
import java.util.Vector;

import javax.swing.DefaultComboBoxModel;
import javax.swing.DefaultListCellRenderer;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JList;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

public class UIComboBox extends UIComponent {

    private JComboBox combo;
    private Vector<Object> vector;

    public UIComboBox(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        combo = new JComboBox();
        vector = new Vector<Object>();

        combo.setRenderer(new DefaultListCellRenderer() {

            public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
                JLabel label = (JLabel) super.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
                if (value instanceof UIListElement.ListElement) {
                    UIListElement.ListElement l = (UIListElement.ListElement) value;
                    label.setIcon(l.getIcon());
                    Font f = l.getFont();
                    if (f != null) {
                        label.setFont(f);
                    }
                    Color c = l.getBackground();
                    if (c != null && !isSelected) {
                        label.setBackground(l.getBackground());
                    }
                    c = l.getForeground();
                    if (c != null) {
                        label.setForeground(l.getForeground());
                    }
                }

                return label;
            }
        });

        return combo;
    }

    public void finish() {
        combo.setModel(new DefaultComboBoxModel(vector));
        vector = null;
    }

    public void add(Object obj) {
        if (vector == null) {
            combo.addItem(obj);
        } else {
            vector.add(obj);
        }
        if (obj instanceof UIListElement.ListElement) {
            ((UIListElement.ListElement) obj).setParent(combo);
        }
    }
}
