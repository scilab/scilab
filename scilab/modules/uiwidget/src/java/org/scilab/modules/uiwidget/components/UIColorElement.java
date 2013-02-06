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

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.util.Map;

import javax.swing.Icon;
import javax.swing.JComponent;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

public class UIColorElement extends UIComponent {

    private static final Dimension SIXTEEN = new Dimension(16, 16);

    private ColorElement elem;

    public UIColorElement(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        elem = new ColorElement("", null, null);

        return elem;
    }

    @UIComponentAnnotation(attributes = {"text", "color", "border-color"})
    public Object newInstance(String text, Color color, Color borderColor) {
        elem = new ColorElement(text, color, borderColor);

        return elem;
    }

    public static class ColorElement {

        private String text;
        private Icon icon;
        private Color color;
        private Color borderColor;
        private Font font;
        private JComponent parent;

        public ColorElement(String text, Color color, Color borderColor) {
            this.text = text == null ? "" : text;
            this.color = color == null ? Color.ORANGE : color;
            this.borderColor = borderColor;
            this.icon = UITools.getColoredIcon(SIXTEEN, color, borderColor);
        }

        public void setParent(JComponent c) {
            this.parent = c;
        }

        public void setFont(Font f) {
            this.font = f;
            update();
        }

        public Font getFont() {
            return font;
        }

        public void setText(String text) {
            this.text = text == null ? "" : text;
        }

        public String getText() {
            return text;
        }

        public void setColor(Color color) {
            this.color = color == null ? Color.ORANGE : color;
            this.icon = UITools.getColoredIcon(SIXTEEN, color, borderColor);
            update();
        }

        public Color getColor() {
            return color;
        }

        public void setBorderColor(Color borderColor) {
            this.borderColor = borderColor;
            this.icon = UITools.getColoredIcon(SIXTEEN, color, borderColor);
            update();
        }

        public Color getBorderColor() {
            return borderColor;
        }

        public Icon getIcon() {
            return icon;
        }

        public String toString() {
            return text;
        }

        public String[] getRep() {
            return new String[] {text, String.format("#%06X", (0xFFFFFF & color.getRGB()))};
        }

        private void update() {
            if (parent != null) {
                parent.repaint();
            }
        }
    }
}
