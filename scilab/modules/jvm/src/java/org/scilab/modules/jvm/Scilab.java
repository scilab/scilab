
/* Copyright INRIA 2007*/

package org.scilab.modules.jvm;

import java.io.IOException;

import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;

import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.utils.LookAndFeel;
import org.scilab.modules.gui.utils.MenuBarBuilder;

/**
 * Main Class for Scilab
 * @author Allan CORNET
 * @author Vincent COUVERT
 * @author Sylvestre Ledru INRIA 2007
  */
public class Scilab {
	
	private static final String CLASS_NOT_FOUND = "Could not find class: ";
	private static final String FILE_NOT_FOUND = "Could not find file: ";
	private static final String CANNOT_CREATE_MENUBAR = "Cannot create Scilab MenuBar.\nCheck if file main_menus.xml is available and valid.";
	
	private static final String MENUSXMLFILE = System.getenv("SCI") + "/modules/gui/etc/main_menus.xml";
	
	private static final int DEFAULTWIDTH = 500;
	private static final int DEFAULTHEIGHT = 500;

	private Console sciConsole;
	private int mode;
	
	private Window mainView;
	
	 /**
	 * Constructor Scilab Class.
	 * @param mode Mode Scilab -NW -NWNI -STD -API
	 */
	public Scilab(int mode) {
		this.mode = mode;
		LookAndFeel.setSystemLookAndFeel();
		if (mode == 2) {

			try {
				mainView = ScilabWindow.createWindow();
				mainView.draw();
				mainView.setTitle("Scilab-5.0");
			} catch (NoClassDefFoundError exception) {
				System.err.println("Cannot create Scilab Window.\nCheck if the thirdparties are available (Flexdock, JOGL...).");
				System.err.println(CLASS_NOT_FOUND + exception.getLocalizedMessage());
				System.exit(-1);
			}
			
			/************/
			/* MENU BAR */
			/************/
			MenuBar menuBar = null;
			try {
				menuBar = MenuBarBuilder.buildMenuBar(MENUSXMLFILE);
				mainView.addMenuBar(menuBar);
			} catch (IllegalArgumentException e) {
				System.err.println(CANNOT_CREATE_MENUBAR);
				System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
				System.exit(-1);
			} catch (SAXException e) {
				System.err.println(CANNOT_CREATE_MENUBAR);
				System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
				System.exit(-1);
			} catch (IOException e) {
				System.err.println(CANNOT_CREATE_MENUBAR);
				System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
				e.printStackTrace();
			} catch (ParserConfigurationException e) {
				System.err.println(CANNOT_CREATE_MENUBAR);
				System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
				System.exit(-1);
			}
			
			/* CONSOLE */
			/* Create a tab to put console into */
			Tab consoleTab = ScilabTab.createTab("Console Scilab");
			consoleTab.setName("Console");
			
			mainView.addTab(consoleTab);

			/* Create the console */
			try {
				sciConsole = ScilabConsole.createConsole();
				consoleTab.addMember(sciConsole);
			} catch (NoClassDefFoundError exception) {
				System.err.println("Cannot create Scilab Console.\nCheck if the thirdparties are available (Rosetta/Jrosetta...).");
				System.err.println(CLASS_NOT_FOUND + exception.getLocalizedMessage());
				System.exit(-1);
			}

			try {
				mainView.draw();
			} catch (UnsatisfiedLinkError exception) {
				System.err.println("Problem while loading the native library (link not resolved)");
				System.err.println(exception.getLocalizedMessage());
				System.exit(-1);
			}
		} else {
			System.out.println("mode -NW");
		}
		
	}
	
	/**
	 * Gets the console object associated to Scilab
	 * @return the console
	 */
	public Console getSciConsole() {
		return sciConsole;
	}
	
	/**
	 * Sets the console object associated to Scilab
	 * @param sciConsole the console to associate to Scilab
	 */
	public void setSciConsole(Console sciConsole) {
		this.sciConsole = sciConsole;
	}
	
	/**
	 * Sets the prompt displayed in Scilab console
	 * @param prompt the prompt to be displayed as a String
	 */
	public void setPrompt(String prompt) {
		this.sciConsole.setPrompt(prompt);
	}
	
	/**
	 * Get main Scilab Window
	 * @return main Scilab window
	 */
	public Window getMainWindow() {
		return mainView;
	}

}
/*--------------------------------------------------------------------------*/

