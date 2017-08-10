/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.graph.view;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.scilab.modules.graph.utils.ScilabGraphUtils;
import org.scilab.modules.jvm.LoadClassPath;

import com.mxgraph.util.mxUtils;

/**
 * Specify the supported label type
 */
public enum SupportedLabelType {
    /** The label is HTML formatted. This is the default */
    HTML,
    /** The label is Latex formatted */
    Latex,
    /** The label is MathML formatted */
    MathML;

    /**
     * Pattern to match the &lt;br&gt; HTML tag
     */
    private static final Pattern BR_PATTERN = Pattern.compile("<br>\\p{Blank}*");

    /** The '$' symbol is used to tag a LaTeX expression */
    private static final char LATEX_TAG = '$';
    /** The '^' symbol is used to tag a LaTeX expression */
    private static final char MATHML_TAG = '^';
    /** The id used on classpath.xml to load MathML JARs */
    private static final String CLASSPATH_MATHML_NAME = "xcos_mathml_rendering";

    /**
     * Get the {@link SupportedLabelType} for the label.
     *
     * The type of the label is found with a specific first character.
     * <ul>
     * <li>'$' : for {@value SupportedLabelType#Latex} markup</li>
     * <li>'^' : for {@value SupportedLabelType#MathML} markup</li>
     * </ul>
     *
     * The {@value SupportedLabelType#HTML} is used as default.
     *
     * @param text The label to parse
     * @return the type of the label
     */
    public static SupportedLabelType getFromText(String text) {
        if (text.length() > 0) {
            if ((text.charAt(0) == LATEX_TAG) && (
                text.charAt(text.length() - 1) == LATEX_TAG)) {
                return Latex;
            } else if ((text.charAt(0) == MATHML_TAG) && (
                text.charAt(text.length() - 1) == MATHML_TAG)) {
                LoadClassPath.loadOnUse(CLASSPATH_MATHML_NAME);
                return MathML;
            }
        }
        return HTML;
    }

    /**
     * Get the {@link SupportedLabelType} for the HTML formatted string.
     *
     * The type of the label is found with a specific first character of the
     * content. The HTML markup is escaped automatically.
     * <ul>
     * <li>'$' : for {@value SupportedLabelType#Latex} markup</li>
     * <li>'^' : for {@value SupportedLabelType#MathML} markup</li>
     * </ul>
     *
     * The {@value SupportedLabelType#HTML} is used as default.
     *
     * @param html The HTML string to parse
     * @return the type of the label
     */
    public static SupportedLabelType getFromHTML(String html) {
        if (html.length() > 0 && html.charAt(0) == '<') {

            final StringBuilder content = new StringBuilder(mxUtils
                    .getBodyMarkup(html, false));

            ScilabGraphUtils.removeBlanks(content);

            if ((content.length() > 0) && (content.charAt(0) == LATEX_TAG)
                    && (content.charAt(content.length() - 1) == LATEX_TAG)) {
                return Latex;
            } else if ((content.length() > 0)
                       && (content.charAt(0) == MATHML_TAG)
                       && (content.charAt(content.length() - 1) == MATHML_TAG)) {
                LoadClassPath.loadOnUse(CLASSPATH_MATHML_NAME);
                return MathML;
            }
        } else {
            return getFromText(html);
        }

        return HTML;
    }

    /**
     * Escape the text for the current type.
     * @param text the text to escape
     * @return the escaped text
     */
    public String escape(String text) {
        StringBuilder escapedText;

        if (text.startsWith("<html")) {
            escapedText = new StringBuilder(ScilabGraphUtils.getBodyMarkup(text, true));
        } else {
            escapedText = new StringBuilder(text);
        }

        switch (this) {
            case MathML:
            case Latex:
                // Unescape content
                ScilabGraphUtils.unescape(escapedText, 0);

                // Removing <br>
                Matcher m = BR_PATTERN.matcher(escapedText);
                while (m.find()) {
                    escapedText.replace(m.start(), m.end(), "");
                    m.reset();
                }

                // Removing blank
                ScilabGraphUtils.removeBlanks(escapedText);

                // Removing the first and last char (tag)
                escapedText.delete(0, 1);
                int length = escapedText.length();
                escapedText.delete(length - 1, length);
                break;

            default:
                break;
        }

        return escapedText.toString();
    }
}
