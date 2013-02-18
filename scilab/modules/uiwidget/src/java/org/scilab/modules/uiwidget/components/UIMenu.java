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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.Icon;
import javax.swing.JMenu;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;

/**
 * JMenu wrapper
 */
public class UIMenu extends UIComponent {

    private JMenu menu;
    private ActionListener clicklistener;
    private String clickaction;
    private boolean onclickEnable = true;

    /**
     * {@inheritDoc}
     */
    public UIMenu(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        menu = new JMenu();

        return menu;
    }

    @UIComponentAnnotation(attributes = {"text", "icon"})
    public Object newInstance(String text, Icon icon) {
        menu = new JMenu(text);

        if (icon != null) {
            menu.setIcon(icon);
        }

        return menu;
    }

    /**
     * Remove the onclick listener
     */
    protected void removeClickListener() {
        if (clicklistener != null) {
            menu.removeActionListener(clicklistener);
            clicklistener = null;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void remove() {
        removeClickListener();
        super.remove();
    }

    /**
     * Get the onclick action
     * @return the action
     */
    public String getOnclick() {
        return this.clickaction;
    }

    /**
     * Set the onclick action
     * @param the action
     */
    public void setOnclick(final String action) {
        if (this.clickaction == null) {
            removeClickListener();
            clicklistener = new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    if (onclickEnable) {
                        UIWidgetTools.execAction(UIMenu.this, UIMenu.this.clickaction);
                    }
                }
            };
            menu.addActionListener(clicklistener);
        }
        this.clickaction = action;
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
}
