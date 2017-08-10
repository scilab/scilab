/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
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

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.text.BadLocationException;
import javax.swing.text.Element;

/**
 * Class Comment Manager
 * @author Calixte DENIZET
 */
public class CommentManager {

    private static final String COM = "//";
    private static final char DEMICOM = '/';
    private static final String EOL = "\n";

    private ScilabDocument doc;
    private Element elem;

    /**
     * Constructor
     * @param doc the document to comment
     */
    public CommentManager(ScilabDocument doc) {
        this.doc = doc;
        this.elem = doc.getDefaultRootElement();
    }


    /**
     * Comment a line
     * @param line the line number
     */
    public void commentLine(int line) {
        Element startL = elem.getElement(line);
        int sstart = startL.getStartOffset();

        try {
            doc.insertString(sstart, COM, null);
        } catch (BadLocationException e) {
            e.printStackTrace();
            return;
        }
    }

    /**
     * Uncomment a line
     * @param line the line number
     * @return true if the line has been uncommented
     */
    public boolean uncommentLine(int line) {
        Element startL = elem.getElement(line);
        int sstart = startL.getStartOffset();
        int send = startL.getEndOffset() - 1;

        try {
            String str = doc.getText(sstart, send - sstart);
            Pattern pat = Pattern.compile("([ \t]*)//");
            Matcher matcher = pat.matcher(str);
            if (matcher.find()) {
                str = matcher.replaceFirst("$1");
                doc.replace(sstart, send - sstart, str, null);
                return true;
            }
        } catch (BadLocationException e) {
            e.printStackTrace();
        }

        return false;
    }

    /**
     * Comment several lines
     * @param start the beginning position in the doc
     * @param end the ending position in the doc
     * @return an array of length 2 containing the new positions
     */
    public int[] commentLines(int start, int end) {
        int first = elem.getElementIndex(start);
        int last = elem.getElementIndex(end);
        int[] ret = new int[] {start, end};

        for (int i = first; i <= last; i++) {
            commentLine(i);
            ret[1] += 2;
        }

        ret[0] += 2;

        return ret;
    }

    /**
     * Comment a part of a line
     * @param position in the doc
     */
    public void commentText(int position) {
        try {
            doc.insertString(position, COM, null);
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
    }

    /**
     * Un-Comment several lines
     * @param start the beginning position in the doc
     * @param end the ending position in the doc
     * @return an array of length 2 containing the new positions
     */
    public int[] uncommentLines(int start, int end) {
        int first = elem.getElementIndex(start);
        int last = elem.getElementIndex(end);
        int[] ret = new int[] {start, end};

        boolean unc = uncommentLine(first);
        if (unc) {
            ret[0] -= 2;
            ret[1] -= 2;
        }

        for (int i = first + 1; i <= last; i++) {
            unc = uncommentLine(i);
            if (unc) {
                ret[1] -= 2;
            }
        }

        return ret;
    }

    /**
     * UnComment a part of a line
     * @param start position start
     */
    public void uncommentText(int start) {
        try {
            int end = elem.getElement(elem.getElementIndex(start)).getEndOffset();
            if (end - start >= 2 && COM.equals(doc.getText(start, 2))) {
                doc.remove(start, 2);
            }
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
    }
}
