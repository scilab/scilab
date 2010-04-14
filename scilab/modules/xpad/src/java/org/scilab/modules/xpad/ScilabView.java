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

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Color;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Shape;
import java.awt.Rectangle;
import java.awt.geom.Rectangle2D;
import java.awt.Component;
import java.awt.Insets;

import javax.swing.text.Utilities;
import javax.swing.text.WrappedPlainView;
import javax.swing.text.PlainView;
import javax.swing.text.ViewFactory;
import javax.swing.text.View;
import javax.swing.text.Segment;
import javax.swing.text.Document;
import javax.swing.event.DocumentEvent;
import javax.swing.text.Element;
import javax.swing.text.JTextComponent;
import javax.swing.text.BadLocationException;

import javax.swing.SizeRequirements;


import java.io.IOException;

public class ScilabView extends WrappedPlainView {

    private ScilabContext context;
    private ScilabLexer lexer;
    private boolean lexerValid;
    private ScilabDocument doc;
    private Segment text = new Segment();
    private static final Rectangle rect = new Rectangle();

    ScilabView(Element elem, ScilabContext context) {
	super(elem);
	this.context = context;
	doc = (ScilabDocument) getDocument();
	doc.setView(this);
	lexer = doc.createLexer();
	lexerValid = false;
    }

    // Can be overridden to beautify the Rectangle 'a' where the text is wrote
    /*public void paint(Graphics g, Shape a) {
      }*/

    public int getLineAllocation(int n) {
	rect.setLocation(0, 4);
	childAllocation(n, rect);
	return rect.y;
    }

    /* The lexer returns all tokens between the pos p0 and p1.
       The value of the returned token determinates the color and the font.
       The lines can be broken by the Pane so we must look at previous
       and next chars to know if p0 or p1 is "inside" a token. */
    protected int drawUnselectedText(Graphics g, int x, int y, int p0, int p1) throws BadLocationException {
	int prevTok = -1;
	int tok = -1;
	int mark = p0;
	boolean isBroken = false;

	Element elem = doc.getDefaultRootElement();
	Element line = elem.getElement(elem.getElementIndex(p0));	
	int startL = line.getStartOffset();
	int endL = line.getEndOffset();

	if (startL != p0) {
	    //we are drawing a broken line
	    try {
		lexer.setRange(startL, endL);
		while (startL < p0) {
		    tok = lexer.yylex();
		    startL = lexer.start + lexer.yychar() + lexer.yylength();
		}
		isBroken = true;
	    } catch (IOException e) { }
	}

	if (!isBroken) {
	    lexer.setRange(p0, endL);
	}
		
	while (p0 < p1) {
	    try {
		if (!isBroken) {
		    tok = lexer.yylex();
		} else {
		    isBroken = false;
		}
	    } catch (IOException e) { }

	    p0 = lexer.start + lexer.yychar();
	    int end = Math.min(p1, p0 + lexer.yylength());

 	    if (end != mark) {
		if (tok != prevTok) {
		    g.setColor(context.tokenColors[tok]);
		    g.setFont(context.tokenFonts[tok]);
		    prevTok = tok;
		}

		doc.getText(mark, end - mark, text);
		
		// A modifier pr gerer le style souligne ...
		if (tok == ScilabLexerConstants.URL) {
		    int w = Utilities.getTabbedTextWidth(text, g.getFontMetrics(), x, null, mark);
		    g.drawLine(x, y + 1, x + w, y + 1);
		}
		
		x = Utilities.drawTabbedText(text, x, y, g, null, mark);
		mark = end;
	    }

	    p0 = end;
	}
	
	return x;
    }    
}