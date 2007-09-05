
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Point;
import java.io.IOException;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.text.BadLocationException;
import javax.swing.text.StyleContext;
import javax.swing.text.StyledDocument;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import com.artenum.console.interfaces.core.ConsoleConfiguration;
import com.artenum.console.interfaces.core.InputParsingManager;
import com.artenum.console.interfaces.ui.OutputView;
import com.artenum.console.interfaces.ui.PromptView;
import com.artenum.console.ui.Console;
import com.artenum.console.util.ConfigurationBuilder;
import com.artenum.console.util.ConsoleBuilder;
import com.artenum.console.util.StringConstants;

/**
 * Main class for Scilab Console based on Generic Console from Artenum
 * @author Vincent COUVERT
 */
public class SciConsole extends JPanel {
	
	private static final long serialVersionUID = 1L;
	
	private static final int LINE_NUMBER_IN_PROMPT = 2;
	
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
	 * Constructor
	 */
	public SciConsole() {
		super(new BorderLayout());
		String configFilePath = System.getenv("SCI") + "/modules/console/etc/configuration.xml";
		String profileName = null;
		
		try {
			config = ConfigurationBuilder.buildConfiguration(configFilePath);
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
		config.setActiveProfile(profileName);
		//
		
		sciConsole = ConsoleBuilder.buildConsole(config, this);
		jSP = new JScrollPane(sciConsole);
		this.add(jSP, BorderLayout.CENTER);

		// The console is given to the outputView so that updateScrollPosition is is accessible
		((SciOutputView) config.getOutputView()).setConsole(this);

		// The console is given to the outputView so that Drag&Drop can work
		((SciInputCommandView) config.getInputCommandView()).setConsole(this);
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
		int outputViewHeight = jSP.getHeight();
		int outputViewWidth = jSP.getWidth();
		
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
		
		int numberOfLines = outputViewHeight / charHeight - 1; /* -1 because of the size of the InputCommandLine */
		int promptWidth = ((JPanel) this.getConfiguration().getPromptView()).getPreferredSize().width;
		
		int numberOfColumns = (outputViewWidth - promptWidth) / maxCharWidth - 1; 
		/* -1 because of the margin between text prompt and command line text */

		GuiManagement.setScilabLines(numberOfLines, numberOfColumns);
	}
	
    /**
     * Updates the scroll bars according to the contents
     */
    public void updateScrollPosition() {
   		jSP.getViewport().setViewPosition(new Point(0, sciConsole.getHeight() - jSP.getViewport().getExtentSize().height));
		jSP.repaint();
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

        try {
            config.getOutputViewStyledDocument().remove(0, config.getOutputViewStyledDocument().getLength());
            config.getOutputViewStyledDocument().insertString(0, "", config.getOutputViewStyledDocument().getStyle(StyleContext.DEFAULT_STYLE));
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
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
    				config.getOutputView().append("Out of Screen");
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
    	((JPanel) config.getPromptView()).getParent().setPreferredSize(newDim);
    	((JPanel) config.getPromptView()).getParent().invalidate();
    	((JPanel) config.getPromptView()).getParent().doLayout();
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
			while (!canReadUserInputValue.tryAcquire(1, TimeUnit.MILLISECONDS)) {
				InterpreterManagement.execScilabEventLoop();
			}
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
	 */
	public void sendCommandsToScilab(String textToExec, boolean displayCmdInOutput) {
		String[] linesToExec = textToExec.split(StringConstants.NEW_LINE);
		int nbStatements = 0;
		boolean firstPrompt = true;
		
		while (nbStatements < linesToExec.length) {
		
			// Send data to the console only if the prompt is visible 
			boolean bufferAvailable = ((SciInputCommandView) config.getInputCommandView()).isEditable();

			if (bufferAvailable) {
				// This loop contains code very similar to the code of ValidationAction.java
				InputParsingManager inputParsingManager = config.getInputParsingManager();
				OutputView outputView = config.getOutputView();
				PromptView promptView = config.getPromptView();

				config.getInputCommandView().append(StringConstants.NEW_LINE);
				
				// Reset command line
				inputParsingManager.reset();
				promptView.updatePrompt();
				
				// Reset history settings
				config.getHistoryManager().setInHistory(false);
				
				// Hide the prompt and command line
				config.getInputCommandView().setEditable(false);
				config.getPromptView().setVisible(false);

				// Print the command in the output view
				outputView.setCaretPositionToEnd();
				
				// Remove the prompt if present at the beginning of the text to execute
				// TODO what about pause mode prompts ??
				String prompt = config.getPromptView().getDefaultPrompt();
				if (linesToExec[nbStatements].startsWith(prompt)) {
					linesToExec[nbStatements] = linesToExec[nbStatements].substring(prompt.length());
				}

				// TODO must be linked to Scilab parser to know if we are in a block
				if (displayCmdInOutput) {
					outputView.append(StringConstants.NEW_LINE);
					if (firstPrompt) {
						firstPrompt = false;
						outputView.append(promptView.getDefaultPrompt());
					} else {
						outputView.append(promptView.getInBlockPrompt());
					}
					outputView.append(linesToExec[nbStatements]);

					outputView.append(StringConstants.NEW_LINE);
				}
				// Store the command in the buffer so that Scilab can read it
				((SciInputCommandView) config.getInputCommandView()).setCmdBuffer(linesToExec[nbStatements]);
				((SciHistoryManager) config.getHistoryManager()).addEntry(linesToExec[nbStatements]);

				nbStatements++;
			}
		}
		
	}
	
	/**
	 * Intercept doLayout events to send the new size of the console to Scilab
	 * @see java.awt.Container#doLayout()
	 */
	public void doLayout() {
		super.doLayout();
		// Send the size of the console to Scilab to adjust display
		scilabLinesUpdate();
	}
}
