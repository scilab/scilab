/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.preferences.Component;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.event.TreeModelListener;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeSelectionModel;

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;

/**
 * Implementation of Select compliant with extended management.
 *
 * @author Pierre GRADIT
 */
public class Tree extends Panel implements XComponent, XChooser, TreeSelectionListener {

    /**
     * Universal identifier for serialization.
     */
    private static final long serialVersionUID = -6127289363733321914L;

    private ActionListener actionListener = null;
    private XTreeModel model;
    private JTree tree;
    private boolean dontChange = false;

    /**
     * Constructor.
     * @param peer: associated view DOM node.
     */
    public Tree(final Node peer) {
        super(peer);
        model = new XTreeModel(peer);
        tree = new JTree();
        tree.setModel(model);
        tree.getSelectionModel().addTreeSelectionListener(this);
        tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
        tree.setRootVisible(false);
        tree.setShowsRootHandles(true);
        tree.setExpandsSelectedPaths(true);

        // Under Windows the directory icon is used: bad....
        DefaultTreeCellRenderer renderer = (DefaultTreeCellRenderer) tree.getCellRenderer();
        renderer.setLeafIcon(null);
        renderer.setClosedIcon(null);
        renderer.setOpenIcon(null);

        JScrollPane scrollPane = new JScrollPane(tree);
        this.add(scrollPane);
    }

    /**
     * Define the set of actuators.
     * @return array of actuator names.
     */
    public final String[] actuators() {
        String[] actuators = {"item"};
        return actuators;
    }

    /**
     * Refresh the component by the use of actuators.
     * @param peer the corresponding view DOM node
     */
    public final void refresh(final Node peer) {
        model.setRoot(peer);
        String path = XCommonManager.getAttribute(XCommonManager.getElementByContext("/"), "path", "/1");

        if (!path.equals(path())) {
            path(path);
        }
    }

    /**
     * Actual response read by the listener.
     * @return the chosen value.
     */
    public final Object choose() {
        return path();
    }

    /**
     * Registration of a single listener.
     * @param listener for the unique actionListener.
     */
    public final void addActionListener(final ActionListener listener) {
        actionListener = listener;
    }

    /**
     * {@inheritDoc}
     */
    public void valueChanged(TreeSelectionEvent e) {
        if (actionListener != null && !dontChange) {
            ActionEvent transmit = new ActionEvent(this, 0, "actionPerformed", e.hashCode(), 0);
            actionListener.actionPerformed(transmit);
        }
    }

    /**
     * Sensor for 'path' attribute.
     * @return the attribute value.
     */
    public final String path() {
        TreePath tp = tree.getSelectionPath();
        if (tp != null) {
            StringBuilder path = new StringBuilder();
            Object[] selection = tp.getPath();
            XAdapterNode prev = (XAdapterNode) selection[0];
            for (int i = 1; i < selection.length; i++) {
                XAdapterNode next = (XAdapterNode) selection[i];
                int index = prev.getIndexOfChild(next) + 1;
                if (i == 1) {
                    path.append(Integer.toString(index));
                } else {
                    // case of bodies...
                    path.append(Integer.toString(index + 1));
                }

                path.append("/");
                prev = next;
            }

            return path.toString();
        }

        return "-1";
    }

    /**
     * Actuator for 'item' attribute.
     * @param text : the attribute value.
     */
    public final void path(final String item) {
        String[] ids = item.split("/");
        int index = 0;
        XAdapterNode node = (XAdapterNode) tree.getModel().getRoot();
        for (int i = 0; i < ids.length; i++) {
            try {
                index += Integer.parseInt(ids[i]) - 1;
                node = node.getChild(index);
                if (i < ids.length - 1) {
                    tree.expandRow(index);
                } else {
                    dontChange = true;
                    tree.setSelectionRow(index);
                    dontChange = false;
                }
            } catch (NumberFormatException e) {
                dontChange = true;
                tree.setSelectionRow(0);
                dontChange = false;
                break;
            }
        }
    }

    /**
     * Developer serialization method.
     * @return equivalent signature.
     */
    public final String toString() {
        return "Tree ...";
    }
}

class XAdapterNode {

    Node peer;

    public XAdapterNode(Node node) {
        peer = node;
    }

    public Node getPeer() {
        return peer;
    }

    public String toString() {
        return XCommonManager.getAttribute(peer, "name", "");
    }

    public String content() {
        return peer.getNodeName();
    }

    public XAdapterNode getChild(int index) {
        NodeList nodelist = peer.getChildNodes();
        int count = index;
        final int size = nodelist.getLength();
        for (int i = 0; i < size; i++) {
            Node node = nodelist.item(i);
            if (node.getNodeName().startsWith("#") || node.getNodeName().equals("actionPerformed")) {
                continue;
            }
            if (count == 0) {
                return new XAdapterNode(node);
            }
            count--;
        }
        return null; //unreachable statement
    }

    public int getChildCount() {
        NodeList nodelist = peer.getChildNodes();
        int count = 0;
        final int size = nodelist.getLength();
        for (int i = 0; i < size; i++) {
            Node node = nodelist.item(i);
            if (node.getNodeName().startsWith("#") || node.getNodeName().equals("actionPerformed")) {
                continue;
            }
            count++;
        }
        return count;
    }

    public int getIndexOfChild(XAdapterNode child) {
        NodeList nodelist = peer.getChildNodes();
        int count = 0;
        final int size = nodelist.getLength();
        for (int i = 0; i < size; i++) {
            Node node = nodelist.item(i);
            if (node.getNodeName().startsWith("#") || node.getNodeName().equals("actionPerformed")) {
                continue;
            }
            if (child.peer == node) {
                return count;
            }
            count++;
        }
        return -1; //unreachable statement
    }
}

/**
 * Scans DOM nodes and translate it into tree model.
 * @author Pierre GRADIT
 *
 */
class XTreeModel implements TreeModel {

    /**
     * Serialization ID.
     */
    private static final long serialVersionUID = -4786321481195930071L;

    /**
     * Data source.
     */
    private XAdapterNode root;
    private Node peer;

    public XTreeModel(Node peer) {
        setRoot(peer);
    }

    public void setRoot(final Node input) {
        if (input != peer) {
            peer = input;
            root = new XAdapterNode(input);
        }
    }

    public Object getChild(Object node, int index) {
        return ((XAdapterNode) node).getChild(index);
    }

    public int getChildCount(Object node) {
        return ((XAdapterNode) node).getChildCount();
    }

    public int getIndexOfChild(Object node, Object other) {
        return ((XAdapterNode) node).getIndexOfChild((XAdapterNode) other);
    }

    public Object getRoot() {
        return root;
    }

    public boolean isLeaf(Object node) {
        return ((XAdapterNode) node).getChildCount() == 0;
    }

    public void valueForPathChanged(TreePath arg0, Object arg1) { }

    public void addTreeModelListener(TreeModelListener arg0) { }

    public void removeTreeModelListener(TreeModelListener arg0) { }
}
