package org.scilab.modules.jvm;

import java.io.File;

import org.scilab.modules.shell.ScilabShell;
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

	public Console sciConsole;
	private int mode;
	
	  static {
		  System.loadLibrary("sciconsole");
		  }
	  /**
	 * Constructor Scilab Class.
	 * @param mode Mode Scilab -NW -NWNI -STD -API
	 */
	public Scilab(int mode) {
		this.mode = mode;
		/*
		 *	 by default we used O.S Look and Feel
		 */		
		LookAndFeel.setSystemLookAndFeel();
		
		if (mode == 2) {
			Window mainView = ScilabWindow.createWindow();
			mainView.draw();
			/* CONSOLE */
			/* Create a tab to put console into */
			Tab consoleTab = ScilabTab.createTab("Console Scilab");
			consoleTab.setName("Console");
			mainView.addTab(consoleTab);
			/* Create the console */
			sciConsole = ScilabConsole.createConsole();
			consoleTab.addMember(sciConsole);
			mainView.draw();
			/* FIGURE */
			/* Create the tab to put a canvas into */
			//Tab figureTab = ScilabTab.createTab("Scilab Figure");
// 			/figureTab.setName("Scilab Figure");
// 			mainView.addTab(figureTab);

// 			/* Create the canvas */
// 			Canvas figureCanvas = ScilabCanvas.createCanvas();
// 			figureCanvas.setDims(new Size(DEFAULTWIDTH, DEFAULTHEIGHT));
// 			figureCanvas.draw();
// 			figureTab.addMember(figureCanvas);
// 			figureTab.draw();
// 			mainView.draw();
		} else {
			System.out.println("mode -NW");
		}
		
	}
}
/*--------------------------------------------------------------------------*/

