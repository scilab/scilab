/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - 2011 - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.scinotes;

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.StringReader;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.UUID;
import java.util.logging.Logger;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.filechooser.FileFilter;
import javax.swing.text.BadLocationException;
import javax.swing.text.EditorKit;
import javax.swing.text.View;
import javax.swing.undo.UndoManager;

import org.w3c.dom.Document;

import org.flexdock.docking.event.DockingEvent;
import org.scilab.modules.commons.CommonFileUtils;
import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.commons.xml.ScilabXMLUtilities;
import org.scilab.modules.commons.xml.XConfiguration;
import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;
import org.scilab.modules.core.Scilab;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.filechooser.Juigetfile;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBox;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.SciFileFilter;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;

import org.scilab.modules.scinotes.actions.DoubleQuoteStringAction;
import org.scilab.modules.scinotes.actions.EncodingAction;
import org.scilab.modules.scinotes.actions.EndOfLineAction;
import org.scilab.modules.scinotes.actions.ExitAction;
import org.scilab.modules.scinotes.actions.FindAction;
import org.scilab.modules.scinotes.actions.IncrementalSearchAction;
import org.scilab.modules.scinotes.actions.IndentAction;
import org.scilab.modules.scinotes.actions.InsertOverwriteAction;
import org.scilab.modules.scinotes.actions.LineBeautifierAction;
import org.scilab.modules.scinotes.actions.OpenSourceFileOnKeywordAction;
import org.scilab.modules.scinotes.actions.RecentFileAction;
import org.scilab.modules.scinotes.actions.RegisterFavoriteDirsAction;
import org.scilab.modules.scinotes.actions.RemoveTrailingWhiteAction;
import org.scilab.modules.scinotes.actions.RestoreOpenedFilesAction;
import org.scilab.modules.scinotes.actions.SciNotesCompletionAction;
import org.scilab.modules.scinotes.actions.SearchWordInFilesAction;
import org.scilab.modules.scinotes.tabfactory.CodeNavigatorTab;
import org.scilab.modules.scinotes.tabfactory.SciNotesTab;
import org.scilab.modules.scinotes.tabfactory.SciNotesTabFactory;
import org.scilab.modules.scinotes.tabfactory.SearchInFilesTab;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.DropFilesListener;
import org.scilab.modules.scinotes.utils.NavigatorWindow;
import org.scilab.modules.scinotes.utils.SaveFile;
import org.scilab.modules.scinotes.utils.SciNotesContents;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.scilab.modules.scinotes.utils.ScilabTabbedPane;
import org.scilab.modules.scinotes.utils.SearchFile;

