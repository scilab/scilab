/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

package org.scilab.modules.commons.gui;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.event.FocusEvent;
import java.awt.event.MouseEvent;

import javax.swing.UIManager;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultCaret;
import javax.swing.text.JTextComponent;

/**
 * This class handles the tabulation
 * @author Calixte DENIZET
 */
public class ScilabCaret extends DefaultCaret {

    private static final long serialVersionUID = 8230195712653828841L;

    private static Color selectionColor;
    private static Color inactiveSelectionColor;

    private JTextComponent editor;

    private boolean overwriteMode;
    private boolean mustAdjustVisibility = true;

    /**
     * Constructor
     * @param editor the editor where the caret lives
     */
    public ScilabCaret(JTextComponent editor) {
        super();
        this.editor = editor;
        setSelectionColor(editor.getSelectionColor(), UIManager.getColor("TextComponent.selectionBackgroundInactive"));
    }

    /**
     * {@inheritDoc}
     */
    protected void adjustVisibility(Rectangle nloc) {
        if (mustAdjustVisibility) {
            super.adjustVisibility(nloc);
        }
    }

    public void setMustAdjustVisibility(boolean mustAdjustVisibility) {
        this.mustAdjustVisibility = mustAdjustVisibility;
    }

    public boolean getMustAdjustVisibility() {
        return this.mustAdjustVisibility;
    }

    /**
     * @param active the color of the selection when it is active
     * @param inactive the color of the selection when it is inactive
     */
    public void setSelectionColor(Color active, Color inactive) {
        selectionColor = active;
        inactiveSelectionColor = inactive;
        if (inactiveSelectionColor == null) {
            float r = 0.6f * selectionColor.getRed() / 255f + 0.4f * Color.LIGHT_GRAY.getRed() / 255f;
            float g = 0.6f * selectionColor.getGreen() / 255f + 0.4f * Color.LIGHT_GRAY.getGreen() / 255f;
            float b = 0.6f * selectionColor.getBlue() / 255f + 0.4f * Color.LIGHT_GRAY.getBlue() / 255f;
            inactiveSelectionColor = new Color(r, g, b);
        }
    }

    /**
     * @return the color of the selection
     */
    public Color getSelectionColor() {
        return selectionColor;
    }

    /**
     * @return the color of the inactive selection
     */
    public Color getInactiveSelectionColor() {
        return inactiveSelectionColor;
    }

    /**
     * {@inheritDoc}
     */
    public void setSelectionVisible(boolean vis) {
        super.setSelectionVisible(true);
    }

    /**
     * {@inheritDoc}
     */
    public void focusGained(FocusEvent e) {
        editor.setSelectionColor(selectionColor);
        editor.repaint();
        super.focusGained(e);
    }

    /**
     * {@inheritDoc}
     */
    public void focusLost(FocusEvent e) {
        editor.setSelectionColor(inactiveSelectionColor);
        editor.repaint();
        super.focusLost(e);
    }

    /**
     * {@inheritDoc}
     */
    public void mouseDragged(MouseEvent e) {
        editor.setSelectionColor(selectionColor);
        super.mouseDragged(e);
    }

    /**
     * @param overwriteMode if true, the caret will be drawn as solid rectangle
     */
    public void setOverwriteMode(boolean overwriteMode) {
        this.overwriteMode = overwriteMode;
    }

    /**
     * {@inheritDoc}
     */
    public void paint(Graphics g) {
        if (isVisible() && overwriteMode) {
            Rectangle r = null;
            int pos = editor.getCaretPosition();
            try {
                r = editor.modelToView(pos);
            } catch (BadLocationException e) { }
            if (r != null && (r.width != 0 || r.height != 0)) {
                if ( (x != r.x) || (y != r.y) ) {
                    repaint();
                    x = r.x;
                    y = r.y;
                    height = r.height;
                    width = editor.getFontMetrics(editor.getFont()).charWidth('W') + 1;
                } else {
                    g.setColor(editor.getCaretColor());
                    g.setXORMode(editor.getBackground());
                    g.fillRect(x, y, width, height);
                }
            }
        } else {
            super.paint(g);
        }
    }

    /**
     * {@inheritDoc}
     */
    protected synchronized void damage(Rectangle r) {
        if (overwriteMode) {
            if (r == null) {
                return;
            }

            x = r.x;
            y = r.y;
            height = r.height;
            repaint();
        } else {
            super.damage(r);
        }
    }
}
