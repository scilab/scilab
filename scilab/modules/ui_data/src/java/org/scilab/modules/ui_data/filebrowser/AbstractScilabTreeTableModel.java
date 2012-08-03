/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser;

import javax.swing.tree.TreePath;
import javax.swing.event.EventListenerList;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;

/**
 * The tree table model abstract implementation
 * @author Calixte DENIZET
 */
public abstract class AbstractScilabTreeTableModel implements ScilabTreeTableModel {

    protected Object root;
    protected Object parent;
    protected EventListenerList listenerList = new EventListenerList();

    /**
     * Default constructor
     * @param root the root element
     */
    public AbstractScilabTreeTableModel(Object root) {
        this.root = root;
    }

    /**
     * Default constructor
     */
    public AbstractScilabTreeTableModel() { }

    /**
     * Set the root element
     * @param root the root
     */
    public void setRoot(Object root) {
        this.root = root;
    }

    /**
     * {@inheritdoc}
     */
    public Object getRoot() {
        return root;
    }

    /**
     * {@inheritdoc}
     */
    public boolean isLeaf(Object node) {
        return ((FileNode) node).isLeaf();
    }

    /**
     * {@inheritdoc}
     */
    public void valueForPathChanged(TreePath path, Object newValue) { }

    /**
     * {@inheritdoc}
     */
    public int getIndexOfChild(Object parent, Object child) {
        if (parent == root && this.parent != null) {
            return ((FileNode) child).getPosition() + 1;
        } else {
            return ((FileNode) child).getPosition();
        }
    }

    /**
     * {@inheritdoc}
     */
    public void addTreeModelListener(TreeModelListener l) {
        listenerList.add(TreeModelListener.class, l);
    }

    /**
     * {@inheritdoc}
     */
    public void removeTreeModelListener(TreeModelListener l) {
        listenerList.remove(TreeModelListener.class, l);
    }

    /**
     * {@inheritdoc}
     */
    protected void fireTreeNodesChanged(Object source, Object[] path, int[] childIndices, Object[] children) {
        Object[] listeners = listenerList.getListenerList();
        TreeModelEvent e = null;
        for (int i = listeners.length - 2; i >= 0; i -= 2) {
            if (listeners[i] == TreeModelListener.class) {
                if (e == null) {
                    e = new TreeModelEvent(source, path, childIndices, children);
                }
                ((TreeModelListener) listeners[i + 1]).treeNodesChanged(e);
            }
        }
    }

    /**
     * {@inheritdoc}
     */
    protected void fireTreeNodesInserted(Object source, Object[] path, int[] childIndices, Object[] children) {
        Object[] listeners = listenerList.getListenerList();
        TreeModelEvent e = null;
        for (int i = listeners.length - 2; i >= 0; i -= 2) {
            if (listeners[i] == TreeModelListener.class) {
                if (e == null) {
                    e = new TreeModelEvent(source, path, childIndices, children);
                }
                ((TreeModelListener) listeners[i + 1]).treeNodesInserted(e);
            }
        }
    }

    /**
     * {@inheritdoc}
     */
    protected void fireTreeNodesRemoved(Object source, Object[] path, int[] childIndices, Object[] children) {
        Object[] listeners = listenerList.getListenerList();
        TreeModelEvent e = null;
        for (int i = listeners.length - 2; i >= 0; i -= 2) {
            if (listeners[i] == TreeModelListener.class) {
                if (e == null) {
                    e = new TreeModelEvent(source, path, childIndices, children);
                }
                ((TreeModelListener) listeners[i + 1]).treeNodesRemoved(e);
            }
        }
    }

    /**
     * {@inheritdoc}
     */
    protected void fireTreeStructureChanged(Object source, Object[] path, int[] childIndices, Object[] children) {
        Object[] listeners = listenerList.getListenerList();
        TreeModelEvent e = null;
        for (int i = listeners.length - 2; i >= 0; i -= 2) {
            if (listeners[i] == TreeModelListener.class) {
                if (e == null) {
                    e = new TreeModelEvent(source, path, childIndices, children);
                }
                ((TreeModelListener) listeners[i + 1]).treeStructureChanged(e);
            }
        }
    }

    /**
     * {@inheritdoc}
     */
    public Class getColumnClass(int column) {
        return Object.class;
    }

    /**
     * {@inheritdoc}
     */
    public boolean isCellEditable(Object node, int column) {
        return false;
    }

    /**
     * {@inheritdoc}
     */
    public void setValueAt(Object aValue, Object node, int column) { }
}
