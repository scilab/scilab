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

import javax.swing.Action;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;

public class UITextfield extends UIComponent {

    private JTextField textfield;
    private boolean password;
    private DocumentListener listener;
    private String action;

    public enum Alignment {
        LEADING (JTextField.LEADING),
        CENTER (JTextField.CENTER),
        LEFT (JTextField.LEFT),
        RIGHT (JTextField.RIGHT),
        TRAILING (JTextField.TRAILING);

        private final int value;

        Alignment(int value) {
            this.value = value;
        }

        public int value() {
            return value;
        }
    }

    public UITextfield(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        textfield = new JTextField();

        return textfield;
    }

    @UIComponentAnnotation(attributes = {"text", "columns", "color", "font", "underline", "bold", "italic", "strike-through", "size", "weight", "password"})
    public Object newInstance(String text, int columns, Color color, String font, boolean underline, boolean bold, boolean italic, boolean strikethrough, double size, UITools.FontWeight weight, boolean password) {
        this.password = password;
        if (password) {
            textfield = new JPasswordField(columns == Integer.MAX_VALUE ? 0 : columns);
        } else {
            textfield = new JTextField(columns == Integer.MAX_VALUE ? 0 : columns);
        }

        textfield.setText(text);

        if (color != null) {
            textfield.setForeground(color);
        }

        if (font != null && !font.isEmpty()) {
            textfield.setFont(UITools.getFont(null, null, font, size, underline, bold, italic, strikethrough, weight));
        } else {
            textfield.setFont(UITools.getFont(null, null, textfield.getFont(), size, underline, bold, italic, strikethrough, weight));
        }

        return textfield;
    }

    public void setSelectAll(boolean all) {
        if (all) {
            textfield.selectAll();
        } else {
            textfield.select(0, 0);
        }
    }

    public void setHorizontalAlignment(Alignment alignment) {
        textfield.setHorizontalAlignment(alignment.value());
    }

    public void setOnenter(Action action) {
        textfield.setAction(action);
    }

    public String getPassword() {
        if (password) {
            return new String(((JPasswordField) textfield).getPassword());
        } else {
            return textfield.getText();
        }
    }

    public void setEchoChar(String s) {
        if (password && s != null && !s.isEmpty()) {
            ((JPasswordField) textfield).setEchoChar(s.charAt(0));
        }
    }

    public void removeListener() {
        if (listener != null) {
            textfield.getDocument().removeDocumentListener(listener);
            listener = null;
        }
    }

    public void remove() {
        removeListener();
        super.remove();
    }

    public void setOnchange(final String action) {
        if (this.action == null) {
            removeListener();
            listener = new DocumentListener() {
                public void insertUpdate(DocumentEvent e) {
                    UIWidgetTools.execAction(UITextfield.this, UITextfield.this.action);
                }

                public void removeUpdate(DocumentEvent e) {
                    UIWidgetTools.execAction(UITextfield.this, UITextfield.this.action);
                }

                public void changedUpdate(DocumentEvent e) { }
            };
            textfield.getDocument().addDocumentListener(listener);
        }
        this.action = action;
    }
}
