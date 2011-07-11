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

package org.scilab.modules.gui.utils;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.Hashtable;

import javax.swing.AbstractButton;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

import org.scilab.modules.gui.utils.Component.Scroll;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/** Updates swing component to reach correspondence with dom nodes.
 */
public class XUpdateVisitor {
    /**
     * stores preceding correspondence to compute diff.
     */
    private Hashtable<Component, XSentinel> correspondance;

    /** Construction of  visitor.
     *
     * @param table : previous correspondence.
     */
    public XUpdateVisitor(final Hashtable<Component, XSentinel> table) {
        correspondance = table;
    }

    /** build a component from scratch with a given node.
     *
     * @param view : container of the built component.
     * @param peer : peer of the container.
     * @param item : peer of the built component.
     * @param index : component index in container layout.
     * @return a built component inserted in its container.
     */
    public final Component build(
            final Container view,
            final Node peer,
            final Node item,
            final int index) {
        Component component  = buildPeerFor(item);
        Object constraints   = getLayoutConstraints(peer, item);
        if (index > -1) {
            view.add(component, constraints, index);
        } else {
            view.add(component, constraints);
        }
        return component;
    }

    /** Suppress a component.
     *
     * @param view : container of the suppressed component.
     * @param component : suppressed component.
     */
    public final void forget(final Container view, final Component component) {
       view.remove(component);
        correspondance.remove(component);
    }

    /** Computes a recursive diff on both tree structure
     *  to achieve correspondence.
     *
     * @param view : the visited container.
     * @param peer : the visited node.
     */
    public final void visit(final Container view, final Node peer) {
        int allIndex     = 0;
        int visibleIndex = 0;
        NodeList nodes = peer.getChildNodes();
        Component component;
        XSentinel sentinel;
        while (allIndex < nodes.getLength()) {
            Node item = nodes.item(allIndex);
            if (isVisible(item)) {
                if (visibleIndex < view.getComponentCount()) {
                    component = view.getComponent(visibleIndex);
                    sentinel  = (XSentinel) correspondance.get(component);
                    if (sentinel == null || !sentinel.checks(item)) {
                        forget(view, component);
                        component = build(view, peer, item, visibleIndex);
                    }
                } else {
                    component = build(view, peer, item, -1);
                }
                if (component instanceof Container) {
                    // Rebuild container children.
                    Container container = (Container) component;
                    visit(container, item);
                }
                visibleIndex += 1;
            }
            allIndex += 1;
        }
        while (visibleIndex < view.getComponentCount()) {
            component = view.getComponent(visibleIndex);
            forget(view, component);
        }
        if (view instanceof XComponent) {
            // Attribute correspondence once children rebuilt.
            XComponent xView = (XComponent) view;
            xView.refresh(peer);
            // Sentinel sets watch.
            sentinel   = new XSentinel(view, peer);
            correspondance.put(view, sentinel);
            addListeners(view, peer, sentinel);
        }
    }

    /** Builds the layout constraint object.
     *
     * @param parent : parent node
     * @param current : current node
     * @return layout constraint (e.g. border side for border layout)
     */
    final Object getLayoutConstraints(final Node parent, final Node current) {
        if (XConfigManager.getAttribute(parent, "layout").equals("border")) {
            return XConfigManager.getAttribute(current, "border-side");
        }
        return null;
        }

    /** Checks whether a node is visible or not.
     *
     * @param node : the checked node
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
        return true;
    }

    /** Link XSentinal as listener for the given component.
     *
     * @param component : listened component
     * @param node : peer node of the component
     * @param sentinel : listener of component, node interpreter
     */
    public final void addListeners(
            final Component component,
            final Node node,
            final XSentinel sentinel
            ) {
        String listener = XConfigManager.getAttribute(node, "listener");
        if (listener.equals("MouseListener")) {
             component.addMouseListener(sentinel);
        }
        if (listener.equals("ActionListener")) {
            if (component instanceof AbstractButton) {
                AbstractButton button = (AbstractButton) component;
                button.addActionListener(sentinel);
            }
            if (component instanceof XChooser) {
                XChooser chooser = (XChooser) component;
                chooser.addActionListener(sentinel);
            }

        }
    }

    /** Address for dynamic class loading.
     *
     */
    private static final String X_PACKAGE
        = "org.scilab.modules.gui.utils.Component.";

    /** Default size for spaces.
    *
    */
    private static final int S_DIM = 5;

