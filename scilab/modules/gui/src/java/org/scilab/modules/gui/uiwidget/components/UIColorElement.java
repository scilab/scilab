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
import java.awt.Dimension;
import java.awt.Font;
import java.util.Map;

import javax.swing.Icon;
import javax.swing.JComponent;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

/**
 * Wrapper for color element to put in a combo
 */
public class UIColorElement extends UIComponent {

    private static final Dimension SIXTEEN = new Dimension(16, 16);

    private ColorElement elem;

    /**
     * {@inheritDoc}
     */
    public UIColorElement(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        elem = new ColorElement("", null, null);

        return elem;
    }

    @UIComponentAnnotation(attributes = {"text", "color", "border-color"})
    public Object newInstance(String text, Color color, Color borderColor) {
        elem = new ColorElement(text, color, borderColor);

        return elem;
    }

    /**
     * Inner class for element properties
     */
    public static class ColorElement {

        private String text;
        private Icon icon;
        private Color color;
        private Color borderColor;
        private Font font;
        private JComponent parent;

        /**
         * Default constructor
         * @param text element text
         * @param color the color of the icon
         * @param borderColor the color of the icon border
         */
        public ColorElement(String text, Color color, Color borderColor) {
            this.text = text == null ? "" : text;
            this.color = color == null ? Color.WHITE : color;
            this.borderColor = borderColor;
            this.icon = UITools.getColoredIcon(SIXTEEN, color, borderColor);
        }

        /**
         * Set the parent
         * @param c the parent
         */
        public void setParent(JComponent c) {
            this.parent = c;
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
         * Set the color
         * @param color the color
         */
        public void setColor(Color color) {
            this.color = color == null ? Color.WHITE : color;
            this.icon = UITools.getColoredIcon(SIXTEEN, color, borderColor);
            update();
        }

        /**
         * Get the color
         * @return the color
         */
        public Color getColor() {
            return color;
        }

        /**
         * Set the border color
         * @param color the border color
         */
        public void setBorderColor(Color borderColor) {
            this.borderColor = borderColor;
            this.icon = UITools.getColoredIcon(SIXTEEN, color, borderColor);
            update();
        }

        /**
         * Get the border color
         * @return the border color
         */
        public Color getBorderColor() {
            return borderColor;
        }

        /**
         * Get the icon
         * @return the icon
         */
        public Icon getIcon() {
            return icon;
        }

        /**
         * {@inheritDoc}
         */
        public String toString() {
            return text;
        }

        /**
         * Get representation
         * @return the representation
         */
        public String[] getRep() {
            return new String[] {text, String.format("#%06X", (0xFFFFFF & color.getRGB()))};
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
