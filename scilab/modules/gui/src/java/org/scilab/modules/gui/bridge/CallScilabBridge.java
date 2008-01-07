
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge;

import java.awt.Color;
import java.awt.Font;

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
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.widget.Widget;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.renderer.FigureMapper;

/**
 * This class is used to call Scilab GUIs objects from Scilab
 * @author Vincent COUVERT
 */
public class CallScilabBridge {
	
	private static final int NB_COLORS = 3;
	
	private static final int POSITION_SIZE = 4;
	
	private static final int X_INDEX = 0;
	private static final int Y_INDEX = 1;
	private static final int WIDTH_INDEX = 2;
	private static final int HEIGHT_INDEX = 3;

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
		int id = UIElementMapper.add(pushButton);
		// Scilab default font
		setPushButtonFontWeight(id, "normal");
		return id;
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
	 * Set the text of a widget in Scilab GUIs
	 * @param objID the ID of the Widget in the UIElementMapper
	 * @param text the text to set to the widget
	 */
	public static void setWidgetText(int objID, String text) {
		((Widget) UIElementMapper.getCorrespondingUIElement(objID)).setText(text);
	}
	
	/**
	 * Get the text of a widget in Scilab GUIs
	 * @param objID the ID of the Widget in the UIElementMapper
	 * @return the text of the widget
	 */
	public static String getWidgetText(int objID) {
		return ((Widget) UIElementMapper.getCorrespondingUIElement(objID)).getText();
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
		//int menuBarId;

		// Create a menuBar if not already one associated to the parentTab
		if (parentTab.getMenuBar() == null) {
			menuBar = ScilabMenuBar.createMenuBar();
			//menuBarId = UIElementMapper.add(menuBar);

			parentTab.addMenuBar(menuBar);
			// parentTab.setMenuBarId(menuBarId);
		} else {
			menuBar = parentTab.getMenuBar();
			//menuBarId = parentTab.getMenuBarId();
		}
		// Add the menu to the tab
		ScilabMenuBarBridge.add(menuBar, (Menu) UIElementMapper.getCorrespondingUIElement(objID));

		// If parent tab is the currently "on top" tab, then the MenuBar is also added to the parent window
		if (ScilabTabBridge.isCurrentTab(parentTab)) {
			UIElementMapper.getCorrespondingUIElement(ScilabTabBridge.getParentWindowId(parentTab)).addMenuBar(menuBar);
			//UIElementMapper.getCorrespondingUIElement(ScilabTabBridge.getParentWindowId(parentTab)).setMenuBarId(menuBarId);
		}
	}
	
	/**
	 * Set a figure as parent for a PushButton
	 * @param figureID the ID of the figure in the FigureMapper
	 * @param objID the ID of the PushButton in the UIElementMapper
	 */
	public static void setPushButtonParent(int figureID, int objID) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
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
		//int menuBarId;

		// Create a menuBar if not already one associated to the parentTab
		if (consoleTab.getMenuBar() == null) {
			menuBar = ScilabMenuBar.createMenuBar();
			//menuBarId = UIElementMapper.add(menuBar);

			consoleTab.addMenuBar(menuBar);
			//consoleTab.setMenuBarId(menuBarId);
		} else {
			menuBar = consoleTab.getMenuBar();
			//menuBarId = consoleTab.getMenuBarId();
		}
		// Add the menu to the tab
		ScilabMenuBarBridge.add(menuBar, (Menu) UIElementMapper.getCorrespondingUIElement(objID));