    /** Build component from scratch with its node description.
     *
     * @param node : description of component
     * @return the built component
     */
    public final Component buildPeerFor(final Node node) {

        String tag = node.getNodeName();
        if (tag.equals("Scroll")) {
            // Specific treatment for scrolls...
            JPanel container = new JPanel();
            return new Scroll(node, container);
        }

        //1. Find the class with the same name.
        Class<Component> componentClass;
        try {
            componentClass = (Class<Component>) Class.forName(X_PACKAGE + tag);
        } catch (ClassNotFoundException e) {
            // Some classes are made directly
            //  - here labels for text node.
            if (tag.equals("#text")) {
                String value = node.getNodeValue();
                return new JLabel(value);
            }
            // - here boxes.
            if (tag.equals("HBox")) {
                Box hbox = Box.createHorizontalBox();
                XConfigManager.drawConstructionBorders(hbox);
                XConfigManager.setDimension(hbox, node);
                return hbox;
            }
            if (tag.equals("VBox")) {
                Box vbox          = Box.createVerticalBox();
                String background = XConfigManager.getAttribute(
                   node,
                   "background"
                   );
                if (!(background.equals(XConfigManager.NAV))) {
                    Color color = XConfigManager.getColor(background);
                    vbox.setOpaque(true);
                    vbox.setBackground(color);
                }
                XConfigManager.drawConstructionBorders(vbox);
                XConfigManager.setDimension(vbox, node);
                return vbox;
            }
            if (tag.equals("VSpace")) {
                int height = XConfigManager.getInt(node, "height", S_DIM);
                return Box.createVerticalStrut(height);
            }
            if (tag.equals("HSpace")) {
                int width = XConfigManager.getInt(node, "width", S_DIM);
                return Box.createHorizontalStrut(width);
            }
            if (tag.equals("Glue")) {
                return Box.createGlue();
            }
            if (tag.equals("VGLUE")) {
                return Box.createVerticalGlue();
            }

            // Declare failure due to class absence
            return new XStub(tag, "ClassNotFoundException");
        }

        //2. Find the constructor.
        Constructor<Component> constructor;
        try {
                // First with a Node,
                Class [] parameter = new Class[]{Node.class};
                constructor = componentClass.getConstructor(parameter);
        } catch (NoSuchMethodException e) {
            try {
                // then without anything.
                constructor = componentClass.getConstructor(new Class[]{});
            } catch (NoSuchMethodException f) {
                // Declare failure due to constructor absence
                System.err.println("NoSuchMethodException:" + f);
                return new XStub(tag, "NoSuchMethodException");
            }
        } catch (SecurityException e) {
            // Declare failure due to constructor rights (it must be public)
            System.err.println("SecurityException:" + e);
            return new XStub(tag, "SecurityException");
        }

        //3. Invoke the constructor.
        Component component;
        try {
            component = (Component) constructor.newInstance(new Object[]{node});
        } catch (InstantiationException e) {
            System.err.println("InstantiationException:" + e);
            return new XStub(tag, "InstantiationException");
        } catch (IllegalAccessException e) {
            System.err.println("IllegalAccessException:" + e);
            return new XStub(tag, "IllegalAccessException");
        } catch (IllegalArgumentException e) {
            System.err.println("IllegalArgumentException:" + e);
            return new XStub(tag, "IllegalArgumentException");
        } catch (InvocationTargetException e) {
            System.err.println("InvocationTargetException:" + e);
            return new XStub(tag, "InvocationTargetException");
        }
        return component;
    }
}

/** Graphical indication of what goes wrong in buildPeerFor(node).
 *
 */
class XStub extends JPanel {
    /** Serialization id.
     *
     */
    private static final long serialVersionUID = -6540983459186007758L;

    /** Default height.
     *
     */
    private static final int D_HEIGHT = 50;

    /** Default width.
     *
     */
    private static final int D_WIDTH = 100;

    /** Constructor.
     * @param tag : class name source of the error
     * @param cause : description of the error
     *
     */
    public XStub(final String tag, final String cause) {
        super();
        Border       black  = BorderFactory.createLineBorder(Color.blue);
        TitledBorder title  = BorderFactory.createTitledBorder(black, tag);
        Dimension dimension = new Dimension(D_WIDTH, D_HEIGHT);
        setPreferredSize(dimension);
        setOpaque(false);

        title.setTitleColor(Color.blue);
        setBorder(title);
        setLayout(new FlowLayout());
        add(new JLabel(cause));
    }

    /** Output method.
     * @return the string representation
     */
    public String toString() {
        return "STUB";
    }
}

