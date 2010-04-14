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

public class ScilabContext implements ViewFactory {

    Color[] tokenColors = new Color[ScilabLexerConstants.NUMBER_OF_TOKENS];
    Font[] tokenFonts = new Font[ScilabLexerConstants.NUMBER_OF_TOKENS];
    private ScilabView view;
    
    public ScilabContext() {
	super();
	Map map = ConfigXpadManager.getAllForegroundColors();
	Iterator it = map.keySet().iterator();
	while (it.hasNext()) {
	    String tokenType = (String) it.next();
	    tokenColors[ScilabLexerConstants.TOKENS.get(tokenType)] = (Color) map.get(tokenType);
	}
	map = ConfigXpadManager.getAllFontStyle();
	it = map.keySet().iterator();
	while (it.hasNext()) {
	    String tokenType = (String) it.next();
	    tokenFonts[ScilabLexerConstants.TOKENS.get(tokenType)] = (Font) map.get(tokenType);
	}
	for (int i = 0; i < tokenColors.length; i++) {
	    if (tokenColors[i] == null) {
		tokenColors[i] = tokenColors[0];
	    }
	    if (tokenFonts[i] == null) {
		tokenFonts[i] = tokenFonts[0];
	    }
	}
	
	// Special case : developers in Scilab in comments
	Font c = tokenFonts[ScilabLexerConstants.COMMENT];
	int style = c.getStyle();
	if (c.isBold()) {
	    tokenFonts[ScilabLexerConstants.AUTHORS] = c.deriveFont(style ^ Font.BOLD);
	} else {
	    tokenFonts[ScilabLexerConstants.AUTHORS] = c.deriveFont(style | Font.BOLD);
	}
	tokenColors[ScilabLexerConstants.AUTHORS] = tokenColors[ScilabLexerConstants.COMMENT];


	/*String[] tab = ScilabKeywords.GetMacrosName();//ScilabKeywords.GetCommandsName();//ScilabKeywords.GetVariablesName();
	String ch="";
	for (int i = 0; i < tab.length-1; i++) {
	    if (tab[i].charAt(0) != '%')
		ch+="\"" + tab[i]+ "\" | ";
	}
	ch+="\""+tab[tab.length-1]+"\"";
	System.out.println(ch);*/
    }
    
    /*    public Color getForeground(int code) {
	if ((code >= 0) && (code < tokenColors.length)) {
	    return tokenColors[code];
	}
	return Color.BLACK;
    }

    public Color getFont(int code) {
	if ((code >= 0) && (code < tokenColors.length)) {
	    return tokenStyle[code];
	}
	return Color.BLACK;
	}*/

    public View getCurrentView() {
	return view;
    }

    public View create(Element elem) {
	view = new ScilabView(elem, this);
	return view;
    }
}