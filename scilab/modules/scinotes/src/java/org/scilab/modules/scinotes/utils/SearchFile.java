/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.utils;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeSelectionModel;

import org.flexdock.docking.event.DockingEvent;
import org.flexdock.docking.defaults.DockingSplitPane;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.UIElementMapper;

import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.SciNotesGUI;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.SearchManager;

/**
 * Class SearchFile: open a window with a JTree to show the results of a search in files.
 * @author Calixte DENIZET
 */
public class SearchFile extends SwingScilabTab implements Tab {

    public static final String SEARCHDONE = "SearchFile.SearchDone";

    private static SearchFile instance;

    private Window parentWindow;
    private SciNotes editor;

    /**
     * Default constructor
     * @param editor the editor where to open a file
     * @param title the window title
     */
    private SearchFile(SciNotes editor, String title) {
        super(title);
        this.editor = editor;
        parentWindow = ScilabWindow.createWindow();
        instance = this;
    }

    /**
     * Get the parent window id for this tab
     * @return the id of the parent window
     */
    public Window getParentWindow() {
        return this.parentWindow;
    }

    /**
     * {@inheritDoc}
     */
    public SimpleTab getAsSimpleTab() {
        return this;
    }

    /**
     * {@inheritDoc}
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        setInfoBar(infoBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        setMenuBar(menuBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        setToolBar(toolBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void dockingComplete(DockingEvent evt) {
        super.dockingComplete(evt);
        /* Trick to always have the editor's toolbar when the navigator
           is docked in the editor */
        if (evt.getNewDockingPort().getDockedComponent() instanceof DockingSplitPane) {
            DockingSplitPane dsp = (DockingSplitPane) evt.getNewDockingPort().getDockedComponent();
            if (dsp.getElderComponent() instanceof SciNotes) {
                addToolBar(editor.getToolBar());
            }
        }
    }

    /**
     * Close the current window if it exists
     */
    public static void closeCurrent() {
        if (instance != null) {
            instance.closeWindow();
        }
    }

    /**
     * Close the current window
     */
    public void closeWindow() {
        ScilabWindow win = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(getParentWindowId());
        win.removeTab(this);
        setVisible(false);
        parentWindow = null;
        instance = null;
        close();
    }

    /**
     * {@inheritDoc}
     */
    public void undockingComplete(DockingEvent evt) {
        super.undockingComplete(evt);
        addToolBar(null);
    }

    /**
     * Get a JTree with the results of a search
     * @param editor the editor where to open a file
     * @param statusbar if non null the status bar where to write file info
     * @param base the base directory
     * @param recursive, if true then a recursive search is made
     * @param ignoreCR, if true then the read file is considered as one line and regex pattern can include \n
     * @param filePattern the pattern to use to select the files. * is equivalent to .* and ? to .?
     * @param fileCaseSensitive, if true then the file pattern is case sensitive
     * @param wordPattern the pattern of the word to search
     * @param wordCaseSensitive, if true then the word pattern is case sensitive
     * @param wholeWord, if true only whole word will be matched, e.g. in "foobar foo bar", if the pattern is "foo", then only the second "foo" will be matched
     * @param regexp, if true the word pattern is considered as a regex
     * @return the corresponding JTree
     */
    public static JTree getJTree(SearchManager.MatchingPositions files, final SciNotes editor, final TextBox statusbar,
                                 String base, boolean recursive, boolean ignoreCR,
                                 String filePattern, boolean fileCaseSensitive,
                                 String wordPattern, boolean wordCaseSensitive, boolean wholeWord, boolean regexp) {
        if (files == null) {
            List<SearchManager.MatchingPositions> list = new ArrayList<SearchManager.MatchingPositions>();
            files = new SearchManager.MatchingPositions(new File(base).getAbsolutePath(), list);
            files.setRoot();
        }

        Pattern word = null;
        if (wordPattern != null && wordPattern.length() != 0) {
            word = SearchManager.generatePattern(wordPattern, wordCaseSensitive, wholeWord, regexp);
        }
        final Pattern p = word;

        files.setRoot();
        final JTree tree = new JTree(files.toDefaultMutableTreeNode());
        MouseListener ml = new MouseAdapter() {
                public void mousePressed(MouseEvent e) {
                    int row = tree.getRowForLocation(e.getX(), e.getY());
                    if(row != -1) {
                        if (e.getClickCount() == 2) {
                            validNode(editor, p, tree.getPathForRow(row));
                        }
                    }
                }
            };
        tree.getInputMap().put(KeyStroke.getKeyStroke("ENTER"), "toggle");
        tree.addKeyListener(new KeyAdapter() {
                public void keyTyped(KeyEvent e) {
                    if (e.getKeyChar() == '\n') {
                        int row = tree.getMinSelectionRow();
                        if (row != -1) {
                            validNode(editor, p, tree.getPathForRow(row));
                        }
                    }
                }
            });
        tree.addMouseListener(ml);
        DefaultTreeCellRenderer renderer = new DefaultTreeCellRenderer() {
                public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected,
                                                              boolean expanded, boolean leaf, int row, boolean hasFocus) {
                    super.getTreeCellRendererComponent(tree, value, selected, expanded, leaf, row, hasFocus);
                    DefaultMutableTreeNode mtn = (DefaultMutableTreeNode) value;
                    SearchManager.Iconable pos = (SearchManager.Iconable) mtn.getUserObject();
                    this.setIcon(pos.getIcon());
                    return this;
                }
            };
        if (statusbar != null) {
            tree.addTreeSelectionListener(new TreeSelectionListener() {
                    public void valueChanged(TreeSelectionEvent e) {
                        TreePath path = e.getNewLeadSelectionPath();
                        Object userObj = ((DefaultMutableTreeNode) path.getLastPathComponent()).getUserObject();
                        if (userObj instanceof SearchManager.MatchingPositions) {
                            statusbar.setText(((SearchManager.MatchingPositions) userObj).getFileName());
                        } else if (userObj instanceof SearchManager.Line) {
                            userObj = ((DefaultMutableTreeNode) ((DefaultMutableTreeNode) path.getLastPathComponent()).getParent()).getUserObject();
                            statusbar.setText(((SearchManager.MatchingPositions) userObj).getFileName());
                        }
                    }
                });
        }
        tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
        tree.setCellRenderer(renderer);

        return tree;
    }

    /**
     * Display a window containing the results of the search.
     * @param component the component which start the search. It is prevented that search is done via a firePropertyChange (SEARCHDONE).
     * @param editor the editor where to open a file
     * @param base the base directory
     * @param recursive, if true then a recursive search is made
     * @param ignoreCR, if true then the read file is considered as one line and regex pattern can include \n
     * @param filePattern the pattern to use to select the files. * is equivalent to .* and ? to .?
     * @param fileCaseSensitive, if true then the file pattern is case sensitive
     * @param wordPattern the pattern of the word to search
     * @param wordCaseSensitive, if true then the word pattern is case sensitive
     * @param wholeWord, if true only whole word will be matched, e.g. in "foobar foo bar", if the pattern is "foo", then only the second "foo" will be matched
     * @param regexp, if true the word pattern is considered as a regex
     * @return a key, can be used to stop the search
     */
    public static Object getSearchResultsWindow(JComponent component, SciNotes editor, String base,
                                                boolean recursive, boolean ignoreCR,
                                                String filePattern, boolean fileCaseSensitive,
                                                String wordPattern, boolean wordCaseSensitive, boolean wholeWord, boolean regexp) {
        MyBackgroundSearch searcher = new MyBackgroundSearch();
        searcher.start(component, editor, base, recursive, ignoreCR, filePattern, fileCaseSensitive, wordPattern, wordCaseSensitive, wholeWord, regexp);
        return searcher;
    }

    /**
     * Stop the current search
     * @param searcher the key returned by getSearchResultsWindow
     */
    public static void stopSearch(Object searcher) {
        if (searcher != null && (searcher instanceof MyBackgroundSearch)) {
            ((MyBackgroundSearch) searcher).stop();
        }
    }

    /**
     * Stop the current search
     * @param searcher the key returned by getSearchResultsWindow
     */
    public static boolean isDone(Object searcher) {
        if (searcher != null && (searcher instanceof MyBackgroundSearch)) {
            return ((MyBackgroundSearch) searcher).isDone();
        }
        return true;
    }

    /**
     * If the user hits ENTER key or double-click on a node, the corresponding file is open in SciNotes.
     * @param editor the editor where to open the file
     * @param the word pattern used
     * @param path the path of the node
     */
    private static void validNode(SciNotes editor, final Pattern pat, TreePath path) {
        DefaultMutableTreeNode node = (DefaultMutableTreeNode) path.getLastPathComponent();
        Object userObj = node.getUserObject();
        int lineNumber = -1;
        String fileName = null;
        boolean line = false;
        if (userObj instanceof SearchManager.MatchingPositions) {
            SearchManager.MatchingPositions pos = (SearchManager.MatchingPositions) userObj;
            lineNumber = 0;
            if (!pos.isDirectory()) {
                fileName = pos.getFileName();
            }
        } else if (userObj instanceof SearchManager.Line) {
            SearchManager.Line l = (SearchManager.Line) userObj;
            lineNumber = l.getNumber();
            fileName = ((SearchManager.MatchingPositions) ((DefaultMutableTreeNode) node.getParent()).getUserObject()).getFileName();
            line = true;
        }

        if (fileName != null) {
            final boolean fline = !line;
            final int ln = lineNumber;
            if (lineNumber != -1) {
                editor.openFile(fileName, 0, null);
                final ScilabEditorPane sep = editor.getTextPane();
                if (sep.getName().equals(fileName)) {
                    SwingUtilities.invokeLater(new Runnable() {
                            public void run() {
                                sep.highlightWords(pat, fline);
                                if (ln != 0) {
                                    sep.scrollTextToLineNumber(ln, false, false, true);
                                }
                            }
                        });
                }
            }
        }
    }

    /**
     * Inner class to allow the make the search as a background task (with a SwingWorker).
     * The task does not block SciNotes and can be killed by the user.
     */
    private static class MyBackgroundSearch extends SearchManager.BackgroundSearch {

        SciNotes editor;
        JComponent component;
        String base;
        boolean recursive;
        boolean ignoreCR;
        String filePattern;
        boolean fileCaseSensitive;
        String wordPattern;
        boolean wordCaseSensitive;
        boolean wholeWord;
        boolean regexp;

        /**
         * Default constructor
         */
        public MyBackgroundSearch() {
            super();
        }

        /**
         * Starts the search
         * @param component the component which start the search. It is prevented that search is done via a firePropertyChange (SEARCHDONE).
         * @param editor the editor where to open a file
         * @param base the base directory
         * @param recursive, if true then a recursive search is made
         * @param ignoreCR, if true then the read file is considered as one line and regex pattern can include \n
         * @param filePattern the pattern to use to select the files. * is equivalent to .* and ? to .?
         * @param fileCaseSensitive, if true then the file pattern is case sensitive
         * @param wordPattern the pattern of the word to search
         * @param wordCaseSensitive, if true then the word pattern is case sensitive
         * @param wholeWord, if true only whole word will be matched, e.g. in "foobar foo bar", if the pattern is "foo", then only the second "foo" will be matched
         * @param regexp, if true the word pattern is considered as a regex
         */
        synchronized public void start(JComponent component, SciNotes editor, String base, boolean recursive, boolean ignoreCR,
                                       String filePattern, boolean fileCaseSensitive,
                                       String wordPattern, boolean wordCaseSensitive, boolean wholeWord, boolean regexp) {
            this.component = component;
            this.editor = editor;
            this.base = base;
            this.recursive = recursive;
            this.ignoreCR = ignoreCR;
            this.filePattern = filePattern;
            this.fileCaseSensitive = fileCaseSensitive;
            this.wordPattern = wordPattern;
            this.wordCaseSensitive = wordCaseSensitive;
            this.wholeWord = wholeWord;
            this.regexp = regexp;
            SearchManager.searchInFiles(this, base, recursive, ignoreCR, filePattern, fileCaseSensitive, wordPattern, wordCaseSensitive, wholeWord, regexp);
        }

        /**
         * Stop the search and fire a propertyChange
         */
        synchronized public void stop() {
            super.stop();
            if (component != null) {
                component.firePropertyChange(SEARCHDONE, false, true);
            }
        }

        /**
         * Called when the results are available
         */
        synchronized public void done() {
            SearchManager.MatchingPositions pos = getResults();
            if (pos == null) {
                return;
            }

            final TextBox infobar = ScilabTextBox.createTextBox();
            final JTree tree = getJTree(pos, editor, infobar, base, recursive, ignoreCR, filePattern, fileCaseSensitive, wordPattern, wordCaseSensitive, wholeWord, regexp);

            SearchFile tab;
            String title;
            boolean preexist = SearchFile.instance != null;
            if (wordPattern == null || wordPattern.isEmpty()) {
                title = SciNotesMessages.SEARCHFILES;
            } else {
                title = SciNotesMessages.SEARCHINFILES;
            }

            title += " " + SciNotesMessages.FILEPATTERN + ": " + filePattern;
            title += ", " + SciNotesMessages.FILECASESENSITIVE + ": " + fileCaseSensitive;
            title += ", " + SciNotesMessages.RECURSIVE + ": " + recursive;
            if (wordPattern != null && !wordPattern.isEmpty()) {
                title += ", " + SciNotesMessages.LINEBYLINE + ": " + !ignoreCR;
                if (regexp) {
                    title += ", " + SciNotesMessages.REGEXPATTERN + ": " + wordPattern;
                } else {
                    title += ", " + SciNotesMessages.WORD + ": " + wordPattern;
                }
                title += ", " + SciNotesMessages.WORDCASESENSITIVE + ": " + wordCaseSensitive;
                title += ", " + SciNotesMessages.WHOLE_WORD + ": " + wholeWord;
            }

            Window parentWindow;

            if (!preexist) {
                tab = new SearchFile(editor, title);
                parentWindow = tab.getParentWindow();
                parentWindow.setTitle(title);
                parentWindow.addTab(tab);
                tab.setWindowIcon(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/32x32/apps/system-search.png").getImage());
            } else {
                tab = instance;
                parentWindow = tab.getParentWindow();
            }

            tab.updateUI();

            final SearchFile ftab = tab;
            CallBack callback = new CallBack(null) {
                    public void callBack() {
                        ftab.closeCurrent();
                    }

                    public void actionPerformed(ActionEvent e) {
                        callBack();
                    }
                };

            tab.setCallback(callback);
            MenuBar menubar = ScilabMenuBar.createMenuBar();
            Menu fileMenu = ScilabMenu.createMenu();
            fileMenu.setText(SciNotesMessages.FILE);
            fileMenu.setMnemonic('F');
            MenuItem menu = ScilabMenuItem.createMenuItem();
            menu.setCallback(callback);
            ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(SciNotesGUI.getActionKeyMap().get("ExitAction"));
            menu.setText(SciNotesMessages.EXIT);
            fileMenu.add(menu);
            menubar.add(fileMenu);

            tab.addMenuBar(menubar);
            tab.addInfoBar(infobar);

            JScrollPane scrollpane = new JScrollPane();
            scrollpane.setViewportView(tree);
            tab.setContentPane(scrollpane);

            if (!preexist) {
                tab.setPreferredSize(new Dimension(650, 250));
                ((SwingScilabWindow) parentWindow.getAsSimpleWindow()).pack();
                ((SwingScilabWindow) parentWindow.getAsSimpleWindow()).setVisible(true);
            }

            SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        tree.addSelectionRow(0);
                        tree.requestFocus();
                        long time = getElapsedTime();
                        String msg = (time <= 1000) ? SciNotesMessages.ELAPSEDTIMELESSONESEC : SciNotesMessages.ELAPSEDTIME;
                        infobar.setText(String.format(msg, ((double) time) / 1000));
                    }
                });

            if (component != null) {
                component.firePropertyChange(SEARCHDONE, false, true);
            }
        }
    }
}
