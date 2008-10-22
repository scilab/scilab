/* Copyright INRIA 2007 */

package window;

import java.awt.EventQueue;
import java.awt.event.*;

import javax.swing.UIManager;


import org.flexdock.util.SwingUtility;
import org.flexdock.docking.DockingManager;
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
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;

/**
 * Basic test 
 * @author Marouane BEN JELLOUL
 */
public class WindowTest1 {

	/**
	 * Constructor
	 */
	protected WindowTest1() {
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
	 * Creates and show WindowTest1
	 */
	private static void createAndShowGui() {
		System.out.println("-*- Starting Program WindowTest1 -*-");

		Window mainView = ScilabWindow.createWindow();
		mainView.setTitle("WindowTest1");
		
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
		
		// adding Main tab to a window
		Tab mainTab = ScilabTab.createTab("Main");
		
		mainView.addTab(mainTab);
		
//		 adding Help tab to a window
		Tab helpTab = ScilabTab.createTab("Help");
		
		mainView.addTab(helpTab);
		
//		 adding Graph tab to a window
		Tab graphTab = ScilabTab.createTab("Graph");
		
		mainView.addTab(graphTab);
		
		// draw
		mainView.draw();
		
		mainView.getInfoBar().setText("Loaded WindowTest1");
		System.out.println("-*- Last Line Program WindowTest1 -*-");
	}	

}
