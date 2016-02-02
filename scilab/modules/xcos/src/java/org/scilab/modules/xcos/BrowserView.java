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

package org.scilab.modules.xcos;

import java.util.Enumeration;
import java.util.LinkedList;

import javax.swing.SwingUtilities;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;

import org.scilab.modules.xcos.explorer.BrowserTreeNodeData;


/**
 * Implement a basic model explorer using a tree view
 */
public class BrowserView extends View {
    private final DefaultTreeModel model;

    /**
     * Default constructor
     */
    public BrowserView() {
        DefaultMutableTreeNode root = new DefaultMutableTreeNode(new BrowserTreeNodeData(), true);

        model = new DefaultTreeModel(root);

        /*
         * retrieve all the existing objects and append them to the model.
         */
        fillModel();
        model.reload();
    }

    private void fillModel() {
        Controller controller = new Controller();
        DefaultMutableTreeNode root = (DefaultMutableTreeNode) model.getRoot();

        // first create all the diagrams
        VectorOfScicosID o = controller.getAll(Kind.DIAGRAM);
        int length = o.size();
        for (int i = 0; i < length; i++) {
            long uid = o.get(i);

            controller.referenceObject(uid);
            DefaultMutableTreeNode current = new DefaultMutableTreeNode(new BrowserTreeNodeData(uid, Kind.DIAGRAM));
            root.add(current);
        }

        // fill each diagram node with its block hierarchy
        LinkedList<DefaultMutableTreeNode> stash = new LinkedList<DefaultMutableTreeNode>();
        for (int i = 0; i < root.getChildCount(); i++) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) root.getChildAt(i);
            stash.add(node);
        }

        VectorOfScicosID children = new VectorOfScicosID(256);
        while (!stash.isEmpty()) {
            DefaultMutableTreeNode node = stash.pop();
            BrowserTreeNodeData data = (BrowserTreeNodeData) node.getUserObject();

            controller.getObjectProperty(data.getId(), data.getKind(), ObjectProperties.CHILDREN, children);
            int childrenLen = children.size();
            for (int j = 0 ; j < childrenLen; j++) {
                long childUID = children.get(j);
                Kind childKind = controller.getKind(childUID);

                controller.referenceObject(childUID);
                DefaultMutableTreeNode child = new DefaultMutableTreeNode(new BrowserTreeNodeData(childUID, childKind));

                node.add(child);
                // manage the children only if this is a block
                if (childKind == Kind.BLOCK) {
                    stash.add(child);
                }
            }
        }
    }

    public DefaultTreeModel getModel() {
        return model;
    }

    /*
     * Implement some View methods on the Event Dispatch Thread
     */

    @Override
    public void objectCreated(final long uid, final Kind kind) {
        if (model == null) {
            return;
        }
        if (kind == Kind.PORT) {
            return;
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                objectCreatedOnEDT(uid, kind);
            }
        });

        // reference on the caller Thread to avoid synchronization later
        new Controller().referenceObject(uid);
    }

    @Override
    public void objectReferenced(final long uid, final Kind kind, long refCount) {
        if (model == null) {
            return;
        }
        if (kind == Kind.PORT) {
            return;
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                objectReferencedOnEDT(uid, kind, refCount);
            }
        });
    }

    @Override
    public void objectUnreferenced(final long uid, final Kind kind, long refCount) {
        if (model == null) {
            return;
        }
        if (kind == Kind.PORT) {
            return;
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                objectUnreferencedOnEDT(uid, kind, refCount);
            }
        });
    };

    @Override
    public void objectDeleted(final long uid, final Kind kind) {
    }

    @Override
    public void propertyUpdated(final long uid, final Kind kind, final ObjectProperties property, final UpdateStatus status) {
        if (model == null) {
            return;
        }
        if (status != UpdateStatus.SUCCESS) {
            return;
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                propertyUpdatedOnEDT(uid, kind, property, status);
            }
        });
    }

    public void objectCreatedOnEDT(final long uid, final Kind kind) {
        DefaultMutableTreeNode root = (DefaultMutableTreeNode) model.getRoot();
        int position = model.getChildCount(root);

        final DefaultMutableTreeNode node = new DefaultMutableTreeNode(new BrowserTreeNodeData(uid, kind), true);
        model.insertNodeInto(node, root, position);
    }

    public void objectReferencedOnEDT(final long uid, final Kind kind, long refCount) {
        DefaultMutableTreeNode node = lookupForUID(uid);
        BrowserTreeNodeData data = (BrowserTreeNodeData) node.getUserObject();

        data.incRefCount();
    }

    public void objectUnreferencedOnEDT(final long uid, final Kind kind, long refCount) {
        DefaultMutableTreeNode node = lookupForUID(uid);
        if (node == null) {
            return;
        }
        BrowserTreeNodeData data = (BrowserTreeNodeData) node.getUserObject();

        if (data.decRefCount() < 1) {
            model.removeNodeFromParent(node);

            // This is a deletion request, other views synchronization will be done on another event
            new Controller().deleteObject(uid);
        }
    }

    public void propertyUpdatedOnEDT(final long uid, final Kind kind, final ObjectProperties property, final UpdateStatus status) {

        /* Manage the children case to append them to associated diagram */
        if (property == ObjectProperties.PARENT_DIAGRAM || property == ObjectProperties.PARENT_BLOCK) {
            switch (kind) {
                case ANNOTATION:
                case BLOCK:
                case LINK: {
                    final Controller controller = new Controller();
                    long[] v = { 0 };
                    if (!controller.getObjectProperty(uid, kind, property, v)) {
                        return;
                    }

                    final DefaultMutableTreeNode parent = lookupForUID(v[0]);
                    final DefaultMutableTreeNode current = lookupForUID(uid);
                    if (current == null || current.getParent() == parent) {
                        return;
                    }

                    model.removeNodeFromParent(current);

                    // current parent is re-set'ed and its old parent is also
                    // updated
                    int position = parent.getChildCount();
                    model.insertNodeInto(current, parent, position);
                }
                break;

                default:
                    break;
            }
        }

    }

    /*
     * Utilities
     */

    /**
     * Lookup for the UID in the TreeModel and return the result or null if not found
     * @param uid the uid to look for
     * @return the found {@link DefaultMutableTreeNode} or <code>null</code> if not found
     */
    private DefaultMutableTreeNode lookupForUID(final long uid) {
        final DefaultMutableTreeNode root = (DefaultMutableTreeNode) model.getRoot();

        DefaultMutableTreeNode node;
        for (Enumeration<DefaultMutableTreeNode> e = root.depthFirstEnumeration(); e.hasMoreElements();) {
            node = e.nextElement();
            BrowserTreeNodeData data = (BrowserTreeNodeData) node.getUserObject();
            if (data.getId() == uid) {
                return node;
            }
        }

        return null;
    }

}
