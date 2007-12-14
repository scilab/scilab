
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge;

import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.graphicWindow.ScilabRendererProperties;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBarBridge;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;
import org.scilab.modules.gui.tab.ScilabTabBridge;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.renderer.FigureMapper;

/**
 * This class is used to call Scilab GUIs objects from Scilab
 * @author Vincent COUVERT
 */
public class CallScilabBridge {

	/**
	 * Constructor
	 */
	protected CallScilabBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}
	
	/**********************************/
	/* Methods used to create objects */
	/**********************************/
	
	/**
	 * Create a new Window in Scilab GUIs
	 * @return the ID of the window in the UIElementMapper
	 */
	public static int newWindow() {
		Window window = ScilabWindow.createWindow();
		return UIElementMapper.add(window);
	}

	/**
	 * Create a new Menubar in Scilab GUIs
	 * @return the ID of the Menubar in the UIElementMapper
	 */
	public static int newMenuBar() {
		MenuBar menuBar = ScilabMenuBar.createMenuBar();
		return UIElementMapper.add(menuBar);
	}

	/**
	 * Create a new Menu in Scilab GUIs
	 * @return the ID of the menu in the UIElementMapper
	 */
	public static int newMenu() {
		Menu menu = ScilabMenu.createMenu();
		return UIElementMapper.add(menu);
	}
	
	/**
	 * Create a new File Chooser in Scilab GUIs
	 * @return the ID of the File Chooser in the UIElementMapper
	 */
	public static int newFileChooser() {
		FileChooser fileChooser = ScilabFileChooser.createFileChooser();
		return UIElementMapper.add(fileChooser);
	}
	
	/**
	 * Create a new PushButton in Scilab GUIs
	 * @return the ID of the PushButton in the UIElementMapper
	 */
	public static int newPushButton() {
		PushButton pushButton = ScilabPushButton.createPushButton();
		return UIElementMapper.add(pushButton);
	}

	/**
	 * Set the dimensions of an object in Scilab GUIs
	 * @param objID the ID of the object in the UIElementMapper
	 * @param width the width of the object
	 * @param height the height of the object
	 */
	public static void setDims(int objID, int width, int height) {
		UIElementMapper.getCorrespondingUIElement(objID).setDims(new Size(width, height));
	}
	
	/**
	 * Set the dimensions of a menu in Scilab GUIs
	 * @param objID the ID of the menu in the UIElementMapper
	 * @param text the text to set to the menu
	 */
	public static void setMenuText(int objID, String text) {
		// TODO do not cast !
		((Menu) UIElementMapper.getCorrespondingUIElement(objID)).setText(text);
	}
	
	/**
	 * Set the dimensions of a pushbutton in Scilab GUIs
	 * @param objID the ID of the pushbutton in the UIElementMapper
	 * @param text the text to set to the pushbutton
	 */
	public static void setPushButtonText(int objID, String text) {
		// TODO do not cast !
		((PushButton) UIElementMapper.getCorrespondingUIElement(objID)).setText(text);
	}
	
	/**
	 * Set the dimensions of an object in Scilab GUIs
	 * @param objID the ID of the object in the UIElementMapper
	 * @return the label of the Menu
	 */
	public static String getMenuText(int objID) {
		// TODO do not cast !
		return ((Menu) UIElementMapper.getCorrespondingUIElement(objID)).getText();
	}
	
	/**
	 * Set the dimensions of an object in Scilab GUIs
	 * @param objID the ID of the object in the UIElementMapper
	 * @return the label of the Menu
	 */
	public static String getPushButtonText(int objID) {
		// TODO do not cast !
        return ((PushButton) UIElementMapper.getCorrespondingUIElement(objID)).getText();
	}

	/*****************************/
	/* Parent setting properties */
	/*****************************/

	/**
	 * Set a figure as parent for an UIElement
	 * @param figureID the ID of the figure in the FigureMapper
	 * @param objID the ID of the object in the UIElementMapper
	 */
	public static void setFigureAsParent(int figureID, int objID) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();

		MenuBar menuBar;
		int menuBarId;

		// Create a menuBar if not already one associated to the parentTab
		if (parentTab.getMenuBarId() == UIElementMapper.getDefaultId()) {
			menuBar = ScilabMenuBar.createMenuBar();
			menuBarId = UIElementMapper.add(menuBar);

			parentTab.addMenuBar(menuBar);
			parentTab.setMenuBarId(menuBarId);
		} else {
			menuBar = (MenuBar) UIElementMapper.getCorrespondingUIElement(parentTab.getMenuBarId());
			menuBarId = parentTab.getMenuBarId();
		}
		// Add the menu to the tab
		ScilabMenuBarBridge.add(menuBar, (Menu) UIElementMapper.getCorrespondingUIElement(objID));

		// If parent tab is the currently "on top" tab, then the MenuBar is also added to the parent window
		if (ScilabTabBridge.isCurrentTab(parentTab)) {
			UIElementMapper.getCorrespondingUIElement(ScilabTabBridge.getParentWindowId(parentTab)).addMenuBar(menuBar);
			UIElementMapper.getCorrespondingUIElement(ScilabTabBridge.getParentWindowId(parentTab)).setMenuBarId(menuBarId);
		}
	}
	
	/**
	 * Set a figure as parent for a PushButton
	 * @param figureID the ID of the figure in the FigureMapper
	 * @param objID the ID of the PushButton in the UIElementMapper
	 */
	public static void setPushButtonParent(int figureID, int objID) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		//parentTab.addMember((PushButton) UIElementMapper.getCorrespondingUIElement(objID));
		PushButton pushButton = (PushButton) UIElementMapper.getCorrespondingUIElement(objID);
		ScilabBridge.addMember(parentTab, pushButton);
	}

	/**
	 * Set root Scilab object (the console tab) as the parent of the menu
	 * @param objID the id of the menu
	 */
	public static void setRootAsParent(int objID) {
		int consoleTabId = UIElementMapper.getConsoleId();
		Tab consoleTab = (Tab) UIElementMapper.getCorrespondingUIElement(consoleTabId);

		MenuBar menuBar;
		int menuBarId;

		// Create a menuBar if not already one associated to the parentTab
		if (consoleTab.getMenuBarId() == UIElementMapper.getDefaultId()) {
			menuBar = ScilabMenuBar.createMenuBar();
			menuBarId = UIElementMapper.add(menuBar);

			consoleTab.addMenuBar(menuBar);
			consoleTab.setMenuBarId(menuBarId);
		} else {
			menuBar = (MenuBar) UIElementMapper.getCorrespondingUIElement(consoleTab.getMenuBarId());
			menuBarId = consoleTab.getMenuBarId();
		}
		// Add the menu to the tab
		ScilabMenuBarBridge.add(menuBar, (Menu) UIElementMapper.getCorrespondingUIElement(objID));

		// If parent tab is the currently "on top" tab, then the MenuBar is also added to the parent window
		if (ScilabTabBridge.isCurrentTab(consoleTab)) {
			UIElementMapper.getCorrespondingUIElement(ScilabTabBridge.getParentWindowId(consoleTab)).addMenuBar(menuBar);
			UIElementMapper.getCorrespondingUIElement(ScilabTabBridge.getParentWindowId(consoleTab)).setMenuBarId(menuBarId);
		}
		
	}
	
	/**
	 * Set an other menu as the parent of the menu
	 * @param menuID the id of the parent menu
	 * @param objID the id of the menu
	 */
	public static void setMenuAsParent(int menuID, int objID) {
		Menu parentMenu = (Menu) UIElementMapper.getCorrespondingUIElement(menuID);
		Menu menu = (Menu) UIElementMapper.getCorrespondingUIElement(objID);
		ScilabBridge.add(parentMenu, menu);
	}

	/**
	 * Set a callback for a Menu
	 * @param objID the ID of the object in the UIElementMapper
	 * @param callback the text of the callback
	 */
	public static void setMenuCallback(int objID, String callback) {
		System.out.println("setMenuCallback is not implemented");
	}
	
	/**
	 * Disable a menu of a Scilab figure giving its name
	 * @param figureID the id of the figure
	 * @param menuName the name of the menu
	 * @param status true to set the menu enabled
	 */
	public static void setFigureMenuEnabled(int figureID, String menuName, boolean status) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		
		MenuBar figureMenuBar = (MenuBar) UIElementMapper.getCorrespondingUIElement(parentTab.getMenuBarId());
		
		figureMenuBar.getAsSimpleMenuBar().setMenuEnabled(menuName, status);
	}

	/**
	 * Disable a menu of a Scilab root window giving its name
	 * @param menuName the name of the menu
	 * @param status true to set the menu enabled
	 */
	public static void setRootMenuEnabled(String menuName, boolean status) {
		int consoleTabId = UIElementMapper.getConsoleId();
		Tab consoleTab = (Tab) UIElementMapper.getCorrespondingUIElement(consoleTabId);
		
		MenuBar consoleMenuBar = (MenuBar) UIElementMapper.getCorrespondingUIElement(consoleTab.getMenuBarId());

		consoleMenuBar.getAsSimpleMenuBar().setMenuEnabled(menuName, status);
	}
	
	/**
	 * Delete a menu of a Scilab figure giving its name
	 * @param figureID the id of the figure
	 * @param menuName the name of the menu
	 */
	public static void removeFigureMenu(int figureID, String menuName) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		
		MenuBar figureMenuBar = (MenuBar) UIElementMapper.getCorrespondingUIElement(parentTab.getMenuBarId());
		
		figureMenuBar.getAsSimpleMenuBar().removeMenu(menuName);
	}

	/**
	 * Delete a menu of a Scilab root window giving its name
	 * @param menuName the name of the menu
	 */
	public static void removeRootMenu(String menuName) {
		int consoleTabId = UIElementMapper.getConsoleId();
		Tab consoleTab = (Tab) UIElementMapper.getCorrespondingUIElement(consoleTabId);
		
		MenuBar consoleMenuBar = (MenuBar) UIElementMapper.getCorrespondingUIElement(consoleTab.getMenuBarId());
		
		if (consoleMenuBar != null) {
			consoleMenuBar.getAsSimpleMenuBar().removeMenu(menuName);
		}
	}
	
	/***************************/
	/* File chooser properties */
	/***************************/
	
	/**
	 * Set the file chooser title 
	 * @param id the id of the fileChooser
	 * @param title the title of the fileChooser
	 */
	public static void setFileChooserTitle(int id, String title) {
		((FileChooser) UIElementMapper.getCorrespondingUIElement(id)).setTitle(title);
	}
	
	/**
	 * Set the initial directory used for file search
	 * @param id the id of the fileChooser
	 * @param path the default path
	 */
	public static void setFileChooserInitialDirectory(int id, String path) {
		((FileChooser) UIElementMapper.getCorrespondingUIElement(id)).setInitialDirectory(path);
	}

	/**
	 * Set the mask for files that can be selected
	 * @param id the id of the fileChooser
	 * @param mask the mask to apply
	 */
	public static void setFileChooserMask(int id, String mask) {
		((FileChooser) UIElementMapper.getCorrespondingUIElement(id)).setMask(mask);
	}

	/**
	 * Display this chooser and wait for user selection 
	 * @param id the id of the fileChooser
	 */
	public static void fileChooserDisplayAndWait(int id) {
		((FileChooser) UIElementMapper.getCorrespondingUIElement(id)).displayAndWait();
	}
	
	/**
	 * Get the number of files selected
	 * @param id the id of the fileChooser
	 * @return the number of files selected
	 */
	public static int getFileChooserSelectionSize(int id) {
		return ((FileChooser) UIElementMapper.getCorrespondingUIElement(id)).getSelectionSize();
	}
	
	/**
	 * Get the names of selected files
	 * @param id the id of the fileChooser
	 * @return the names of selected files
	 */
	public static String[] getFileChooserSelection(int id) {
		return ((FileChooser) UIElementMapper.getCorrespondingUIElement(id)).getSelection();
	}
	
	/**
	 * Set the flag indicating that we want only select directories
	 * @param id the id of the fileChooser
	 */
	public static void setFileChooserDirectorySelectionOnly(int id) {
		((FileChooser) UIElementMapper.getCorrespondingUIElement(id)).setDirectorySelectionOnly();
	}

	/**
	 * Set the flag indicating that we want only select files
	 * @param id the id of the fileChooser
	 */
	public static void setFileChooserFileSelectionOnly(int id) {
		((FileChooser) UIElementMapper.getCorrespondingUIElement(id)).setFileSelectionOnly();
	}
	
}
