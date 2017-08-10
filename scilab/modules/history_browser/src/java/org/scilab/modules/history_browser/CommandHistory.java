/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2011 - Calixte DENIZET
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

package org.scilab.modules.history_browser;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Collections;

import javax.swing.BoundedRangeModel;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.Timer;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.ExpandVetoException;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBox;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.history_browser.actions.ClearAction;
import org.scilab.modules.history_browser.actions.CloseAction;
import org.scilab.modules.history_browser.actions.CopyAction;
import org.scilab.modules.history_browser.actions.CutAction;
import org.scilab.modules.history_browser.actions.DeleteAction;
import org.scilab.modules.history_browser.actions.EditInScinotesAction;
import org.scilab.modules.history_browser.actions.EvaluateAction;
import org.scilab.modules.history_browser.actions.HelpAction;
import org.scilab.modules.history_browser.actions.PrefsAction;
import org.scilab.modules.history_manager.HistoryManagement;
import org.scilab.modules.localization.Messages;

/**
 * Main Scilab Command History GUI
 * @author Vincent COUVERT
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class CommandHistory extends SwingScilabDockablePanel implements SimpleTab {

    public static final String COMMANDHISTORYUUID = "856207f6-0a60-47a0-b9f4-232feedd4bf4";

    private static final int DEFAULT_WIDTH = 450;
    private static final int DEFAULT_HEIGHT = 550;
    private static final String NEWLINE = "\n";
    private static final String SESSION_BEGINNING = "// -- ";
    private static final String SESSION_ENDING = " -- //";

    private static HistoryTree scilabHistoryTree;
    private static DefaultMutableTreeNode scilabHistoryRootNode;
    private static DefaultMutableTreeNode currentSessionNode;
    private static DefaultTreeModel scilabHistoryTreeModel;
    private static SwingScilabDockablePanel browserTab;
    private static JScrollPane scrollPane;

    private static boolean modelLoaded;
    private static boolean initialized;

    private static java.util.List<String> linesToAppend;
    private static javax.swing.Timer linesToAppendTimer;

    static {
        ScilabTabFactory.getInstance().addTabFactory(CommandHistoryTabFactory.getInstance());

        linesToAppend = Collections.synchronizedList(new ArrayList<String>());
        linesToAppendTimer = new Timer(0, new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                appendLinesOnEDT();
            }
        });
        linesToAppendTimer.setRepeats(false);
    }

    /**
     * Constructor
     */
    private CommandHistory() {
        super(CommandHistoryMessages.TITLE, COMMANDHISTORYUUID);
        setAssociatedXMLIDForHelp("browsehistory");
        initialize();
        addMenuBar(createMenuBar());
        addToolBar(createToolBar());
        addInfoBar(ScilabTextBox.createTextBox());

        scilabHistoryTree.addMouseListener(new CommandHistoryMouseListener());

        DeleteAction.registerKeyAction();
        EvaluateAction.registerKeyAction();
        CopyAction.registerKeyAction();
        CutAction.registerKeyAction();
        CloseAction.registerKeyAction();

        scrollPane = new JScrollPane(scilabHistoryTree);
        JPanel contentPane = new JPanel(new BorderLayout());
        contentPane.add(scrollPane);
        setContentPane(contentPane);
    }

    /**
     * Initialize the History Browser at Scilab launch
     * Called directly from Scilab
     */
    public static void initialize() {
        if (!initialized) {
            scilabHistoryRootNode = new DefaultMutableTreeNode(Messages.gettext("History loading in progress..."));
            scilabHistoryTreeModel = new DefaultTreeModel(scilabHistoryRootNode);
            scilabHistoryTree = new HistoryTree(scilabHistoryTreeModel);
            scilabHistoryTree.setShowsRootHandles(true);
            scilabHistoryTree.setDragEnabled(true);
            scilabHistoryTree.setEnabled(true);
            scilabHistoryTree.setRootVisible(false);
            scilabHistoryTree.setScrollsOnExpand(true);
            scilabHistoryTree.setVisible(false);

            // Under Windows the directory icon is used: bad....
            DefaultTreeCellRenderer renderer = (DefaultTreeCellRenderer) scilabHistoryTree.getCellRenderer();
            renderer.setLeafIcon(null);
            renderer.setClosedIcon(null);
            renderer.setOpenIcon(null);

            initialized = true;
        }
    }

    /**
     * Create a new Command History tab
     * @return the corresponding tab
     */
    public static SwingScilabDockablePanel createCommandHistoryTab() {
        browserTab = new CommandHistory();
        WindowsConfigurationManager.restorationFinished(browserTab);

        return browserTab;
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
    public SwingScilabWindow getParentWindow() {
        return SwingScilabWindow.allScilabWindows.get(getParentWindowId());
    }

    /**
     * {@inheritDoc}
     */
    public SimpleTab getAsSimpleTab() {
        return this;
    }

    /**
     * Update the browser once an history file has been loaded
     */
    public static void loadFromFile() {
        final String historyLines[] = HistoryManagement.getAllLinesOfScilabHistory();

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                reset();
                int nbEntries = historyLines.length;
                for (int entryIndex = 0; entryIndex < nbEntries; entryIndex++) {
                    /* Do not expand at each insertion for performances reasons */
                    appendLineAndExpand(historyLines[entryIndex], false);
                }
            }
        });

        /* Expand all sessions tree */
        expandAll();
    }

    public static void expandAll() {
        if (isHistoryVisible()) {
            // put the expansion in an invokeLater to avoid some kind of freeze with huge history
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    scilabHistoryTree.setVisible(true);
                    if (!modelLoaded) {
                        scilabHistoryTreeModel.nodeStructureChanged((TreeNode) scilabHistoryTreeModel.getRoot());
                        modelLoaded = true;
                    }

                    final Object root = scilabHistoryTreeModel.getRoot();
                    final TreePath pathRoot = new TreePath(root);
                    final int N = scilabHistoryTreeModel.getChildCount(root);
                    scilabHistoryTree.mustFire = false;
                    for (int i = 0; i < N; i++) {
                        Object o = scilabHistoryTreeModel.getChild(root, i);
                        if (!scilabHistoryTreeModel.isLeaf(o)) {
                            scilabHistoryTree.expandPath(pathRoot.pathByAddingChild(o));
                        }
                    }
                    scilabHistoryTree.mustFire = true;
                    scilabHistoryTree.fireTreeExpanded(pathRoot);

                    scrollAtBottom();
                }
            });
        }
    }

    /**
     * Add a new line to the History Browser
     * @param lineToAppend the line to append
     */
    public static void appendLine(String lineToAppend) {
        synchronized (linesToAppend) {
            linesToAppend.add(lineToAppend);
            linesToAppendTimer.start();
        }
    }

    public static void appendLinesOnEDT() {
        synchronized (linesToAppend) {
            for (String lineToAppend : linesToAppend) {
                appendLineAndExpand(lineToAppend, true);
            }
            linesToAppend.clear();
        }
    }

    /**
     * check if line is a begin session
     * @param line to check
     * @retour true or false
     */
    private static boolean isBeginSessionLine(String lineToAppend) {
        if (lineToAppend.startsWith(SESSION_BEGINNING) && lineToAppend.endsWith(SESSION_ENDING)) {
            return true;
        }
        return false;
    }

    /**
     * Add a new line to the History Browser
     * @param lineToAppend the line to append
     * @param expand do we need to expand all session nodes?
     */
    public static void appendLineAndExpand(String lineToAppend, boolean expand) {
        if (isBeginSessionLine(lineToAppend)) {
            // Create a new session node
            currentSessionNode = new DefaultMutableTreeNode(new SessionString(lineToAppend));
            scilabHistoryTreeModel.insertNodeInto(currentSessionNode, scilabHistoryRootNode, scilabHistoryRootNode.getChildCount());
            if (expand && isHistoryVisible()) {
                scilabHistoryTree.expandRow(scilabHistoryTree.getRowCount() - 1);
                scilabHistoryTree.scrollPathToVisible(new TreePath(currentSessionNode.getPath()));
            }
        } else {
            boolean mustScroll = false;
            if (expand && isHistoryVisible()) {
                JScrollBar vb = scrollPane.getVerticalScrollBar();
                if (vb != null) {
                    BoundedRangeModel model = vb.getModel();
                    // mustScroll is true if the knob is at the bottom of the scollbar.
                    mustScroll = model.getValue() == model.getMaximum() - model.getExtent();
                }
            }
            DefaultMutableTreeNode childNode = new DefaultMutableTreeNode(lineToAppend);
            scilabHistoryTreeModel.insertNodeInto(childNode, currentSessionNode, currentSessionNode.getChildCount());
            if (expand && isHistoryVisible()) {
                scilabHistoryTree.expandRow(scilabHistoryTree.getRowCount() - 1);
                if (mustScroll) {
                    scilabHistoryTree.scrollPathToVisible(new TreePath(childNode.getPath()));
                }
            }
        }
    }

    /**
     * Reset the History Browser (after a clear)
     */
    public static void reset() {
        scilabHistoryRootNode.removeAllChildren();
        scilabHistoryTreeModel.reload();
        currentSessionNode = null;
    }

    /**
     * Remove an entry from History
     * @param lineNumber the number of the line
     */
    public static void deleteLine(int lineNumber) {
        int numberOfSessions = scilabHistoryRootNode.getChildCount();
        int sessionIndex = 0;
        int numberOfLines = 0;

        while (sessionIndex < numberOfSessions) {
            if (numberOfLines == lineNumber) {
                // Do we try to delete the last session node (which it is the current session used)
                if (sessionIndex == (numberOfSessions - 1)) {
                    // Yes, we forbid it
                    MessageBox errorMsg = ScilabMessageBox.createMessageBox();
                    errorMsg.setTitle(CommandHistoryMessages.ERROR);
                    errorMsg.setMessage(CommandHistoryMessages.CANNOT_DELETE_CURRENT_SESSION_NODE);
                    errorMsg.setIcon("error");
                    errorMsg.displayAndWait();
                    return;
                }

                // Otherwise it is OK, delete session node
                TreeNode sessionNode = scilabHistoryRootNode.getChildAt(sessionIndex);
                scilabHistoryRootNode.remove(sessionIndex);
                scilabHistoryTreeModel.nodesWereRemoved(scilabHistoryRootNode, new int[] {sessionIndex},
                                                        new Object[] {sessionNode});
                break;
            }

            DefaultMutableTreeNode sessionNode = (DefaultMutableTreeNode) scilabHistoryRootNode.getChildAt(sessionIndex);

            // Did we reach the session containing the line to remove ?
            if (numberOfLines + sessionNode.getChildCount() >= lineNumber) {
                // Yes, delete the child line
                int childIndex = lineNumber - numberOfLines - 1;
                TreeNode childNode = sessionNode.getChildAt(childIndex);
                sessionNode.remove(childIndex);
                scilabHistoryTreeModel.nodesWereRemoved(sessionNode, new int[] {childIndex},
                                                        new Object[] {childNode} );
                break;
            } else {
                /* No, jump to next session */
                numberOfLines += sessionNode.getChildCount() + 1;
                sessionIndex++;
            }
        }
    }

    /**
     * Close the tab
     */
    public static void closeHistory() {
        browserTab = null;
    }

    /**
     * @return the browserTab
     */
    public static SwingScilabDockablePanel getBrowserTab() {
        return browserTab;
    }

    /**
     * Manage History Browser visibility
     */
    public static void setVisible() {
        if (browserTab == null) {
            boolean success = WindowsConfigurationManager.restoreUUID(COMMANDHISTORYUUID);
            if (!success) {
                CommandHistoryTabFactory.getInstance().getTab(COMMANDHISTORYUUID);
                SwingScilabWindow window = SwingScilabWindow.createWindow(true);
                window.addTab(browserTab);
                window.setLocation(0, 0);
                window.setSize(500, 500);
                window.setVisible(true);
            }
        }
        browserTab.setVisible(true);
        expandAll();
    }

    /**
     * Launch the history browser
     */
    public static void launchHistoryBrowser() {
        setVisible();
    }

    /**
     * Get History Browser visibility
     * @return visibility status
     */
    private static boolean isHistoryVisible() {
        return browserTab != null && browserTab.isVisible();
    }

    /**
     * Create History Browser MenuBar
     * @return the menu bar
     */
    private static MenuBar createMenuBar() {
        MenuBar menuBar = ScilabMenuBar.createMenuBar();

        Menu fileMenu = ScilabMenu.createMenu();
        fileMenu.setText(CommandHistoryMessages.FILE);
        fileMenu.setMnemonic('F');

        fileMenu.add(CloseAction.createMenuItem());

        menuBar.add(fileMenu);

        Menu editMenu = ScilabMenu.createMenu();
        editMenu.setText(CommandHistoryMessages.EDIT);
        editMenu.setMnemonic('E');

        editMenu.add(CopyAction.createMenuItem());
        editMenu.add(CutAction.createMenuItem());
        editMenu.add(EvaluateAction.createMenuItem());
        editMenu.add(EditInScinotesAction.createMenuItem());
        editMenu.addSeparator();
        editMenu.add(DeleteAction.createMenuItem());
        editMenu.add(ClearAction.createMenuItem());

        menuBar.add(editMenu);

        Menu helpMenu = ScilabMenu.createMenu();
        helpMenu.setText(CommandHistoryMessages.HELP);
        helpMenu.setMnemonic('?');

        helpMenu.add(HelpAction.createMenuItem());

        menuBar.add(helpMenu);

        return menuBar;
    }

    /**
     * Create History Browser ToolBar
     * @return the tool bar
     */
    private static ToolBar createToolBar() {
        ToolBar toolBar = ScilabToolBar.createToolBar();
        SwingScilabToolBar stb = (SwingScilabToolBar) toolBar.getAsSimpleToolBar();

        stb.add(CopyAction.createPushButton());
        stb.add(CutAction.createPushButton());
        stb.add(DeleteAction.createPushButton());

        stb.addSeparator();
        stb.add(PrefsAction.createPushButton());
        stb.addSeparator();

        stb.add(HelpAction.createPushButton());

        return toolBar;
    }

    /**
     * Get the JTree
     * @return the tree
     */
    public static JTree getTree() {
        return scilabHistoryTree;
    }

    /**
     * Get the JTree Model
     * @return the tree model
     */
    public static DefaultTreeModel getTreeModel() {
        return scilabHistoryTreeModel;
    }

    /**
     * Get the selected commands and store them into an "executable" string
     * @return the string
     */
    public static String getSelectedCommands() {
        TreePath[] selectedPaths = CommandHistory.getTree().getSelectionPaths();

        if (selectedPaths == null) {
            return null;
        }

        String selectedEntries = new String();

        for (int i = 0; i < selectedPaths.length; i++) {
            Object obj = ((DefaultMutableTreeNode) selectedPaths[i].getLastPathComponent()).getUserObject();
            selectedEntries += obj.toString();

            if (i < selectedPaths.length - 1) {
                selectedEntries += NEWLINE;
            }
        }

        return selectedEntries;
    }

    private static void scrollAtBottom() {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                scrollPane.getHorizontalScrollBar().setValue(0);
                scrollPane.getVerticalScrollBar().setValue(scrollPane.getVerticalScrollBar().getMaximum());
            }
        });

    }

    /**
     * Inner class to render the session nodes in green
     */
    static class SessionString {

        String s;

        SessionString(String s) {
            this.s = s;
        }

        public String toString() {
            return s;
        }
    }

    @SuppressWarnings(value = { "serial" })
    static class HistoryTree extends JTree {

        private boolean first = true;
        private Color defaultColor;
        private Color sessionColor = new Color(1, 168, 1);
        boolean mustFire = true;

        HistoryTree(TreeModel model) {
            super(model);
            setRowHeight(16);
            setLargeModel(true);

            setCellRenderer(new DefaultTreeCellRenderer() {
                {
                    defaultColor = getTextNonSelectionColor();
                }

                public Component getTreeCellRendererComponent(JTree tree, Object value, boolean sel, boolean expanded, boolean leaf, int row, boolean hasFocus) {
                    super.getTreeCellRendererComponent(tree, value, sel, expanded, leaf, row, hasFocus);
                    if (((DefaultMutableTreeNode) value).getUserObject() instanceof SessionString) {
                        setTextNonSelectionColor(sessionColor);
                    } else {
                        setTextNonSelectionColor(defaultColor);
                    }

                    return this;
                }
            });
        }

        public void fireTreeExpanded(TreePath path) {
            if (mustFire) {
                super.fireTreeExpanded(path);
            }
        }

        public void fireTreeWillExpand(TreePath path) throws ExpandVetoException {
            if (mustFire) {
                super.fireTreeWillExpand(path);
            }
        }

        public void paint(final Graphics g) {
            if (first) {
                g.setFont(getFont());
                int height = g.getFontMetrics().getHeight();
                setRowHeight(height);
                setLargeModel(true);
                first = false;
                scrollPane.getVerticalScrollBar().setUnitIncrement(height);
                scrollAtBottom();
            }
            try {
                super.paint(g);
            } catch (Exception e) {
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        paint(g);
                    }
                });
            }
        }
    }
}
