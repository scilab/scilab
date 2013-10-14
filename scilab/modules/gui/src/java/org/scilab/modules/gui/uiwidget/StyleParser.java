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

package org.scilab.modules.gui.uiwidget;

import java.util.Map;

/**
 * Access to the CSS style parser
 */
public final class StyleParser {

    /**
     * Parse a CSS line
     * @param str the line
     * @return CSS properties as a Map
     */
    public static final Map<String, String> parseLine(String str) {
        try {
            return CSSParser.parseLine(str);
        } catch (CSSParserException e) {
            System.err.println(e);
            return null;
        }
    }

    /**
     * Parse a CSS block
     * @param str the block
     * @return CSS properties as a Map
     */
    public static final Map<String, Map<String, String>> parseBlock(String str) {
        try {
            return CSSParser.parseBlock(str);
        } catch (CSSParserException e) {
            System.err.println(e);
            return null;
        }
    }

    /**
     * Parse a CSS file
     * @param file the file name
     * @return CSS properties as a Map
     */
    public static final Map<String, Map<String, String>> parseFile(String file) {
        try {
            return CSSParser.parseFile(file);
        } catch (CSSParserException e) {
            e.printStackTrace();
            System.err.println(e);
            return null;
        }
    }
}
