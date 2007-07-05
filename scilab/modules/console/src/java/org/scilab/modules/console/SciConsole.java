
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.BorderLayout;
import java.io.IOException;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import com.artenum.console.interfaces.core.ConsoleConfiguration;
import com.artenum.console.interfaces.ui.OutputView;
import com.artenum.console.ui.Console;
import com.artenum.console.util.ConfigurationBuilder;
import com.artenum.console.util.ConsoleBuilder;

/**
 * Main class for Scilab Console based on Generic Console from Artenum
 * @author Vincent COUVERT
 */
public class SciConsole extends JPanel {
	
	private static final long serialVersionUID = 1L;
	
	/**
	 * Configuration associated to the console oject
	 */
	private ConsoleConfiguration config;
	
	/**
	 * Scroll Pane used in Scilab Console
	 */
	private JScrollPane jSP;
	
	/**
	 * Constructor
	 */
	public SciConsole() {
		super(new BorderLayout());
		String configFilePath = System.getenv("SCI") + "/modules/console/etc/configuration.xml";
		String profileName = null;
		Console sciConsole = null;
		
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

}
