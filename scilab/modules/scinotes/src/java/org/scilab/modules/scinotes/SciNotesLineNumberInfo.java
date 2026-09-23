/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.scinotes;

import java.util.Stack;

import javax.swing.text.Element;

/**
 * Computes the line numbers and nesting levels used by SciNotes local numbering.
 */
final class SciNotesLineNumberInfo {

    private final int[] lineNumbers;
    private final byte[] lineLevels;

    private SciNotesLineNumberInfo(int[] lineNumbers, byte[] lineLevels) {
        this.lineNumbers = lineNumbers;
        this.lineLevels = lineLevels;
    }

    public int[] getLineNumbers() {
        return lineNumbers;
    }

    public byte[] getLineLevels() {
        return lineLevels;
    }

    public static SciNotesLineNumberInfo compute(ScilabDocument doc) {
        synchronized (doc) {
            Stack<Integer> stk = new Stack<Integer>();
            Element root = doc.getDefaultRootElement();
            int nlines = root.getElementCount();
            int[] lineNumbers = new int[nlines + 1];
            byte[] lineLevels = new byte[nlines + 1];
            lineNumbers[0] = 1;
            int current = 1;
            boolean prevBroken = false;
            ScilabDocument.ScilabLeafElement elem;
            for (int i = 0; i < nlines; i++) {
                elem = (ScilabDocument.ScilabLeafElement) root.getElement(i);
                int type = elem.getType();
                switch (type) {
                    case ScilabDocument.ScilabLeafElement.NOTHING :
                        if (prevBroken) {
                            lineNumbers[i] = -1;
                            if (!elem.isBroken()) {
                                prevBroken = false;
                            }
                        } else {
                            lineNumbers[i] = current++;
                        }
                        lineLevels[i] = (byte) stk.size();
                        break;
                    case ScilabDocument.ScilabLeafElement.FUN :
                        stk.push(Integer.valueOf(current));
                        lineLevels[i] = (byte) stk.size();
                        current = 2;
                        lineNumbers[i] = 1;
                        if (elem.isBroken()) {
                            prevBroken = true;
                        }
                        break;
                    case ScilabDocument.ScilabLeafElement.ENDFUN :
                        lineNumbers[i] = current++;
                        lineLevels[i] = (byte) stk.size();
                        if (!stk.empty()) {
                            current = stk.pop().intValue() + lineNumbers[i];
                        }
                        break;
                    default :
                        break;
                }
            }

            return new SciNotesLineNumberInfo(lineNumbers, lineLevels);
        }
    }
}
