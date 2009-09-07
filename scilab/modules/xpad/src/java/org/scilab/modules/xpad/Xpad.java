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
import java.awt.Event;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.IOException;
import java.util.Scanner;

import javax.swing.BorderFactory;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextPane;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
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
import org.scilab.modules.gui.textbox.TextBox;
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
import org.scilab.modules.xpad.actions.FindNextAction;
import org.scilab.modules.xpad.actions.FindPreviousAction;
import org.scilab.modules.xpad.actions.GotoLineAction;
import org.scilab.modules.xpad.actions.HelpAction;
import org.scilab.modules.xpad.actions.IndentAction;
import org.scilab.modules.xpad.actions.LineNumbersAction;
import org.scilab.modules.xpad.actions.LoadIntoScilabAction;
import org.scilab.modules.xpad.actions.NewAction;
import org.scilab.modules.xpad.actions.OpenAction;
import org.scilab.modules.xpad.actions.PageSetupAction;
import org.scilab.modules.xpad.actions.PasteAction;
import org.scilab.modules.xpad.actions.PrintAction;
import org.scilab.modules.xpad.actions.PrintPreviewAction;
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

public class Xpad extends SwingScilabTab implements Tab { 

	private final Window parentWindow;
	private JTabbedPane tabPane;
	private JTextPane textPane;
	private JScrollPane scrollingText;

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
		fileMenu.setText("File");
		fileMenu.setMnemonic('F');
		fileMenu.add(new NewAction(editor));
		fileMenu.add(new OpenAction(editor));
		fileMenu.addSeparator();
		fileMenu.add(new SaveAction(editor));
		fileMenu.add(new SaveAsAction(editor));
		fileMenu.addSeparator();
		fileMenu.add(new PageSetupAction(editor));
		fileMenu.add(new PrintPreviewAction(editor));
		fileMenu.add(new PrintAction(editor));
		fileMenu.addSeparator();
		fileMenu.add(new CloseAction(editor));
		fileMenu.addSeparator();
		fileMenu.add(new ExitAction(editor));
		menuBar.add(fileMenu);

		//Create EDIT menubar
		Menu editMenu = ScilabMenu.createMenu();
		editMenu.setText("Edit"); 
		editMenu.setMnemonic('E');
		editMenu.add(new UndoAction(editor));
		editMenu.add(new RedoAction(editor));
		editMenu.addSeparator(); 
		editMenu.add(new CutAction(editor));
		editMenu.add(new CopyAction(editor));
		editMenu.add(new PasteAction(editor));
		editMenu.addSeparator(); 
		editMenu.add(new SelectAllAction(editor));
		editMenu.add(new DeleteAction(editor));
		editMenu.addSeparator();
		editMenu.add(new CommentAction(editor));
		editMenu.add(new UnCommentAction(editor));
		editMenu.addSeparator();
		editMenu.add(new IndentAction(editor));
		menuBar.add(editMenu);

		// Create SEARCH menubar
		Menu searchMenu = ScilabMenu.createMenu(); 
		searchMenu.setText("Search");
		searchMenu.setMnemonic('S');
		searchMenu.add(new FindAction(editor));
		searchMenu.add(new FindNextAction(editor));
		searchMenu.add(new FindPreviousAction(editor));
		searchMenu.add(new GotoLineAction(editor));
		menuBar.add(searchMenu);

		// Create VIEW Menubar
		Menu viewMenu = ScilabMenu.createMenu();
		viewMenu.setText("View");
		viewMenu.add(new ShowToolBarAction(editor));
		viewMenu.addSeparator();
		viewMenu.add(new WordWrapAction(editor));
		viewMenu.add(new LineNumbersAction(editor));
		viewMenu.add(new SetColorsAction(editor));
		viewMenu.add(new SetFontAction(editor));
		viewMenu.add(new ResetFontAction(editor));
		menuBar.add(viewMenu);

