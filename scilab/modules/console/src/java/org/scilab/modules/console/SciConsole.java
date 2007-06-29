
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.BorderLayout;
import java.io.IOException;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import com.artenum.console.interfaces.core.ConsoleConfiguration;
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
	 * Configuration associated to the console ojetc
	 */
	private ConsoleConfiguration config;

	/**
	 * Constructor
	 */
	public SciConsole() {
		super(new BorderLayout());
		String configFilePath = System.getenv("SCI") + "/modules/console/etc/configuration.xml";
		String profileName = null;
		Console sciConsole = null;
		JScrollPane jSP = null;
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
}
