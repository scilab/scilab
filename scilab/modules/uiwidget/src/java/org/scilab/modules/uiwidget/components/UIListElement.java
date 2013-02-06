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
import java.awt.Font;
import java.util.Map;

import javax.swing.ImageIcon;
import javax.swing.JComponent;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

public class UIListElement extends UIComponent {

    private ListElement elem;

    public UIListElement(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        elem = new ListElement("", null);

        return elem;
    }

    @UIComponentAnnotation(attributes = {"text", "icon"})
    public Object newInstance(String text, ImageIcon icon) {
        elem = new ListElement(text, icon);

        return elem;
    }

    public static class ListElement {

        private String text;
        private ImageIcon icon;
        private Color fg;
        private Color bg;
        private Font font;
        private JComponent parent;

        public ListElement(String text, ImageIcon icon) {
            this.text = text == null ? "" : text;
            this.icon = icon;
        }

        public void setParent(JComponent c) {
            this.parent = c;
        }

        public void setText(String text) {
            this.text = text == null ? "" : text;
        }

        public String getText() {
            return text;
        }

        public void setFont(Font f) {
            this.font = f;
            update();
        }

        public Font getFont() {
            return font;
        }

        public void setIcon(ImageIcon i) {
            this.icon = i;
            update();
        }

        public ImageIcon getIcon() {
            return icon;
        }

        public void setBackground(Color c) {
            this.bg = c;
            update();
        }

        public Color getBackground() {
            return bg;
        }

        public void setForeground(Color c) {
            this.fg = c;
            update();
        }

        public Color getForeground() {
            return fg;
        }

        public String toString() {
            return text;
        }

        private void update() {
            if (parent != null) {
                parent.repaint();
            }
        }
    }
}
