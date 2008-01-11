
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.checkbox.ScilabCheckBox;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.editbox.ScilabEditBox;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.frame.ScilabFrame;
import org.scilab.modules.gui.graphicWindow.ScilabRendererProperties;
import org.scilab.modules.gui.label.Label;
import org.scilab.modules.gui.label.ScilabLabel;
import org.scilab.modules.gui.listbox.ListBox;
import org.scilab.modules.gui.listbox.ScilabListBox;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBarBridge;
import org.scilab.modules.gui.popupmenu.PopupMenu;
import org.scilab.modules.gui.popupmenu.ScilabPopupMenu;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.radiobutton.ScilabRadioButton;
import org.scilab.modules.gui.slider.ScilabSlider;
import org.scilab.modules.gui.slider.Slider;
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
	private static final String OBLIQUEFONT = "oblique";
	private static final String ITALICFONT = "italic";
	private static final String BOLDFONT = "bold";
	private static final int DEFAULTFONTSIZE = 12;

	/**
	 * Constructor
	 */
	protected CallScilabBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}
	
	/******************/
	/*                */  
	/* CONSOLE BRIDGE */
	/*                */  
	/******************/
	
	/**
	 * Read a line from the Console
	 * @return the line read
	 */
	public static String readLine() {
		return ScilabConsole.getConsole().readLine();
	}
	
	/**
	 * Display a line in the Console
	 * @param dataToDisplay the line to display
	 */
	public static void display(String dataToDisplay) {
		ScilabConsole.getConsole().display(dataToDisplay);
	}
	
	/**
	 * Get the status of the Console
	 * @return true if the console is at the prompt
	 */
	public static boolean isWaitingForInput() {
		return ScilabConsole.getConsole().isWaitingForInput();
	}

	/**
	 * Update the number of lines and columns that Scilab use to format data to display
	 */
	public static void scilabLinesUpdate() {
		ScilabConsole.getConsole().scilabLinesUpdate();
	}
	
	/**
	 * Clear the Console
	 */
	public static void clear() {
		ScilabConsole.getConsole().clear();
	}
	
	/**
	 * Clear some lines in the Console
	 * @param nbLines the number of lines to clear
	 */
	public static void clear(int nbLines) {
		ScilabConsole.getConsole().clear(nbLines);
	}
	
	/**
	 * Get a char to know if the user wants more data to be displayed
	 * @return the user answer
	 */
	public static int getCharWithoutOutput() {
		return ScilabConsole.getConsole().getAsSimpleConsole().getCharWithoutOutput();
	}
	
	/**
	 * Put the prompt on the top left corner
	 */
	public static void toHome() {
		ScilabConsole.getConsole().toHome();
	}
	
	/**
	 * Set the contents of the prompt
	 * @param prompt the content to set (default is -->)
	 */
	public static void setPrompt(String prompt) {
		ScilabConsole.getConsole().setPrompt(prompt);
	}
	
	/**************************/
	/*                        */
	/* OBJECT CREATION BRIDGE */
	/*                        */
	/**************************/
	
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
		setWidgetFontSize(id, DEFAULTFONTSIZE);
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
		setWidgetFontSize(id, DEFAULTFONTSIZE);
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
		setWidgetFontSize(id, DEFAULTFONTSIZE);
		label.setBackground(Color.LIGHT_GRAY);
		label.setForeground(Color.BLACK);
		return id;
	}

	/**
	 * Create a new CheckBox in Scilab GUIs
	 * @return the ID of the CheckBox in the UIElementMapper
	 */
	public static int newCheckBox() {
		CheckBox checkBox = ScilabCheckBox.createCheckBox();
		int id = UIElementMapper.add(checkBox);
		// Scilab default font
		setWidgetFontWeight(id, NORMALFONT);
		setWidgetFontSize(id, DEFAULTFONTSIZE);
		checkBox.setBackground(Color.LIGHT_GRAY);
		checkBox.setForeground(Color.BLACK);
		return id;
	}

	/**
	 * Create a new RadioButton in Scilab GUIs
	 * @return the ID of the RadioButton in the UIElementMapper
	 */
	public static int newRadioButton() {
		RadioButton radioButton = ScilabRadioButton.createRadioButton();
		int id = UIElementMapper.add(radioButton);
		// Scilab default font
		setWidgetFontWeight(id, NORMALFONT);
		setWidgetFontSize(id, DEFAULTFONTSIZE);
		radioButton.setBackground(Color.LIGHT_GRAY);
		radioButton.setForeground(Color.BLACK);
		return id;
	}

	/**
	 * Create a new Slider in Scilab GUIs
	 * @return the ID of the Slider in the UIElementMapper
	 */
	public static int newSlider() {
		Slider slider = ScilabSlider.createSlider();
		int id = UIElementMapper.add(slider);
		// Scilab default font
		setWidgetFontWeight(id, NORMALFONT);
		setWidgetFontSize(id, DEFAULTFONTSIZE);
		slider.setBackground(Color.LIGHT_GRAY);
		slider.setForeground(Color.BLACK);
		return id;
	}

	/**
	 * Create a new ListBox in Scilab GUIs
	 * @return the ID of the ListBox in the UIElementMapper
	 */
	public static int newListBox() {
		ListBox listBox = ScilabListBox.createListBox();
		int id = UIElementMapper.add(listBox);
		// Scilab default font
		setWidgetFontWeight(id, NORMALFONT);
		setWidgetFontSize(id, DEFAULTFONTSIZE);
		listBox.setBackground(Color.LIGHT_GRAY);
		listBox.setForeground(Color.BLACK);
		return id;
	}

	/**
	 * Create a new PopupMenu in Scilab GUIs
	 * @return the ID of the PopupMenu in the UIElementMapper
	 */
	public static int newPopupMenu() {
		PopupMenu popupMenu = ScilabPopupMenu.createPopupMenu();
		int id = UIElementMapper.add(popupMenu);
		// Scilab default font
		setWidgetFontWeight(id, NORMALFONT);
		setWidgetFontSize(id, DEFAULTFONTSIZE);
		popupMenu.setBackground(Color.LIGHT_GRAY);
		popupMenu.setForeground(Color.BLACK);
		return id;
	}

	/**
	 * Create a new Frame in Scilab GUIs
	 * @return the ID of the PopupMenu in the UIElementMapper
	 */
	public static int newFrame() {
		Frame frame = ScilabFrame.createFrame();
		int id = UIElementMapper.add(frame);
		// Scilab default font
		setFrameFontWeight(id, NORMALFONT);
		setFrameFontSize(id, DEFAULTFONTSIZE);
		frame.setBackground(Color.LIGHT_GRAY);
		frame.setForeground(Color.BLACK);
		return id;
	}

	/****************************/
	/*                          */
	/* OBJECT DIMENSIONS BRIDGE */
	/*                          */
	/****************************/
	
	/**
	 * Set the dimensions of an object in Scilab GUIs
	 * @param objID the ID of the object in the UIElementMapper
	 * @param width the width of the object
	 * @param height the height of the object
	 */
	public static void setDims(int objID, int width, int height) {
		UIElementMapper.getCorrespondingUIElement(objID).setDims(new Size(width, height));
	}
	
	/**********************/
	/*                    */
	/* OBJECT TEXT BRIDGE */
	/*                    */
	/**********************/
	
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

	/**
	 * Set the text of a Frame in Scilab GUIs
	 * @param objID the ID of the Widget in the UIElementMapper
	 * @param text the text to set to the Frame
	 */
	public static void setFrameText(int objID, String text) {
		((Frame) UIElementMapper.getCorrespondingUIElement(objID)).setText(text);
	}
	
	/**
	 * Get the text of a Frame in Scilab GUIs
	 * @param objID the ID of the Frame in the UIElementMapper
	 * @return the text of the Frame
	 */
	public static String getFrameText(int objID) {
		return ((Frame) UIElementMapper.getCorrespondingUIElement(objID)).getText();
	}

	/******************/
	/*                */
	/* PARENT SETTING */
	/*                */
	/******************/

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
	 * Set a figure as parent for a CheckBox
	 * @param figureID the ID of the figure in the FigureMapper
	 * @param objID the ID of the PushButton in the UIElementMapper
	 */
	public static void setCheckBoxParent(int figureID, int objID) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		CheckBox checkBox = (CheckBox) UIElementMapper.getCorrespondingUIElement(objID);
		ScilabBridge.addMember(parentTab, checkBox);
	}

	/**
	 * Set a figure as parent for a RadioButton
	 * @param figureID the ID of the figure in the FigureMapper
	 * @param objID the ID of the PushButton in the UIElementMapper
	 */
	public static void setRadioButtonParent(int figureID, int objID) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		RadioButton radioButton = (RadioButton) UIElementMapper.getCorrespondingUIElement(objID);
		ScilabBridge.addMember(parentTab, radioButton);
	}

	/**
	 * Set a figure as parent for a Slider
	 * @param figureID the ID of the figure in the FigureMapper
	 * @param objID the ID of the PushButton in the UIElementMapper
	 */
	public static void setSliderParent(int figureID, int objID) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		Slider slider = (Slider) UIElementMapper.getCorrespondingUIElement(objID);
		ScilabBridge.addMember(parentTab, slider);
	}

	/**
	 * Set a figure as parent for a ListBox
	 * @param figureID the ID of the figure in the FigureMapper
	 * @param objID the ID of the ListBox in the UIElementMapper
	 */
	public static void setListBoxParent(int figureID, int objID) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		ListBox listBox = (ListBox) UIElementMapper.getCorrespondingUIElement(objID);
		ScilabBridge.addMember(parentTab, listBox);
	}

	/**
	 * Set a figure as parent for a PopupMenu
	 * @param figureID the ID of the figure in the FigureMapper
	 * @param objID the ID of the PopupMenu in the UIElementMapper
	 */
	public static void setPopupMenuParent(int figureID, int objID) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		PopupMenu popupMenu = (PopupMenu) UIElementMapper.getCorrespondingUIElement(objID);
		ScilabBridge.addMember(parentTab, popupMenu);
	}

	/**
	 * Set a figure as parent for a Frame
	 * @param figureID the ID of the figure in the FigureMapper
	 * @param objID the ID of the PopupMenu in the UIElementMapper
	 */
	public static void setFrameParent(int figureID, int objID) {
		Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
		Frame frame = (Frame) UIElementMapper.getCorrespondingUIElement(objID);
		ScilabBridge.addMember(parentTab, frame);
	}

	/**
	 * Set root Scilab object (the console tab) as the parent of the menu
	 * @param objID the id of the menu
	 */
	public static void setRootAsParent(int objID) {
		ScilabConsole.getConsole().getMenuBar().add((Menu) UIElementMapper.getCorrespondingUIElement(objID));
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
	
	/*******************/
	/*                 */
	/* OBJECT CALLBACK */
	/*                 */
	/*******************/
	
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
	 * Set a callback for a Frame
	 * @param objID the ID of the object in the UIElementMapper
	 * @param callback the text of the callback
	 */
	public static void setFrameCallback(int objID, String callback) {
		System.out.println("setFrameCallback is not implemented");
	}
	
	/**
	 * Get the callback for a Frame
	 * @param objID the ID of the object in the UIElementMapper
	 * @return the text of the callback
	 */
	public static String getFrameCallback(int objID) {
		System.out.println("getFrameCallback is not implemented");
		return "";
	}

	/************************/
	/*                      */
	/* ENABLE/DISABLE MENUS */
	/*                      */
	/************************/

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
	
	/****************/
	/*              */
	/* REMOVE MENUS */
	/*              */
	/****************/

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
	
	/***********************/
	/*                     */
	/* FILE CHOOSER BRIDGE */
	/*                     */
	/***********************/
	
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
	
	/************************/
	/*                      */
	/* OBJECT COLORS BRIDGE */
	/*                      */
	/************************/
	
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
	 * Set the background color of a Frame
	 * @param id the id of the Frame
	 * @param red the red value for the color
	 * @param green the green value for the color
	 * @param blue the blue value for the color
	 */
	public static void setFrameBackgroundColor(int id, int red, int green, int blue) {
		((Frame) UIElementMapper.getCorrespondingUIElement(id)).setBackground(new Color(red, green, blue));
	}
	
	/**
	 * Get the background color of a Frame 
	 * @param id the id of the Frame
	 * @return the color [R, G, B]
	 */
	public static int[] getFrameBackgroundColor(int id) {
		Color tmpColor = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getBackground();
		int[] color = new int[NB_COLORS];
		color[0] = tmpColor.getRed();
		color[1] = tmpColor.getGreen();
		color[2] = tmpColor.getBlue();
		return color;
	}
	
	/**
	 * Set the foreground color of a Frame
	 * @param id the id of the Frame
	 * @param red the red value for the color
	 * @param green the green value for the color
	 * @param blue the blue value for the color
	 */
	public static void setFrameForegroundColor(int id, int red, int green, int blue) {
		((Frame) UIElementMapper.getCorrespondingUIElement(id)).setForeground(new Color(red, green, blue));
	}
	
	/**
	 * Get the foreground color of a Frame 
	 * @param id the id of the Frame
	 * @return the color [R, G, B]
	 */
	public static int[] getFrameForegroundColor(int id) {
		Color tmpColor = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getForeground();
		int[] color = new int[NB_COLORS];
		color[0] = tmpColor.getRed();
		color[1] = tmpColor.getGreen();
		color[2] = tmpColor.getBlue();
		return color;
	}

	/**********************/
	/*                    */
	/* OBJECT FONT BRIDGE */
	/*                    */
	/**********************/

	/**
	 * Set the weight of a Widget font
	 * @param id the id of the Widget
	 * @param weight the weight of the Widget font
	 */
	public static void setWidgetFontWeight(int id, String weight) {
		Font font = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getFont();

		if (weight.equals(BOLDFONT)) {
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

		if (angle.equals(ITALICFONT) || angle.equals(OBLIQUEFONT)) {
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
	 * @param id the id of the Widget
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

	/**
	 * Set the weight of a Frame font
	 * @param id the id of the Frame
	 * @param weight the weight of the Frame font
	 */
	public static void setFrameFontWeight(int id, String weight) {
		Font font = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getFont();

		if (weight.equals(BOLDFONT)) {
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

		((Frame) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
	}
	
	/**
	 * Set the angle of a Frame font
	 * @param id the id of the Frame
	 * @param angle the angle of the Frame font
	 */
	public static void setFrameFontAngle(int id, String angle) {
		Font font = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getFont();

		if (angle.equals(ITALICFONT) || angle.equals(OBLIQUEFONT)) {
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

		((Frame) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
	}
	
	/**
	 * Set the size of a Frame font
	 * @param id the id of the Frame
	 * @param size the size of the Frame font
	 */
	public static void setFrameFontSize(int id, int size) {
		Font font = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getFont();
		font = new Font(font.getName(), font.getStyle(), size);
		((Frame) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
	}

	/**
	 * Get the size of a Frame font
	 * @param id the id of the Frame
	 * @return the size of the Frame font
	 */
	public static int getFrameFontSize(int id) {
		return ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getFont().getSize();
	}

	/**************************/
	/*                        */
	/* OBJECT POSITION BRIDGE */
	/*                        */
	/**************************/
	
	/**
	 * Set the position (in Scilab terms) of a Widget 
	 * @param id the id of the Widget
	 * @param x the X-coordinate for the Widget
	 * @param y the Y-coordinate for the Widget
	 * @param width the width of the Widget
	 * @param height the height of the Widget
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
	
	/**
	 * Set the position (in Scilab terms) of a Frame 
	 * @param id the id of the Frame
	 * @param x the X-coordinate for the Frame
	 * @param y the Y-coordinate for the Frame
	 * @param width the width of the Frame
	 * @param height the height of the Frame
	 */
	public static void setFramePosition(int id, int x, int y, int width, int height) {
		UIElementMapper.getCorrespondingUIElement(id).setPosition(new Position(x, y));
		UIElementMapper.getCorrespondingUIElement(id).setDims(new Size(width, height));
	}
	
	/**
	 * Get the position (in Scilab terms) of a Frame 
	 * @param id the id of the Frame
	 * @return the position (X-coordinate, Y-coordinate, width, height) of the button
	 */
	public static int[] getFramePosition(int id) {
		int[] position = new int[POSITION_SIZE];
		
		position[X_INDEX] = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getPosition().getX();
		position[Y_INDEX] = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getPosition().getY();
		position[WIDTH_INDEX] = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getDims().getWidth();
		position[HEIGHT_INDEX] = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getDims().getHeight();
		
		return position;
	}
	
	/***************************/
	/*                         */
	/* OBJECT ALIGNMENT BRIDGE */
	/*                         */
	/***************************/

	/**
	 * Set the horizontal alignment for the Widget text
	 * @param id the id of the Widget
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setWidgetHorizontalAlignment(int id, String alignment) {
		((Widget) UIElementMapper.getCorrespondingUIElement(id)).setHorizontalAlignment(alignment);
	}

	/**
	 * Set the vertical alignment for the Widget text
	 * @param id the id of the Widget
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public static void setWidgetVerticalAlignment(int id, String alignment) {
		((Widget) UIElementMapper.getCorrespondingUIElement(id)).setVerticalAlignment(alignment);
	}
}
