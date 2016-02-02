/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.explorer;

import java.awt.Dimension;
import java.io.IOException;
import java.util.List;
import java.util.ListIterator;
import java.util.UUID;

import javax.swing.JEditorPane;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.ScrollPaneConstants;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.html.HTMLDocument;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreePath;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.ClosingOperationsManager.ClosingOperation;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.xcos.BrowserView;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Tab of the Xcos Browser
 */
public class BrowserTab extends SwingScilabDockablePanel implements SimpleTab {
    public static final String DEFAULT_WIN_UUID = "xcos-browser-default-window";
    public static final String DEFAULT_TAB_UUID = "xcos-browser-default-tab";

    private final BrowserView view;

    /**
     * Construct a new tab for the explorer
     *
     * @param id
     *            The diagram ID to browse
     */
    public BrowserTab(BrowserView view, String uuid) {
        super(XcosMessages.DIAGRAM_BROWSER, uuid);

        this.view = view;

        initComponents();
    }

    /*
     * Static API for Tabs
     */

    public static void restore(final BrowserView view, String uuid) {
        if (uuid == null) {
            uuid = UUID.randomUUID().toString();
        }

        BrowserTab tab = new BrowserTab(view, uuid);
        tab.createDefaultWindow().setVisible(true);
        tab.setCurrent();

        ScilabTabFactory.getInstance().addToCache(tab);
        ClosingOperationsManager.addDependencyWithRoot(tab);
        ClosingOperationsManager.registerClosingOperation(tab, new ClosingOperation() {

            @Override
            public void updateDependencies(List<SwingScilabDockablePanel> list, ListIterator<SwingScilabDockablePanel> it) { }

            @Override
            public void destroy() {
                Xcos.getInstance().clearBrowser();
            }

            @Override
            public int canClose() {
                return 1;
            }

            @Override
            public String askForClosing(List<SwingScilabDockablePanel> list) {
                return null;
            }
        });
    }

    /*
     * Utilities inner classes
     */
    static private class UpdateEditorContentListener implements TreeSelectionListener {
        final HTMLDocument doc;

        public UpdateEditorContentListener(HTMLDocument doc) {
            this.doc = doc;
        }

        @Override
        public void valueChanged(TreeSelectionEvent e) {
            final TreePath p = e.getPath();
            BrowserTreeNodeData node = (BrowserTreeNodeData) ((DefaultMutableTreeNode) p.getLastPathComponent()).getUserObject();

            try {
                node.fillOrUpdateContent(doc);
            } catch (BadLocationException e1) {
                // TODO Auto-generated catch block
                e1.printStackTrace();
            } catch (IOException e1) {
                // TODO Auto-generated catch block
                e1.printStackTrace();
            }
        }
    }

    static private class ExplorerModelListener implements TreeModelListener {
        final private JTree tree;
        final HTMLDocument doc;

        public ExplorerModelListener(HTMLDocument doc, JTree tree) {
            this.tree = tree;
            this.doc = doc;
        }

        @Override
        public void treeNodesChanged(TreeModelEvent e) {
            if (tree.getSelectionModel().isPathSelected(e.getTreePath())) {
                final TreePath p = e.getTreePath();
                BrowserTreeNodeData node = (BrowserTreeNodeData) ((DefaultMutableTreeNode) p.getLastPathComponent()).getUserObject();

                try {
                    node.fillOrUpdateContent(doc);
                } catch (BadLocationException e1) {
                    // TODO Auto-generated catch block
                    e1.printStackTrace();
                } catch (IOException e1) {
                    // TODO Auto-generated catch block
                    e1.printStackTrace();
                }
            }
        }

        @Override
        public void treeNodesInserted(TreeModelEvent e) {
            if (e.getTreePath().getLastPathComponent() == tree.getModel().getRoot()) {
                tree.expandPath(e.getTreePath());
            }
        }

        @Override
        public void treeNodesRemoved(TreeModelEvent e) {
            // TODO Auto-generated method stub

        }

        @Override
        public void treeStructureChanged(TreeModelEvent e) {
            // TODO Auto-generated method stub

        }

    }

    /*
     * Specific implementation
     */

    private void initComponents() {
        final JTree tree = new JTree(view.getModel());
        tree.setRootVisible(false);

        final JEditorPane editor = new JEditorPane("text/html", "<html><head><style type=\"text/css\">table { width: 100%; }\ncaption { text-align: center;\nfont: bold; }\ntd { text-align: left; }\n</style></head><body /></html>");
        editor.setEditable(false);

        tree.addTreeSelectionListener(new UpdateEditorContentListener((HTMLDocument) editor.getDocument()));
        tree.getModel().addTreeModelListener(new ExplorerModelListener((HTMLDocument) editor.getDocument(), tree));

        JSplitPane pane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, new JScrollPane(tree, ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED, ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED), new JScrollPane(editor, ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED, ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED));
        setContentPane(pane);

        // default size
        Dimension preferredSize = new Dimension(161, 100);
        pane.getLeftComponent().setPreferredSize(preferredSize);
        pane.getRightComponent().setPreferredSize(preferredSize);
    }

    private SwingScilabWindow createDefaultWindow() {
        final SwingScilabWindow win;

        final SwingScilabWindow configuration = WindowsConfigurationManager.createWindow(DEFAULT_WIN_UUID, false);
        if (configuration != null) {
            win = configuration;
        } else {
            win = SwingScilabWindow.createWindow(true);
        }

        win.addTab(this);
        return win;
    }
}
