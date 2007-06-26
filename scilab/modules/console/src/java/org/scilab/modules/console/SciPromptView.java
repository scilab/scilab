
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.Color;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;

import com.artenum.console.interfaces.core.InputParsingManager;
import com.artenum.console.interfaces.ui.PromptView;

/**
 * Scilab Console UI which contains the prompt
 * @author Vincent COUVERT
 */
public class SciPromptView extends JPanel implements PromptView {
	
	private static final long serialVersionUID = 1L;
	
	private static final String DEFAULT_PROMPT = "-->";
	private static final String DEFAULT_IN_BLOCK_PROMPT = "-B->";
	private static final String HTML_START = "<html>";
	private static final String HTML_NEW_LINE = "<br/>";
	private static final String HTML_END = "</html>";
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
		promptUI.setBorder(BorderFactory.createEmptyBorder(2, 2, 2, 2));
		promptUI.setOpaque(true);
		setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
		add(promptUI);
		add(Box.createVerticalGlue());
	}

	/**
	 * Sets the default prompt symbol
	 * @param defaultPrompt the default prompt as a string
	 * @see com.artenum.console.interfaces.ui.PromptView#setDefaultPrompt(java.lang.String)
	 */
	public void setDefaultPrompt(String defaultPrompt) {
		this.defaultPrompt = defaultPrompt;
		updatePrompt();
	}

	/**
	 * Sets the block prompt symbol (prompt displayed when editing a block)
	 * @param inBlockPrompt the default prompt as a string
	 * @see com.artenum.console.interfaces.ui.PromptView#setInBlockPrompt(java.lang.String)
	 */
	public void setInBlockPrompt(String inBlockPrompt) {
		this.inBlockPrompt = inBlockPrompt;
		updatePrompt();
	}

	/**
	 * Sets the parsing manager associated to this prompt
	 * @param inputParsingManager the parsing manager
	 * @see com.artenum.console.interfaces.ui.PromptView#setInputParsingManager(com.artenum.console.interfaces.core.InputParsingManager)
	 */
	public void setInputParsingManager(InputParsingManager inputParsingManager) {
		this.inputParsingManager = inputParsingManager;
	}

	/**
	 * Updates the prompt object contents according to the current state (in a block or not)
	 * @see com.artenum.console.interfaces.ui.PromptView#updatePrompt()
	 */
	public void updatePrompt() {
		promptTextContent.setLength(0);
		promptTextContent.append(HTML_START);
		promptTextContent.append(defaultPrompt.replaceAll(">", "&gt;"));
		int nbLineToShow = inputParsingManager.getNumberOfLines();
		while (nbLineToShow-- > 1) {
			promptTextContent.append(HTML_NEW_LINE);
			promptTextContent.append(inBlockPrompt);
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
	 * Gets the default prompt symbol
	 * @return the default prompt symbol
	 * @see com.artenum.console.interfaces.ui.PromptView#getDefaultPrompt()
	 */
	public String getDefaultPrompt() {
		return defaultPrompt;
	}

	/**
	 * Gets the block prompt symbol
	 * @return the block prompt symbol
	 * @see com.artenum.console.interfaces.ui.PromptView#getInBlockPrompt()
	 */
	public String getInBlockPrompt() {
		return inBlockPrompt;
	}

}
