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
import java.awt.Container;
import java.util.Map;

import javax.swing.AbstractButton;

import org.scilab.modules.preferences.Component.Entry;
import org.scilab.modules.preferences.Component.FileSelector;
import org.scilab.modules.preferences.Component.Scroll;
import org.scilab.modules.preferences.Component.Table;
import org.scilab.modules.preferences.Component.TextArea;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 * Updates swing component to reach correspondence with dom nodes.
 */
public class XUpdateVisitor {

    /**
     * stores preceding correspondence to compute diff.
     */
    private final Map<Component, XSentinel> matching;

    /**
     * Construction of visitor.
     *
     * @param table
     *            : previous correspondence.
     */
    public XUpdateVisitor(final Map<Component, XSentinel> table) {
        matching = table;
    }

    /**
     * build a component from scratch with a given node.
     *
     * @param view
     *            : container of the built component.
     * @param peer
     *            : peer of the container.
     * @param item
     *            : peer of the built component.
     * @param index
     *            : component index in container layout.
     * @return a built component inserted in its container.
     */
    public final Component build(final Container view, final Node peer, final Node item, final int index) {
        Component component = buildPeerFor(item);
        Object constraints = getLayoutConstraints(peer, item);
        if (index >= 0) {
            view.add(component, constraints, index);
        } else {
            view.add(component, constraints);
        }
        return component;
    }

    /**
     * Suppress a component.
     *
     * @param view
     *            : container of the suppressed component.
     * @param component
     *            : suppressed component.
     */
    public final void forget(final Container view, final Component component) {
        view.remove(component);
        matching.remove(component);
    }

    /**
     * Computes a recursive diff on both tree structure to achieve
     * correspondence.
     *
     * @param view
     *            : the visited container.
     * @param peer
     *            : the visited node.
     */
    public final void visit(final Container view, final Node peer) {
        int visibleIndex = 0;
        int count;
        NodeList nodes = peer.getChildNodes();
        int size = nodes.getLength();
        Component component;
        XSentinel sentinel;
        String indent;

        if (view instanceof Scroll) {
            count = ((Scroll) view).getXComponentCount();
        } else {
            count = view.getComponentCount();
        }

        for (int allIndex = 0; allIndex < size; allIndex++) {
            Node item = nodes.item(allIndex);
            if (isVisible(item)) {
                if (visibleIndex < count) {
                    if (view instanceof Scroll) {
                        component = ((Scroll) view).getXComponent(visibleIndex);
                    } else {
                        component = view.getComponent(visibleIndex);
                    }
                    sentinel = matching.get(component);
                    if (sentinel == null || !sentinel.checks(item)) {
                        forget(view, component);
                        component = build(view, peer, item, visibleIndex);
                    }
                } else {
                    component = build(view, peer, item, -1);
                }
                if (component instanceof XComponent) {
                    // Rebuild container children.
                    Container container = (Container) component;
                    visit(container, item);
                }
                visibleIndex++;
            }
        }

        // Clean children tail.
        while (visibleIndex < view.getComponentCount()) {
            component = view.getComponent(visibleIndex);
            if (component instanceof XComponent) {
                forget(view, component);
                continue;
            }
            visibleIndex++;
        }

        // Sentinel sets watch.
        sentinel = matching.get(view);
        if (sentinel == null) {
            sentinel = new XSentinel(view, peer);
            matching.put(view, sentinel);
            if (view instanceof XComponent) {
                addListeners(view, peer, sentinel);
            }
        } else {
            sentinel.setPeer(peer);
        }
        // Attribute correspondence once children rebuilt.
        if (view instanceof XComponent) {
            XComponent xView = (XComponent) view;
            xView.refresh(peer);
        }
    }

