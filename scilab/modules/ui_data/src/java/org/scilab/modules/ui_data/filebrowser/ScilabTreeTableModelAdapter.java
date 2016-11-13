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

import javax.swing.table.AbstractTableModel;
import javax.swing.JTree;
import javax.swing.tree.TreePath;
import javax.swing.event.TreeExpansionEvent;
import javax.swing.event.TreeExpansionListener;

/**
 * The tree table model abstract implementation
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ScilabTreeTableModelAdapter extends AbstractTableModel {

    private JTree tree;
    private ScilabTreeTableModel treeTableModel;

    /**
     * Default constructor
     * @param treeTableModel the table model
     * @param tree the tree
     */
    public ScilabTreeTableModelAdapter(ScilabTreeTableModel treeTableModel, JTree tree) {
        this.tree = tree;
        this.treeTableModel = treeTableModel;
        tree.addTreeExpansionListener(new TreeExpansionListener() {
            public void treeExpanded(TreeExpansionEvent event) {
                fireTableDataChanged();
            }
            public void treeCollapsed(TreeExpansionEvent event) {
                fireTableDataChanged();
            }
        });
    }

    /**
     * {@inheritdoc}
     */
    public int getColumnCount() {
        return treeTableModel.getColumnCount();
    }

    /**
     * {@inheritdoc}
     */
    public String getColumnName(int column) {
        return treeTableModel.getColumnName(column);
    }

    /**
     * {@inheritdoc}
     */
    public Class getColumnClass(int column) {
        return treeTableModel.getColumnClass(column);
    }

    /**
     * {@inheritdoc}
     */
    public int getRowCount() {
        return tree.getRowCount();
    }

    /**
     * {@inheritdoc}
     */
    protected Object nodeForRow(int row) {
        TreePath treePath = tree.getPathForRow(row);
        return treePath.getLastPathComponent();
    }

    /**
     * {@inheritdoc}
     */
    public Object getValueAt(int row, int column) {
        return treeTableModel.getValueAt(nodeForRow(row), column);
    }

    /**
     * {@inheritdoc}
     */
    public boolean isCellEditable(int row, int column) {
        return treeTableModel.isCellEditable(nodeForRow(row), column);
    }

    /**
     * {@inheritdoc}
     */
    public void setValueAt(Object value, int row, int column) {
        treeTableModel.setValueAt(value, nodeForRow(row), column);
    }
}
