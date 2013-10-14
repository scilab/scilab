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

import java.awt.Point;
import java.awt.event.AdjustmentListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.DefaultListModel;
import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;

import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

public class ListBox extends ScilabWidget {

    private MouseAdapter listener;
    private boolean clickEnabled = true;
    private JList list;

    public ListBox(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        list = new JList();
        list.setLayoutOrientation(JList.VERTICAL);
        list.setModel(new DefaultListModel());

        JScrollPane scroll = new JScrollPane();
        scroll.setViewportView(list);
        modifiableComponent = list;

        return scroll;
    }

    public void initialize() {
        super.initialize();
        try {
            setRelief(ScilabRelief.FLAT);
        } catch (UIWidgetException e) { }
    }

    public void setString(String[][] str) {
        JScrollPane scroll = (JScrollPane) getComponent();
        DefaultListModel model = new DefaultListModel();
        String[] s = getRowString(str);
        if (s != null) {
            if (s.length == 1) {
                s = s[0].split("\\|");
            }
            model.ensureCapacity(s.length);
            for (int i = 0; i < s.length; i++) {
                model.addElement(s[i]);
            }
            super.setString(str);
        }

        list.setModel(model);
        scroll.revalidate();
    }

    public void setListboxTop(int top) {
        super.setListboxTop(top);
        JScrollPane scroll = (JScrollPane) getComponent();
        if (top > 0 & top != getListboxTop()) {
            Point p = list.indexToLocation(top - 1);
            if (p != null) {
                p.y = Math.min(p.y, list.getHeight() - scroll.getViewport().getExtentSize().height - 1);
                scroll.getViewport().setViewPosition(p);
                scroll.doLayout();
            }
        }
    }

    public int getListboxTop() {
        JScrollPane scroll = (JScrollPane) getComponent();
        int listboxtopValue = list.locationToIndex(scroll.getViewport().getViewPosition()) + 1;

        return listboxtopValue;
    }

    public void setMin(double min) {
        super.setMin(min);
        list.setSelectionMode(Math.abs(getMax() - getMin()) > 1 ? ListSelectionModel.MULTIPLE_INTERVAL_SELECTION : ListSelectionModel.SINGLE_SELECTION);
    }

    public void setMax(double max) {
        super.setMax(max);
        list.setSelectionMode(Math.abs(getMax() - getMin()) > 1 ? ListSelectionModel.MULTIPLE_INTERVAL_SELECTION : ListSelectionModel.SINGLE_SELECTION);
    }

    public void setValue(double[] value) {
        super.setValue(value);
        if (value != null) {
            int[] indices = new int[value.length];
            for (int i = 0; i < value.length; i++) {
                indices[i] = (int) (value[i] - 1);
            }

            list.setSelectedIndices(indices);
        }
    }

    public double[] getValue() {
        int[] indices = list.getSelectedIndices();
        if (indices != null && indices.length > 0) {
            double[] dindices = new double[indices.length];
            for (int i = 0; i < indices.length; i++) {
                dindices[i] = (double) (indices[i] + 1);
            }

            return dindices;
        }

        return null;
    }

    public void setEnabled(boolean enable) {
        if (enable != clickEnabled) {
            clickEnabled = enable;
            list.setEnabled(enable);
        }
    }

    public void setCallback(String callback) {
        if (commonCallBack != null) {
            list.removeMouseListener(listener);
            listener = null;
        }

        super.setCallback(callback);

        if (commonCallBack != null) {
            listener = new MouseAdapter() {
                public void mouseClicked(MouseEvent e) {
                    if (clickEnabled && e.getButton() == MouseEvent.BUTTON1 && commonCallBack != null) {
                        commonCallBack.actionPerformed(null);
                    }
                }
            };
            list.addMouseListener(listener);
        }
    }
}
