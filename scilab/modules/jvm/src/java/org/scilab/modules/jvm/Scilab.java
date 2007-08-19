package org.scilab.modules.jvm;

import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.utils.LookAndFeel;
//import org.scilab.modules.gui.utils.Size;




/**
 * Main Class for Scilab
 * @author Allan CORNET INRIA 2007
  */
public class Scilab {
	
	private static final int DEFAULTWIDTH = 500;
	private static final int DEFAULTHEIGHT = 500;

	private Console sciConsole = null;
	private int mode;
	
	private Window mainView;
	
	 /**
	 * Constructor Scilab Class.
	 * @param mode Mode Scilab -NW -NWNI -STD -API
	 */
	public Scilab(int mode) {
		this.mode = mode;
	
		if (mode == 2) {
			mainView = ScilabWindow.createWindow();
			mainView.draw();
			mainView.setTitle("Scilab-5.0");
			/* CONSOLE */
			/* Create a tab to put console into */
			Tab consoleTab = ScilabTab.createTab("Console Scilab");
			consoleTab.setName("Console");
			mainView.addTab(consoleTab);
			/* Create the console */
			sciConsole = ScilabConsole.createConsole();
			consoleTab.addMember(sciConsole);
			mainView.draw();
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
	
	public Window getMainWindow()
	{
		return mainView;
	}
}
/*--------------------------------------------------------------------------*/

