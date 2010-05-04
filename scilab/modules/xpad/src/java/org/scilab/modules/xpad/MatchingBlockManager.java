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

import java.awt.Color;

import javax.swing.text.DefaultHighlighter;
import javax.swing.text.Highlighter;
import javax.swing.text.BadLocationException;

/**
 * Useful to match opening and closing keywords from left to right or from right to left
 * @author Calixte DENIZET
 */
public class MatchingBlockManager {

    private ScilabDocument doc;
    private MatchingBlockScanner scanner;
    private Highlighter highlighter;
    private MatchingBlockScanner.MatchingPositions smpos;
    private DefaultHighlighter.DefaultHighlightPainter hp;
    private Object first;
    private Object second;
    private boolean all;
    private Color color;
    private boolean lr;

    /**
     * Constructor
     * @param doc the doc to highlight
     * @param lr if true the matching is from left to right
     * @param highlighter the highlighter to use
     * @param color the color of the highlight
     */
    public MatchingBlockManager(ScilabDocument doc, boolean lr, Highlighter highlighter, Color color) {
	this.doc = doc;
	this.scanner = new MatchingBlockScanner(doc);
	this.highlighter = highlighter;
	this.lr = lr;
	setColor(color);
    }

    /**
     * @param b if true, then highlights then contents between the matching keywords
     */
    public void highlightAllTheBlock(boolean b) {
	this.all = b;
    }

    /**
     * Change the color of the highlighter
     * @param color the color to use
     */
    public void setColor(Color color) {
	if (color != this.color) {
	    this.color = color;
	    if (first != null) {
		highlighter.removeHighlight(first);
		first = null;
	    }
	    if (second != null) {
		highlighter.removeHighlight(second);
		second = null;
	    }
	    hp = new DefaultHighlighter.DefaultHighlightPainter(color);
	}
    }

    /**
     * Search the matching keywords
     * @param tok the type of the token at the position pos in the document
     * @param pos the positon in the doc
     */
    public void searchMatchingBlock(int tok, int pos) {
	MatchingBlockScanner.MatchingPositions mpos = null;
	if (tok == ScilabLexerConstants.CKEYWORD || tok == ScilabLexerConstants.SKEYWORD || tok == ScilabLexerConstants.FKEYWORD || tok == ScilabLexerConstants.OPEN || tok == ScilabLexerConstants.CLOSE) {
	    mpos = scanner.getMatchingBlock(pos, lr);
	}
	if (mpos != this.smpos) {
	    this.smpos = mpos;
	    if (first != null) {
		highlighter.removeHighlight(first);
		if (!all && second != null) {
		    highlighter.removeHighlight(second);
		}
	    }
	    if (mpos != null) {
		try {
		    if (!all) {
			first = highlighter.addHighlight(mpos.firstB, mpos.firstE, hp);
			second = highlighter.addHighlight(mpos.secondB, mpos.secondE, hp);
		    } else {
			first = highlighter.addHighlight(mpos.firstB, mpos.secondE, hp);
		    }
		} catch (BadLocationException e) {
		    System.err.println(e);
		}
	    }
	}
    }
}
