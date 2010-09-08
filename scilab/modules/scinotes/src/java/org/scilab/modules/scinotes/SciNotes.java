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

package org.scilab.modules.scinotes;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Image;
import java.awt.Point;
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
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Iterator;
import java.util.UUID;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.ChangedCharSetException;
import javax.swing.text.EditorKit;
import javax.swing.undo.UndoManager;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.filechooser.Juigetfile;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBox;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.SciFileFilter;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.scinotes.actions.ExitAction;
import org.scilab.modules.scinotes.actions.FindAction;
import org.scilab.modules.scinotes.actions.SetColorsAction;
import org.scilab.modules.scinotes.actions.LineBeautifierAction;
import org.scilab.modules.scinotes.actions.OpenSourceFileOnKeywordAction;
import org.scilab.modules.scinotes.actions.RecentFileAction;
import org.scilab.modules.scinotes.actions.SciNotesCompletionAction;
import org.scilab.modules.scinotes.actions.EncodingAction;
import org.scilab.modules.scinotes.actions.EndOfLineAction;
import org.scilab.modules.scinotes.actions.RestoreOpenedFilesAction;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.DropFilesListener;
import org.scilab.modules.scinotes.utils.NavigatorWindow;
import org.scilab.modules.scinotes.utils.SaveFile;
import org.scilab.modules.scinotes.utils.ScilabTabbedPane;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Main SciNotes class.
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public class SciNotes extends SwingScilabTab implements Tab {

    private static final long serialVersionUID = -6410183357490518676L;

    private static final String SCINOTES = "SciNotes";
    private static final String SCI_EXTENSION = ".sci";
    private static final String SCE_EXTENSION = ".sce";
    private static final String TST_EXTENSION = ".tst";
    private static final String QUIT_EXTENSION = ".quit";
    private static final String DEM_EXTENSION = ".dem";
    private static final String START_EXTENSION = ".start";
    private static final String ALL_TST_FILES = "*.tst";
    private static final String ALL_QUIT_FILES = "*.quit";
    private static final String ALL_START_FILES = "*.start";
    private static final String ALL_SCI_FILES = "*.sci";
    private static final String ALL_SCE_FILES = "*.sce";
    private static final String ALL_DEM_FILES = "*.dem";
    private static final String ALL_SCX_FILES = "*.sc*";
    private static final String ALL_SCILAB = "all";
    private static final String ALL_FILES = "*.*";
    private static final String DOT = ".";

    private static final String DEFAULTACTIONPATH = "org.scilab.modules.scinotes.actions";

    private static final int ZERO = 0;
    private static final int ONE = 1;
    private static final int TWO = 2;
    private static final int THREE = 3;
    private static final int FOUR = 4;

    private static List<SciNotes> scinotesList = new ArrayList();
    private static SciNotes editor;

    private final Window parentWindow;
    private UUID uuid;

    private JTabbedPane tabPane;
    private int numberOfUntitled;
    private EditorKit editorKit;

    private PushButton undoButton;
    private PushButton redoButton;

    private Color highlightColor;
    private Color highlightContourColor;

    private boolean protectOpenFileList;

    private List<Integer> tabList = new ArrayList();
    private List<Integer> closedTabList = new ArrayList();

    private String fileFullPath = "";

    /**
     * Create SciNotes instance inside parent Window
     * @param parentWindow the parent Window
     */
    public SciNotes(Window parentWindow) {
        super(SCINOTES);
	setWindowIcon(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/32x32/apps/accessories-text-editor.png").getImage());
        scinotesList.add(this);
        this.parentWindow = parentWindow;
        this.uuid = UUID.randomUUID();
        numberOfUntitled = 0;
        editorKit = new ScilabEditorKit();
        SwingScilabWindow window = (SwingScilabWindow) parentWindow.getAsSimpleWindow();
        Position pos = ConfigSciNotesManager.getMainWindowPosition();
        window.setLocation(pos.getX(), pos.getY());
        Size size = ConfigSciNotesManager.getMainWindowSize();
        window.setSize(size.getWidth(), size.getHeight());
        protectOpenFileList = false;
        tabPane = new ScilabTabbedPane(this);
        tabPane.addChangeListener(new ChangeListener() {
                public void stateChanged(ChangeEvent e) {
                    String path = new String("");
                    if (getTextPane() != null) {
                        updateUI();
                        getTextPane().updateInfosWhenFocused();

                        // Update encoding menu
                        EncodingAction.updateEncodingMenu((ScilabDocument) getTextPane().getDocument());

                        // Update End Of Line  menu
                        EndOfLineAction.updateEolMenu((ScilabDocument) getTextPane().getDocument());
                        setTitle(getTextPane().getTitle());
                    }
                }
            });
        this.setContentPane(tabPane);
    }

    /**
     * {@inheritDoc}
     */
    public void setTitle(String title) {
        super.setTitle(title);
        final SwingScilabWindow window = (SwingScilabWindow) SwingUtilities
            .getAncestorOfClass(SwingScilabWindow.class, tabPane);
        if (window != null) {
            window.setTitle(title);
        }
    }

    /**
     * Launch SciNotes from command line.
     * @param args command line args
     */
    public static void main(String[] args) {
        scinotes();
    }

    /**
     * Launch SciNotes with an empty file
     *
     * This method *must not* be called on the EDT thread.
     */
    public static void scinotes() {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {

                    public void run() {
                        SciNotes editorInstance = launchSciNotes();
                        // Open an empty file if no tabs were opened at launch.
                        if (editorInstance.getTabPane().getTabCount() == 0)  {
                            editorInstance.openFile(null, 0, null);
                        }
                    }
                });
        } catch (InterruptedException e) {
            LogFactory.getLog(SciNotes.class).error(e);
            throw new RuntimeException(e);
        } catch (InvocationTargetException e) {
            LogFactory.getLog(SciNotes.class).error(e);
            e.printStackTrace();
            throw new RuntimeException(e);
        }
    }

    /**
     * Launch SciNotes with a file name to open.
     * @param filePath the name of the file to open
     *
     * This method *must not* be called on the EDT thread.
     */
    public static void scinotes(final String filePath) {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {

                    public void run() {
                        launchSciNotes().openFile(filePath, 0, null);
                    }
                });
        } catch (InterruptedException e) {
            LogFactory.getLog(SciNotes.class).error(e);
            throw new RuntimeException(e);
        } catch (InvocationTargetException e) {
            LogFactory.getLog(SciNotes.class).error(e);
            throw new RuntimeException(e);
        }
    }

    /**
     * Launch SciNotes with a file name to open and a line to highlight.
     * @param filePath the name of the file to open
     * @param lineNumber the line to highlight
     *
     * This method *must not* be called on the EDT thread.
     */
    public static void scinotes(final String filePath, final int lineNumber) {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {

                    public void run() {
                        launchSciNotes().openFile(filePath, lineNumber, null);
                    }
                });
        } catch (InterruptedException e) {
            LogFactory.getLog(SciNotes.class).error(e);
            throw new RuntimeException(e);
        } catch (InvocationTargetException e) {
            LogFactory.getLog(SciNotes.class).error(e);
            throw new RuntimeException(e);
        }
    }

    /**
     * Launch SciNotes with a file name to open and a line to highlight.
     * @param filePath the name of the file to open
     * @param option such as 'readonly'
     *
     * This method *must not* be called on the EDT thread.
     */
    public static void scinotes(final String filePath, final String option) {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {

                    public void run() {
                        launchSciNotes().openFile(filePath, 0, option);
                    }
                });
        } catch (InterruptedException e) {
            LogFactory.getLog(SciNotes.class).error(e);
            throw new RuntimeException(e);
        } catch (InvocationTargetException e) {
            LogFactory.getLog(SciNotes.class).error(e);
            throw new RuntimeException(e);
        }
    }

    /**
     * Launch SciNotes with a provided text (from the help for example).
     * @param text the text which should be modified
     */
    public static void scinotesWithText(String text) {
        SciNotes editorInstance = launchSciNotes();
        ScilabEditorPane theTextPane = editorInstance.addEmptyTab();
        ScilabDocument styleDocument = (ScilabDocument) theTextPane.getDocument();
        try {
            editorInstance.getEditorKit().read(new StringReader(text), styleDocument, 0);
        } catch (IOException e) {
            System.err.println(SciNotesMessages.ERROR_WITH_STRING);
        } catch (BadLocationException e) {
            System.err.println(SciNotesMessages.ERROR_WITH_STRING);
        }
    }

    /**
     * Clone the current tab and if b is true close the tab
     * @param ed the editor where the tab is
     * @param b a boolean
     */
    public static void cloneAndCloseCurrentTab(SciNotes ed, boolean b) {
        ScilabDocument cdoc = (ScilabDocument) ed.getTextPane().getDocument();
        String title = ed.getTabPane().getTitleAt(ed.getTabPane().getSelectedIndex());
        ScilabEditorPane currentSep = ed.getTextPane();
        String winTitle = ed.getTitle();

        if (b) {
            ed.closeTabAt(ed.getTabPane().getSelectedIndex());
            if (ed.getTabPane().getTabCount() == 0) {
                ed.addEmptyTab();
            }
        }

        editor = null;
        scinotesWithText(cdoc.getText());
        editor.getTabPane().setTitleAt(0, title);
        currentSep.copyProps(editor.getTextPane());
        editor.setTitle(winTitle);
        ScilabDocument sdoc = (ScilabDocument) editor.getTextPane().getDocument();
        sdoc.setContentModified(cdoc.isContentModified());
        sdoc.getUndoManager().discardAllEdits();
        editor.getTextPane().setCaretPosition(0);
        editor.enableUndoButton(false);
        editor.enableRedoButton(false);
        ConfigSciNotesManager.saveToOpenFiles(editor.getTextPane().getName(), editor, editor.getTextPane());
    }

    /**
     * Create SciNotes instance
     * @return the instance
     */
    public static SciNotes launchSciNotes() {
        if (editor == null) {
            editor = createEditor();

            // If this is the first launch, restore previous session if desired by user.
            if (editor.isOnlyInstance()) {
                /* Check the config flag and check that there are files to open. */
                if (!ConfigSciNotesManager.getRestoreOpenedFiles() || ConfigSciNotesManager.countExistingOpenFiles() ==  0)  {
                    ConfigSciNotesManager.removeAllOpenFiles();
                    return editor; // Exit without restoring files
                }

                RestoreOpenedFilesAction.displayDialog((JFrame) editor.parentWindow.getAsSimpleWindow());
                List<List<File>> list = RestoreOpenedFilesAction.getSelectedFiles();

                if (list != null) {
                    // Restore previously opened files and create additional editor instances.
                    int i;
                    boolean mkeditor;

                    for (i = 0, mkeditor = false; i < list.size(); i++) {
                        // Retrieve the list of files associated with the i-th editor instance.
                        List<File> filesToOpen = list.get(i);

                        if (filesToOpen.size() > 0) {
                            // launch additional editors
                            if (mkeditor)  {
                                editor = createEditor();
                            }
                            // Open files
                            for (int j = 0; j < filesToOpen.size(); j++) {
                                editor.openFile(filesToOpen.get(j).getPath(), 0, null);
                            }
                            mkeditor = true; // Next set of files will be opened in a new editor
                        }
                    }
                }
            }
        }

        return editor;
    }

    /**
     * Open a file.
     *
     * This method *must* be called on the EDT thread.
     *
     * @param filePath the file path (may be null)
     * @param lineNumber the line number (use 0 for unspecified)
     * @param option the options (may be null)
     */
    public void openFile(String filePath, int lineNumber, String option) {
        if (!SwingUtilities.isEventDispatchThread()) {
            throw new RuntimeException("called outside the EDT thread.");
        }

        if (filePath == null) {
            addEmptyTab();
            ConfigSciNotesManager.saveToOpenFiles(getTextPane().getName(), this, getTextPane());
            return;
        }

        File f = fileToCanonicalFile(new File(filePath));
        if (f.isDirectory()) { /* Bug 5131 */
            ConfigManager.saveLastOpenedDirectory(f.getPath());
            addEmptyTab();
            ConfigSciNotesManager.saveToOpenFiles(getTextPane().getName(), this, getTextPane());
            return;
        }

        readFileAndWait(f);
        getTextPane().scrollTextToLineNumber(lineNumber, true);
        if (option != null && "readonly".equals(option.toLowerCase())) {
            getTextPane().setReadOnly(true);
            getInfoBar().setText(getTextPane().getInfoBarText());
            ConfigSciNotesManager.removeFromOpenFiles(this, getTextPane());
        } else {
            ConfigSciNotesManager.saveToRecentOpenedFiles(filePath);
            RecentFileAction.updateRecentOpenedFilesMenu(this);
        }
    }

    /**
     * Close SciNotes instance including all tabs.
     */
    public void closeSciNotes() {
        FindAction.close();
        SetColorsAction.closeSetColorsWindow();
        OpenSourceFileOnKeywordAction.closeOpenSourceWindow();

        while (getTabPane().getTabCount() > 0) {
            closeTabAt(0, true);
        }
        scinotesList.remove(this);
        if (scinotesList.size() == 0) {
            NavigatorWindow.closeCurrent();
        }
        editor = null;
        SwingScilabWindow window = (SwingScilabWindow) parentWindow.getAsSimpleWindow();
        Point p = window.getLocation();
        ConfigSciNotesManager.saveMainWindowPosition(new Position(p.x, p.y));
        Dimension d = window.getSize();
        ConfigSciNotesManager.saveMainWindowSize(new Size(d.width, d.height));
        ConfigSciNotesManager.resetDocument();
    }

    /**
     * Create SciNotes instance.
     * @return the instance
     */
    private static SciNotes createEditor() {
        ConfigSciNotesManager.createUserCopy();
        Window mainWindow = ScilabWindow.createWindow();

        final SciNotes editorInstance = new SciNotes(mainWindow);

        SciNotesGUI.init(mainWindow, editorInstance, SCINOTES);
        editorInstance.setCallback(new CallBack(SciNotesMessages.DEFAULT + SciNotesMessages.DOTS) {
                /**
                 * serialVersionUID
                 */
                private static final long serialVersionUID = -4121140054209319523L;

                /**
                 * Action callback on Exit menu
                 */
                public void callBack() {
                    SwingUtilities.invokeLater(new Runnable() {
                            public void run() {
                                if (editorInstance.getTabPane().getTabCount() != 1) {
                                    if (ScilabModalDialog.show(editorInstance,
                                                               SciNotesMessages.EXIT_CONFIRM, SciNotesMessages.EXIT,
                                                               IconType.WARNING_ICON, ButtonType.YES_NO) == AnswerOption.YES_OPTION) {
                                        ExitAction.doExit(editorInstance);
                                    }
                                } else {
                                    ExitAction.doExit(editorInstance);
                                }
                            }
                        });
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

        /* Test for modification added after bug 5103 fix: do not ask the user for an Untitled not-modified file saving when closing SciNotes */
        if (((ScilabDocument) textPaneAt.getDocument()).isContentModified()) {
            if (!save(indexTab, false, scilabClose)) {
                return false;
            }
        }

        if (textPaneAt.getName() == null) {
            String closedTabName = tabPane.getTitleAt(indexTab);
            String closedTabNameIndex = closedTabName.substring(closedTabName.length() - 1, closedTabName.length());
            tabList.remove(Integer.valueOf(closedTabNameIndex));
            closedTabList.add(Integer.valueOf(closedTabNameIndex));
        }

        // correction for bug 5404, closing the last tabPane generate an exception
        // that's why we need to remove ChangeListeners before closing
        if ((indexTab == 0) && (getTabPane().getTabCount() == 1)) {
            for (int i = 0; i < tabPane.getChangeListeners().length; i++) {
                tabPane.removeChangeListener(tabPane.getChangeListeners()[i]);
            }
        }

        // Remove the filename associated with the tab from the list of open files.
        if (!protectOpenFileList) {
            ConfigSciNotesManager.removeFromOpenFiles(this, textPaneAt);
        }

        NavigatorWindow.removePane(textPaneAt);

        textPaneAt.close();
        tabPane.remove(indexTab);

        textPaneAt = getTextPane();
        if (textPaneAt != null) {
            NavigatorWindow.updateNavigator((ScilabDocument) textPaneAt.getDocument());
        }
        return true;
    }

    /**
     * Set or unset protection of the currently open file list.
     * Call before closing tabs if the tabs should be restored when scinotes restarts.
     * @param protect Enables protection of the open file list if true.
     */
    public void setProtectOpenFileList(boolean protect) {
        protectOpenFileList = protect;
    }

    /**
     * Determines if this editor is the only editor instance.
     * @return true if this is the last editor instance.
     */
    public boolean isOnlyInstance() {
        if (scinotesList.size() > 1) {
            return false;
        }

        return true;
    }

    /**
     * @return the list of the different openened editors
     */
    public static List<SciNotes> getSciNotesList() {
        return scinotesList;
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
        if (!textPaneAt.checkExternalModif() && !((ScilabDocument) textPaneAt.getDocument()).isContentModified()
            && (textPaneAt.getName() != null)) { /* Bug 5103 fix */
            return true;
        }

        if (!force) {
            AnswerOption answer;
            if (scilabClose) {
                answer = ScilabModalDialog.show(this, getTabPane().getTitleAt(indexTab) + SciNotesMessages.MODIFIED,
                                                SciNotesMessages.SCILAB_EDITOR, IconType.QUESTION_ICON, ButtonType.YES_NO);
            } else {
                answer = ScilabModalDialog.show(this, getTabPane().getTitleAt(indexTab) + SciNotesMessages.MODIFIED,
                                                SciNotesMessages.SCILAB_EDITOR, IconType.QUESTION_ICON, ButtonType.YES_NO_CANCEL);
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
            fileToSave = chooseFileToSave(SciNotesMessages.SAVE);
        } else {
            //check if the file has been modified by external software
            fileToSave = checkExternalModification(fileToSave);
        }

        if (fileToSave == null || fileToSave.length() == 0) {
            return true; /* Bug 5189: The user cancels ==> do not want an error message */
        }

        File newSavedFile = new File(fileToSave);

        if (!SaveFile.doSave(textPaneAt, newSavedFile, editorKit)) {
            return false;
        }

        ScilabDocument styledDocument = (ScilabDocument) textPaneAt.getDocument();
        styledDocument.setContentModified(false);

        // Get current file path for Execute file into Scilab
        fileFullPath = newSavedFile.getAbsolutePath();
        getTextPane().setLastModified(newSavedFile.lastModified());

        if (textPaneAt.getName() == null) {
            String name = getTabPane().getTitleAt(getTabPane().getSelectedIndex());
            String index = name.substring(name.length() - 1, name.length());
            tabList.remove(Integer.valueOf(index));
            closedTabList.add(Integer.valueOf(index));
        }

        textPaneAt.setName(fileToSave);
        getTabPane().setTitleAt(getTabPane().getSelectedIndex() , newSavedFile.getName());

        setTitle(textPaneAt.getTitle());

        return true;
    }

    /**
     * Overwrite the file at a given index.
     * @param indexTab the textPane index containing the file contents
     * @return execution status
     */
    public boolean overwrite(int indexTab) {
        ScilabEditorPane textPaneAt = getTextPane(indexTab);
        String fileToSave = textPaneAt.getName();
        if (fileToSave == null) {
            return true;
        }

        File newSavedFile = new File(fileToSave);
        if (!newSavedFile.exists()) {
            return true;
        }

        if (!SaveFile.doSave(textPaneAt, newSavedFile, editorKit)) {
            return false;
        }

        ScilabDocument styledDocument = (ScilabDocument) textPaneAt.getDocument();
        styledDocument.setContentModified(false);

        getTabPane().setTitleAt(getTabPane().getSelectedIndex() , newSavedFile.getName());

        // Get current file path for Execute file into Scilab
        fileFullPath = newSavedFile.getAbsolutePath();
        getTextPane().setLastModified(newSavedFile.lastModified());

        textPaneAt.setName(fileToSave);
        setTitle(textPaneAt.getTitle());

        return true;
    }

    /**
     * Check for external modification. If it is the case, propose
     * to select a new file name.
     * @param filename The name of the file
     * @return the filename where to save
     */
    public String checkExternalModification(String filename) {
        File newSavedFile = new File(filename);
        if (newSavedFile.lastModified() > getTextPane().getLastModified()) {
            if (ScilabModalDialog.show(this, String.format(SciNotesMessages.EXTERNAL_MODIFICATION, newSavedFile.getPath()),
                                       SciNotesMessages.REPLACE_FILE_TITLE, IconType.QUESTION_ICON,
                                       ButtonType.YES_NO) == AnswerOption.NO_OPTION) {
                return chooseFileToSave(SciNotesMessages.SAVE);
            }
        }
        return filename;
    }

    /**
     * Return through a file selector the name of the selected
     * file.
     * @param title the title for JFileChooser
     * @return the file picked up by the user
     */
    public String chooseFileToSave(String title) {
        String extension = new String();

        String initialDirectoryPath = getTextPane().getName();
        if (initialDirectoryPath == null) {
            initialDirectoryPath =  ConfigManager.getLastOpenedDirectory();
        }

        SciFileFilter sceFilter = new SciFileFilter(ALL_SCE_FILES, null, 0);
        SciFileFilter sciFilter = new SciFileFilter(ALL_SCI_FILES, null, 1);
        SciFileFilter scxFilter = new SciFileFilter(ALL_SCX_FILES, null, 2);
        SciFileFilter tstFilter = new SciFileFilter(ALL_TST_FILES, null, 3);
        SciFileFilter startFilter = new SciFileFilter(ALL_START_FILES, null, 4);
        SciFileFilter quitFilter = new SciFileFilter(ALL_QUIT_FILES, null, 5);
        SciFileFilter demFilter = new SciFileFilter(ALL_DEM_FILES, null, 6);
        SciFileFilter allFilter = new SciFileFilter(ALL_FILES, null, 7);
        SciFileFilter allScilabFilter = new SciFileFilter(ALL_SCILAB, null, 8);

        SwingScilabFileChooser fileChooser = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());

        fileChooser.setInitialDirectory(ConfigManager.getLastOpenedDirectory());
        fileChooser.setAcceptAllFileFilterUsed(false);
        fileChooser.setInitialDirectory(initialDirectoryPath);
        fileChooser.setUiDialogType(Juigetfile.SAVE_DIALOG);

        // order is also important here
        fileChooser.addChoosableFileFilter(sceFilter);
        fileChooser.addChoosableFileFilter(sciFilter);
        fileChooser.addChoosableFileFilter(scxFilter);
        fileChooser.addChoosableFileFilter(tstFilter);
        fileChooser.addChoosableFileFilter(startFilter);
        fileChooser.addChoosableFileFilter(quitFilter);
        fileChooser.addChoosableFileFilter(demFilter);
        fileChooser.addChoosableFileFilter(allFilter);
        fileChooser.addChoosableFileFilter(allScilabFilter);

        //select default file type
        fileChooser.setFileFilter(sceFilter);
        fileChooser.setTitle(title);

        String name = ((ScilabDocument) getTextPane().getDocument()).getFirstFunctionName();
        if (name != null) {
            fileChooser.setSelectedFile(new File(name + SCI_EXTENSION));
        }

        int retval = fileChooser.showSaveDialog(this);

        if (retval == JFileChooser.APPROVE_OPTION) {
            File f = fileToCanonicalFile(fileChooser.getSelectedFile());
            initialDirectoryPath = f.getPath();
            if (f.exists()) {
                if (ScilabModalDialog.show(this, SciNotesMessages.REPLACE_FILE_TITLE,
                                           SciNotesMessages.FILE_ALREADY_EXIST, IconType.QUESTION_ICON,
                                           ButtonType.YES_NO) == AnswerOption.NO_OPTION) {
                    return chooseFileToSave(SciNotesMessages.SAVE);
                }
            }

            /*we test if the file has already a scilab extension*/
            boolean hasNoExtension = true;

            // if the file name is like this : any character , a dot , then 2,3or 4 characters, then
            // we consider the file has already an extension
            // we previously only check for .sci and .sce extension, but what if the user open a txt file
            String fileName = f.getName();
            if (fileName.lastIndexOf(DOT) != -1) {
                if (fileName.substring(fileName.lastIndexOf(DOT), fileName.length()).length() >= 2
                    && fileName.substring(fileName.lastIndexOf(DOT), fileName.length()).length() <= 4) {
                    hasNoExtension = false;
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
                } else if (fileChooser.getFileFilter() == tstFilter) {
                    extension = TST_EXTENSION;
                } else if (fileChooser.getFileFilter() == startFilter) {
                    extension = START_EXTENSION;
                } else if (fileChooser.getFileFilter() == quitFilter) {
                    extension = QUIT_EXTENSION;
                } else if (fileChooser.getFileFilter() == demFilter) {
                    extension = DEM_EXTENSION;
                } else {
                    extension = "";
                }
                return f.getPath() + extension;
            }
            return f.getPath();
        } else if (retval == JFileChooser.CANCEL_OPTION) {
            return "";
        }

        return null;
    }

    /**
     * Save a file.
     * @return execution status
     */
    public boolean saveAs() {
        String filename = chooseFileToSave(SciNotesMessages.SAVE_AS);
        if (filename == null || filename.length() == 0) {
            return true;
        }

        File f = new File(filename);
        ScilabDocument styledDocument = (ScilabDocument) getTextPane().getDocument();

        if (!SaveFile.doSave(getTextPane(), f, editorKit)) {
            return false;
        }

        if (getTextPane().getName() == null) {
            String name = getTabPane().getTitleAt(getTabPane().getSelectedIndex());
            String index = name.substring(name.length() - 1, name.length());
            tabList.remove(Integer.valueOf(index));
            closedTabList.add(Integer.valueOf(index));
        }

        ConfigManager.saveLastOpenedDirectory(f.getPath());
        ConfigSciNotesManager.saveToRecentOpenedFiles(f.getPath());
        ConfigSciNotesManager.renameOpenFilesItem(f.getPath(), this, getTextPane());
        getTextPane().setName(f.getPath());
        getTabPane().setTitleAt(getTabPane().getSelectedIndex(), f.getName());
        setTitle(getTextPane().getTitle());

        RecentFileAction.updateRecentOpenedFilesMenu(this);

        NavigatorWindow.updateNavigator(styledDocument);
        styledDocument.setContentModified(false);
        getTextPane().setLastModified(f.lastModified());
        getTextPane().setReadOnly(false);
        getInfoBar().setText(getTextPane().getInfoBarText());

        // Get current file path for Execute file into Scilab
        fileFullPath = f.getAbsolutePath();

        return true;
    }

    /**
     * Create a new tab in SciNotes.
     * @param title the title of the tab
     * @return the text component inside the tab
     */
    public ScilabEditorPane addTab(String title) {
        return addTab(title, Integer.MAX_VALUE);
    }

    /**
     * Create a new tab in SciNotes.
     * @param title the title of the tab
     * @param index the index where to put the new tab
     * @return the text component inside the tab
     */
    public ScilabEditorPane addTab(String title, int index) {
        ScilabEditorPane sep = new ScilabEditorPane(this);
        initPane(sep);
        int ind = Math.min(Math.max(0, index), tabPane.getTabCount());
        tabPane.insertTab(title, null, sep.getParentComponent(), "", ind);
        tabPane.setSelectedIndex(ind);
        setContentPane(tabPane);
        initInputMap(sep);
        updateTabTitle();
        getInfoBar().setText(sep.getInfoBarText());
        repaint();
        sep.init();
        return sep;
    }

    /**
     * Init a pane
     * @param pane the pane to init
     */
    public void initPane(ScilabEditorPane pane) {
        initPane(pane, false);
    }

    /**
     * Init the EditorPane.
     * @param pane the EditorPane
     * @param plain true for a plain view or false for a wrapped view
     */
    public void initPane(ScilabEditorPane pane, boolean plain) {
        setHighlight(pane);
        pane.setEditorKit(new ScilabEditorKit(plain));

        // Panel of line number for the text pane
        pane.getXln().setWhereamiLineNumbering(ConfigSciNotesManager.getLineNumberingState());
        activateHelpOnTyping(pane);

        pane.setBorder(BorderFactory.createEmptyBorder(2, 2, 2, 2));
        pane.setFont(ConfigSciNotesManager.getFont());

        pane.setBackground(ConfigSciNotesManager.getSciNotesBackgroundColor());
        pane.setCaretColor(ConfigSciNotesManager.getSciNotesForegroundColor());

        pane.setFocusable(true);
        pane.setRequestFocusEnabled(true);
        pane.setDragEnabled(true); /* Bug 5497 */

        DropFilesListener dndTarget = new DropFilesListener(pane);

        pane.setComponentPopupMenu(SciNotesGUI.generateRightClickPopup(this));
    }

    /**
     * Init the EditorPane with the InputMap found in ConfigSciNotesManager
     * @param pane the EditorPane
     */
    public void initInputMap(ScilabEditorPane pane) {
        setKeyStrokeAction(pane, this);
        LineBeautifierAction.putInInputMap(pane);
    }

    /**
     * Split the EditorPane
     * @param vertical true for a vertical split
     */
    public void splitTab(boolean vertical) {
        ScilabEditorPane pane = getTextPane();
        ScilabEditorPane leftPane = new ScilabEditorPane(editor);
        ScilabEditorPane rightPane = new ScilabEditorPane(editor);
        NavigatorWindow.changePaneOnSplit(pane, leftPane);
        initPane(leftPane, true);
        initPane(rightPane, true);
        leftPane.setOtherPaneInSplit(rightPane);
        rightPane.setOtherPaneInSplit(leftPane);
        pane.copyProps(leftPane);
        pane.copyProps(rightPane);
        ScilabDocument doc = (ScilabDocument) pane.getDocument();
        leftPane.setDocument(doc);
        rightPane.setDocument(doc);
        leftPane.setCaretPosition(0);
        rightPane.setCaretPosition(0);
        JSplitPane split;
        if (vertical) {
            split = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
        } else {
            split = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
        }
        leftPane.setSplitPane(split);
        rightPane.setSplitPane(split);
        tabPane.setComponentAt(tabPane.getSelectedIndex(), split);
        split.setLeftComponent(leftPane.getScrollPane());
        split.setRightComponent(rightPane.getScrollPane());
        split.setResizeWeight(0.5);
        setContentPane(tabPane);
        activateHelpOnTyping(leftPane);
        activateHelpOnTyping(rightPane);
        initInputMap(leftPane);
        initInputMap(rightPane);
        if (doc.getBinary()) {
            leftPane.setBinary(true);
            rightPane.setBinary(true);
        }
        getInfoBar().setText(leftPane.getInfoBarText());
        updateTabTitle();
    }

    /**
     * Remove a split
     */
    public void removeSplit() {
        if (tabPane.getSelectedComponent() instanceof JSplitPane) {
            ScilabEditorPane pane = new ScilabEditorPane(editor);
            ScilabEditorPane textpane = getTextPane();
            NavigatorWindow.changePaneOnSplit(textpane, pane);
            initPane(pane);
            textpane.setOtherPaneInSplit(null);
            textpane.copyProps(pane);
            ScilabDocument doc = (ScilabDocument) textpane.getDocument();
            pane.setDocument(doc);
            pane.setCaretPosition(0);
            activateHelpOnTyping(pane);
            tabPane.setComponentAt(tabPane.getSelectedIndex(), pane.getScrollPane());
            setContentPane(tabPane);
            initInputMap(pane);
            if (doc.getBinary()) {
                pane.setBinary(true);
            }
            getInfoBar().setText(pane.getInfoBarText());
            updateTabTitle();
        }
    }

    /**
     * Create an empty tab inside SciNotes.
     * @return the text component inside the tab
     */
    public ScilabEditorPane addEmptyTab() {
        ScilabEditorPane sep;
        if (closedTabList.size() > 0) {
            Integer n = (Integer) Collections.min(closedTabList);
            closedTabList.remove(n);
            sep = addTab(SciNotesMessages.UNTITLED + n);
            sep.setShortName(SciNotesMessages.UNTITLED + n);
            sep.setTitle(SciNotesMessages.UNTITLED + n);
        } else {
            numberOfUntitled++;
            tabList.add(Integer.valueOf(numberOfUntitled));
            sep = addTab(SciNotesMessages.UNTITLED + numberOfUntitled);
            sep.setShortName(SciNotesMessages.UNTITLED + numberOfUntitled);
            sep.setTitle(SciNotesMessages.UNTITLED + numberOfUntitled);
        }
        setTitle(sep.getTitle());

        return sep;
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
        } catch (NullPointerException e) { // not a file name, no path prefix to remove, but maybe a '*'
            textPaneName = getTabPane().getTitleAt(getTabPane().getSelectedIndex());
            if (textPaneName.charAt(0) == '*') {
                newTitle.append(textPaneName.substring(1, textPaneName.length()));
            } else {
                newTitle.append(textPaneName);
            }
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
     * Reload a file inside SciNotes.
     * @param index the index
     */
    public void reload(int index) {
        ScilabEditorPane textPaneAt = getTextPane(index);
        if ((index == 0) && (getTabPane().getTabCount() == 1)) {
            for (int j = 0; j < tabPane.getChangeListeners().length; j++) {
                tabPane.removeChangeListener(tabPane.getChangeListeners()[j]);
            }
        }
        ConfigSciNotesManager.removeFromOpenFiles(this, textPaneAt);
        tabPane.remove(index);
        File f = new File(textPaneAt.getName());
        if (f.exists()) {
            loadFile(f, index);
        } else {
            createNewFile(f);
        }
        ConfigSciNotesManager.saveToOpenFiles(f.getPath(), this, getTextPane());
    }

    /**
     * Load a file inside SciNotes.
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
            if (f.exists()) {
                loadFile(f);
            } else {
                createNewFile(f);
            }
            ConfigSciNotesManager.saveToOpenFiles(f.getPath(), this, getTextPane());
        }

        // Get current file path for Execute file into Scilab
        fileFullPath = f.getAbsolutePath();
    }

    /**
     * Load a file inside SciNotes.
     * @param file the file to open
     */
    public void readFileAndWait(File file) {
        /** Is this file already opened */
        File f = file;
        boolean alreadyOpened = false;
        int index = -1;
        for (int i = 0; i < tabPane.getTabCount(); i++) {
            ScilabEditorPane textPaneAt = getTextPane(i);
            if (f.getAbsolutePath().equals(textPaneAt.getName())) {
                /* File is already opened */
                tabPane.setSelectedIndex(i);
                if (f.lastModified() > textPaneAt.getLastModified()) {

                    /*
                     * Create a new messagebox to know what the user wants to do
                     * if the file has been modified outside SciNotes
                     */
                    MessageBox messageBox = ScilabMessageBox.createMessageBox();
                    messageBox.setTitle(SciNotesMessages.REPLACE_FILE_TITLE);
                    messageBox.setMessage(String.format(SciNotesMessages.EXTERNAL_MODIFICATION, textPaneAt.getName()));

                    String[] labels = new String[]{SciNotesMessages.RELOAD, SciNotesMessages.OVERWRITE, SciNotesMessages.IGNORE};
                    messageBox.setButtonsLabels(labels);

                    messageBox.setIcon("question"); // Question icon

                    messageBox.setParentForLocation(this); // Centered on SciNotes main window

                    messageBox.displayAndWait(); // Waits for a user action

                    switch (messageBox.getSelectedButton()) {
                    case 1: // Reload
                        if ((i == 0) && (getTabPane().getTabCount() == 1)) {
                            for (int j = 0; j < tabPane.getChangeListeners().length; j++) {
                                tabPane.removeChangeListener(tabPane.getChangeListeners()[j]);
                            }
                        }
                        tabPane.remove(i);
                        f = new File(textPaneAt.getName());
                        index = i;
                        break;
                    case 2: // Overwrite 2
                        overwrite(i);
                        alreadyOpened = true;
                        break;
                    default: // Ignore
                        alreadyOpened = true;
                    }
                } else {
                    alreadyOpened = true;
                }
                break;
            }
        }

        if (!alreadyOpened) {
            if (f.exists()) {
                loadFile(f, index);
            } else {
                createNewFile(f);
            }
            ConfigSciNotesManager.saveToOpenFiles(f.getPath(), this, getTextPane());
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
    public static void setWhereamiLineNumbering(int state) {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = (ScilabEditorPane) ed.getTextPane(i);
                sep.getXln().setWhereamiLineNumbering(state);
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().getXln().setWhereamiLineNumbering(state);
                }
            }
            ed.repaint();
        }
    }

    /**
     * Auto-indent mode management
     * @param b true to activate auto-indent mode
     */
    public static void setAutoIndent(boolean b) {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = (ScilabEditorPane) ed.getTextPane(i);
                ((ScilabDocument) sep.getDocument()).setAutoIndent(b);
                if (sep.getOtherPaneInSplit() != null) {
                    ((ScilabDocument) sep.getOtherPaneInSplit().getDocument()).setAutoIndent(b);
                }
            }
        }
    }

    /**
     * Enable the highlighted line in this editor
     * @param b boolean
     */
    public static void enableHighlightedLine(boolean b) {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = (ScilabEditorPane) ed.getTextPane(i);
                sep.enableHighlightedLine(b);
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().enableHighlightedLine(b);
                }
            }
        }
    }

    /**
     * Enable the help on typing in the current textPane
     * @param pane the pane
     */
    public static void activateHelpOnTyping(ScilabEditorPane pane) {
        pane.activateHelpOnTyping();
    }

    /**
     * Enable the help on typing in this editor
     */
    public static void activateHelpOnTyping() {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = (ScilabEditorPane) ed.getTextPane(i);
                sep.activateHelpOnTyping();
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().activateHelpOnTyping();
                }
            }
        }
    }

    /**
     * Set the color of the highlighted line in this editor
     * @param c Color
     */
    public static void setHighlightedLineColor(Color c) {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = (ScilabEditorPane) ed.getTextPane(i);
                sep.setHighlightedLineColor(c);
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().setHighlightedLineColor(c);
                }
            }
        }
    }

    /**
     * Set the color of the contour of the highlighted line in this editor
     * @param c Color
     */
    public static void setHighlightedContourColor(Color c) {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = (ScilabEditorPane) ed.getTextPane(i);
                sep.setHighlightedContourColor(c);
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().setHighlightedContourColor(c);
                }
            }
        }
    }

    /**
     * Set the highlighted line in this textPane
     * @param sep ScilabEditorPane
     */
    public static void setHighlight(ScilabEditorPane sep) {
        sep.enableHighlightedLine(ConfigSciNotesManager.getHighlightState());
        Color[] arr = ConfigSciNotesManager.getHighlightColors();
        sep.setHighlightedLineColor(arr[0]);
        sep.setHighlightedContourColor(arr[1]);
    }

    /**
     * Get SciNotes as a Tab.
     * @return SciNotes instance
     * @see org.scilab.modules.gui.tab.Tab#getAsSimpleTab()
     */
    public SimpleTab getAsSimpleTab() {
        return this;
    }

    /**
     * Get SciNotes parent Window.
     * @return parent Window
     * @see org.scilab.modules.gui.tab.Tab#getParentWindow()
     */
    public Window getParentWindow() {
        return parentWindow;
    }

    /**
     * Get the UUID associated with the editor instance.
     * @return unique identifier
     */
    public UUID getUUID() {
        return uuid;
    }

    /**
     * Add a status bar to SciNotes.
     * @param infoBarToAdd the status bar to be added
     * @see org.scilab.modules.gui.uielement.UIElement#addInfoBar(org.scilab.modules.gui.textbox.TextBox)
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        setInfoBar(infoBarToAdd);
    }

    /**
     * Add a menu bar to SciNotes.
     * @param menuBarToAdd the menu bar to be added
     * @see org.scilab.modules.gui.uielement.UIElement#addMenuBar(org.scilab.modules.gui.menubar.MenuBar)
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        setMenuBar(menuBarToAdd);
    }

    /**
     * Add a tool bar to SciNotes.
     * @param toolBarToAdd the tool bar to be added
     * @see org.scilab.modules.gui.uielement.UIElement#addToolBar(org.scilab.modules.gui.toolbar.ToolBar)
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        setToolBar(toolBarToAdd);
    }

    /**
     * Get SciNotes main JTabbedPane.
     * @return SciNotes main JTabbedPane
     */
    public JTabbedPane getTabPane() {
        return tabPane;
    }

    /**
     * Set SciNotes main JTabbedPane.
     * @param tabPane SciNotes main JTabbedPane
     */
    public void setTabPane(JTabbedPane tabPane) {
        this.tabPane = tabPane;
    }

    /**
     * Return the Full path of the file.
     * @return the full path
     */
    public String getFileFullPath() {
        return fileFullPath;
    }

    /**
     * Load a file and add it at the end
     * @param f the file to load
     */
    public void loadFile(File f) {
        loadFile(f, -1);
    }

    /**
     * Load a file and add it at the index
     * @param f the file to load
     * @param index the index where to put the file
     */
    public void loadFile(File f, int index) {
        getInfoBar().setText(SciNotesMessages.LOADING);
        // Get current file path for Execute file into Scilab
        fileFullPath = f.getAbsolutePath();

        ScilabDocument styleDocument = null;
        ScilabEditorPane theTextPane;

        // File exist
        if (f.exists()) {
            if (index != -1) {
                theTextPane = addTab(f.getName(), index);
            } else {
                theTextPane = addTab(f.getName());
            }
            styleDocument = (ScilabDocument) theTextPane.getDocument();
            styleDocument.disableUndoManager();
            theTextPane.setLastModified(f.lastModified());

            FileInputStream fis = null;
            InputStreamReader isr = null;
            BufferedReader br = null;

            try {
                styleDocument.setUpdater(false);
                boolean indentMode = styleDocument.getAutoIndent();
                styleDocument.setAutoIndent(false);
                fis = new FileInputStream(f);
                try {
                    try {
                        isr = new InputStreamReader(fis, styleDocument.getEncoding());
                        br = new BufferedReader(isr);
                        editorKit.read(br, styleDocument, 0);
                    } catch (ChangedCharSetException e) {
                        isr = new InputStreamReader(fis, e.getCharSetSpec());
                        br = new BufferedReader(isr);
                        editorKit.read(br, styleDocument, 0);
                    }
                } catch (BadLocationException e) {
                    e.printStackTrace();
                }
                styleDocument.setAutoIndent(indentMode);
                styleDocument.setUpdater(true);
            } catch (IOException ioex) {
                ioex.printStackTrace();
            } finally {
                try {
                    if (fis != null) {
                        fis.close();
                    }
                    if (isr != null) {
                        isr.close();
                    }
                    if (br != null) {
                        br.close();
                    }
                } catch (IOException e) { }
            }

            theTextPane.setName(f.getAbsolutePath());
            getTabPane().setTitleAt(getTabPane().getSelectedIndex() , f.getName());
            setTitle(theTextPane.getTitle());
            styleDocument.setContentModified(false);
            styleDocument.enableUndoManager();

            if (styleDocument.getBinary()) {
                theTextPane.setBinary(true);
            }
            getInfoBar().setText(theTextPane.getInfoBarText());

            EncodingAction.updateEncodingMenu((ScilabDocument) getTextPane().getDocument());

            // Empty the undo Manager
            UndoManager undo = ((ScilabDocument) getTextPane().getDocument()).getUndoManager();
            undo.discardAllEdits();
        }
    }

    /**
     * Creates a file if it doesn't exist
     * @param f the file to create
     */
    public void createNewFile(File f) {
        ScilabEditorPane theTextPane = addEmptyTab();
        ScilabDocument styleDocument = null;
        int choice = JOptionPane.showConfirmDialog(SciNotes.this,
                                                   String.format(SciNotesMessages.FILE_DOESNT_EXIST, f.getName()),
                                                   SCINOTES,
                                                   JOptionPane.YES_NO_OPTION);
        if (choice == 0) { //OK
            styleDocument = (ScilabDocument) theTextPane.getDocument();
            styleDocument.disableUndoManager();

            BufferedWriter bw = null;
            OutputStreamWriter osw = null;
            FileOutputStream fos = null;

            try {
                fos = new FileOutputStream(f);
                osw = new OutputStreamWriter(fos, styleDocument.getEncoding());
                bw = new BufferedWriter(osw);
                editorKit.write(bw, styleDocument, 0, styleDocument.getLength());
                bw.flush();
            } catch (IOException e) {
                e.printStackTrace();
            } catch (BadLocationException e) {
                e.printStackTrace();
            } finally {
                try {
                    if (fos != null) {
                        fos.close();
                    }
                    if (osw != null) {
                        osw.close();
                    }
                    if (bw != null) {
                        bw.close();
                    }
                } catch (IOException e) { }
            }

            ConfigManager.saveLastOpenedDirectory(f.getPath());
            ConfigSciNotesManager.saveToRecentOpenedFiles(f.getPath());
            ConfigSciNotesManager.saveToOpenFiles(editor.getTextPane().getName(), editor, editor.getTextPane());
            theTextPane.setName(f.getPath());
            getTabPane().setTitleAt(getTabPane().getSelectedIndex() , f.getName());
            setTitle(theTextPane.getTitle());
            RecentFileAction.updateRecentOpenedFilesMenu(this);

            styleDocument.setContentModified(false);
            styleDocument.enableUndoManager();
            theTextPane.setLastModified(f.lastModified());

            // Get current file path for Execute file into Scilab
            fileFullPath = f.getAbsolutePath();
        }

        getInfoBar().setText("");
    }

    /**
     * EditorKit Getter
     * @return EditorKit
     */
    public EditorKit getEditorKit() {
        return editorKit;
    }

    /**
     * @param scinotes the focused editor
     */
    public static void setEditor(SciNotes scinotes) {
        editor = scinotes;
    }

    /**
     * @return the focused editor
     */
    public static SciNotes getEditor() {
        return editor;
    }

    /**
     * EditorKit Setter
     * @param editorKit EditorKit
     */
    public void setEditorKit(EditorKit editorKit) {
        this.editorKit = editorKit;
    }

    /**
     * Close scinotes from scilab.
     *
     * This method can be called outside the EDT thread.
     */
    public static void closeSciNotesFromScilab() {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {

                    public void run() {
                        SciNotes[] arr = scinotesList.toArray(new SciNotes[0]);
                        for (int i = 0; i < arr.length; i++) {
                            arr[i].setProtectOpenFileList(true);
                            ExitAction.doExit(arr[i]);
                        }
                        scinotesList.clear();
                        NavigatorWindow.closeCurrent();
                    }
                });
        } catch (InterruptedException e) {
            LogFactory.getLog(SciNotes.class).error(e);
        } catch (InvocationTargetException e) {
            LogFactory.getLog(SciNotes.class).error(e);
        }
    }

    /**
     * @param f the file
     * @return the canonical file if possible
     */
    public static File fileToCanonicalFile(File f) {
        /* Fix bug 5648 */
        try {
            return f.getCanonicalFile();
        } catch (IOException e) {
            return f;
        }
    }

    /**
     * Set the shortcuts in the pane relatively to the file keysConfiguration.xml
     * @param sep the textpane
     * @param ed the SciNotes editor
     */
    private static void setKeyStrokeAction(ScilabEditorPane sep, SciNotes ed) {
        Map<String, KeyStroke> map = new HashMap();
        ConfigSciNotesManager.addMapActionNameKeys(map);

        ClassLoader loader = ClassLoader.getSystemClassLoader();
        Iterator<String> iter = map.keySet().iterator();
        while (iter.hasNext()) {
            String action = iter.next();
            if (!action.equals("SciNotesCompletionAction")) {
                String className;
                if (action.lastIndexOf(DOT) != -1)  {
                    className = action;
                } else {
                    className = DEFAULTACTIONPATH + DOT + action;
                }
                try {
                    Class clazz = loader.loadClass(className);
                    Constructor constructor = clazz.getConstructor(new Class[]{String.class, SciNotes.class});
                    Object act = constructor.newInstance(new Object[]{"", ed});
                    sep.getInputMap().put(map.get(action), act);
                } catch (ClassNotFoundException e) {
                    System.err.println("No action: " + className);
                } catch (InstantiationException e) {
                    System.err.println("Problem to instantiate in action: " + className);
                } catch (NoSuchMethodException e) {
                    System.err.println("No valid constructor in action: " + className);
                } catch (IllegalAccessException e) {
                    System.err.println("The constructor must be public: " + className);
                } catch (InvocationTargetException e) {
                    System.err.println("The constructor in " + className + " threw an exception :");
                    e.printStackTrace();
                }
            } else {
                sep.getInputMap().put(map.get(action), new SciNotesCompletionAction(sep, ed));
            }
        }
    }
}
