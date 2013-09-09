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

import javax.swing.JMenu;
import javax.swing.JMenuBar;

import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.bridge.menubar.SwingScilabMenuBar;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

/**
 * JMenuBar wrapper
 */
public class UIMenuBar extends UIComponent {

    protected MenuBar bar;

    /**
     * {@inheritDoc}
     */
    public UIMenuBar(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        bar = ScilabMenuBar.createMenuBar();

        return bar.getAsSimpleMenuBar();
    }

    /**
     * {@inheritDoc}
     */
    public void add(final UIComponent c) throws UIWidgetException {
        SwingScilabMenuBar menubar = (SwingScilabMenuBar) bar.getAsSimpleMenuBar();
        Object o = c.getComponent();
        if (o instanceof JMenu) {
            menubar.add((JMenu) o);
        } else if (c instanceof Menu) {
            menubar.add(((Menu) c).button);
        } else {
            super.add(c);
        }

        if (menubar.isVisible()) {
            menubar.revalidate();
            menubar.repaint();
        }
    }

    public void removeMenu(String menuName) {
        UIComponent menu = getNamedMenu(menuName);
        if (menu != null) {
            menu.remove();
        }
    }

    public void enableMenu(String menuName, boolean enabled) {
        UIComponent menu = getNamedMenu(menuName);
        if (menu != null) {
            try {
                menu.setEnable(enabled);
            } catch (Exception e) { }
        }
    }

    private UIComponent getNamedMenu(String menuName) {
        if (childrenList != null) {
            for (UIComponent child : childrenList) {
                String label = null;
                if (child instanceof Menu) {
                    label = ((Menu) child).getLabel();
                } else if (child instanceof UIMenu) {
                    label = ((UIMenu) child).getLabel();
                }

                if (label != null && label.equals(menuName)) {
                    return child;
                }
            }
        }

        return null;
    }
}