		// If parent tab is the currently "on top" tab, then the MenuBar is also added to the parent window
		if (ScilabTabBridge.isCurrentTab(consoleTab)) {
			consoleTab.addMenuBar(menuBar);
			//consoleTab.setMenuBarId(menuBarId);
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
	
	/*************/
	/* Callbacks */
	/*************/

	/**
	 * Set a callback for a Menu
	 * @param menuID the ID of the object in the UIElementMapper
	 * @param callback the text of the callback
	 */
	public static void setMenuCallback(int menuID, String callback) {
		((Menu) UIElementMapper.getCorrespondingUIElement(menuID)).setCallback(callback, 0);
	}
	
	/**
	 * Get the callback for a Menu
	 * @param objID the ID of the object in the UIElementMapper
	 * @return the text of the callback
	 */
	public static String getMenuCallback(int objID) {
		System.out.println("getMenuCallback is not implemented");
		return "";
	}
	
	/**
	 * Set a callback for a PushButton
	 * @param objID the ID of the object in the UIElementMapper
	 * @param callback the text of the callback
	 */
	public static void setPushButtonCallback(int objID, String callback) {
		System.out.println("setPushButtonCallback is not implemented");
	}
	
	/**
	 * Get the callback for a PushButton
	 * @param objID the ID of the object in the UIElementMapper
	 * @return the text of the callback
	 */
	public static String getPushButtonCallback(int objID) {
		System.out.println("getPushButtonCallback is not implemented");
		return "";
	}

	/**
	 * Disable a menu of a Scilab figure giving its name
	 * @param figureID the id of the figure
	 * @param menuName the name of the menu
	 * @param status true to set the menu enabled
	 */
	public static void setFigureMenuEnabled(int figureID, String menuName, boolean status) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		
		MenuBar figureMenuBar = parentTab.getMenuBar();
		
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
		
		MenuBar consoleMenuBar = consoleTab.getMenuBar();

		consoleMenuBar.getAsSimpleMenuBar().setMenuEnabled(menuName, status);
	}
	
	/**
	 * Delete a menu of a Scilab figure giving its name
	 * @param figureID the id of the figure
	 * @param menuName the name of the menu
	 */
	public static void removeFigureMenu(int figureID, String menuName) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		
		MenuBar figureMenuBar = parentTab.getMenuBar();
		
