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

import java.util.Map;
import java.util.Iterator;
import java.awt.Color;
import java.awt.Font;

import javax.swing.text.ViewFactory;
import javax.swing.text.View;
import javax.swing.text.Element;

import org.scilab.modules.xpad.utils.ConfigXpadManager;

/**
 * The class ScilabContext provides a context to render a Scilab's document.
 * @author Calixte DENIZET
 */
public class ScilabContext implements ViewFactory {

    /**
     * Contains the colors of the different tokens
     */
    Color[] tokenColors;
    
    /**
     * Contains the fonts of the different tokens
     */
    Font[] tokenFonts;
    
    /**
     * Contains the attrib (underline or stroke) of the different tokens
     */
    int[] tokenAttrib = new int[ScilabLexerConstants.NUMBEROFTOKENS];

    private ScilabView view;
    
    /**
     * The constructor
     */
    public ScilabContext() {
	super();
	genColors();
	genFonts();

	Map map = ConfigXpadManager.getAllAttributes();
	Iterator it = map.keySet().iterator();
	while (it.hasNext()) {
	    String tokenType = (String) it.next();
	    tokenAttrib[ScilabLexerConstants.TOKENS.get(tokenType)] = ((Integer) map.get(tokenType)).intValue();
	}
    }

    /**
     * Generate the colors to use to render the document
     */
    public void genColors() {
	tokenColors = new Color[ScilabLexerConstants.NUMBEROFTOKENS];
	Map map = ConfigXpadManager.getAllForegroundColors();
	Iterator it = map.keySet().iterator();
	while (it.hasNext()) {
	    String tokenType = (String) it.next();
	    tokenColors[ScilabLexerConstants.TOKENS.get(tokenType)] = (Color) map.get(tokenType);
	}
	
	for (int i = 0; i < tokenColors.length; i++) {
	    if (tokenColors[i] == null) {
		tokenColors[i] = tokenColors[0];
	    }
	}
	
	/* Special case : Scilab's developers in comments */ 
	tokenColors[ScilabLexerConstants.AUTHORS] = tokenColors[ScilabLexerConstants.COMMENT];
    }

    /**
     * Generate a color for a type of keyword
     * @param name the name can be found in xpadConfiguration.xml
     * @param color the color to use
     */
    public void genColors(String name, Color color) {
	Map map = ConfigXpadManager.getAllForegroundColors();
	if (tokenColors == null) {
	    genColors();
	}
	tokenColors[ScilabLexerConstants.TOKENS.get(name)] = color;
	tokenColors[ScilabLexerConstants.AUTHORS] = tokenColors[ScilabLexerConstants.COMMENT];
    }
    
    /**
     * Generate the fonts to use to render the document
     */
    public void genFonts() {
	genFonts(null);
    }

    /**
     * Generate the fonts to use to render the document
     * @param font the base font to use
     */
    public void genFonts(Font font) {
	Map map;
	if (font != null) {
	    map = ConfigXpadManager.getAllFontStyle(font);
	} else {
	    map = ConfigXpadManager.getAllFontStyle();
	}   
	
	tokenFonts = new Font[ScilabLexerConstants.NUMBEROFTOKENS];
	
	Iterator it = map.keySet().iterator();
	while (it.hasNext()) {
	    String tokenType = (String) it.next();
	    tokenFonts[ScilabLexerConstants.TOKENS.get(tokenType)] = (Font) map.get(tokenType);
	}

	for (int i = 0; i < tokenFonts.length; i++) {
	    if (tokenFonts[i] == null) {
		tokenFonts[i] = tokenFonts[0];
	    }
	}
	
	if (view != null) {
	    view.reinitialize();
	}

	/* Special case : Scilab's developers in comments */
	Font c = tokenFonts[ScilabLexerConstants.COMMENT];
	int style = c.getStyle();
	if (c.isBold()) {
	    tokenFonts[ScilabLexerConstants.AUTHORS] = c.deriveFont(style ^ Font.BOLD);
	} else {
	    tokenFonts[ScilabLexerConstants.AUTHORS] = c.deriveFont(style | Font.BOLD);
	}
    }

    /**
     * @return the view to use to render the document
     */
    public View getCurrentView() {
	return view;
    }

    /**
     * Create a view with a given element
     * @param elem the Element to view
     * @return the view associated with the element
     */
    public View create(Element elem) {
	view = new ScilabView(elem, this);
	return view;
    }
}
