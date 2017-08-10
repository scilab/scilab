/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
        if (!promptUI.getText().equals(defaultPrompt)) {
            promptUI.setText(defaultPrompt);
        }
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
     * @return the prompt JLabel
     */
    public JLabel getPromptUI() {
        return promptUI;
    }

}
