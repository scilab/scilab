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

import java.awt.Rectangle;
import java.awt.Shape;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.MouseListener;
import java.awt.event.MouseEvent;

import java.util.List;
import java.util.ArrayList;
import java.io.IOException;

import javax.swing.JEditorPane;
import javax.swing.text.JTextComponent;
import javax.swing.text.View;
import javax.swing.text.Element;
import javax.swing.text.Document;
import javax.swing.text.DefaultHighlighter;
import javax.swing.text.BadLocationException;
import javax.swing.event.CaretListener;
import javax.swing.event.CaretEvent;
import javax.swing.text.Highlighter;

public class ScilabEditorPane extends JEditorPane implements Highlighter.HighlightPainter, CaretListener, MouseListener {

    private Color highlightColor = new Color(228,233,244);
    private boolean highlightEnable = false;
    private ScilabLexer lexer = null;
    private SingleScanner scanner = null;

    private List<KeywordListener> kwList = new ArrayList();

    public ScilabEditorPane() {
	super();
	addCaretListener(this);
	enableHighlightedLine();
    }

    protected boolean enableScanner() {
	ScilabDocument doc = (ScilabDocument) getDocument();
	if (doc != null) {
	    if (scanner == null) {
		scanner = new SingleScanner(doc);
	    }
	    return true;
	} else {
	    return false;
	}
    }

    public SingleScanner getScanner() {
	return scanner;
    } 

    public void addKeywordListener(KeywordListener kw) {
	addMouseListener(this);
	kwList.add(kw);
	if (lexer == null) {
	    lexer = ((ScilabDocument) getDocument()).createLexer();
	}
    }
    
    public void removeKeywordListener(KeywordListener kw) {
	removeMouseListener(this);
	kwList.remove(kw);
	lexer = null;
    }

    public void setHighlightedLineColor(Color c) {
	highlightColor = c;
    }

    public Color getHighlightedLineColor() {
	return highlightColor;
    }

    public void enableHighlightedLine() {
	if (!highlightEnable) {
	    try {
		getHighlighter().addHighlight(0, 0, this);
	    } catch (BadLocationException e) { }
	    highlightEnable = true;
	}
    }

    public void disableHighlightedLine() {
	if (highlightEnable) {
	    getHighlighter().removeAllHighlights();
	    highlightEnable = false;
	}
    }

    public void caretUpdate(CaretEvent e) {
	if (highlightEnable) {
	    repaint();
	}
    }

    public void paint(Graphics g, int p0, int p1, Shape bounds, JTextComponent c) {
	try {
	    Rectangle r = modelToView(getCaretPosition());
	    g.setColor(highlightColor);
	    g.fillRect(0, r.y, getWidth(), r.height);
	} catch (BadLocationException e) { }
    }

    public void mouseClicked(MouseEvent e) {
	int p0 = getCaretPosition();
	Element elem = getDocument().getDefaultRootElement();
	Element line = elem.getElement(elem.getElementIndex(p0));	
	int startL = line.getStartOffset();
	int endL = line.getEndOffset();
	int tok = -1;
	
	try {
	    lexer.setRange(startL, endL);
	    while (startL < p0) {
		tok = lexer.yylex();
		startL = lexer.start + lexer.yychar() + lexer.yylength();
	    }
	} catch (IOException ex) { }
	
	KeywordEvent kev = new KeywordEvent(this, tok, lexer.start + lexer.yychar(), lexer.yylength());
	for (int i = 0; i < kwList.size(); i++) {
	    kwList.get(i).keywordClicked(kev);
	}
    }
    
    public void mouseEntered(MouseEvent e) { }
    public void mouseExited(MouseEvent e) { }
    public void mousePressed(MouseEvent e) { }
    public void mouseReleased(MouseEvent e) { }	
}
    