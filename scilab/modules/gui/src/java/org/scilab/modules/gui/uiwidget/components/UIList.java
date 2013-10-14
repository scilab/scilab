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
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

import javax.swing.DefaultListCellRenderer;
import javax.swing.DefaultListModel;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.ListModel;
import javax.swing.ListSelectionModel;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.gui.uiwidget.UIWidgetTools;
import org.scilab.modules.gui.uiwidget.callback.UICallback;

/**
 * JList wrapper
 */
public class UIList extends UIComponent {

    private JList list;
    private MouseListener listener;
    private boolean onclickEnable = true;
    private MyListModel model;
    private Vector<Object> vector;
    private boolean isFinished;
    private UICallback action;

    /**
     * {@inheritDoc}
     */
    public UIList(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        list = new JList();
        vector = new Vector<Object>();
        model = new MyListModel(vector);
        list.setModel(model);
        list.setCellRenderer(new DefaultListCellRenderer() {

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
                        label.setBackground(c);
                    }
                    c = l.getForeground();
                    if (c != null) {
                        label.setForeground(c);
                    }
                }

                return label;
            }
        });

        setMultiple(false);

        return list;
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
        vector.addElement(obj);
        if (obj instanceof UIListElement.ListElement) {
            ((UIListElement.ListElement) obj).setParent(list);
        }
    }

    /**
     * Remove a listener
     */
    public void removeListener() {
        if (listener != null) {
            list.removeMouseListener(listener);
            listener = null;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void remove() {
        removeListener();
        super.remove();
    }

    /**
     * Set the selected item
     * @param item the item to select
     */
    public void setSelectedItem(String item) {
        if (item != null) {
            ListModel model = list.getModel();
            for (int i = 0; i < model.getSize(); i++) {
                Object o = model.getElementAt(i);
                if (o != null && o.toString().equals(item)) {
                    list.setSelectedValue(o, true);
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
        if (list.getSelectedIndex() != -1) {
            return list.getSelectedValue().toString();
        }

        return null;
    }

    /**
     * Alias for setSelectedItem
     */
    public void setString(String item) {
        setSelectedItem(item);
    }

    /**
     * Alias for getSelectedItem
     */
    public String getString() {
        return getSelectedItem();
    }

    /**
     * Set the selected items
     * @param items the items to select
     */
    public void setStrings(String[] items) {
        if (items == null) {
            list.setSelectedIndices(new int[0]);
        } else {
            ListModel model = list.getModel();
            List<Integer> indices = new ArrayList<Integer>(items.length);
            for (int i = 0; i < items.length; i++) {
                for (int j = 0; j < model.getSize(); j++) {
                    Object o = model.getElementAt(j);
                    if (o != null && o.toString().equals(items[i])) {
                        indices.add(j);
                        break;
                    }
                }
            }
            if (!indices.isEmpty()) {
                int[] ind = new int[indices.size()];
                for (int i = 0; i < ind.length; i++) {
                    ind[i] = indices.get(i);
                }
                list.setSelectedIndices(ind);
            } else {
                list.setSelectedIndices(new int[0]);
            }
        }
    }

    /**
     * Get the selected items
     * @return the selected items
     */
    public String[] getStrings() {
        Object[] objs = list.getSelectedValues();
        if (objs != null && objs.length != 0) {
            String[] strs = new String[objs.length];
            for (int i = 0; i < objs.length; i++) {
                strs[i] = objs[i] == null ? "" : objs[i].toString();
            }

            return strs;
        }

        return null;
    }

    /**
     * Set the selection type
     * @param multiple if true the multiple selection is allowed
     */
    public void setMultiple(boolean multiple) {
        if (multiple) {
            list.getSelectionModel().setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        } else {
            list.getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        }
    }

    /**
     * Get the selection type
     * @return true if the multiple selection is allowed
     */
    public boolean getMultiple() {
        int m = list.getSelectionModel().getSelectionMode();
        return m == ListSelectionModel.MULTIPLE_INTERVAL_SELECTION;
    }

    /**
     * Set the list items
     * @param items the items to set
     */
    public void setItems(String[] items) {
        vector.clear();
        if (items != null) {
            for (String item : items) {
                add(new UIListElement.ListElement(item, null));
            }
            model.fireContentsChanged(model, 0, vector.size() - 1);
        } else {
            model.fireContentsChanged(model, 0, 0);
        }
        list.setSelectedIndices(new int[0]);
    }

    /**
     * Get the list items
     * @return the list items
     */
    public String[] getItems() {
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
        setSelectedIndex(0);
    }

    /**
     * Set the selected index
     * @param index to select
     */
    public void setSelectedIndex(int index) {
        try {
            list.setSelectedIndex(index);
        } catch (IllegalArgumentException e) { }
    }

    /**
     * Get the selected index
     * @return selected index
     */
    public int getSelectedIndex() {
        return list.getSelectedIndex();
    }

    /**
     * Set the selected indices
     * @param indices to select
     */
    public void setSelectedIndices(int[] indices) {
        if (indices == null) {
            list.setSelectedIndices(new int[0]);
        } else {
            list.setSelectedIndices(indices);
        }
    }

    /**
     * Get the selected indices
     * @return selected indices
     */
    public int[] getSelectedIndices() {
        int[] indices = list.getSelectedIndices();
        if (indices.length == 0) {
            return null;
        }

        return indices;
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
     * Change the index of the selected element
     * @param index the new indew
     */
    public void setPositionSelected(int index) {
        int selected = list.getSelectedIndex();
        if (index >= 0 && index < vector.size() && selected != -1 && index != selected) {
            Object o = vector.remove(selected);
            vector.insertElementAt(o, index);
            model.fireContentsChanged(model, 0, vector.size() - 1);
        }
    }

    /**
     * Get the onclick action
     * @return the action
     */
    public UICallback getOnclick() {
        return action;
    }

    /**
     * Set the onclick action
     * @param the action
     */
    public void setOnclick(String action) {
        if (this.action == null) {
            removeListener();
            listener = new MouseAdapter() {
                public void mouseClicked(MouseEvent e) {
                    if (onclickEnable) {
			Object selected = list.getSelectedValue();
			String s = selected == null ? "[]" : ("\"" + selected.toString().replaceAll("\"", "\"\"").replaceAll("\'", "\'\'") + "\"");
                        UIWidgetTools.execAction(UIList.this.action, s);
                    }
                }
            };
            list.addMouseListener(listener);
        }
        this.action = UICallback.newInstance(this, action);
    }

    /**
     * Check if the onclick is enabled
     * @return true if enabled
     */
    public boolean getOnclickEnable() {
        return onclickEnable;
    }

    /**
     * Set if the onclick is enabled
     * @param b true if enabled
     */
    public void setOnclickEnable(boolean b) {
        onclickEnable = b;
    }

    /**
     * Inner class to have a public fire
     */
    private static class MyListModel extends DefaultListModel {

        public MyListModel(Vector v) {
            super();
            try {
                Field delegate = DefaultListModel.class.getDeclaredField("delegate");
                delegate.setAccessible(true);
                delegate.set(this, v);
            } catch (Exception e) {
                System.err.println(e);
            }
        }

        public void fireContentsChanged(Object o, int i, int f) {
            super.fireContentsChanged(o, i, f);
        }
    }
}
