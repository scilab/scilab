/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.history_browser;

import java.awt.BorderLayout;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBox;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.history_manager.HistoryManagement;
import org.scilab.modules.history_browser.CommandHistoryMessages;
import org.scilab.modules.history_browser.CommandHistoryMouseListener;
import org.scilab.modules.history_browser.actions.ClearAction;
import org.scilab.modules.history_browser.actions.CloseAction;
import org.scilab.modules.history_browser.actions.CopyAction;
import org.scilab.modules.history_browser.actions.CutAction;
import org.scilab.modules.history_browser.actions.DeleteAction;
import org.scilab.modules.history_browser.actions.EditInScinotesAction;
import org.scilab.modules.history_browser.actions.EvaluateAction;
import org.scilab.modules.history_browser.actions.HelpAction;

/**
 * Main Scilab Command History GUI
 * @author Vincent COUVERT
 */
public final class CommandHistory {

        private static final int DEFAULT_WIDTH = 450;
        private static final int DEFAULT_HEIGHT = 550;
        private static final String NEWLINE = "\n";
        private static final String SESSION_BEGINNING = "// -- ";
        private static final String SESSION_ENDING = " -- //";

        private static JTree scilabHistoryTree;

        private static DefaultMutableTreeNode scilabHistoryRootNode;

        private static DefaultMutableTreeNode currentSessionNode;

        private static DefaultTreeModel scilabHistoryTreeModel;

        private static Tab browserTab;

        private static JScrollPane scrollPane;

        /**
         * Constructor
         */
        private CommandHistory() {
        }

        /**
         * Initialize the History Browser at Scilab launch
         * Called directly from Scilab
         */
        public static void initialize() {
                Window browserWindow = ScilabWindow.createWindow();
                browserWindow.setDims(new Size(DEFAULT_WIDTH, DEFAULT_HEIGHT));

                browserTab = ScilabTab.createTab(CommandHistoryMessages.TITLE);
                browserTab.setCallback(CloseAction.getCallBack());
                browserTab.addMenuBar(createMenuBar());
                browserTab.addToolBar(createToolBar());
                browserTab.addInfoBar(ScilabTextBox.createTextBox());

                browserWindow.addTab(browserTab);

                scilabHistoryRootNode = new DefaultMutableTreeNode(""); // Will not be visible
                scilabHistoryTreeModel = new DefaultTreeModel(scilabHistoryRootNode);
                scilabHistoryTree = new JTree(scilabHistoryTreeModel);
                scilabHistoryTree.setShowsRootHandles(true);
                scilabHistoryTree.setDragEnabled(true);
                scilabHistoryTree.setEnabled(true);
                scilabHistoryTree.setRootVisible(false);
                scilabHistoryTree.setScrollsOnExpand(true);

                scilabHistoryTree.addMouseListener(new CommandHistoryMouseListener());

                scrollPane = new JScrollPane(scilabHistoryTree);
                JPanel contentPane = new JPanel(new BorderLayout());
                contentPane.add(scrollPane);
                ((SwingScilabTab) browserTab.getAsSimpleTab()).setContentPane(contentPane);
                setVisible(false);
        }

        /**
         * Update the browser once an history file has been loaded
         */
        public static void loadFromFile() {
                reset();
                String historyLines[] = HistoryManagement.getAllLinesOfScilabHistory();
                int nbEntries = historyLines.length;
                for (int entryIndex = 0; entryIndex < nbEntries; entryIndex++) {
                   /* Do not expand at each insertion for performances reasons */
                   appendLineAndExpand(historyLines[entryIndex], false);
                }
                /* Expand all sessions tree */
                scilabHistoryTreeModel.nodeStructureChanged((TreeNode) scilabHistoryTreeModel.getRoot());
                for (int i = 0; i < scilabHistoryTree.getRowCount(); i++) {
                   scilabHistoryTree.expandRow(i);
                }
                scilabHistoryTree.scrollPathToVisible(scilabHistoryTree.getPathForRow(scilabHistoryTree.getRowCount() - 1));
        }

        /**
         * Add a new line the the History Browser
         * @param lineToAppend the line to append
         */
        public static void appendLine(String lineToAppend) {
                appendLineAndExpand(lineToAppend, true);
        }

        /**
        * check if line is a begin session
        * @param line to check
        * @retour true or false
        */
        private static boolean isBeginSessionLine(String lineToAppend) {
                if (lineToAppend.startsWith(SESSION_BEGINNING) &&
                    lineToAppend.endsWith(SESSION_ENDING)) {
                       return true;
                    }
               return false;
        }

