/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.ui_data.filebrowser;

import java.awt.Component;
import java.awt.Container;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JTable;
import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.event.TreeExpansionEvent;
import javax.swing.event.TreeExpansionListener;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.event.TreeWillExpandListener;
import javax.swing.table.TableCellRenderer;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeSelectionModel;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;

import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.events.callback.CommonCallBack;

/**
 * The table renderer to render the tree in the first column of the JTable
 */
@SuppressWarnings(value = { "serial" })
public class ScilabTreeTableCellRenderer extends JTree implements TableCellRenderer {

    private SwingScilabTreeTable table;
    protected int visibleRow;

    /**
     * {@inheritdoc}
     */
    public ScilabTreeTableCellRenderer(SwingScilabTreeTable table, TreeModel model) {
        super(model);
        this.table = table;
        DefaultTreeCellRenderer renderer = new DefaultTreeCellRenderer() {

            public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {
                FileNode fn = (FileNode) value;
                if (leaf) {
                    if (fn instanceof ScilabFileBrowserModel.ParentNode) {
                        this.setLeafIcon(FileUtils.getUpDirIcon());
                    } else {
                        this.setLeafIcon(fn.getIcon());
                    }
                } else if (fn.isUserHome()) {
                    this.setClosedIcon(FileUtils.getClosedUserHomeIcon());
                    this.setOpenIcon(FileUtils.getOpenUserHomeIcon());
                } else if (fn.isSCI()) {
                    this.setClosedIcon(FileUtils.getSCIIcon());
                    this.setOpenIcon(FileUtils.getOpenSCIIcon());
                } else {
                    this.setClosedIcon(FileUtils.getClosedDirIcon());
                    this.setOpenIcon(FileUtils.getOpenDirIcon());
                }
                super.getTreeCellRendererComponent(tree, value, selected, expanded, leaf, row, hasFocus);

                return this;
            }
        };

        /* Force the JTable and JTree to share their row selection models.
           And let the table to handle the selection rather than the tree; */
        setSelectionModel(new DefaultTreeSelectionModel() {
            {
                ScilabTreeTableCellRenderer.this.table.setSelectionModel(listSelectionModel);
            }

            public void setSelectionPaths(TreePath[] pPaths) { }

            public void addSelectionPaths(TreePath[] paths) { }

            public void removeSelectionPaths(TreePath[] paths) { }
        });

        setCellRenderer(renderer);
        setRootVisible(true);
        setRowHeight(table.getRowHeight());
        setLargeModel(true);
        setEditable(true);
        setToggleClickCount(0);

        addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                int selRow = getRowForLocation(e.getX(), e.getY());
                TreePath selPath = getPathForLocation(e.getX(), e.getY());
                if (selRow != -1) {
                    switch (e.getClickCount()) {
                        case 1 :
                            /*int sel = SwingScilabTreeTable.this.getSelectedRow();
                              System.out.println(sel+":::"+selRow+":::"+isEditable());
                              if (sel == selRow) {
                              System.out.println(getCellEditor().getTreeCellEditorComponent(ScilabTreeTableCellRenderer.this, getCellEditor().getCellEditorValue(), true, false, true, selRow));
                              }
                              e.consume();*/
                            break;
                        case 2:
                            ((CommonCallBack) ScilabTreeTableCellRenderer.this.table.getActionMap().get("validate")).callBack();
                            e.consume();
                    }
                }
            }

            public void mouseReleased(MouseEvent e) { }
        });

        addTreeWillExpandListener(new TreeWillExpandListener() {
            public void treeWillCollapse(TreeExpansionEvent event) { }

            public void treeWillExpand(TreeExpansionEvent event) {
                Container win = SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, ScilabTreeTableCellRenderer.this.table);
                if (win != null) {
                    win.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
                }
            }
        });
        addTreeExpansionListener(new TreeExpansionListener() {
            public void treeCollapsed(TreeExpansionEvent event) { }

            public void treeExpanded(TreeExpansionEvent event) {
                Container win = SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, ScilabTreeTableCellRenderer.this.table);
                if (win != null) {
                    win.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
                }
            }
        });
    }

    /**
     * {@inheritDoc}
     */
    protected TreeModelListener createTreeModelListener() {
        return new TreeModelListener() {
            public void treeNodesChanged(TreeModelEvent e) { }

            public void treeNodesInserted(TreeModelEvent e) { }

            public void treeStructureChanged(TreeModelEvent e) { }

            public void treeNodesRemoved(TreeModelEvent e) { }
        };
    }

    /**
     * {@inheritdoc}
     */
    public void setBounds(int x, int y, int w, int h) {
        super.setBounds(x, 0, w, table.getHeight());
    }

    /**
     * {@inheritdoc}
     */
    public void paint(Graphics g) {
        g.translate(0, -visibleRow * getRowHeight());
        try {
            ui.update(g, this);
        } catch (NullPointerException e) {
            // Occurs sometimes...
            g.translate(0, visibleRow * getRowHeight());
            paint(g);
        }
    }

    /**
     * {@inheritdoc}
     */
    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        if (isSelected) {
            setBackground(table.getSelectionBackground());
        } else {
            setBackground(table.getBackground());
        }
        visibleRow = row;

        return this;
    }
}