		figureMenuBar.getAsSimpleMenuBar().removeMenu(menuName);
	}

	/**
	 * Delete a menu of a Scilab root window giving its name
	 * @param menuName the name of the menu
	 */
	public static void removeRootMenu(String menuName) {
		int consoleTabId = UIElementMapper.getConsoleId();
		Tab consoleTab = (Tab) UIElementMapper.getCorrespondingUIElement(consoleTabId);
		
		MenuBar consoleMenuBar = consoleTab.getMenuBar();
		
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
	
	/******************/
	/* Colors setting */
	/******************/
	
	/**
	 * Set the background color of a push button 
	 * @param id the id of the push button
	 * @param red the red value for the color
	 * @param green the green value for the color
	 * @param blue the blue value for the color
	 */
	public static void setPushButtonBackgroundColor(int id, int red, int green, int blue) {
		((PushButton) UIElementMapper.getCorrespondingUIElement(id)).setBackground(new Color(red, green, blue));
	}
	
	/**
	 * Get the background color of a pushbutton 
	 * @param id the id of the pushbutton
	 * @return the color [R, G, B]
	 */
	public static int[] getPushButtonBackgroundColor(int id) {
		Color tmpColor = ((PushButton) UIElementMapper.getCorrespondingUIElement(id)).getBackground();
		int[] color = new int[NB_COLORS];
		color[0] = tmpColor.getRed();
		color[1] = tmpColor.getGreen();
		color[2] = tmpColor.getBlue();
		return color;
	}
	
	/**
	 * Set the foreground color of a push button 
	 * @param id the id of the push button
	 * @param red the red value for the color
	 * @param green the green value for the color
	 * @param blue the blue value for the color
	 */
	public static void setPushButtonForegroundColor(int id, int red, int green, int blue) {
		((PushButton) UIElementMapper.getCorrespondingUIElement(id)).setForeground(new Color(red, green, blue));
	}
	
	/**
	 * Get the foreground color of a pushbutton 
	 * @param id the id of the pushbutton
	 * @return the color [R, G, B]
	 */
	public static int[] getPushButtonForegroundColor(int id) {
		Color tmpColor = ((PushButton) UIElementMapper.getCorrespondingUIElement(id)).getForeground();
		int[] color = new int[NB_COLORS];
		color[0] = tmpColor.getRed();
		color[1] = tmpColor.getGreen();
		color[2] = tmpColor.getBlue();
		return color;
	}

	/**
	 * Set the foreground color of a menu
	 * @param id the id of the menu
	 * @param red the red value for the color
	 * @param green the green value for the color
	 * @param blue the blue value for the color
	 */
	public static void setMenuForegroundColor(int id, int red, int green, int blue) {
		((Menu) UIElementMapper.getCorrespondingUIElement(id)).setForeground(new Color(red, green, blue));
	}
	
	/**
	 * Get the foreground color of a menu 
	 * @param id the id of the menu
	 * @return the color [R, G, B]
	 */
	public static int[] getMenuForegroundColor(int id) {
		Color tmpColor = ((Menu) UIElementMapper.getCorrespondingUIElement(id)).getForeground();
		int[] color = new int[NB_COLORS];
		color[0] = tmpColor.getRed();
		color[1] = tmpColor.getGreen();
		color[2] = tmpColor.getBlue();
		return color;
	}

	/****************/
	/* Font setting */
	/****************/

	/**
	 * Set the weight of a pushbutton font
	 * @param id the id of the push button
	 * @param weight the weight of the button font
	 */
	public static void setPushButtonFontWeight(int id, String weight) {
		Font font = ((PushButton) UIElementMapper.getCorrespondingUIElement(id)).getFont();

		if (weight.equals("bold")) {
			if (font.isItalic()) {
				font = new Font(font.getName(), Font.ITALIC + Font.BOLD, font.getSize());
			} else {
				font = new Font(font.getName(), Font.BOLD, font.getSize());
			}
		} else {
			if (font.isItalic()) {
				font = new Font(font.getName(), Font.ITALIC, font.getSize());
			} else {
				font = new Font(font.getName(), Font.PLAIN, font.getSize());
			}
		}

		ScilabBridge.setFont((PushButton) UIElementMapper.getCorrespondingUIElement(id), font);
	}
	
	/**
	 * Set the angle of a pushbutton font
	 * @param id the id of the push button
	 * @param angle the angle of the button font
	 */
	public static void setPushButtonFontAngle(int id, String angle) {
		Font font = ((PushButton) UIElementMapper.getCorrespondingUIElement(id)).getFont();

		if (angle.equals("italic") || angle.equals("oblique")) {
			if (font.isBold()) {
				font = new Font(font.getName(), Font.ITALIC + Font.BOLD, font.getSize());
			} else {
				font = new Font(font.getName(), Font.ITALIC, font.getSize());
			}
		} else {
			if (font.isBold()) {
				font = new Font(font.getName(), Font.BOLD, font.getSize());
			} else {
				font = new Font(font.getName(), Font.PLAIN, font.getSize());
			}
		}

		ScilabBridge.setFont((PushButton) UIElementMapper.getCorrespondingUIElement(id), font);
	}
	
	/**
	 * Set the size of a pushbutton font
	 * @param id the id of the push button
	 * @param size the size of the button font
	 */
	public static void setPushButtonFontSize(int id, int size) {
		Font font = ((PushButton) UIElementMapper.getCorrespondingUIElement(id)).getFont();
		font = new Font(font.getName(), font.getStyle(), size);
		ScilabBridge.setFont((PushButton) UIElementMapper.getCorrespondingUIElement(id), font);
	}

	/**
	 * Get the size of a pushbutton font
	 * @param id the id of the push button
	 * @return the size of the button font
	 */
	public static int getPushButtonFontSize(int id) {
		return ScilabBridge.getFont((PushButton) UIElementMapper.getCorrespondingUIElement(id)).getSize();
	}

	/********************/
	/* Position setting */
	/********************/
	
	/**
	 * Set the position (in Scilab terms) of a widget 
	 * @param id the id of the widget
	 * @param x the X-coordinate for the button
	 * @param y the Y-coordinate for the button
	 * @param width the width of the button
	 * @param height the height of the button
	 */
	public static void setWidgetPosition(int id, int x, int y, int width, int height) {
		UIElementMapper.getCorrespondingUIElement(id).setPosition(new Position(x, y));
		//ScilabBridge.setPosition(UIElementMapper.getCorrespondingUIElement(id), new Position(x, y));
		UIElementMapper.getCorrespondingUIElement(id).setDims(new Size(width, height));
	}
	
	/**
	 * Get the position (in Scilab terms) of a widget 
	 * @param id the id of the widget
	 * @return the position (X-coordinate, Y-coordinate, width, height) of the button
	 */
	public static int[] getWidgetPosition(int id) {
		int[] position = new int[POSITION_SIZE];
		
		position[X_INDEX] = ((PushButton) UIElementMapper.getCorrespondingUIElement(id)).getPosition().getX();
		position[Y_INDEX] = ((PushButton) UIElementMapper.getCorrespondingUIElement(id)).getPosition().getY();
		position[WIDTH_INDEX] = ((PushButton) UIElementMapper.getCorrespondingUIElement(id)).getDims().getWidth();
		position[HEIGHT_INDEX] = ((PushButton) UIElementMapper.getCorrespondingUIElement(id)).getDims().getHeight();
		
		return position;
	}
}
