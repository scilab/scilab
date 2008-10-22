/* Copyright INRIA 2007 */

package window;

import java.awt.EventQueue;
import java.awt.event.*;

import javax.swing.UIManager;

import org.flexdock.util.SwingUtility;
import org.flexdock.docking.DockingManager;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.canvas.ScilabCanvas;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.frame.ScilabFrame;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;

/**
 * Basic test 
 * @author Marouane BEN JELLOUL
 */
public class WindowTest2 {

	/**
	 * Constructor
	 */
	protected WindowTest2() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Launch main WindowTest1
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
	 * Creates and show WindowTest2
	 */
	private static void createAndShowGui() {
		System.out.println("-*- Starting Program WindowTest2 -*-");

		Window mainView = ScilabWindow.createWindow();
		mainView.setTitle("WindowTest2");
		
		// menuBar
		MenuBar menuBar = ScilabMenuBar.createMenuBar();
		
		// menu File
		Menu menu = ScilabMenu.createMenu(); 
		menu.setText("File");
		menu.setMnemonic(KeyEvent.VK_F);
		MenuItem menuItem = ScilabMenuItem.createMenuItem();
		menuItem.setText("New Scilab");
		menuItem.setMnemonic(KeyEvent.VK_N);
		menu.add(menuItem);
		menu.addSeparator();
		menuItem = ScilabMenuItem.createMenuItem();
		menuItem.setText("Exec ...");
		menuItem.setMnemonic(KeyEvent.VK_E);
		menu.add(menuItem);
		menuBar.add(menu);
		
		// menu Edit
		menu = ScilabMenu.createMenu(); 
		menu.setText("Edit");
		menu.setMnemonic(KeyEvent.VK_E);
		menuItem = ScilabMenuItem.createMenuItem();
		menuItem.setText("Select All");
		menuItem.setMnemonic(KeyEvent.VK_S);
		menu.add(menuItem);
		menuBar.add(menu);
		menu.addSeparator();
		Menu subMenu = ScilabMenu.createMenu();
		subMenu.setText("Sub Menu");
		menuItem = ScilabMenuItem.createMenuItem();
		menuItem.setText("Sub Menu Item 1");
		subMenu.add(menuItem);
		menuItem = ScilabMenuItem.createMenuItem();
		menuItem.setText("Sub Menu Item 1");
		subMenu.add(menuItem);
		menu.add(subMenu);
		menuBar.add(menu);
		
		//System.out.println("mainView.menuBar is null : result = " + mainView.getMenuBar());
		mainView.addMenuBar(menuBar);
		//System.out.println("mainView.menuBar Added : result = " + mainView.getMenuBar());
		
		// toolBar
		ToolBar toolBar = ScilabToolBar.createToolBar();
		PushButton pushButton = ScilabPushButton.createPushButton();
		pushButton.setText("PB One");
		toolBar.add(pushButton);
		pushButton = ScilabPushButton.createPushButton();
		pushButton.setText("PB Two");
		toolBar.add(pushButton);

		//System.out.println("mainView.toolBar is null : result = " + mainView.getToolBar());
		mainView.addToolBar(toolBar);
		//System.out.println("mainView.toolBar Added : result = " + mainView.getToolBar());
		
//		 infoBar
		TextBox infoBar = ScilabTextBox.createTextBox();
		
		System.out.println("mainView.infoBar is null : result = " + mainView.getInfoBar());
		mainView.addInfoBar(infoBar);
		System.out.println("mainView.infoBar Added : result = " + mainView.getInfoBar());
		
//		adding Main tab to a window
		Tab mainTab = ScilabTab.createTab("Main");
		
		mainView.addTab(mainTab);
		
//		 adding Help tab to a window
		Tab helpTab = ScilabTab.createTab("Help");
		
		mainView.addTab(helpTab);

//		 adding Console tab to a window
		Tab consoleTab = ScilabTab.createTab("Console");
//		Console console = ScilabConsole.createConsole();
//		consoleTab.addMember(console);
		mainView.addTab(consoleTab);

//		 adding Canvas tab to a window
		Tab canvasTab = ScilabTab.createTab("Canvas");		
		Canvas canvas = ScilabCanvas.createCanvas();
		canvasTab.addMember(canvas);
		mainView.addTab(canvasTab);

//		creating Test tab
		Tab testTab = ScilabTab.createTab("Test");
		
//		adding a frame to the testTab
		Frame frame1 = ScilabFrame.createFrame();
		
		// pushButton on frame
		PushButton pushButton1 = ScilabPushButton.createPushButton();
		pushButton1.setText("push Button in a Frame");
		pushButton1.setPosition(new Position(0, 0));
		pushButton1.setDims(new Size(200, 20));
		frame1.addMember(pushButton1);
		
		testTab.addMember(frame1);
		
//		adding Test tab to a window
		mainView.addTab(testTab);
		// draw
		mainView.draw();
		
		mainView.getInfoBar().setText("Loaded WindowTest2");
		System.out.println("-*- Last Line Program WindowTest2 -*-");
	}	

}
