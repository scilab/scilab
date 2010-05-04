/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad;

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
	    char[] str = new char[n];
	    for (int i = 0; i < n; i++) {
		str[i] = ' ';
	    }
	    this.tab = new String(str);
	    lengthTab = n;
	} else {
	    this.tab = "\t";
	    lengthTab = Math.max(n, 1);
	    doc.putProperty("tabSize", new Integer(lengthTab));
	}
	if (indent != null) {
	    indent.setProperties(tab, n);
	}
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
     */
    public void insertTab(int position) {
	try {
	    if (isTabInsertable) {
		doc.insertString(position, tab, null);
	    } else {
		tabifyLines(position, position - 1);
	    }
	} catch (BadLocationException e) {
	    e.printStackTrace();
	}
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
	int[] ret = new int[2];
	
	try {
	    String str = doc.getText(start, end - start + 1);
	    String rep = EOL + tab;
	    str = tab + str.replaceAll(EOL, rep);		
	    ret[0] = start + lengthTab;
	    ret[1] = sstart + str.length();
	    doc.replace(sstart, end - sstart + 1, str, null);
	    return ret;
	} catch (BadLocationException e) { 
	    e.printStackTrace();
	    return null;
	}
    }

    /**
     * Suppress a tabulation inside a text or untabify line (depends on setTabInsertable)
     * @param position of the caret
     */
    public void untabifyText(int position) {
	try {
	    if (isTabInsertable) {
		int end = elem.getElement(elem.getElementIndex(position)).getEndOffset();
		if (end - position >= lengthTab && tab.equals(doc.getText(position, lengthTab))) {
		    doc.remove(position, lengthTab);
		}
	    } else {
		untabifyLine(position);
	    }
	} catch (BadLocationException e) {
	    e.printStackTrace();
	}
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
	    String untab = EOL + tab;
	    str = str.replaceAll(untab, EOL);
	    IndentScanner iscan = indent.getIndentScanner();
	    int n = iscan.getTabsAtBeginning(elem.getElementIndex(sstart));
	    if (n >= lengthTab) {
		str = str.substring(lengthTab);
		if (start - sstart >= lengthTab + 1) {
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
}
