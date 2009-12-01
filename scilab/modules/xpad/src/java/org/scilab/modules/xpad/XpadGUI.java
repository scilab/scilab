/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.util.ArrayList;

import javax.swing.ButtonGroup;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.JTextPane;
import javax.swing.text.DefaultEditorKit;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.xpad.actions.AboutAction;
import org.scilab.modules.xpad.actions.AutoIndentAction;
import org.scilab.modules.xpad.actions.CloseAction;
import org.scilab.modules.xpad.actions.CloseAllAction;
import org.scilab.modules.xpad.actions.CloseAllButThisAction;
import org.scilab.modules.xpad.actions.ColorizeAction;
import org.scilab.modules.xpad.actions.CommentAction;
import org.scilab.modules.xpad.actions.CopyAction;
import org.scilab.modules.xpad.actions.CutAction;
import org.scilab.modules.xpad.actions.DeleteAction;
import org.scilab.modules.xpad.actions.EncodingAction;
import org.scilab.modules.xpad.actions.EvaluateSelectionAction;
import org.scilab.modules.xpad.actions.ExecuteFileIntoScilabAction;
import org.scilab.modules.xpad.actions.ExitAction;
import org.scilab.modules.xpad.actions.FindAction;
import org.scilab.modules.xpad.actions.GotoLineAction;
import org.scilab.modules.xpad.actions.HelpAction;
import org.scilab.modules.xpad.actions.HighlightCurrentLineAction;
import org.scilab.modules.xpad.actions.IndentAction;
import org.scilab.modules.xpad.actions.LineNumbersAction;
import org.scilab.modules.xpad.actions.LoadIntoScilabAction;
import org.scilab.modules.xpad.actions.NewAction;
import org.scilab.modules.xpad.actions.OpenAction;
import org.scilab.modules.xpad.actions.PageSetupAction;
import org.scilab.modules.xpad.actions.PasteAction;
import org.scilab.modules.xpad.actions.PrintAction;
import org.scilab.modules.xpad.actions.PrintPreviewAction;
import org.scilab.modules.xpad.actions.RecentFileAction;
import org.scilab.modules.xpad.actions.RedoAction;
import org.scilab.modules.xpad.actions.ResetFontAction;
import org.scilab.modules.xpad.actions.SaveAction;
import org.scilab.modules.xpad.actions.SaveAsAction;
import org.scilab.modules.xpad.actions.SelectAllAction;
import org.scilab.modules.xpad.actions.SetColorsAction;
import org.scilab.modules.xpad.actions.SetFontAction;
import org.scilab.modules.xpad.actions.TabifyAction;
import org.scilab.modules.xpad.actions.UnCommentAction;
import org.scilab.modules.xpad.actions.UnTabifyAction;
import org.scilab.modules.xpad.actions.UndoAction;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;

public class XpadGUI {
	private static JRadioButtonMenuItem[] radioTypes;
	private static MenuItem evaluateSelectionMenuItem;
	private static TextBox infoBar;

