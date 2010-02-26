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

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Point;
import java.io.IOException;
import java.util.concurrent.Semaphore;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.StyledDocument;
import javax.swing.KeyStroke;
import javax.xml.parsers.ParserConfigurationException;
import org.scilab.modules.localization.Messages;

import org.xml.sax.SAXException;

import com.artenum.rosetta.interfaces.core.ConsoleConfiguration;
import com.artenum.rosetta.interfaces.core.InputParsingManager;
import com.artenum.rosetta.interfaces.ui.InputCommandView;
import com.artenum.rosetta.interfaces.ui.OutputView;
import com.artenum.rosetta.interfaces.ui.PromptView;
import com.artenum.rosetta.ui.Console;
import com.artenum.rosetta.util.ConfigurationBuilder;
import com.artenum.rosetta.util.ConsoleBuilder;
import com.artenum.rosetta.util.StringConstants;

/**
 * Main class for Scilab Console based on Generic Console from Artenum
 * @author Vincent COUVERT
 */
public abstract class SciConsole extends JPanel {

	private static final long serialVersionUID = 1L;

	private static final int LINE_NUMBER_IN_PROMPT = 2;

	private static final String BACKSLASH_R = "\r";

	/**
	 * Maximum length of a command send to Scilab
	 */
	private static final int MAX_CMD_LENGTH = 512;

	/**
	 * Configuration associated to the console oject
	 */
	private ConsoleConfiguration config;

	/**
	 * Scroll Pane used in Scilab Console
	 */
	private JScrollPane jSP;

	/**
	 * Generic console object
	 */
	private Console sciConsole;

	/**
	 * Flag indicating if the input command vieaw size has been forced to a value by a call to toHome
	 */
	private boolean inputCommandViewSizeForced;

	/**
	 * Value used to get one char from user input (when using [more y or n ?])
	 */
	private int userInputValue;

	/**
	 * Protection for userInputValue variable R/W
	 */
	private Semaphore canReadUserInputValue = new Semaphore(1);

	/**
	 * Boolean flag used to store the state of Scilab (true is all works done)
	 */
	private boolean workDone;

