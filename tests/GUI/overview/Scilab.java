
/* Copyright INRIA 2007 */


package overview;

import java.awt.EventQueue;

import javax.swing.UIManager;

import org.flexdock.docking.DockingManager;
import org.flexdock.util.SwingUtility;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.canvas.ScilabCanvas;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;

/**
 * Test class for Scilab 5.0 GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class Scilab {

	private static final int NEWWIDTH = 100;
	private static final int NEWHEIGHT = 100;

	private static final int NEWXPOSITION = 100;
	private static final int NEWYPOSITION = 100;

	private static final int DEFAULTWIDTH = 500;
	private static final int DEFAULTHEIGHT = 500;

	private static final String X = "x";
	private static final String AND_Y_EQUAL = "and y=";
	
	/**
	 * Constructor
	 */
	protected Scilab() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Launch main Scilab
	 * @param args not used
	 */
	public static void main(String[] args) {
		SwingUtility.setPlaf(UIManager.getSystemLookAndFeelClassName());

		DockingManager.setFloatingEnabled(true);

		EventQueue.invokeLater(new Runnable() {
			public void run() {
				createAndShowGui();
			}
		});
	}

	/**
	 * Creates and show Scilab main GUI
	 */
	private static void createAndShowGui() {
		System.out.println("-*- Starting Program -*-");

		Window mainView = ScilabWindow.createWindow();
		mainView.draw();

		/* DIMENSIONS */
		/* Defaut dimensions */
		Size windowSize = mainView.getDims();
		System.out.println("Window Size = " + windowSize.getWidth() + X + windowSize.getHeight());
		/* User defined dimensions */
		Size newWindowSize = new Size(NEWWIDTH, NEWHEIGHT);
		mainView.setDims(newWindowSize);
		windowSize = mainView.getDims();
		System.out.println("-*- Has size changed ??? -*-");
		System.out.println("Window Size: " + windowSize.getWidth() + X + windowSize.getHeight());

		/* POSITION */
		/* Default position */
		Position windowPosition = mainView.getPosition();
		System.out.println("Window Position: x=" + windowPosition.getX() + AND_Y_EQUAL + windowPosition.getY());
		/* User defined position */
		Position newWindowPosition = new Position(NEWXPOSITION, NEWYPOSITION);
		mainView.setPosition(newWindowPosition);
		windowPosition = mainView.getPosition();
		System.out.println("-*- Has position changed ??? -*-");
		System.out.println("New Window Position: x=" + windowPosition.getX() + AND_Y_EQUAL + windowPosition.getY());

		/* TITLE */
		System.out.println("Window Title: " + mainView.getTitle());
		mainView.setTitle("Scilab IHM in Java");
		System.out.println("-*- Has title changed ??? -*-");
		System.out.println("New Window Title: " + mainView.getTitle());

		/* VISIBILITY */
		System.out.println("Window is visible ? (YES=default value): " + mainView.isVisible());
		mainView.setVisible(false);
		System.out.println("Window is visible ? (NO): " + mainView.isVisible());
		mainView.setVisible(true);
		System.out.println("Window is visible ? (YES): " + mainView.isVisible());

		/* CONSOLE */
		mainView.setVisible(true); /* Added because the windows resize below does not work !! */
		newWindowSize = new Size(DEFAULTWIDTH, DEFAULTHEIGHT);
		mainView.setDims(newWindowSize);
		/* Create a tab to put console into */
		Tab consoleTab = ScilabTab.createTab("Console Scilab");
		//consoleTab.setName("Console");
		mainView.addTab(consoleTab);
		/* Create the console */
		Console sciConsole = ScilabConsole.createConsole();
		consoleTab.addMember(sciConsole);
		mainView.draw();

		/* FIGURE */
		/* Create the tab to put a canvas into */
		Tab figureTab = ScilabTab.createTab("Scilab Figure");
		//figureTab.setName("Scilab Figure");
		mainView.addTab(figureTab);

		/* Create the Frame */
		//Frame canvasFrame = ScilabFrame.createFrame();
		/* Create the canvas */
		Canvas figureCanvas = ScilabCanvas.createCanvas();
		figureCanvas.setDims(new Size(DEFAULTWIDTH, DEFAULTHEIGHT));
		figureCanvas.draw();
		//canvasFrame.addMember(figureCanvas);
		//canvasFrame.setDims(new Size(DEFAULTWIDTH, DEFAULTHEIGHT));
		//canvasFrame.draw();
		//figureTab.addMember(canvasFrame);
		figureTab.addMember(figureCanvas);
		figureTab.draw();
		mainView.draw();
	}
}
