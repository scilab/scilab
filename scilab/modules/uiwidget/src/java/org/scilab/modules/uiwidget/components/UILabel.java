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

import java.awt.Color;
import java.awt.Font;
import java.util.Map;

import javax.swing.Icon;
import javax.swing.JLabel;

import org.scilab.modules.uiwidget.StringConverters;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

public class UILabel extends UIComponent {

    private final static int defaultAlignment = new JLabel().getHorizontalAlignment();

    private JLabel label;

    public enum Alignment {
        LEADING (JLabel.LEADING),
        CENTER (JLabel.CENTER),
        LEFT (JLabel.LEFT),
        RIGHT (JLabel.RIGHT),
        TRAILING (JLabel.TRAILING);

        private final int value;

        Alignment(int value) {
            this.value = value;
        }

        public int value() {
            return value;
        }

        public static Alignment get(int value) {
            switch (value) {
                case JLabel.LEADING:
                    return Alignment.LEADING;
                case JLabel.CENTER:
                    return Alignment.CENTER;
                case JLabel.LEFT:
                    return Alignment.LEFT;
                case JLabel.RIGHT:
                    return Alignment.RIGHT;
                case JLabel.TRAILING:
                    return Alignment.TRAILING;
                default:
                    return Alignment.LEADING;
            }
        }
    }

    public UILabel(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        label = new JLabel();

        return label;
    }

    @UIComponentAnnotation(attributes = {"text", "icon"})
    public Object newInstance(String text, Icon icon) {
        if (icon != null) {
            label = new JLabel(text, icon, defaultAlignment);
        } else {
            label = new JLabel(text);
        }

        return label;
    }

    public void setUiStyle(Map<String, String> style) throws UIWidgetException {
        if (style != null) {
            String al = style.get("alignment");
            if (al != null && !al.isEmpty()) {
                Alignment a = StringConverters.getObjectFromValue(Alignment.class, al);
                label.setHorizontalAlignment(a.value());
                style.remove("alignment");
            }
            super.setUiStyle(style);
        }
    }

    public void setBackground(Color color) {
        if (color != null) {
            label.setOpaque(true);
            label.setBackground(color);
        }
    }

    public void setHorizontalAlignment(Alignment alignment) {
        label.setHorizontalAlignment(alignment.value());
    }
}
