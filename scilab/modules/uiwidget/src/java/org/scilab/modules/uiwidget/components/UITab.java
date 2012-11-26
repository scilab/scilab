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

import java.awt.Component;
import java.awt.LayoutManager;
import java.util.Map;

import javax.swing.JComponent;
import javax.swing.JTabbedPane;
import javax.swing.event.ChangeListener;
import javax.swing.event.ChangeEvent;

import org.scilab.modules.uiwidget.UIAccessTools;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

import javax.swing.event.AncestorListener;
import javax.swing.event.AncestorEvent;

public class UITab extends UIComponent {

    private JTabbedPane tab;

    public enum Placement {
        TOP (JTabbedPane.TOP),
        BOTTOM (JTabbedPane.BOTTOM),
        LEFT (JTabbedPane.LEFT),
        RIGHT (JTabbedPane.RIGHT);

        private final int value;

        Placement(int value) {
            this.value = value;
        }

        public int value() {
            return value;
        }
    }

    public UITab(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        tab = new JTabbedPane();

        return tab;
    }

    public void finish() {
        //tab.revalidate();
        //tab.layout();
        //tab.validate();
    }

    @UIComponentAnnotation(attributes = {"placement"})
    public Object newInstance(Placement placement) {
        tab = new JTabbedPane(placement.value());
        tab.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                Component c = tab.getSelectedComponent();
                if (!c.isValid()) {
                    c.validate();
                }
            }
        });
        tab.addAncestorListener(new AncestorListener() {
            public void ancestorMoved(AncestorEvent e) {
            }

            public void ancestorRemoved(AncestorEvent e) {
            }

            public void ancestorAdded(AncestorEvent e) {
                /*if (!tab.isValid()) {
                tab.validate();
                }
                System.out.println(tab.isValid());*/
            }
        });
        return tab;
    }

    public void add(final UIComponent comp) throws UIWidgetException {
        if (comp.getComponent() instanceof JComponent) {
            UIAccessTools.execOnEDT(new Runnable() {
                public void run() {
                    try {
                        tab.addTab(comp.getTabTitle() == null ? "" : comp.getTabTitle(), comp.getJComponent());
                    } catch (Exception e) { }
                }
            });
        } else {
            super.add(comp);
        }
    }

    public void setLayout(String layout) throws UIWidgetException {
        throw new UIWidgetException("Cannot set a layout on a JTabbedPane");
    }
}