        /**
         * Add a new line the the History Browser
         * @param lineToAppend the line to append
         * @param expand do we need to expand all session nodes?
         */
        public static void appendLineAndExpand(String lineToAppend, boolean expand) {
                if (isBeginSessionLine(lineToAppend)) {
                        // Create a new session node
                        currentSessionNode = new DefaultMutableTreeNode(lineToAppend);
                        scilabHistoryTreeModel.insertNodeInto(currentSessionNode, scilabHistoryRootNode, scilabHistoryRootNode.getChildCount());
                        if (expand) {
                                /* Expand all sessions tree */
                                for (int i = 0; i < scilabHistoryTree.getRowCount(); i++) {
                                        scilabHistoryTree.expandRow(i);
                                }
                                scilabHistoryTree.scrollPathToVisible(new TreePath(currentSessionNode.getPath()));
                                scilabHistoryTreeModel.nodeStructureChanged((TreeNode) scilabHistoryTreeModel.getRoot());
                        }
                } else {
                        DefaultMutableTreeNode childNode = new DefaultMutableTreeNode(lineToAppend);
                        scilabHistoryTreeModel.insertNodeInto(childNode, currentSessionNode, currentSessionNode.getChildCount());
                        if (expand) {
                                scilabHistoryTree.expandRow(scilabHistoryTree.getRowCount() - 1);
                                scilabHistoryTree.scrollPathToVisible(new TreePath(childNode.getPath()));
                                scilabHistoryTreeModel.nodeStructureChanged((TreeNode) currentSessionNode);
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
                                if (sessionIndex == (numberOfSessions - 1)) {
                                        /* Can not remove current session node */
                                        MessageBox errorMsg = ScilabMessageBox.createMessageBox();
                                        errorMsg.setTitle(CommandHistoryMessages.ERROR);
                                        errorMsg.setMessage(CommandHistoryMessages.CANNOT_DELETE_CURRENT_SESSION_NODE);
                                        errorMsg.setIcon("error");
                                        errorMsg.displayAndWait();
                                        return;
                                }
                                scilabHistoryRootNode.remove(sessionIndex);
                                scilabHistoryTreeModel.nodeStructureChanged((TreeNode) scilabHistoryTreeModel.getRoot());
                                break;
                        }

                        /* Session line */
                        numberOfLines++;

                        if (numberOfLines + scilabHistoryRootNode.getChildAt(sessionIndex).getChildCount() > lineNumber) {
                                /* The line has to be remove in current session */
                                ((DefaultMutableTreeNode) scilabHistoryRootNode.getChildAt(sessionIndex)).remove(lineNumber - numberOfLines);
                                scilabHistoryTreeModel.nodeStructureChanged((TreeNode) scilabHistoryRootNode.getChildAt(sessionIndex));
                                break;
                        } else {
                                /* An other session */
                                numberOfLines += scilabHistoryRootNode.getChildAt(sessionIndex).getChildCount();
                                sessionIndex++;
                        }

                }
        }

        /**
         * Manage History Browser visibility
         * @param status visibility status
         */
        public static void setVisible(boolean status) {
                if (status && browserTab.getParentWindow() == null) {
                    Window browserWindow = ScilabWindow.createWindow();
                    browserWindow.setVisible(true);
                    browserWindow.addTab(browserTab);
                }

                if (browserTab.getParentWindow() != null) {
                    if (browserTab.getParentWindow().getNbDockedObjects() == 1) {
                        browserTab.getParentWindow().setVisible(status);
                    } else {
                        DockingManager.undock((Dockable) browserTab.getAsSimpleTab());
                        browserTab.setParentWindowId(-1);
                    }
                }

                browserTab.setVisible(status);
        }

        /**
         * Get History Browser visibility
         * @return visibility status
         */
        private static boolean isVisible() {
                return browserTab.isVisible();
        }

        /**
         * Toggle History Browser visibility
         */
        public static void toggleVisibility() {
                setVisible(!isVisible());
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

                toolBar.add(CopyAction.createPushButton());
                toolBar.add(CutAction.createPushButton());
                toolBar.add(DeleteAction.createPushButton());

                toolBar.addSeparator();

                toolBar.add(HelpAction.createPushButton());

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
                        selectedEntries += selectedPaths[i].getLastPathComponent().toString();
                        if (i < selectedPaths.length - 1) {
                                selectedEntries += NEWLINE;
                        }
                }

                return selectedEntries;
        }
}
