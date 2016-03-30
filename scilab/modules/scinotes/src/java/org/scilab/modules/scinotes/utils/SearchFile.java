/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.scinotes.utils;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;
import java.util.regex.Pattern;

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
import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.xml.ScilabXMLUtilities;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.SciNotesGUI;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.SearchManager;
import org.scilab.modules.scinotes.tabfactory.SearchInFilesTabFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

/**
 * Class SearchFile: open a window with a JTree to show the results of a search in files.
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class SearchFile extends SwingScilabDockablePanel {

    public static final String SEARCHDONE = "SearchFile.SearchDone";

    static {
        ScilabTabFactory.getInstance().addTabFactory(SearchInFilesTabFactory.getInstance());
    }

    private SwingScilabWindow parentWindow;
    private SciNotes editor;
    private MyBackgroundSearch mySearch;
    private JTree tree;

    /**
     * Default constructor
     * @param editor the editor where to open a file
     * @param uuid the tab uuid
     */
    public SearchFile(SciNotes editor, String uuid) {
        super(SciNotesMessages.SEARCHINFILES, uuid == null ? UUID.randomUUID().toString() : uuid);
        this.editor = editor;
        editor.addSearchInFiles(this);
        ConfigSciNotesManager.saveSearchInFilesState(editor.getPersistentId(), getPersistentId());
        initTab();
        restoreSearchFile();
        WindowsConfigurationManager.restorationFinished(this);
    }

    /**
     * Default constructor
     * @param editor the editor where to open a file
     */
    public SearchFile(SciNotes editor) {
        this(editor, null);
    }

    /**
     * Set the parent window
     */
    public void setParentWindow() {
        parentWindow = SwingScilabWindow.createWindow(true);
        parentWindow.addTab(this);
        parentWindow.setVisible(true);
    }

    /**
     * Get the parent window for this tab
     * @return the parent window
     */
    public SwingScilabWindow getParentWindow() {
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
    @Override
    public void dockingComplete(DockingEvent evt) {
        super.dockingComplete(evt);
        changeToolBar();
    }

    /**
     * Try to add a SciNotes toolbar
     */
    public void changeToolBar() {
        SwingScilabWindow win = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, this);
        Set<SwingScilabDockablePanel> set = win.getDockingPort().getDockables();
        for (SwingScilabDockablePanel tab : set) {
            if (tab == editor) {
                addToolBar(editor.getToolBar());
                break;
            }
        }
    }

    /**
     * Close the current window
     */
    public void closeSearchInFiles() {
        saveSearchFile();
        editor.removeSearchInFiles();
        editor = null;
        parentWindow = null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void undockingComplete(DockingEvent evt) {
        super.undockingComplete(evt);
        addToolBar(null);
    }

    /**
     * Set a MyBackgroundSearch to this SearchFile
     * @param search a MyBackgroundSearch
     */
    public void setMyBackgroundSearch(MyBackgroundSearch search) {
        mySearch = search;
    }

    private void saveSearchFile() {
        if (mySearch != null) {
            try {
                FileWriter fwriter = new FileWriter(ScilabConstants.SCIHOME.toString() + File.separator + getPersistentId() + ".xml");
                BufferedWriter buffer = new BufferedWriter(fwriter);
                buffer.append("<SearchResults editor=\"" + editor.getUUID() + "\"");
                buffer.append(" base=\"" + mySearch.base + "\"");
                buffer.append(" recursive=\"" + mySearch.recursive + "\"");
                buffer.append(" ignoreCR=\"" + mySearch.ignoreCR + "\"");
                buffer.append(" filePattern=\"" + ScilabXMLUtilities.getXMLString(mySearch.filePattern) + "\"");
                buffer.append(" fileCaseSensitive=\"" + mySearch.fileCaseSensitive + "\"");
                if (mySearch.wordPattern != null && !mySearch.wordPattern.isEmpty()) {
                    buffer.append(" wordPattern=\"" + ScilabXMLUtilities.getXMLString(mySearch.wordPattern) + "\"");
                }
                buffer.append(" wordCaseSensitive=\"" + mySearch.wordCaseSensitive + "\"");
                buffer.append(" wholeWord=\"" + mySearch.wholeWord + "\"");
                buffer.append(" regexp=\"" + mySearch.regexp + "\"");
                buffer.append(">\n");
                mySearch.getResults().toXML(buffer, 1);
                buffer.append("</SearchResults>");
                buffer.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private void restoreSearchFile() {
        File f = new File(ScilabConstants.SCIHOME + File.separator + getPersistentId() + ".xml");
        if (f.exists()) {
            Document doc = ScilabXMLUtilities.readDocument(f.getAbsolutePath());
            if (doc != null) {
                Element root = doc.getDocumentElement();
                NodeList list = root.getElementsByTagName("Position");
                SearchManager.MatchingPositions pos;
                if (list.getLength() != 0) {
                    pos = new SearchManager.MatchingPositions((Element) list.item(0));
                } else {
                    return;
                }

                String base = root.getAttribute("base");
                boolean recursive = Boolean.parseBoolean(root.getAttribute("recursive"));
                boolean ignoreCR = Boolean.parseBoolean(root.getAttribute("ignoreCR"));
                String filePattern = root.getAttribute("filePattern").replaceAll("&quot;", "\"");
                boolean fileCaseSensitive = Boolean.parseBoolean(root.getAttribute("fileCaseSensitive"));
                String wordPattern = root.getAttribute("filePattern").replaceAll("&quot;", "\"");
                boolean wordCaseSensitive = Boolean.parseBoolean(root.getAttribute("wordCaseSensitive"));
                boolean wholeWord = Boolean.parseBoolean(root.getAttribute("wholeWord"));
                boolean regexp = Boolean.parseBoolean(root.getAttribute("regexp"));

                fillTab(pos, base, recursive, ignoreCR, filePattern, fileCaseSensitive, wordPattern, wordCaseSensitive, wholeWord, regexp);
            }
        }
    }

    public void fillTab(SearchManager.MatchingPositions pos, String base, boolean recursive, boolean ignoreCR,
                        String filePattern, boolean fileCaseSensitive, String wordPattern,
                        boolean wordCaseSensitive, boolean wholeWord, boolean regexp) {
        String title;

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

        tree = getJTree(pos, editor, getInfoBar(), base, recursive, ignoreCR, filePattern, fileCaseSensitive, wordPattern, wordCaseSensitive, wholeWord, regexp);

        setTitle(title);
        JScrollPane scrollpane = new JScrollPane();
        setContentPane(scrollpane);
        scrollpane.setViewportView(tree);
        setVisible(true);
    }

    /**
     * Initialize the tab
     */
    private void initTab() {
        final TextBox infobar = ScilabTextBox.createTextBox();
        setWindowIcon("system-search");
        updateUI();

        CommonCallBack callback = new CommonCallBack(null) {
            @Override
            public void callBack() {
                ClosingOperationsManager.startClosingOperation((SwingScilabDockablePanel) SearchFile.this);
            }

            @Override
            public void actionPerformed(ActionEvent e) {
                callBack();
            }
        };

        setCallback(callback);
        MenuBar menubar = ScilabMenuBar.createMenuBar();
        Menu fileMenu = ScilabMenu.createMenu();
        fileMenu.setText(SciNotesMessages.FILE);
        fileMenu.setMnemonic('F');
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(callback);
        ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(SciNotes.getActionKeys().get("scinotes-exit"));
        menu.setText(SciNotesMessages.EXIT);
        fileMenu.add(menu);
        menubar.add(fileMenu);

        addMenuBar(menubar);
        addInfoBar(infobar);

        setPreferredSize(new Dimension(650, 250));
    }

    /**
     * @return the JTree int this tab
     */
    public JTree getJTree() {
        return tree;
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
            @Override
            public void mousePressed(MouseEvent e) {
                int row = tree.getRowForLocation(e.getX(), e.getY());
                if (row != -1) {
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
            @Override
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
                @Override
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
        MyBackgroundSearch searcher = new MyBackgroundSearch(component, editor, base, recursive, ignoreCR, filePattern, fileCaseSensitive, wordPattern, wordCaseSensitive, wholeWord, regexp);
        searcher.start();
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
                        @Override
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
        public MyBackgroundSearch(JComponent component, SciNotes editor, String base, boolean recursive, boolean ignoreCR,
                                  String filePattern, boolean fileCaseSensitive,
                                  String wordPattern, boolean wordCaseSensitive, boolean wholeWord, boolean regexp) {
            super();
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
        }

        /**
         * Starts the search
         */
        synchronized public void start() {
            SearchManager.searchInFiles(this, base, recursive, ignoreCR, filePattern, fileCaseSensitive, wordPattern, wordCaseSensitive, wholeWord, regexp);
        }

        /**
         * Stop the search and fire a propertyChange
         */
        @Override
        synchronized public void stop() {
            super.stop();
            if (component != null) {
                component.firePropertyChange(SEARCHDONE, false, true);
            }
        }

        /**
         * Called when the results are available
         */
        @Override
        synchronized public void done() {
            SearchManager.MatchingPositions pos = getResults();
            if (pos == null) {
                if (component != null) {
                    component.firePropertyChange(SEARCHDONE, false, true);
                }
                return;
            }

            SearchFile searchFile = editor.getSearchInFiles();
            final SearchFile sf;
            if (searchFile == null) {
                editor.addSearchInFiles();
                sf = editor.getSearchInFiles();
            } else {
                sf = searchFile;
            }

            sf.fillTab(pos, base, recursive, ignoreCR, filePattern, fileCaseSensitive, wordPattern, wordCaseSensitive, wholeWord, regexp);
            sf.setMyBackgroundSearch(this);

            SwingUtilities.invokeLater(new Runnable() {
                @Override
                public void run() {
                    sf.getJTree().addSelectionRow(0);
                    sf.getJTree().requestFocus();
                    long time = getElapsedTime();

                    sf.getInfoBar().setText(String.format(SciNotesMessages.ELAPSEDTIME, ((double) time) / 1000));
                }
            });

            if (component != null) {
                component.firePropertyChange(SEARCHDONE, false, true);
            }
        }
    }
}
