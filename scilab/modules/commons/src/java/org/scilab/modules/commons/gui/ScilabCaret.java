/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons.gui;

import java.awt.Color;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.MouseEvent;

import javax.swing.text.JTextComponent;
import javax.swing.UIManager;
import javax.swing.text.DefaultCaret;

/**
 * This class handles the tabulation
 * @author Calixte DENIZET
 */
public class ScilabCaret extends DefaultCaret {

    private static final long serialVersionUID = 8230195712653828841L;

    private static Color selectionColor;
    private static Color inactiveSelectionColor;

    private JTextComponent editor;

    /**
     * Constructor
     * @param editor the editor where the caret lives
     */
    public ScilabCaret(JTextComponent editor) {
        super();
        this.editor = editor;
        selectionColor = editor.getSelectionColor();
        inactiveSelectionColor = UIManager.getColor("TextComponent.selectionBackgroundInactive");
        if (inactiveSelectionColor == null) {
            float r = 0.6f * selectionColor.getRed() / 255f + 0.4f * Color.LIGHT_GRAY.getRed() / 255f;
            float g = 0.6f * selectionColor.getGreen() / 255f + 0.4f * Color.LIGHT_GRAY.getGreen() / 255f;
            float b = 0.6f * selectionColor.getBlue() / 255f + 0.4f * Color.LIGHT_GRAY.getBlue() / 255f;
            inactiveSelectionColor = new Color(r, g, b);
        }
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
        super.focusGained(e);
    }

    /**
     * {@inheritDoc}
     */
    public void focusLost(FocusEvent e) {
        editor.setSelectionColor(inactiveSelectionColor);
        super.focusLost(e);
    }

    /**
     * {@inheritDoc}
     */
    public void mouseDragged(MouseEvent e) {
        editor.setSelectionColor(selectionColor);
        super.mouseDragged(e);
    }
}
