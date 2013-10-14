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

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

/**
 * Text data
 */
public class TextData extends UIComponent {

    private String text = "";

    /**
     * {@inheritDoc}
     */
    public TextData(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        return text;
    }

    /**
     * Add text
     * @param text the text to add
     */
    public void addText(String text) {
        this.text = trim(text);
    }

    /**
     * Get the text
     * @return the text
     */
    public String getText() {
        return text;
    }

    /**
     * Trim the string
     * @param str the string
     * @return the trimed string
     */
    private static String trim(String str) {
        final char[] chars = str.toCharArray();
        int len = chars.length;
        StringBuilder buffer = new StringBuilder(len);
        int i = 0;
        int start;
        int end;
        int startMotif;
        int lenMotif;
        boolean newLine = false;

        while (i < len && (chars[i] == '\n' || chars[i] == '\r')) {
            i++;
        }
        startMotif = i;

        while (i < len && (chars[i] == ' ' || chars[i] == '\t')) {
            i++;
        }

        if (i == len) {
            return "";
        }

        start = i;
        lenMotif = i - startMotif;

        i = len - 1;
        while (i >= 0 && (chars[i] == '\n' || chars[i] == '\r')) {
            i--;
        }
        end = i;

        for (i = start; i <= end; i++) {
            if (chars[i] == '\r') {
                if (i < end && chars[i + 1] == '\n') {
                    buffer.append('\n');
                    i++;
                }
                newLine = true;
            } else if (chars[i] == '\n') {
                buffer.append('\n');
                newLine = true;
            }

            if (newLine) {
                int j = 0;
                for (; j < lenMotif; j++) {
                    if ((i + j < end) && chars[j + startMotif] != chars[i + 1 + j]) {
                        break;
                    }
                }
                if (j == lenMotif) {
                    i += lenMotif;
                } else {
                    buffer.append(chars[i]);
                }
                newLine = false;
            } else {
                buffer.append(chars[i]);
            }
        }

        return buffer.toString();
    }
}
