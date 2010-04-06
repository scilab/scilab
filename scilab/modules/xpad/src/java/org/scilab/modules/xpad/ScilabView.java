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
import java.awt.Color;
import java.awt.Font;
import java.awt.Shape;

import javax.swing.text.Utilities;
import javax.swing.text.WrappedPlainView;
import javax.swing.text.Segment;
import javax.swing.text.Document;
import javax.swing.text.Element;
import javax.swing.text.BadLocationException;

import java.io.IOException;

public class ScilabView extends WrappedPlainView {

    private ScilabContext context;
    private ScilabLexer lexer;
    private boolean lexerValid;
    private ScilabDocument doc;
    private Segment text = new Segment();

    ScilabView(Element elem, ScilabContext context) {
	super(elem);
	this.context = context;
	this.doc = (ScilabDocument) getDocument();
	lexer = doc.createLexer();
	lexerValid = false;
    }
    
    /* The lexer returns all tokens between the pos p0 and p1.
       The value of the returned token determinates the color and the font.
       The lines can be broken by the Pane so we must look at previous
       and next chars to know if p0 or p1 is "inside" a token. */

    protected int drawUnselectedText(Graphics g, int x, int y, int p0, int p1) throws BadLocationException {
	int prevTok = -1;
	int tok = -1;
	int mark = p0;
	boolean isBroken = false;//System.out.println("Before drawing p0="+p0+",p1="+p1+",x="+x+",y="+y);

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
		x = Utilities.drawTabbedText(text, x, y, g, null, mark);
		mark = end;
	    }

	    p0 = end;
	}
	
	return x;
    }    
}