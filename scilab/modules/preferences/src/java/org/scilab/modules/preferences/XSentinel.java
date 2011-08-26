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

package org.scilab.modules.preferences;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.Arrays;
import java.util.EventObject;

import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.AbstractTableModel;

import org.scilab.modules.preferences.Component.Table;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 *    XSentinel embeds correspondence between swing components
 *    and visible DOM nodes. Each instance has two functions :
 *    1. Checks if a given node can be updated through actuators
 *        from the former one
 *    2. Interprets incoming events and transmit corresponding
 *        action to XConfigManager.
 *
 * @author Pierre GRADIT
 *
 **/
class XSentinel implements MouseListener, ActionListener, TableModelListener {
    /** Associated DOM Node.
     * TODO Add accessors for this attribute. */
    protected Node      peer;
    /** Attributes which can be modified.
     * TODO Add accessors for this attribute. */
    protected String [] actuators;
    /** Reduced string for actuation test.
     * TODO Add accessors for this attribute. */
    public    String    reduced = null;

    /** Last time-stamp to avoid event duplication.
     */
    private   long    timestamp = 0;
    
    /** Construction of a correspondence.
     *
     * @param component : Swing component
     * @param node : DOM node
     */
    public XSentinel(final Component component, final Node node) {
        peer = node;
        if (component instanceof XComponent) {
            XComponent  xComponent = (XComponent) component;
            actuators = xComponent.actuators();
        } else {
            actuators = new String[0];
        }
    }

    /** Evaluate reachability through actuators as string equality.
     *
     * @param node : a given peer node to be reached.
     * @param actuators : the set of actuators.
     * @return the string verifying that string equality implies
     * reachability through actuators.
     */
    public static String signature(final Node node, final String [] actuators) {
        String signature        = "";
        if (!node.hasAttributes()) {
                return "";
        }
        NamedNodeMap attributes = node.getAttributes();
        int size                = attributes.getLength();
        String [] keys          = new String[size];

        for (int i = 0; i < size; i++) {
            Node item = attributes.item(i);
            keys[i]   = item.getNodeName();
        }
        Arrays.sort(keys);
        /* as DOM attributes are not ordered,
         *   sort is needed to achieve proper reduction.
         */
        signature += node.getNodeName();
        for (int i = 0; i < size; i++) {
            String attrName = keys[i];
            if (Arrays.binarySearch(actuators, attrName) == -1 ) {
            	/* As actuators can be performed without deleting node
            	 * their value is removed from signature.
            	 */
                Node item    = attributes.getNamedItem(attrName);
                String value = item.getNodeValue().replaceAll("[ \t\n]+", " ");
                signature   += " " + attrName + "='" + value + "'";
            }
        }
        return signature;
    }

    public void setPeer(final Node next) {
        peer = next;
    }
    
    /** Check whether the next node can be reached through actuators.
     *
     * @param next : the node to be reached.
     * @return can the node be reached ?
     */
    public boolean checks(final Node next) {
        String checker  = signature(next, actuators);
        if (reduced == null) {
            reduced  = signature(peer, actuators);
        }
        return reduced.equals(checker);
    }
    /** Process event node through component.
     *  (must be updated if a new manager is created)
     * @param component : the source of the action.
     * @param node : the node embedding the action.
     */
    boolean triggerEventNodes(
            final Component component,
            final Node [] nodes) {
        if (XConfigManager.active) {
            return XConfigManager.xEvent(nodes, component);
        }
        if (XWizardManager.active) {
            return XWizardManager.xEvent(nodes, component);
        }
        return false;
    }

//
    /** Returns the first event node of the given type.
    *
    * @param node : the node to browse.
    * @param type : the seek type in peer children.
    */
   Node [] getEventNodes(
           final Node node,
           final String type) {
       /**
         *  returns the first event node of given type.
         **/
       NodeList nodelist = node.getChildNodes();
       int size = 0;
       for (int i = 0; i < nodelist.getLength(); i++) {
           Node child = nodelist.item(i);
           if (child.getNodeName().equals(type)) {
               size ++;
           }
       }
       Node [] result = new Node[size];
       int j = 0;
       for (int i = 0; i < nodelist.getLength(); i++) {
           Node child = nodelist.item(i);
           if (child.getNodeName().equals(type)) {
               result[j] = child;
               j ++;
           }
       }
       return result;
   }

    /** Mouse listener callback. @param e : event*/
    public void mouseClicked(final MouseEvent e) {
        long when = e.getWhen();
        if (when != timestamp) {
            Node [] action = getEventNodes(peer, "mouseClicked");
            triggerEventNodes((Component) e.getSource(),action) ;
            timestamp = when;
        } else {
            if (XConfigManager.differential) {
                System.out.println(" |  dummy mouseClicked discarded!");
            }
        }
    }
    /** Mouse listener callback. @param e : event*/
    public void mouseEntered(final MouseEvent e) {
    }
    /** Mouse listener callback. @param e : event*/
    public void mouseExited(final MouseEvent e) {
    }
    /** Mouse listener callback. @param e : event*/
    public void mousePressed(final MouseEvent e) {
    }
    /** Mouse listener callback. @param e : event*/
    public void mouseReleased(final MouseEvent e) {
    }

    /** Action listener callback. @param e : event*/
    public void actionPerformed(final ActionEvent e) {
        long when = e.getWhen();
        if (when != timestamp) {
            Node [] actions = {};
            if (actions.length == 0) {
                actions = getEventNodes(peer, "actionPerformed");
            }
            if (actions.length == 0) {
                actions = getEventNodes(peer, e.getActionCommand());
            }
            if (actions.length > 0) {
                triggerEventNodes((Component) e.getSource(),actions) ;
                timestamp = when;
            } else {
                //TODO : add something here
            }
            return;
        } else {
            if (XConfigManager.differential) {
                System.out.println(" |  dummy actionPerformed discarded!");
            }
        }
    }

    /** Table listener callback. @param e : event*/
    public void tableChanged(final TableModelEvent e) {
        Node [] actions = getEventNodes(peer, "tableChanged");
        Table.processModelEvent(e, actions);
        triggerEventNodes(null, actions);
        // source is used for "choose" behavior not "set" ones.
    }
}


