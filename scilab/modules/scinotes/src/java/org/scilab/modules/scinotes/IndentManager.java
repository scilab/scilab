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

import javax.swing.text.Element;
import javax.swing.text.Segment;
import javax.swing.text.BadLocationException;

/**
 * This class handle the indentation level
 * @author Calixte DENIZET
 */
public class IndentManager {

    private ScilabDocument doc;
    private Element elem;
    private IndentScanner scanner;
    private char indentChar = ' ';
    private int num = 4;
    private boolean isPsychoRigid = true;

    /**
     * Constructor.
     * @param doc the doc to indent
     */
    public IndentManager(ScilabDocument doc) {
        this.doc = doc;
        this.elem = doc.getDefaultRootElement();
        scanner = new IndentScanner(doc);
    }

    /**
     * Set the character to use to indent and if the indentChar is not '\t', then
     * num represents the number of whites to indent. If the variable isPsychoRigid
     * is true, then the indent level is strict and the indentation of a line doesn't
     * depend of the difference the two previous. For example (with isPsychoRigid false):
     * if a == 1 then
     * -----if b == 1 then
     *      -----c = 1 ...
     *
     * If isPsychoRigid is true and num equals to 3 :
     * if a == 1 then
     * -------if b == 1 then
     *        ---c = 1 ...
     *
     * The user decided to increase the indent but the PsychoRigid mode says that the
     * indent level is 3.
     *
     * @param indentChar the char use to indent
     * @param num the number of chars use to indent
     * @param isPsychoRigid for the psycho-rigid mode
     */
    public void setProperties(char indentChar, int num, boolean isPsychoRigid) {
        this.indentChar = indentChar;
        if (indentChar != '\t') {
            this.num = num;
        } else {
            this.num = 1;
        }
        this.isPsychoRigid = isPsychoRigid;
    }

    /**
     * The same as setProperties in keeping the same psycho-rigid mode
     * @param indentChar the char use to indent
     * @param num the number of chars use to indent
     */
    public void setProperties(char indentChar, int num) {
        setProperties(indentChar, num, isPsychoRigid);
    }

    /**
     * Set the psycho-rigid mode or not
     * @param b the mode
     */
    public void setPsychoRigidMode(boolean b) {
        this.isPsychoRigid = b;
    }

    /**
     * Indent the lines of this doc containing start and end.
     * @param start the start
     * @param end the end
     * @return an array of length 2 containing the new start and the new end of the indented text
     */
    public int[] indentDoc(int start, int end) {
        try {
            if (start > end ) {
                end = start;
            }
            int[] level = new int[2];
            int lineStart = elem.getElementIndex(start);
            int lineEnd = elem.getElementIndex(end);
            int[] tabs = new int[lineEnd - lineStart + 1];
            int[] ret = new int[2];

            int[] ind = new int[2];

            getNums(lineStart - 1, ind);
            if (lineStart > 0) {
                scanner.getIndentLevel(elem.getElement(lineStart - 1).getStartOffset(), level);
                tabs[0] = ind[1] + level[1] * num;
            }

            for (int lineNumber = 0; lineNumber <= lineEnd - lineStart; lineNumber++) {
                int pos = elem.getElement(lineNumber + lineStart).getEndOffset() - 1;
                scanner.getIndentLevel(pos, level);
                tabs[lineNumber] = Math.max(tabs[lineNumber] - level[0] * num, 0);
                if (lineNumber != lineEnd - lineStart) {
                    tabs[lineNumber + 1] = tabs[lineNumber] + level[1] * num;
                }
            }

            Segment seg = new Segment();
            StringBuffer buffer = new StringBuffer();
            int e = start;

            for (int lineNumber = lineStart; lineNumber <= lineEnd; lineNumber++) {
                if (lineNumber < lineEnd) {
                    e = elem.getElement(lineNumber).getEndOffset();
                } else {
                    e = elem.getElement(lineNumber).getEndOffset() - 1;
                }
                int t = elem.getElement(lineNumber).getStartOffset() + scanner.getTabsAtBeginning(lineNumber);
                if (e - t + 1 == 0) {
                    e = t;
                }
                doc.getText(t, e - t, seg);
                ret[0] = start;
                if (e > t + 1) {
                    char[] str = new char[tabs[lineNumber - lineStart]];
                    for (int i = 0; i < str.length; i++) {
                        str[i] = indentChar;
                    }
                    buffer.append(str);
                    if (lineNumber == lineStart) {
                        ret[0] = start + str.length - scanner.getTabsAtBeginning(lineNumber);
                    }
                }
                buffer.append(seg.array, seg.offset, seg.count);
            }

            int sstart = elem.getElement(lineStart).getStartOffset();
            doc.replace(sstart, e - sstart, buffer.toString(), null);
            ret[1] = sstart + buffer.length() - (e - end - 1);
            return ret;
        } catch (BadLocationException e) {
            System.err.println(e);
            return null;
        }
    }

    /**
     * Indent the line of this doc containing position.
     * @param position the position in the doc
     */
    public void indentDoc(int position) {
        if (doc.getAutoIndent()) {
            int[] level = new int[2];
            int[] ind = new int[2];

            try {
                int lineNumber = elem.getElementIndex(position);
                int pos = elem.getElement(lineNumber).getEndOffset() - 1;

                /* - level[0] is the left shift (else, elseif, ...) for the current line
                   - level[1] is the right shift for the next line */
                scanner.getIndentLevel(pos, level);

                int remove = 0;
                if (lineNumber >= 0) {
                    /* - ind[0] is equal to the number of "\t" or " " of the previous line
                       - ind[1] for the current line */
                    getNums(lineNumber, ind);
                    if (level[0] > 0 && ind[0] <= ind[1]) {
                        remove = level[0] * num;

                        if (lineNumber > 0 && ind[0] == ind[1]) {
                            /* Bug 7550 :
                               prev and cur line are at the same level
                               if cur should be indented, then we don't remove tabs */
                            int posp = elem.getElement(lineNumber - 1).getEndOffset() - 1;
                            int[] levelp = new int[2];
                            scanner.getIndentLevel(posp, levelp);
                            if (levelp[1] != 0) {
                                remove = 0;
                            }
                        }

                        if (ind[1] < remove) {
                            remove = ind[1];
                        }
                        if (remove != 0) {
                            int startL = elem.getElement(lineNumber).getStartOffset();
                            doc.remove(startL, remove);
                        }
                    }
                }

                int len = ind[1] + level[1] * num - remove;
                char[] str = new char[len];
                for (int i = 0; i < len; i++) {
                    str[i] = indentChar;
                }

                doc.insertString(pos + 1 - remove, new String(str), null);
            } catch (BadLocationException e) {
                System.err.println(e);
            }
        }
    }

    /**
     * @return the scanner used by this IndentManager
     */
    public IndentScanner getIndentScanner() {
        return scanner;
    }

    /**
     * Calculate the indentation of the two previous lines and the number of chars
     * used to indent is modified if we are not in psycho-rigid mode
     * @param lineNumber the number of the line
     * @param ind an array of length 2 which will contain the indent level of the line and the previous
     */
    private void getNums(int lineNumber, int[] ind) {
        ind[1] = scanner.getIndentNumber(lineNumber, indentChar);
        ind[0] = scanner.getIndentNumber(lineNumber - 1, indentChar);
        if (!isPsychoRigid) {
            int m = ind[1] - ind[0];
            if (m > 0) {
                num = m;
            }
        }
    }
}
