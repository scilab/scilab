/*9
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
import java.awt.event.ActionEvent;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.StringReader;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.ChangedCharSetException;
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
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.SciFileFilter;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.xpad.actions.ExitAction;
import org.scilab.modules.xpad.actions.FindAction;
import org.scilab.modules.xpad.actions.GotoLineAction;
import org.scilab.modules.xpad.actions.LineBeautifierAction;
import org.scilab.modules.xpad.actions.RecentFileAction;
import org.scilab.modules.xpad.actions.SetColorsAction;
import org.scilab.modules.xpad.actions.TabifyAction;
import org.scilab.modules.xpad.actions.UnTabifyAction;
import org.scilab.modules.xpad.style.ColorizationManager;
import org.scilab.modules.xpad.style.CompoundUndoManager;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.DropFilesListener;
import org.scilab.modules.xpad.utils.SaveFile;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * Main Xpad class
 * @author Bruno JOFRET
 */
public class Xpad extends SwingScilabTab implements Tab { 

	private static final long serialVersionUID = -6410183357490518676L;
	
	private static final String XPAD = "Editor";
	private static final String SCI_EXTENSION = ".sci";
	private static final String SCE_EXTENSION = ".sce";
	private static final String ALL_SCI_FILES = "*.sci";
	private static final String ALL_SCE_FILES = "*.sce";
	
	private static Xpad editor;

	private final Window parentWindow;
	private static XpadGUI xpadGUI;
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
	
	//private static org.scilab.modules.gui.menuitem.MenuItem evaluateSelectionMenuItem;
	
