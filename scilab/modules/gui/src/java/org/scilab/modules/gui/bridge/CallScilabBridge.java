
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.editbox.ScilabEditBox;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.graphicWindow.ScilabRendererProperties;
import org.scilab.modules.gui.label.Label;
import org.scilab.modules.gui.label.ScilabLabel;
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
	
	private static final String NORMALFONT = "normal";

	/**
	 * Constructor
	 */
	protected CallScilabBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}
	
	/* Console */
	
	
	public static String readLine() {
		return ScilabConsole.getConsole().readLine();
	}
	
	public static void display(String dataToDisplay) {
		ScilabConsole.getConsole().display(dataToDisplay);
	}
	
	public static boolean isWaitingForInput() {
		return ScilabConsole.getConsole().isWaitingForInput();
	}

	public static void scilabLinesUpdate() {
		ScilabConsole.getConsole().scilabLinesUpdate();
	}
	
	public static void clear() {
		ScilabConsole.getConsole().clear();
	}
	
	public static void clear(int nbLines) {
		ScilabConsole.getConsole().clear(nbLines);
	}
	
	public static int getCharWithoutOutput() {
		// FIXME : Something unclear there... 
		return ScilabConsole.getConsole().getAsSimpleConsole().getCharWithoutOutput();
	}
	
	public static void toHome() {
		ScilabConsole.getConsole().toHome();
	}
	
	public static void setPrompt(String prompt) {
		ScilabConsole.getConsole().setPrompt(prompt);
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
		setWidgetFontWeight(id, NORMALFONT);
		return id;
	}

	/**
	 * Create a new EditBox in Scilab GUIs
	 * @return the ID of the Edit in the UIElementMapper
	 */
	public static int newEditBox() {
		EditBox editBox = ScilabEditBox.createEditBox();
		int id = UIElementMapper.add(editBox);
		// Scilab default font
		setWidgetFontWeight(id, NORMALFONT);
		return id;
	}

	/**
	 * Create a new Label in Scilab GUIs
	 * @return the ID of the Label in the UIElementMapper
	 */
	public static int newLabel() {
		Label label = ScilabLabel.createLabel();
		int id = UIElementMapper.add(label);
		// Scilab default font
		setWidgetFontWeight(id, NORMALFONT);
		label.setBackground(Color.GRAY);
		label.setForeground(Color.BLACK);
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
	 * Set a figure as parent for a EditBox
	 * @param figureID the ID of the figure in the FigureMapper
	 * @param objID the ID of the PushButton in the UIElementMapper
	 */
	public static void setEditBoxParent(int figureID, int objID) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		EditBox editBox = (EditBox) UIElementMapper.getCorrespondingUIElement(objID);
		ScilabBridge.addMember(parentTab, editBox);
	}

	/**
	 * Set a figure as parent for a Label
	 * @param figureID the ID of the figure in the FigureMapper
	 * @param objID the ID of the PushButton in the UIElementMapper
	 */
	public static void setLabelParent(int figureID, int objID) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		Label label = (Label) UIElementMapper.getCorrespondingUIElement(objID);
		ScilabBridge.addMember(parentTab, label);
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
	 * Set a callback for a Widget
	 * @param objID the ID of the object in the UIElementMapper
	 * @param callback the text of the callback
	 */
	public static void setWidgetCallback(int objID, String callback) {
		System.out.println("setWidgetCallback is not implemented");
	}
	
	/**
	 * Get the callback for a Widget
	 * @param objID the ID of the object in the UIElementMapper
	 * @return the text of the callback
	 */
	public static String getWidgetCallback(int objID) {
		System.out.println("getWidgetCallback is not implemented");
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
		ScilabConsole.getConsole().getMenuBar().getAsSimpleMenuBar().setMenuEnabled(menuName, status);
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
		ScilabConsole.getConsole().getMenuBar().getAsSimpleMenuBar().removeMenu(menuName);
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
	 * Set the background color of a Widget
	 * @param id the id of the Widget
	 * @param red the red value for the color
	 * @param green the green value for the color
	 * @param blue the blue value for the color
	 */
	public static void setWidgetBackgroundColor(int id, int red, int green, int blue) {
		((Widget) UIElementMapper.getCorrespondingUIElement(id)).setBackground(new Color(red, green, blue));
	}
	
	/**
	 * Get the background color of a widget 
	 * @param id the id of the widget
	 * @return the color [R, G, B]
	 */
	public static int[] getWidgetBackgroundColor(int id) {
		Color tmpColor = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getBackground();
		int[] color = new int[NB_COLORS];
		color[0] = tmpColor.getRed();
		color[1] = tmpColor.getGreen();
		color[2] = tmpColor.getBlue();
		return color;
	}
	
	/**
	 * Set the foreground color of a Widget
	 * @param id the id of the Widget
	 * @param red the red value for the color
	 * @param green the green value for the color
	 * @param blue the blue value for the color
	 */
	public static void setWidgetForegroundColor(int id, int red, int green, int blue) {
		((Widget) UIElementMapper.getCorrespondingUIElement(id)).setForeground(new Color(red, green, blue));
	}
	
	/**
	 * Get the foreground color of a Widget 
	 * @param id the id of the Widget
	 * @return the color [R, G, B]
	 */
	public static int[] getWidgetForegroundColor(int id) {
		Color tmpColor = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getForeground();
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
	 * Set the weight of a Widget font
	 * @param id the id of the Widget
	 * @param weight the weight of the Widget font
	 */
	public static void setWidgetFontWeight(int id, String weight) {
		Font font = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getFont();

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

		((Widget) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
	}
	
	/**
	 * Set the angle of a Widget font
	 * @param id the id of the Widget
	 * @param angle the angle of the Widget font
	 */
	public static void setWidgetFontAngle(int id, String angle) {
		Font font = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getFont();

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

		((Widget) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
	}
	
	/**
	 * Set the size of a Widget font
	 * @param id the id of the Widget button
	 * @param size the size of the Widget font
	 */
	public static void setWidgetFontSize(int id, int size) {
		Font font = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getFont();
		font = new Font(font.getName(), font.getStyle(), size);
		((Widget) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
	}

	/**
	 * Get the size of a Widget font
	 * @param id the id of the Widget
	 * @return the size of the Widget font
	 */
	public static int getWidgetFontSize(int id) {
		return ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getFont().getSize();
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
		UIElementMapper.getCorrespondingUIElement(id).setDims(new Size(width, height));
	}
	
	/**
	 * Get the position (in Scilab terms) of a widget 
	 * @param id the id of the widget
	 * @return the position (X-coordinate, Y-coordinate, width, height) of the button
	 */
	public static int[] getWidgetPosition(int id) {
		int[] position = new int[POSITION_SIZE];
		
		position[X_INDEX] = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getPosition().getX();
		position[Y_INDEX] = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getPosition().getY();
		position[WIDTH_INDEX] = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getDims().getWidth();
		position[HEIGHT_INDEX] = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getDims().getHeight();
		
		return position;
	}
}
