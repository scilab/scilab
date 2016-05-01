/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.scinotes;

import javax.swing.text.BadLocationException;
import javax.swing.text.Element;

/**
 * This class handles the tabulation
 * @author Calixte DENIZET
 */
public class TrailingWhiteManager {

    private static final String REGEXP_WHITES_AT_END = "[ \t]+$";
    private static final String REGEXP_WHITES_AT_EOL = "[ \t]+\n";

    private ScilabDocument doc;
    private Element elem;

    /**
     * The constructor
     * @param doc the document
     */
    public TrailingWhiteManager(ScilabDocument doc) {
        this.doc = doc;
        this.elem = doc.getDefaultRootElement();
    }

    /**
     * Remove the trailing blanks at the end of the line containing position
     * @param position the position in the doc
     */
    public int removeTrailingWhite(int position) {
        int nline = elem.getElementIndex(position);
        Element line = elem.getElement(nline);
        int start = line.getStartOffset();
        int end = line.getEndOffset() - 1;

        try {
            String str = doc.getText(start, end - start);
            str = str.replaceFirst(REGEXP_WHITES_AT_END, "");
            doc.replace(start, end - start, str, null);
            return Math.min(position, start + str.length());
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
        return position;
    }

    /**
     * Remove the blanks at the end of each lines
     * @param start the starting position in the doc
     * @param end the ending position in the doc
     * @return the new positions of the selected text
     */
    public int[] removeTrailingWhite(int start, int end) {
        int nfirst = elem.getElementIndex(start);
        int nlast = elem.getElementIndex(end);
        int sstart = elem.getElement(nfirst).getStartOffset();
        int eend = elem.getElement(nlast).getEndOffset() - 1;
        int[] ret = new int[2];
        ret[0] = start;

        try {
            String str = doc.getText(sstart, eend - sstart);
            str = str.replaceAll(REGEXP_WHITES_AT_EOL, "\n");
            int len = str.length();
            str = str.replaceFirst(REGEXP_WHITES_AT_END, "");
            len = len - str.length();

            ret[1] = sstart + str.length() - (eend - end - len - 1);

            doc.replace(sstart, eend - sstart, str, null);
        } catch (BadLocationException e) {
            e.printStackTrace();
            return null;
        }

        return ret;
    }
}
