
/* Copyright INRIA 2007*/

package org.scilab.modules.jvm;

import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.utils.LookAndFeel;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.utils.ToolBarBuilder;

/**
 * Main Class for Scilab
 * @author Allan CORNET
 * @author Vincent COUVERT
 * @author Sylvestre Ledru INRIA 2007
 * @author Bruno JOFRET
 */
public class Scilab {

	private static final String CLASS_NOT_FOUND = "Could not find class: ";
	private static final String FILE_NOT_FOUND = "Could not find file: ";

	private static final String CANNOT_CREATE_MENUBAR = "Cannot create Figure MenuBar.\n"
							+ "Check if file main_menubar.xml is available and valid.";
	private static final String CANNOT_CREATE_TOOLBAR = "Cannot create Figure ToolBar.\n"
							+ "Check if file main_toolbar.xml is available and valid.";

	private static final String SCIDIR = System.getenv("SCI");

	private static final String MENUBARXMLFILE = SCIDIR + "/modules/gui/etc/main_menubar.xml";
	private static final String TOOLBARXMLFILE = SCIDIR + "/modules/gui/etc/main_toolbar.xml";

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

		if (mode == 2) { /* Mode GUI */

			try {
				mainView = ScilabWindow.createWindow();
				mainView.draw();
				mainView.setTitle("Scilab-5.0");
			} catch (NoClassDefFoundError exception) {
				System.err.println("Cannot create Scilab Window.\nCheck if the thirdparties are available (Flexdock, JOGL...).\nSee SCI/etc/classpath.xml for default paths.");
				System.err.println(CLASS_NOT_FOUND + exception.getLocalizedMessage());
				System.exit(-1);
			}

			/************/
			/* MENU BAR */
			/************/
			MenuBar menuBar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE);

			/************/
			/* TOOL BAR */
			/************/
			ToolBar toolBar = ToolBarBuilder.buildToolBar(TOOLBARXMLFILE);

			/* Create the console */
			Tab consoleTab = null;
			try {
				/* CONSOLE */
				/* Create a tab to put console into */
				consoleTab = ScilabTab.createTab("Console Scilab");
				consoleTab.setName("Console");

				sciConsole = ScilabConsole.createConsole();
				//consoleTab.addMember(sciConsole);
			} catch (NoClassDefFoundError exception) {
				System.err.println("Cannot create Scilab Console.\nCheck if the thirdparties are available (JoGL/JRosetta...).\nSee SCI/etc/classpath.xml for default paths.");
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

			/** Adding content into container */
			sciConsole.addToolBar(toolBar);
			sciConsole.addMenuBar(menuBar);
			consoleTab.addMember(sciConsole);
			mainView.addTab(consoleTab);
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