    /**
     * Builds the layout constraint object.
     *
     * @param parent
     *            : parent node
     * @param current
     *            : current node
     * @return layout constraint (e.g. border side for border layout)
     */
    final Object getLayoutConstraints(final Node parent, final Node current) {
        if (XConfigManager.getAttribute(parent, "layout").equals("border")) {
            return XConfigManager.getAttribute(current, "border-side");
        }

        if (parent.getNodeName().equals("Grid")) {
            return current;
        }
        return null;
    }

    /**
     * Checks whether a node is visible or not.
     *
     * @param node
     *            : the checked node
     * @return its visibility
     */
    public final boolean isVisible(final Node node) {
        // a. Event nodes are invisibles.
        if (node.getNodeName().equals("mouseClicked")) {
            return false;
        }
        if (node.getNodeName().equals("actionPerformed")) {
            return false;
        }
        if (node.getNodeName().equals("entryChanged")) {
            return false;
        }
        if (node.getNodeName().startsWith("propertyChange")) {
            return false;
        }
        // b. Text nodes with only invisible characters are invisible.
        if (node.getNodeName().equals("#text")) {
            if (node.getNodeValue().replaceAll("^[ \t\n]+$", "").equals("")) {
                return false;
            }
        }
        // c. Chooser options are invisible.
        if (node.getNodeName().equals("option")) {
            return false;
        }
        // d. Table descriptors are invisible.
        if (node.getNodeName().startsWith("table")) {
            return false;
        }

        // d. List element are invisible.
        if (node.getNodeName().equals("listElement")) {
            return false;
        }

        // d. List element are invisible.
        if (node.getNodeName().equals("html")) {
            return false;
        }
        return true;
    }

    /**
     * Link XSentinal as listener for the given component.
     *
     * @param component
     *            : listened component
     * @param node
     *            : peer node of the component
     * @param sentinel
     *            : listener of component, node interpreter
     */
    public final void addListeners(final Component component, final Node node, final XSentinel sentinel) {
        String listener = XCommonManager.getAttribute(node, "listener");
        if (listener.equals("ActionListener")) {
            if (component instanceof AbstractButton) {
                AbstractButton button = (AbstractButton) component;
                button.addActionListener(sentinel);
                return;
            }
            if (component instanceof XChooser) {
                XChooser chooser = (XChooser) component;
                chooser.addActionListener(sentinel);
                return;
            }
        }

        if (listener.equals("MouseListener")) {
            // component.addKeyListener(sentinel); Provide focus with proper
            // focus policy.
            component.addMouseListener(sentinel);
            return;
        }

        if (listener.equals("KeyListener")) {
            component.addKeyListener(sentinel);
            return;
        }

        if (listener.equals("EntryListener")) {
            if (component instanceof Entry) {
                ((Entry) component).getDocument().addDocumentListener(sentinel);
                return;
            }

            if (component instanceof FileSelector) {
                ((FileSelector) component).addDocumentListener(sentinel);
                return;
            }

            if (component instanceof TextArea) {
                ((TextArea) component).addDocumentListener(sentinel);
                return;
            }
        }

        if (listener.equals("TableListener")) {
            if (component instanceof Table) {
                Table table = (Table) component;
                table.addTableModelListener(sentinel);
                return;
            }
        }

        if (listener.startsWith("PropertyChangeListener")) {
            final int nameIndex = listener.indexOf('#');
            final boolean hasPropertyName = nameIndex > 0;

            final String propertyName;
            if (hasPropertyName) {
                propertyName = listener.substring(nameIndex + 1);
            } else {
                propertyName = null;
            }

            if (component instanceof java.awt.Component) {
                if (hasPropertyName) {
                    component.addPropertyChangeListener(propertyName, sentinel);
                } else {
                    component.addPropertyChangeListener(sentinel);
                }

                return;
            }
        }
    }

    /**
     * Build component from scratch with its node description.
     *
     * @param node
     *            : description of component
     * @return the built component
     */
    @SuppressWarnings("unchecked")
    public final Component buildPeerFor(final Node node) {
        return ComponentFactory.getComponent(node);
    }
}
