/*
 * Uicontrol2 ( http://forge.scilab.org/index.php/p/uicontrol2/ ) - This file is a part of Uicontrol2
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
import java.util.Map;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

import org.scilab.modules.uiwidget.StringConverters;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

public class UIButton extends UIComponent {

    private JButton button;
    private Action action;
    private JPopupMenu menu;
    private Action menuAction;
    private boolean hasRolloverIcon;

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
    }

    public UIButton(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

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

    public void setRolloverIcon(Icon icon) {
        button.setRolloverEnabled(icon != null);
        button.setRolloverIcon(icon);
        hasRolloverIcon = icon != null;
    }

    public void setRollover(boolean b) {
        Icon icon = button.getIcon();
        if (icon != null && !hasRolloverIcon && b) {
            button.setRolloverEnabled(true);
            button.setRolloverIcon(UITools.getRolloverIcon(icon));
        }
    }

    public void setAlignment(Alignment a) {
        button.setHorizontalAlignment(a.value());
    }

    public void setUiStyle(Map<String, String> style) throws UIWidgetException {
        String al = style.get("alignment");
        if (al != null && !al.isEmpty()) {
            Alignment a = StringConverters.getObjectFromValue(Alignment.class, al);
            button.setHorizontalAlignment(a.value());
            style.remove("alignment");
        }
        super.setUiStyle(style);
    }

    public void setRelief(Relief relief) {
        switch (relief) {
            case NONE:
                //button.setContentAreaFilled(false);
                //button.setOpaque(true);
                button.setBorder(BorderFactory.createEmptyBorder());
                break;
            case HALF:
            case NORMAL:
                break;
        }
    }

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

    public void setOnclick(Action action) {
        if (this.action != null) {
            button.removeActionListener(this.action);
        }
        if (action != null) {
            button.addActionListener(action);
        }
        this.action = action;
    }

    public void setLabel(String label) {
        button.setText(label);
    }

    public void setHorizontalAlignment(Alignment alignment) {
        button.setHorizontalAlignment(alignment.value());
    }
}
