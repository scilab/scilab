/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.console;

import java.awt.Color;
import java.awt.Font;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;

import com.artenum.rosetta.interfaces.core.InputParsingManager;
import com.artenum.rosetta.interfaces.ui.PromptView;

/**
 * Scilab Console UI which contains the prompt
 * @author Vincent COUVERT
 */
public class SciPromptView extends JPanel implements PromptView {

	private static final long serialVersionUID = 1L;

	private static final String DEFAULT_PROMPT = "-->";
	private static final String DEFAULT_IN_BLOCK_PROMPT = DEFAULT_PROMPT;
	private static final String HTML_START = "<html>";
	private static final String HTML_NEW_LINE = "<br/>";
	private static final String HTML_END = "</html>";
	private static final String SEMI_COLON = ";";
	private static final String PARAGRAPH_END = "</P>";
	private static final String ZERO_STRING = "0";
	private static final int LEFT_BORDER = 0;
	private static final int BOTTOM_BORDER = 2;

	private String defaultPrompt;
	private String inBlockPrompt;
	private InputParsingManager inputParsingManager;

	// Working vars
	private StringBuffer promptTextContent;
	private JLabel promptUI;

	/**
	 * Constructor
	 */
	public SciPromptView() {
		// Internal part
		promptTextContent = new StringBuffer();
		defaultPrompt = DEFAULT_PROMPT;
		inBlockPrompt = DEFAULT_IN_BLOCK_PROMPT;

		// Gui part
		promptUI = new JLabel();
		promptUI.setBorder(BorderFactory.createEmptyBorder(0, LEFT_BORDER, BOTTOM_BORDER, 0));
		promptUI.setOpaque(true);
		setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
		add(promptUI);
		add(Box.createVerticalGlue());
		// Prompt is invisible when created
		setVisible(false);
	}

	/**
	 * Sets the default prompt symbol
	 * @param defaultPrompt the default prompt as a string
	 * @see com.artenum.rosetta.interfaces.ui.PromptView#setDefaultPrompt(java.lang.String)
	 */
	public void setDefaultPrompt(String defaultPrompt) {
		this.defaultPrompt = defaultPrompt;
		updatePrompt();
	}

	/**
	 * Sets the block prompt symbol (prompt displayed when editing a block)
	 * @param inBlockPrompt the default prompt as a string
	 * @see com.artenum.rosetta.interfaces.ui.PromptView#setInBlockPrompt(java.lang.String)
	 */
	public void setInBlockPrompt(String inBlockPrompt) {
		this.inBlockPrompt = inBlockPrompt;
		updatePrompt();
	}

	/**
	 * Sets the parsing manager associated to this prompt
	 * @param inputParsingManager the parsing manager
	 * @see com.artenum.rosetta.interfaces.ui.PromptView#setInputParsingManager(com.artenum.rosetta.interfaces.core.InputParsingManager)
	 */
	public void setInputParsingManager(InputParsingManager inputParsingManager) {
		this.inputParsingManager = inputParsingManager;
	}

	/**
	 * Updates the prompt object contents according to the current state (in a block or not)
	 * @see com.artenum.rosetta.interfaces.ui.PromptView#updatePrompt()
	 */
	public void updatePrompt() {
		promptTextContent.setLength(0);
		promptTextContent.append(HTML_START);

		// Text Color
		String textStyle = "<P style=\"padding:1px;color:" + colorToString(promptUI.getForeground()) + SEMI_COLON;

		// Text Font
		if (promptUI.isFontSet()) {
			textStyle += "font-size:" + promptUI.getFont().getSize() + "pt;";
			textStyle += "font-family:" + promptUI.getFont().getName() + SEMI_COLON;
			if (promptUI.getFont().isBold()) {
				textStyle += "font-weight:bold;";
			}

			if (promptUI.getFont().isItalic()) {
				textStyle += "font-style:italic;";
			}
		}
		textStyle += "\">";

		promptTextContent.append(textStyle + defaultPrompt.replaceAll(">", "&gt;") + PARAGRAPH_END);

		int nbLineToShow = inputParsingManager.getNumberOfLines();
		while (nbLineToShow-- > 1) {
			promptTextContent.append(HTML_NEW_LINE);
			promptTextContent.append(textStyle + inBlockPrompt + PARAGRAPH_END);
		}
		promptTextContent.append(HTML_END);
		promptUI.setText(promptTextContent.toString());
	}

	/**
	 * Sets the background of the prompt
	 * @param bgColor the color to set
	 * @see javax.swing.JComponent#setBackground(java.awt.Color)
	 */
	public void setBackground(Color bgColor) {
		super.setBackground(bgColor);
		if (promptUI != null) {
			promptUI.setBackground(bgColor);
		}
	}

	/**
	 * Sets the foreground of the prompt
	 * @param fgColor the color to set
	 * @see javax.swing.JComponent#setForeground(java.awt.Color)
	 */
	public void setForeground(Color fgColor) {
		super.setForeground(fgColor);
		if (promptUI != null) {
			promptUI.setForeground(fgColor);
		}
	}

	/**
	 * Sets the font of the prompt
	 * @param font the font to set
	 * @see javax.swing.JComponent#setBackground(java.awt.font)
	 */
	public void setFont(Font font) {
		super.setFont(font);
		if (promptUI != null) {
			promptUI.setFont(font);
		}
	}

	/**
	 * Gets the default prompt symbol
	 * @return the default prompt symbol
	 * @see com.artenum.rosetta.interfaces.ui.PromptView#getDefaultPrompt()
	 */
	public String getDefaultPrompt() {
		return defaultPrompt;
	}

	/**
	 * Gets the block prompt symbol
	 * @return the block prompt symbol
	 * @see com.artenum.rosetta.interfaces.ui.PromptView#getInBlockPrompt()
	 */
	public String getInBlockPrompt() {
		return inBlockPrompt;
	}

	/**
	 * Sets the visibility status of the prompt view
	 * @param status new visibility status
	 * @see javax.swing.JComponent#setVisible(boolean)
	 */
	public void setVisible(boolean status) {
		super.setVisible(status);
	}

	/**
	 * Convert a color into an hexadecimal string
	 * @param c the color to convert
	 * @return the corresponding hexadecimal string
	 */
	private static String colorToString(Color c) {
		String strColor = "#";
		String s = Integer.toHexString(c.getRed());
		if (s.length() == 1) {
			strColor += ZERO_STRING + s;
		} else {
			strColor += s;
		}
		s = Integer.toHexString(c.getGreen());
		if (s.length() == 1) {
			strColor += ZERO_STRING + s;
		} else {
			strColor += s;
		}
		s = Integer.toHexString(c.getBlue());
		if (s.length() == 1) {
			strColor += ZERO_STRING + s;
		} else {
			strColor += s;
		}
		return strColor;
	}

	/**
	 * @return the prompt JLabel
	 */
	public JLabel getPromptUI() {
		return promptUI;
	}

}