	private File fileToEncode;
	 
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
						path  =  " (" + getTextPane().getName() + ")";
					}
					setTitle(tabPane.getTitleAt(tabPane.getSelectedIndex()) + path + " - " + XpadMessages.SCILAB_EDITOR);
					
					// This listener is for 'evaluate selection' of the Execute menu
					// it enable the menuItem only if something is selected
					textPane.addCaretListener(new CaretListener() {
						public void caretUpdate(CaretEvent e) {
						    int dot = e.getDot();
						    int mark = e.getMark();
						    if (dot == mark) {  // no selection
						    	XpadGUI.getEvaluateSelectionMenuItem().setEnabled(false);
						    } else {
						    	XpadGUI.getEvaluateSelectionMenuItem().setEnabled(true);
						    }
						}
					});
					updateUI();
					
					// Update encoding menu
					xpadGUI.updateEncodingMenu((ScilabStyleDocument) getTextPane().getStyledDocument());
					
					// Update End Of Line  menu					
					xpadGUI.updateEolMenu((ScilabStyleDocument) getTextPane().getStyledDocument());
				}
			}
		});

		this.setContentPane(tabPane);
	}

	/**
	 * Return the text editor GUI.
	 * @return The text editor gui
	 */
	public final XpadGUI getXpadGUI() {
		return xpadGUI;
	}

	/**
	 * Launch Xpad from command line.
	 * @param args command line args
	 */
	public static void main(String[] args) {
		xpad();
	}

	/**
	 * Launch Xpad with an empty file
	 */
	public static void xpad() {
		try  {
			SwingUtilities.invokeAndWait( new Thread(){
				public void run() {
					Xpad editorInstance = launchXpad();
					editorInstance.addEmptyTab();				
				}
			});
		} catch(InterruptedException e) {
			System.err.println("EDT interrupted "+e);
		} catch(java.lang.reflect.InvocationTargetException e) {
			System.err.println(" xpad() throw: "+e);
		}
		
	}

	/**
	 * Launch Xpad with a file name to open.
	 * @param filePath the name of the file to open
	 */
	public static void xpad(String filePath) {
		Xpad editorInstance = launchXpad();
		File f = new File(filePath);
		if (f.isDirectory()) { /* Bug 5131 */
		    ScilabModalDialog.show(Xpad.getEditor(), String.format(XpadMessages.CANNOT_LOAD_DIRECTORY, f.getAbsolutePath()),
			    XpadMessages.XPAD_ERROR, IconType.ERROR_ICON);
		    xpad();
		    return;
		}
		ConfigXpadManager.saveToRecentOpenedFiles(filePath);
		editorInstance.updateRecentOpenedFilesMenu();
		editorInstance.readFileAndWait(f);
		editorInstance.lastKnownSavedState = System.currentTimeMillis();
	}

	/**
	 * Launch Xpad with a file name to open and a line to highlight.
	 * @param filePath the name of the file to open
	 * @param lineNumber the line to highlight
	 */
	public static void xpad(String filePath, int lineNumber) {
		Xpad editorInstance = launchXpad();
		File f = new File(filePath);
		ConfigXpadManager.saveToRecentOpenedFiles(filePath);
		editorInstance.updateRecentOpenedFilesMenu();
		editorInstance.readFileAndWait(f);
		editorInstance.getXln().highlightLine(lineNumber);
		editorInstance.lastKnownSavedState = System.currentTimeMillis();
	}

	/**
	 * Launch Xpad with a provided text (from the help for example).
	 * @param text the text which should be modified
	 */
	public static void xpadWithText(String text) {
		Xpad editorInstance = launchXpad();
		editorInstance.lastKnownSavedState = System.currentTimeMillis(); 
		JTextPane theTextPane = editorInstance.addEmptyTab();
		ScilabStyleDocument styleDocument = (ScilabStyleDocument) theTextPane.getStyledDocument();
		try {
			editorInstance.getEditorKit().read(new StringReader(text), styleDocument, 0);
			boolean colorStatus = new ColorizationManager().colorize(styleDocument, 0, styleDocument.getLength());
			if (!colorStatus) {
				editorInstance.getInfoBar().setText(XpadMessages.COLORIZATION_CANCELED);
			} else {
				editorInstance.getInfoBar().setText("");
			}

		} catch (IOException e) {
			System.err.println("Error while reading the String");
		} catch (BadLocationException e) {
			System.err.println("Error while reading the String");
		}
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
	 * Close Xpad instance including all tabs.
	 */
	public static void closeXpad() {
		
		FindAction.closeFindReplaceWindow();
		GotoLineAction.closeGotoLineWindow();
		SetColorsAction.closeSetColorsWindow();
		editor = null;
	}


	/**
	 * Create Xpad instance.
	 * @return the instance
	 */
	private static Xpad createEditor() {
		ConfigXpadManager.createUserCopy();
		Window mainWindow = ScilabWindow.createWindow();

		Xpad editorInstance = new Xpad(mainWindow);

		xpadGUI = new XpadGUI(mainWindow, editorInstance, XPAD);
		editorInstance.setCallback(new CallBack(XpadMessages.DEFAULT + XpadMessages.DOTS) {
			/**
			 * serialVersionUID
			 */
			private static final long serialVersionUID = -4121140054209319523L;

			/**
			 * Action callback on Exit menu
			 */
			public void callBack() {
				if (ScilabModalDialog.show(Xpad.getEditor(), XpadMessages.EXIT_CONFIRM, XpadMessages.EXIT, 
		    			IconType.WARNING_ICON, ButtonType.YES_NO) == AnswerOption.YES_OPTION) {
						ExitAction.doExit(Xpad.getEditor());						
				}
			}
			
			/**
			 * actionPerformed
			 * @param e ActionEvent
			 */
			public void actionPerformed(ActionEvent e) {
				callBack();
			} 
		});
			
		return editorInstance;
	}

	/**
	 * Close a tab using its index.
	 * @param indexTab the index of the tab
	 * @return if the tab has been really closed
	 */
	public boolean closeTabAt(int indexTab) {
	    return closeTabAt(indexTab, false);
	}

	/**
	 * Close a tab using its index.
	 * @param indexTab the index of the tab
	 * @param scilabClose if true, save dialog confirmation cannot be canceled
	 * @return if the tab has been really closed
	 */
	public boolean closeTabAt(int indexTab, boolean scilabClose) {
		
		JTextPane textPaneAt = (JTextPane) ((JScrollPane) tabPane.getComponentAt(indexTab)).getViewport().getComponent(0);
		
		/* Test for modification added after bug 5103 fix: do not ask the user for an Untitled not-modified file saving when closing Xpad */
		if (((ScilabStyleDocument) textPaneAt.getStyledDocument()).isContentModified()) {
			if (!save(indexTab, false, scilabClose)) {
				return false;
			}
		}

		if (textPaneAt.getName() == null) {
			String closedTabName = tabPane.getTitleAt(indexTab);
			String closedTabNameIndex = closedTabName.substring(closedTabName.length() - 1, closedTabName.length());
			tabList.removeElement(Integer.parseInt(closedTabNameIndex));
			closedTabList.add(Integer.parseInt(closedTabNameIndex));
		}
		
		// correction for bug 5404, closing the last tabPane generate an exception
		// that's why we need to remove ChangeListeners before closing
		if ((indexTab == 0) && (getEditor().getTabPane().getTabCount() == 1)) {
			for (int i = 0; i < tabPane.getChangeListeners().length; i++) {
				tabPane.removeChangeListener(tabPane.getChangeListeners()[i]);
			}
		}
		
		tabPane.remove(indexTab);
		return true;
		
	}

	/**
	 * Save a file.
	 * @param indexTab the textPane index containing the file contents
	 * @return execution status
	 */
	public boolean save(int indexTab) {
	    return save(indexTab, false, false);
	}

	/**
	 * Save a file.
	 * @param indexTab the textPane index containing the file contents
	 * @param force : force save without confirmation dialog ( SaveAction )
	 * @return execution status
	 */
	public boolean save(int indexTab, boolean force) {
	    return save(indexTab, force, false);
	}

	/**
	 * Save a file.
	 * @param indexTab the textPane index containing the file contents
	 * @param force : force save without confirmation dialog ( SaveAction )
	 * @param scilabClose : if true, save dialog confirmation cannot be canceled
	 * @return execution status
	 */

	public boolean save(int indexTab, boolean force, boolean scilabClose){

		JTextPane textPaneAt = (JTextPane) ((JScrollPane) tabPane.getComponentAt(indexTab)).getViewport().getComponent(0);
		//if the file ( empty, new or loaded ) is not modified, exit save process and return true
		if (!((ScilabStyleDocument) textPaneAt.getStyledDocument()).isContentModified() 
				&& (textPaneAt.getName() != null)) { /* Bug 5103 fix */
			return true;
		}
		
		if (!force) {
		    AnswerOption answer;
		    if (scilabClose == true) {
				answer = ScilabModalDialog.show(Xpad.getEditor(), editor.getTabPane().getTitleAt(indexTab) + XpadMessages.MODIFIED, 
				XpadMessages.SCILAB_EDITOR, IconType.QUESTION_ICON, ButtonType.YES_NO);
		    } else {
				answer = ScilabModalDialog.show(Xpad.getEditor(), editor.getTabPane().getTitleAt(indexTab) + XpadMessages.MODIFIED, 
				XpadMessages.SCILAB_EDITOR, IconType.QUESTION_ICON, ButtonType.YES_NO_CANCEL);
		    }

		    switch (answer) {
		    case YES_OPTION : //Yes, continue
		    	break;
		    case NO_OPTION ://No, exit and returns true
		    	return true;
		    case CANCEL_OPTION : //Cancel, exit and return false
		    	return false;
			default:
				break;
		    }
		}

		String fileToSave = textPaneAt.getName();
		if (fileToSave == null) {
			//need a filename, call chooseFileToSave
			fileToSave = chooseFileToSave();
		} else {
			//check if the file has been modified by external software
			fileToSave = checkExternalModification(fileToSave);
		}

		if (fileToSave == null) {
			return true; /* Bug 5189: The user cancels ==> do not want an error message */
		}

		File newSavedFile = new File(fileToSave);
		
		if (SaveFile.doSave(textPaneAt, newSavedFile, editorKit) == false) return false;

		ScilabStyleDocument styledDocument = (ScilabStyleDocument) textPaneAt.getStyledDocument();
		styledDocument.setContentModified(false);

		getTabPane().setTitleAt(getTabPane().getSelectedIndex() , newSavedFile.getName());
		editor.setTitle(newSavedFile.getPath() + " - " + XpadMessages.SCILAB_EDITOR);
		
		// Get current file path for Execute file into Scilab 
		fileFullPath = newSavedFile.getAbsolutePath();
		lastKnownSavedState = System.currentTimeMillis();

		textPaneAt.setName(fileToSave);
		return true;
	}

	/**
	 * Check for external modification. If it is the case, propose
	 * to select a new file name.
	 * @param filename The name of the file
	 * @return the filename where to save
	 */
	public String checkExternalModification(String filename) {
	    File newSavedFiled = new File(filename);

	    if ((lastKnownSavedState != 0) && (newSavedFiled.lastModified() > lastKnownSavedState)) {
		if (ScilabModalDialog.show(Xpad.getEditor(), String.format(XpadMessages.EXTERNAL_MODIFICATION, newSavedFiled.getPath()), 
			XpadMessages.REPLACE_FILE_TITLE, IconType.QUESTION_ICON, 
			ButtonType.YES_NO) == AnswerOption.NO_OPTION) {
		    return chooseFileToSave();
		}
	    }
	    return filename;
	}

	/**
	 * Return through a file selector the name of the selected
	 * file.
	 * @return the file picked up by the user
	 */
	public String chooseFileToSave() {
		String extension = new String();

		String initialDirectoryPath = getTextPane().getName();
		if (initialDirectoryPath == null) {
			initialDirectoryPath =  ConfigManager.getLastOpenedDirectory();
		}

		SciFileFilter sceFilter = new SciFileFilter(ALL_SCE_FILES , null , 0);
		SciFileFilter sciFilter = new SciFileFilter(ALL_SCI_FILES , null , 1);
		SciFileFilter scxFilter = new SciFileFilter("*.sc*" , null , 2);
		SciFileFilter allFilter = new SciFileFilter("*.*" , null , 3);

		SwingScilabFileChooser fileChooser = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());

		fileChooser.setInitialDirectory(ConfigManager.getLastOpenedDirectory());
		fileChooser .setAcceptAllFileFilterUsed(false);
		fileChooser .setInitialDirectory(initialDirectoryPath);
		fileChooser .setUiDialogType(Juigetfile.SAVE_DIALOG);		
		
		// order is also important here
		fileChooser.addChoosableFileFilter(sceFilter);
		fileChooser.addChoosableFileFilter(sciFilter);
		fileChooser.addChoosableFileFilter(scxFilter);
		fileChooser.addChoosableFileFilter(allFilter);

		//select default file type
		fileChooser.setFileFilter(sceFilter);
		int retval = fileChooser.showSaveDialog(this);

		if (retval == JFileChooser.APPROVE_OPTION) {
			File f = fileChooser.getSelectedFile();
			initialDirectoryPath = f.getPath();
			if (f.exists()) {
			    if (ScilabModalDialog.show(Xpad.getEditor(), XpadMessages.REPLACE_FILE_TITLE, 
				    XpadMessages.FILE_ALREADY_EXIST, IconType.QUESTION_ICON,
				    ButtonType.YES_NO) == AnswerOption.NO_OPTION) {
				return chooseFileToSave();
			    }
			}

			/*we test if the file has already a scilab extension*/
			boolean hasNoExtension = true;

			// if the file name is like this : any character , a dot , then 2,3or 4 characters, then
			// we consider the file has already an extension
			// we previously only check for .sci and .sce extension, but what if the user open a txt file
			String fileName = f.getName();
			if (fileName.lastIndexOf(".")!= -1 ){
				if ( fileName.substring(fileName.lastIndexOf("."),fileName.length()).length() >= 2
					&& fileName.substring(fileName.lastIndexOf("."),fileName.length()).length() <= 4){
					hasNoExtension = false;
				}
				
			}
				/*
			for (int i = 0; i < Juigetfile.DEFAULT_MASK.length; i++) {
				if (f.getName().endsWith(SCI_EXTENSION) || f.getName().endsWith(SCE_EXTENSION)) {
					hasNoExtension = false;
					break;
				}

			}
			*/
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
				return f.getPath() + extension;
			}
			return f.getPath();
		}
		return null;
	}

	/**
	 * Save a file.
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

		
		//prefer to use chooseFileToSave function !
		SciFileFilter sceFilter = new SciFileFilter(ALL_SCE_FILES , null , 0);
		SciFileFilter sciFilter = new SciFileFilter(ALL_SCI_FILES , null , 1);
		SciFileFilter scxFilter = new SciFileFilter("*.sc*" , null , 2);
		SciFileFilter allFilter = new SciFileFilter("*.*" , null , 3);
		

		SwingScilabFileChooser fileChooser = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());

		fileChooser.setInitialDirectory(ConfigManager.getLastOpenedDirectory());
		fileChooser.setAcceptAllFileFilterUsed(false);
		fileChooser.setInitialDirectory(initialDirectoryPath);
		fileChooser.setUiDialogType(Juigetfile.SAVE_DIALOG);
		
		// order is also important here
		fileChooser.addChoosableFileFilter(sceFilter);
		fileChooser.addChoosableFileFilter(sciFilter);
		fileChooser.addChoosableFileFilter(scxFilter);
		fileChooser.addChoosableFileFilter(allFilter);
		
		//select default file type
		fileChooser.setFileFilter(sceFilter);
		fileChooser.setTitle(XpadMessages.SAVE_AS); /* Bug 4869 */
		
		if (textPane.getName() != null) { /* Bug 5319 */
			fileChooser.setSelectedFile(new File(textPane.getName()));
		}
		
		int retval = fileChooser.showSaveDialog(this);

		if (retval == JFileChooser.APPROVE_OPTION) {
			File f = fileChooser.getSelectedFile();
			initialDirectoryPath = f.getPath();
			if (f.exists()) {
				AnswerOption ans = ScilabModalDialog.show(Xpad.getEditor(), XpadMessages.REPLACE_FILE_TITLE,
						XpadMessages.FILE_ALREADY_EXIST, 
						IconType.QUESTION_ICON, ButtonType.YES_NO);
			    if (ans == AnswerOption.NO_OPTION) {
			    	return this.saveAs(this.getTextPane());
			    }
			}

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

			ScilabStyleDocument styledDocument = (ScilabStyleDocument) textPane.getStyledDocument();

			if (SaveFile.doSave(textPane, f, editorKit) == false) return false;

			ConfigManager.saveLastOpenedDirectory(f.getPath());
			ConfigXpadManager.saveToRecentOpenedFiles(f.getPath());
			textPane.setName(f.getPath());
			getTabPane().setTitleAt(getTabPane().getSelectedIndex() , f.getName());
			editor.setTitle(f.getPath() + " - " + XpadMessages.SCILAB_EDITOR);
			updateRecentOpenedFilesMenu();

			styledDocument.setContentModified(false);
			lastKnownSavedState = System.currentTimeMillis();
			isSuccess = true;

			// Get current file path for Execute file into Scilab 
			fileFullPath = f.getAbsolutePath();

		} else if (retval == JFileChooser.CANCEL_OPTION) {
			/* Bug 5189: The user cancels ==> do not want an error message */
			isSuccess = true;
		}
		return isSuccess;
	}

	/**
	 * Create a new tab in Xpad.
	 * @param title the title of the tab
	 * @return the text component inside the tab
	 */
	public JTextPane addTab(String title) {
		textPane = new JTextPane();
		textPane.setEditorKit(new ScilabEditorKit());
		textPane.getDocument().addDocumentListener(new DocumentListener() {
			ColorizationManager colorizationManager = new ColorizationManager();

			public void changedUpdate(DocumentEvent documentEvent) {
			}

			public void insertUpdate(DocumentEvent documentEvent) {
				handleEvent(documentEvent);
		    }

		    public void removeUpdate(DocumentEvent documentEvent) {
		    	handleEvent(documentEvent);
		    }

		    private void handleEvent(DocumentEvent documentEvent) {
		        DocumentEvent.EventType type = documentEvent.getType();
		        if (type.equals(DocumentEvent.EventType.INSERT) || type.equals(DocumentEvent.EventType.REMOVE) ) {
		        	ScilabStyleDocument doc = ((ScilabStyleDocument)documentEvent.getDocument());
		        	if(doc.getAutoColorize()) {
		        		SwingUtilities.invokeLater(colorizationManager.new ColorUpdater(documentEvent));
		        	}
		        	doc.setContentModified(true);
		        	// tab title updating must be deferred after UndoManager processes the related UndoableEvent
		        	// (and updates nbEdit accordingly)
		        	SwingUtilities.invokeLater(new TabTitleUpdater(Xpad.this));
		        } 
		   }
	});

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
		textPane.setCharacterAttributes(textPane.getStyle("Default"), true);

		TabifyAction.putInInputMap(textPane, this);
		UnTabifyAction.putInInputMap(textPane, this);
		LineBeautifierAction.putInInputMap(textPane);
		
		textPane.setFocusable(true);
		textPane.setRequestFocusEnabled(true);
		textPane.requestFocus();
		textPane.grabFocus();
		textPane.setDragEnabled(true); /* Bug 5497 */
		
		DropFilesListener dndTarget = new DropFilesListener(textPane);
		
		XpadGUI.createPopupMenu(textPane);
		return textPane;
	}

	/**
	 * Create an empty tab inside Xpad.
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
	 * Auto-colorize mode management
	 * @param b true to activate auto-colorize mode
	 */
	public void setAutoColorize(boolean b) {
		((ScilabStyleDocument) getTextPane().getStyledDocument()).setAutoColorize(b);
	}
	
	/**
	 * Add or remove '*' prefix in current tab tile according to isContentModified().
	 */
	public void updateTabTitle(){
		StringBuffer newTitle = new StringBuffer();
		JTextPane currentTextPane = getTextPane();
		if(((ScilabStyleDocument) currentTextPane.getStyledDocument()).isContentModified()) {
			newTitle.append('*');
		}
		String textPaneName = currentTextPane.getName();
		try {
			File f = new File(textPaneName);
			newTitle.append(f.getName());
		} catch (Exception e) { // not a file name, no path prefix to remove, but maybe a '*'
			textPaneName = getTabPane().getTitleAt(getTabPane().getSelectedIndex());
			newTitle.append(textPaneName.charAt(0)=='*'? textPaneName.substring(1, textPaneName.length()) : textPaneName);
		}
		getTabPane().setTitleAt(getTabPane().getSelectedIndex() , newTitle.toString());
	}
	
	class CaretUpdater implements Runnable {
		private JTextPane jtc;
		private int offset;
		
		CaretUpdater(JTextPane jtc, DocumentEvent e){
			this.jtc= jtc;
			this.offset = e.getOffset() + e.getLength();
			
		}
		public void run(){
			jtc.setCaretPosition(Math.min(offset, jtc.getDocument().getLength()));
		}	
	}
	
	class UpdateListener implements DocumentListener {
		public void insertUpdate(final DocumentEvent e)
		{
			updateColor(e);
			SwingUtilities.invokeLater(new CaretUpdater(getTextPane(), e));
		}
		public void removeUpdate(DocumentEvent e)
		{
			updateColor(e);
			getTextPane().setCaretPosition(e.getOffset());
		}
		public void changedUpdate(DocumentEvent e) {}
		
		void updateColor(DocumentEvent e){
			if( e.getType() != DocumentEvent.EventType.CHANGE) {
				SwingUtilities.invokeLater(new ColorizationManager().new ColorUpdater(e));
			}
		}
	}

	/**
	 * Undo last modification.
	 */
	public void undo() {
		ScilabStyleDocument doc = (ScilabStyleDocument) getTextPane().getStyledDocument();
		synchronized (doc) {
			CompoundUndoManager undo = doc.getUndoManager();
			if (undo.canUndo()) {
				UpdateListener cl = new UpdateListener();
				try {
					doc.addDocumentListener(cl);
					undo.undo();
					if (!undo.canUndo()) { // remove "*" prefix from tab name
						doc.setContentModified(false);
						Xpad.this.updateTabTitle();
					}
					repaint();
				} catch (CannotUndoException ex) {
					ex.printStackTrace();
				} finally {
					doc.removeDocumentListener(cl);
				}
			}
		}
	}

	/**
	 * Redo last modification.
	 */
	public void redo() {
		ScilabStyleDocument doc = (ScilabStyleDocument) getTextPane().getStyledDocument();
		synchronized (doc) {
			CompoundUndoManager redo = doc.getUndoManager();
			if (redo.canRedo()) {
				UpdateListener cl = new UpdateListener();
				try {
					doc.addDocumentListener(cl);
					redo.redo();
					if (!doc.isContentModified()) {
						doc.setContentModified(true);
						Xpad.this.updateTabTitle();
					}
				} catch (CannotRedoException ex) {
					ex.printStackTrace();
				} finally {
					doc.removeDocumentListener(cl);
				}
			}
		}
	}

	/**
	 * Load a file inside Xpad.
	 * @param f the file to open
	 */
	public void readFile(File f) {
		/** Is this file already opened */
		boolean alreadyOpened = false;
		for (int i = 0; i < tabPane.getTabCount(); i++) {
			JTextPane  textPaneAt = (JTextPane) ((JScrollPane) tabPane.getComponentAt(i)).getViewport().getComponent(0);
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

		// Get current file path for Execute file into Scilab
		fileFullPath = f.getAbsolutePath();
	}

	/**
	 * Load a file inside Xpad.
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
	 * Get current text component.
	 * @return the text component
	 */
	public JTextPane getTextPane() {
		try {
			return (JTextPane) ((JScrollPane) tabPane.getSelectedComponent()).getViewport().getComponent(0);
		} catch (NullPointerException e) {
			System.err.println("Could not retrieve the current text tab."+e);
			return null;
		} catch( ArrayIndexOutOfBoundsException e) { // can happen between Xpad construction and first call to addTab()
			//System.err.println("no tab (yet?)."+e); 
			return null;
		}
	}

	/**
	 * Get Xpad as a Tab.
	 * @return Xpad instance
	 * @see org.scilab.modules.gui.tab.Tab#getAsSimpleTab()
	 */
	public SimpleTab getAsSimpleTab() {
		return this;
	}

	/**
	 * Get Xpad parent Window.
	 * @return parent Window
	 * @see org.scilab.modules.gui.tab.Tab#getParentWindow()
	 */
	public Window getParentWindow() {
		return parentWindow;
	}

	/**
	 * Add a status bar to Xpad.
	 * @param infoBarToAdd the status bar to be added
	 * @see org.scilab.modules.gui.uielement.UIElement#addInfoBar(org.scilab.modules.gui.textbox.TextBox)
	 */
	public void addInfoBar(TextBox infoBarToAdd) {
		setInfoBar(infoBarToAdd);
	}

	/**
	 * Add a menu bar to Xpad.
	 * @param menuBarToAdd the menu bar to be added
	 * @see org.scilab.modules.gui.uielement.UIElement#addMenuBar(org.scilab.modules.gui.menubar.MenuBar)
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		setMenuBar(menuBarToAdd);
	}

	/**
	 * Add a tool bar to Xpad.
	 * @param toolBarToAdd the tool bar to be added
	 * @see org.scilab.modules.gui.uielement.UIElement#addToolBar(org.scilab.modules.gui.toolbar.ToolBar)
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		setToolBar(toolBarToAdd);
	}

	/**
	 * Get Xpad main JTabbedPane.
	 * @return Xpad main JTabbedPane
	 */
	public JTabbedPane getTabPane() {
		return tabPane;
	}

	/**
	 * Set Xpad main JTabbedPane.
	 * @param tabPane Xpad main JTabbedPane
	 */
	public void setTabPane(JTabbedPane tabPane) {
		this.tabPane = tabPane;
	}

	/**
	 * Get the panel used to display line numbers.
	 * @return the panel
	 */
	public XpadLineNumberPanel getXln() {
		return xln;
	}

	/**
	 * Set the panel used to display line numbers.
	 * @param xln the panel
	 */
	public void setXln(XpadLineNumberPanel xln) {
		this.xln = xln;
	}

	/**
	 * Get recent file menu.
	 * @return the menu
	 */
	public Menu getRecentsMenu() {
		return  this.recentsMenu;
	}

	/**
	 * Line numbers display status.
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
	 * Highlight mode management.
	 * @param display true to hightlight current line
	 */
	public void enableLineHighlight(boolean display) {
		xln.setCurrentLineHighlightColor(display);
		repaint();
	}

	/**
	 * Update menu displaying recent opened files.
	 */
	public void updateRecentOpenedFilesMenu() {
		ArrayList<File> recentFiles = ConfigXpadManager.getAllRecentOpenedFiles();

		((SwingScilabMenu) getRecentsMenu().getAsSimpleMenu()).removeAll();
		for (int i = 0; i < recentFiles.size(); i++) {
			getRecentsMenu().add(RecentFileAction.createMenu(editor, recentFiles.get(i)));
		}

	}

	/**
	 * Return the Full path of the file.
	 * @return the full path
	 */
	public String getFileFullPath() {
		return fileFullPath;
	}

	/**
	 * Dedicated class to read the file.
	 */
	private class ReadFileThread extends Thread {

		private File fileToRead;

		public ReadFileThread(File f) {
			this.fileToRead = f;
			setFileToEncode(f);
		}

		@SuppressWarnings("deprecation")
		public void run() {
			readFile(fileToRead);
			this.stop();
		}

		public void readFile(File f) {
			getInfoBar().setText(XpadMessages.LOADING);

			boolean colorStatus = false;
			// Get current file path for Execute file into Scilab 
			fileFullPath = f.getAbsolutePath();

			ScilabStyleDocument styleDocument = null;
			JTextPane theTextPane;

			// File exist
			if (f.exists()) {
				theTextPane = addTab(f.getName()); 
				styleDocument = (ScilabStyleDocument) theTextPane.getStyledDocument();

				try {
					synchronized (styleDocument) {
						styleDocument.setUpdater(false);
						boolean indentMode = styleDocument.getAutoIndent();
						styleDocument.setAutoIndent(false); 
						try {
							try {
							editorKit.read(new BufferedReader(new InputStreamReader(new FileInputStream(f),styleDocument.getEncoding())), styleDocument, 0);
							} catch(ChangedCharSetException e) {
								editorKit.read(new BufferedReader(new InputStreamReader(new FileInputStream(f),e.getCharSetSpec())), styleDocument, 0);
							}

						} catch (BadLocationException e) {
							e.printStackTrace();
						}
						styleDocument.setAutoIndent(indentMode);
						styleDocument.setUpdater(true);
					}
				} catch (IOException ioex) {
					ioex.printStackTrace();
				}

				theTextPane.setName(f.getAbsolutePath());
				getTabPane().setTitleAt(getTabPane().getSelectedIndex() ,f.getName());
				styleDocument.setContentModified(false);

				getInfoBar().setText("");

				xpadGUI.updateEncodingMenu((ScilabStyleDocument)getTextPane().getStyledDocument());
				
				// File does not exist	
			} else {
				theTextPane = addEmptyTab(); 
				int choice = JOptionPane.showConfirmDialog(
                        editor,
                        String.format(XpadMessages.FILE_DOESNT_EXIST, f.getName()),
                        "Editor",
                        JOptionPane.YES_NO_OPTION);
				if (choice == 0) { //OK
					styleDocument = (ScilabStyleDocument) theTextPane.getStyledDocument();

					BufferedWriter out = null;
					try {
						out = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(f), styleDocument.getEncoding()));
						try {
							editorKit.write(out, styleDocument, 0, styleDocument.getLength());
							out.flush();
							out.close();
						} catch (IOException e) {
							e.printStackTrace();
						} catch (BadLocationException e) {
							e.printStackTrace();
						}
					} catch (UnsupportedEncodingException e2) {
						e2.printStackTrace();
					} catch (FileNotFoundException e2) {
						e2.printStackTrace();
					}

					ConfigManager.saveLastOpenedDirectory(f.getPath());
					ConfigXpadManager.saveToRecentOpenedFiles(f.getPath());
					theTextPane.setName(f.getPath());
					getTabPane().setTitleAt(getTabPane().getSelectedIndex() ,f.getName());
					editor.setTitle(f.getPath() + " - " + XpadMessages.SCILAB_EDITOR);
					updateRecentOpenedFilesMenu();

					styleDocument.setContentModified(false);
					lastKnownSavedState = System.currentTimeMillis();

					// Get current file path for Execute file into Scilab 
					fileFullPath = f.getAbsolutePath();

					getInfoBar().setText("");
				} else {
					getInfoBar().setText("");
				}
			}

			// Empty the undo Manager
			UndoManager undo = ((ScilabStyleDocument) getTextPane().getStyledDocument()).getUndoManager();
			undo.discardAllEdits();

			synchronized (synchro) {
				synchro.notify();
			}
		}
	}

	public EditorKit getEditorKit() {
		return editorKit;
	}

	public void setEditorKit(EditorKit editorKit) {
		this.editorKit = editorKit;
	}
	
	public File getFileToEncode() {
		return fileToEncode;
	}

	public void setFileToEncode(File fileToEncode) {
		this.fileToEncode = fileToEncode;
	}
	
	public static Xpad getEditor() {
	    return editor;
	}

	public static void closeXpadFromScilab() {
	    Xpad xpad = getEditor();
	    if (xpad == null) {
		return;
	    }

	    while (xpad.getTabPane().getComponentCount() > 0) {
	    	//close and save all editors if they are modified
	    	xpad.closeTabAt(0, true);
	    }
	}

}

class TabTitleUpdater implements Runnable {
	Xpad editor;
	TabTitleUpdater( Xpad e ) {
		editor = e;
	}
	 public void run() {
		 editor.updateTabTitle();
	}
}