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
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JMenuItem;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.KeyStroke;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;

/**
 * JMenuItem wrapper
 */
public class UIMenuItem extends UIComponent {

    private MenuType type = MenuType.NORMAL;
    private JMenuItem menu;
    private ActionListener clicklistener;
    private String clickaction;
    private boolean onclickEnable = true;

    public enum MenuType {
        NORMAL, RADIO, CHECK;

        public String getAsString() {
            switch (this) {
                case RADIO:
                    return "radio";
                case CHECK:
                    return "check";
                default:
                    return "normal";
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public UIMenuItem(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        menu = new JMenuItem();

        return menu;
    }

    @UIComponentAnnotation(attributes = {"text", "icon", "accelerator", "type", "selected"})
    public Object newInstance(String text, Icon icon, KeyStroke accel, MenuType type, boolean selected) {
        this.type = type;
        switch (type) {
            case NORMAL:
                menu = new JMenuItem(text);
                break;
            case RADIO:
                menu = new JRadioButtonMenuItem(text, selected);
                break;
            case CHECK:
                menu = new JCheckBoxMenuItem(text, selected);
                break;
        }

        if (icon != null) {
            menu.setIcon(icon);
        }

        if (accel != null) {
            menu.setAccelerator(accel);
        }

        return menu;
    }

    /**
     * Set the menu type
     * @param type the menu type
     */
    public void setType(MenuType type) {
        if (this.type != type) {
            this.type = type;
            JMenuItem mi = null;
            switch (type) {
                case NORMAL:
                    mi = new JMenuItem(menu.getText());
                    break;
                case RADIO:
                    mi = new JRadioButtonMenuItem(menu.getText(), menu.isSelected());
                    break;
                case CHECK:
                    mi = new JCheckBoxMenuItem(menu.getText(), menu.isSelected());
                    break;
            }

            mi.setIcon(menu.getIcon());
            mi.setAccelerator(menu.getAccelerator());

            replaceBy(mi);
        }
    }

    /**
     * Get the menu type
     * @return menu type
     */
    public String getType() {
        return type.getAsString();
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
                        UIWidgetTools.execAction(UIMenuItem.this, UIMenuItem.this.clickaction);
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
