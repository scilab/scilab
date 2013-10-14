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
import java.awt.Container;
import java.awt.Font;
import java.util.Map;

import javax.swing.JTextArea;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

public class UITextarea extends UIComponent {

    private JTextArea area;

    public UITextarea(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        area = new JTextArea();

        return area;
    }

    @UIComponentAnnotation(attributes = {"text", "rows", "columns", "color", "font", "underline", "bold", "italic", "strike-through", "size", "weight"})
    public Object newInstance(String text, int rows, int columns, Color color, String font, boolean underline, boolean bold, boolean italic, boolean strikethrough, double size, UITools.FontWeight weight) {
        area = new JTextArea(rows == Integer.MAX_VALUE ? 0 : rows, columns == Integer.MAX_VALUE ? 0 : columns);
        area.setText(text);

        if (color != null) {
            area.setForeground(color);
        }

        if (font != null && !font.isEmpty()) {
            area.setFont(UITools.getFont(null, null, font, size, underline, bold, italic, strikethrough, weight));
        } else {
            area.setFont(UITools.getFont(null, null, area.getFont(), size, underline, bold, italic, strikethrough, weight));
        }
        return area;
    }

    public void setSelectAll(boolean all) {
        if (all) {
            area.selectAll();
        } else {
            area.select(0, 0);
        }
    }

    public void setText(String[] strs) {
        area.setText("");
        if (strs != null) {
            for (int i = 0; i < strs.length; i++) {
                area.append(strs[i]);
                if (i != strs.length - 1) {
                    area.append("\n");
                }
            }
        }
    }
}
