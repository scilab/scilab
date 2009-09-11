/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad;

import java.awt.Color;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

import javax.swing.BorderFactory;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextPane;
import javax.swing.undo.CannotRedoException;
import javax.swing.undo.CannotUndoException;
import javax.swing.undo.UndoManager;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xpad.actions.ASCIIEncodingAction;
import org.scilab.modules.xpad.actions.AboutAction;
import org.scilab.modules.xpad.actions.AutoIndentAction;
import org.scilab.modules.xpad.actions.CloseAction;
import org.scilab.modules.xpad.actions.ColorizeAction;
import org.scilab.modules.xpad.actions.CommentAction;
import org.scilab.modules.xpad.actions.CopyAction;
import org.scilab.modules.xpad.actions.CutAction;
import org.scilab.modules.xpad.actions.DeleteAction;
import org.scilab.modules.xpad.actions.EvaluateSelectionAction;
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
import org.scilab.modules.xpad.actions.ScilabStyleAction;
import org.scilab.modules.xpad.actions.SelectAllAction;
import org.scilab.modules.xpad.actions.SetColorsAction;
import org.scilab.modules.xpad.actions.SetFontAction;
import org.scilab.modules.xpad.actions.ShowToolBarAction;
import org.scilab.modules.xpad.actions.TextStyleAction;
import org.scilab.modules.xpad.actions.UTF8EncodingAction;
import org.scilab.modules.xpad.actions.UnCommentAction;
import org.scilab.modules.xpad.actions.UndoAction;
import org.scilab.modules.xpad.actions.WordWrapAction;
import org.scilab.modules.xpad.actions.XMLStyleAction;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;

public class Xpad extends SwingScilabTab implements Tab { 

	private final Window parentWindow;
	private JTabbedPane tabPane;
	private JTextPane textPane;
	private JScrollPane scrollingText;
	private XpadLineNumberPanel xln;

	private static Xpad editor = null;

	public static void main(String[] args) {
		xpad();
	}

	public static void xpad() {
		Xpad editor = launchXpad();
		editor.addEmptyTab();
	}

	public static void xpad(String filePath) {
		Xpad editor = launchXpad();
		editor.readFile(new File(filePath));
	}

	private static Xpad launchXpad() {
		if (editor == null) {
			editor = createEditor();
		}
		return editor;
	}

	public static void closeXpad() {
		editor = null;
	}

