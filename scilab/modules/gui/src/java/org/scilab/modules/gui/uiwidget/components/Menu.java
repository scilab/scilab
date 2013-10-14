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

import java.awt.Container;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.Icon;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;

import org.scilab.modules.types.ScilabList;
import org.scilab.modules.gui.uiwidget.ScilabTypeConverters;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.gui.uiwidget.callback.UICallback;

/**
 * Wrapper for uicontrol menu
 */
public class Menu extends UIComponent {

    protected JMenuItem button;
    protected String callback;
    protected int callbackType;
    protected UICallback commonCallBack;
    protected MouseAdapter mouseListener = new MouseAdapter() {
        public void mouseClicked(MouseEvent e) {
            if (commonCallBack != null) {
                button.setSelected(false);
                commonCallBack.actionPerformed(null);
            }
        }
    };
    protected String text;

    /**
     * {@inheritDoc}
     */
    public Menu(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        button = new JMenu();

        return button;
    }

    @UIComponentAnnotation(attributes = {"label", "icon", "checked", "accelerator"})
    public Object newInstance(String label, Icon icon, boolean checked, KeyStroke accelerator) {
        UIComponent parent = getParent();
        if (parent instanceof UIMenuBar) {
            button = new JMenu(label);
        } else {
            if (checked) {
                button = new JCheckBoxMenuItem(label, true);
            } else {
                button = new JMenuItem(label);
            }
        }

        setText(label);
        button.setIcon(icon);

        if (accelerator != null) {
            button.setAccelerator(accelerator);
        }

        return button;
    }

    /**
     * {@inheritDoc}
     */
    public void add(final UIComponent c) throws UIWidgetException {
        if (c instanceof Menu) {
            Menu menu = (Menu) c;
            if (!(button instanceof JMenu)) {
                JMenu m = new JMenu(button.getText());
                m.setIcon(button.getIcon());
                m.setMnemonic(button.getMnemonic());
                m.setEnabled(button.isEnabled());
                m.setForeground(button.getForeground());
                m.setVisible(button.isVisible());
                m.addMouseListener(mouseListener);

                if (commonCallBack != null) {
                    button.removeActionListener(commonCallBack);
                }

                replaceBy(m);
                button = m;
            }

            button.add(menu.button);
        } else {
            super.add(c);
        }
    }

    /**
     * Set a checked menu
     * @param b true if the menu must be checked
     */
    public void setChecked(boolean b) {
        if (b && !(button instanceof JCheckBoxMenuItem) && !(button instanceof JMenu)) {
            JCheckBoxMenuItem item = new JCheckBoxMenuItem(button.getText());
            item.setIcon(button.getIcon());
            item.setMnemonic(button.getMnemonic());
            item.setEnabled(button.isEnabled());
            item.setForeground(button.getForeground());
            item.setVisible(button.isVisible());

            if (commonCallBack != null) {
                item.addActionListener(commonCallBack);
                button.removeActionListener(commonCallBack);
            }

            replaceBy(item);
            button = item;
        }
        button.setSelected(b);
    }

    /**
     * Get the menu state
     * @return true if the menu is checked
     */
    public boolean getChecked() {
        return button.isSelected();
    }

    /**
     * Set the callback given as a Scilab list
     * @param callback a Scilab list containing the callback type followed by the callback string
     */
    public void setCallback(ScilabList callback) {
        if (callback.size() == 2) {
            setCallback(ScilabTypeConverters.getObjectFromValue(String.class, callback.get(1)), ScilabTypeConverters.getObjectFromValue(int.class, callback.get(0)));
        }
    }

    /**
     * Set the callback given as a Scilab list
     * @param callback a Scilab list containing the callback type followed by the callback string
     */
    protected void setCallback(String callback, int callbackType) {
        this.callback = callback;
        this.callbackType = callbackType;

        if (button instanceof JMenu) {
            button.removeMouseListener(mouseListener);
        } else if (commonCallBack != null) {
            button.removeActionListener(commonCallBack);
        }

        if (callback != null && !callback.isEmpty()) {
            commonCallBack = UICallback.newInstance(this, callback, UICallback.Type.intToType(callbackType));
            if (button instanceof JMenu) {
                button.addMouseListener(mouseListener);
            } else {
                button.addActionListener(commonCallBack);
            }
        } else {
            commonCallBack = null;
        }
    }

    public void setCallback(String callback) {
        setCallback(callback, 0);
    }

    public String getCallback() {
        return this.callback;
    }

    public void setCallbackType(int callbackType) {
        if (this.callbackType != callbackType) {
            setCallback(this.callback, callbackType);
        }
    }

    public int getCallbackType() {
        return this.callbackType;
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
     * Set the button text
     * @param text the button text
     */
    public void setText(String text) {
        boolean containsMnemonic = UITools.setTextAndMnemonic(text, button);
        if (containsMnemonic) {
            this.text = text;
        } else {
            if (this.text != null) {
                button.setMnemonic(0);
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
            return button.getText();
        }

        return this.text;
    }
}
