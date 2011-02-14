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

package org.scilab.modules.scinotes.utils;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Rectangle;

import javax.swing.Icon;
import javax.swing.JPanel;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.utils.ScilabSpecialTextUtilities;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.SciNotes;

/**
 * Class to have a preview of a LaTeX string
 * @author Calixte DENIZET
 */
public final class SciNotesLaTeXViewer extends JPanel {

    private static final int INSET = 3;
    private static final Rectangle NULLRECT = new Rectangle(0, 0, 0, 0);

    private static int defaultSize = 15;
    private static Icon icon;
    private static int width;
    private static int height;
    private static SciNotesLaTeXViewer viewer = new SciNotesLaTeXViewer();

    /**
     * Default constructor
     */
    private SciNotesLaTeXViewer() {
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
        super.paint(g);
        g.setColor(Color.WHITE);
        g.fillRect(0, 0, width + 2 * INSET, height + 2 * INSET);
        g.setColor(Color.LIGHT_GRAY);
        g.drawRect(0, 0, width + 2 * INSET - 1, height + 2 * INSET - 1);
        icon.paintIcon(this, g, INSET, INSET);
    }

    /**
     * @param sep the pane where to remove the LaTeXViewer
     */
    public static void removeLaTeXViewer(ScilabEditorPane sep) {
        if (viewer.icon != null) {
            viewer.icon = null;
            viewer.setVisible(false);
            sep.remove(viewer);
            sep.repaint();
        }
    }

    /**
     * @param sep the pane where to display the LaTeX
     * @param exp the expression
     * @param b the beginning of the expression
     * @param e the end of the expression
     */
    public static void displayExpression(ScilabEditorPane sep, String exp, int b, int e) {
        int n = 1;
        // We have $...$ or "$...$"
        if (exp.charAt(0) != '$') {
            n = 2;
        }
        viewer.icon = ScilabSpecialTextUtilities.compileLaTeXExpression(exp.substring(n, exp.length() - n), defaultSize);
        if (viewer.icon == null) {
            viewer.icon = ScilabSpecialTextUtilities.compileLaTeXExpression("\\text{Error in \\LaTeX expression}", defaultSize);
        }

        width = viewer.icon.getIconWidth();
        height = viewer.icon.getIconHeight();
        viewer.setSize(width + 2 * INSET, height + 2 * INSET);
        sep.add(viewer);
        Rectangle begin;
        Rectangle end;
        try {
            begin = sep.modelToView(b);
            end = sep.modelToView(e);
        } catch (BadLocationException ex) {
            begin = NULLRECT;
            end = NULLRECT;
        }

        int scrollValue = sep.getScrollPane().getVerticalScrollBar().getValue();
        int scrollHeight = sep.getScrollPane().getHeight();
        int abs;
        int ord;
        if (begin.y == end.y) {
            //We're on the same line
            abs = Math.max(1, (end.x + begin.x - width) / 2);
            ord = begin.y + begin.height + 1;
            if (height + ord > scrollValue + scrollHeight) {
                ord = begin.y - 1 - height;
            }
        } else if (begin.y + begin.height == end.y) {
            //The line is drawn on two lines
            ord = end.y + end.height + 1;
            if (height + ord > scrollValue + scrollHeight) {
                ord = begin.y - 1 - height;
                abs = begin.x;
            } else {
                abs = end.x;
            }
        } else {
            //Guess
            abs = Math.max(1, (sep.getWidth() - width) / 2);
            ord = end.y + end.height + 1;
            if (height + ord > scrollValue + scrollHeight) {
                ord = begin.y - 1 - height;
            }
        }

        viewer.setLocation(abs, ord);
        viewer.setVisible(true);
        viewer.repaint();
    }
}
