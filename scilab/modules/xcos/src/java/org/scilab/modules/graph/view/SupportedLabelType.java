/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.view;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.scilab.modules.graph.utils.ScilabGraphUtils;

import com.mxgraph.util.mxUtils;

/**
 * Specify the supported label type
 */
public enum SupportedLabelType {
	/** The label is HTML formatted. This is the default */
	HTML,
	/** The label is Latex formatted */
	Latex,
	/** The label is MathML formatted */
	MathML;
	
	/**
	 * Pattern to match the &lt;br&gt; HTML tag
	 */
	private static final Pattern BR_PATTERN = Pattern.compile("<br>\\p{Blank}*");
	
	/**
	 * Get the {@link SupportedLabelType} for the label.
	 * 
	 * The type of the label is found with a specific first character.
	 * <ul>
	 * <li>'$' : for {@value SupportedLabelType#Latex} markup</li>
	 * <li>'^' : for {@value SupportedLabelType#MathML} markup</li>
	 * </ul>
	 * 
	 * The {@value SupportedLabelType#HTML} is used as default. 
	 * 
	 * @param text The label to parse
	 * @return the type of the label
	 */
	public static SupportedLabelType getFromText(String text) {
		if (text.length() > 0) {
			char[] dst = new char[1];
			text.getChars(0, 1, dst, 0);
			
	    	if (dst[0] == '$') {
	    		return Latex;
	    	} else if (dst[0] == '^') {
	    		return MathML;
	    	}
		}
		return HTML;
	}
	
	/**
	 * Get the {@link SupportedLabelType} for the HTML formatted string.
	 * 
	 * The type of the label is found with a specific first character of the
	 * content. The HTML markup is escaped automatically.
	 * <ul>
	 * <li>'$' : for {@value SupportedLabelType#Latex} markup</li>
	 * <li>'^' : for {@value SupportedLabelType#MathML} markup</li>
	 * </ul>
	 * 
	 * The {@value SupportedLabelType#HTML} is used as default. 
	 * 
	 * @param html The HTML string to parse
	 * @return the type of the label
	 */
	public static SupportedLabelType getFromHTML(String html) {
		if (html.length() > 0 && html.charAt(0) == '<') {

			StringBuilder content;
			content = new StringBuilder(mxUtils.getBodyMarkup(html, false));
			
			ScilabGraphUtils.removeBlanks(content);
						
			if (content.charAt(0) == '$') {
				return Latex;
			} else if (content.charAt(0) == '^') {
				return MathML;
			}
		}
		return HTML;
	}
	
	/**
	 * Escape the text for the current type.
	 * @param text the text to escape
	 * @return the escaped text
	 */
	public String escape(String text) {
		StringBuilder escapedText;
		
		if (text.startsWith("<html")) {
			escapedText = new StringBuilder(ScilabGraphUtils.getBodyMarkup(text, true));
		} else {
			escapedText = new StringBuilder(text);
		}
		
		switch (this) {
		case MathML:
		case Latex:
			// Unescape content
			ScilabGraphUtils.unescape(escapedText, 0);
			
			// Removing <br>
    		Matcher m = BR_PATTERN.matcher(escapedText);
    		while (m.find()) {
    			escapedText.replace(m.start(), m.end(), "");
    			m.reset();
			}
    		
    		// Removing blank
			ScilabGraphUtils.removeBlanks(escapedText);
			
			// Removing the first and last char (tag)
			escapedText.delete(0, 1);
			int length = escapedText.length();
			escapedText.delete(length - 2, length - 1);
			break;

		default:
			break;
		}
		
		return escapedText.toString();
	}
}
