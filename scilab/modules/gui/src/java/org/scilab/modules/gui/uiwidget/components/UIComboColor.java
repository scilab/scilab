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

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.ComboBoxModel;
import javax.swing.DefaultComboBoxModel;
import javax.swing.DefaultListCellRenderer;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.ListCellRenderer;

import org.scilab.modules.gui.uiwidget.StringConverters;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.gui.uiwidget.UIWidgetTools;

/**
 * Wrapper for a JComboBox used to select a color
 */
public class UIComboColor extends UIComboBox {

    /**
     * {@inheritDoc}
     */
    public UIComboColor(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        Object o = super.newInstance();
        combo.setRenderer(new DefaultListCellRenderer() {

            public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
                JLabel label = (JLabel) defaultRenderer.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
                if (value instanceof UIColorElement.ColorElement) {
                    UIColorElement.ColorElement c = (UIColorElement.ColorElement) value;
                    label.setIcon(c.getIcon());

                    Font f = c.getFont();
                    if (f != null) {
                        label.setFont(f);
                    }
                }

                return label;
            }
        });

        return o;
    }

    /**
     * {@inheritDoc}
     */
    public void add(Object obj) {
        vector.add(obj);
        if (obj instanceof UIColorElement.ColorElement) {
            ((UIColorElement.ColorElement) obj).setParent(combo);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void setItems(String[][] items) {
        if (items != null) {
            vector.clear();
            vector.ensureCapacity(items.length);
            for (String[] item : items) {
                if (item != null && item.length == 2) {
                    add(new UIColorElement.ColorElement(item[0], UITools.getColor(item[1]), Color.BLACK));
                }
            }
            model.fireContentsChanged(model, 0, vector.size() - 1);
            resetIndex();
        }
    }

    /**
     * {@inheritDoc}
     */
    public Object[] getItems() {
        if (!vector.isEmpty()) {
            String[][] arr = new String[2][vector.size()];
            for (int i = 0; i < vector.size(); i++) {
                String[] rep = ((UIColorElement.ColorElement) vector.get(i)).getRep();
                arr[0][i] = rep[0];
                arr[1][i] = rep[1];
            }

            return arr;
        }

        return null;
    }
}
