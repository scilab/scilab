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
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextPane;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.plaf.basic.BasicButtonUI;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultEditorKit;
import javax.swing.text.EditorKit;
import javax.swing.undo.CannotRedoException;
import javax.swing.undo.CannotUndoException;
import javax.swing.undo.UndoManager;

import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.filechooser.Juigetfile;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
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
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.SciFileFilter;
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
import org.scilab.modules.xpad.actions.ExecuteIntoScilabAction;
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
import org.scilab.modules.xpad.actions.TabifyAction;
import org.scilab.modules.xpad.actions.TextStyleAction;
import org.scilab.modules.xpad.actions.UTF8EncodingAction;
import org.scilab.modules.xpad.actions.UnCommentAction;
import org.scilab.modules.xpad.actions.UnTabifyAction;
import org.scilab.modules.xpad.actions.UndoAction;
import org.scilab.modules.xpad.actions.WordWrapAction;
import org.scilab.modules.xpad.actions.XMLStyleAction;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * Main Xpad class
 * @author Bruno JOFRET
 */
public class Xpad extends SwingScilabTab implements Tab { 

	private static final long serialVersionUID = -6410183357490518676L;

	private static final String XPAD = "Xpad";
	private static final String SCI_EXTENSION = ".sci";
	private static final String SCE_EXTENSION = ".sce";
	private static final String ALL_SCI_FILES = "*.sci";
	private static final String ALL_SCE_FILES = "*.sce";
	private static final int BUTTON_SIZE = 17;
	
	private static Xpad editor;

	private final Window parentWindow;
	private JTabbedPane tabPane;
	private JTextPane textPane;
	private JScrollPane scrollingText;
	private XpadLineNumberPanel xln;
	private Menu recentsMenu;
	private int numberOfUntitled;
	private EditorKit editorKit;
	private long lastKnownSavedState;
	private Object synchro = new Object();

	private Vector<Integer> tabList = new Vector<Integer>();
	private Vector<Integer> closedTabList = new Vector<Integer>();
	
	private String fileFullPath = "";
	
	private static org.scilab.modules.gui.menuitem.MenuItem evaluateSelectionMenuItem;
	

