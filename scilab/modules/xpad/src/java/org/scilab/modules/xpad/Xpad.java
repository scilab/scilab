/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - Calixte DENIZET
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
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
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
import java.util.List;
import java.util.Collections;
import java.util.Vector;
import java.util.Map;
import java.util.HashMap;

import javax.swing.KeyStroke;
import javax.swing.BorderFactory;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;
import javax.swing.SwingUtilities;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.ChangedCharSetException;
import javax.swing.text.EditorKit;
import javax.swing.undo.UndoManager;

import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.filechooser.Juigetfile;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.pushbutton.PushButton;
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
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.xpad.actions.ExitAction;
import org.scilab.modules.xpad.actions.FindAction;
import org.scilab.modules.xpad.actions.GotoLineAction;
import org.scilab.modules.xpad.actions.RecentFileAction;
import org.scilab.modules.xpad.actions.SetColorsAction;
import org.scilab.modules.xpad.actions.TabifyAction;
import org.scilab.modules.xpad.actions.UndoAction;
import org.scilab.modules.xpad.actions.RedoAction;
import org.scilab.modules.xpad.actions.CopyAction;
import org.scilab.modules.xpad.actions.CutAction;
import org.scilab.modules.xpad.actions.PasteAction;
import org.scilab.modules.xpad.actions.SelectAllAction;
import org.scilab.modules.xpad.actions.HighlightCurrentLineAction;
import org.scilab.modules.xpad.actions.UnTabifyAction;
import org.scilab.modules.xpad.actions.FindNextAction;
import org.scilab.modules.xpad.actions.FindPreviousAction;
import org.scilab.modules.xpad.actions.LineBeautifierAction;
import org.scilab.modules.xpad.actions.XpadCompletionAction;
import org.scilab.modules.xpad.style.CompoundUndoManager;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.DropFilesListener;
import org.scilab.modules.xpad.utils.SaveFile;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * Main Xpad class.
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public class Xpad extends SwingScilabTab implements Tab {

    private static final long serialVersionUID = -6410183357490518676L;

    private static final String XPAD = "Editor";
    private static final String SCI_EXTENSION = ".sci";
    private static final String SCE_EXTENSION = ".sce";
    private static final String ALL_SCI_FILES = "*.sci";
    private static final String ALL_SCE_FILES = "*.sce";
    private static final String ALL_SCX_FILES = "*.sc*";
    private static final String ALL_FILES = "*.*";
    private static final int ZERO = 0;
    private static final int ONE = 1;
    private static final int TWO = 2;
    private static final int THREE = 3;

    private static List<Xpad> xpadList = new ArrayList();
    static Xpad editor;

    private static XpadGUI xpadGUI;
    private final Window parentWindow;

    private JTabbedPane tabPane;
    private ScilabEditorPane textPane;
    private JScrollPane scrollingText;
    private Menu recentsMenu;
    private int numberOfUntitled;
    private EditorKit editorKit;
    private long lastKnownSavedState;
    private Object synchro = new Object();
    private FindAction find;

    private PushButton undoButton;
    private PushButton redoButton;

    private boolean highlight;
    private Color highlightColor;
    private Color highlightContourColor;

    private boolean helpOnTyping;

    private int whereami;

    private Vector<Integer> tabList = new Vector<Integer>();
    private Vector<Integer> closedTabList = new Vector<Integer>();

    private String fileFullPath = "";

    private File fileToEncode;

    /**
     * Create Xpad instance inside parent Window
     * @param parentWindow the parent Window
     */
    public Xpad(Window parentWindow) {
        super(XPAD);
        xpadList.add(this);
        this.parentWindow = parentWindow;
        recentsMenu = ScilabMenu.createMenu();
        numberOfUntitled = 0;
        editorKit = new ScilabEditorKit();
        setDefaultHighlight();
        setDefaultHelpOnTyping();
        lastKnownSavedState = 0;

        SwingScilabWindow window = (SwingScilabWindow) parentWindow.getAsSimpleWindow();
        Position pos = ConfigXpadManager.getMainWindowPosition();
        window.setLocation(pos.getX(), pos.getY());
        Size size = ConfigXpadManager.getMainWindowSize();
        window.setSize(size.getWidth(), size.getHeight());

        tabPane = new JTabbedPane();
        tabPane.addChangeListener(new ChangeListener() {
                public void stateChanged(ChangeEvent e) {
                    String path = new String("");
                    if (getTextPane() != null) {
                        if (getTextPane().getName() != null) {
                            path  =  " (" + getTextPane().getName() + ")";
                        }
                        setTitle(tabPane.getTitleAt(tabPane.getSelectedIndex()) + path + " - " + XpadMessages.SCILAB_EDITOR);
                        updateUI();

                        // Update encoding menu
                        xpadGUI.updateEncodingMenu((ScilabDocument) getTextPane().getDocument());

                        // Update End Of Line  menu
                        xpadGUI.updateEolMenu((ScilabDocument) getTextPane().getDocument());
                    }
                }
            });
        /*        tabPane.addFocusListener(new FocusListener() {
                public void focusGained(FocusEvent e) {
                    ScilabEditorPane pane = getTextPane();
                    if (pane != null) {
                        pane.grabFocus();
                    }
                }

                public void focusLost(FocusEvent e) { }
                });*/
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
            SwingUtilities.invokeAndWait(new Thread() {
                    public void run() {
                        Xpad editorInstance = launchXpad();
                        editorInstance.addEmptyTab();
                    }
                });
        } catch (InterruptedException e) {
            System.err.println("EDT interrupted " + e);
        } catch (java.lang.reflect.InvocationTargetException e) {
            System.err.println(" xpad() throw: " + e);
            e.printStackTrace();
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
            ScilabModalDialog.show(editorInstance, String.format(XpadMessages.CANNOT_LOAD_DIRECTORY, f.getAbsolutePath()),
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
        editorInstance.lastKnownSavedState = System.currentTimeMillis();
        editorInstance.getTextPane().scrollTextToLineNumber(lineNumber);
    }

    /**
     * Launch Xpad with a provided text (from the help for example).
     * @param text the text which should be modified
     */
    public static void xpadWithText(String text) {
        Xpad editorInstance = launchXpad();
        editorInstance.lastKnownSavedState = System.currentTimeMillis();
        ScilabEditorPane theTextPane = editorInstance.addEmptyTab();
        ScilabDocument styleDocument = (ScilabDocument) theTextPane.getDocument();
        try {
            editorInstance.getEditorKit().read(new StringReader(text), styleDocument, 0);
        } catch (IOException e) {
            System.err.println("Error while reading the String");
        } catch (BadLocationException e) {
            System.err.println("Error while reading the String");
        }
    }

    /**
     * Clone the current tab and if b is true close the tab
     * @param b a boolean
     */
    public static void cloneAndCloseCurrentTab(boolean b) {
        Xpad current = editor;
        ScilabDocument cdoc = (ScilabDocument) current.getTextPane().getDocument();
        editor = null;
        xpadWithText(cdoc.getText());
        String title = current.getTabPane().getTitleAt(current.getTabPane().getSelectedIndex());
        editor.getTabPane().setTitleAt(0, title);
        editor.getTextPane().setName(current.getTextPane().getName());
        editor.setTitle(current.getTitle());
        ScilabDocument sdoc = (ScilabDocument) editor.getTextPane().getDocument();
        sdoc.setContentModified(cdoc.isContentModified());
        sdoc.getUndoManager().discardAllEdits();
        editor.getTextPane().setCaretPosition(0);
        editor.enableUndoButton(false);
        editor.enableRedoButton(false);

        if (b) {
            current.closeTabAt(current.getTabPane().getSelectedIndex());
            if (current.getTabPane().getTabCount() == 0) {
                current.addEmptyTab();
            }
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
    public void closeXpad() {
        if (find != null) {
            find.closeFindReplaceWindow();
        }
        GotoLineAction.closeGotoLineWindow();
        SetColorsAction.closeSetColorsWindow();
        while (getTabPane().getComponentCount() > 0) {
            closeTabAt(0, true);
        }
        editor = null;
        SwingScilabWindow window = (SwingScilabWindow) parentWindow.getAsSimpleWindow();
        Point p = window.getLocation();
        ConfigXpadManager.saveMainWindowPosition(new Position(p.x, p.y));
        Dimension d = window.getSize();
        ConfigXpadManager.saveMainWindowSize(new Size(d.width, d.height));
    }

    /**
     * Create Xpad instance.
     * @return the instance
     */
    private static Xpad createEditor() {
        ConfigXpadManager.createUserCopy();
        Window mainWindow = ScilabWindow.createWindow();

        final Xpad editorInstance = new Xpad(mainWindow);

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
                    if (editorInstance.getTabPane().getTabCount() != 1) {
                        if (ScilabModalDialog.show(editorInstance, XpadMessages.EXIT_CONFIRM, XpadMessages.EXIT,
                                                   IconType.WARNING_ICON, ButtonType.YES_NO) == AnswerOption.YES_OPTION) {
                            ExitAction.doExit(editorInstance);
                        }
                    } else {
                        ExitAction.doExit(editorInstance);
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
        ScilabEditorPane textPaneAt = getTextPane(indexTab);

        /* Test for modification added after bug 5103 fix: do not ask the user for an Untitled not-modified file saving when closing Xpad */
        if (((ScilabDocument) textPaneAt.getDocument()).isContentModified()) {
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
        if ((indexTab == 0) && (getTabPane().getTabCount() == 1)) {
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

    public boolean save(int indexTab, boolean force, boolean scilabClose) {
        ScilabEditorPane textPaneAt = getTextPane(indexTab);
        //if the file ( empty, new or loaded ) is not modified, exit save process and return true
        if (!((ScilabDocument) textPaneAt.getDocument()).isContentModified()
            && (textPaneAt.getName() != null)) { /* Bug 5103 fix */
            return true;
        }

        if (!force) {
            AnswerOption answer;
            if (scilabClose) {
                answer = ScilabModalDialog.show(this, getTabPane().getTitleAt(indexTab) + XpadMessages.MODIFIED,
                                                XpadMessages.SCILAB_EDITOR, IconType.QUESTION_ICON, ButtonType.YES_NO);
            } else {
                answer = ScilabModalDialog.show(this, getTabPane().getTitleAt(indexTab) + XpadMessages.MODIFIED,
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

        if (!SaveFile.doSave(textPaneAt, newSavedFile, editorKit)) {
            return false;
        }

        ScilabDocument styledDocument = (ScilabDocument) textPaneAt.getDocument();
        styledDocument.setContentModified(false);

        getTabPane().setTitleAt(getTabPane().getSelectedIndex() , newSavedFile.getName());
        setTitle(newSavedFile.getPath() + " - " + XpadMessages.SCILAB_EDITOR);

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
            if (ScilabModalDialog.show(this, String.format(XpadMessages.EXTERNAL_MODIFICATION, newSavedFiled.getPath()),
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

        SciFileFilter sceFilter = new SciFileFilter(ALL_SCE_FILES , null , ZERO);
        SciFileFilter sciFilter = new SciFileFilter(ALL_SCI_FILES , null , ONE);
        SciFileFilter scxFilter = new SciFileFilter(ALL_SCX_FILES , null , TWO);
        SciFileFilter allFilter = new SciFileFilter(ALL_FILES , null , THREE);

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
        
        String name = ((ScilabDocument) getTextPane().getDocument()).getFirstFunctionName();
        if (name != null) {
            fileChooser.setSelectedFile(new File(name + ".sci"));
        } 
        
        int retval = fileChooser.showSaveDialog(this);

        if (retval == JFileChooser.APPROVE_OPTION) {
            File f = fileChooser.getSelectedFile();
            initialDirectoryPath = f.getPath();
            if (f.exists()) {
                if (ScilabModalDialog.show(this, XpadMessages.REPLACE_FILE_TITLE,
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
            if (fileName.lastIndexOf(".") != -1) {
                if (fileName.substring(fileName.lastIndexOf("."), fileName.length()).length() >= 2
                    && fileName.substring(fileName.lastIndexOf("."), fileName.length()).length() <= 4) {
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
    public boolean saveAs(ScilabEditorPane textPane) {
        boolean isSuccess = false;
        String extension = new String();

        String initialDirectoryPath = getTextPane().getName();
        if (initialDirectoryPath == null) {
            initialDirectoryPath =  ConfigManager.getLastOpenedDirectory();
        }

        //prefer to use chooseFileToSave function !
        SciFileFilter sceFilter = new SciFileFilter(ALL_SCE_FILES , null , 0);
        SciFileFilter sciFilter = new SciFileFilter(ALL_SCI_FILES , null , 1);
        SciFileFilter scxFilter = new SciFileFilter(ALL_SCX_FILES , null , 2);
        SciFileFilter allFilter = new SciFileFilter(ALL_FILES , null , 3);


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

        String name = ((ScilabDocument) textPane.getDocument()).getFirstFunctionName();
        if (name != null) {
            fileChooser.setSelectedFile(new File(name + ".sci"));
        } else if (textPane.getName() != null) { /* Bug 5319 */
            fileChooser.setSelectedFile(new File(textPane.getName()));
        }

        int retval = fileChooser.showSaveDialog(this);

        if (retval == JFileChooser.APPROVE_OPTION) {
            File f = fileChooser.getSelectedFile();
            initialDirectoryPath = f.getPath();
            if (f.exists()) {
                AnswerOption ans = ScilabModalDialog.show(this, XpadMessages.REPLACE_FILE_TITLE,
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

            ScilabDocument styledDocument = (ScilabDocument) textPane.getDocument();

            if (!SaveFile.doSave(textPane, f, editorKit)) {
                return false;
            }

            ConfigManager.saveLastOpenedDirectory(f.getPath());
            ConfigXpadManager.saveToRecentOpenedFiles(f.getPath());
            textPane.setName(f.getPath());
            getTabPane().setTitleAt(getTabPane().getSelectedIndex() , f.getName());
            setTitle(f.getPath() + " - " + XpadMessages.SCILAB_EDITOR);
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
    public ScilabEditorPane addTab(String title) {
        ScilabEditorPane textPane = new ScilabEditorPane(this);
        initPane(textPane);
        tabPane.add(title, textPane.getParentComponent());
        tabPane.setSelectedIndex(tabPane.getTabCount() - 1);
        setContentPane(tabPane);
        initInputMap(textPane);
        updateTabTitle();
        return textPane;
    }

    /**
     * Init the EditorPane.
     * @param pane the EditorPane
     */
    public void initPane(ScilabEditorPane pane) {
        setHighlight(pane);
        pane.setEditorKit(new ScilabEditorKit());

        // Panel of line number for the text pane
        pane.getXln().setWhereamiLineNumbering(ConfigXpadManager.getLineNumberingState());
        setHelpOnTyping(pane);

        pane.setBorder(BorderFactory.createEmptyBorder(2, 2, 2, 2));
        pane.setFont(ConfigXpadManager.getFont());

        pane.setBackground(ConfigXpadManager.getXpadBackgroundColor());
        pane.setCaretColor(ConfigXpadManager.getXpadForegroundColor());

        pane.setFocusable(true);
        pane.setRequestFocusEnabled(true);
        pane.requestFocus();
        pane.grabFocus();
        pane.setDragEnabled(true); /* Bug 5497 */

        DropFilesListener dndTarget = new DropFilesListener(pane);

        XpadGUI.createPopupMenu(pane);
    }

    /**
     * Init the EditorPane with the InputMap found in ConfigXpadManager
     * @param pane the EditorPane
     */
    public void initInputMap(ScilabEditorPane pane) {
        Map<String, KeyStroke> map = new HashMap();
        ConfigXpadManager.addMapActionNameKeys(map);
        TabifyAction.putInInputMap(pane, this, map.get("TabifyAction"));
        UndoAction.putInInputMap(pane, this, map.get("UndoAction"));
        RedoAction.putInInputMap(pane, this, map.get("RedoAction"));
        PasteAction.putInInputMap(pane, this, map.get("PasteAction"));
        CopyAction.putInInputMap(pane, this, map.get("CopyAction"));
        CutAction.putInInputMap(pane, this, map.get("CutAction"));
        UnTabifyAction.putInInputMap(pane, this, map.get("UnTabifyAction"));
        FindNextAction.putInInputMap(pane, this, map.get("FindNextAction"));
        FindPreviousAction.putInInputMap(pane, this, map.get("FindPreviousAction"));
        HighlightCurrentLineAction.putInInputMap(pane, this, map.get("HighlightCurrentLineAction"));
        SelectAllAction.putInInputMap(pane, this, map.get("SelectAllAction"));
        LineBeautifierAction.putInInputMap(pane);
        XpadCompletionAction.putInInputMap(pane, this, map.get("XpadCompletionAction"));
    }

    /**
     * Create an empty tab inside Xpad.
     * @return the text component inside the tab
     */
    public ScilabEditorPane addEmptyTab() {
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
     * Add or remove '*' prefix in current tab tile according to isContentModified().
     */
    public void updateTabTitle() {
        StringBuffer newTitle = new StringBuffer();
        ScilabEditorPane currentTextPane = getTextPane();
        if (((ScilabDocument) currentTextPane.getDocument()).isContentModified()) {
            newTitle.append('*');
        }

        String textPaneName = currentTextPane.getName();
        try {
            File f = new File(textPaneName);
            newTitle.append(f.getName());
        } catch (Exception e) { // not a file name, no path prefix to remove, but maybe a '*'
            textPaneName = getTabPane().getTitleAt(getTabPane().getSelectedIndex());
            newTitle.append(textPaneName.charAt(0) == '*' ? textPaneName.substring(1, textPaneName.length()) : textPaneName);
        }
        getTabPane().setTitleAt(getTabPane().getSelectedIndex() , newTitle.toString());
    }

    /**
     * @param button the UndoButton used in this editor
     */
    public void setUndoButton(PushButton button) {
        undoButton = button;
        enableUndoButton(false);
    }

    /**
     * @param b true to enable the button
     */
    public void enableUndoButton(boolean b) {
        if (undoButton != null) {
            undoButton.setEnabled(b);
        }
    }

    /**
     * Undo last modification.
     */
    public void undo() {
        ScilabDocument doc = (ScilabDocument) getTextPane().getDocument();
        synchronized (doc) {
            doc.getUndoManager().undo();
        }
    }

    /**
     * @param button the RedoButton used in this editor
     */
    public void setRedoButton(PushButton button) {
        redoButton = button;
        enableRedoButton(false);
    }

    /**
     * @param b true to enable the button
     */
    public void enableRedoButton(boolean b) {
        if (redoButton != null) {
            redoButton.setEnabled(b);
        }
    }

    /**
     * Redo last modification.
     */
    public void redo() {
        ScilabDocument doc = (ScilabDocument) getTextPane().getDocument();
        synchronized (doc) {
            doc.getUndoManager().redo();
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
            ScilabEditorPane textPaneAt = getTextPane(i);
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
            ScilabEditorPane textPaneAt = getTextPane(i);
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
     * Get current text component. If the window is splitted, then
     * return the focused text component.
     * @return the text component
     */
    public ScilabEditorPane getTextPane() {
        try {
            Component c = tabPane.getSelectedComponent();
            if (c instanceof JScrollPane) {
                return (ScilabEditorPane) ((JScrollPane) c).getViewport().getComponent(0);
            } else if (c instanceof JSplitPane) {
                ScilabEditorPane sep = (ScilabEditorPane) ((JScrollPane) ((JSplitPane) c).getLeftComponent()).getViewport().getComponent(0);
                if (sep == ScilabEditorPane.getFocusedPane()) {
                    return sep;
                } else {
                    return (ScilabEditorPane) ((JScrollPane) ((JSplitPane) c).getRightComponent()).getViewport().getComponent(0);
                }
            }
            return null;
        } catch (NullPointerException e) {
            return null;
        } catch (ArrayIndexOutOfBoundsException e) {
            return null;
        }
    }

    /**
     * Get text component at index.
     * @param index the index of the textpane
     * @return the text component
     */
    public ScilabEditorPane getTextPane(int index) {
        try {
            Component c = tabPane.getComponentAt(index);
            if (c instanceof JScrollPane) {
                return (ScilabEditorPane) ((JScrollPane) c).getViewport().getComponent(0);
            } else if (c instanceof JSplitPane) {
                ScilabEditorPane sep = (ScilabEditorPane) ((JScrollPane) ((JSplitPane) c).getLeftComponent()).getViewport().getComponent(0);
                if (sep == ScilabEditorPane.getFocusedPane()) {
                    return sep;
                } else {
                    return (ScilabEditorPane) ((JScrollPane) ((JSplitPane) c).getRightComponent()).getViewport().getComponent(0);
                }
            }
            return null;
        } catch (NullPointerException e) {
            return null;
        } catch (ArrayIndexOutOfBoundsException e) {
            return null;
        }
    }

    /**
     * Enable the whereami-line numbering
     * @param state int
     */
    public void setWhereamiLineNumbering(int state) {
        int n = tabPane.getTabCount();
        for (int i = 0; i < n; i++) {
            ScilabEditorPane sep = (ScilabEditorPane) getTextPane(i);
            sep.getXln().setWhereamiLineNumbering(state);
            if (sep.getRightTextPane() != null) {
                sep.getRightTextPane().getXln().setWhereamiLineNumbering(state);
            }
        }
        repaint();
        whereami = state;
    }

    /**
     * Auto-indent mode management
     * @param b true to activate auto-indent mode
     */
    public void setAutoIndent(boolean b) {
        int n = tabPane.getTabCount();
        for (int i = 0; i < n; i++) {
            ScilabEditorPane sep = (ScilabEditorPane) getTextPane(i);
            ((ScilabDocument) sep.getDocument()).setAutoIndent(b);
            if (sep.getRightTextPane() != null) {
                ((ScilabDocument) sep.getRightTextPane().getDocument()).setAutoIndent(b);
            }
        }
    }

    /**
     * Enable the highlighted line in this editor
     * @param b boolean
     */
    public void enableHighlightedLine(boolean b) {
        int n = tabPane.getTabCount();
        for (int i = 0; i < n; i++) {
            ScilabEditorPane sep = (ScilabEditorPane) getTextPane(i);
            sep.enableHighlightedLine(b);
            if (sep.getRightTextPane() != null) {
                sep.getRightTextPane().enableHighlightedLine(b);
            }
        }
        highlight = b;
    }

    /**
     * Enable the help on typing in the current textPane
     * @param b boolean
     */
    public void setHelpOnTyping(ScilabEditorPane pane) {
        pane.activateHelpOnTyping(helpOnTyping);
    }

    /**
     * Enable the help on typing in this editor
     * @param b boolean
     */
    public void setHelpOnTyping(boolean b) {
        int n = tabPane.getTabCount();
        for (int i = 0; i < n; i++) {
            ScilabEditorPane sep = (ScilabEditorPane) getTextPane(i);
            sep.activateHelpOnTyping(b);
            if (sep.getRightTextPane() != null) {
                sep.getRightTextPane().activateHelpOnTyping(b);
            }
        }
        helpOnTyping = b;
    }

    /**
     * Enable the help on typing in this editor according to xpadConfiguration.xml
     */
    public void setDefaultHelpOnTyping() {
        helpOnTyping = ConfigXpadManager.getHelpOnTypingState();
    }

    /**
     * Set the color of the highlighted line in this editor
     * @param c Color
     */
    public void setHighlightedLineColor(Color c) {
        int n = tabPane.getTabCount();
        for (int i = 0; i < n; i++) {
            ScilabEditorPane sep = (ScilabEditorPane) getTextPane(i);
            sep.setHighlightedLineColor(c);
            if (sep.getRightTextPane() != null) {
                sep.getRightTextPane().setHighlightedLineColor(c);
            }
        }
        highlightColor = c;
    }

    /**
     * Set the color of the contour of the highlighted line in this editor
     * @param c Color
     */
    public void setHighlightedContourColor(Color c) {
        int n = tabPane.getTabCount();
        for (int i = 0; i < n; i++) {
            ScilabEditorPane sep = (ScilabEditorPane) getTextPane(i);
            sep.setHighlightedContourColor(c);
            if (sep.getRightTextPane() != null) {
                sep.getRightTextPane().setHighlightedContourColor(c);
            }
        }
        highlightContourColor = c;
    }

    /**
     * Set the defaults for the highlighted line in this editor
     * Defaults read in xpadConfiguration.xml
     */
    public void setDefaultHighlight() {
        this.highlight = ConfigXpadManager.getHighlightState();
        Color[] arr = ConfigXpadManager.getHighlightColors();
        this.highlightColor = arr[0];
        this.highlightContourColor = arr[1];
    }

    /**
     * Set the highlighted line in this textPane
     * @param sep ScilabEditorPane
     */
    public void setHighlight(ScilabEditorPane sep) {
        sep.enableHighlightedLine(highlight);
        sep.setHighlightedLineColor(highlightColor);
        sep.setHighlightedContourColor(highlightContourColor);
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
     * Get recent file menu.
     * @return the menu
     */
    public Menu getRecentsMenu() {
        return this.recentsMenu;
    }

    /**
     * Update menu displaying recent opened files.
     */
    public void updateRecentOpenedFilesMenu() {
        List<File> recentFiles = ConfigXpadManager.getAllRecentOpenedFiles();

        ((SwingScilabMenu) getRecentsMenu().getAsSimpleMenu()).removeAll();
        for (int i = 0; i < recentFiles.size(); i++) {
            getRecentsMenu().add(RecentFileAction.createMenu(this, recentFiles.get(i)));
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

        /**
         * ReadFileThread
         * @param f File
         */
        public ReadFileThread(File f) {
            this.fileToRead = f;
            setFileToEncode(f);
        }

        @SuppressWarnings("deprecation")
        /**
         * Function Run
         */
            public void run() {
            readFile(fileToRead);
            this.stop();
        }

        /**
         * readFile
         * @param f File
         */
        public void readFile(File f) {
            getInfoBar().setText(XpadMessages.LOADING);
            // Get current file path for Execute file into Scilab
            fileFullPath = f.getAbsolutePath();

            ScilabDocument styleDocument = null;
            ScilabEditorPane theTextPane;

            // File exist
            if (f.exists()) {
                theTextPane = addTab(f.getName());
                styleDocument = (ScilabDocument) theTextPane.getDocument();
                styleDocument.disableUndoManager();

                try {
                    synchronized (styleDocument) {
                        styleDocument.setUpdater(false);
                        boolean indentMode = styleDocument.getAutoIndent();
                        styleDocument.setAutoIndent(false);
                        try {
                            try {
                                editorKit.read(new BufferedReader(new InputStreamReader(new FileInputStream(f), styleDocument.getEncoding())), styleDocument, 0);
                            } catch (ChangedCharSetException e) {
                                editorKit.read(new BufferedReader(new InputStreamReader(new FileInputStream(f), e.getCharSetSpec())), styleDocument, 0);
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
                getTabPane().setTitleAt(getTabPane().getSelectedIndex() , f.getName());
                styleDocument.setContentModified(false);
                styleDocument.enableUndoManager();

                getInfoBar().setText("");

                xpadGUI.updateEncodingMenu((ScilabDocument) getTextPane().getDocument());

                // File does not exist
            } else {
                theTextPane = addEmptyTab();
                int choice = JOptionPane.showConfirmDialog(Xpad.this,
                                                           String.format(XpadMessages.FILE_DOESNT_EXIST, f.getName()),
                                                           "Editor",
                                                           JOptionPane.YES_NO_OPTION);
                if (choice == 0) { //OK
                    styleDocument = (ScilabDocument) theTextPane.getDocument();
                    styleDocument.disableUndoManager();

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
                    getTabPane().setTitleAt(getTabPane().getSelectedIndex() , f.getName());
                    setTitle(f.getPath() + " - " + XpadMessages.SCILAB_EDITOR);
                    updateRecentOpenedFilesMenu();

                    styleDocument.setContentModified(false);
                    styleDocument.enableUndoManager();

                    lastKnownSavedState = System.currentTimeMillis();

                    // Get current file path for Execute file into Scilab
                    fileFullPath = f.getAbsolutePath();

                    getInfoBar().setText("");
                } else {
                    getInfoBar().setText("");
                }
            }

            // Empty the undo Manager
            UndoManager undo = ((ScilabDocument) getTextPane().getDocument()).getUndoManager();
            undo.discardAllEdits();

            synchronized (synchro) {
                synchro.notify();
            }
        }
    }

    /**
     * EditorKit Getter
     * @return EditorKit
     */
    public EditorKit getEditorKit() {
        return editorKit;
    }

    /**
     * EditorKit Setter
     * @param editorKit EditorKit
     */
    public void setEditorKit(EditorKit editorKit) {
        this.editorKit = editorKit;
    }

    /**
     * Getter for file to encode
     * @return a File
     */
    public File getFileToEncode() {
        return fileToEncode;
    }

    /**
     * Setter for file to encode
     * @param fileToEncode a File
     */
    public void setFileToEncode(File fileToEncode) {
        this.fileToEncode = fileToEncode;
    }

    /**
     * Xpad Getter
     * @return editor Xpad
     */
    public void addFindActionWindow(FindAction find) {
        this.find = find;
    }

    /**
     * Close xpad from scilab
     */
    public static void closeXpadFromScilab() {
        for (int i = 0; i < xpadList.size(); i++) {
            Xpad ed = xpadList.get(i);
            ed.closeXpad();
        }
        xpadList.clear();
    }
}