		// Create DOCUMENT MenuBar
		Menu documentMenu = ScilabMenu.createMenu();
		documentMenu.setText("Document");
		Menu syntaxTypeMenu = ScilabMenu.createMenu();
		syntaxTypeMenu.setText("Syntax Type");
		documentMenu.add(syntaxTypeMenu);
		syntaxTypeMenu.add(new TextStyleAction(editor));
		syntaxTypeMenu.add(new ScilabStyleAction(editor));
		syntaxTypeMenu.add(new XMLStyleAction(editor));
		documentMenu.addSeparator();
		Menu encodingTypeMenu = ScilabMenu.createMenu();
		encodingTypeMenu.setText("Encoding Type");
		documentMenu.add(encodingTypeMenu);
		encodingTypeMenu.add(new ASCIIEncodingAction(editor));
		encodingTypeMenu.add(new UTF8EncodingAction(editor));
		documentMenu.addSeparator();
		documentMenu.add(new ColorizeAction(editor));
		documentMenu.add(new AutoIndentAction(editor));
		menuBar.add(documentMenu);

		// Create EXECUTE menubar
		Menu executeMenu = ScilabMenu.createMenu();
		executeMenu.setText("Execute");
		executeMenu.setMnemonic('e');
		executeMenu.add(new LoadIntoScilabAction(editor));
		executeMenu.add(new EvaluateSelectionAction(editor));
		menuBar.add(executeMenu);

		//Create HELP menubar
		Menu helpMenu = ScilabMenu.createMenu();
		helpMenu.setText("?");
		helpMenu.add(new HelpAction(editor));
		helpMenu.add(new AboutAction(editor));
		menuBar.add(helpMenu);

		editor.setMenuBar(menuBar);
		mainWindow.setTitle("XPad");
		mainWindow.setVisible(true);
		editor.setCallback((new ExitAction(editor)).getCallback());
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
		textPane = new JTextPane() {
			public boolean getScrollableTracksViewportWidth() {
				return false;
			};
//			public void paint(Graphics g) {
//				super.paint(g);
//				Xpad.this.repaint();
//			};
		};

		scrollingText = new JScrollPane(textPane);
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

		KeyStroke key;
		key = KeyStroke.getKeyStroke(KeyEvent.VK_Z, Event.CTRL_MASK);
		textPane.getInputMap().put(key, new UndoAction(this));
		key = KeyStroke.getKeyStroke(KeyEvent.VK_Y, Event.CTRL_MASK);
		textPane.getInputMap().put(key, new RedoAction(this));
		key = KeyStroke.getKeyStroke(KeyEvent.VK_F, Event.CTRL_MASK);
		textPane.getInputMap().put(key, new FindAction(this));
		
//		tabPane.add(textPane, BorderLayout.WEST);
//		tabPane.add(scrollingText, BorderLayout.CENTER);
		
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
	
	/**********************************************/

//	public void paint(Graphics g) {
//		textPane.paint(g);
//
//		// We need to properly convert the points to match the viewport
//		// Read docs for viewport
//		int start = textPane.viewToModel(scrollingText.getViewport().getViewPosition()); //starting pos in document
//		int end = textPane.viewToModel(new Point(scrollingText.getViewport().getViewPosition().x + textPane.getWidth(),
//												 scrollingText.getViewport().getViewPosition().y + textPane.getHeight()));
//		// end pos in doc
//
//		// translate offsets to lines
//		Document doc = textPane.getDocument();
//		int startline = doc.getDefaultRootElement().getElementIndex(start) + 1;
//		int endline = doc.getDefaultRootElement().getElementIndex(end) + 1;
//
//		int fontHeight = g.getFontMetrics(textPane.getFont()).getHeight();
//		int fontDesc = g.getFontMetrics(textPane.getFont()).getDescent();
//		int starting_y = -1;
//
//		try	{
//			starting_y = textPane.modelToView(start).y - scrollingText.getViewport().getViewPosition().y + fontHeight - fontDesc;
//		} catch (BadLocationException e1) {
//			e1.printStackTrace();
//		}
//
//		for (int line = startline, y = starting_y; line <= endline; y += fontHeight, line++) {
//			g.drawString(Integer.toString(line), 0, y);
//		}
//	}

}