	/**
	 * Create Xpad instance inside parent Window
	 * @param parentWindow the parent Window
	 */
	public Xpad(Window parentWindow) {
		super(XPAD);
		this.parentWindow = parentWindow;
		recentsMenu = ScilabMenu.createMenu();
		numberOfUntitled = 0;
		editorKit = new DefaultEditorKit();
		lastKnownSavedState = 0;
		tabPane = new JTabbedPane();
		tabPane.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				String path = new String("");
				if (getTextPane() != null) {
					if (getTextPane().getName() != null) {
						path  =  " ( " + getTextPane().getName() + ")";
					}
					setTitle(tabPane.getTitleAt(tabPane.getSelectedIndex()) + path + " - " + XpadMessages.SCILAB_EDITOR);
					
					// This listener is for 'evaluate selection' of the Execute menu
					// it enable the menuItem only if something is selected
					textPane.addCaretListener(new CaretListener() {
						public void caretUpdate(CaretEvent e) {
						    int dot = e.getDot();
						    int mark = e.getMark();
						    if (dot == mark) {  // no selection
						    	evaluateSelectionMenuItem.setEnabled(false);
						    } else if (dot < mark) {
						    	evaluateSelectionMenuItem.setEnabled(true);
						    } else {
						    	evaluateSelectionMenuItem.setEnabled(true);
						    }
						}
					});
					updateUI();
				}
			}
		});

		this.setContentPane(tabPane);
	}

	/**
	 * Launch Xpad from command line
	 * @param args command line args
	 */
	public static void main(String[] args) {
		xpad();
	}

	/**
	 * Launch Xpad with an empty file
	 */
	public static void xpad() {
		Xpad editorInstance = launchXpad();
		editorInstance.addEmptyTab();
	}

	/**
	 * Launch Xpad with a file name to open
	 * @param filePath the name of the file to open
	 */
	public static void xpad(String filePath) {
		Xpad editorInstance = launchXpad();
		File f = new File(filePath);
		ConfigXpadManager.saveToRecentOpenedFiles(filePath);
		editorInstance.updateRecentOpenedFilesMenu();
		editorInstance.readFile(f);
		editorInstance.lastKnownSavedState = System.currentTimeMillis();
	}

	/**
	 * Launch Xpad with a file name to open and a line to highlight
	 * @param filePath the name of the file to open
	 * @param lineNumber the line to highlight
	 */
	public static void xpadHighlightLine(String filePath, int lineNumber) {
		Xpad editorInstance = launchXpad();
		File f = new File(filePath);
		ConfigXpadManager.saveToRecentOpenedFiles(filePath);
		editorInstance.updateRecentOpenedFilesMenu();
		editorInstance.readFileAndWait(f);
		editorInstance.getXln().highlightLine(lineNumber);
		editorInstance.lastKnownSavedState= System.currentTimeMillis();
	}

	/**
	 * Create Xpad instance
	 * @return the instance
	 */
	private static Xpad launchXpad() {
		if (editor == null) {
			editor = createEditor();
		}
		return editor;
	}

	/**
	 * Close Xpad instance including all tabs
	 */
	public static void closeXpad() {
		
		FindAction.closeFindReplaceWindow();
		GotoLineAction.closeGotoLineWindow();
		SetColorsAction.closeSetColorsWindow();
		for (int i = 0; i < editor.getTabPane().getComponentCount(); i++) {
			editor.closeTabAt(i);
		}
		editor = null;
	}

	/**
	 * Create Xpad instance
	 * @return the instance
	 */
	private static Xpad createEditor() {
		ConfigXpadManager.createUserCopy();	
		ArrayList<File> recentFiles = ConfigXpadManager.getAllRecentOpenedFiles();

		Window mainWindow = ScilabWindow.createWindow();
		Xpad editorInstance = new Xpad(mainWindow);

		mainWindow.setTitle(XPAD);
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
		//		 recentsMenu = ScilabMenu.createMenu();
		editorInstance.recentsMenu.setText(XpadMessages.RECENT_FILES);
		for (int i = 0; i < recentFiles.size(); i++) {
			editorInstance.recentsMenu.add(RecentFileAction.createMenu(editorInstance , recentFiles.get(i)));
		}

		fileMenu.add(editorInstance.recentsMenu);

		fileMenu.addSeparator();
		fileMenu.add(SaveAction.createMenu(editorInstance));
		fileMenu.add(SaveAsAction.createMenu(editorInstance));
		fileMenu.addSeparator();
		fileMenu.add(PageSetupAction.createMenu(editorInstance));
		fileMenu.add(PrintPreviewAction.createMenu(editorInstance));
		fileMenu.add(PrintAction.createMenu(editorInstance));
		fileMenu.addSeparator();
		fileMenu.add(CloseAction.createMenu(editorInstance));
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
		viewMenu.add(ShowToolBarAction.createCheckBoxMenu(editorInstance));
		viewMenu.addSeparator();
		viewMenu.add(HighlightCurrentLineAction.createCheckBoxMenu(editorInstance));
		viewMenu.add(WordWrapAction.createCheckBoxMenu(editorInstance));
		viewMenu.add(LineNumbersAction.createCheckBoxMenu(editorInstance));
		viewMenu.add(SetColorsAction.createMenu(editorInstance));
		viewMenu.add(SetFontAction.createMenu(editorInstance));
		viewMenu.add(ResetFontAction.createMenu(editorInstance));
		menuBar.add(viewMenu);

		// Create DOCUMENT MenuBar
		Menu documentMenu = ScilabMenu.createMenu();
		documentMenu.setText(XpadMessages.DOCUMENT);
		Menu syntaxTypeMenu = ScilabMenu.createMenu();
		syntaxTypeMenu.setText(XpadMessages.SYNTAX_TYPE);
		documentMenu.add(syntaxTypeMenu);
		syntaxTypeMenu.add(TextStyleAction.createCheckBoxMenu(editorInstance));
		syntaxTypeMenu.add(ScilabStyleAction.createCheckBoxMenu(editorInstance));
		syntaxTypeMenu.add(XMLStyleAction.createCheckBoxMenu(editorInstance));
		documentMenu.addSeparator();
		Menu encodingTypeMenu = ScilabMenu.createMenu();
		encodingTypeMenu.setText(XpadMessages.ENCODING_TYPE);
		documentMenu.add(encodingTypeMenu);
		encodingTypeMenu.add(ASCIIEncodingAction.createCheckBoxMenu(editorInstance));
		encodingTypeMenu.add(UTF8EncodingAction.createCheckBoxMenu(editorInstance));
		documentMenu.addSeparator();
		documentMenu.add(ColorizeAction.createMenu(editorInstance));
		documentMenu.add(AutoIndentAction.createCheckBoxMenu(editorInstance));
		menuBar.add(documentMenu);

		// Create EXECUTE menubar
		Menu executeMenu = ScilabMenu.createMenu();
		executeMenu.setText(XpadMessages.EXECUTE);
		executeMenu.setMnemonic('e');
		executeMenu.add(LoadIntoScilabAction.createMenu(editorInstance));
		evaluateSelectionMenuItem = EvaluateSelectionAction.createMenu(editorInstance);
		executeMenu.add(evaluateSelectionMenuItem);
		executeMenu.add(ExecuteIntoScilabAction.createMenu(editorInstance));
		menuBar.add(executeMenu);

		//Create HELP menubar
		Menu helpMenu = ScilabMenu.createMenu();
		helpMenu.setText("?");
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
		//toolBar.add(PrintPreviewAction.createButton(editorInstance)); // PRINT PREVIEW
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

		TextBox infoBar = ScilabTextBox.createTextBox();

		editorInstance.setMenuBar(menuBar);
		editorInstance.setToolBar(toolBar);
		editorInstance.setInfoBar(infoBar);
		mainWindow.setTitle(XPAD);
		mainWindow.setVisible(true);
		editorInstance.setCallback(ExitAction.createMenu(editorInstance).getCallback());
		return editorInstance;
	}

	/**
	 * Close current tab (file)
	 */
	public void closeCurrentTab() {
		closeTabAt(tabPane.getSelectedIndex());
	}

	/**
	 * Close a tab using its index
	 * @param indexTab the index of the tab
	 */
	public void closeTabAt(int indexTab) {
		
		JTextPane textPaneAt = (JTextPane) ((JScrollPane) tabPane.getComponentAt(indexTab)).getViewport().getComponent(0);

		if (((ScilabStyleDocument) textPaneAt.getStyledDocument()).isContentModified()) {
			int choice = JOptionPane.showConfirmDialog(this, XpadMessages.MODIFIED);
			if (choice == 0) {
				save(textPaneAt);
			} else if (choice == 1) {
				String closedTabName = tabPane.getTitleAt(tabPane.getSelectedIndex());
				if(getTextPane().getName() == null) {
					String closedTabNameIndex = closedTabName.substring(closedTabName.length() - 1, closedTabName.length());
					tabList.removeElement(Integer.parseInt(closedTabNameIndex));
					closedTabList.add(Integer.parseInt(closedTabNameIndex));
				}
				tabPane.remove(tabPane.getSelectedComponent());
			} else if (choice == 2) {
				return;
			}
		} else {
			String closedTabName = tabPane.getTitleAt(tabPane.getSelectedIndex());
			if(getTextPane().getName() == null) {
				String closedTabNameIndex = closedTabName.substring(closedTabName.length() - 1, closedTabName.length());
				tabList.removeElement(Integer.parseInt(closedTabNameIndex));
				closedTabList.add(Integer.parseInt(closedTabNameIndex));
			}
			tabPane.remove(tabPane.getSelectedComponent());
		}

	}


	/**
	 * Save a file
	 * @param textPane the textPane containing the file contents
	 * @return execution status
	 */
	public boolean save(JTextPane textPane) {

		boolean isSuccess = false;
		if (textPane.getName() != null) {
			try {
// TODO: imho should use File.createTempFile("Sci",".sci") and .renameTo(textPane.getName()) to be safe
				File newSavedFiled = new File(textPane.getName());
				if( (lastKnownSavedState !=0) && (newSavedFiled.lastModified()> lastKnownSavedState)){
					int actionDialog = JOptionPane.showConfirmDialog(this
						, String.format(XpadMessages.EXTERNAL_MODIFICATION, newSavedFiled.getPath())
								 ,XpadMessages.REPLACE_FILE_TITLE, JOptionPane.YES_NO_OPTION);
					if (actionDialog == JOptionPane.NO_OPTION) {
						return this.saveAs(this.getTextPane());
					}
				}
				String doc = textPane.getText();
				ScilabStyleDocument styledDocument = (ScilabStyleDocument) textPane.getStyledDocument();
				FileWriter writer = new FileWriter(newSavedFiled, false);
				try {
					editorKit.write(writer, styledDocument, 0, styledDocument.getLength());
				} catch (BadLocationException e){
					System.err.println("");
			    	e.printStackTrace();
				}
				writer.flush();
				writer.close();
				styledDocument.setContentModified(false);

				int index = getTabPane().getSelectedIndex();
				getTabPane().setTitleAt(index, newSavedFiled.getName());
				editor.setTitle(newSavedFiled.getPath() + " - " + XpadMessages.SCILAB_EDITOR);
				isSuccess = true;
				
				// Get current file path for Execute into Scilab
				fileFullPath = newSavedFiled.getAbsolutePath();

			} catch (IOException ioex) {
				JOptionPane.showMessageDialog(this, ioex);
			}
		} else {
			isSuccess = saveAs(textPane); 
		}
		lastKnownSavedState = System.currentTimeMillis();
		return isSuccess;
	}

	/**
	 * Save a file
	 * @param textPane the textPane containing the file contents
	 * @return execution status
	 */
	public boolean saveAs(JTextPane textPane) {

		boolean isSuccess = false;
		String extension = new String();

		String initialDirectoryPath = getTextPane().getName();
		if (initialDirectoryPath == null) {
			initialDirectoryPath =  ConfigManager.getLastOpenedDirectory();
		}

		SciFileFilter sceFilter = new SciFileFilter(ALL_SCE_FILES , null , 0);
		SciFileFilter scxFilter = new SciFileFilter("*.sc*" , null , 1);
		SciFileFilter sciFilter = new SciFileFilter(ALL_SCI_FILES , null , 2);

		SwingScilabFileChooser fileChooser = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());

		fileChooser.setInitialDirectory(ConfigManager.getLastOpenedDirectory());
		fileChooser .setAcceptAllFileFilterUsed(true);
		fileChooser .setInitialDirectory(initialDirectoryPath);
		fileChooser .setUiDialogType(Juigetfile.SAVE_DIALOG);		
		fileChooser.addChoosableFileFilter(scxFilter);
		fileChooser.addChoosableFileFilter(sceFilter);
		fileChooser.addChoosableFileFilter(sciFilter);

		int retval = fileChooser.showSaveDialog(this);

		if (retval == JFileChooser.APPROVE_OPTION) {
			File f = fileChooser.getSelectedFile();
			initialDirectoryPath = f.getPath();
			if (f.exists()) {
				int actionDialog = JOptionPane.showConfirmDialog(this, XpadMessages.REPLACE_FILE_TITLE, 
						XpadMessages.FILE_ALREADY_EXIST, JOptionPane.YES_NO_OPTION);
				if (actionDialog == JOptionPane.NO_OPTION) {
					return this.saveAs(this.getTextPane());
				}

			}

			try {
				String doc = this.getTextPane().getText();

				/*we test if the file has already a scilab extension*/
				boolean hasNoExtension = true;

				for (int i = 0; i < Juigetfile.DEFAULT_MASK.length; i++) {
					if (f.getName().endsWith(SCI_EXTENSION) || f.getName().endsWith(SCE_EXTENSION)) {
						hasNoExtension = false;
						break;
					}

				}
				/*if no extension , we add it */
				if (hasNoExtension) {

					if (fileChooser.getFileFilter() == sciFilter) {
						extension = SCI_EXTENSION;
					} else if (fileChooser.getFileFilter() == sceFilter) {
						extension = SCE_EXTENSION;
					} else if (fileChooser.getFileFilter() == scxFilter) {
						extension = SCE_EXTENSION;
					} else {
						extension = "";
					}
					f = new File(f.getPath() + extension);
				}
								
				// TODO factor common code with "Save"
				ScilabStyleDocument styledDocument = (ScilabStyleDocument) textPane.getStyledDocument();
				FileWriter writer = new FileWriter(f);
				try {
					editorKit.write(writer, styledDocument, 0, styledDocument.getLength());
				} catch (BadLocationException e){
					System.err.println("");
			    	e.printStackTrace();
				}
				writer.flush();
				writer.close();

				ConfigManager.saveLastOpenedDirectory(f.getPath());
				ConfigXpadManager.saveToRecentOpenedFiles(f.getPath());
				textPane.setName(f.getPath());
				getTabPane().setTitleAt(getTabPane().getSelectedIndex() , f.getName());
				editor.setTitle(f.getPath() + " - " + XpadMessages.SCILAB_EDITOR);
				updateRecentOpenedFilesMenu();

				styledDocument.setContentModified(false);
				lastKnownSavedState = System.currentTimeMillis();
				isSuccess = true;
				
				// Get current file path for Execute into Scilab
				fileFullPath = f.getAbsolutePath();

			} catch (IOException ioex) {
				ioex.printStackTrace();
				JOptionPane.showMessageDialog(this, ioex);
			}

		}
		return isSuccess;
	}

	/**
	 * Create a new tab in Xpad
	 * @param title the title of the tab
	 * @return the text component inside the tab
	 */
	public JTextPane addTab(String title) {
		textPane = new JTextPane();

		scrollingText = new JScrollPane(textPane);

		// Panel of line number for the text pane
		xln = new XpadLineNumberPanel(textPane);
		scrollingText.setRowHeaderView(xln);

		tabPane.add(title, scrollingText);
		tabPane.setSelectedIndex(tabPane.getTabCount() - 1);
		this.setContentPane(tabPane);
		textPane.setBorder(BorderFactory.createEmptyBorder(2, 2, 2, 2));
		textPane.setFont(ConfigXpadManager.getFont());

		textPane.setBackground(ConfigXpadManager.getXpadBackgroundColor());
		textPane.setCaretColor(Color.BLACK);
		textPane.setStyledDocument(new ScilabStyleDocument(this));
		textPane.setCharacterAttributes(textPane.getStyle("Default"), true);

		textPane.setFocusable(true);

		return textPane;
	}

	/**
	 * Create an empty tab inside Xpad
	 * @return the text component inside the tab
	 */
	public JTextPane addEmptyTab() {

		if (closedTabList.size() > 0) {
			Object obj = Collections.min(closedTabList);
			closedTabList.removeElement(Integer.parseInt(obj.toString()));
			return addTab(XpadMessages.UNTITLED + obj.toString());

		} else {
			numberOfUntitled++;
			tabList.add(numberOfUntitled);
			return addTab(XpadMessages.UNTITLED + numberOfUntitled);
		}
	}

	/**
	 * Auto-indent mode management
	 * @param b true to activate auto-indent mode
	 */
	public void setAutoIndent(boolean b) {
		((ScilabStyleDocument) getTextPane().getStyledDocument()).setAutoIndent(b);
	}

	/**
	 * Undo last modification
	 */
	public void undo() {
		ScilabStyleDocument doc = (ScilabStyleDocument) getTextPane().getStyledDocument();
		UndoManager undo = doc.getUndoManager();
		if (undo.canUndo()) {
			try {
				System.out.println(undo.canUndo());
				System.err.println("Will undo " + undo.getUndoPresentationName());
				undo.undo();
				if(!undo.canUndo()){ // remove "*" prefix from tab name
					JTabbedPane current = getTabPane();
					int index = current.getSelectedIndex();
					String namePrefixedByStar = current.getTitleAt(index);
					current.setTitleAt(index, namePrefixedByStar.substring(1, namePrefixedByStar.length()));
					doc.setContentModified(false);
				}			
				repaint();
			} catch (CannotUndoException ex) {
				System.out.println("Unable to undo: " + ex);
				ex.printStackTrace();
			}
		}
	}

	/**
	 * Redo last modification
	 */
	public void redo() {
		UndoManager redo = ((ScilabStyleDocument) getTextPane().getStyledDocument()).getUndoManager();
		if (redo.canRedo()) {
			try {
				System.err.println("Will redo " + redo.getRedoPresentationName());
				redo.redo();
			} catch (CannotRedoException ex) {
				System.out.println("Unable to redo: " + ex);
				ex.printStackTrace();
			}
		}
	}

	/**
	 * Load a file inside Xpad
	 * @param f the file to open
	 */
	public void readFile(File f) {
		/** Is this file already opened */
		boolean alreadyOpened = false;
		for (int i = 0; i < tabPane.getTabCount(); i++) {
			JTextPane textPaneAt = (JTextPane) ((JScrollPane) tabPane.getComponentAt(i)).getViewport().getComponent(0);
			if (f.getAbsolutePath().equals(textPaneAt.getName())) {
				/* File is already opnened */
				tabPane.setSelectedIndex(i);
				alreadyOpened = true;
				break;
			}
		}
		
		if (!alreadyOpened) {
			ReadFileThread myReadThread = new ReadFileThread(f);
			myReadThread.start();
		}
	}

	/**
	 * Load a file inside Xpad
	 * @param f the file to open
	 */
	public void readFileAndWait(File f) {
		/** Is this file already opened */
		boolean alreadyOpened = false;
		for (int i = 0; i < tabPane.getTabCount(); i++) {
			JTextPane textPaneAt = (JTextPane) ((JScrollPane) tabPane.getComponentAt(i)).getViewport().getComponent(0);
			if (f.getAbsolutePath().equals(textPaneAt.getName())) {
				/* File is already opnened */
				tabPane.setSelectedIndex(i);
				alreadyOpened = true;
				break;
			}
		}
		if (!alreadyOpened) {
			ReadFileThread myReadThread = new ReadFileThread(f);
			myReadThread.start();
			synchronized (synchro) {
				try {
					synchro.wait();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}

	/**
	 * Get current text component
	 * @return the text component
	 */
	public JTextPane getTextPane() {
		try {
			return (JTextPane) ((JScrollPane) tabPane.getSelectedComponent()).getViewport().getComponent(0);
		} catch (NullPointerException e) {
			return null;
		}
	}

	/**
	 * Get Xpad as a Tab
	 * @return Xpad instance
	 * @see org.scilab.modules.gui.tab.Tab#getAsSimpleTab()
	 */
	public SimpleTab getAsSimpleTab() {
		return this;
	}

	/**
	 * Get Xpad parent Window
	 * @return parent Window
	 * @see org.scilab.modules.gui.tab.Tab#getParentWindow()
	 */
	public Window getParentWindow() {
		return parentWindow;
	}

	/**
	 * Add a status bar to Xpad
	 * @param infoBarToAdd the status bar to be added
	 * @see org.scilab.modules.gui.uielement.UIElement#addInfoBar(org.scilab.modules.gui.textbox.TextBox)
	 */
	public void addInfoBar(TextBox infoBarToAdd) {
		setInfoBar(infoBarToAdd);
	}

	/**
	 * Add a menu bar to Xpad
	 * @param menuBarToAdd the menu bar to be added
	 * @see org.scilab.modules.gui.uielement.UIElement#addMenuBar(org.scilab.modules.gui.menubar.MenuBar)
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		setMenuBar(menuBarToAdd);
	}

	/**
	 * Add a tool bar to Xpad
	 * @param toolBarToAdd the tool bar to be added
	 * @see org.scilab.modules.gui.uielement.UIElement#addToolBar(org.scilab.modules.gui.toolbar.ToolBar)
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		setToolBar(toolBarToAdd);
	}

	/**
	 * Get Xpad main JTabbedPane
	 * @return Xpad main JTabbedPane
	 */
	public JTabbedPane getTabPane() {
		return tabPane;
	}

	/**
	 * Set Xpad main JTabbedPane
	 * @param tabPane Xpad main JTabbedPane
	 */
	public void setTabPane(JTabbedPane tabPane) {
		this.tabPane = tabPane;
	}

	/**
	 * Get the panel used to display line numbers
	 * @return the panel
	 */
	public XpadLineNumberPanel getXln() {
		return xln;
	}

	/**
	 * Set the panel used to display line numbers
	 * @param xln the panel
	 */
	public void setXln(XpadLineNumberPanel xln) {
		this.xln = xln;
	}

	/**
	 * Get recent file menu
	 * @return the menu
	 */
	public Menu getRecentsMenu() {
		return  this.recentsMenu;
	}

	/**
	 * Line numbers display status
	 * @param display true to view line numbers
	 */
	public void displayLineNumbers(boolean display) {
		if (display) {
			scrollingText.setRowHeaderView(xln);
			repaint();
		} else {
			scrollingText.setRowHeaderView(null);
			repaint();
		}
	}

	/**
	 * Highlight mode management
	 * @param display true to hightlight current line
	 */
	public void enableLineHighlight(boolean display) {
		xln.setCurrentLineHighlightColor(display);
		repaint();
	}

	/**
	 * Update menu displaying recent opened files
	 */
	public void updateRecentOpenedFilesMenu() {
		ArrayList<File> recentFiles = ConfigXpadManager.getAllRecentOpenedFiles();

		((SwingScilabMenu) getRecentsMenu().getAsSimpleMenu()).removeAll();
		for (int i = 0; i < recentFiles.size(); i++) {
			getRecentsMenu().add(RecentFileAction.createMenu(editor, recentFiles.get(i)));
		}

	}

	/**
	 * Button displayed on top of tabs
	 * @author Bruno JOFRET
	 */
	private class TabButton extends JButton implements ActionListener {
		
		private static final long serialVersionUID = 2643702189238507205L;

		/**
		 * Constructor 
		 */
		public TabButton() {
			int size = BUTTON_SIZE;
			setPreferredSize(new Dimension(size, size));
			setToolTipText(XpadMessages.CLOSE_TAB_TIP);

			setUI(new BasicButtonUI());
			//Make it transparent
			setContentAreaFilled(false);
			//No need to be focusable
			setFocusable(false);
			setBorder(BorderFactory.createEtchedBorder());
			setBorderPainted(false);
			setRolloverEnabled(true);
			//Close the proper tab by clicking the button
			addActionListener(this);            
		}

		/**
		 * User actions management
		 * @param e the action
		 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
		 */
		public void actionPerformed(ActionEvent e) {

		}

		/**
		 * Update management
		 * @see javax.swing.JButton#updateUI()
		 */
		public void updateUI() {
			//we don't want to update UI for this button
		}
	}
	
	private class ReadFileThread extends Thread{
  		
		private File fileToRead;
		
		public ReadFileThread(File f) {
			this.fileToRead = f;
		}

		public void run() {
			readFile(fileToRead);
			this.stop();
			System.err.println("I'm still alive baaaaahhhh");
		}
	   
		public void readFile(File f) {
			/* First try to open file before creating tab */
			getInfoBar().setText("Loading...");
			StringBuilder contents = new StringBuilder();
			JTextPane theTextPane = addTab(f.getName()); 
			String eof = System.getProperty("line.separator");
			ScilabStyleDocument styleDocument = (ScilabStyleDocument) theTextPane.getStyledDocument();
			System.out.println("File = " + f.getAbsolutePath());
			theTextPane.setName(f.getAbsolutePath());
			try {
					synchronized (styleDocument) {
						styleDocument.disableUpdaters();
						boolean indentMode= styleDocument.getAutoIndent();
						styleDocument.setAutoIndent(false); 
						try {
							editorKit.read(new BufferedReader(new FileReader(f)), styleDocument, 0);
						} catch (BadLocationException e) {
							System.err.println("");
							e.printStackTrace();
						}
						// TODO : make colorize threadsafe to be able to keep the colorizing updater running when loading
						styleDocument.colorize(0, styleDocument.getLength());
						styleDocument.setAutoIndent(indentMode);
						styleDocument.enableUpdaters();
					}
				getTabPane().setTitleAt(getTabPane().getSelectedIndex() , f.getName());
				styleDocument.setContentModified(false);
				getInfoBar().setText("");
				
				// Empty the undo Manager
				UndoManager undo = ((ScilabStyleDocument) getTextPane().getStyledDocument()).getUndoManager();
				undo.discardAllEdits();
				
			} catch (IOException ioex) {

				int choice = JOptionPane.showConfirmDialog(editor, String.format(XpadMessages.FILE_DOESNT_EXIST,f.getAbsolutePath()));
				if (choice  == 0) {
					try {
						FileWriter writer = new FileWriter(f);
						writer.write("");
						writer.flush();
						writer.close();

						readFile(f);
					} catch (IOException ioexc) {
						JOptionPane.showMessageDialog(editor , ioexc);
					}
				}	
			}
			synchronized (synchro) {
				synchro.notify();
			}
		}

	}

	public String getFileFullPath() {
		return fileFullPath;
	}

}
