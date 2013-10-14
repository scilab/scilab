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
import java.awt.Graphics;
import java.awt.GraphicsEnvironment;
import java.awt.Toolkit;

import javax.swing.JComponent;
import javax.swing.UIManager;

import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;

import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

/**
 * Label with LaTeX wrapper
 */
public class UILaTeXLabel extends UIComponent {

    private final static Font DEFAULTFONT = new Font("serif", Font.PLAIN, 12);
    private final static boolean isWindows = System.getProperty("os.name").toLowerCase().indexOf("win") != -1;
    private static float FACTOR = 1f;

    static {
        if (!GraphicsEnvironment.isHeadless()) {
            FACTOR = (float) Toolkit.getDefaultToolkit().getScreenResolution() / 72f;
        }
        ScilabCommonsUtils.loadOnUse("graphics_latex_textrendering");
    }

    protected LaTeXLabel label;

    /**
     * {@inheritDoc}
     */
    public UILaTeXLabel(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        label = new LaTeXLabel("", null, 0);

        return label;
    }

    @UIComponentAnnotation(attributes = {"text", "latex-style"})
    public Object newInstance(String text, String latexStyle) {
        label = new LaTeXLabel(text, latexStyle, 0);

        return label;
    }

    /**
     * {@inheritDoc}
     */
    public void setBackground(Color color) {
        if (color != null) {
            label.setOpaque(true);
            label.setBackground(color);
        }
    }

    /**
     * Alias for setText
     */
    public void setString(String str) {
        label.setText(str);
    }

    /**
     * Alias for getText
     */
    public String getString() {
        return label.getText();
    }

    /**
     * Component to display LaTeX
     */
    public static class LaTeXLabel extends JComponent {

        private TeXIcon icon;
        private float fontSize;
        private int latexStyle;
        private String latex;
        private Dimension preferred;

        /**
         * Default constructor
         * @param text the LaTeX string
         * @param style "display" or "text"
         * @param size the font size in point
         */
        public LaTeXLabel(String text, String style, float size) {
            this.fontSize = size <= 0 ? 12f : size;
            this.latexStyle = TeXConstants.STYLE_DISPLAY;
            this.latex = text;
            if (style != null && style.equalsIgnoreCase("text")) {
                this.latexStyle = TeXConstants.STYLE_TEXT;
            }

            icon = new TeXFormula(text).new TeXIconBuilder().setStyle(this.latexStyle).setSize(this.fontSize * FACTOR).build();
            this.preferred = new Dimension(icon.getIconWidth(), icon.getIconHeight());
        }

        /**
         * Set the LaTeX content
         * @param text the LaTeX
         */
        public void setText(String text) {
            this.latex = latex;
            update();
        }

        /**
         * Get the LaTeX content
         * @return the LaTeX string
         */
        public String getText() {
            return latex;
        }

        /**
         * {@inheritDoc}
         */
        public Dimension getPreferredSize() {
            return preferred;
        }

        /**
         * {@inheritDoc}
         */
        public Dimension getMinimumSize() {
            return getPreferredSize();
        }

        /**
         * {@inheritDoc}
         */
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            if (isOpaque() && getBackground() != null) {
                final Color old = g.getColor();
                g.setColor(getBackground());
                g.fillRect(0, 0, getWidth(), getHeight());
                g.setColor(old);
            }
            if (isEnabled()) {
                icon.setForeground(getForeground());
                icon.paintIcon(this, g, 0, 0);
            } else {
                if (isWindows && (UIManager.getColor("Label.disabledForeground") instanceof Color) && (UIManager.getColor("Label.disabledShadow") instanceof Color)) {
                    icon.setForeground(UIManager.getColor("Label.disabledShadow"));
                    icon.paintIcon(this, g, 1, 1);
                    icon.setForeground(UIManager.getColor("Label.disabledForeground"));
                    icon.paintIcon(this, g, 0, 0);
                } else {
                    icon.setForeground(getBackground().brighter());
                    icon.paintIcon(this, g, 1, 1);
                    icon.setForeground(getBackground().darker());
                    icon.paintIcon(this, g, 0, 0);
                }
            }
        }

        /**
         * {@inheritDoc}
         */
        public int getBaseline(int width, int height) {
            return (int) ((float) height * (icon.getBaseLine()));
        }

        /**
         * {@inheritDoc}
         */
        public void setFont(Font f) {
            this.fontSize = f.getSize2D();
            update();
        }

        /**
         * {@inheritDoc}
         */
        public Font getFont() {
            return DEFAULTFONT;
        }

        /**
         * Update the component
         */
        private void update() {
            icon = new TeXFormula(latex).new TeXIconBuilder().setStyle(latexStyle).setSize(fontSize).build();
            preferred.width = icon.getIconWidth();
            preferred.height = icon.getIconHeight();
            revalidate();
        }
    }
}
