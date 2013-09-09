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
import org.scilab.modules.uiwidget.callback.UICallback;

/**
 * JMenuItem wrapper
 */
public class UIMenuItem extends UIComponent {

    protected MenuType type = MenuType.NORMAL;
    protected JMenuItem menu;
    protected ActionListener clicklistener;
    protected UICallback clickaction;
    protected boolean onclickEnable = true;
    protected String text;

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

    @UIComponentAnnotation(attributes = {"text", "icon", "accelerator", "menu-type", "selected", "mnemonic"})
    public Object newInstance(String text, Icon icon, KeyStroke accel, MenuType type, boolean selected, KeyStroke mnemonic) {
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

        if (mnemonic == null) {
            setText(text);
        } else {
            setMnemonic(mnemonic);
        }

        return menu;
    }

    /**
     * Set the mnemonic
     * @param mnemonic the mnemonic
     */
    public void setMnemonic(KeyStroke mnemonic) {
        if (mnemonic != null) {
            menu.setMnemonic(mnemonic.getKeyCode());
        }
    }

    /**
     * Get the mnemonic
     * @return the mnemonic
     */
    public KeyStroke getMnemonic() {
        int mnemonic = menu.getMnemonic();
        if (mnemonic > 0) {
            return KeyStroke.getKeyStroke(mnemonic, 0);
        }

        return null;
    }

    /**
     * Set the menu type
     * @param type the menu type
     */
    public void setMenuType(MenuType type) {
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
            mi.setMnemonic(menu.getMnemonic());
            mi.setForeground(menu.getForeground());
            mi.setBackground(menu.getBackground());
            mi.setEnabled(menu.isEnabled());
            mi.setVisible(menu.isVisible());

            replaceBy(mi);
            if (clicklistener != null) {
                menu.removeActionListener(clicklistener);
                mi.addActionListener(clicklistener);
            }

            menu = mi;
        }
    }

    /**
     * Get the menu type
     * @return menu type
     */
    public String getMenuType() {
        return type.getAsString();
    }

    /**
     * Set a checked menu
     * @param b true if the menu must be checked
     */
    public void setChecked(boolean b) {
        setMenuType(MenuType.CHECK);
        menu.setSelected(b);
    }

    /**
     * Get the menu state
     * @return true if the menu is checked
     */
    public boolean getChecked() {
        return menu.isSelected();
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
    public UICallback getOnclick() {
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
                        UIWidgetTools.execAction(UIMenuItem.this.clickaction);
                    }
                }
            };
            menu.addActionListener(clicklistener);
        }
        this.clickaction = UICallback.newInstance(this, action);
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
     * Alias for setText
     */
    public void setLabel(String label) {
        setText(label);
    }

    /**
     * Alias for getText
     */
    public String getLabel() {
        return getText();
    }

    /**
     * Alias for setText
     */
    public void setString(String label) {
        setText(label);
    }

    /**
     * Alias for getText
     */
    public String getString() {
        return getText();
    }

    /**
     * Set the button text
     * @param text the button text
     */
    public void setText(String text) {
        boolean containsMnemonic = UITools.setTextAndMnemonic(text, menu);
        if (containsMnemonic) {
            this.text = text;
        } else {
            if (this.text != null) {
                menu.setMnemonic(0);
            }
            this.text = null;
        }
    }

    /**
     * Get the button text
     * @return the button text
     */
    public String getText() {
        if (this.text == null) {
            return menu.getText();
        }

        return this.text;
    }
}
