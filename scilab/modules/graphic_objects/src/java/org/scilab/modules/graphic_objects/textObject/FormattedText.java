/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.textObject;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * FormattedText class
 * @author Manuel JULIACHS
 */
public class FormattedText {
    /** FormattedText properties names */
    public enum FormattedTextProperty { TEXT, FONT };

    /** Text */
    private String text;

    /** Font */
    private Font font;

    /** Constructor */
    public FormattedText() {
        text = "";
        font = new Font();
    }

    /** Constructor */
    public FormattedText(String text, Font font) {
        this.text = text == null ? "" : text;
        this.font = font;
    }

    /**
     * Copy constructor
     * @param formText the formatted text to copy
     */
    public FormattedText(FormattedText formText) {
        this.text = new String(formText.getText());
        this.font = new Font(formText.getFont());
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof FormattedText) {
            FormattedText ft = (FormattedText) o;
            return ft.text.equals(text) && ft.font.equals(font);
        }

        return false;
    }

    /**
     * @return the font
     */
    public Font getFont() {
        return font;
    }

    /**
     * @param font the font to set
     */
    public UpdateStatus setFont(Font font) {
        this.font = font;
        return UpdateStatus.Success;
    }

    /**
     * @return the text
     */
    public String getText() {
        return text;
    }

    /**
     * @param text the text to set
     */
    public UpdateStatus setText(String text) {
        this.text = text == null ? "" : text;
        return UpdateStatus.Success;
    }

}
