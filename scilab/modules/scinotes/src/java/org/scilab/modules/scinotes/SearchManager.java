/* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.text.BadLocationException;
import javax.swing.text.Document;

/**
 * Class SearchManager
 * @author Sylvestre Koumar
 * @author Calixte DENIZET
 */
public class SearchManager {

    /**
     * FIND AND REPLACE START
     * @param scilabDocument document
     * @param word string
     * @param start int
     * @param end int
     * @param caseSensitive boolean
     * @param wholeWord boolean
     * @param useRegexp boolean
     * @return List
     */
    public static List<Integer[]> findWord(Document scilabDocument, String word,
                                           int start, int end,
                                           boolean caseSensitive, boolean wholeWord, boolean useRegexp) {
        String fullText = "";
        try {
            fullText = scilabDocument.getText(start, end - start + 1);
        } catch (BadLocationException ex) {
            ex.printStackTrace();
        }

        if (fullText.length() == 0) {
            return null;
        }

        List<Integer[]> offsetList = new ArrayList<Integer[]>();

        //If we don't give any word to find
        if (word != null && !word.equals("")) {
            Pattern pattern = generatePattern(word, caseSensitive, wholeWord, useRegexp);
            Matcher matcher = pattern.matcher(fullText);

            while (matcher.find()) {
                if (matcher.start() != matcher.end()) {
                    offsetList.add(new Integer[] {matcher.start() + start, matcher.end() + start});
                }
            }
        }
        return offsetList;
    }

    /**
     * Generate the good pattern according to the differents boolean
     * @param exp the searched expression
     * @param caseSensitive boolean
     * @param wholeWord boolean
     * @param useRegexp boolean
     * @return the pattern
     */
    public static Pattern generatePattern(String exp, boolean caseSensitive, boolean wholeWord, boolean useRegexp) {
        String word = exp;
        if (word != null && !word.equals("")) {
            if (!useRegexp) {
                word = "\\Q" + word + "\\E";
                if (wholeWord) {
                    word = "\\b" + word + "\\b";
                }
            }

            if (!caseSensitive) {
                word = "(?i)" + word;
            }

            if (useRegexp) {
                word = "(?m)" + word;
            }

            return Pattern.compile(word);
        } else {
            return Pattern.compile("");
        }
    }

}
