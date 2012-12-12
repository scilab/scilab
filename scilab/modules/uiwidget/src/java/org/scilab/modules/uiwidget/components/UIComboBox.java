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

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;

public class UIComboBox extends UIComponent {

    private JComboBox combo;
    private ActionListener listener;
    private boolean onchangeEnable = true;
    private String action;
    private Vector<Object> vector;
    private ListCellRenderer defaultRenderer;
    private MyComboBoxModel model;

    public UIComboBox(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        combo = new JComboBox();
        vector = new Vector<Object>();
        defaultRenderer = combo.getRenderer();
        model = new MyComboBoxModel(vector);
        combo.setModel(model);
        combo.setRenderer(new DefaultListCellRenderer() {

            public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
                JLabel label = (JLabel) defaultRenderer.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
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
        model.fireContentsChanged(model, 0, vector.size() - 1);
        resetIndex();
    }

    public void add(Object obj) {
        vector.add(obj);
        if (obj instanceof UIListElement.ListElement) {
            ((UIListElement.ListElement) obj).setParent(combo);
        }
    }

    public void removeListener() {
        if (listener != null) {
            combo.removeActionListener(listener);
            listener = null;
        }
    }

    public void remove() {
        defaultRenderer = null;
        removeListener();
        super.remove();
    }

    public void setString(String item) {
        setSelectedItem(item);
    }

    public String getString() {
        return getSelectedItem();
    }

    public void setSelectedItem(String item) {
        if (item != null) {
            ComboBoxModel model = combo.getModel();
            for (int i = 0; i < model.getSize(); i++) {
                Object o = model.getElementAt(i);
                if (o != null && o.toString().equals(item)) {
                    combo.setSelectedItem(o);
                    break;
                }
            }
        }
    }

    public void setItems(String[] items) {
        if (items != null) {
            vector.clear();
            for (String item : items) {
                add(new UIListElement.ListElement(item, null));
            }
            model.fireContentsChanged(model, 0, vector.size() - 1);
            resetIndex();
        }
    }

    private void resetIndex() {
        boolean old = onchangeEnable;
        onchangeEnable = false;
        setSelectedIndex(0);
        onchangeEnable = old;
    }

    public String getSelectedItem() {
        return combo.getSelectedItem().toString();
    }

    public void setSelectedIndex(int index) {
        try {
            combo.setSelectedIndex(index);
        } catch (IllegalArgumentException e) { }
    }

    public int getSelectedIndex() {
        return combo.getSelectedIndex();
    }

    public int getValue() {
        return getSelectedIndex();
    }

    public void setValue(int index) {
        setSelectedIndex(index);
    }

    public String getOnchange() {
        return action;
    }

    public void setOnchange(String action) {
        if (this.action == null) {
            removeListener();
            listener = new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    if (onchangeEnable && combo.getSelectedItem() != null) {
                        UIWidgetTools.execAction(UIComboBox.this, UIComboBox.this.action, "\"" + combo.getSelectedItem().toString().replaceAll("\"", "\"\"").replaceAll("\'", "\'\'") + "\"");
                    }
                }
            };
            combo.addActionListener(listener);
        }
        this.action = action;
    }

    public boolean getOnchangeEnable() {
        return onchangeEnable;
    }

    public void setOnchangeEnable(boolean b) {
        onchangeEnable = b;
    }

    private static class MyComboBoxModel extends DefaultComboBoxModel {

        public MyComboBoxModel(Vector v) {
            super(v);
        }

        public void fireContentsChanged(Object o, int i, int f) {
            super.fireContentsChanged(o, i, f);
        }
    }
}
