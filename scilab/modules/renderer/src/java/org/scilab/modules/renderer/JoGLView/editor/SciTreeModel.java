/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.modules.renderer.JoGLView.editor;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import javax.swing.event.EventListenerList;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;
import java.util.Vector;

/**
 * @author Pierre Lando
 */
public class SciTreeModel implements TreeModel, GraphicView {
    private final Integer rootId;
    private final EventListenerList listeners = new EventListenerList();

    public SciTreeModel(Integer id) {
        this.rootId = id;
        GraphicController.getController().register(this);
    }

    public Object getRoot() {
        return rootId;
    }

    public Object getChild(Object parent, int index) {
        if (parent instanceof Integer) {
            Integer id = (Integer) parent;
            Object children =  GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CHILDREN__);
            if (children instanceof Integer[]) {
                if (index >= 0 && index < ((Integer[]) children).length) {
                    return ((Integer[]) children)[index];
                }
            }
        }
        return null;
    }

    public int getChildCount(Object parent) {
        if (parent instanceof Integer) {
            Integer id = (Integer) parent;
            Object children =  GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CHILDREN__);
            if (children instanceof Integer[]) {
                return ((Integer[]) children).length;
            }
        }
        return 0;
    }

    public boolean isLeaf(Object node) {
        return getChildCount(node) == 0;
    }

    public void valueForPathChanged(TreePath path, Object newValue) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    public int getIndexOfChild(Object parent, Object child) {
        Integer childId;
        if (child instanceof Integer) {
            childId = (Integer) child;
            if (parent instanceof Integer) {
                Integer id = (Integer) parent;
                Object children =  GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CHILDREN__);
                if (children instanceof Integer[]) {
                    for (int i = 0 ; i < ((Integer[]) children).length ; i++) {
                        if (((Integer[]) children)[i] == childId) {
                            return i;
                        }
                    }
                }
            }
        }
        return -1;
    }

    public void addTreeModelListener(TreeModelListener l) {
        listeners.add(TreeModelListener.class, l);
    }

    public void removeTreeModelListener(TreeModelListener l) {
        listeners.remove(TreeModelListener.class, l);
    }

    public void updateObject(Integer id, int property) {
        System.out.println("SciTreeModel");
        Vector<Integer> path = getPath(id);
        if (path != null) {
            for (TreeModelListener listener : listeners.getListeners(TreeModelListener.class)) {
                TreeModelEvent e = new TreeModelEvent(id, path.toArray(), new int[] {0}, new Object[] {id});
                listener.treeStructureChanged(e);
            }
        }
    }

    public void createObject(Integer id) {
        Vector<Integer> path = getPath(id);
        if (path != null) {
            for (TreeModelListener listener : listeners.getListeners(TreeModelListener.class)) {
                TreeModelEvent e = new TreeModelEvent(id, path.toArray(), new int[] {0}, new Object[] {id});
                listener.treeNodesInserted(e);
            }
        }
    }

    public void deleteObject(Integer id) {
        Vector<Integer> path = getPath(id);
        if (path != null) {
            for (TreeModelListener listener : listeners.getListeners(TreeModelListener.class)) {
                TreeModelEvent e = new TreeModelEvent(id, path.toArray(), new int[] {0}, new Object[] {id});
                listener.treeNodesRemoved(e);
            }
        }
    }

    private Vector<Integer> getPath(Integer id) {
        Integer parentId = getParent(id);
        // TODO : remove parentId.equals("")
        if (parentId == null || parentId.equals("")) {
            return null;
        } else if (parentId.equals(rootId)) {
            Vector<Integer> v = new Vector<Integer>();
            v.add(rootId);
            return v;
        } else {
            Vector<Integer> v = getPath(parentId);
            if (v != null) {
                v.add(parentId);
            }
            return v;
        }
    }

    private Integer getParent(Integer id) {
        Object parent = GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_PARENT__);
        if (parent instanceof Integer) {
            return (Integer) parent;
        }
        return null;
    }
}
