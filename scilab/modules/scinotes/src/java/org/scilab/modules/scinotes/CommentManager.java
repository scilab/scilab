/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
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
     * Comment several lines
     * @param start the beginning position in the doc
     * @param end the ending position in the doc
     * @return an array of length 2 containing the new positions
     */
    public int[] commentLines(int start, int end) {
        Element startL = elem.getElement(elem.getElementIndex(start));
        int sstart = startL.getStartOffset();
        int[] ret = new int[]{0, 0};
        int send = end;

        try {
            String str = doc.getText(sstart, send - sstart + 1);
            if (str.charAt(str.length() - 1) == EOL.charAt(0)) {
                send--;
                str = str.substring(0, str.length() - 1);
                ret[1]++;
            }
            str = COM + str.replaceAll(EOL, EOL + COM);
            ret[0] = start + 2;
            ret[1] += sstart + str.length();
            doc.replace(sstart, send - sstart + 1, str, null);
            return ret;
        } catch (BadLocationException e) {
            e.printStackTrace();
            return null;
        }
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
        int sstart = elem.getElement(elem.getElementIndex(start)).getStartOffset();
        int[] ret = new int[2];
        ret[0] = start;

        try {
            String str = doc.getText(sstart, end - sstart + 1);

            str = str.replaceAll(EOL + COM, EOL);

            if (str.charAt(0) == DEMICOM && str.charAt(1) == DEMICOM) {
                str = str.substring(2);
                if (start - sstart >= 2) {
                    ret[0] -= 2;
                }
            }
            ret[1] = sstart + str.length();

            doc.replace(sstart, end - sstart + 1, str, null);
        } catch (BadLocationException e) {
            e.printStackTrace();
            return null;
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
