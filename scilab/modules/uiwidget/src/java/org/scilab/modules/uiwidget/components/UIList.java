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
import java.util.Map;

import javax.swing.DefaultListCellRenderer;
import javax.swing.DefaultListModel;
import javax.swing.JLabel;
import javax.swing.JList;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;

public class UIList extends UIComponent {

    private JList list;
    private MouseListener listener;
    private DefaultListModel model;
    private boolean isFinished;
    private String action;

    public UIList(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        list = new JList();
        model = new DefaultListModel();
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

        return list;
    }

    public void finish() {
        list.setModel(model);
        list.setSelectedIndex(0);
    }

    public void add(Object obj) {
        model.addElement(obj);
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
}
