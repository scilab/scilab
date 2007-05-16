package org.scilab.modules.jvm;

import org.scilab.modules.console.ScilabConsole;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;

/**
 * Main Class for Scilab
 * @author Allan CORNET INRIA 2007
  */
public class Scilab {
	
	private static final int DEFAULTWIDTH = 500;
	private static final int DEFAULTHEIGHT = 500;

	//private ScilabConsole console;
	
	private int mode;
	/**
	 * Constructor Scilab Class.
	 * @param mode Mode Scilab -NW -NWNI -STD -API
	 */
	public Scilab(int mode) {
		this.mode = mode;
		//console = new ScilabConsole();
        //console.printf("First Step\n");
        
		Window mainView = ScilabWindow.createWindow();
		mainView.draw();

		/* CONSOLE */
//		newWindowSize = new Size(DEFAULTWIDTH, DEFAULTHEIGHT);
//		mainView.setDims(newWindowSize);
//		/* Create a tab to put console into */
//		Tab consoleTab = ScilabTab.createTab("Console Scilab");
//		//consoleTab.setName("Console");
//		mainView.addTab(consoleTab);
//		/* Create the console */
//		Console sciConsole = ScilabConsole.createConsole();
//		consoleTab.addMember(sciConsole);
//		mainView.draw();
//
//		/* FIGURE */
//		/* Create the tab to put a canvas into */
//		Tab figureTab = ScilabTab.createTab("Scilab Figure");
//		//figureTab.setName("Scilab Figure");
//		mainView.addTab(figureTab);
//
//		/* Create the canvas */
//		Canvas figureCanvas = ScilabCanvas.createCanvas();
//		figureCanvas.setDims(new Size(DEFAULTWIDTH, DEFAULTHEIGHT));
//		figureCanvas.draw();
//		figureTab.addMember(figureCanvas);
//		figureTab.draw();
//		mainView.draw();

	}
}
/*--------------------------------------------------------------------------*/

