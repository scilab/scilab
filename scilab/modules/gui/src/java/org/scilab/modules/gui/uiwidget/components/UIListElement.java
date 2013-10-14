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

package org.scilab.modules.gui.uiwidget.components;

import java.awt.Color;
import java.awt.Font;

import javax.swing.ImageIcon;
import javax.swing.JComponent;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

/**
 * Wrapper for list element to put in a UIList
 */
public class UIListElement extends UIComponent {

    private ListElement elem;

    /**
     * {@inheritDoc}
     */
    public UIListElement(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        elem = new ListElement("", null);

        return elem;
    }

    @UIComponentAnnotation(attributes = {"text", "icon"})
    public Object newInstance(String text, ImageIcon icon) {
        elem = new ListElement(text, icon);

        return elem;
    }

    /**
     * Inner class for element properties
     */
    public static class ListElement {

        private String text;
        private ImageIcon icon;
        private Color fg;
        private Color bg;
        private Font font;
        private JComponent parent;

        /**
         * Default constructor
         * @param text element text
         * @param icon the element icon
         */
        public ListElement(String text, ImageIcon icon) {
            this.text = text == null ? "" : text;
            this.icon = icon;
        }

        /**
         * Set the parent
         * @param c the parent
         */
        public void setParent(JComponent c) {
            this.parent = c;
        }

        /**
         * Set the text
         * @param text the text
         */
        public void setText(String text) {
            this.text = text == null ? "" : text;
        }

        /**
         * Get the text
         * @return the text
         */
        public String getText() {
            return text;
        }

        /**
         * Set the font
         * @param f the font
         */
        public void setFont(Font f) {
            this.font = f;
            update();
        }

        /**
         * Get the font
         * @return the font
         */
        public Font getFont() {
            return font;
        }

        /**
         * Set the icon
         * @param i the icon
         */
        public void setIcon(ImageIcon i) {
            this.icon = i;
            update();
        }

        /**
         * Get the icon
         * @return the icon
         */
        public ImageIcon getIcon() {
            return icon;
        }

        /**
         * Set the background color
         * @param c the bg color
         */
        public void setBackground(Color c) {
            this.bg = c;
            update();
        }

        /**
         * Get the background color
         * @return the bg color
         */
        public Color getBackground() {
            return bg;
        }

        /**
         * Set the foreground color
         * @param c the fg color
         */
        public void setForeground(Color c) {
            this.fg = c;
            update();
        }

        /**
         * Get the foreground color
         * @return the fg color
         */
        public Color getForeground() {
            return fg;
        }

        /**
         * {@inheritDoc}
         */
        public String toString() {
            return text;
        }

        /**
         * Update the parent
         */
        private void update() {
            if (parent != null) {
                parent.repaint();
            }
        }
    }
}
