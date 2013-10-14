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

import javax.swing.JLabel;
import javax.swing.JTextPane;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

/**
 * Wrapper for a multiline label (in fact a JTextPane)
 */
public class UIMultiLineLabel extends UIComponent {

    private final static int defaultAlignment = new JLabel().getHorizontalAlignment();
    private final static Color defaultFg = new JLabel().getForeground();

    private JTextPane label;

    /**
     * {@inheritDoc}
     */
    public UIMultiLineLabel(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        label = new JTextPane();

        return label;
    }

    @UIComponentAnnotation(attributes = {"text"})
    public Object newInstance(String text) {
        label = new JTextPane();
        label.setOpaque(false);
        label.setForeground(defaultFg);
        label.removeMouseListener(label.getMouseListeners()[0]);
        label.removeFocusListener(label.getFocusListeners()[0]);
        label.setEditable(false);
        label.setHighlighter(null);
        label.setCaret(null);
        label.setDragEnabled(false);
        label.setText(text);

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
    public void setString(String[] str) {
        setText(str);
    }

    /**
     * Alias for getText
     */
    public String[] getString() {
        return getText();
    }

    /**
     * Set the multiline text
     * @param str array of line
     */
    public void setText(String[] str) {
        if (str != null && str.length != 0) {
            StringBuilder buffer = new StringBuilder();
            for (int i = 0; i < str.length - 1; i++) {
                buffer.append(str[i]).append("\n");
            }
            buffer.append(str[str.length - 1]);
            label.setText(buffer.toString());
        } else {
            label.setText("");
        }
    }

    /**
     * Get the multiline text
     * @return the array of line
     */
    public String[] getText() {
        String str = label.getText();
        if (str != null) {
            String[] s = str.split("[\n]");
            return s;
        }

        return null;
    }
}
