/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

import org.scilab.modules.console.ConsoleOptions;

/**
 * Class to have a preview of a LaTeX string
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
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
        setDefaultSize(ConsoleOptions.getLaTeXFont().size);
    }

    public static void configurationChanged(org.scilab.modules.console.ConsoleConfiguration.Conf conf) {
        if (conf.latex) {
            setDefaultSize(ConsoleOptions.getLaTeXFont().size);
        }
    }

    /**
     * Set the default size to render LaTeX
     * @param size the font size
     */
    public static void setDefaultSize(int size) {
        ScilabLaTeXViewer.defaultSize = size;
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
        if (ScilabLaTeXViewer.icon != null) {
            ScilabLaTeXViewer.icon = null;
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

        ScilabLaTeXViewer.icon = ScilabSpecialTextUtilities.compilePartialLaTeXExpression(latex, defaultSize);
        if (ScilabLaTeXViewer.icon == null) {
            return 0;
        }

        width = ScilabLaTeXViewer.icon.getIconWidth();
        height = ScilabLaTeXViewer.icon.getIconHeight();
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
