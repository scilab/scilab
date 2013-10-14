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
import java.awt.event.ActionEvent;
import java.text.DecimalFormat;
import java.util.Map;

import javax.swing.AbstractAction;
import javax.swing.JFormattedTextField;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.gui.uiwidget.UIWidgetTools;
import org.scilab.modules.gui.uiwidget.callback.UICallback;

/**
 * JTextField wrapper
 */
public class UITextfield extends UIComponent {

    private JTextField textfield;
    private boolean password;
    private DocumentListener listener;
    private boolean onchangeEnable = true;
    private UICallback onchangeAction;
    private boolean onenterEnable = true;
    private UICallback onenterAction;

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

        public static String getAsString(int value) {
            switch (value) {
                case JTextField.LEADING:
                    return "leading";
                case JTextField.CENTER:
                    return "center";
                case JTextField.LEFT:
                    return "left";
                case JTextField.RIGHT:
                    return "right";
                case JTextField.TRAILING:
                    return "trailing";
                default:
                    return "leading";
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public UITextfield(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        textfield = new JTextField();

        return textfield;
    }

    @UIComponentAnnotation(attributes = {"text", "columns", "color", "font", "underline", "bold", "italic", "strike-through", "size", "weight", "password", "format"})
    public Object newInstance(String text, int columns, Color color, String font, boolean underline, boolean bold, boolean italic, boolean strikethrough, double size, UITools.FontWeight weight, boolean password, DecimalFormat format) {
        this.password = password;
        if (format == null) {
            if (password) {
                textfield = new JPasswordField(columns == Integer.MAX_VALUE ? 0 : columns);
            } else {
                textfield = new JTextField(columns == Integer.MAX_VALUE ? 0 : columns);
            }
        } else {
            textfield = new JFormattedTextField(format);
            textfield.setColumns(columns == Integer.MAX_VALUE ? 0 : columns);
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

    /**
     * Set the alignment
     * @param a the alignment
     */
    public void setAlignment(Alignment a) {
        textfield.setHorizontalAlignment(a.value());
    }

    /**
     * Get the alignment
     * @return he alignment
     */
    public String getAlignment() {
        return Alignment.getAsString(textfield.getHorizontalAlignment());
    }

    /**
     * Set the alignment
     * @param a the alignment
     */
    public void setHorizontalAlignment(Alignment alignment) {
        textfield.setHorizontalAlignment(alignment.value());
    }

    /**
     * Get the alignment
     * @return he alignment
     */
    public String getHorizontalAlignment() {
        return Alignment.getAsString(textfield.getHorizontalAlignment());
    }

    public void setSelectAll(boolean all) {
        if (all) {
            textfield.selectAll();
        } else {
            textfield.select(0, 0);
        }
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

    public void setString(String s) {
        textfield.setText(s);
    }

    public String getString() {
        return textfield.getText();
    }

    public void removeDocumentListener() {
        if (listener != null) {
            textfield.getDocument().removeDocumentListener(listener);
            listener = null;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void remove() {
        removeDocumentListener();
        textfield.setAction(null);
        super.remove();
    }

    public UICallback getOnchange() {
        return onchangeAction;
    }

    public void setOnchange(final String onchangeAction) {
        if (this.onchangeAction == null) {
            removeDocumentListener();
            listener = new DocumentListener() {
                public void insertUpdate(DocumentEvent e) {
                    if (onchangeEnable) {
                        UIWidgetTools.execAction(UITextfield.this.onchangeAction);
                    }
                }

                public void removeUpdate(DocumentEvent e) {
                    if (onchangeEnable) {
                        UIWidgetTools.execAction(UITextfield.this.onchangeAction);
                    }
                }

                public void changedUpdate(DocumentEvent e) { }
            };
            textfield.getDocument().addDocumentListener(listener);
        }
        this.onchangeAction = UICallback.newInstance(this, onchangeAction);
    }

    public boolean getOnchangeEnable() {
        return onchangeEnable;
    }

    public void setOnchangeEnable(boolean b) {
        onchangeEnable = b;
    }

    public UICallback getOnenter() {
        return onenterAction;
    }

    public void setOnenter(final String onenterAction) {
        if (this.onenterAction == null) {
            textfield.setAction(new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    if (onenterEnable) {
                        UIWidgetTools.execAction(UITextfield.this.onenterAction);
                    }
                }
            });
        }
        this.onenterAction = UICallback.newInstance(this, onenterAction);
    }

    public boolean getOnenterEnable() {
        return onenterEnable;
    }

    public void setOnenterEnable(boolean b) {
        onenterEnable = b;
    }
}
