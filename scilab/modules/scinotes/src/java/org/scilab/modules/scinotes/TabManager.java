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
public class TabManager {

    private static final String EOL = "\n";

    private String tab;
    private int lengthTab;
    private ScilabDocument doc;
    private Element elem;
    private IndentManager indent;
    private boolean isTabInsertable = true;

    /**
     * We can link this TabManager with an IndentManager just to say that the size of a tabulation changed
     * @param doc the document
     * @param indent an IndentManager
     */
    public TabManager(ScilabDocument doc, IndentManager indent) {
        this.doc = doc;
        this.elem = doc.getDefaultRootElement();
        this.indent = indent;
        setTabulation(' ', 3);
    }

    /**
     * Set the type and the size of a tabulation
     * @param tab is a char : ' ' or '\t'
     * @param n is the length of a tab
     */
    public void setTabulation(char tab, int n) {
        if (tab == ' ') {
            this.tab = " ";
            lengthTab = n;
        } else {
            this.tab = "\t";
            lengthTab = 1;
        }
        doc.putProperty("tabSize", new Integer(Math.max(n, 1)));
        if (indent != null) {
            indent.setProperties(tab, SciNotesOptions.getSciNotesDisplay().indentSize);
        }
    }

    public String getTabulationString(int position) {
        if (tab.equals(" ")) {
            Element startL = elem.getElement(elem.getElementIndex(position));
            int sstart = position - startL.getStartOffset();
            int len = lengthTab - (sstart % lengthTab);
            if (len == 0) {
                len = lengthTab;
            }
            char[] str = new char[len];
            for (int i = 0; i < len; i++) {
                str[i] = ' ';
            }

            return new String(str);
        } else {
            return tab;
        }
    }

    public String getBasicTabulationString() {
        if (tab.equals(" ")) {
            char[] str = new char[lengthTab];
            for (int i = 0; i < lengthTab; i++) {
                str[i] = ' ';
            }

            return new String(str);
        } else {
            return tab;
        }
    }

    /**
     * Set the type and the size of a tabulation
     * @param tabulation a Tabulation
     */
    public void setTabulation(Tabulation tabulation) {
        setTabulation(tabulation.tab, tabulation.number);
    }

    /**
     * Set the type and the size of a tabulation
     */
    public void setDefaultTabulation() {
        setTabulation(new Tabulation());
    }

    /**
     * Insert a tab inside a line (true) or tabify a line (false)
     * @param b the boolean
     */
    public void setTabInsertable(boolean b) {
        isTabInsertable = b;
    }

    /**
     * @return a String which represents a tab
     */
    public String getTabulation() {
        return tab;
    }

    /**
     * Insert a tab just after the caret position (depends on setTabInsertable)
     * @param position the position in the doc
     * @return the length of a tab
     */
    public int insertTab(int position) {
        try {
            if (isTabInsertable) {
                String tab = getTabulationString(position);
                doc.insertString(position, tab, null);

                return tab.length();
            } else {
                tabifyLines(position, position - 1);
            }
        } catch (BadLocationException e) {
            e.printStackTrace();
        }

        return lengthTab;
    }

    /**
     * Tabify several lines.
     * @param start the starting position in the doc
     * @param end the ending position in the doc
     * @return the new positions of the selected text
     */
    public int[] tabifyLines(int start, int end) {
        Element startL = elem.getElement(elem.getElementIndex(start));
        int sstart = startL.getStartOffset();
        int[] ret = new int[] {0, 0};
        int send = end;

        try {
            String str = doc.getText(sstart, send - sstart + 1);
            if (str.charAt(str.length() - 1) == EOL.charAt(0)) {
                send--;
                str = str.substring(0, str.length() - 1);
                ret[1]++;
            }
            String tab = getTabulationString(start);
            String plainTab = getTabulationString(0);
            String rep = EOL + plainTab;
            str = tab + str.replaceAll(EOL, rep);
            ret[0] = start + tab.length();
            ret[1] += sstart + str.length();
            doc.replace(sstart, send - sstart + 1, str, null);
            return ret;
        } catch (BadLocationException e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * Suppress a tabulation just before the position or untabify line (depends on setTabInsertable)
     * @param position of the caret
     * @return the length of a tab
     */
    public int removeTab(int position) {
        try {
            if (isTabInsertable) {
                int pos = Math.max(0, position - lengthTab);
                if (tab.equals(doc.getText(pos, lengthTab))) {
                    doc.remove(pos, lengthTab);
                } else {
                    return 0;
                }
            } else {
                untabifyLine(position);
            }
        } catch (BadLocationException e) {
            e.printStackTrace();
        }

        return lengthTab;
    }

    /**
     * Delete a tab at the beginning of a line where the caret is if possible.
     * @param position the position in the doc
     */
    public void untabifyLine(int position) {
        IndentScanner iscan = indent.getIndentScanner();
        int line = elem.getElementIndex(position);
        int n = iscan.getTabsAtBeginning(line);

        try {
            if (n >= lengthTab) {
                doc.remove(elem.getElement(line).getStartOffset(), lengthTab);
            }
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
    }

    /**
     * Remove one tabulation step from every line starting with tabulation between start and end
     * @param start the starting position in the doc
     * @param end the ending position in the doc
     * @return the new positions of the selected text
     */
    public int[] untabifyLines(int start, int end) {
        int sstart = elem.getElement(elem.getElementIndex(start)).getStartOffset();
        int[] ret = new int[2];
        ret[0] = start;

        try {
            String str = doc.getText(sstart, end - sstart + 1);
            String untab = EOL + getBasicTabulationString();
            str = str.replaceAll(untab, EOL);
            IndentScanner iscan = indent.getIndentScanner();
            int n = iscan.getTabsAtBeginning(elem.getElementIndex(sstart));
            if (n >= lengthTab) {
                str = str.substring(lengthTab);
                if (start - sstart + 1 >= lengthTab) {
                    ret[0] = start - lengthTab;
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
     * Inner class to represent a tabulation
     */
    public static class Tabulation {

        /**
         * Should be '\t' or ' '
         */
        public char tab;

        /**
         * The number of whites equivalent at this tabulation
         */
        public int number;

        /**
         * Should be one of the constants of ScilabView
         */
        public int type;

        /**
         * If type is ScilabView.TABCHARACTER, then rep is the char representing a tabulation
         * in the view
         */
        public char rep;

        /**
         * Constructor
         * @param tab '\t' or ' '
         * @param number the number of whites
         * @param type see the constants in ScilabView
         * @param rep the char to represent a tabulation in a view
         */
        public Tabulation(char tab, int number, int type, char rep) {
            this.tab = tab;
            this.number = number;
            this.type = type;
            this.rep = rep;
        }

        public Tabulation() {
            this.tab = SciNotesOptions.getSciNotesDisplay().useSpaces ? ' ' : '\t';
            this.number = SciNotesOptions.getSciNotesDisplay().tabSize;
            this.type = SciNotesOptions.getSciNotesDisplay().tabRepresentation;
            this.rep = ' ';
        }
    }
}