	private static Xpad createEditor() {
		ConfigXpadManager.createUserCopy();	
		ArrayList<File> recentFiles = ConfigXpadManager.getAllRecentOpenedFiles() ;

		Window mainWindow = ScilabWindow.createWindow();
		Xpad editor = new Xpad(mainWindow);

		mainWindow.setTitle("Xpad");
		mainWindow.addTab(editor);

		// Set Xpad Window position /size 
		mainWindow.setPosition( ConfigXpadManager.getMainWindowPosition());
		mainWindow.setDims(ConfigXpadManager.getMainWindowSize());

		MenuBar menuBar = ScilabMenuBar.createMenuBar();
		//Create FILE menubar
		Menu fileMenu = ScilabMenu.createMenu();
		fileMenu.setText(XpadMessages.FILE);
		fileMenu.setMnemonic('F');
		fileMenu.add(NewAction.createMenu(editor));
		fileMenu.add(OpenAction.createMenu(editor));
		Menu recentsMenu = ScilabMenu.createMenu();
		recentsMenu.setText("Recent Files");
		for (int i = 0 ; i < recentFiles.size() ; i++ ){
			recentsMenu.add(RecentFileAction.createMenu(editor , recentFiles.get(i)));
		}
		
		fileMenu.add(recentsMenu);
		
		fileMenu.addSeparator();
		fileMenu.add(SaveAction.createMenu(editor));
		fileMenu.add(SaveAsAction.createMenu(editor));
		fileMenu.addSeparator();
		fileMenu.add(PageSetupAction.createMenu(editor));
		fileMenu.add(PrintPreviewAction.createMenu(editor));
		fileMenu.add(PrintAction.createMenu(editor));
		fileMenu.addSeparator();
		fileMenu.add(CloseAction.createMenu(editor));
		fileMenu.addSeparator();
		fileMenu.add(ExitAction.createMenu(editor));
		menuBar.add(fileMenu);

		//Create EDIT menubar
		Menu editMenu = ScilabMenu.createMenu();
		editMenu.setText(XpadMessages.EDIT); 
		editMenu.setMnemonic('E');
		editMenu.add(UndoAction.createMenu(editor));
		editMenu.add(RedoAction.createMenu(editor));
		editMenu.addSeparator(); 
		editMenu.add(CutAction.createMenu(editor));
		editMenu.add(CopyAction.createMenu(editor));
		editMenu.add(PasteAction.createMenu(editor));
		editMenu.addSeparator(); 
		editMenu.add(SelectAllAction.createMenu(editor));
		editMenu.add(DeleteAction.createMenu(editor));
		editMenu.addSeparator();
		editMenu.add(CommentAction.createMenu(editor));
		editMenu.add(UnCommentAction.createMenu(editor));
		editMenu.addSeparator();
		editMenu.add(IndentAction.createMenu(editor));
		menuBar.add(editMenu);

		// Create SEARCH menubar
		Menu searchMenu = ScilabMenu.createMenu(); 
		searchMenu.setText(XpadMessages.SEARCH);
		searchMenu.setMnemonic('S');
		searchMenu.add(FindAction.createMenu(editor));
		searchMenu.add(GotoLineAction.createMenu(editor));
		menuBar.add(searchMenu);

		// Create VIEW Menubar
		Menu viewMenu = ScilabMenu.createMenu();
		viewMenu.setText(XpadMessages.VIEW);
		viewMenu.add(ShowToolBarAction.createCheckBoxMenu(editor));
		viewMenu.addSeparator();
		viewMenu.add(HighlightCurrentLineAction.createCheckBoxMenu(editor));
		viewMenu.add(WordWrapAction.createCheckBoxMenu(editor));
		viewMenu.add(LineNumbersAction.createCheckBoxMenu(editor));
		viewMenu.add(SetColorsAction.createMenu(editor));
		viewMenu.add(SetFontAction.createMenu(editor));
		viewMenu.add(ResetFontAction.createMenu(editor));
		menuBar.add(viewMenu);

		// Create DOCUMENT MenuBar
		Menu documentMenu = ScilabMenu.createMenu();
		documentMenu.setText(XpadMessages.DOCUMENT);
		Menu syntaxTypeMenu = ScilabMenu.createMenu();
		syntaxTypeMenu.setText("Syntax Type");
		documentMenu.add(syntaxTypeMenu);
		syntaxTypeMenu.add(TextStyleAction.createCheckBoxMenu(editor));
		syntaxTypeMenu.add(ScilabStyleAction.createCheckBoxMenu(editor));
		syntaxTypeMenu.add(XMLStyleAction.createCheckBoxMenu(editor));
		documentMenu.addSeparator();
		Menu encodingTypeMenu = ScilabMenu.createMenu();
		encodingTypeMenu.setText("Encoding Type");
		documentMenu.add(encodingTypeMenu);
		encodingTypeMenu.add(ASCIIEncodingAction.createCheckBoxMenu(editor));
		encodingTypeMenu.add(UTF8EncodingAction.createCheckBoxMenu(editor));
		documentMenu.addSeparator();
		documentMenu.add(ColorizeAction.createMenu(editor));
		documentMenu.add(AutoIndentAction.createCheckBoxMenu(editor));
		menuBar.add(documentMenu);

		// Create EXECUTE menubar
		Menu executeMenu = ScilabMenu.createMenu();
		executeMenu.setText(XpadMessages.EXECUTE);
		executeMenu.setMnemonic('e');
		executeMenu.add(LoadIntoScilabAction.createMenu(editor));
		executeMenu.add(EvaluateSelectionAction.createMenu(editor));
		menuBar.add(executeMenu);

		//Create HELP menubar
		Menu helpMenu = ScilabMenu.createMenu();
		helpMenu.setText("?");
		helpMenu.add(HelpAction.createMenu(editor));
		helpMenu.add(AboutAction.createMenu(editor));
		menuBar.add(helpMenu);

		// Create TOOLBAR
		ToolBar toolBar = ScilabToolBar.createToolBar();
		toolBar.add(NewAction.createButton(editor)); // NEW
		toolBar.add(OpenAction.createButton(editor)); // OPEN
		toolBar.addSeparator();
		toolBar.add(SaveAction.createButton(editor)); // SAVE
		//toolBar.add(SaveAsAction.createButton(editor)); // SAVE AS
		toolBar.addSeparator();
		//toolBar.add(PrintPreviewAction.createButton(editor)); // PRINT PREVIEW
		toolBar.add(PrintAction.createButton(editor)); // PRINT
		toolBar.addSeparator();
		toolBar.add(UndoAction.createButton(editor)); // UNDO
		toolBar.add(RedoAction.createButton(editor)); // REDO
		toolBar.addSeparator();
		toolBar.add(CutAction.createButton(editor)); // CUT
		toolBar.add(CopyAction.createButton(editor)); // COPY
		toolBar.add(PasteAction.createButton(editor)); // PASTE
		toolBar.addSeparator();
		toolBar.add(FindAction.createButton(editor)); // FIND / REPLACE
		
		
		TextBox infoBar = ScilabTextBox.createTextBox();
		
		
		editor.setMenuBar(menuBar);
		editor.setToolBar(toolBar);
		editor.setInfoBar(infoBar);
		mainWindow.setTitle("XPad");
		mainWindow.setVisible(true);
		editor.setCallback(ExitAction.createMenu(editor).getCallback());
		return editor;
	}