	public XpadGUI(Window mainWindow, Xpad editorInstance, String title) {
		ArrayList<File> recentFiles = ConfigXpadManager.getAllRecentOpenedFiles();

		mainWindow.setTitle(title);
		mainWindow.addTab(editorInstance);

		// Set Xpad Window position /size
		mainWindow.setPosition(ConfigXpadManager.getMainWindowPosition());
		mainWindow.setDims(ConfigXpadManager.getMainWindowSize());

		MenuBar menuBar = ScilabMenuBar.createMenuBar();
		//Create FILE menubar
		Menu fileMenu = ScilabMenu.createMenu();
		fileMenu.setText(XpadMessages.FILE);
		fileMenu.setMnemonic('F');
		fileMenu.add(NewAction.createMenu(editorInstance));
		fileMenu.add(OpenAction.createMenu(editorInstance));
		Menu recentsMenu = editorInstance.getRecentsMenu();
		recentsMenu.setText(XpadMessages.RECENT_FILES);
		for (int i = 0; i < recentFiles.size(); i++) {
			recentsMenu.add(RecentFileAction.createMenu(editorInstance , recentFiles.get(i)));
		}

		fileMenu.add(recentsMenu);

		fileMenu.addSeparator();
		fileMenu.add(SaveAction.createMenu(editorInstance));
		fileMenu.add(SaveAsAction.createMenu(editorInstance));
		fileMenu.addSeparator();
		fileMenu.add(PageSetupAction.createMenu(editorInstance));
		fileMenu.add(PrintPreviewAction.createMenu(editorInstance));
		fileMenu.add(PrintAction.createMenu(editorInstance));
		fileMenu.addSeparator();
		fileMenu.add(CloseAction.createMenu(editorInstance));
		fileMenu.add(CloseAllAction.createMenu(editorInstance));
		fileMenu.add(CloseAllButThisAction.createMenu(editorInstance));
		fileMenu.addSeparator();
		fileMenu.add(ExitAction.createMenu(editorInstance));
		menuBar.add(fileMenu);

		//Create EDIT menubar
		Menu editMenu = ScilabMenu.createMenu();
		editMenu.setText(XpadMessages.EDIT); 
		editMenu.setMnemonic('E');
		editMenu.add(UndoAction.createMenu(editorInstance));
		editMenu.add(RedoAction.createMenu(editorInstance));
		editMenu.addSeparator(); 
		editMenu.add(CutAction.createMenu(editorInstance));
		editMenu.add(CopyAction.createMenu(editorInstance));
		editMenu.add(PasteAction.createMenu(editorInstance));
		editMenu.addSeparator(); 
		editMenu.add(SelectAllAction.createMenu(editorInstance));
		editMenu.add(DeleteAction.createMenu(editorInstance));
		editMenu.addSeparator();
		editMenu.add(CommentAction.createMenu(editorInstance));
		editMenu.add(UnCommentAction.createMenu(editorInstance));
		editMenu.addSeparator();
		editMenu.add(TabifyAction.createMenu(editorInstance));
		editMenu.add(UnTabifyAction.createMenu(editorInstance));
		editMenu.addSeparator();
		editMenu.add(IndentAction.createMenu(editorInstance));
		menuBar.add(editMenu);

		// Create SEARCH menubar
		Menu searchMenu = ScilabMenu.createMenu(); 
		searchMenu.setText(XpadMessages.SEARCH);
		searchMenu.setMnemonic('S');
		searchMenu.add(FindAction.createMenu(editorInstance));
		searchMenu.add(GotoLineAction.createMenu(editorInstance));
		menuBar.add(searchMenu);

		// Create VIEW Menubar
		Menu viewMenu = ScilabMenu.createMenu();
		viewMenu.setText(XpadMessages.VIEW);
		viewMenu.setMnemonic('S');
		viewMenu.add(HighlightCurrentLineAction.createCheckBoxMenu(editorInstance));
		viewMenu.add(LineNumbersAction.createCheckBoxMenu(editorInstance));
		viewMenu.add(SetColorsAction.createMenu(editorInstance));
		viewMenu.add(SetFontAction.createMenu(editorInstance));
		viewMenu.add(ResetFontAction.createMenu(editorInstance));
		menuBar.add(viewMenu);

		// Create DOCUMENT MenuBar
		Menu documentMenu = ScilabMenu.createMenu();
		documentMenu.setText(XpadMessages.DOCUMENT);
		documentMenu.setMnemonic('D');
		Menu syntaxTypeMenu = ScilabMenu.createMenu();
		Menu encodingTypeMenu = ScilabMenu.createMenu();
		encodingTypeMenu.setText(XpadMessages.ENCODING_TYPE);
		documentMenu.add(encodingTypeMenu);

		ArrayList<String> encodings = EncodingAction.getEcodings();
		
		JRadioButtonMenuItem[] radioTypes = new JRadioButtonMenuItem[encodings.size()];
		ButtonGroup group = new ButtonGroup();
		for (int i = 0; i < encodings.size(); i++) {
			radioTypes[i] = (new EncodingAction(encodings.get(i).toString(), editorInstance)).createRadioButtonMenuItem(editorInstance);
			group.add(radioTypes[i]);
			((JMenu) encodingTypeMenu.getAsSimpleMenu()).add(radioTypes[i]);
			
			// Editor's default encoding is UTF-8
			if (encodings.get(i).toString().equals("UTF-8")) {
				radioTypes[i].setSelected(true);
			}
		}
		
		documentMenu.addSeparator();
		documentMenu.add(ColorizeAction.createCheckBoxMenu(editorInstance));
		documentMenu.add(AutoIndentAction.createCheckBoxMenu(editorInstance));
		menuBar.add(documentMenu);

		// Create EXECUTE menubar
		Menu executeMenu = ScilabMenu.createMenu();
		executeMenu.setText(XpadMessages.EXECUTE);
		executeMenu.setMnemonic('e');
		executeMenu.add(LoadIntoScilabAction.createMenu(editorInstance));
		evaluateSelectionMenuItem = EvaluateSelectionAction.createMenu(editorInstance);
		executeMenu.add(evaluateSelectionMenuItem);
		executeMenu.add(ExecuteFileIntoScilabAction.createMenu(editorInstance));
		menuBar.add(executeMenu);

		//Create HELP menubar
		Menu helpMenu = ScilabMenu.createMenu();
		helpMenu.setText(XpadMessages.QUESTION_MARK);
		helpMenu.add(HelpAction.createMenu(editorInstance));
		helpMenu.add(AboutAction.createMenu(editorInstance));
		menuBar.add(helpMenu);

		// Create TOOLBAR
		ToolBar toolBar = ScilabToolBar.createToolBar();
		toolBar.add(NewAction.createButton(editorInstance)); // NEW
		toolBar.add(OpenAction.createButton(editorInstance)); // OPEN
		toolBar.addSeparator();
		toolBar.add(SaveAction.createButton(editorInstance)); // SAVE
		toolBar.add(SaveAsAction.createButton(editorInstance)); // SAVE AS
		toolBar.addSeparator();
		toolBar.add(PrintAction.createButton(editorInstance)); // PRINT
		toolBar.addSeparator();
		toolBar.add(UndoAction.createButton(editorInstance));
		toolBar.add(RedoAction.createButton(editorInstance));
		toolBar.addSeparator();
		toolBar.add(CutAction.createButton(editorInstance)); // CUT
		toolBar.add(CopyAction.createButton(editorInstance)); // COPY
		toolBar.add(PasteAction.createButton(editorInstance)); // PASTE
		toolBar.addSeparator();
		toolBar.add(FindAction.createButton(editorInstance)); // FIND / REPLACE

		infoBar = ScilabTextBox.createTextBox();

		editorInstance.setMenuBar(menuBar);
		editorInstance.setToolBar(toolBar);
		editorInstance.setInfoBar(infoBar);
		mainWindow.setTitle(title);
		mainWindow.setVisible(true);
	}

