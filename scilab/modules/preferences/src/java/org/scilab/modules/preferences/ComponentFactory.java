/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

import org.w3c.dom.Node;

import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.preferences.Component.Scroll;

/**
 * Component factory
 *
 * @author Calixte DENIZET
 *
 */
class ComponentFactory {

    private static final String X_PACKAGE = "org.scilab.modules.preferences.Component.";
    private static final int SPACE = 5;
    private static final Map < String, Constructor <? extends Component >> cache = new HashMap < String, Constructor <? extends Component >> ();
    private static final Map<String, MyAction> actions = new HashMap<String, MyAction>();

    static {
        actions.put("Scroll", new MyAction() {
            public Component getComponent(final Node node) {
                return new Scroll(node, new JPanel());
            }
        });
        actions.put("#text", new MyAction() {
            public Component getComponent(final Node node) {
                return new JLabel(node.getNodeValue());
            }
        });
        actions.put("VSpace", new MyAction() {
            public Component getComponent(final Node node) {
                return Box.createVerticalStrut(XConfigManager.getInt(node, "height", SPACE));
            }
        });
        actions.put("HSpace", new MyAction() {
            public Component getComponent(final Node node) {
                return Box.createHorizontalStrut(XConfigManager.getInt(node, "width", SPACE));
            }
        });
        actions.put("Glue", new MyAction() {
            public Component getComponent(final Node node) {
                return Box.createGlue();
            }
        });
    }

    public static final Component getComponent(final Node node) {
        Component component;
        String tag = node.getNodeName();
        MyAction action = actions.get(tag);
        if (action != null) {
            component = action.getComponent(node);
            return component;
        }

        Constructor <? extends Component > constructor = cache.get(tag);
        if (constructor == null) {
            if ("PreviewCode".equals(tag)) {
                ScilabCommonsUtils.loadOnUse("SciNotes");
            }

            Class <? extends Component > componentClass;
            try {
                componentClass = Class.forName(X_PACKAGE + tag).asSubclass(Component.class);
            } catch (ClassNotFoundException e) {
                System.err.println(e);
                return new XStub(node, "ClassNotFoundException");
            }

            try {
                // First with a Node as argument
                Class[] parameter = new Class[] {Node.class};
                constructor = componentClass.getConstructor(parameter);
            } catch (NoSuchMethodException e) {
                System.err.println("NoSuchMethodException:" + e);
                return new XStub(node, "NoSuchMethodException");
            } catch (SecurityException e) {
                // Declare failure due to constructor rights (it must be public)
                System.err.println("SecurityException:" + e);
                return new XStub(node, "SecurityException");
            }

            cache.put(tag, constructor);
        }

        try {
            component = (Component) constructor.newInstance(new Object[] {node});
        } catch (InstantiationException e) {
            System.err.println("InstantiationException:" + e);
            return new XStub(node, "InstantiationException");
        } catch (IllegalAccessException e) {
            System.err.println("IllegalAccessException:" + e);
            return new XStub(node, "IllegalAccessException");
        } catch (IllegalArgumentException e) {
            System.err.println("IllegalArgumentException:" + e);
            return new XStub(node, "IllegalArgumentException");
        } catch (InvocationTargetException e) {
            System.err.println("InvocationTargetException:" + e.getTargetException());
            e.getTargetException().printStackTrace();
            return new XStub(node, "InvocationTargetException");
        }

        return component;
    }

    private static interface MyAction {
        Component getComponent(final Node node);
    }

    /**
     * Graphical indication of what goes wrong in buildPeerFor(node).
     */
    private static class XStub extends JPanel {

        /**
         * Serialization id.
         */
        private static final long serialVersionUID = -6540983459186007758L;

        /**
         * Default height.
         */
        private static final int D_HEIGHT = 50;

        /**
         * Default width.
         */
        private static final int D_WIDTH = 100;

        /** Constructor.
         * @param tag : class name source of the error
         * @param cause : description of the error
         *
         */
        public XStub(final Node node, final String cause) {
            super();
            Border red = BorderFactory.createLineBorder(Color.RED);
            TitledBorder title = BorderFactory.createTitledBorder(red, node.getNodeName() + " " + cause);
            Dimension dimension = new Dimension(D_WIDTH, D_HEIGHT);
            setPreferredSize(dimension);
            setOpaque(false);
            XConfigManager.setDimension(this, node);

            title.setTitleColor(Color.RED);
            setBorder(title);
            setLayout(new FlowLayout());
        }

        /**
         * Output method.
         * @return the string representation
         */
        public String toString() {
            return "STUB";
        }
    }
}
