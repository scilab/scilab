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

package org.scilab.modules.preferences;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Set;
import java.util.TreeSet;

import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;

import org.scilab.modules.preferences.Component.Table;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 * XSentinel embeds correspondence between swing components and visible DOM
 * nodes. Each instance has two functions : 1. Checks if a given node can be
 * updated through actuators from the former one 2. Interprets incoming events
 * and transmit corresponding action to XConfigManager.
 *
 * @author Pierre GRADIT
 *
 **/
public class XSentinel implements MouseListener, ActionListener, TableModelListener, KeyListener, DocumentListener, PropertyChangeListener {

    private static final Set<String> LAYOUT = new TreeSet<String>(Arrays.asList(new String[] { "listener", "gridx", "gridy", "gridwidth", "gridheight",
            "weightx", "weighty", "anchor", "ipadx", "ipday", "insets", "fill", "border-side", "fixed-height", "fixed-width"
                                                                                             }));

    /**
     * Associated DOM Node. TODO Add accessors for this attribute.
     */
    protected Node peer;

    /**
     * Attributes which can be modified. TODO Add accessors for this attribute.
     */
    protected String[] actuators;

    /**
     * Reduced string for actuation test. TODO Add accessors for this attribute.
     */
    public String reduced;

    /**
     * Last time-stamp to avoid event duplication.
     */
    private long timestamp = 0;

    private XComponent xComponent;

    /**
     * Construction of a correspondence.
     *
     * @param component
     *            : Swing component
     * @param node
     *            : DOM node
     */
    public XSentinel(final Component component, final Node node) {
        peer = node;
        if (component instanceof XComponent) {
            xComponent = (XComponent) component;
            actuators = xComponent.actuators();
        } else {
            actuators = new String[0];
        }
    }

    /**
     * Evaluate reachability through actuators as string equality.
     *
     * @param node
     *            : a given peer node to be reached.
     * @param actuators
     *            : the set of actuators.
     * @return the string verifying that string equality implies reachability
     *         through actuators.
     */
    public static String signature(final Node node, final String[] actuators) {
        StringBuilder signature = new StringBuilder(node.getNodeName());
        if (!node.hasAttributes()) {
            return signature.toString();
        }

        // Filtering attributes against actuators (and grid layout constraint).
        NamedNodeMap attributes = node.getAttributes();
        int size = attributes.getLength();
        Set<String> set = new TreeSet<String>(Arrays.asList(actuators));
        set.addAll(LAYOUT);

        for (int i = 0; i < size; i++) {
            Node item = attributes.item(i);
            String attrName = item.getNodeName();
            if (!set.contains(attrName)) {
                /*
                 * As "actuators" can be performed without deleting node their
                 * value is removed from signature, grid layout manages
                 * "layout"...
                 */
                String value = item.getNodeValue().replaceAll("[ \t\n]+", " ");
                signature.append(" ");
                signature.append(attrName);
                signature.append("='");
                signature.append(value);
                signature.append("'");
            }
        }

        return signature.toString();
    }

    public void setPeer(final Node next) {
        peer = next;
    }

    /**
     * Check whether the next node can be reached through actuators.
     *
     * @param next
     *            : the node to be reached.
     * @return can the node be reached ?
     */
    public boolean checks(final Node next) {
        String checker = signature(next, actuators);
        if (reduced == null) {
            reduced = signature(peer, actuators);
        }
        return reduced.equals(checker);
    }

    /**
     * Process event node through component. (must be updated if a new manager
     * is created)
     *
     * @param component
     *            : the source of the action.
     * @param node
     *            : the node embedding the action.
     */
    boolean triggerEventNodes(final Component component, final Node[] nodes) {
        if (XConfigManager.active) {
            return XConfigManager.xEvent(nodes, component);
        }
        if (XWizardManager.active) {
            return XWizardManager.xEvent(nodes, component);
        }
        return false;
    }

    //
    /**
     * Returns the first event node of the given type.
     *
     * @param node
     *            : the node to browse.
     * @param type
     *            : the seek type in peer children.
     */
    Node[] getEventNodes(final Node node, final String type) {
        ArrayList<Node> list = new ArrayList<Node>();
        NodeList nodelist = node.getChildNodes();
        for (int i = 0; i < nodelist.getLength(); i++) {
            Node child = nodelist.item(i);
            if (child.getNodeName().equals(type)) {
                list.add(child);
            }
        }

        return list.toArray(new Node[list.size()]);
    }

    /** Mouse listener callback. @param e : event */
    @Override
    public void mouseClicked(final MouseEvent e) {
        long when = e.getWhen();
        if (when != timestamp) {
            Node[] action = getEventNodes(peer, "mouseClicked");
            triggerEventNodes((Component) e.getSource(), action);
            timestamp = when;
        }
    }

    /** Mouse listener callback. @param e : event */
    @Override
    public void mouseEntered(final MouseEvent e) {
    }

    /** Mouse listener callback. @param e : event */
    @Override
    public void mouseExited(final MouseEvent e) {
    }

    /** Mouse listener callback. @param e : event */
    @Override
    public void mousePressed(final MouseEvent e) {
    }

    /** Mouse listener callback. @param e : event */
    @Override
    public void mouseReleased(final MouseEvent e) {
    }

    /** Action listener callback. @param e : event */
    @Override
    public void actionPerformed(final ActionEvent e) {
        long when = e.getWhen();
        if (when != timestamp) {
            Node[] actions = getEventNodes(peer, "actionPerformed");
            if (actions.length == 0) {
                actions = getEventNodes(peer, e.getActionCommand());
            }
            if (actions.length != 0) {
                triggerEventNodes((Component) e.getSource(), actions);
                timestamp = when;
            }
        } else {
            if (XConfigManager.differential) {
                System.out.println(" |  dummy actionPerformed discarded!");
            }
        }
    }

    /** Mouse listener callback. @param e : event */
    @Override
    public void keyPressed(final KeyEvent e) {
    }

    /** Mouse listener callback. @param e : event */
    @Override
    public void keyReleased(final KeyEvent e) {
    }

    /** Mouse listener callback. @param e : event */
    @Override
    public void keyTyped(final KeyEvent e) {
        long when = e.getWhen();
        if (when != timestamp) {
            Node[] action = getEventNodes(peer, "keyTyped");
            triggerEventNodes((Component) e.getSource(), action);
            timestamp = when;
        }
    }

    @Override
    public void changedUpdate(DocumentEvent e) {
    }

    @Override
    public void insertUpdate(DocumentEvent e) {
        Node[] action = getEventNodes(peer, "entryChanged");
        triggerEventNodes((Component) xComponent, action);
    }

    @Override
    public void removeUpdate(DocumentEvent e) {
        Node[] action = getEventNodes(peer, "entryChanged");
        triggerEventNodes((Component) xComponent, action);
    }

    /** Table listener callback. @param e : event */
    @Override
    public void tableChanged(final TableModelEvent e) {
        Node[] actions = getEventNodes(peer, "tableChanged");
        Table.processModelEvent(e, actions);
        triggerEventNodes(null, actions);
        // source is used for "choose" behavior not "set" ones.
    }

    /** Global property change listener */
    @Override
    public void propertyChange(final PropertyChangeEvent e) {
        Node[] action = getEventNodes(peer, "propertyChange");
        final ArrayList<Node> called = new ArrayList<Node>(action.length);

        for (Node a : action) {
            final String name = XCommonManager.getAttribute(a, "name", null);

            if (name == null || name.equals(e.getPropertyName())) {
                called.add(a);
            }
        }
        triggerEventNodes((Component) xComponent, called.toArray(new Node[called.size()]));
    }
}
