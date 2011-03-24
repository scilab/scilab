/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.textObject;

/**
 * FormattedText class
 * @author Manuel JULIACHS
 */
public class FormattedText {
	/** FormattedText properties names */
	public enum FormattedTextProperty { TEXT, FONT };

	/** Text */
	private String text;

	/** Font */
	private Font font;

	/** Constructor */
	public FormattedText() {
		text = "";
		font = new Font();
	}

	/**
	 * Copy constructor
	 * @param formText the formatted text to copy
	 */
	public FormattedText(FormattedText formText) {
		this.text = new String(formText.getText());
		this.font = new Font(formText.getFont());
	}

	/**
	 * @return the font
	 */
	public Font getFont() {
		return font;
	}

	/**
	 * @param font the font to set
	 */
	public void setFont(Font font) {
		this.font = font;
	}

	/**
	 * @return the text
	 */
	public String getText() {
		return text;
	}

	/**
	 * @param text the text to set
	 */
	public void setText(String text) {
		this.text = text;
	}

}
