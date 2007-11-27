
/* Copyright INRIA 2007*/

package org.scilab.modules.jvm;

import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.utils.LookAndFeel;





/**
 * Main Class for Scilab
 * @author Allan CORNET
 * @author Vincent COUVERT
 * @author Sylvestre Ledru INRIA 2007
  */
public class Scilab {
	
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
			}catch (NoClassDefFoundError exception){
				System.err.println("Cannot create Scilab Window.\nCheck if the thirdparties are available (Flexdock, JOGL...).");
				System.err.println("Could not find class: "+exception.getLocalizedMessage());
				System.exit(-1);
			}
			
			/* MENU BAR */
			MenuBar menuBar = ScilabMenuBar.createMenuBar();
			//mainView.addMenuBar(menuBar);

			Menu fileMenu = ScilabMenu.createMenu();
			fileMenu.setText("File");
			menuBar.add(fileMenu);
			
			MenuItem quitScilab = ScilabMenuItem.createMenuItem();
			quitScilab.setText("Exit");
			quitScilab.setMnemonic('E');
			quitScilab.setCallback("exit();");
			fileMenu.add(quitScilab);
			
			/* CONSOLE */
			/* Create a tab to put console into */
			Tab consoleTab = ScilabTab.createTab("Console Scilab");
			consoleTab.setName("Console");
			
            // Special case because the menuBar is associated to the mainView and not to the tab
			// TODO This could perhaps be done in mainView.addTab ???
			//consoleTab.addMenuBar(menuBar); 
			//consoleTab.setMenuBarId(menuBar.getAsSimpleMenuBar().getElementId());
			
			mainView.addTab(consoleTab);

			/* Create the console */
			try {
				sciConsole = ScilabConsole.createConsole();
				consoleTab.addMember(sciConsole);
			} catch (NoClassDefFoundError exception) {
				System.err.println("Cannot create Scilab Console.\nCheck if the thirdparties are available (Rosetta/Jrosetta...).");
				System.err.println("Could not find class: " + exception.getLocalizedMessage());
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

