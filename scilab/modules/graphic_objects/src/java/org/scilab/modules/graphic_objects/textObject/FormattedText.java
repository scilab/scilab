/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2024 - UTC - Stéphane MOTTELET
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
    public enum FormattedTextProperty { TEXT, FONT, INTERPRETER };

    /** Text */
    private String text;

    /** Font */
    private Font font;

    /** interpreter */
    public enum InterpreterType {
        AUTO, LATEX, MATHML, NONE;
        public static InterpreterType intToEnum(Integer value) {
            switch (value) {
                default:
                case 0:
                    return AUTO;
                case 1:
                    return LATEX;
                case 2:
                    return MATHML;
                case 3:
                    return NONE;
            }
        }

        public static InterpreterType stringToEnum(String value) {
            if (value.equals("latex")) {
                return LATEX;
            }

            if (value.equals("mathml")) {
                return MATHML;
            }

            if (value.equals("none")) {
                return NONE;
            }

            return AUTO;
        }

        public static String enumToString(InterpreterType value) {
            switch (value) {
                case MATHML:
                    return "mathml";
                case LATEX:
                    return "latex";
                case NONE:
                    return "none";
                default:
                case AUTO:
                    return "auto";
            }
        }
    }

    private InterpreterType interpreter = InterpreterType.AUTO;

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

    @Override
    public int hashCode() {
        int result = text != null ? text.hashCode() : 0;
        result = 31 * result + (font != null ? font.hashCode() : 0);
        return result;
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

    public Integer getInterpreter() {
        return interpreter.ordinal();
    }

    public InterpreterType getInterpreterAsEnum() {
        return interpreter;
    }

    public UpdateStatus setInterpreter(Integer value) {
        return setInterpreter(InterpreterType.intToEnum(value));
    }

    public UpdateStatus setInterpreter(InterpreterType value) {
        if (interpreter == value) {
            return UpdateStatus.NoChange;
        }

        interpreter = value;
        return UpdateStatus.Success;
    }

}