	public Xpad(Window parentWindow) {
		super("Xpad");
		this.parentWindow = parentWindow;
		tabPane = new JTabbedPane();
		this.setContentPane(tabPane);
	}

	public void closeCurrentTab() {
		tabPane.remove(tabPane.getSelectedComponent());
	}

	public JTextPane addTab(String title) {
		textPane = new JTextPane();
// {
//			public boolean getScrollableTracksViewportWidth() {
//				return false;
//			};
//		};

		scrollingText = new JScrollPane(textPane);
		
		// Panel of line number for the text pane
		xln = new XpadLineNumberPanel(textPane);
		scrollingText.setRowHeaderView(xln);
		
		tabPane.add(title, scrollingText);
		tabPane.setSelectedIndex(tabPane.getTabCount() - 1);
		this.setContentPane(tabPane);
		textPane.setBorder(BorderFactory.createEmptyBorder(2,2,2,2));
		textPane.setFont(ConfigXpadManager.getFont());

		textPane.setBackground(ConfigXpadManager.getXpadBackgroundColor());
		textPane.setCaretColor(Color.BLACK);
		textPane.setStyledDocument(new ScilabStyleDocument());
		textPane.setCharacterAttributes(textPane.getStyle("Default"), true);

		textPane.setFocusable(true);

		return textPane;
	}

	public JTextPane addEmptyTab() {
		return addTab("Untitled");
	}

	public void setAutoIndent(boolean b) {
		((ScilabStyleDocument) getTextPane().getStyledDocument()).setAutoIndent(b);
	}

	public void undo() {
		UndoManager undo = ((ScilabStyleDocument) getTextPane().getStyledDocument()).getUndoManager();
		try {
			System.err.println("Will undo "+undo.getUndoPresentationName());
			undo.undo();
		} catch (CannotUndoException ex) {
			System.out.println("Unable to undo: " + ex);
			ex.printStackTrace();
		}
	}

	public void redo() {
		UndoManager redo = ((ScilabStyleDocument) getTextPane().getStyledDocument()).getUndoManager();
		try {
			System.err.println("Will redo "+redo.getRedoPresentationName());
			redo.redo();
		} catch (CannotRedoException ex) {
			System.out.println("Unable to redo: " + ex);
			ex.printStackTrace();
		}
	}

	public void readFile(File f) {
		/* First try to open file before creating tab */
		try {
			StringBuilder contents = new StringBuilder();
			Scanner scanner = new Scanner(f);
			while (scanner.hasNextLine()) {
				contents.append(scanner.nextLine());
				contents.append(System.getProperty("line.separator"));
			}
			JTextPane textPane = addTab(f.getName()); 
			System.out.println("File = "+f.getAbsolutePath());
			textPane.setName(f.getAbsolutePath());

			((ScilabStyleDocument) textPane.getStyledDocument()).disableUpdaters(); 
			textPane.setText(contents.toString());
			if (((ScilabStyleDocument) textPane.getStyledDocument()).getColorize()) {
				((ScilabStyleDocument) textPane.getStyledDocument()).colorize();
			}
			if (((ScilabStyleDocument) textPane.getStyledDocument()).getAutoIndent()) {
				((ScilabStyleDocument) textPane.getStyledDocument()).indent();
			}
			((ScilabStyleDocument) textPane.getStyledDocument()).enableUpdaters();
		} catch (IOException ioex) {
			JOptionPane.showMessageDialog(this, ioex.getMessage());
		}
	}

	public JTextPane getTextPane() {
		return (JTextPane) ((JScrollPane) tabPane.getSelectedComponent()).getViewport().getComponent(0);
	}

	public SimpleTab getAsSimpleTab() {
		return this;
	}

	public Window getParentWindow() {
		return parentWindow;
	}

	public void addInfoBar(TextBox infoBarToAdd) {
		setInfoBar(infoBarToAdd);
	}

	public void addMenuBar(MenuBar menuBarToAdd) {
		setMenuBar(menuBarToAdd);
	}

	public void addToolBar(ToolBar toolBarToAdd) {
		setToolBar(toolBarToAdd);
	}

	public JTabbedPane getTabPane() {
		return tabPane;
	}

	public void setTabPane(JTabbedPane tabPane) {
		this.tabPane = tabPane;
	}

	public XpadLineNumberPanel getXln() {
		return xln;
	}

	public void setXln(XpadLineNumberPanel xln) {
		this.xln = xln;
	}
	
	public void displayLineNumbers(boolean display) {
		if (display) {
			scrollingText.setRowHeaderView(xln);
			repaint();
		} else {
			scrollingText.setRowHeaderView(null);
			repaint();
		}
	}
	
	public void enableLineHighlight(boolean display) {
		xln.setCurrentLineHighlightColor(display);
		repaint();
	}

}
