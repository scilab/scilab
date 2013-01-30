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
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import javax.swing.DefaultListCellRenderer;
import javax.swing.DefaultListModel;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.ListModel;
import javax.swing.ListSelectionModel;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;

public class UIList extends UIComponent {

    private JList list;
    private MouseListener listener;
    private boolean onclickEnable = true;
    private MyListModel model;
    private Vector<Object> vector;
    private boolean isFinished;
    private String action;

    public UIList(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

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

    public void finish() {
        model.fireContentsChanged(model, 0, vector.size() - 1);
        resetIndex();
    }

    public void add(Object obj) {
        vector.addElement(obj);
        if (obj instanceof UIListElement.ListElement) {
            ((UIListElement.ListElement) obj).setParent(list);
        }
    }

    public void removeListener() {
        if (listener != null) {
            list.removeMouseListener(listener);
            listener = null;
        }
    }

    public void remove() {
        removeListener();
        super.remove();
    }

    public void setString(String item) {
        setSelectedItem(item);
    }

    public String getString() {
        if (getSelectedIndex() == -1) {
            return null;
        }

        return getSelectedItem();
    }

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

    public void setMultiple(boolean multiple) {
        if (multiple) {
            list.getSelectionModel().setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        } else {
            list.getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        }
    }

    public boolean getMultiple() {
        int m = list.getSelectionModel().getSelectionMode();
        return m == ListSelectionModel.MULTIPLE_INTERVAL_SELECTION;
    }

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

    private void resetIndex() {
        setSelectedIndex(0);
    }

    public String getSelectedItem() {
        if (list.getSelectedIndex() != -1) {
            return list.getSelectedValue().toString();
        }

        return null;
    }

    public void setSelectedIndex(int index) {
        try {
            list.setSelectedIndex(index);
        } catch (IllegalArgumentException e) { }
    }

    public int getSelectedIndex() {
        return list.getSelectedIndex();
    }

    public void setSelectedIndices(int[] indices) {
        if (indices == null) {
            list.setSelectedIndices(new int[0]);
        } else {
            list.setSelectedIndices(indices);
        }
    }

    public int[] getSelectedIndices() {
        int[] indices = list.getSelectedIndices();
        if (indices.length == 0) {
            return null;
        }

        return indices;
    }

    public int getValue() {
        return getSelectedIndex();
    }

    public void setValue(int index) {
        setSelectedIndex(index);
    }

    public void setPositionSelected(int index) {
        int selected = list.getSelectedIndex();
        if (index >= 0 && index < vector.size() && selected != -1 && index != selected) {
            Object o = vector.remove(selected);
            vector.insertElementAt(o, index);
            model.fireContentsChanged(model, 0, vector.size() - 1);
        }
    }

    public String getOnclick() {
        return action;
    }

    public void setOnclick(String action) {
        if (this.action == null) {
            removeListener();
            listener = new MouseAdapter() {
                public void mouseClicked(MouseEvent e) {
                    UIWidgetTools.execAction(UIList.this, UIList.this.action, "\"" + list.getSelectedValue().toString().replaceAll("\"", "\"\"").replaceAll("\'", "\'\'") + "\"");
                }
            };
            list.addMouseListener(listener);
        }
        this.action = action;
    }

    public boolean getOnclickEnable() {
        return onclickEnable;
    }

    public void setOnclickEnable(boolean b) {
        onclickEnable = b;
    }

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
