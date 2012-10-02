/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    private final String rootId;
    private final EventListenerList listeners = new EventListenerList();

    public SciTreeModel(String id) {
        this.rootId = id;
        GraphicController.getController().register(this);
    }

    public Object getRoot() {
        return rootId;
    }

    public Object getChild(Object parent, int index) {
        if (parent instanceof String) {
            String id = (String) parent;
            Object children =  GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CHILDREN__);
            if (children instanceof String[]) {
                if (index >= 0 && index < ((String[]) children).length) {
                    return ((String[]) children)[index];
                }
            }
        }
        return null;
    }

    public int getChildCount(Object parent) {
        if (parent instanceof String) {
            String id = (String) parent;
            Object children =  GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CHILDREN__);
            if (children instanceof String[]) {
                return ((String[]) children).length;
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
        String childId;
        if (child instanceof String) {
            childId = (String) child;
            if (parent instanceof String) {
                String id = (String) parent;
                Object children =  GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CHILDREN__);
                if (children instanceof String[]) {
                    for (int i = 0 ; i < ((String[]) children).length ; i++) {
                        if (((String[]) children)[i].equals(childId)) {
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

    public void updateObject(String id, int property) {
        Vector<String> path = getPath(id);
        if (path != null) {
            for (TreeModelListener listener : listeners.getListeners(TreeModelListener.class)) {
                TreeModelEvent e = new TreeModelEvent(id, path.toArray(), new int[]{0}, new Object[]{id});
                listener.treeStructureChanged(e);
            }
        }
    }

    public void createObject(String id) {
        Vector<String> path = getPath(id);
        if (path != null) {
            for (TreeModelListener listener : listeners.getListeners(TreeModelListener.class)) {
                TreeModelEvent e = new TreeModelEvent(id, path.toArray(), new int[]{0}, new Object[]{id});
                listener.treeNodesInserted(e);
            }
        }
    }

    public void deleteObject(String id) {
        Vector<String> path = getPath(id);
        if (path != null) {
            for (TreeModelListener listener : listeners.getListeners(TreeModelListener.class)) {
                TreeModelEvent e = new TreeModelEvent(id, path.toArray(), new int[]{0}, new Object[]{id});
                listener.treeNodesRemoved(e);
            }
        }
    }

    private Vector<String> getPath(String id) {
        String parentId = getParent(id);
        // TODO : remove parentId.equals("")
        if (parentId == null || parentId.equals("")) {
            return null;
        } else if (parentId.equals(rootId)) {
            Vector<String> v = new Vector<String>();
            v.add(rootId);
            return v;
        } else {
            Vector<String> v = getPath(parentId);
            if (v != null) {
                v.add(parentId);
            }
            return v;
        }
    }

    private String getParent(String id) {
        Object parent = GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_PARENT__);
        if (parent instanceof String) {
            return (String) parent;
        }
        return null; 
    }
}
