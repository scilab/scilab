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

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.Map;

import javax.swing.JComponent;
import javax.swing.JSplitPane;
import javax.swing.plaf.basic.BasicSplitPaneUI;

import org.scilab.modules.gui.uiwidget.UIAccessTools;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

public class UISplit extends UIComponent {

    private JSplitPane split;
    private double proportion;
    private boolean first;
    private boolean constantProportion;
    private boolean propListenerInstalled;
    private int dividerHashCode = -1;
    private double percent = -1;

    public enum Orientation {
        HORIZONTAL (JSplitPane.HORIZONTAL_SPLIT),
        VERTICAL (JSplitPane.VERTICAL_SPLIT);

        private final int value;

        Orientation(int value) {
            this.value = value;
        }

        public int value() {
            return value;
        }
    }

    public UISplit(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        split = new JSplitPane();

        return split;
    }

    @UIComponentAnnotation(attributes = {"orientation", "proportion"})
    public Object newInstance(Orientation orientation, double proportion) {
        this.proportion = Double.isNaN(proportion) ? 0.5 : Math.max(0, Math.min(proportion, 1));
        split = new JSplitPane(orientation.value()) {
            public void setBounds(int x, int y, int w, int h) {
                super.setBounds(x, y, w, h);
                setResizeWeight(UISplit.this.proportion);
                setDividerLocation((int) (UISplit.this.proportion * UISplit.this.getSplitSize()));
            }
        };
        setConstantPercent(true);

        return split;
    }

    public void add(final UIComponent comp) throws UIWidgetException {
        if (comp.getComponent() instanceof JComponent) {
            UIAccessTools.execOnEDT(new Runnable() {
                public void run() {
                    try {
                        if (!first) {
                            split.setLeftComponent(comp.getJComponent());
                            first = true;
                        } else {
                            split.setRightComponent(comp.getJComponent());
                            split.setDividerLocation(proportion);
                        }
                    } catch (Exception e) { }
                }
            });
        } else {
            super.add(comp);
        }
    }

    public void setLayout(String layout) throws UIWidgetException {
        throw new UIWidgetException("Cannot set a layout on a JSplitPane");
    }

    public void setConstantPercent(boolean constantProportion) {
        this.constantProportion = constantProportion;
        if (constantProportion && !propListenerInstalled) {
            propListenerInstalled = true;
            split.addPropertyChangeListener(JSplitPane.DIVIDER_LOCATION_PROPERTY, new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent pce) {
                    if (UISplit.this.constantProportion && split.getUI() instanceof BasicSplitPaneUI) {
                        BasicSplitPaneUI ui = (BasicSplitPaneUI) split.getUI();
                        if (dividerHashCode != ui.getDivider().hashCode()) {
                            dividerHashCode = ui.getDivider().hashCode();
                            ui.getDivider().addMouseListener(new MouseAdapter() {
                                public void mouseReleased(MouseEvent e) {
                                    UISplit.this.percent = getDividerProportion();
                                    split.setResizeWeight(percent);
                                }
                            });
                        }
                    }
                }
            });
        }
    }

    private int getSplitSize() {
        return split.getOrientation() == JSplitPane.HORIZONTAL_SPLIT ? split.getWidth() : split.getHeight();
    }

    private double getDividerProportion() {
        int size = getSplitSize();
        int divLoc = split.getDividerLocation();

        return size == 0 ? 0 : (double) divLoc / ((double) size - split.getDividerSize());
    }
}
