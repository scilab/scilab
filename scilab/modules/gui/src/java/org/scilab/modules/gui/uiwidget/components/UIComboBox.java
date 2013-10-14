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

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.gui.uiwidget.UIWidgetTools;
import org.scilab.modules.gui.uiwidget.callback.UICallback;

/**
 * JComboBox wrapper
 */
public class UIComboBox extends UIComponent {

    protected JComboBox combo;
    protected ActionListener listener;
    protected boolean onchangeEnable = true;
    protected UICallback action;
    protected Vector<Object> vector;
    protected ListCellRenderer defaultRenderer;
    protected MyComboBoxModel model;

    /**
     * {@inheritDoc}
     */
    public UIComboBox(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
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

    /**
     * {@inheritDoc}
     */
    public void finish() {
        model.fireContentsChanged(model, 0, vector.size() - 1);
        resetIndex();
    }

    /**
     * Add an object
     * @param obj the object to add
     */
    public void add(Object obj) {
        vector.add(obj);
        if (obj instanceof UIListElement.ListElement) {
            ((UIListElement.ListElement) obj).setParent(combo);
        }
    }

    /**
     * Remove a listener
     */
    protected void removeListener() {
        if (listener != null) {
            combo.removeActionListener(listener);
            listener = null;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void remove() {
        defaultRenderer = null;
        removeListener();
        super.remove();
    }

    /**
     * Set the selected item
     * @param item the item to select
     */
    public void setString(String item) {
        setSelectedItem(item);
    }

    /**
     * Get the selected item
     * @return the selected item
     */
    public String getString() {
        return getSelectedItem();
    }

    /**
     * Set the selected item
     * @param item the item to select
     */
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

    /**
     * Get the selected item
     * @return the selected item
     */
    public String getSelectedItem() {
        if (combo.getSelectedIndex() == -1) {
            return null;
        }

        return combo.getSelectedItem().toString();
    }

    /**
     * Set the combobox items
     * @param items the items to set
     */
    public void setItems(String[] items) {
        if (items != null) {
            vector.clear();
            vector.ensureCapacity(items.length);
            for (String item : items) {
                add(new UIListElement.ListElement(item, null));
            }
            model.fireContentsChanged(model, 0, vector.size() - 1);
            resetIndex();
        }
    }

    /**
     * Get the combobox items
     * @return the combobox items
     */
    public Object[] getItems() {
        if (!vector.isEmpty()) {
            String[] arr = new String[vector.size()];
            for (int i = 0; i < arr.length; i++) {
                arr[i] = vector.get(i).toString();
            }

            return arr;
        }

        return null;
    }

    /**
     * Reset the combobox index
     */
    protected void resetIndex() {
        boolean old = onchangeEnable;
        onchangeEnable = false;
        setSelectedIndex(0);
        onchangeEnable = old;
    }

    /**
     * Set the selected index
     * @param index to select
     */
    public void setSelectedIndex(int index) {
        try {
            combo.setSelectedIndex(index);
        } catch (IllegalArgumentException e) { }
    }

    /**
     * Get the selected index
     * @return selected index
     */
    public int getSelectedIndex() {
        return combo.getSelectedIndex();
    }

    /**
     * Alias for getSelectedIndex
     */
    public int getValue() {
        return getSelectedIndex();
    }

    /**
     * Alias for setSelectedIndex
     */
    public void setValue(int index) {
        setSelectedIndex(index);
    }

    /**
     * Get the onchange action
     * @return the action
     */
    public UICallback getOnchange() {
        return action;
    }

    /**
     * Set the onchange action
     * @param the action
     */
    public void setOnchange(String action) {
        if (this.action == null) {
            removeListener();
            listener = new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    if (onchangeEnable && combo.getSelectedItem() != null) {
                        UIWidgetTools.execAction(UIComboBox.this.action, "\"" + combo.getSelectedItem().toString().replaceAll("\"", "\"\"").replaceAll("\'", "\'\'") + "\"");
                    }
                }
            };
            combo.addActionListener(listener);
        }
        this.action = UICallback.newInstance(this, action);
    }

    /**
     * Check if the onchange is enabled
     * @return true if enabled
     */
    public boolean getOnchangeEnable() {
        return onchangeEnable;
    }

    /**
     * Set if the onchange is enabled
     * @param b true if enabled
     */
    public void setOnchangeEnable(boolean b) {
        onchangeEnable = b;
    }

    /**
     * Inner class to have a public fire
     */
    protected static class MyComboBoxModel extends DefaultComboBoxModel {

        /**
         * {@inheritDoc}
         */
        public MyComboBoxModel(Vector v) {
            super(v);
        }

        /**
         * {@inheritDoc}
         */
        public void fireContentsChanged(Object o, int i, int f) {
            super.fireContentsChanged(o, i, f);
        }
    }
}
