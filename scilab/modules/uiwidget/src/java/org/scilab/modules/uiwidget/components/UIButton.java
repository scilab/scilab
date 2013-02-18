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
import java.util.Map;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

import org.scilab.modules.uiwidget.StringConverters;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;

/**
 * JButton wrapper
 */
public class UIButton extends UIComponent {

    private JButton button;
    private JPopupMenu menu;
    private Action menuAction;
    private boolean hasRolloverIcon;
    private ActionListener clicklistener;
    private String clickaction;
    private boolean onclickEnable = true;

    public enum Relief {
        NONE,
        HALF,
        NORMAL;
    }

    public enum Alignment {
        LEADING (JButton.LEADING),
        CENTER (JButton.CENTER),
        LEFT (JButton.LEFT),
        RIGHT (JButton.RIGHT),
        TRAILING (JButton.TRAILING);

        private final int value;

        Alignment(int value) {
            this.value = value;
        }

        public int value() {
            return value;
        }

        public static Alignment get(int value) {
            switch (value) {
                case JButton.LEADING:
                    return Alignment.LEADING;
                case JButton.CENTER:
                    return Alignment.CENTER;
                case JButton.LEFT:
                    return Alignment.LEFT;
                case JButton.RIGHT:
                    return Alignment.RIGHT;
                case JButton.TRAILING:
                    return Alignment.TRAILING;
                default:
                    return Alignment.LEADING;
            }
        }

        public static String getAsString(int value) {
            switch (value) {
                case JButton.LEADING:
                    return "leading";
                case JButton.CENTER:
                    return "center";
                case JButton.LEFT:
                    return "left";
                case JButton.RIGHT:
                    return "right";
                case JButton.TRAILING:
                    return "trailing";
                default:
                    return "leading";
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public UIButton(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        button = new JButton();

        return button;
    }

    @UIComponentAnnotation(attributes = {"text", "icon"})
    public Object newInstance(String text, Icon icon) {
        if (icon == null) {
            button = new JButton(text);
        } else {
            button = new JButton(text, icon);
        }

        return button;
    }

    /**
     * Set Rollover icon
     * @param icon an icon
     */
    public void setRolloverIcon(Icon icon) {
        button.setRolloverEnabled(icon != null);
        button.setRolloverIcon(icon);
        hasRolloverIcon = icon != null;
    }

    /**
     * Set Rollover
     * @param b if true enable default rollover
     */
    public void setRollover(boolean b) {
        Icon icon = button.getIcon();
        if (icon != null && !hasRolloverIcon && b) {
            button.setRolloverEnabled(true);
            button.setRolloverIcon(UITools.getRolloverIcon(icon));
        }
    }

    /**
     * Set the alignment
     * @param a the alignment
     */
    public void setAlignment(Alignment a) {
        button.setHorizontalAlignment(a.value());
    }

    /**
     * Get the alignment
     * @return he alignment
     */
    public String getAlignment() {
        return Alignment.getAsString(button.getHorizontalAlignment());
    }

    /**
     * Set the alignment
     * @param a the alignment
     */
    public void setHorizontalAlignment(Alignment alignment) {
        button.setHorizontalAlignment(alignment.value());
    }

    /**
     * Get the alignment
     * @return he alignment
     */
    public String getHorizontalAlignment() {
        return Alignment.getAsString(button.getHorizontalAlignment());
    }

    /**
     * {@inheritDoc}
     */
    public void setUiStyle(Map<String, String> style) throws UIWidgetException {
        String al = style.get("alignment");
        if (al != null && !al.isEmpty()) {
            Alignment a = StringConverters.getObjectFromValue(Alignment.class, al);
            button.setHorizontalAlignment(a.value());
            style.remove("alignment");
        }
        super.setUiStyle(style);
    }

    /**
     * Add a JMenuitem
     * @param menuitem the menu item
     */
    public void add(JMenuItem menuitem) {
        if (menu == null) {
            menu = new JPopupMenu();
            menu.setBorderPainted(true);
            menuAction = new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    menu.show(button, 0, button.getBounds(null).height);
                }
            };
            button.addActionListener(menuAction);
        }
        menu.add(menuitem);
    }

    /**
     * Remove the onclick listener
     */
    protected void removeClickListener() {
        if (clicklistener != null) {
            button.removeActionListener(clicklistener);
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
                        UIWidgetTools.execAction(UIButton.this, UIButton.this.clickaction);
                    }
                }
            };
            button.addActionListener(clicklistener);
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

    /**
     * Alias for setText
     */
    public void setLabel(String label) {
        button.setText(label);
    }

    /**
     * Alias for getText
     */
    public String getLabel() {
        return button.getText();
    }

    /**
     * Alias for setText
     */
    public void setString(String label) {
        button.setText(label);
    }

    /**
     * Alias for getText
     */
    public String getString() {
        return button.getText();
    }
}