	/**
	 * Constructor
	 * @param configFilePath the configuration file to use
	 */
	public SciConsole(String configFilePath) {
		super(new BorderLayout());

		try {
			config = ConfigurationBuilder.buildConfiguration(configFilePath);
			config.setActiveProfile("scilab");
			if (System.getProperty("os.name").toLowerCase().indexOf("mac") != -1)
			{
				ConsoleConfiguration configMac = ConfigurationBuilder.buildConfiguration(configFilePath);;
				configMac.setActiveProfile("macosx");
				for (KeyStroke key : config.getKeyMapping().keys()){
					config.getKeyMapping().put(key,"");
				}
				for (KeyStroke key : configMac.getKeyMapping().keys()){
					config.getKeyMapping().put(key, configMac.getKeyMapping().get(key));
				}
			}
		} catch (IllegalArgumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (SAXException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		sciConsole = ConsoleBuilder.buildConsole(config, this);
		jSP = new JScrollPane(sciConsole);

		this.add(jSP, BorderLayout.CENTER);

		// The console is given to the outputView so that updateScrollPosition is is accessible
		((SciOutputView) config.getOutputView()).setConsole(this);

		// The console is given to the outputView so that Drag&Drop can work
		((SciInputCommandView) config.getInputCommandView()).setConsole(this);

		// The console is given to the CompletionWindow
		((SciCompletionWindow) config.getCompletionWindow()).setConsole(this);
		((SciCompletionWindow) config.getCompletionWindow()).setGraphicalContext(this);

		// The promptview is given to the Parsing Manager
		// Used to get the position of the CompletionWindow
		((SciInputParsingManager) config.getInputParsingManager()).setPromptView(this.getConfiguration().getPromptView());

		// Reset history settings - bug 3612
		((SciHistoryManager)config.getHistoryManager()).setInHistory(false);
	}

	/**
	 * Gets the configuration associated to the console
	 * @return the configuration
	 */
	public ConsoleConfiguration getConfiguration() {
		return config;
	}

	/**
	 * Sets the configuration associated to the console
	 * @param newConfig the new config to set
	 */
	public void setConfiguration(ConsoleConfiguration newConfig) {
		config = newConfig;
	}

	/**
	 * Updates Scilab internal variables containing the size of the console
	 * These variables are used to format data before displaying it
	 */
	public void scilabLinesUpdate() {
		// Size of the console
		int outputViewWidth = jSP.getWidth();

		// Size of a char
		OutputView outputView = this.getConfiguration().getOutputView();
		int[] charsWidth = ((JTextPane) outputView).getFontMetrics(((JTextPane) outputView).getFont()).getWidths();

		// This loop is not needed for monospaced fonts !
		int maxCharWidth = charsWidth[0];
		for (int i = 1; i < charsWidth.length; i++) {
		    if (charsWidth[i] > maxCharWidth) {
		    	maxCharWidth = charsWidth[i];
		    }
		}

		int numberOfLines = getNumberOfLines();
		int promptWidth = ((JPanel) this.getConfiguration().getPromptView()).getPreferredSize().width;

		int numberOfColumns = (outputViewWidth - promptWidth) / maxCharWidth - 1;
		/* -1 because of the margin between text prompt and command line text */

		GuiManagement.setScilabLines(numberOfLines, numberOfColumns);
	}

	/**
	 * Get the number of lines that can be displayed in the visible part of the console
	 * @return the number of lines
	 */
	public int getNumberOfLines() {
		// Size of the console
		int outputViewHeight = jSP.getHeight();

		// Size of a char
		OutputView outputView = this.getConfiguration().getOutputView();
		int charHeight = ((JTextPane) outputView).getFontMetrics(((JTextPane) outputView).getFont()).getHeight();
		int[] charsWidth = ((JTextPane) outputView).getFontMetrics(((JTextPane) outputView).getFont()).getWidths();

		// This loop is not needed for monospaced fonts !
		int maxCharWidth = charsWidth[0];
		for (int i = 1; i < charsWidth.length; i++) {
		    if (charsWidth[i] > maxCharWidth) {
		    	maxCharWidth = charsWidth[i];
		    }
		}

		return outputViewHeight / charHeight - 1; /* -1 because of the size of the InputCommandLine */
	}

    /**
     * Updates the scroll bars according to the contents
     */
    public void updateScrollPosition() {
    	SwingUtilities.invokeLater(new Runnable() {
    		public void run() {
    			jSP.getViewport().setViewPosition(new Point(0,
    					sciConsole.getPreferredSize().height - jSP.getViewport().getExtentSize().height));
				//jSP.revalidate();
			}	
		});	
		//jSP.getVerticalScrollBar().setValue(jSP.getVerticalScrollBar().getMaximum());
    	//jSP.invalidate();
    	//jSP.getViewport().setViewPosition(new Point(0, sciConsole.getPreferredSize().height - jSP.getViewport().getExtentSize().height));
 		//jSP.revalidate();
		
    	// Update the scrollbar properties
		jSP.getVerticalScrollBar().setBlockIncrement(jSP.getViewport().getExtentSize().height);
		jSP.getHorizontalScrollBar().setBlockIncrement(jSP.getViewport().getExtentSize().width);
   }

    /**
     * Clears the console and the output view
     */
    public void clear() {
		try {
            config.getInputCommandViewStyledDocument().remove(0, config.getInputCommandViewStyledDocument().getLength());
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
		config.getOutputView().reset();
		/* Bug 4014 */
		/* We add a space to add a line */
		/* clc , F2 and menus have same position */
		config.getOutputView().append(" ");
   }

    /**
     * Clears lines from the end of the output view
     * If nbLines == -1 ==> Called from SwingScilabConsole.getCharWithoutOutput() ([more y or n ?])
     * If nbLines == 0 ==> Clear the InputCommandLine
     * @param nbLines the number of lines to be deleted
     */
    public void clear(int nbLines) {

    	if (nbLines == 0) {
    		// Clear the prompt
    		config.getInputCommandView().reset();
    	} else {
    		// Clear lines in output command view
    		try {
    			// We have to remove the command entered by the user
    			int totalNumberOfLines = nbLines + LINE_NUMBER_IN_PROMPT;

    			StyledDocument outputStyle = config.getOutputViewStyledDocument();
    			String outputTxt =  outputStyle.getText(0, outputStyle.getLength());

    			// Are there enough lines in the output view ?
    			String[] allLines = outputTxt.split(StringConstants.NEW_LINE);
    			if (allLines.length < totalNumberOfLines) {
    				// Delete lines
    				config.getOutputView().reset();
    				config.getOutputView().append(Messages.gettext("Out of Screen"));
    			} else {
    				// Delete lines
    				int lastEOL;
    				for (int i = 0; i < totalNumberOfLines; i++) {
    					outputTxt = outputStyle.getText(0, outputStyle.getLength());
    					lastEOL = outputTxt.lastIndexOf(StringConstants.NEW_LINE);
    					outputStyle.remove(lastEOL, outputStyle.getLength() - lastEOL);
    				}
    			}
    		} catch (BadLocationException e) {
    			// TODO Auto-generated catch block
    			e.printStackTrace();
    		}
		}
    }

    /**
     * Puts the prompt in the top left corner of the console
     */
    public void toHome() {
    	Dimension jSPExtSize = jSP.getViewport().getExtentSize();
    	Dimension newDim = new Dimension(jSPExtSize.width - jSP.getVerticalScrollBar().getPreferredSize().width, jSPExtSize.height);
    	((JTextPane) config.getInputCommandView()).setPreferredSize(newDim);
    	((JTextPane) config.getInputCommandView()).invalidate();
    	((JTextPane) config.getInputCommandView()).doLayout();
    	inputCommandViewSizeForced = true;
   }

    /**
     * Sets the flags indicating if the input command view has been resize by calling toHome()
     * @param status the new status
     */
    public void setInputCommandViewSizeForced(boolean status) {
    	inputCommandViewSizeForced = status;
    }

    /**
     * Gets the flags indicating if the input command view has been resize by calling toHome()
     * @return true if a toHome() call is still affecting the size of the input command view
     */
    public boolean getInputCommandViewSizeForced() {
    	return inputCommandViewSizeForced;
    }

	/**
	 * Gets the user input value
	 * @return the value entered by the used
	 */
	public int getUserInputValue() {
		try {
			canReadUserInputValue.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		return userInputValue;
	}

	/**
	 * Sets the value entered by the user
	 * @param userInputValue new value
	 */
	public void setUserInputValue(int userInputValue) {
		this.userInputValue = userInputValue;
		canReadUserInputValue.release();
	}

	/**
	 * Gets the semaphore protection so that it can be acquired
	 * @return the semaphore
	 */
	public Semaphore getCanReadUserInputValue() {
		return canReadUserInputValue;
	}

	/**
	 * Send commands to be executed by Scilab (after a copy/paste or drag&drop...)
	 * @param textToExec all text lines to executed
	 * @param displayCmdInOutput flag indicating if the input command has to be displayed in the output view
	 * @param storeInHistory flag indicating if the input command has to be stored in the history
	 */
	public void sendCommandsToScilab(String textToExec, boolean displayCmdInOutput, boolean storeInHistory) {
		String[] linesToExec = textToExec.split(StringConstants.NEW_LINE);
		int nbStatements = 0;

		// Display Cursor to show Scilab is busy
		this.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

		while (nbStatements < linesToExec.length) {
			// This loop contains code very similar to the code of ValidationAction.java
			InputParsingManager inputParsingManager = config.getInputParsingManager();
			PromptView promptView = config.getPromptView();

			// Reset command line
			inputParsingManager.reset();
			promptView.updatePrompt();

			// Reset history settings
			config.getHistoryManager().setInHistory(false);

			// Hide the prompt and command line
			config.getInputCommandView().setEditable(false);
			config.getPromptView().setVisible(false);

			// Remove the prompt if present at the beginning of the text to execute
			// Bug 3002 fix: this "functionality" has been removed because:
			// - Remove the --> even if not from paste action
			// - Does not remove pause prompts

			// Store the command in the buffer so that Scilab can read it
			if (linesToExec[nbStatements].length() > MAX_CMD_LENGTH) {
				config.getOutputView().append("Command is too long (more than " + MAX_CMD_LENGTH
						+ " characters long): could not send it to Scilab\n");
				((SciInputCommandView) config.getInputCommandView()).setCmdBuffer("", false);
				return;
			}

			((SciInputCommandView) config.getInputCommandView())
					.setCmdBuffer(linesToExec[nbStatements].replace(BACKSLASH_R, ""), displayCmdInOutput);
			if (storeInHistory) {
				((SciHistoryManager) config.getHistoryManager()).addEntry(linesToExec[nbStatements].replace(BACKSLASH_R, ""));
			}
			nbStatements++;
		}

	}

	/**
	 * Intercept doLayout events to send the new size of the console to Scilab
	 * @see java.awt.Container#doLayout()
	 */
	public void doLayout() {
		super.doLayout();
		
		// Send the size of the console to Scilab to adjust display (if resized)
		scilabLinesUpdate();
		
    	// Update the scrollbar properties (if resized)
		jSP.getVerticalScrollBar().setBlockIncrement(jSP.getViewport().getExtentSize().height);
		jSP.getHorizontalScrollBar().setBlockIncrement(jSP.getViewport().getExtentSize().width);
	}

	/**
	 * Get the JScrollPane associated to the console
	 * @return the JScrollPane associated to the console
	 */
	public JScrollPane getJScrollPane() {
		return jSP;
	}

	/**
	 * Get the Console object associated to the console
	 * @return the Console object associated to the console
	 */
	public Console getSciConsole() {
		return sciConsole;
	}

	/**
	 * Get the current status of the console
	 * If the prompt view is visible, Scilab is waiting for commands
	 * @return true is Scilab is waiting for commands
	 */
	public boolean isWaitingForInput() {
		return ((JTextPane) config.getInputCommandView()).isEditable();
	}

	/**
	 * This methods is used by Scilab to get a new command to execute
	 * @return the command to execute
	 */
	public String readLine() {

		InputCommandView inputCmdView = this.getConfiguration().getInputCommandView();

		getConfiguration().getOutputView().setCaretPositionToEnd();
		
		displayPrompt();

		// Display Cursor to show Scilab is available.
		this.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));

		// Reads the buffer
		return ((SciInputCommandView) inputCmdView).getCmdBuffer();
	}

	/**
	 * This method is used to display the prompt
	 */
	public abstract void displayPrompt();

	/**
	 * Does Scilab have finished its work ?
	 * @return true if Scilab is waiting for new commands
	 */
	public boolean isWorkDone() {
		return workDone;
	}

	/**
	 * Set the font of the Console
	 * @param font the font to set
	 */
	public void setFont(Font font) {
		if (sciConsole != null) {
			sciConsole.setFont(font);
			
			/* Have to update the output view contents with new font */
			String txt;
			try {
				txt = config.getOutputViewStyledDocument().getText(0, config.getOutputViewStyledDocument().getLength());
				config.getOutputViewStyledDocument().remove(0, config.getOutputViewStyledDocument().getLength());
				config.getOutputView().append(txt);
			} catch (BadLocationException e) {
				System.out.println(Messages.gettext("Could not change the Console Font."));
				return;
			}
			
			/* Update the prompt */
			((JLabel) ((SciPromptView) config.getPromptView()).getPromptUI()).setFont(font);
			config.getPromptView().updatePrompt();
		}
	}

	/**
	 * Get the font of the Console
	 * @return the font
	 */
	public Font getFont() {
		if (sciConsole != null) {
			return ((JLabel) ((SciPromptView) config.getPromptView()).getPromptUI()).getFont();
		} else {
			return null;
		}
	}
	
	/**
	 * Get the Foreground Color of the Console
	 * @return the Foreground Color
	 */
	public Color getForeground() {
		if (sciConsole != null) {
			return sciConsole.getForeground();
		} else {
			return null;
		}
	}
	
	/**
	 * Get the Background Color of the Console
	 * @return the Background Color
	 */
	public Color getBackground() {
		if (sciConsole != null) {
			return sciConsole.getBackground();
		} else {
			return null;
		}
	}

	/**
	 * Set the Foreground Color of the Console
	 * @param color the Foreground Color
	 */
	public void setForeground(Color color) {
		if (sciConsole != null) {
			sciConsole.setForeground(color);
			
			/* Have to update the output view contents with new Foreground */
			String txt;
			try {
				txt = config.getOutputViewStyledDocument().getText(0, config.getOutputViewStyledDocument().getLength());
				config.getOutputViewStyledDocument().remove(0, config.getOutputViewStyledDocument().getLength());
				config.getOutputView().append(txt);
			} catch (BadLocationException e) {
				System.out.println(Messages.gettext("Could not change the Console Foreground."));
				return;
			}
			
			/* Update the prompt */
			((JLabel) ((SciPromptView) config.getPromptView()).getPromptUI()).setForeground(color);
			config.getPromptView().updatePrompt();
		}
	}
	
	/**
	 * Set the Background Color of the Console
	 * @param color the Background Color
	 */
	public void setBackground(Color color) {
		if (sciConsole != null) {
			sciConsole.setBackground(color);
		}
	}

}
