/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.helpbrowser;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.dockable.ScilabDockable;
import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;

/**
 * Class for Scilab Help Browser in GUIs
 * @author Vincent COUVERT
 */
public class ScilabHelpBrowser extends ScilabDockable implements HelpBrowser {

	private static HelpBrowser instance;
	
	private static Tab helpTab;
	
	private SimpleHelpBrowser component;

	/**
	 * Constructor
	 */
	protected ScilabHelpBrowser() {
		component = ScilabBridge.createHelpBrowser();
	}

	/**
	 * Creates a Scilab Help Browser
	 * @return the created Help Browser
	 */
	public static HelpBrowser createHelpBrowser() {
		if (instance == null) {
			instance = new ScilabHelpBrowser();
			helpTab = ScilabTab.createTab("Help Browser");
			helpTab.addMember(instance);
			helpTab.setCallback(ScilabCallBack
					.createCallback("org.scilab.modules.gui.bridge.CallScilabBridge.closeHelpBrowser", ScilabCallBack.JAVA));
			
			MenuBar menubar = ScilabMenuBar.createMenuBar();
			
			Menu menu = ScilabMenu.createMenu();
			menu.setText("File");
			menubar.add(menu);
			
			menu = ScilabMenu.createMenu();
			menu.setText("?");
			menubar.add(menu);
			
			helpTab.addMenuBar(menubar);

			TextBox infobar = ScilabTextBox.createTextBox();
			helpTab.addInfoBar(infobar);

			Window helpWindow = ScilabWindow.createWindow();
			helpWindow.addTab(helpTab);
			helpWindow.draw();
		}
		return instance;
	}

	/**
	 * Get the Scilab Help Browser
	 * @return the console
	 */
	public static HelpBrowser getHelpBrowser() {
		if (instance == null) {
			instance = new ScilabHelpBrowser();
		}
		return instance;
	}
	
	/**
	 * Display the Help Browser
	 */
	public void display() {
		ScilabBridge.display(this);
	}
	
	/**
	 * Display the matching items for a specified keyword
	 * @param keyword the keyword
	 * @return true if the keyword exists
	 */
	public boolean searchKeywork(String keyword) {
		return ScilabBridge.searchKeywork(this, keyword);
	}
	
	/**
	 * Close the HelpBrowser
	 */
	public void close() {
		ScilabBridge.close(this);
		helpTab.close();
		instance = null;
	}


	/**
	 * Not used
	 */
	public void draw() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleHelpBrowser getAsSimpleHelpBrowser() {
		return component;
	}

	/**
	 * Not used
	 * @return nothing
	 */
	public Size getDims() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Not used
	 * @return nothing
	 */
	public Position getPosition() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Not used
	 * @return nothing
	 */
	public boolean isVisible() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Not used
	 * @param newSize not used
	 */
	public void setDims(Size newSize) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Not used
	 * @param newPosition not used
	 */
	public void setPosition(Position newPosition) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Not used
	 * @param newVisibleState not used
	 */
	public void setVisible(boolean newVisibleState) {
		throw new UnsupportedOperationException();
	}
}
