/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) INRIA - Vincent COUVERT
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) INRIA - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.jvm;

import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.utils.LookAndFeelManager;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.utils.ToolBarBuilder;

/**
 * Main Class for Scilab
 * @author Allan CORNET
 * @author Vincent COUVERT
 * @author Sylvestre Ledru INRIA 2007
 * @author Bruno JOFRET
 */
public class Scilab {

	private static final String CLASS_NOT_FOUND = "Could not find class: ";

	private static final String SCIDIR = System.getenv("SCI");

	private static final String MENUBARXMLFILE = SCIDIR + "/modules/gui/etc/main_menubar.xml";
	private static final String TOOLBARXMLFILE = SCIDIR + "/modules/gui/etc/main_toolbar.xml";
	
	private static final String SEE_DEFAULT_PATHS = "See SCI/etc/classpath.xml for default paths.";
	
	private int mode;

	private Window mainView;

	 /**
	 * Constructor Scilab Class.
	 * @param mode Mode Scilab -NW -NWNI -STD -API
	 */
	public Scilab(int mode) {
		String gtkLookAndFeel = "com.sun.java.swing.plaf.gtk.GTKLookAndFeel";
		this.mode = mode;
		LookAndFeelManager lookAndFeel = null;

		try {
			lookAndFeel = new LookAndFeelManager();
		} catch(java.lang.NoClassDefFoundError exception) {
			System.err.println("Could not initialize graphics Environment");
			System.err.println("An error occurred: "+ exception.getLocalizedMessage());
			System.exit(-1);

		}

		/* TODO Manages also windows */

		if (lookAndFeel.isSupportedLookAndFeel(gtkLookAndFeel)) {
			lookAndFeel.setLookAndFeel(gtkLookAndFeel);
		} else {
			lookAndFeel.setSystemLookAndFeel();
		}

		if (mode == 2) { /* Mode GUI */

			try {
				mainView = ScilabWindow.createWindow();
			} catch (NoClassDefFoundError exception) {
				System.err.println("Cannot create Scilab Window.\nCheck if the thirdparties are available (Flexdock, JOGL...).\n"+SEE_DEFAULT_PATHS);
				System.err.println(CLASS_NOT_FOUND + exception.getLocalizedMessage());
				System.exit(-1);
			}

			/************/
			/* MENU BAR */
			/************/
			MenuBar menuBar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE);

			/************/
			/* TOOL BAR */
			/************/
			ToolBar toolBar = ToolBarBuilder.buildToolBar(TOOLBARXMLFILE);

			/* Create the console */
			Tab consoleTab = null;
			try {
				/* CONSOLE */
				/* Create a tab to put console into */
				consoleTab = ScilabTab.createTab("Console");
				/* Exit Scilab when the console is closed */
				consoleTab.setCallback(CallBack.createCallback("exit();", CallBack.SCILAB_INSTRUCTION));

				ScilabConsole.createConsole();
			} catch (NoClassDefFoundError exception) {
				System.err.println("Cannot create Scilab Console.\nCheck if the thirdparties are available (JoGL/JRosetta...).\n"
						+ SEE_DEFAULT_PATHS);
				System.err.println(CLASS_NOT_FOUND + exception.getLocalizedMessage());
				System.exit(-1);
			}

			/** Adding content into container */
			ScilabConsole.getConsole().addToolBar(toolBar);
			ScilabConsole.getConsole().addMenuBar(menuBar);
			consoleTab.addMember(ScilabConsole.getConsole());
			mainView.addTab(consoleTab);
			mainView.draw();
		} else {
			System.out.println("mode -NW");
		}

	}

	/**
	 * Sets the prompt displayed in Scilab console
	 * @param prompt the prompt to be displayed as a String
	 */
	public void setPrompt(String prompt) {
		ScilabConsole.getConsole().setPrompt(prompt);
	}
	
}
/*--------------------------------------------------------------------------*/