	/**
	 * Update the selected item in the encoding pull down menu of the document.
	 * @param scilabDocument the document for which the encoding menu should
	 * be updated
	 */
	public void updateEncodingMenu(ScilabStyleDocument scilabDocument) {
		if (radioTypes != null) {
			for (int k = 0; k < radioTypes.length; k++) {
				if (scilabDocument.getScilabDocument() instanceof ScilabStyleDocument) {
					if ((scilabDocument.getScilabDocument()).getEncoding().equals(radioTypes[k].getText())) {
						radioTypes[k].setSelected(true);
					}
				}
			}
		}
	}
	
	public static MenuItem getEvaluateSelectionMenuItem() {
		return evaluateSelectionMenuItem;
	}

	/**
	 * Create the popup menu on the help
	 * @param c The graphic component
	 */
	public static void createPopupMenu(final JTextPane c) {
		
		final JPopupMenu popup = new JPopupMenu();

		JMenuItem menuItem = null; 

		/* Execute into Scilab */
		ActionListener actionListenerExecuteIntoScilab = new ActionListener() {
			public void actionPerformed(ActionEvent actionEvent) {
				String selection = c.getSelectedText();
				if (selection == null) {
					infoBar.setText(Messages.gettext("No text selected"));
				} else {
					ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(selection, true /* display */, true /* store in history */);
				}
			}
		};
		menuItem = new JMenuItem(Messages.gettext("Execute selection into Scilab"));
		menuItem.addActionListener(actionListenerExecuteIntoScilab);
		if (!ScilabConsole.isExistingConsole()) { /* Only available in STD mode */
			menuItem.setEnabled(false);
		}
		popup.add(menuItem);


		/* Edit in the Scilab Text Editor */
		ActionListener actionListenerLoadIntoTextEditor = new ActionListener() {
			public void actionPerformed(ActionEvent actionEvent) {
				String selection = c.getSelectedText();
				if (selection == null) {
					infoBar.setText(Messages.gettext("No text selected"));
				} else {
					Xpad.xpadWithText(selection);
				}
			}
		};



		menuItem = new JMenuItem(Messages.gettext("Edit selection in a new tab"));
		menuItem.addActionListener(actionListenerLoadIntoTextEditor);
		popup.add(menuItem);
		popup.addSeparator();


		/* Copy */
		menuItem = new JMenuItem(new DefaultEditorKit.CopyAction());
		menuItem.setText(Messages.gettext("Copy"));
		popup.add(menuItem); 
		popup.addSeparator();
		
		/* Cut */
		menuItem = new JMenuItem(new DefaultEditorKit.CutAction());
		menuItem.setText(Messages.gettext("Cut"));
		popup.add(menuItem); 
		popup.addSeparator();

		/* Paste */
		menuItem = new JMenuItem(new DefaultEditorKit.PasteAction());
		menuItem.setText(Messages.gettext("Paste"));
		popup.add(menuItem); 
		popup.addSeparator();

		/* Select all */
		ActionListener actionListenerSelectAll = new ActionListener() {
			public void actionPerformed(ActionEvent actionEvent) {
				c.selectAll();
			}
		};
		menuItem = new JMenuItem(Messages.gettext("Select All"));
		menuItem.addActionListener(actionListenerSelectAll);
		popup.add(menuItem);

		/* Edit in the Scilab Text Editor */
		final JMenuItem helpMenuItem = new JMenuItem("Help on the selected text");

		ActionListener actionListenerHelpOnKeyword = new ActionListener() {
			public void actionPerformed(ActionEvent actionEvent) {
				String selection = c.getSelectedText();
				if (selection == null) {
					infoBar.setText(Messages.gettext("No text selected"));
				} else {
					/* Double the quote/double quote in order to avoid
					 * and error with the call of help()
					 */
					selection = selection.replaceAll("'", "''");
					selection = selection.replaceAll("\"", "\"\"");
					
					/* @TODO: Check if it is possible to call directly
					 * from the Java engine the help
					 * Last time I check, we needed some information
					 * provided by the Scilab native engine
					 */
					InterpreterManagement.requestScilabExec("help('" + selection + "')");
				}
			}
		};
		
		/* Not sure it is the best listener */
		PropertyChangeListener listenerTextItem = new PropertyChangeListener() {			
			public void propertyChange(PropertyChangeEvent arg0) {
				String keyword = c.getSelectedText();
				if (keyword == null) {
					helpMenuItem.setText(Messages.gettext("Help about a selected text"));
				} else {
					int nbOfDisplayedOnlyXChar = 10;
					if (keyword.length() > nbOfDisplayedOnlyXChar) {
						keyword = keyword.substring(0, nbOfDisplayedOnlyXChar) + "...";
					}
					helpMenuItem.setText(Messages.gettext("Help about '") + keyword + "'");
				}
			}
		};
		helpMenuItem.addPropertyChangeListener(listenerTextItem);
		helpMenuItem.addActionListener(actionListenerHelpOnKeyword);
		popup.add(helpMenuItem);
				
		/* Creates the Popupmenu on the component */
		c.setComponentPopupMenu(popup);
	}	
}