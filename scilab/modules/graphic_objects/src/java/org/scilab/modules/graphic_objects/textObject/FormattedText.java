/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
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