/**
 * Main SciNotes class.
 *
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public class SciNotes extends SwingScilabDockablePanel {

    private static final long serialVersionUID = -6410183357490518676L;

    private static final String XPATH_SCINOTES_KEY = "//general/shortcuts/body/actions/action-folder[@name='Scinotes']/action";
    private static final String XPATH_SCINOTES_ACTION = "/map/scinotes/entry";

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

    private static final Map<String, String> actionToName;
    private static Map<String, KeyStroke> actionKeys;

    private static List<SciNotes> scinotesList = new ArrayList<SciNotes>();
    private static SciNotes editor;
    private static boolean mustWrapLines;

    private SwingScilabWindow parentWindow;
    private UUID uuid;

    private ScilabTabbedPane tabPane;
    private final SciNotesContents contentPane;
    private NavigatorWindow navigator;
    private SearchFile searchInFiles;

    private int numberOfUntitled;
    private EditorKit editorKit;

    private JButton undoButton;
    private JButton redoButton;

    private boolean protectOpenFileList;
    private boolean restored;
    private boolean firstOpen = true;

    private final List<Integer> tabList = new ArrayList<Integer>();
    private final List<Integer> closedTabList = new ArrayList<Integer>();

    static {
        ConfigSciNotesManager.createUserCopy();
        ScilabTabFactory.getInstance().addTabFactory(SciNotesTabFactory.getInstance());
        Scilab.registerInitialHook(new Runnable() {
            @Override
            public void run() {
                updateSciNotes();
            }
        });

        Document doc = ScilabXMLUtilities.readDocument(System.getenv("SCI") + "/modules/console/etc/Actions-Configuration.xml");
        actionToName = XConfiguration.get(doc, "name", String.class, "action", String.class, XPATH_SCINOTES_ACTION);
        XConfiguration.addXConfigurationListener(new SciNotesConfiguration());
    }

    /**
     * Create SciNotes instance
     */
    public SciNotes(String uuid) {
        super(SCINOTES, uuid);
        setWindowIcon("accessories-text-editor");
        setAssociatedXMLIDForHelp("scinotes");
        this.uuid = UUID.fromString(uuid);
        SciNotesAutosave.autosave();
        editor = this;
        scinotesList.add(this);
        numberOfUntitled = 0;
        mustWrapLines = SciNotesOptions.getSciNotesDisplay().wrapLines;
        editorKit = new ScilabEditorKit(!mustWrapLines);
        protectOpenFileList = false;
        contentPane = new SciNotesContents(this);
        tabPane = contentPane.getScilabTabbedPane();
        tabPane.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                if (getTextPane() != null) {
                    // updateUI();
                    getTextPane().updateInfosWhenFocused();
                    getTextPane().requestFocus();
                    getTextPane().highlightWords(IncrementalSearchAction.getWord(SciNotes.this), IncrementalSearchAction.getExact(SciNotes.this));

                    // Update encoding menu
                    EncodingAction.updateEncodingMenu((ScilabDocument) getTextPane().getDocument());

                    // Update End Of Line menu
                    EndOfLineAction.updateEolMenu((ScilabDocument) getTextPane().getDocument());
                    setTitle(getTextPane().getTitle());
                }
            }
        });
        this.setContentPane(contentPane);
    }

    /**
     * Default constructor
     */
    public SciNotes() {
        this(UUID.randomUUID().toString());
    }

    public static void configurationChanged(SciNotesConfiguration.Conf conf) {
        setWhereamiLineNumbering();
        setAutoIndent();
        setHorizontalWrap();
        setDefaultTabulation();
        if (conf.keymap) {
            actionKeys = null;
            setKeyStrokeActions();
            setAllMenus();
        }
        if (conf.autosave) {
            SciNotesAutosave.stopAutosave();
            SciNotesAutosave.autosave();
        }
        if (conf.preferences) {
            RecentFileAction.updateRecentOpenedFilesMenu();
        }

        updatePanes(conf);
    }

    public static Map<String, KeyStroke> getActionKeys() {
        if (actionKeys == null) {
            Document doc = XConfiguration.getXConfigurationDocument();
            actionKeys = XConfiguration.get(doc, "name", String.class, "key", KeyStroke.class, XPATH_SCINOTES_KEY);
        }

        return actionKeys;
    }

    public static Map<String, String> getActionName() {
        return actionToName;
    }

    /**
     * Get an opened editor from its uuid
     *
     * @param uuid
     *            the uuid
     * @return the corresponding editor
     */
    public static SciNotes getEditorFromUUID(String uuid) {
        for (SciNotes ed : scinotesList) {
            if (ed.getPersistentId().equals(uuid)) {
                return ed;
            }
        }

        return null;
    }

    public void setParentWindow() {
        this.parentWindow = SwingScilabWindow.createWindow(true);
        parentWindow.setLocation(150, 50);
        parentWindow.setSize(650, 550);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setTitle(String title) {
        super.setTitle(title);
        SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, tabPane);
        if (window != null) {
            window.setTitle(title);
        }
    }

    public void insertBottomComponent(Component c) {
        contentPane.insertBottomComponent(c);
    }

    /**
     * @return the SwingScilabWindow containing this editor
     */
    public SwingScilabWindow getSwingParentWindow() {
        return (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, this);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void undockingComplete(DockingEvent evt) {
        super.undockingComplete(evt);
        if (navigator != null) {
            navigator.addToolBar(null);
        }
        if (searchInFiles != null) {
            searchInFiles.addToolBar(null);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void dockingComplete(DockingEvent evt) {
        super.dockingComplete(evt);
        if (navigator != null) {
            navigator.changeToolBar();
        }
        if (searchInFiles != null) {
            searchInFiles.changeToolBar();
        }
    }

    /**
     * Add a code navigator
     */
    public void addNavigator() {
        if (navigator == null) {
            String navUUID = ConfigSciNotesManager.getCodeNavigatorStateForEditor(getPersistentId());
            boolean success = WindowsConfigurationManager.restoreUUID(navUUID);
            if (!success) {
                navigator = CodeNavigatorTab.getCodeNavigatorInstance(this, null);
                navigator.setParentWindow();
            }
            int count = getTabPane().getTabCount();
            for (int i = 0; i < count; i++) {
                navigator.addEditorPane(getTextPane(i));
            }
        }
    }

    /**
     * Add a code navigator
     */
    public void addNavigator(NavigatorWindow navigator) {
        this.navigator = navigator;
    }

    /**
     * Remove the navigator
     */
    public void removeNavigator() {
        navigator = null;
    }

    /**
     * @return the navigator associated with this editor
     */
    public NavigatorWindow getNavigator() {
        return navigator;
    }

    /**
     * Add a Search in files
     */
    public void addSearchInFiles() {
        if (searchInFiles == null) {
            String sfUUID = ConfigSciNotesManager.getSearchInFilesStateForEditor(getPersistentId());
            boolean success = WindowsConfigurationManager.restoreUUID(sfUUID);
            if (!success) {
                searchInFiles = SearchInFilesTab.getSearchInFilesTabInstance(this, null);
                searchInFiles.setParentWindow();
            }
        }
    }

    /**
     * Add a Search In Files
     */
    public void addSearchInFiles(SearchFile sf) {
        this.searchInFiles = sf;
    }

    /**
     * Remove Search in Files
     */
    public void removeSearchInFiles() {
        searchInFiles = null;
    }

    /**
     * @return the Search In Files associated with this editor
     */
    public SearchFile getSearchInFiles() {
        return searchInFiles;
    }

    /**
     * Update the editor
     */
    public static void updateSciNotes() {
        ScilabLexer.update();
        for (SciNotes editor : scinotesList) {
            if (editor.getTextPane() != null) {
                editor.getTextPane().repaint();
            }
        }
    }

    /**
     * Launch SciNotes with an empty file
     *
     * This method *must not* be called on the EDT thread.
     */
    public static void scinotes() {
        ScilabLexer.update();
        try {
            SwingUtilities.invokeAndWait(new Runnable() {

                @Override
                public void run() {
                    launchSciNotes();
                    // Open an empty file if no tabs were opened at launch.
                    if (editor.getTabPane().getTabCount() == 0) {
                        editor.openFile(null, 0, null);
                    }
                }
            });
        } catch (InterruptedException e) {
            Logger.getLogger(SciNotes.class.getName()).severe(e.toString());
            throw new RuntimeException(e);
        } catch (InvocationTargetException e) {
            Logger.getLogger(SciNotes.class.getName()).severe(e.toString());
            e.printStackTrace();
            throw new RuntimeException(e);
        }
    }

    /**
     * Launch SciNotes with a file name to open.
     *
     * @param filePath
     *            the name of the file to open
     *
     *            This method *must not* be called on the EDT thread.
     */
    public static void scinotes(final String filePath) {
        ScilabLexer.update();
        try {
            SwingUtilities.invokeAndWait(new Runnable() {

                @Override
                public void run() {
                    launchSciNotes();
                    editor.openFile(filePath, 0, null);
                }
            });
        } catch (InterruptedException e) {
            Logger.getLogger(SciNotes.class.getName()).severe(e.toString());
            throw new RuntimeException(e);
        } catch (InvocationTargetException e) {
            e.printStackTrace();
            Logger.getLogger(SciNotes.class.getName()).severe(e.toString());
            throw new RuntimeException(e);
        }
    }

    /**
     * Launch SciNotes with a file name to open and a line to highlight.
     *
     * @param filePath
     *            the name of the file to open
     * @param lineNumber
     *            the line to highlight
     *
     *            This method *must not* be called on the EDT thread.
     */
    public static void scinotes(final String filePath, final int lineNumber, final String functionName) {
        ScilabLexer.update();
        try {
            SwingUtilities.invokeAndWait(new Runnable() {

                @Override
                public void run() {
                    launchSciNotes();
                    editor.openFile(filePath, lineNumber, functionName);
                }
            });
        } catch (InterruptedException e) {
            Logger.getLogger(SciNotes.class.getName()).severe(e.toString());
            throw new RuntimeException(e);
        } catch (InvocationTargetException e) {
            e.printStackTrace();
            Logger.getLogger(SciNotes.class.getName()).severe(e.toString());
            throw new RuntimeException(e);
        }
    }

    /**
     * Launch SciNotes with a file name to open and a line to highlight.
     *
     * @param filePath
     *            the name of the file to open
     * @param option
     *            such as 'readonly'
     *
     *            This method *must not* be called on the EDT thread.
     */
    public static void scinotes(final String filePath, final String[] options) throws Exception {
        boolean hasAction = false;
        if (options != null && options.length != 0) {
            try {
                hasAction = executeAction(filePath, options);
            } catch (FileNotFoundException e) {
                throw new Exception(String.format(SciNotesMessages.INVALID_FILE, filePath));
            } catch (IOException e) {
                throw new Exception(String.format(SciNotesMessages.IO_EXCEPTION, e.getLocalizedMessage()));
            }
        }

        if (hasAction) {
            return;
        }

        ScilabLexer.update();
        try {
            SwingUtilities.invokeAndWait(new Runnable() {

                @Override
                public void run() {
                    launchSciNotes();
                    if (options != null && options.length != 0) {
                        editor.openFile(filePath, 0, options[0]);
                    } else {
                        editor.openFile(filePath, 0, "");
                    }
                }
            });
        } catch (InterruptedException e) {
            Logger.getLogger(SciNotes.class.getName()).severe(e.toString());
            throw new RuntimeException(e);
        } catch (InvocationTargetException e) {
            e.printStackTrace();
            Logger.getLogger(SciNotes.class.getName()).severe(e.toString());
            throw new RuntimeException(e);
        }
    }

    /**
     * Launch SciNotes with a provided text (from the help for example).
     *
     * @param text
     *            the text which should be modified
     */
    public static void scinotesWithText(String text) {
        ScilabLexer.update();
        launchSciNotes();
        ScilabEditorPane theTextPane;
        if (editor.getTabPane().getTabCount() != 0 && editor.getTextPane(0) != null) {
            String name = editor.getTextPane(0).getName();
            if (name == null) {
                theTextPane = editor.getTextPane(0);
            } else {
                theTextPane = editor.addEmptyTab();
            }
        } else {
            theTextPane = editor.addEmptyTab();
        }

        ScilabDocument styleDocument = (ScilabDocument) theTextPane.getDocument();
        try {
            editor.getEditorKit().read(new StringReader(text), styleDocument, theTextPane.getCaretPosition());
        } catch (IOException e) {
            System.err.println(SciNotesMessages.ERROR_WITH_STRING);
        } catch (BadLocationException e) {
            System.err.println(SciNotesMessages.ERROR_WITH_STRING);
        }
    }

    /**
     * Clone the current tab and if b is true close the tab
     *
     * @param ed
     *            the editor where the tab is
     * @param b
     *            a boolean
     */
    public static void cloneAndCloseCurrentTab(SciNotes ed, boolean b) {
        ScilabDocument cdoc = (ScilabDocument) ed.getTextPane().getDocument();
        String title = ed.getTabPane().getScilabTitleAt(ed.getTabPane().getSelectedIndex());
        ScilabEditorPane currentSep = ed.getTextPane();
        String winTitle = ed.getTitle();

        if (b) {
            ed.closeTabAtWithoutConfirmation(ed.getTabPane().getSelectedIndex());
            if (ed.getTabPane().getTabCount() == 0) {
                ClosingOperationsManager.startClosingOperationWithoutSave((SwingScilabDockablePanel) ed);
                ConfigSciNotesManager.removeEditorUUID(ed.uuid.toString());
            }
        }

        editor = null;

        scinotesWithText(cdoc.getText());
        ScilabEditorPane sep = editor.getTextPane();

        if (currentSep.getName() != null) {
            sep.setTitle(title);
            editor.setTitle(winTitle);
        }

        currentSep.copyProps(sep);
        ScilabDocument sdoc = (ScilabDocument) sep.getDocument();
        sdoc.setContentModified(cdoc.isContentModified());
        sdoc.getUndoManager().discardAllEdits();
        sep.setCaretPosition(0);
        editor.enableUndoButton(false);
        editor.enableRedoButton(false);
        ConfigSciNotesManager.saveToOpenFiles(sep.getName(), editor, editor.getTextPane());

        editor.activeRestoreTab();
    }

    /**
     * Clone the current tab and if b is true close the tab
     *
     * @param ed
     *            the editor where the tab is
     * @param b
     *            a boolean
     */
    public static void cloneAndCloseCurrentTab(SciNotes ed, boolean b, int x, int y) {
        cloneAndCloseCurrentTab(ed, b);
        editor.getSwingParentWindow().setLocation(x, y);
    }

    /**
     * Execute after when the restoration is finished
     */
    @Override
    public void endedRestoration() {
        if (!SwingUtilities.isEventDispatchThread()) {
            try {
                SwingUtilities.invokeAndWait(new Runnable() {

                    @Override
                    public void run() {
                        restorePreviousSession();
                    }
                });
            } catch (InterruptedException e) {
                Logger.getLogger(SciNotes.class.getName()).severe(e.toString());
                throw new RuntimeException(e);
            } catch (InvocationTargetException e) {
                Logger.getLogger(SciNotes.class.getName()).severe(e.toString());
                throw new RuntimeException(e);
            }
        } else {
            restorePreviousSession();
        }
    }

    /**
     * Restore the previous session
     */
    public void restorePreviousSession() {
        restored = true;
        if (!SciNotesOptions.getSciNotesPreferences().restartOpen || !ConfigSciNotesManager.getRestoreOpenedFiles() || ConfigSciNotesManager.countExistingOpenFiles(getUUID()) == 0) {
            if (getTabPane().getTabCount() == 0) {
                openFile(null, 0, null);
            }

            WindowsConfigurationManager.restorationFinished(this);

            return;
        }

        addRestoreTab();
        WindowsConfigurationManager.restorationFinished(SciNotes.this);
    }

    /**
     * Create SciNotes instance
     *
     * @return the instance
     */
    public static void launchSciNotes() {
        if (editor == null) {
            List<String> uuids = ConfigSciNotesManager.getEditorsUUID();
            boolean success = false;
            if (!uuids.isEmpty()) {
                for (String uuid : uuids) {
                    boolean exists = false;
                    for (SciNotes ed : scinotesList) {
                        if (ed.uuid.toString().equals(uuid)) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        success = WindowsConfigurationManager.restoreUUID(uuid);
                        if (success) {
                            break;
                        }
                        ConfigSciNotesManager.removeEditorUUID(uuid);
                    }
                }
            }
            if (!success) {
                editor = SciNotesTab.getEditorInstance(null);
                editor.setParentWindow();
                ConfigSciNotesManager.saveEditorUUID(editor.getPersistentId());
                SciNotesGUI.init(editor.getParentWindow(), editor, SCINOTES);
                WindowsConfigurationManager.unregisterEndedRestoration(editor);
            }
        }

        if (!editor.restored) {
            editor.restorePreviousSession();
        }
    }

    /**
     * Open a file.
     *
     * This method *must* be called on the EDT thread.
     *
     * @param filePath
     *            the file path (may be null)
     * @param lineNumber
     *            the line number (use 0 for unspecified)
     * @param option
     *            the options (may be null)
     */
    public void openFile(String filePath, int lineNumber, String option) {
        if (!SwingUtilities.isEventDispatchThread()) {
            throw new RuntimeException("called outside the EDT thread.");
        }

        if (filePath == null) {
            addEmptyTab();
            activeRestoreTab();
            return;
        }

        File f = fileToCanonicalFile(new File(filePath));
        if (!f.getParentFile().exists()) {
            JOptionPane.showMessageDialog(SciNotes.this, SciNotesMessages.OPEN_ERROR);
            addEmptyTab();
            activeRestoreTab();
            return;
        }

        if (f.isDirectory()) { /* Bug 5131 */
            ConfigManager.saveLastOpenedDirectory(f.getPath());
            addEmptyTab();
            activeRestoreTab();
            return;
        }

        readFileAndWait(f);
        if (option == null || option.length() == 0 || "readonly".equals(option.toLowerCase())) {
            getTextPane().scrollTextToLineNumber(lineNumber, true);
        } else {
            getTextPane().scrollTextToLineNumberInWhereami(lineNumber, option, true);
        }
        if ((option != null && "readonly".equals(option.toLowerCase()))) {
            getTextPane().setReadOnly(true);
            getInfoBar().setText(getTextPane().getInfoBarText());
            ConfigSciNotesManager.removeFromOpenFiles(this, getTextPane());
        } else {
            ConfigSciNotesManager.saveToRecentOpenedFiles(filePath);
            RecentFileAction.updateRecentOpenedFilesMenu(this);
        }

        activeRestoreTab();
    }

    public void activeRestoreTab() {
        if (getTextPane(0) == null) {
            tabPane.setSelectedIndex(0);
            getInfoBar().setText("");
        }
    }

    /**
     * Close SciNotes instance including all tabs.
     */
    public void closeSciNotes() {
        for (int i = 0; i < getTabPane().getTabCount(); i++) {
            ScilabEditorPane textPaneAt = getTextPane(i);
            if (textPaneAt != null) {
                textPaneAt.destroy();
            }
        }

        FindAction.close();
        IncrementalSearchAction.close(this);
        RecentFileAction.close(this);
        OpenSourceFileOnKeywordAction.closeOpenSourceWindow();
        SearchWordInFilesAction.closeWindow();

        /*
         * setProtectOpenFileList(true); while (getTabPane().getTabCount() > 0)
         * { closeTabAt(0, true); } setProtectOpenFileList(false);
         */

        scinotesList.remove(this);
        if (scinotesList.size() == 0) {
            SciNotesAutosave.stopAutosave();
            ScilabEditorPane.clean();
            RegisterFavoriteDirsAction.close();
            OpenSourceFileOnKeywordAction.close();
            EncodingAction.close();
            EndOfLineAction.close();
            HelpOnTypingManager.close();
        }

        editor = null;
        SciNotesGUI.clean(this);
        close();
        ConfigSciNotesManager.resetDocument();
    }

    public boolean canClose() {
        int numberOfTab = getTabPane().getTabCount();
        setProtectOpenFileList(true);

        for (int i = 0; i < numberOfTab; i++) {
            boolean ans = checkToCloseTabAt(i);
            if (!ans) {
                setProtectOpenFileList(false);
                return false;
            }
        }
        setProtectOpenFileList(false);

        return true;
    }

    public String askForClosing() {
        int numberOfTab = getTabPane().getTabCount();
        boolean isContentModified = false;
        for (int i = 0; i < getTabPane().getTabCount(); i++) {
            ScilabEditorPane textPaneAt = getTextPane(i);
            if (textPaneAt != null && ((ScilabDocument) textPaneAt.getDocument()).isContentModified()) {
                isContentModified = true;
                break;
            }
        }

        if (isContentModified) {
            return "SciNotes";
        }

        return null;
    }

    public static void closeEditor(SciNotes ed) {
        if (scinotesList.size() > 1) {
            ClosingOperationsManager.startClosingOperationWithoutSave((SwingScilabDockablePanel) ed);
        } else {
            ClosingOperationsManager.startClosingOperation((SwingScilabDockablePanel) ed);
        }
    }

    /**
     * Close a tab using its index.
     *
     * @param indexTab
     *            the index of the tab
     * @return if the tab has been really closed
     */
    public boolean closeTabAtWithoutConfirmation(int indexTab) {
        return closeTabAt(indexTab, false, false);
    }

    /**
     * Close a tab using its index.
     *
     * @param indexTab
     *            the index of the tab
     * @return if the tab has been really closed
     */
    public boolean closeTabAt(int indexTab) {
        return closeTabAt(indexTab, false, true);
    }

    /**
     * Close a tab using its index.
     *
     * @param indexTab
     *            the index of the tab
     * @param scilabClose
     *            if true, save dialog confirmation cannot be canceled
     * @return if the tab has been really closed
     */
    public boolean closeTabAt(int indexTab, boolean scilabClose, boolean confirmation) {
        ScilabEditorPane textPaneAt = getTextPane(indexTab);

        /*
         * Test for modification added after bug 5103 fix: do not ask the user
         * for an Untitled not-modified file saving when closing SciNotes
         */
        if (confirmation && ((ScilabDocument) textPaneAt.getDocument()).isContentModified()) {
            if (!save(indexTab, false, scilabClose)) {
                return false;
            }
        }

        textPaneAt.close();

        if (textPaneAt.getName() == null) {
            String closedTabName = tabPane.getScilabTitleAt(indexTab);
            String closedTabNameIndex = closedTabName.substring(closedTabName.length() - 1, closedTabName.length());
            tabList.remove(Integer.valueOf(closedTabNameIndex));
            closedTabList.add(Integer.valueOf(closedTabNameIndex));
        }

        // correction for bug 5404, closing the last tabPane generate an
        // exception
        // that's why we need to remove ChangeListeners before closing
        if ((indexTab == 0) && (getTabPane().getTabCount() == 1)) {
            for (int i = 0; i < tabPane.getChangeListeners().length; i++) {
                tabPane.removeChangeListener(tabPane.getChangeListeners()[i]);
            }
        }

        // Remove the filename associated with the tab from the list of open
        // files.
        if (!protectOpenFileList) {
            ConfigSciNotesManager.removeFromOpenFiles(this, textPaneAt);
        }

        if (navigator != null) {
            navigator.removePane(textPaneAt);
        }

        tabPane.remove(indexTab);

        textPaneAt = getTextPane();
        if (textPaneAt != null) {
            if (navigator != null) {
                navigator.update((ScilabDocument) textPaneAt.getDocument());
            }
        }
        return true;
    }

    public boolean checkToCloseTabAt(int index) {
        ScilabEditorPane textPaneAt = getTextPane(index);

        /*
         * Test for modification added after bug 5103 fix: do not ask the user
         * for an Untitled not-modified file saving when closing SciNotes
         */
        if (textPaneAt != null && ((ScilabDocument) textPaneAt.getDocument()).isContentModified()) {
            if (!save(index, false, false)) {
                return false;
            }
        }

        return true;
    }

    /**
     * Set or unset protection of the currently open file list. Call before
     * closing tabs if the tabs should be restored when scinotes restarts.
     *
     * @param protect
     *            Enables protection of the open file list if true.
     */
    public void setProtectOpenFileList(boolean protect) {
        protectOpenFileList = protect;
    }

    /**
     * Determines if this editor is the only editor instance.
     *
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
     *
     * @param indexTab
     *            the textPane index containing the file contents
     * @return execution status
     */
    public boolean save(int indexTab) {
        return save(indexTab, false, false);
    }

    /**
     * Save a file.
     *
     * @param indexTab
     *            the textPane index containing the file contents
     * @param force
     *            : force save without confirmation dialog ( SaveAction )
     * @return execution status
     */
    public boolean save(int indexTab, boolean force) {
        return save(indexTab, force, false);
    }

    /**
     * Save a file.
     *
     * @param indexTab
     *            the textPane index containing the file contents
     * @param force
     *            : force save without confirmation dialog ( SaveAction )
     * @param scilabClose
     *            : if true, save dialog confirmation cannot be canceled
     * @return execution status
     */
    public boolean save(int indexTab, boolean force, boolean scilabClose) {
        ScilabEditorPane textPaneAt = getTextPane(indexTab);
        getTabPane().setSelectedIndex(indexTab);

        // if the file ( empty, new or loaded ) is not modified, exit save
        // process and return true
        if (!textPaneAt.checkExternalModif() && !((ScilabDocument) textPaneAt.getDocument()).isContentModified() && (textPaneAt.getName() != null)) {
            /*
             * Bug
             * 5103
             * fix
             */
            return true;
        }

        if (!force) {
            AnswerOption answer;
            if (scilabClose) {
                answer = ScilabModalDialog.show(this, String.format(SciNotesMessages.MODIFIED, textPaneAt.getShortName()), SciNotesMessages.SCILAB_EDITOR,
                                                IconType.QUESTION_ICON, ButtonType.YES_NO);
            } else {
                answer = ScilabModalDialog.show(this, String.format(SciNotesMessages.MODIFIED, textPaneAt.getShortName()), SciNotesMessages.SCILAB_EDITOR,
                                                IconType.QUESTION_ICON, ButtonType.YES_NO_CANCEL);
            }

            switch (answer) {
                case YES_OPTION: // Yes, continue
                    break;
                case NO_OPTION:// No, exit and returns true
                    return true;
                case CANCEL_OPTION: // Cancel, exit and return false
                    return false;
                default:
                    break;
            }
        }

        String fileToSave = textPaneAt.getName();
        if (fileToSave == null) {
            // need a filename, call chooseFileToSave
            fileToSave = chooseFileToSave(SciNotesMessages.SAVE);
        } else {
            // check if the file has been modified by external software
            fileToSave = checkExternalModification(fileToSave);
        }

        if (fileToSave == null || fileToSave.length() == 0) {
            return true; /*
                          * Bug 5189: The user cancels ==> do not want an error
                          * message
                          */
        }

        File newSavedFile = new File(fileToSave);

        if (!SaveFile.doSave(textPaneAt, indexTab, newSavedFile, editorKit)) {
            return false;
        }

        ScilabDocument styledDocument = (ScilabDocument) textPaneAt.getDocument();
        styledDocument.setContentModified(false);

        textPaneAt.setLastModified(newSavedFile.lastModified());

        if (textPaneAt.getName() == null) {
            String name = getTabPane().getScilabTitleAt(indexTab);
            String index = name.substring(name.length() - 1, name.length());
            tabList.remove(Integer.valueOf(index));
            closedTabList.add(Integer.valueOf(index));
        }

        textPaneAt.setName(fileToSave);
        getTabPane().setTitleAt(indexTab, newSavedFile.getName());

        setTitle(textPaneAt.getTitle());
        ConfigSciNotesManager.saveToOpenFiles(fileToSave, this, textPaneAt);

        return true;
    }

    /**
     * Overwrite the file at a given index.
     *
     * @param indexTab
     *            the textPane index containing the file contents
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

        if (!SaveFile.doSave(textPaneAt, indexTab, newSavedFile, editorKit)) {
            return false;
        }

        ScilabDocument styledDocument = (ScilabDocument) textPaneAt.getDocument();
        styledDocument.setContentModified(false);

        getTabPane().setTitleAt(getTabPane().getSelectedIndex(), newSavedFile.getName());

        // Get current file path for Execute file into Scilab
        getTextPane().setLastModified(newSavedFile.lastModified());

        textPaneAt.setName(fileToSave);
        setTitle(textPaneAt.getTitle());

        return true;
    }

    /**
     * Check for external modification. If it is the case, propose to select a
     * new file name.
     *
     * @param filename
     *            The name of the file
     * @return the filename where to save
     */
    public String checkExternalModification(String filename) {
        File newSavedFile = new File(filename);
        if (newSavedFile.lastModified() > getTextPane().getLastModified()) {
            if (ScilabModalDialog.show(this, String.format(SciNotesMessages.EXTERNAL_MODIFICATION, newSavedFile.getPath()),
                                       SciNotesMessages.REPLACE_FILE_TITLE, IconType.QUESTION_ICON, ButtonType.YES_NO) == AnswerOption.NO_OPTION) {
                return chooseFileToSave(SciNotesMessages.SAVE);
            }
        }
        return filename;
    }

    /**
     * Return through a file selector the name of the selected file.
     *
     * @param title
     *            the title for JFileChooser
     * @return the file picked up by the user
     */
    public String chooseFileToSave(String title) {
        return chooseFileToSave(title, null);
    }

    /**
     * Return through a file selector the name of the selected file.
     *
     * @param title
     *            the title for JFileChooser
     * @param path
     *            the path where to open the filechooser
     * @return the file picked up by the user
     */
    public String chooseFileToSave(String title, String path) {
        String extension = new String();

        String initialDirectoryPath = path;
        if (initialDirectoryPath == null) {
            initialDirectoryPath = getTextPane().getName();
        }
        if (initialDirectoryPath == null) {
            if (firstOpen) {
                initialDirectoryPath = CommonFileUtils.getCWD();
                firstOpen = false;
            } else {
                initialDirectoryPath = ConfigManager.getLastOpenedDirectory();
            }
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

        String name = initialDirectoryPath;
        File tempFile = new File(name);

        // Select default file type
        SciFileFilter fileFilter = sceFilter;
        // Look for a supported extension
        for (FileFilter filter : fileChooser.getChoosableFileFilters()) {
            if (((SciFileFilter) filter).accept(tempFile)) {
                fileFilter = (SciFileFilter) filter;
                break;
            }
        }
        fileChooser.setFileFilter(fileFilter);
        fileChooser.setTitle(title);

        if (name == null) {
            name = ((ScilabDocument) getTextPane().getDocument()).getFirstFunctionName();
            if (name != null) {
                name += SCI_EXTENSION;
            }
        }

        if (name != null) {
            fileChooser.setSelectedFile(new File(name));
        }

        int retval = fileChooser.showSaveDialog(this);

        if (retval == JFileChooser.APPROVE_OPTION) {
            File f = fileToCanonicalFile(fileChooser.getSelectedFile());
            initialDirectoryPath = f.getPath();
            if (f.exists()) {
                if (ScilabModalDialog.show(this, SciNotesMessages.REPLACE_FILE_TITLE, SciNotesMessages.FILE_ALREADY_EXIST, IconType.QUESTION_ICON,
                                           ButtonType.YES_NO) == AnswerOption.NO_OPTION) {
                    return chooseFileToSave(SciNotesMessages.SAVE);
                }
            }

            /* we test if the file has already a scilab extension */
            boolean hasNoExtension = true;

            // if the file name is like this : any character , a dot , then
            // 2,3or 4 characters, then
            // we consider the file has already an extension
            // we previously only check for .sci and .sce extension, but what if
            // the user open a txt file
            String fileName = f.getName();
            if (fileName.lastIndexOf(DOT) != -1) {
                if (fileName.substring(fileName.lastIndexOf(DOT), fileName.length()).length() >= 2
                        && fileName.substring(fileName.lastIndexOf(DOT), fileName.length()).length() <= 4) {
                    hasNoExtension = false;
                }
            }

            /* if no extension , we add it */
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

            if (initialDirectoryPath != null) {
                ConfigManager.saveLastOpenedDirectory(initialDirectoryPath);
            }

            return f.getPath();
        } else if (retval == JFileChooser.CANCEL_OPTION) {
            return "";
        }

        return null;
    }

    /**
     * Save a file.
     *
     * @param path
     *            the initial path where the filechooser will be open
     * @return execution status
     */
    public boolean saveAs(String path) {
        String filename = chooseFileToSave(SciNotesMessages.SAVE_AS, path);
        if (filename == null || filename.length() == 0) {
            return true;
        }

        File f = new File(filename);
        ScilabDocument styledDocument = (ScilabDocument) getTextPane().getDocument();

        if (!SaveFile.doSave(getTextPane(), getTabPane().getSelectedIndex(), f, editorKit)) {
            return false;
        }

        if (getTextPane().getName() == null) {
            String name = getTabPane().getScilabTitleAt(getTabPane().getSelectedIndex());
            String index = name.substring(name.length() - 1, name.length());
            tabList.remove(Integer.valueOf(index));
            closedTabList.add(Integer.valueOf(index));
        }

        firstOpen = false;
        ConfigManager.saveLastOpenedDirectory(f.getPath());
        ConfigSciNotesManager.saveToRecentOpenedFiles(f.getPath());
        ConfigSciNotesManager.renameOpenFilesItem(f.getPath(), this, getTextPane());
        getTextPane().setName(f.getPath());
        getTabPane().setTitleAt(getTabPane().getSelectedIndex(), f.getName());
        setTitle(getTextPane().getTitle());

        RecentFileAction.updateRecentOpenedFilesMenu(this);

        if (navigator != null) {
            navigator.update(styledDocument);
        }

        styledDocument.setContentModified(false);
        getTextPane().setLastModified(f.lastModified());
        getTextPane().setReadOnly(false);
        getInfoBar().setText(getTextPane().getInfoBarText());

        return true;
    }

    /**
     * Create a new tab in SciNotes.
     *
     * @param title
     *            the title of the tab
     * @return the text component inside the tab
     */
    public ScilabEditorPane addTab(String title) {
        return addTab(title, Integer.MAX_VALUE);
    }

    /**
     * Create a new tab in SciNotes.
     *
     * @param title
     *            the title of the tab
     * @param index
     *            the index where to put the new tab
     * @return the text component inside the tab
     */
    public ScilabEditorPane addTab(String title, int index) {
        return addTab(title, index, 0);
    }

    /**
     * Create a new tab in SciNotes.
     *
     * @param title
     *            the title of the tab
     * @param index
     *            the index where to put the new tab
     * @return the text component inside the tab
     */
    public ScilabEditorPane addTab(String title, int index, int caretPos) {
        ScilabEditorPane sep = new ScilabEditorPane(this);
        initPane(sep);
        int ind = Math.min(Math.max(0, index), tabPane.getTabCount());
        tabPane.insertTab(title, null, sep.getEditorComponent(), "", ind);
        tabPane.setSelectedIndex(ind);
        initInputMap(sep);
        updateTabTitle();
        getInfoBar().setText(sep.getInfoBarText());
        repaint();
        sep.init(caretPos);
        return sep;
    }

    public void addRestoreTab() {
        try {
            JPanel panel = RestoreOpenedFilesAction.getTab(this, getUUID().toString());
            if (panel != null) {
                tabPane.addTab("Restore", panel);
            }
        } catch (Exception e) {
            System.err.println(e);
        }
    }

    /**
     * Init a pane
     *
     * @param pane
     *            the pane to init
     */
    public void initPane(ScilabEditorPane pane) {
        initPane(pane, !SciNotesOptions.getSciNotesDisplay().wrapLines);
    }

    /**
     * Init the EditorPane.
     *
     * @param pane
     *            the EditorPane
     * @param plain
     *            true for a plain view or false for a wrapped view
     */
    public void initPane(ScilabEditorPane pane, boolean plain) {
        setHighlight(pane);
        ScilabEditorKit kit = new ScilabEditorKit(plain);
        pane.setEditorKit(kit);

        // Panel of line number for the text pane
        pane.getXln().setWhereamiLineNumbering(SciNotesOptions.getSciNotesDisplay().showLineNumbers, SciNotesOptions.getSciNotesDisplay().whereami);
        activateHelpOnTyping(pane);

        pane.setBorder(BorderFactory.createEmptyBorder(2, 2, 2, 2));
        pane.setFont(kit.getStylePreferences().tokenFonts[0]);

        pane.setBackground(SciNotesOptions.getSciNotesDisplay().backgroundColor);
        pane.setCaretColor(SciNotesOptions.getSciNotesDisplay().caretColor);

        pane.setFocusable(true);
        pane.setRequestFocusEnabled(true);
        pane.setDragEnabled(true); /* Bug 5497 */

        DropFilesListener dndTarget = new DropFilesListener(pane);

        pane.setComponentPopupMenu(SciNotesGUI.generateRightClickPopup(this));
    }

    /**
     * Init the EditorPane with the InputMap found in ConfigSciNotesManager
     *
     * @param pane
     *            the EditorPane
     */
    public void initInputMap(ScilabEditorPane pane) {
        setKeyStrokeAction(pane, this);
        LineBeautifierAction.putInInputMap(pane);
        InsertOverwriteAction.putInInputMap(pane);
    }

    /**
     * Split the EditorPane
     *
     * @param vertical
     *            true for a vertical split
     */
    public void splitTab(boolean vertical) {
        ScilabEditorPane pane = getTextPane();
        Component bottom = pane.getEditorComponent().getBottom();
        int state = pane.getXln().getState();
        ScilabEditorPane leftPane = new ScilabEditorPane(editor);
        ScilabEditorPane rightPane = new ScilabEditorPane(editor);
        if (navigator != null) {
            navigator.changePaneOnSplit(pane, leftPane);
        }
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
        leftPane.getXln().setWhereamiLineNumbering(state);
        rightPane.getXln().setWhereamiLineNumbering(state);
        split.setLeftComponent(leftPane.getScrollPane());
        split.setRightComponent(rightPane.getScrollPane());
        split.setResizeWeight(0.5);
        rightPane.setEditorComponent(leftPane.getEditorComponent());
        leftPane.setSplitPane(split);
        rightPane.setSplitPane(split);

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
        tabPane.setComponentAt(tabPane.getSelectedIndex(), leftPane.getEditorComponent());
        leftPane.getEditorComponent().insertBottomComponent(bottom);
        leftPane.requestFocus();
    }

    /**
     * Remove a split
     */
    public void removeSplit() {
        if (((EditorComponent) tabPane.getSelectedComponent()).isSplited()) {
            ScilabEditorPane pane = new ScilabEditorPane(editor);
            ScilabEditorPane textpane = getTextPane();
            Component bottom = textpane.getEditorComponent().getBottom();
            if (navigator != null) {
                navigator.changePaneOnSplit(textpane, pane);
            }
            initPane(pane);
            textpane.setOtherPaneInSplit(null);
            textpane.copyProps(pane);
            ScilabDocument doc = (ScilabDocument) textpane.getDocument();
            pane.setDocument(doc);
            pane.setCaretPosition(0);
            activateHelpOnTyping(pane);
            tabPane.setComponentAt(tabPane.getSelectedIndex(), pane.getEditorComponent());
            initInputMap(pane);
            if (doc.getBinary()) {
                pane.setBinary(true);
            }
            getInfoBar().setText(pane.getInfoBarText());
            updateTabTitle();
            pane.getEditorComponent().insertBottomComponent(bottom);
            pane.requestFocus();
        }
    }

    /**
     * Create an empty tab inside SciNotes.
     *
     * @return the text component inside the tab
     */
    public ScilabEditorPane addEmptyTab() {
        ScilabEditorPane sep;
        int n = getNumberForEmptyTab();
        sep = addTab(SciNotesMessages.UNTITLED + n);
        sep.setShortName(SciNotesMessages.UNTITLED + n);
        sep.setTitle(SciNotesMessages.UNTITLED + n);
        setTitle(sep.getTitle());
        SciNotesOptions.Header header = SciNotesOptions.getSciNotesHeader();
        if (header.header != null) {
            ((ScilabDocument) sep.getDocument()).disableUndoManager();
            try {
                sep.getDocument().insertString(0, header.header, null);
            } catch (BadLocationException e) { }
            sep.init(header.header.length());
            ((ScilabDocument) sep.getDocument()).setContentModified(false);
            ((ScilabDocument) sep.getDocument()).enableUndoManager();
        }

        return sep;
    }

    /**
     * Get an integer used for untitled document
     */
    public int getNumberForEmptyTab() {
        if (closedTabList.size() > 0) {
            Integer n = Collections.min(closedTabList);
            closedTabList.remove(n);
            return n.intValue();
        } else {
            numberOfUntitled++;
            tabList.add(Integer.valueOf(numberOfUntitled));
            return numberOfUntitled;
        }
    }

    /**
     * Add or remove '*' prefix in current tab tile according to
     * isContentModified().
     */
    public void updateTabTitle() {
        updateTabTitle(getTabPane().getSelectedIndex());
    }

    /**
     * Add or remove '*' prefix in tab title (corresponding to the given index) according to
     * isContentModified().
     */
    public void updateTabTitle(int index) {
        StringBuffer newTitle = new StringBuffer();
        ScilabEditorPane currentTextPane = getTextPane(index);
        if (((ScilabDocument) currentTextPane.getDocument()).isContentModified()) {
            newTitle.append('*');
        }

        String textPaneName = currentTextPane.getName();
        try {
            File f = new File(textPaneName);
            newTitle.append(f.getName());
        } catch (NullPointerException e) { // not a file name, no path prefix to
            // remove, but maybe a '*'
            textPaneName = getTabPane().getScilabTitleAt(index);
            if (textPaneName.charAt(0) == '*') {
                newTitle.append(textPaneName.substring(1, textPaneName.length()));
            } else {
                newTitle.append(textPaneName);
            }
        }
        getTabPane().setTitleAt(index, newTitle.toString());
    }

    /**
     * @param button
     *            the UndoButton used in this editor
     */
    public void setUndoButton(JButton button) {
        undoButton = button;
        enableUndoButton(false);
    }

    /**
     * @param b
     *            true to enable the button
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
     * @param button
     *            the RedoButton used in this editor
     */
    public void setRedoButton(JButton button) {
        redoButton = button;
        enableRedoButton(false);
    }

    /**
     * @param b
     *            true to enable the button
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
     *
     * @param index
     *            the index
     */
    public void reload(int index) {
        ScilabEditorPane textPaneAt = getTextPane(index);
        if (textPaneAt != null && textPaneAt.getName() != null) {
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
        }
    }

    /**
     * Load a file inside SciNotes.
     *
     * @param f
     *            the file to open
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
        }
    }

    /**
     * Load a file inside SciNotes.
     *
     * @param file
     *            the file to open
     */
    public void readFileAndWait(File file) {
        /** Is this file already opened */
        File f = file;
        boolean alreadyOpened = false;
        int index = -1;
        for (int i = 0; i < tabPane.getTabCount(); i++) {
            ScilabEditorPane textPaneAt = getTextPane(i);
            if (textPaneAt != null && f.getAbsolutePath().equals(textPaneAt.getName())) {
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

                    String[] labels = new String[] { SciNotesMessages.RELOAD, SciNotesMessages.OVERWRITE, SciNotesMessages.IGNORE };
                    messageBox.setButtonsLabels(labels);

                    messageBox.setIcon("question"); // Question icon

                    messageBox.setParentForLocation(this); // Centered on
                    // SciNotes main
                    // window

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
        }
    }

    public int getTextPaneIndex(ScilabEditorPane sep) {
        int n = tabPane.getTabCount();
        for (int i = 0; i < n; ++i) {
            ScilabEditorPane pane = getTextPane(i);
            if (pane != null && (pane == sep || pane.getOtherPaneInSplit() == sep)) {
                return i;
            }
        }

        return -1;
    }

    /**
     * Get current text component. If the window is splitted, then return the
     * focused text component.
     *
     * @return the text component
     */
    public ScilabEditorPane getTextPane() {
        try {
            if (tabPane.getSelectedComponent() instanceof EditorComponent) {
                EditorComponent c = (EditorComponent) tabPane.getSelectedComponent();
                ScilabEditorPane pane = c.getEditorPane();
                if (ScilabEditorPane.getFocusedPane() == pane.getOtherPaneInSplit()) {
                    return pane.getOtherPaneInSplit();
                }

                return pane;
            }
        } catch (NullPointerException e) {
        } catch (ArrayIndexOutOfBoundsException e) {
        }

        return null;
    }

    /**
     * Get text component at index.
     *
     * @param index
     *            the index of the textpane
     * @return the text component
     */
    public ScilabEditorPane getTextPane(int index) {
        try {
            if (tabPane.getComponentAt(index) instanceof EditorComponent) {
                EditorComponent c = (EditorComponent) tabPane.getComponentAt(index);
                ScilabEditorPane pane = c.getEditorPane();
                if (ScilabEditorPane.getFocusedPane() == pane.getOtherPaneInSplit()) {
                    return pane.getOtherPaneInSplit();
                }

                return pane;
            }
        } catch (NullPointerException e) {
        } catch (ArrayIndexOutOfBoundsException e) {
        }

        return null;
    }

    /**
     * Set the keystroke actions
     */
    public static void setAllMenus() {
        for (SciNotes ed : scinotesList) {
            SciNotesGUI.reinitMenus(ed);
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = ed.getTextPane(i);
                sep.setComponentPopupMenu(SciNotesGUI.generateRightClickPopup(ed));
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().setComponentPopupMenu(SciNotesGUI.generateRightClickPopup(ed));
                }
            }
        }
    }

    /**
     * Set the keystroke actions
     */
    public static void setKeyStrokeActions() {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = ed.getTextPane(i);
                ed.initInputMap(sep);
                if (sep.getOtherPaneInSplit() != null) {
                    ed.initInputMap(sep.getOtherPaneInSplit());
                }
            }
        }
    }

    /**
     * Enable the whereami-line numbering
     *
     * @param state
     *            int
     */
    public static void setWhereamiLineNumbering() {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = ed.getTextPane(i);
                sep.getXln().setWhereamiLineNumbering(SciNotesOptions.getSciNotesDisplay().showLineNumbers, SciNotesOptions.getSciNotesDisplay().whereami);
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().getXln().setWhereamiLineNumbering(SciNotesOptions.getSciNotesDisplay().showLineNumbers, SciNotesOptions.getSciNotesDisplay().whereami);
                }
            }
            ed.repaint();
        }
    }

    /**
     * Auto-indent mode management
     *
     * @param b
     *            true to activate auto-indent mode
     */
    public static void setAutoIndent() {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = ed.getTextPane(i);
                ((ScilabDocument) sep.getDocument()).setAutoIndent(SciNotesOptions.getSciNotesDisplay().automaticIndent);
                if (sep.getOtherPaneInSplit() != null) {
                    ((ScilabDocument) sep.getOtherPaneInSplit().getDocument()).setAutoIndent(SciNotesOptions.getSciNotesDisplay().automaticIndent);
                }
            }
        }
    }

    /**
     * Horizontal Wrap mode management
     *
     * @param b
     *            true to activate horizontal wrapping mode
     */
    public static void setHorizontalWrap() {
        if (SciNotesOptions.getSciNotesDisplay().wrapLines != mustWrapLines) {
            mustWrapLines = SciNotesOptions.getSciNotesDisplay().wrapLines;
            for (SciNotes ed : scinotesList) {
                int n = ed.getTabPane().getTabCount();
                for (int i = 0; i < n; i++) {
                    ScilabEditorPane sep = ed.getTextPane(i);
                    if (sep.getOtherPaneInSplit() == null) {
                        ScilabEditorPane pane = new ScilabEditorPane(editor);
                        ed.initPane(pane, !mustWrapLines);
                        sep.copyProps(pane);
                        pane.setDocument(sep.getDocument());
                        pane.setCaretPosition(sep.getCaretPosition());
                        pane.getXln().setWhereamiLineNumbering(SciNotesOptions.getSciNotesDisplay().showLineNumbers, SciNotesOptions.getSciNotesDisplay().whereami);
                        ed.tabPane.setComponentAt(i, pane.getEditorComponent());
                        SciNotes.activateHelpOnTyping(pane);
                        ed.initInputMap(pane);
                        if (((ScilabDocument) sep.getDocument()).getBinary()) {
                            pane.setBinary(true);
                        }
                        ed.getInfoBar().setText(pane.getInfoBarText());
                    }
                }
            }
        }
    }

    /**
     * Set a line numbering compatible with the whereami function
     * @param state 0 for nothing, 1 for normal and 2 for whereami
     */

    /**
     * Enable the highlighted line in this editor
     *
     * @param b
     *            boolean
     */
    public static void enableHighlightedLine(boolean b) {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = ed.getTextPane(i);
                sep.enableHighlightedLine(SciNotesOptions.getSciNotesDisplay().highlightCurrentLine);
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().enableHighlightedLine(SciNotesOptions.getSciNotesDisplay().highlightCurrentLine);
                }
            }
        }
    }

    /**
     * Enable the help on typing in the current textPane
     *
     * @param pane
     *            the pane
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
                ScilabEditorPane sep = ed.getTextPane(i);
                sep.activateHelpOnTyping();
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().activateHelpOnTyping();
                }
            }
        }
    }

    /**
     * Set the color of the highlighted line in this editor
     *
     * @param c
     *            Color
     */
    public static void setDefaultTabulation() {
        TabManager.Tabulation tab = new TabManager.Tabulation();
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = ed.getTextPane(i);
                sep.getTabManager().setTabulation(tab);
                View view = ((ScilabDocument) sep.getDocument()).getView();
                if (view != null) {
                    if (view instanceof ScilabView) {
                        ((ScilabView) view).setTabRepresentation(tab);
                        ((ScilabView) view).reinitialize();
                    } else {
                        ((ScilabPlainView) view).setTabRepresentation(tab);
                        ((ScilabPlainView) view).reinitialize();
                    }
                }
            }
        }
    }

    /**
     * Set the color of the highlighted line in this editor
     *
     * @param c
     *            Color
     */
    public static void updatePanes(SciNotesConfiguration.Conf conf) {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = ed.getTextPane(i);
                sep.configurationChanged(conf);
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().configurationChanged(conf);
                }
                sep.repaint();
            }
        }
    }

    public static void updateFontSize(int inc) {
        Font baseFont = null;
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = ed.getTextPane(i);
                ((ScilabEditorKit) sep.getEditorKit()).getStylePreferences().changeBaseFontSize(inc);
                if (baseFont == null) {
                    baseFont = ((ScilabEditorKit) sep.getEditorKit()).getStylePreferences().getBaseFont();
                }
                if (sep.getOtherPaneInSplit() != null) {
                    ((ScilabEditorKit) sep.getOtherPaneInSplit().getEditorKit()).getStylePreferences().changeBaseFontSize(n);
                    sep.getOtherPaneInSplit().resetFont();
                }
                sep.resetFont();
            }
        }

        if (baseFont != null) {
            ScilabContext.saveFont(baseFont);
        }
    }

    /**
     * Set the color of the highlighted line in this editor
     *
     * @param c
     *            Color
     */
    public static void setHighlightedLineColor() {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = ed.getTextPane(i);
                sep.setHighlightedLineColor(SciNotesOptions.getSciNotesDisplay().currentLineColor);
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().setHighlightedLineColor(SciNotesOptions.getSciNotesDisplay().currentLineColor);
                }
            }
        }
    }

    /**
     * Set the color of the contour of the highlighted line in this editor
     *
     * @param c
     *            Color
     */
    public static void setHighlightedContourColor() {
        for (SciNotes ed : scinotesList) {
            int n = ed.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane sep = ed.getTextPane(i);
                sep.setHighlightedContourColor(null);
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().setHighlightedContourColor(null);
                }
            }
        }
    }

    /**
     * Set the highlighted line in this textPane
     *
     * @param sep
     *            ScilabEditorPane
     */
    public static void setHighlight(ScilabEditorPane sep) {
        sep.enableHighlightedLine(SciNotesOptions.getSciNotesDisplay().highlightCurrentLine);
        sep.setHighlightedLineColor(SciNotesOptions.getSciNotesDisplay().currentLineColor);
        sep.setHighlightedContourColor(null);
    }

    /**
     * Get SciNotes as a Tab.
     *
     * @return SciNotes instance
     * @see org.scilab.modules.gui.tab.Tab#getAsSimpleTab()
     */
    public SimpleTab getAsSimpleTab() {
        return this;
    }

    /**
     * Get SciNotes parent Window.
     *
     * @return parent Window
     * @see org.scilab.modules.gui.tab.Tab#getParentWindow()
     */
    public SwingScilabWindow getParentWindow() {
        if (parentWindow == null) {
            return super.getParentWindow();
        }
        return parentWindow;
    }

    /**
     * Get the UUID associated with the editor instance.
     *
     * @return unique identifier
     */
    public UUID getUUID() {
        return uuid;
    }

    /**
     * Add a status bar to SciNotes.
     *
     * @param infoBarToAdd
     *            the status bar to be added
     * @see org.scilab.modules.gui.uielement.UIElement#addInfoBar(org.scilab.modules.gui.textbox.TextBox)
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        setInfoBar(infoBarToAdd);
    }

    /**
     * Add a menu bar to SciNotes.
     *
     * @param menuBarToAdd
     *            the menu bar to be added
     * @see org.scilab.modules.gui.uielement.UIElement#addMenuBar(org.scilab.modules.gui.menubar.MenuBar)
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        setMenuBar(menuBarToAdd);
    }

    /**
     * Add a tool bar to SciNotes.
     *
     * @param toolBarToAdd
     *            the tool bar to be added
     * @see org.scilab.modules.gui.uielement.UIElement#addToolBar(org.scilab.modules.gui.toolbar.ToolBar)
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        setToolBar(toolBarToAdd);
    }

    /**
     * Get SciNotes main ScilabTabbedPane.
     *
     * @return SciNotes main ScilabTabbedPane
     */
    public ScilabTabbedPane getTabPane() {
        return tabPane;
    }

    /**
     * Set SciNotes main ScilabTabbedPane.
     *
     * @param tabPane
     *            SciNotes main ScilabTabbedPane
     */
    public void setTabPane(ScilabTabbedPane tabPane) {
        this.tabPane = tabPane;
    }

    /**
     * Load a file and add it at the end
     *
     * @param f
     *            the file to load
     */
    public void loadFile(File f) {
        loadFile(f, -1);
    }

    /**
     * Load a file and add it at the index
     *
     * @param f
     *            the file to load
     * @param index
     *            the index where to put the file
     */
    public void loadFile(File f, int index) {
        ScilabDocument styleDocument = null;
        ScilabEditorPane theTextPane;

        // File exist
        if (f.exists()) {
            if (!f.canRead()) {
                ScilabModalDialog.show(this, SciNotesMessages.NOTREADABLE, SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
                if (getTabPane().getTabCount() == 0) {
                    addEmptyTab();
                }
                return;
            }

            getInfoBar().setText(SciNotesMessages.LOADING);

            if (index != -1) {
                theTextPane = addTab(f.getName(), index);
            } else {
                theTextPane = addTab(f.getName());
            }
            styleDocument = (ScilabDocument) theTextPane.getDocument();
            styleDocument.disableUndoManager();
            theTextPane.setLastModified(f.lastModified());

            try {
                styleDocument.setUpdater(false);
                boolean indentMode = styleDocument.getAutoIndent();
                styleDocument.setAutoIndent(false);
                try {
                    ((ScilabEditorKit) editorKit).read(this, f, styleDocument, 0);
                } catch (BadLocationException e) {
                    e.printStackTrace();
                }
                styleDocument.setAutoIndent(indentMode);
                styleDocument.setUpdater(true);
            } catch (IOException ioex) {
                ioex.printStackTrace();
            }

            theTextPane.setName(f.getAbsolutePath());
            getTabPane().setTitleAt(getTabPane().getSelectedIndex(), f.getName());
            setTitle(theTextPane.getTitle());
            styleDocument.setContentModified(false);
            styleDocument.enableUndoManager();

            if (styleDocument.getBinary()) {
                theTextPane.setBinary(true);
            }

            if (!f.canWrite()) {
                getTextPane().setReadOnly(true);
                JOptionPane.showMessageDialog(SciNotes.this, SciNotesMessages.READONLY);
            }

            getInfoBar().setText(theTextPane.getInfoBarText());

            EncodingAction.updateEncodingMenu((ScilabDocument) getTextPane().getDocument());

            firstOpen = false;
            ConfigManager.saveLastOpenedDirectory(f.getPath());
            ConfigSciNotesManager.saveToRecentOpenedFiles(f.getPath());
            ConfigSciNotesManager.saveToOpenFiles(f.getPath(), this, getTextPane());

            // Empty the undo Manager
            UndoManager undo = ((ScilabDocument) getTextPane().getDocument()).getUndoManager();
            undo.discardAllEdits();

            if (getTabPane().getTabCount() == 2) {
                ScilabEditorPane pane = getTextPane(0);
                if (pane != null && pane.getName() == null && !((ScilabDocument) pane.getDocument()).isContentModified()) {
                    closeTabAt(0);
                }
            }
        }
    }

    /**
     * Creates a file if it doesn't exist
     *
     * @param f
     *            the file to create
     */
    public void createNewFile(File f) {
        ScilabEditorPane theTextPane = addEmptyTab();
        ScilabDocument styleDocument = null;
        int choice = JOptionPane.showConfirmDialog(SciNotes.this, String.format(SciNotesMessages.FILE_DOESNT_EXIST, f.getName()), SCINOTES,
                     JOptionPane.YES_NO_OPTION);
        if (choice == 0) { // OK
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
                System.err.println(e);
            } catch (BadLocationException e) {
                System.err.println(e);
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
                } catch (IOException e) {
                    System.err.println(e);
                }
            }

            firstOpen = false;
            ConfigManager.saveLastOpenedDirectory(f.getPath());
            ConfigSciNotesManager.saveToRecentOpenedFiles(f.getPath());
            ConfigSciNotesManager.saveToOpenFiles(theTextPane.getName(), this, theTextPane);

            theTextPane.setName(f.getPath());
            getTabPane().setTitleAt(getTabPane().getSelectedIndex(), f.getName());
            setTitle(theTextPane.getTitle());
            RecentFileAction.updateRecentOpenedFilesMenu(this);

            styleDocument.setContentModified(false);
            styleDocument.enableUndoManager();
            theTextPane.setLastModified(f.lastModified());
        }

        getInfoBar().setText("");
    }

    /**
     * EditorKit Getter
     *
     * @return EditorKit
     */
    public EditorKit getEditorKit() {
        return editorKit;
    }

    /**
     * @param scinotes
     *            the focused editor
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
     *
     * @param editorKit
     *            EditorKit
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

                @Override
                public void run() {
                    SciNotes[] arr = scinotesList.toArray(new SciNotes[0]);
                    for (int i = 0; i < arr.length; i++) {
                        arr[i].setProtectOpenFileList(true);
                        ExitAction.doExit(arr[i]);
                    }
                    scinotesList.clear();
                }
            });
        } catch (InterruptedException e) {
            Logger.getLogger(SciNotes.class.getName()).severe(e.toString());
        } catch (InvocationTargetException e) {
            Logger.getLogger(SciNotes.class.getName()).severe(e.toString());
        }
    }

    /**
     * @param f
     *            the file
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
     * Set the shortcuts in the pane relatively to the file
     * keysConfiguration.xml
     *
     * @param sep
     *            the textpane
     * @param ed
     *            the SciNotes editor
     */
    private static void setKeyStrokeAction(ScilabEditorPane sep, SciNotes ed) {
        ScilabEditorPane s = ed.getTextPane(0);
        if (s != null && s != sep) {
            sep.setInputMap(JComponent.WHEN_FOCUSED, s.getInputMap());
            return;
        }

        Map<String, KeyStroke> map = getActionKeys();
        ClassLoader loader = ClassLoader.getSystemClassLoader();

        for (Map.Entry<String, KeyStroke> entry : map.entrySet()) {
            String actionName = entry.getKey();
            KeyStroke key = entry.getValue();
            String action = actionToName.get(actionName);
            if (key != null) {
                if (!action.equals("SciNotesCompletionAction")) {
                    String className;
                    if (action.lastIndexOf(DOT) != -1) {
                        className = action;
                    } else {
                        className = DEFAULTACTIONPATH + DOT + action;
                    }
                    try {
                        Class clazz = loader.loadClass(className);
                        Constructor constructor = clazz.getConstructor(new Class[] { String.class, SciNotes.class });
                        Object act = constructor.newInstance(new Object[] { "", ed });
                        sep.getInputMap().put(key, act);
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
                    sep.getInputMap().put(key, new SciNotesCompletionAction(sep, ed));
                }
            }
        }

        // Add default common shortcuts
        sep.getInputMap().put(KeyStroke.getKeyStroke("shift DELETE"), sep.getInputMap().get(map.get("scinotes-cut")));
        sep.getInputMap().put(KeyStroke.getKeyStroke("CUT"), sep.getInputMap().get(map.get("scinotes-cut")));
        sep.getInputMap().put(ScilabKeyStroke.getKeyStroke("OSSCKEY INSERT"), sep.getInputMap().get(map.get("scinotes-copy")));
        sep.getInputMap().put(KeyStroke.getKeyStroke("COPY"), sep.getInputMap().get(map.get("scinotes-copy")));
        sep.getInputMap().put(KeyStroke.getKeyStroke("shift INSERT"), sep.getInputMap().get(map.get("scinotes-paste")));
        sep.getInputMap().put(KeyStroke.getKeyStroke("PASTE"), sep.getInputMap().get(map.get("scinotes-paste")));
    }

    /**
     * Execute an action on file
     * @param fileName the name of the file
     * @param action the action
     */
    public static void executeAction(String fileName, ActionOnDocument action) throws IOException {
        Charset charset = null;
        try {
            charset = ScilabEditorKit.tryToGuessEncoding(new File(fileName));
        } catch (CharacterCodingException e) {
            throw new IOException(SciNotesMessages.CANNOT_GUESS_ENCODING + ": " + fileName);
        }
        FileInputStream fis = new FileInputStream(fileName);
        InputStreamReader isr = new InputStreamReader(fis, charset);
        BufferedReader reader = new BufferedReader(isr);
        ScilabDocument doc = new ScilabDocument();
        ScilabEditorKit kit = new ScilabEditorKit();
        try {
            kit.read(reader, doc, 0);
        } catch (BadLocationException e) {
            System.err.println(e);
        }

        doc.addDocumentListener(doc);
        if (!doc.getBinary()) {
            action.actionOn(doc);
        }

        reader.close();
        if (doc.isContentModified()) {
            SaveFile.doSave(doc, new File(fileName), kit);
        }
    }

    /**
     * Execute an action on file
     * @param fileName the name of the file
     * @param actionsName the actions as an array
     */
    public static boolean executeAction(String fileName, final String[] actionsName) throws IOException {
        final boolean[] hasAction = new boolean[] { false };
        ActionOnDocument action = new ActionOnDocument() {
            public void actionOn(ScilabDocument doc) throws IOException {
                for (String act : actionsName) {
                    if (act.equalsIgnoreCase("indent")) {
                        hasAction[0] = true;
                        org.scilab.modules.scinotes.actions.IndentAction.getActionOnDocument().actionOn(doc);
                    } else if (act.equalsIgnoreCase("trailing")) {
                        hasAction[0] = true;
                        org.scilab.modules.scinotes.actions.RemoveTrailingWhiteAction.getActionOnDocument().actionOn(doc);
                    } else if (act.equalsIgnoreCase("quote")) {
                        hasAction[0] = true;
                        org.scilab.modules.scinotes.actions.DoubleQuoteStringAction.getActionOnDocument().actionOn(doc);
                    }
                }
            }
        };

        executeAction(fileName, action);

        return hasAction[0];
    }

    /**
     * Execute an action on file
     * @param fileName the name of the file
     * @param acts actions separated with , or ;
     */
    public static void executeAction(String fileName, String acts) throws IOException {
        StringTokenizer toks = new StringTokenizer(acts, ",;");
        String[] actions = new String[toks.countTokens()];
        for (int i = 0; i < actions.length; i++) {
            actions[i] = toks.nextToken();
        }
        executeAction(fileName, actions);
    }

    /**
     * An interface to implement to execute an action on a document
     */
    public static interface ActionOnDocument {

        public void actionOn(ScilabDocument doc) throws IOException;
    }
}
