/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.preferences.Component;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTree;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.event.TreeModelListener;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeSelectionModel;

import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/** Implementation of Select compliant with extended management.
*
* @author Pierre GRADIT
*
*/
public class Tree extends JPanel implements XComponent, XChooser, TreeSelectionListener {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -6127289363733321914L;

    
    /** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public final String [] actuators() {
        String [] actuators = {"item"};
        return actuators;
    }
    XTreeModel model;
    JTree tree;
    JScrollPane scrollPane;

    /** Constructor.
    *
    * @param peer : associated view DOM node.
    */
    public Tree(final Node peer) {
        super(new BorderLayout());
        model        = new XTreeModel(peer);
        tree         = new JTree ();
        tree.setModel(model);
        tree.getSelectionModel().addTreeSelectionListener(this);
        tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
        tree.setRootVisible(false);
        scrollPane = new JScrollPane(tree);
        this.add(scrollPane);
    }
 
    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public final void refresh(final Node peer) {
        model.setRoot(peer);
        String path = XCommonManager.getAttribute(peer , "path", "-1");

       // if (!path.equals(path())) {
       //     path(path);
       // }
        //repaint();
    }

    /** Row selection management.
    *
    */
    private ActionListener actionListener = null;

    /** Actual response read by the listener.
    *
    * @return the chosen value.
    */
    public final Object choose() {
        return path();
    }

    /** Registration of a single listener.
     * @param listener for the unique actionListener.
     */
    public final void addActionListener(final ActionListener listener) {
        actionListener = listener;
    }

    boolean externalChange;
    
    public void valueChanged(TreeSelectionEvent e) {
        if (actionListener != null) {
            ActionEvent transmit  = new ActionEvent(this, 0,"actionPerformed", e.hashCode(), 0);
            actionListener.actionPerformed(transmit);
        }
    }

    /** Sensor for 'path' attribute.
    *
    * @return the attribute value.
    */
    public final String path() {
        Object [] selection = tree.getSelectionPath().getPath();
        String path = "";
        XAdapterNode prev = (XAdapterNode) selection[0];
        for (int i = 1; i < selection.length ; i++) {
            XAdapterNode next = (XAdapterNode) selection[i];
            int index = prev.getIndexOfChild(next) + 1;
            if (i > 1) {
                // case of bodies...
                index ++;
            }
            path += index; 
            path += "/";
            prev  = next;
        }
        System.out.println("--->" + path);
        return path;
    }

    /** Actuator for 'item' attribute.
    *
    * @param text : the attribute value.
    */
    public final void path(final String item) {
        //not yet...
    }

    /** Developer serialization method.
    *
    * @return equivalent signature.
    */
    public final String toString() {
        String signature = "Tree ...";
        return signature;
    }

}

class XAdapterNode {
    
    Node peer;
    
    public XAdapterNode(Node node) {
        peer = node;
    }
    
    public String toString() {
        // renderer !
        return XCommonManager.getAttribute(peer, "name", "");
    }
    
    public String content() {
        return peer.getNodeName();
    }
    
    public XAdapterNode getChild(int index) {
       NodeList nodelist = peer.getChildNodes();
       int count = index;
       for (int i = 0; i < nodelist.getLength(); i++) {
           if (nodelist.item(i).getNodeName().startsWith("#")) {
               continue;
           }
           if (count == 0) {
               return new XAdapterNode(nodelist.item(i));
           }
           count -= 1;
       }
       return null; //unreachable statement
   }

    public int getChildCount() {
        NodeList nodelist = peer.getChildNodes();
        int count = 0;
        for (int i = 0; i < nodelist.getLength(); i++) {
            if (nodelist.item(i).getNodeName().startsWith("#")) {
                continue;
            }
            count += 1;
        }
        return count;
    }
    
    public int getIndexOfChild(XAdapterNode child) {
        NodeList nodelist = peer.getChildNodes();
        int count = 0;
        for (int i = 0; i < nodelist.getLength(); i++) {
            if (nodelist.item(i).getNodeName().startsWith("#")) {
                continue;
            }
            if (child.peer==nodelist.item(i)) {
                return count;
            }
            count += 1;
        }
        return -1; //unreachable statement
    }
}

/** Scans DOM nodes and translate it into tree model.
 *
 * @author gradit
 *
 */
class XTreeModel implements TreeModel {

    /** Serialization ID.
     *
     */
    private static final long serialVersionUID = -4786321481195930071L;

    /** Data source.
     *
     */
    private XAdapterNode root;

    public XTreeModel(Node peer) {
        setRoot(peer);
    }

    public void setRoot(final Node input) {
        root = new XAdapterNode(input);
    }

    public Object getChild(Object node, int index) {
        return ((XAdapterNode)node).getChild(index);
    }

    public int getChildCount(Object node) {
        return ((XAdapterNode)node).getChildCount();
    }

    public int getIndexOfChild(Object node, Object other) {
        return ((XAdapterNode)node).getIndexOfChild((XAdapterNode) other);
    }

    public Object getRoot() {
        return root;
    }

    public boolean isLeaf(Object node) {
        return ((XAdapterNode)node).getChildCount() == 0;
    }

    public void valueForPathChanged(TreePath arg0, Object arg1) {
        // empty
    }

    public void addTreeModelListener(TreeModelListener arg0) {
        // empty
    }

    public void removeTreeModelListener(TreeModelListener arg0) {
        // empty
    }


}
