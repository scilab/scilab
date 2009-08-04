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
import java.awt.Font;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.IOException;
import java.util.Scanner;

import javax.swing.BorderFactory;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.KeyStroke;
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
import org.scilab.modules.xpad.actions.SaveAction;
import org.scilab.modules.xpad.actions.SaveAsAction;
import org.scilab.modules.xpad.actions.ScilabStyleAction;
import org.scilab.modules.xpad.actions.SelectAllAction;
import org.scilab.modules.xpad.actions.SetColorsAction;
import org.scilab.modules.xpad.actions.ShowToolBarAction;
import org.scilab.modules.xpad.actions.TextStyleAction;
import org.scilab.modules.xpad.actions.UTF8EncodingAction;
import org.scilab.modules.xpad.actions.UnCommentAction;
import org.scilab.modules.xpad.actions.UndoAction;
import org.scilab.modules.xpad.actions.WordWrapAction;
import org.scilab.modules.xpad.actions.XMLStyleAction;
import org.scilab.modules.xpad.style.ScilabStyleDocument;

import com.sun.org.apache.bcel.internal.generic.GETSTATIC;

public class Xpad extends SwingScilabTab implements Tab { 

    private final Window parentWindow;
    private final JTextPane textPane;

    public static void main(String[] args) {
	xpad();
    }

    public static void xpad() {
	launchXpad();
    }
    
    public static void xpad(String filePath) {
	Xpad editor = launchXpad();
	editor.readFile(new File(filePath));
    }
    
    private static Xpad launchXpad() {
	Window mainWindow = ScilabWindow.createWindow();
	Xpad editor = new Xpad(mainWindow);

	mainWindow.setTitle("Xpad");
	mainWindow.addTab(editor);

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
	return editor;
    }

    public Xpad(Window parentWindow) {
	super("Xpad");
	this.textPane = new JTextPane() {
	    public boolean getScrollableTracksViewportWidth() {
		return false;
	  };
	};
	this.parentWindow = parentWindow;

	JScrollPane scrollingText = new JScrollPane(textPane);
	this.setContentPane(scrollingText);
	textPane.setBorder(BorderFactory.createEmptyBorder(2,2,2,2));
	textPane.setFont(new Font("monospaced", Font.PLAIN, 14));
	textPane.setBackground(Color.WHITE);
	textPane.setCaretColor(Color.BLACK);
	textPane.setStyledDocument(new ScilabStyleDocument());
	textPane.setCharacterAttributes(textPane.getStyle("Default"), true);

	
	
	/* Associate ExitAction to Tab closing */
	setCallback((new ExitAction(this)).getCallback());
	textPane.setFocusable(true);
	KeyStroke key = KeyStroke.getKeyStroke(KeyEvent.VK_Z, Event.CTRL_MASK);
	textPane.getInputMap().put(key, new UndoAction(this));
	key = KeyStroke.getKeyStroke(KeyEvent.VK_Y, Event.CTRL_MASK);
	textPane.getInputMap().put(key, new RedoAction(this));
	key = KeyStroke.getKeyStroke(KeyEvent.VK_F, Event.CTRL_MASK);
	textPane.getInputMap().put(key, new FindAction(this));
	
    }

    public void setAutoIndent(boolean b) {
	((ScilabStyleDocument) textPane.getStyledDocument()).setAutoIndent(b);
    }

    public void undo() {
	UndoManager undo = ((ScilabStyleDocument) textPane.getStyledDocument()).getUndoManager();
	try {
	    System.err.println("Will undo "+undo.getUndoPresentationName());
	    undo.undo();
	} catch (CannotUndoException ex) {
	    System.out.println("Unable to undo: " + ex);
	    ex.printStackTrace();
	}
    }

    public void redo() {
	UndoManager redo = ((ScilabStyleDocument) textPane.getStyledDocument()).getUndoManager();
	try {
	    System.err.println("Will redo "+redo.getRedoPresentationName());
	    redo.redo();
	} catch (CannotRedoException ex) {
	    System.out.println("Unable to redo: " + ex);
	    ex.printStackTrace();
	}
    }

    public void readFile(File f) {
	boolean colorize = ((ScilabStyleDocument) textPane.getStyledDocument()).getColorize();
	((ScilabStyleDocument) textPane.getStyledDocument()).setColorize(false);
	try {
	    Scanner scanner = new Scanner(f);
	    StringBuilder contents = new StringBuilder();
	    while (scanner.hasNextLine()) {
		contents.append(scanner.nextLine());
		contents.append(System.getProperty("line.separator"));
	    }
	    getTextPane().setText(contents.toString());
	} catch (IOException ioex) {
	    JOptionPane.showMessageDialog(this, ioex.getMessage());
	}
	finally {
	    ((ScilabStyleDocument) textPane.getStyledDocument()).setColorize(colorize);
	    if (colorize) {
		((ScilabStyleDocument) textPane.getStyledDocument()).colorize();
	    }
	}
    }

    public JTextPane getTextPane() {
	return textPane;
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
}
