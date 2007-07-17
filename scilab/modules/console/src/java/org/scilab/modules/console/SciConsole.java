
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Point;
import java.io.IOException;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.text.BadLocationException;
import javax.swing.text.StyleContext;
import javax.swing.text.StyledDocument;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import com.artenum.console.interfaces.core.ConsoleConfiguration;
import com.artenum.console.interfaces.ui.OutputView;
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
		int promptWidth = ((JLabel) this.getConfiguration().getPromptView()).getWidth();
		int numberOfColumns = (outputViewWidth - promptWidth) / maxCharWidth;
		
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
     * @param nbLines the number of lines to be deleted
     */
    public void clear(int nbLines) {
    	try {
    		// We have to remove the command entered by the user
    		int totalNumberOfLines = nbLines + LINE_NUMBER_IN_PROMPT;
    		
    		StyledDocument outputStyle = config.getOutputViewStyledDocument();
    		String outputTxt =  outputStyle.getText(0, outputStyle.getLength());
			
			// Are there enough lines in the output view ?
			String[] allLines = outputTxt.split(StringConstants.NEW_LINE);
			if (allLines.length < totalNumberOfLines) {
				// TODO : error message in console
				System.out.println("Out of Screen");
			} else {
				// Delete lines
				for (int i = 0; i < totalNumberOfLines; i++) {
					int lastEOL = outputStyle.getText(0, outputStyle.getLength()).lastIndexOf(StringConstants.NEW_LINE);
					outputStyle.remove(lastEOL, outputStyle.getLength() - lastEOL);
				}
			}
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
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
}
