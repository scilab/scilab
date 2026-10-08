/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - 2011 - Calixte DENIZET
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2021 - UTC - Stéphane MOTTELET
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
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

import java.awt.Component;
import java.awt.Font;
import java.awt.Point;
import java.awt.Toolkit;
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
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.UUID;
import java.util.logging.Logger;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.EditorKit;
import javax.swing.text.View;
import javax.swing.undo.UndoManager;

import org.flexdock.docking.event.DockingEvent;
import org.scilab.modules.commons.CommonFileUtils;
import org.scilab.modules.commons.gui.ScilabGUIUtilities;
import org.scilab.modules.commons.gui.ScilabCefPanel;
import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.commons.xml.ScilabXMLUtilities;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.core.Scilab;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.filechooser.FileChooser;
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
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.scinotes.actions.EncodingAction;
import org.scilab.modules.scinotes.actions.EndOfLineAction;
import org.scilab.modules.scinotes.actions.ExitAction;
import org.scilab.modules.scinotes.actions.FindAction;
import org.scilab.modules.scinotes.actions.IncrementalSearchAction;
import org.scilab.modules.scinotes.actions.InsertOverwriteAction;
import org.scilab.modules.scinotes.actions.LineBeautifierAction;
import org.scilab.modules.scinotes.actions.OpenSourceFileOnKeywordAction;
import org.scilab.modules.scinotes.actions.RecentFileAction;
import org.scilab.modules.scinotes.actions.RegisterFavoriteDirsAction;
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
import org.w3c.dom.Document;

