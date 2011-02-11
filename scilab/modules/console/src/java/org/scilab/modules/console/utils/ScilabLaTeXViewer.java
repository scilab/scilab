/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.console.utils;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Rectangle;

import javax.swing.Icon;
import javax.swing.JPanel;
import javax.swing.text.BadLocationException;
import javax.swing.text.JTextComponent;

/**
 * Class to have a preview of a LaTeX string
 * @author Calixte DENIZET
 */
public final class ScilabLaTeXViewer extends JPanel {

    private static final int INSET = 3;
    private static final Rectangle NULLRECT = new Rectangle(0, 0, 0, 0);

    private static int defaultSize = 15;
    private static Icon icon;
    private static int width;
    private static int height;
    private static ScilabLaTeXViewer viewer = new ScilabLaTeXViewer();

    /**
     * Default constructor
     */
    private ScilabLaTeXViewer() {
        // I disable the double-buffering, it's useless here
        super(false);
    }

    /**
     * Set the default size to render LaTeX
     * @param size the font size
     */
    public static void setDefaultSize(int size) {
        viewer.defaultSize = size;
    }

    /**
     * {@inheritedDoc}
     */
    public void paint(Graphics g) {
        g.setColor(Color.WHITE);
        g.fillRect(0, 0, width + 2 * INSET, height + 2 * INSET);
        g.setColor(Color.LIGHT_GRAY);
        g.drawRect(0, 0, width + 2 * INSET - 1, height + 2 * INSET - 1);
        icon.paintIcon(this, g, INSET, INSET);
    }

    /**
     * @param sep the pane where to remove the LaTeXViewer
     */
    public static void removeLaTeXViewer(JTextComponent tc) {
        if (viewer.icon != null) {
            viewer.icon = null;
            viewer.setVisible(false);
            tc.remove(viewer);
            tc.repaint();
        }
    }

    /**
     * @param sep the pane where to display the LaTeX
     * @param exp the expression
     * @param b the beginning of the expression
     * @param e the end of the expression
     */
    public static void displayExpressionIfVisible(JTextComponent tc, int totalHeight, String exp, int b, int e) {
        if (viewer.isVisible()) {
            displayExpression(tc, totalHeight, exp, b, e);
        }
    }

    /**
     * @param sep the pane where to display the LaTeX
     * @param exp the expression
     * @param b the beginning of the expression
     * @param e the end of the expression
     */
    public static int displayExpression(JTextComponent tc, int totalHeight, String exp, int b, int e) {
        String latex = exp;
        if (exp.startsWith("$") || exp.startsWith("\"$") || exp.startsWith("'$")) {
            int n = 1;
            // We have $...$ or "$...$"
            if (exp.charAt(0) != '$') {
                n = 2;
            }
            latex = exp.substring(n, exp.length() - n);
        }

        viewer.icon = ScilabSpecialTextUtilities.compilePartialLaTeXExpression(latex, defaultSize);
        if (viewer.icon == null) {
            return 0;
        }

        width = viewer.icon.getIconWidth();
        height = viewer.icon.getIconHeight();
        viewer.setSize(width + 2 * INSET, height + 2 * INSET);
        tc.add(viewer);
        Rectangle begin;
        Rectangle end;
        try {
            begin = tc.modelToView(b);
            end = tc.modelToView(e);
        } catch (BadLocationException ex) {
            begin = NULLRECT;
            end = NULLRECT;
        }

        int abs;
        int ord;
        if (begin.y == end.y) {
            //We're on the same line
            abs = Math.max(1, (end.x + begin.x - width) / 2);
            ord = begin.y + begin.height + 1;
            if (height + ord > totalHeight) {
                ord = begin.y - 1 - height - 2 * INSET;
            }
        } else if (begin.y + begin.height == end.y) {
            //The line is drawn on two lines
            ord = end.y + end.height + 1;
            if (height + ord > totalHeight) {
                ord = begin.y - 1 - height - 2 * INSET;
                abs = begin.x;
            } else {
                abs = end.x;
            }
        } else {
            //Guess
            abs = Math.max(1, (tc.getWidth() - width) / 2);
            ord = end.y + end.height + 1;
            if (height + ord > totalHeight) {
                ord = begin.y - 1 - height - 2 * INSET;
            }
        }

        viewer.setLocation(abs, ord);
        viewer.setVisible(true);
        viewer.repaint();

        return height + 2 * INSET;
    }
}