/**
 * Main SciNotes class.
 *
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public final class SciNotes extends SwingScilabDockablePanel {

    private static final long serialVersionUID = -6410183357490518676L;

    private static final String XPATH_SCINOTES_KEY = "//general/shortcuts/body/actions/action-folder[@name='Scinotes']/action";
    private static final String XPATH_SCINOTES_ACTION = "/map/scinotes/entry";

    private static final String SCINOTES = "SciNotes";
    private static final String SCINOTES_EDITOR_PROPERTY = "scinotes.editor";
    private static final String MONACO_EDITOR_PROPERTY_VALUE = "monaco";
    private static final String SCI_EXTENSION = ".sci";
    private static final String SCE_EXTENSION = ".sce";
    private static final String TST_EXTENSION = ".tst";
    private static final String QUIT_EXTENSION = ".quit";
    private static final String DEM_EXTENSION = ".dem";
    private static final String START_EXTENSION = ".start";
    private static final List<String> SCILAB_EXTENSIONS = Collections.unmodifiableList(Arrays.asList(
                SCI_EXTENSION, SCE_EXTENSION, TST_EXTENSION, QUIT_EXTENSION, DEM_EXTENSION, START_EXTENSION));
    private static final String ALL_TST_FILES = "*.tst";
    private static final String ALL_QUIT_FILES = "*.quit";
    private static final String ALL_START_FILES = "*.start";
    private static final String ALL_SCI_FILES = "*.sci";
    private static final String ALL_SCE_FILES = "*.sce";
    private static final String ALL_DEM_FILES = "*.dem";
    private static final String ALL_SCX_FILES = "*.sci | *.sce";
    private static final String ALL_SCILAB = "all";
    private static final String ALL_FILES = "*.*";
    private static final String DOT = ".";
    private static final String XCONF_ACTION_NAME = "name";
    private static final String XCONF_ACTION_KEY = "key";

    private static final String DEFAULTACTIONPATH = "org.scilab.modules.scinotes.actions";

    private static final Map<String, String> actionToName;
    private static Map<String, KeyStroke> actionKeys;

    private static List<SciNotes> scinotesList = new ArrayList<SciNotes>();
    private static SciNotes editor;
    private static boolean mustWrapLines;

    /** Screen location requested for the next newly created SciNotes window, if any. */
    private static Point pendingWindowLocation;

    /**
     * True while a cross-window or drag-out tab transfer is tearing down its
     * source tab. Guards MonacoEditorPane against grabbing native CEF focus
     * for a sibling tab that JTabbedPane auto-selects in the source window
     * during that teardown, which would otherwise flash focus back and forth
     * between the source and destination windows.
     */
    private static volatile boolean tabTransferInProgress;

    private SwingScilabWindow parentWindow;
    private UUID uuid;

    private ScilabTabbedPane tabPane;
    private transient ChangeListener tabChangeListener;
    private final SciNotesContents contentPane;
    private NavigatorWindow navigator;
    private SearchFile searchInFiles;

    private int numberOfUntitled;
    private EditorKit editorKit;

    private JButton undoButton;
    private JButton redoButton;

    private boolean protectOpenFileList;
    private boolean restored;
    private boolean restoringSession;
    private boolean firstOpen = true;

    private final transient List<Integer> tabList = new ArrayList<Integer>();
    private final transient List<Integer> closedTabList = new ArrayList<Integer>();

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
        actionToName = XConfiguration.get(doc, XCONF_ACTION_NAME, String.class,
            "action", String.class, XPATH_SCINOTES_ACTION);
        XConfiguration.addXConfigurationListener(new SciNotesConfiguration());
    }

    /**
     * Create SciNotes instance
     */
    public SciNotes(String uuid) {
        super(SCINOTES, uuid);
        init();
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
        tabChangeListener = new ChangeListener() {
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
        };
        tabPane.addChangeListener(tabChangeListener);
        this.setContentPane(contentPane);
    }

    /**
     * Default constructor
     */
    public SciNotes() {
        this(UUID.randomUUID().toString());
    }

    void showMonacoBrowser(ScilabCefPanel browser, Component target) {
        contentPane.showBrowserOverlay(browser, target);
    }

    void showMonacoBrowserSplit(ScilabCefPanel browser, Component target, Component secondaryTarget,
            SciNotesContents.SplitLayoutListener listener) {
        contentPane.showBrowserOverlaySplit(browser, target, secondaryTarget, listener);
    }

    void removeMonacoBrowser(ScilabCefPanel browser) {
        contentPane.removeBrowserOverlay(browser);
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
            actionKeys = new HashMap<String, KeyStroke>(
                XConfiguration.get(doc, XCONF_ACTION_NAME, String.class,
                    XCONF_ACTION_KEY, KeyStroke.class, XPATH_SCINOTES_KEY));

            Map<String, KeyStroke> defaults = XConfiguration.get(XConfiguration.createDocument(),
                XCONF_ACTION_NAME, String.class, XCONF_ACTION_KEY, KeyStroke.class, XPATH_SCINOTES_KEY);
            // Older profiles keep their configured values and receive only unknown actions.
            for (Map.Entry<String, KeyStroke> entry : defaults.entrySet()) {
                if (!actionKeys.containsKey(entry.getKey())) {
                    actionKeys.put(entry.getKey(), entry.getValue());
                }
            }
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
        refreshAssistantLayout();
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
        refreshAssistantLayout();
    }

    /**
     * Force a revalidate/repaint of the window after docking changes:
     * heavyweight JCEF components don't repaint correctly when FlexDock
     * rearranges the docking port.
     */
    private void refreshAssistantLayout() {
        if (assistantDock != null && assistantVisible) {
            javax.swing.SwingUtilities.invokeLater(() -> {
                java.awt.Window w = javax.swing.SwingUtilities.getWindowAncestor(assistantDock);
                if (w != null) {
                    w.invalidate();
                    w.validate();
                    w.repaint();
                }
            });
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
                throw new RuntimeException(String.format(SciNotesMessages.INVALID_FILE, filePath));
            } catch (IOException e) {
                throw new RuntimeException(String.format(SciNotesMessages.IO_EXCEPTION, e.getLocalizedMessage()));
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
        scinotesWithText(text, false);
    }

    private static void scinotesWithText(String text, boolean newWindow) {
        ScilabLexer.update();
        launchSciNotes(newWindow);
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
        int sourceSelectionStart = currentSep.getSelectionStart();
        int sourceSelectionEnd = currentSep.getSelectionEnd();
        boolean sourceHidden = b && prepareTabTransferSource(ed, currentSep);

        try {
            editor = null;

            scinotesWithText(cdoc.getText(), true);
            ScilabEditorPane sep = editor.getTextPane();

            if (currentSep.getName() != null) {
                sep.setTitle(title);
                editor.setTitle(winTitle);
            }

            currentSep.copyProps(sep);
            if (currentSep.getName() != null) {
                /*
                 * scinotesWithText() opens an untitled tab; copyProps() has
                 * just given the pane its file, so the tab stops showing an
                 * Untitled number, and gives that number back.
                 */
                int index = editor.getTabPane().indexOfComponent(sep.getEditorComponent());
                if (index >= 0) {
                    editor.releaseUntitledNumber(editor.getTabPane().getScilabTitleAt(index));
                    editor.getTabPane().setTitleAt(index, new File(currentSep.getName()).getName());
                }
            }
            ScilabDocument sdoc = (ScilabDocument) sep.getDocument();
            copyDocumentProperties(cdoc, sdoc);
            sdoc.setContentModified(cdoc.isContentModified());
            sdoc.setContentModifiedSinceBackup(cdoc.isContentModifiedSinceBackup());
            sdoc.getUndoManager().discardAllEdits();
            setTransferredSelection(sep, sourceSelectionStart, sourceSelectionEnd);
            editor.enableUndoButton(false);
            editor.enableRedoButton(false);
            ConfigSciNotesManager.saveToOpenFiles(sep.getName(), editor, editor.getTextPane());

            editor.activeRestoreTab();
        } catch (RuntimeException e) {
            restoreTabTransferSource(ed, currentSep, sourceHidden);
            throw e;
        }

        if (b) {
            /*
             * Do not dispose a Monaco/JCEF source while its AWT drag callback
             * is still unwinding. The destination is fully constructed first,
             * and the source is closed on the following EDT turn.
             */
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    /*
                     * Closing the source tab can auto-select a sibling tab
                     * still open in the source window. Without this guard
                     * that sibling's MonacoEditorPane would grab native CEF
                     * focus for the source browser and immediately hand it
                     * back, flashing both windows' titlebars right after the
                     * window was just detached.
                     */
                    tabTransferInProgress = true;
                    try {
                        int sourceIndex = ed.getTextPaneIndex(currentSep);
                        if (sourceIndex >= 0) {
                            ed.closeTabAtWithoutConfirmation(sourceIndex);
                        }
                        if (ed.getTabPane().getTabCount() == 0) {
                            ClosingOperationsManager.startClosingOperationWithoutSave((SwingScilabDockablePanel) ed);
                            ConfigSciNotesManager.removeEditorUUID(ed.uuid.toString());
                        }
                    } finally {
                        tabTransferInProgress = false;
                    }
                    if (editor != null) {
                        ScilabEditorPane newPane = editor.getTextPane();
                        if (newPane != null) {
                            newPane.requestFocus();
                        }
                    }
                }
            });
        }
    }

    /**
     * Copy an editor pane into a newly owned tab of another SciNotes window.
     * The returned pane has its own Monaco model and uses the destination
     * window's SharedBrowser; the source remains untouched on failure.
     *
     * @param destination destination SciNotes window
     * @param source source editor pane
     * @param title displayed tab title
     * @param index requested destination index
     * @return the new pane, or null when the copy could not be completed
     */
    public static ScilabEditorPane copyTabForTransfer(SciNotes destination,
            ScilabEditorPane source, String title, int index) {
        if (destination == null || source == null) {
            return null;
        }

        ScilabEditorPane copied = null;
        try {
            ScilabDocument sourceDocument = (ScilabDocument) source.getDocument();
            int selectionStart = source.getSelectionStart();
            int selectionEnd = source.getSelectionEnd();
            copied = destination.addTabForTransfer(title, index);
            ScilabDocument copiedDocument = (ScilabDocument) copied.getDocument();

            boolean updater = copiedDocument.isUpdater();
            copiedDocument.setUpdater(false);
            boolean autoIndent = copiedDocument.getAutoIndent();
            copiedDocument.setAutoIndent(false);
            try {
                copiedDocument.insertString(0, sourceDocument.getText(), null);
            } finally {
                copiedDocument.setAutoIndent(autoIndent);
                copiedDocument.setUpdater(updater);
            }

            source.copyProps(copied);
            copyDocumentProperties(sourceDocument, copiedDocument);
            copiedDocument.setContentModified(sourceDocument.isContentModified());
            copiedDocument.setContentModifiedSinceBackup(sourceDocument.isContentModifiedSinceBackup());
            copiedDocument.getUndoManager().discardAllEdits();
            setTransferredSelection(copied, selectionStart, selectionEnd);
            destination.updateTabTitle(destination.getTextPaneIndex(copied));
            return copied;
        } catch (BadLocationException | RuntimeException e) {
            if (copied != null) {
                int copiedIndex = destination.getTextPaneIndex(copied);
                if (copiedIndex >= 0) {
                    destination.closeTabAtWithoutConfirmation(copiedIndex);
                }
            }
            System.err.println("SciNotes: unable to transfer editor tab: " + e.getMessage());
            return null;
        }
    }

    private static void copyDocumentProperties(ScilabDocument source, ScilabDocument destination) {
        destination.setEncoding(source.getEncoding());
        destination.setEOL(source.getEOL());
        destination.setBinary(source.getBinary());
    }

    private static void setTransferredSelection(ScilabEditorPane pane, int start, int end) {
        int length = pane.getDocument().getLength();
        int safeStart = Math.min(Math.max(0, start), length);
        int safeEnd = Math.min(Math.max(0, end), length);
        pane.setCaretPosition(safeStart);
        pane.moveCaretPosition(safeEnd);
    }

    /**
     * Hide an autonomous SciNotes window immediately before its last
     * successfully transferred tab is removed. The regular FlexDock closing
     * operation still owns disposal and configuration cleanup.
     *
     * @param source source SciNotes window
     * @param transferredPane pane about to be removed
     * @return true when an autonomous source window was hidden
     */
    public static boolean hideWindowBeforeLastTabTransfer(SciNotes source,
            ScilabEditorPane transferredPane) {
        if (source == null || transferredPane == null
                || source.getTabPane().getTabCount() != 1
                || source.getTextPane(0) != transferredPane) {
            return false;
        }

        SwingScilabWindow window = source.getParentWindow();
        if (window != null && window.getNbDockedObjects() == 1) {
            window.setVisible(false);
            Toolkit.getDefaultToolkit().sync();
            return true;
        }
        return false;
    }

    /**
     * Commit a destination pane which was prepared without changing the
     * selected tab. This is intentionally separate from copying so a DnD
     * transaction can hide its disappearing source window first.
     *
     * @param destination destination SciNotes window
     * @param transferredPane prepared destination pane
     * @return true when the pane was activated
     */
    public static boolean activateTransferredTab(SciNotes destination,
            ScilabEditorPane transferredPane) {
        if (destination == null || transferredPane == null) {
            return false;
        }

        int index = destination.getTextPaneIndex(transferredPane);
        if (index < 0) {
            return false;
        }

        try {
            destination.getTabPane().setSelectedIndex(index);
            destination.updateTabTitle(index);
            destination.getInfoBar().setText(transferredPane.getInfoBarText());
            transferredPane.requestFocus();
            if (transferredPane instanceof MonacoEditorPane) {
                ((MonacoEditorPane) transferredPane).restoreTransferredSelection();
            }
            return true;
        } catch (RuntimeException e) {
            return false;
        }
    }

    /**
     * Release the source editor focus and hide its autonomous window when its
     * last tab is about to be transferred.
     *
     * @param source source SciNotes window
     * @param sourcePane source pane
     * @return true when the source window was hidden
     */
    public static boolean prepareTabTransferSource(SciNotes source,
            ScilabEditorPane sourcePane) {
        if (sourcePane instanceof MonacoEditorPane) {
            ((MonacoEditorPane) sourcePane).releaseFocusForTransfer();
        }
        return hideWindowBeforeLastTabTransfer(source, sourcePane);
    }

    /**
     * Restore a source after a prepared destination failed to activate.
     *
     * @param source source SciNotes window
     * @param sourcePane source pane
     * @param sourceHidden whether preparation hid the source window
     */
    public static void restoreTabTransferSource(SciNotes source,
            ScilabEditorPane sourcePane, boolean sourceHidden) {
        if (source == null || sourcePane == null) {
            return;
        }
        if (sourceHidden) {
            SwingScilabWindow sourceWindow = source.getParentWindow();
            if (sourceWindow != null) {
                sourceWindow.setVisible(true);
            }
        }
        int sourceIndex = source.getTextPaneIndex(sourcePane);
        if (sourceIndex >= 0) {
            source.getTabPane().setSelectedIndex(sourceIndex);
            sourcePane.requestFocus();
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
    public static void cloneAndCloseCurrentTab(SciNotes ed, boolean b, int x, int y) {
        /*
         * SciNotesGUI.init() shows the new window before this method gets a
         * chance to move it, which makes it flash at its default position
         * before jumping to the drop location. Hand the target location over
         * so it can be applied before the window becomes visible; still set
         * it explicitly afterward as a fallback for window-creation paths
         * that do not consume the pending location (e.g. session restore).
         */
        pendingWindowLocation = new Point(x, y);
        try {
            cloneAndCloseCurrentTab(ed, b);
        } finally {
            pendingWindowLocation = null;
        }
        editor.getSwingParentWindow().setLocation(x, y);
    }

    /**
     * Consume the screen location requested for the next newly created
     * SciNotes window, if any.
     *
     * @return the requested location, or null when none is pending
     */
    public static Point takePendingWindowLocation() {
        Point location = pendingWindowLocation;
        pendingWindowLocation = null;
        return location;
    }

    /**
     * @return true while a cross-window or drag-out tab transfer is tearing
     *         down its source tab (see {@link #tabTransferInProgress}).
     */
    public static boolean isTabTransferInProgress() {
        return tabTransferInProgress;
    }

    /**
     * @param inProgress see {@link #tabTransferInProgress}
     */
    public static void setTabTransferInProgress(boolean inProgress) {
        tabTransferInProgress = inProgress;
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

        // Restore files from previous version. While replaying the saved list,
        // do not rewrite it from each loadFile() call.
        restoringSession = true;
        try {
            RestoreOpenedFilesAction.forceRestore(this, getUUID().toString());
        } finally {
            restoringSession = false;
        }

        WindowsConfigurationManager.restorationFinished(SciNotes.this);
        if  (System.getProperty("os.name").toLowerCase().contains("mac")) {
          RestoreOpenedFilesAction.restoreEnabledComponents(this);
        }
    }

    /**
     * Create SciNotes instance
     *
     * @return the instance
     */
    public static void launchSciNotes() {
        launchSciNotes(false);
    }

    /**
     * Create SciNotes instance
     *
     * @param fresh
     *            true to always open a new, empty window: a tab moved to a
     *            window of its own must not bring back the files of a
     *            SciNotes window closed earlier
     */
    private static void launchSciNotes(boolean fresh) {
        if (editor == null && !fresh && !scinotesList.isEmpty()) {
            // The focused window was closed: use another open one rather
            // than creating a window
            editor = scinotesList.get(scinotesList.size() - 1);
        }
        boolean created = editor == null;
        if (editor == null) {
            List<String> uuids = fresh ? new ArrayList<String>() : ConfigSciNotesManager.getEditorsUUID();
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
                        if (ConfigSciNotesManager.countExistingOpenFiles(UUID.fromString(uuid)) > 0) {
                            editor = SciNotesTab.getEditorInstance(uuid);
                            ScilabTabFactory.getInstance().addToCache(editor);
                            editor.setParentWindow();
                            ConfigSciNotesManager.saveEditorUUID(editor.getPersistentId());
                            SciNotesGUI.init(editor.getParentWindow(), editor, SCINOTES);
                            WindowsConfigurationManager.unregisterEndedRestoration(editor);
                            success = true;
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
        } else {
            /* restore if it is iconified */
            if (editor.getParentWindow() != null) {
                int state = editor.getParentWindow().getExtendedState();
                if ((state & JFrame.ICONIFIED) == JFrame.ICONIFIED) {
                    editor.getParentWindow().setExtendedState(state - JFrame.ICONIFIED);
                }
            }
        }

        if (!editor.restored) {
            editor.restorePreviousSession();
        }

        ScilabGUIUtilities.toFront(editor,SciNotesMessages.SCILAB_EDITOR);

        // Show the assistant with the first window, unless the user closed it
        // last time. Deferred: the window is laid out first, so the panel
        // gets half of its width.
        if (created && !fresh && assistantHost == null
                && org.scilab.modules.scinotes.assistant.AssistantPanel.isOpenWithSciNotes()) {
            final SciNotes ed = editor;
            SwingUtilities.invokeLater(() -> {
                if (assistantHost == null && scinotesList.contains(ed)) {
                    ed.toggleAssistant();
                }
            });
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
        if (ClosingOperationsManager.isClosingScilab() || scinotesList.size() <= 1) {
            // Scilab exits, or the last window closes: its files are
            // reopened with it next time
            saveOpenFilesForRestore();
        } else {
            // A window closed while others stay open: forget it, else a
            // window created later (e.g. a tab dragged out) restores its tabs
            ConfigSciNotesManager.removeEditorUUID(uuid.toString());
        }

        for (int i = 0; i < getTabPane().getTabCount(); i++) {
            ScilabEditorPane textPaneAt = getTextPane(i);
            if (textPaneAt != null) {
                textPaneAt.destroy();
            }
        }

        // Assistant: release its browser (a live CEF browser blocks CEF shutdown)
        java.awt.Window assistantWindow = null;
        if (assistantPanel != null) {
            assistantWindow = javax.swing.SwingUtilities.getWindowAncestor(contentPane);
            // Close the browser before detaching it, as Monaco does: once
            // closing, CefBrowserWr.removeNotify() posts no native reparent,
            // and dispose() waits for CEF to have destroyed it
            assistantPanel.dispose();
            assistantPanel = null;
        }
        if (assistantDock != null) {
            contentPane.remove(assistantDock);
            assistantDock = null;
            assistantVisible = false;
        }
        if (assistantHost == this) {
            // The single assistant goes with its window: stop its tasks, the
            // next one opens in the window asking for it
            assistantHost = null;
            assistantDebounceTimer.stop();
            if (assistantTrackedPane != null) {
                assistantTrackedPane.removeCaretListener(assistantCaretListener);
                assistantTrackedPane = null;
            }
            org.scilab.modules.scinotes.assistant.AssistantPanel.shutdown();
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
            // Kill the AI CLIs still running and stop the assistant servers
            org.scilab.modules.scinotes.assistant.AssistantPanel.shutdown();
        }

        editor = null;
        // The assistant followed a tab of this window: back to another one
        if (assistantHost != null) {
            assistantHost.activeEditorChanged();
        }
        SciNotesGUI.clean(this);
        close();
        ConfigSciNotesManager.resetDocument();

        // SciNotes docked with other panels: the window stays, and the area
        // of the native assistant browser is not repainted by itself
        if (assistantWindow != null) {
            final java.awt.Window w = assistantWindow;
            javax.swing.SwingUtilities.invokeLater(() -> {
                if (w.isDisplayable()) {
                    w.invalidate();
                    w.validate();
                    w.repaint();
                }
            });
        }
    }

    /**
     * Persist the current tab list before the dockable panel is destroyed.
     * This keeps session restore independent from the individual open/save
     * paths used to create each tab.
     */
    private void saveOpenFilesForRestore() {
        ConfigSciNotesManager.saveEditorUUID(getPersistentId());
        ConfigSciNotesManager.removeFromOpenFiles(getUUID());

        int count = getTabPane().getTabCount();
        for (int i = 0; i < count; i++) {
            ScilabEditorPane pane = getTextPane(i);
            if (pane != null && pane.getName() != null) {
                ConfigSciNotesManager.saveToOpenFiles(pane.getName(), this, pane);
            }
        }
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
        for (int i = 0; i < numberOfTab; i++) {
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
        /*
         * Every tab on a file holds the same text, so the last one closing is
         * the one that decides whether it is saved.
         */
        if (confirmation && ((ScilabDocument) textPaneAt.getDocument()).isContentModified()
                && !stillOpenElsewhere(textPaneAt, false)) {
            if (!save(indexTab, false, scilabClose)) {
                return false;
            }
        }

        // A split tab holds two panes (see splitTab()/removeSplit()); closing
        // the tab must close both, not just the one getTextPane(indexTab)
        // resolves to. Left uncleaned, a MonacoEditorPane half stays
        // registered forever in its SharedBrowser (see unregister()) even
        // though its Swing component tree is gone with the tab.
        ScilabEditorPane otherPaneAt = textPaneAt.getOtherPaneInSplit();
        textPaneAt.close();
        if (otherPaneAt != null) {
            otherPaneAt.close();
        }

        if (textPaneAt.getName() == null) {
            releaseUntitledNumber(tabPane.getScilabTitleAt(indexTab));
        }

        // correction for bug 5404, closing the last tabPane generate an
        // exception
        // that's why we need to remove ChangeListeners before closing
        boolean removedTabListeners = false;
        if ((indexTab == 0) && (getTabPane().getTabCount() == 1)) {
            for (ChangeListener cl : tabPane.getChangeListeners()) {
                tabPane.removeChangeListener(cl);
            }
            removedTabListeners = true;
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

        // The bug-5404 workaround above drops every tab ChangeListener; without
        // restoring ours, later tab switches (e.g. after re-opening files) no
        // longer refresh the editor for the newly selected tab.
        if (removedTabListeners) {
            reinstallTabChangeListener();
        }

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
         *
         * This window is closing with all its tabs: ask once for a file it
         * shows several times, and not at all when another window keeps it.
         */
        if (textPaneAt != null && ((ScilabDocument) textPaneAt.getDocument()).isContentModified()
                && !stillOpenElsewhere(textPaneAt, true) && isFirstTabOnFile(index)) {
            if (!save(index, false, false)) {
                return false;
            }
        }

        return true;
    }

    /**
     * @param otherWindowsOnly true to ignore this window, whose tabs are
     *        closing too
     * @return whether another tab keeps this pane's file open
     */
    private static boolean stillOpenElsewhere(ScilabEditorPane pane, boolean otherWindowsOnly) {
        return pane instanceof MonacoEditorPane
               && ((MonacoEditorPane) pane).isOpenInAnotherPane(otherWindowsOnly);
    }

    /** Whether no earlier tab of this window shows the same file. */
    private boolean isFirstTabOnFile(int indexTab) {
        ScilabEditorPane pane = getTextPane(indexTab);
        String name = pane == null ? null : pane.getName();
        if (name == null) {
            return true;
        }
        for (int i = 0; i < indexTab; i++) {
            ScilabEditorPane other = getTextPane(i);
            if (other != null && name.equals(other.getName())) {
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
     * The extensions of the files SciNotes reads as Scilab code.
     *
     * @return the extensions, dot included
     */
    public static List<String> getScilabExtensions() {
        return SCILAB_EXTENSIONS;
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
        acknowledgeSharedSave(textPaneAt, newSavedFile);

        if (textPaneAt.getName() == null) {
            releaseUntitledNumber(getTabPane().getScilabTitleAt(indexTab));
        }

        textPaneAt.setName(fileToSave);
        getTabPane().setTitleAt(indexTab, newSavedFile.getName());

        setTitle(textPaneAt.getTitle());
        ConfigSciNotesManager.saveToOpenFiles(fileToSave, this, textPaneAt);
        ConfigSciNotesManager.saveToRecentOpenedFiles(fileToSave);
        RecentFileAction.updateRecentOpenedFilesMenu(this);

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
        acknowledgeSharedSave(textPaneAt, newSavedFile);

        textPaneAt.setName(fileToSave);
        setTitle(textPaneAt.getTitle());

        return true;
    }

    /*
     * The same file can be open in other tabs and other windows: they show
     * the text just written, so they are saved too.
     */
    private static void acknowledgeSharedSave(ScilabEditorPane pane, File savedFile) {
        if (pane instanceof MonacoEditorPane) {
            ((MonacoEditorPane) pane).acknowledgeSharedSave(savedFile);
        }
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
        String extension = "";
        String name = null;
        String initialDirectoryPath = path;
        if (initialDirectoryPath == null) {
            name = getTextPane().getName();
            if (name != null) {
                File file = new File(name);
                name = file.getName();
                initialDirectoryPath = file.getParent();
            }
        }
        if (name == null) {
            if (firstOpen) {
                initialDirectoryPath = CommonFileUtils.getCWD();
                firstOpen = false;
            } else {
                initialDirectoryPath = ConfigManager.getLastOpenedDirectory();
            }
        }

        String[] DEFAULT_MASK = {ALL_SCE_FILES,ALL_SCI_FILES,ALL_SCX_FILES,ALL_TST_FILES,ALL_START_FILES,ALL_QUIT_FILES,ALL_DEM_FILES,ALL_FILES,ALL_SCILAB};

        FileChooser sfc = ScilabFileChooser.createFileChooser();
        sfc.addMask(DEFAULT_MASK, null);
        sfc.setInitialDirectory(initialDirectoryPath);
        sfc.setTitle(title);
        sfc.setMultipleSelection(false);
        sfc.setUiDialogType(Juigetfile.SAVE_DIALOG);

        if (name == null) {
            name = ((ScilabDocument) getTextPane().getDocument()).getFirstFunctionName();
            if (name != null) {
                name += SCI_EXTENSION;
            }
        }

        sfc.setInitialFileName(name);
        sfc.displayAndWait();
        sfc.invalidate();

        String[] selection = sfc.getSelection();
        if (selection.length>0 && selection[0] != "") {
            String fileName = selection[0];

            ConfigManager.saveLastOpenedDirectory(sfc.getSelectionPathName());

            return fileName;
        }  else {
            return "";
        }
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
            releaseUntitledNumber(getTabPane().getScilabTitleAt(getTabPane().getSelectedIndex()));
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
        return addTab(title, index, caretPos, true);
    }

    private ScilabEditorPane addTabForTransfer(String title, int index) {
        return addTab(title, index, 0, false);
    }

    private ScilabEditorPane addTab(String title, int index, int caretPos, boolean activate) {
        ScilabEditorPane sep = createEditorPane();
        initPane(sep);
        int ind = Math.min(Math.max(0, index), tabPane.getTabCount());
        tabPane.insertTab(title, null, sep.getEditorComponent(), "", ind);
        initInputMap(sep);
        if (activate) {
            tabPane.setSelectedIndex(ind);
            updateTabTitle();
            getInfoBar().setText(sep.getInfoBarText());
        }
        repaint();
        if (activate) {
            sep.init(caretPos);
        }
        return sep;
    }

    /**
     * Create the editor pane used by a SciNotes tab.
     * Monaco is opt-in while the integration is still experimental.
     */
    private ScilabEditorPane createEditorPane() {
        ScilabEditorPane pane;
        if (MONACO_EDITOR_PROPERTY_VALUE.equalsIgnoreCase(System.getProperty(SCINOTES_EDITOR_PROPERTY))) {
            pane = new MonacoEditorPane(this);
        } else {
            pane = new ScilabEditorPane(this);
        }
        pane.init();
        return pane;
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
        int orientation = vertical ? JSplitPane.VERTICAL_SPLIT : JSplitPane.HORIZONTAL_SPLIT;
        /*
         * The split menu is a radio group, and clicking the already selected
         * entry still fires its action. Rebuilding an identical split would
         * throw both panes (and, for Monaco, their models and views) away and
         * recreate them for nothing -- a visible reload.
         */
        ScilabEditorPane splitSibling = pane.getOtherPaneInSplit();
        if (pane.getEditorComponent().isSplited()) {
            if (pane.getEditorComponent().getSplitPane().getOrientation() == orientation) {
                return;
            }
        }
        Component bottom = pane.getEditorComponent().getBottom();
        int state = pane.getXln().getState();
        ScilabEditorPane leftPane = createEditorPaneLike(pane);
        ScilabEditorPane rightPane = createEditorPaneLike(pane);
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
        JSplitPane split = new JSplitPane(orientation);
        leftPane.getXln().setWhereamiLineNumbering(state);
        rightPane.getXln().setWhereamiLineNumbering(state);
        split.setLeftComponent(getSplitComponent(leftPane));
        split.setRightComponent(getSplitComponent(rightPane));
        split.setResizeWeight(0.5);
        if (leftPane instanceof MonacoEditorPane) {
            /*
             * The Monaco overlay is one native window stretched over both
             * halves (see MonacoEditorPane.SharedBrowser.activateSplit()); a
             * real JSplitPane divider between them would sit under that
             * window and become undraggable. Disable it: the page draws and
             * drags its own separator, and moves this divider on release
             * (see configureSplitHandle() in monaco-split.js).
             */
            split.setDividerSize(0);
        }
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
        if (pane instanceof MonacoEditorPane) {
            pane.close();
            // Changing the orientation of an existing split replaces both old
            // panes, not just the one getTextPane() resolved to; the other
            // would otherwise stay registered in its SharedBrowser with its
            // Monaco model never disposed.
            if (splitSibling != null) {
                splitSibling.close();
            }
            /*
             * There is only one native Monaco browser per window: rightPane
             * cannot get its own document like leftPane just did. Turn it
             * into a second live view of leftPane's model instead (see
             * MonacoEditorPane.enterSplitView()).
             */
            ((MonacoEditorPane) rightPane).enterSplitView((MonacoEditorPane) leftPane);
        }
    }

    /**
     * Remove a split
     */
    public void removeSplit() {
        if (((EditorComponent) tabPane.getSelectedComponent()).isSplited()) {
            ScilabEditorPane textpane = getTextPane();
            ScilabEditorPane pane = createEditorPaneLike(textpane);
            Component bottom = textpane.getEditorComponent().getBottom();
            ScilabEditorPane otherPane = textpane.getOtherPaneInSplit();
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
            if (textpane instanceof MonacoEditorPane) {
                textpane.close();
                if (otherPane != null) {
                    otherPane.close();
                }
                ((MonacoEditorPane) pane).activateInSharedBrowser();
            }
        }
    }

    private ScilabEditorPane createEditorPaneLike(ScilabEditorPane pane) {
        ScilabEditorPane newPane;
        if (pane instanceof MonacoEditorPane) {
            newPane = new MonacoEditorPane(this);
        } else {
            newPane = new ScilabEditorPane(this);
        }
        newPane.init();
        return newPane;
    }

    private Component getSplitComponent(ScilabEditorPane pane) {
        if (pane instanceof MonacoEditorPane) {
            return pane.getEditorComponent().detachEditorViewComponent();
        }

        return pane.getScrollPane();
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
     * Give the number of an untitled tab back to the pool, so that a later
     * empty tab can reuse it.
     *
     * @param title
     *            the title of the tab being named or closed
     */
    private void releaseUntitledNumber(String title) {
        if (title == null) {
            return;
        }

        // A tab that an interrupted open left unnamed keeps the file name it
        // was created with, and holds no number of the pool.
        String name = title.startsWith("*") ? title.substring(1) : title;
        if (!name.startsWith(SciNotesMessages.UNTITLED)) {
            return;
        }

        try {
            Integer number = Integer.valueOf(name.substring(SciNotesMessages.UNTITLED.length()).trim());
            tabList.remove(number);
            closedTabList.add(number);
        } catch (NumberFormatException e) {
            // Not an untitled tab after all.
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
        ScilabEditorPane pane = getTextPane();
        if (pane instanceof MonacoEditorPane) {
            // Monaco owns the live text and its own undo stack.
            ((MonacoEditorPane) pane).undo();
            return;
        }
        ScilabDocument doc = (ScilabDocument) pane.getDocument();
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
        ScilabEditorPane pane = getTextPane();
        if (pane instanceof MonacoEditorPane) {
            // Monaco owns the live text and its own undo stack.
            ((MonacoEditorPane) pane).redo();
            return;
        }
        ScilabDocument doc = (ScilabDocument) pane.getDocument();
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
        if (textPaneAt instanceof MonacoEditorPane && ((MonacoEditorPane) textPaneAt).reloadFileInPlace()) {
            return;
        }

        if (textPaneAt != null && textPaneAt.getName() != null) {
            boolean removedTabListeners = false;
            if ((index == 0) && (getTabPane().getTabCount() == 1)) {
                for (ChangeListener cl : tabPane.getChangeListeners()) {
                    tabPane.removeChangeListener(cl);
                }
                removedTabListeners = true;
            }
            ConfigSciNotesManager.removeFromOpenFiles(this, textPaneAt);
            tabPane.remove(index);
            if (removedTabListeners) {
                reinstallTabChangeListener();
            }
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
            if (textPaneAt == null) {
                // getTextPane(i) can transiently be null while a freshly added
                // tab is still wiring up; skip rather than NPE and abort a
                // multi-file open.
                continue;
            }
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
                            boolean removedTabListeners = false;
                            if ((i == 0) && (getTabPane().getTabCount() == 1)) {
                                for (ChangeListener cl : tabPane.getChangeListeners()) {
                                    tabPane.removeChangeListener(cl);
                                }
                                removedTabListeners = true;
                            }
                            tabPane.remove(i);
                            if (removedTabListeners) {
                                reinstallTabChangeListener();
                            }
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
                if (sep instanceof MonacoEditorPane) {
                    ((MonacoEditorPane) sep).applyDisplayOptions();
                }
                if (sep.getOtherPaneInSplit() != null) {
                    sep.getOtherPaneInSplit().getXln().setWhereamiLineNumbering(SciNotesOptions.getSciNotesDisplay().showLineNumbers, SciNotesOptions.getSciNotesDisplay().whereami);
                    if (sep.getOtherPaneInSplit() instanceof MonacoEditorPane) {
                        ((MonacoEditorPane) sep.getOtherPaneInSplit()).applyDisplayOptions();
                    }
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
                    if (sep instanceof MonacoEditorPane) {
                        ((MonacoEditorPane) sep).applyDisplayOptions();
                        continue;
                    }
                    if (sep.getOtherPaneInSplit() == null) {
                        ScilabEditorPane pane = new ScilabEditorPane(editor);
                        pane.init();
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
                ((ScilabEditorKit) sep.getEditorKit()).getStylePreferences().changeBaseFontSize(-inc);
                if (baseFont == null) {
                    baseFont = ((ScilabEditorKit) sep.getEditorKit()).getStylePreferences().getBaseFont();
                }
                if (sep.getOtherPaneInSplit() != null) {
                    ((ScilabEditorKit) sep.getOtherPaneInSplit().getEditorKit()).getStylePreferences().changeBaseFontSize(-inc);
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
     * Re-attach SciNotes' own tab ChangeListener after code that strips every
     * tab listener (the bug-5404 last-tab workaround). Without this the editor
     * stops reacting to tab switches.
     */
    private void reinstallTabChangeListener() {
        for (ChangeListener cl : tabPane.getChangeListeners()) {
            if (cl == tabChangeListener) {
                return;
            }
        }
        tabPane.addChangeListener(tabChangeListener);
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
            if (!restoringSession) {
                ConfigSciNotesManager.saveToOpenFiles(f.getPath(), this, getTextPane());
            }

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
        // Called on every tab change and focus of an editor pane, in any
        // window: the assistant follows the file the user works on
        if (assistantHost != null) {
            assistantHost.activeEditorChanged();
        }
    }

    /**
     * @return the focused editor
     */
    public static SciNotes getEditor() {
        return editor;
    }

    /**
     * The editor the assistant acts on: the last focused one, else the one
     * holding the assistant, else any open one.
     *
     * @return the active editor, or null when no SciNotes is open
     */
    public static SciNotes getActiveEditor() {
        if (editor != null && scinotesList.contains(editor)) {
            return editor;
        }
        if (assistantHost != null) {
            return assistantHost;
        }
        return scinotesList.isEmpty() ? null : scinotesList.get(0);
    }

    /**
     * The SciNotes window holding the assistant, null when none. There is a
     * single assistant for all the windows, as in VS Code: its editor tools
     * act on the tabs of every window.
     */
    private static SciNotes assistantHost;

    private org.scilab.modules.scinotes.assistant.AssistantPanel assistantPanel;
    /** Right side of the window: resize handle + assistant panel. */
    private javax.swing.JPanel assistantDock;
    private boolean assistantVisible = false;
    private int assistantWidth = -1;
    private javax.swing.event.CaretListener assistantCaretListener;
    private ScilabEditorPane assistantTrackedPane;
    private javax.swing.Timer assistantDebounceTimer;

    /** Width bounds of the assistant side panel. */
    private static final int ASSISTANT_MIN_WIDTH = 280;
    private static final int EDITOR_MIN_WIDTH = 200;

    /**
     * Toggle the assistant side panel, on the right side of SciNotes.
     *
     * The panel goes to the EAST of the SciNotes contents (not of the
     * window, which is the whole console when SciNotes is docked in it) and
     * is only shown/hidden afterwards: the editor layer in the CENTER (with
     * the Monaco editors) is never removed nor re-parented. Moving it in and
     * out of a split pane re-parented the native JCEF browsers of Monaco,
     * which broke them at random (grey or white editors), especially while
     * they were still being created at SciNotes startup.
     *
     * There is a single assistant: asked from another window, it is brought
     * to the front if shown, else moved into the window asking for it.
     */
    public void toggleAssistant() {
        if (!SwingUtilities.isEventDispatchThread()) {
            // assistant_gui() calls it from the Scilab thread
            SwingUtilities.invokeLater(() -> toggleAssistant());
            return;
        }
        if (assistantHost != null && assistantHost != this) {
            if (assistantHost.assistantVisible) {
                assistantHost.showAssistant();
                return;
            }
            takeAssistantFrom(assistantHost);
        }
        try {
            if (assistantVisible) {
                assistantDock.setVisible(false);
                assistantVisible = false;
            } else {
                if (assistantPanel == null) {
                    assistantPanel = new org.scilab.modules.scinotes.assistant.AssistantPanel();
                    assistantPanel.setOnReady(() -> sendEditorContextToAssistant());
                    assistantHost = this;
                    // Debounce: the context is sent 300ms after the last event
                    assistantDebounceTimer = new javax.swing.Timer(300, e -> sendEditorContextToAssistant());
                    assistantDebounceTimer.setRepeats(false);
                }
                if (assistantDock == null) {
                    assistantDock = new javax.swing.JPanel(new java.awt.BorderLayout());
                    assistantDock.add(createAssistantResizeHandle(), java.awt.BorderLayout.LINE_START);
                    assistantDock.add(assistantPanel, java.awt.BorderLayout.CENTER);
                    contentPane.add(assistantDock, java.awt.BorderLayout.LINE_END);
                }
                if (assistantWidth <= 0) {
                    // The width set by the user last time, else half the window
                    assistantWidth = org.scilab.modules.scinotes.assistant.AssistantPanel.getSavedWidth();
                }
                if (assistantWidth <= 0) {
                    assistantWidth = Math.max(ASSISTANT_MIN_WIDTH, contentPane.getWidth() / 2);
                }
                setAssistantWidth(assistantWidth);
                assistantDock.setVisible(true);
                assistantVisible = true;

                // Follow the active editor (the first context is sent when
                // the frontend sends 'ready')
                activeEditorChanged();
            }

            contentPane.revalidate();
            contentPane.repaint();
            // Reopened with the next SciNotes only if left open
            org.scilab.modules.scinotes.assistant.AssistantPanel.setOpenWithSciNotes(assistantVisible);
        } catch (Exception e) {
            System.err.println("Failed to toggle assistant: " + e.getMessage());
            e.printStackTrace();
        }
    }

    /** Apply a width to the assistant side panel, within bounds. */
    private void setAssistantWidth(int width) {
        java.awt.Container parent = assistantDock.getParent();
        int max = parent != null && parent.getWidth() > 0
            ? parent.getWidth() - EDITOR_MIN_WIDTH : Integer.MAX_VALUE;
        assistantWidth = Math.max(ASSISTANT_MIN_WIDTH, Math.min(width, max));
        assistantDock.setPreferredSize(new java.awt.Dimension(assistantWidth, 0));
    }

    /** Vertical bar on the left of the assistant panel: drag it to resize the panel. */
    private javax.swing.JComponent createAssistantResizeHandle() {
        javax.swing.JPanel handle = new javax.swing.JPanel();
        handle.setPreferredSize(new java.awt.Dimension(5, 0));
        handle.setCursor(java.awt.Cursor.getPredefinedCursor(java.awt.Cursor.W_RESIZE_CURSOR));
        handle.setBorder(javax.swing.BorderFactory.createMatteBorder(0, 1, 0, 0,
            javax.swing.UIManager.getColor("Separator.foreground")));
        java.awt.event.MouseAdapter drag = new java.awt.event.MouseAdapter() {
            private int startX;
            private int startWidth;

            @Override
            public void mousePressed(java.awt.event.MouseEvent e) {
                startX = e.getXOnScreen();
                startWidth = assistantDock.getWidth();
            }

            @Override
            public void mouseDragged(java.awt.event.MouseEvent e) {
                setAssistantWidth(startWidth + startX - e.getXOnScreen());
                assistantDock.revalidate();
            }

            @Override
            public void mouseReleased(java.awt.event.MouseEvent e) {
                // Saved once the drag ends, not on every move
                org.scilab.modules.scinotes.assistant.AssistantPanel.saveWidth(assistantWidth);
            }
        };
        handle.addMouseListener(drag);
        handle.addMouseMotionListener(drag);
        return handle;
    }

    /**
     * Move the hidden assistant of another window into this one. Its browser
     * is moved as a docking move does, not recreated: the conversation
     * displayed and a task still running are kept.
     *
     * @param from the window holding the assistant
     */
    private void takeAssistantFrom(SciNotes from) {
        assistantPanel = from.assistantPanel;
        assistantWidth = from.assistantWidth;
        from.assistantPanel = null;
        from.assistantDebounceTimer.stop();
        from.assistantDebounceTimer = null;
        if (from.assistantTrackedPane != null) {
            from.assistantTrackedPane.removeCaretListener(from.assistantCaretListener);
            from.assistantTrackedPane = null;
        }

        // Keep the browser alive: out of a docking move, removing it from
        // the window disposes it (see ScilabCefPanel.removeNotify())
        ScilabCefPanel.setDockInProgress(true);
        try {
            from.assistantDock.remove(assistantPanel);
            from.contentPane.remove(from.assistantDock);
        } finally {
            ScilabCefPanel.setDockInProgress(false);
        }
        from.assistantDock = null;
        from.contentPane.revalidate();
        from.contentPane.repaint();

        assistantPanel.setOnReady(() -> sendEditorContextToAssistant());
        assistantDebounceTimer = new javax.swing.Timer(300, e -> sendEditorContextToAssistant());
        assistantDebounceTimer.setRepeats(false);
        assistantHost = this;
    }

    /** Show the assistant of this window and bring the window to the front. */
    private void showAssistant() {
        if (!assistantVisible) {
            toggleAssistant();
        }
        java.awt.Window w = SwingUtilities.getWindowAncestor(contentPane);
        if (w instanceof java.awt.Frame && (((java.awt.Frame) w).getExtendedState() & java.awt.Frame.ICONIFIED) != 0) {
            ((java.awt.Frame) w).setExtendedState(((java.awt.Frame) w).getExtendedState() & ~java.awt.Frame.ICONIFIED);
        }
        if (w != null) {
            w.toFront();
        }
        assistantPanel.requestBrowserFocus();
    }

    /**
     * The active editor changed (tab change, focus of another window) or the
     * assistant was shown: follow the caret of the new active tab and
     * refresh the context displayed by the assistant.
     */
    private void activeEditorChanged() {
        if (!assistantVisible) {
            return;
        }
        trackActivePaneCaret();
        assistantDebounceTimer.restart();
    }

    /**
     * Send the context of the active editor to the assistant, if any.
     */
    public static void refreshAssistantContext() {
        if (assistantHost != null) {
            assistantHost.sendEditorContextToAssistant();
        }
    }

    /**
     * Attach a CaretListener to the tab of the active editor, which may be in
     * another window. Detach from the previous one if needed.
     */
    private void trackActivePaneCaret() {
        if (assistantCaretListener == null) {
            assistantCaretListener = e -> {
                if (assistantVisible && assistantDebounceTimer != null) {
                    assistantDebounceTimer.restart();
                }
            };
        }

        // Detach from previous pane
        if (assistantTrackedPane != null) {
            assistantTrackedPane.removeCaretListener(assistantCaretListener);
        }

        // Attach to current pane
        SciNotes active = getActiveEditor();
        ScilabEditorPane pane = active != null ? active.getTextPane() : null;
        assistantTrackedPane = pane;
        if (pane != null) {
            pane.addCaretListener(assistantCaretListener);
        }
    }

    /**
     * Send the context of the active editor (filename, selection info) to the assistant frontend.
     */
    public void sendEditorContextToAssistant() {
        if (assistantPanel == null || !assistantVisible) return;

        SciNotes active = getActiveEditor();
        ScilabEditorPane pane = active != null ? active.getTextPane() : null;
        if (pane == null) {
            assistantPanel.sendEvent("{\"type\":\"editor_context\",\"filename\":\"\",\"selection\":\"\"}");
            return;
        }

        String filename = pane.getName();
        if (filename == null) filename = "";

        String shortName = pane.getShortName();
        if (shortName == null || shortName.isEmpty()) shortName = "(untitled)";

        String selection = pane.getSelectedText();
        int selectionLines = 0;
        int selStart = 0;
        int selEnd = 0;
        if (selection != null && !selection.isEmpty()) {
            selStart = pane.getDocument().getDefaultRootElement()
                .getElementIndex(pane.getSelectionStart()) + 1;
            selEnd = pane.getDocument().getDefaultRootElement()
                .getElementIndex(pane.getSelectionEnd()) + 1;
            selectionLines = selEnd - selStart + 1;
        }

        StringBuilder json = new StringBuilder();
        json.append("{\"type\":\"editor_context\"");
        json.append(",\"filename\":\"").append(jsonEscape(filename)).append("\"");
        json.append(",\"shortName\":\"").append(jsonEscape(shortName)).append("\"");
        json.append(",\"selectionLines\":").append(selectionLines);
        json.append(",\"selStart\":").append(selStart);
        json.append(",\"selEnd\":").append(selEnd);
        json.append("}");

        assistantPanel.sendEvent(json.toString());
    }

    private static String jsonEscape(String s) {
        if (s == null) return "";
        return s.replace("\\", "\\\\").replace("\"", "\\\"")
                .replace("\n", "\\n").replace("\r", "\\r").replace("\t", "\\t");
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
                        Class<?> clazz = loader.loadClass(className);
                        Constructor<?> constructor = clazz.getConstructor(new Class<?>[] { String.class, SciNotes.class });
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

        ScilabDocument doc = new ScilabDocument();
        ScilabEditorKit kit = new ScilabEditorKit();

        try ( FileInputStream fis = new FileInputStream(fileName);
                    InputStreamReader isr = new InputStreamReader(fis, charset);
                    BufferedReader reader = new BufferedReader(isr) ) {

            kit.read(reader, doc, 0);

        } catch (BadLocationException e) {
            System.err.println(e);
        } catch (IOException ioe) {
            System.err.println(ioe);
        }

        doc.addDocumentListener(doc);
        if (!doc.getBinary()) {
            action.actionOn(doc);
        }

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
        final ArrayList<ActionOnDocument> actions = new ArrayList<>();
        for (String act : actionsName) {
            if (act.equalsIgnoreCase("indent")) {
                actions.add(org.scilab.modules.scinotes.actions.IndentAction.getActionOnDocument());
            } else if (act.equalsIgnoreCase("trailing")) {
                actions.add(org.scilab.modules.scinotes.actions.RemoveTrailingWhiteAction.getActionOnDocument());
            } else if (act.equalsIgnoreCase("quote")) {
                actions.add(org.scilab.modules.scinotes.actions.DoubleQuoteStringAction.getActionOnDocument());
            }
        }

        if (actions.size() > 0) {
            executeAction(fileName, new ActionOnDocument() {
                public void actionOn(ScilabDocument doc) throws IOException {
                    for (ActionOnDocument act : actions)
                        act.actionOn(doc);
                };
            });
        }

        return actions.size() > 0;
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
