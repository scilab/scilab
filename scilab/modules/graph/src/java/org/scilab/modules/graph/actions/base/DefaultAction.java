/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009-2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.graph.actions.base;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.lang.ref.WeakReference;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.bridge.checkboxmenuitem.SwingScilabCheckBoxMenuItem;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.checkboxmenuitem.ScilabCheckBoxMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;

import com.mxgraph.swing.mxGraphComponent;

/**
 * Default action for a Scilab Graph
 */
public abstract class DefaultAction extends CommonCallBack {
    private final WeakReference<ScilabGraph> scilabGraph;

    /**
     * Default constructor.
     *
     * The {@link AbstractAction} object is configured using the reflection API.
     * So you have to be sure that the following fields are declared as static
     * final fields of each subclasses.
     * <ul>
     * <li>String NAME : The name of the action</li>
     * <li>String SMALL_ICON : The associated icon name (located on
     * $SCI/modules/gui/images/icons)</li>
     * <li>int MNEMONIC_KEY : The key associated with the action (see
     * {@link KeyEvent})</li>
     * <li>int ACCELERATOR_KEY : The key mask to apply to the mnemonic</li>
     * </ul>
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     */
    public DefaultAction(ScilabGraph scilabGraph) {
        super("");
        this.scilabGraph = new WeakReference<ScilabGraph>(scilabGraph);

        installProperties();
    }

    /**
     * Install the static actions properties on the instance
     */
    private void installProperties() {
        String name = "";
        ImageIcon icon = null;
        int mnemonic = 0;
        int accelerator = 0;
        try {
            name = (String) getClass().getField("NAME").get(null);

            /*
             * Getting icon from the registered icon path
             */
            String iconName = (String) getClass().getField("SMALL_ICON").get(null);
            if (iconName != null && !iconName.isEmpty()) {
                icon = new ImageIcon(FindIconHelper.findIcon(iconName));
            }

            mnemonic = getClass().getField("MNEMONIC_KEY").getInt(null);
            accelerator = getClass().getField("ACCELERATOR_KEY").getInt(null);
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        }

        assert !"".equals(name);
        putValue(Action.NAME, name);
        putValue(Action.SHORT_DESCRIPTION, name);
        putValue(Action.LONG_DESCRIPTION, name);
        if (icon != null) {
            putValue(Action.SMALL_ICON, icon);
        }

        /*
         * Set up the accelerator instead of the mnemonic as the menu is the
         * preferred way on keyboard control. We are using Action.MNEMONIC_KEY
         * as keyboard key and Action.ACCELERATOR_KEY as a mask.
         *
         * Install it only when there is a real shortcut (with a mnemonic).
         */
        if (mnemonic != 0) {
            putValue(Action.MNEMONIC_KEY, mnemonic);
            putValue(Action.ACCELERATOR_KEY, KeyStroke.getKeyStroke(mnemonic, accelerator));
        }
    }

    /**
     * Create a menu item associated with the graph
     *
     * @param graph
     *            the graph to work on
     * @param klass
     *            the associated klass
     * @return the menu item
     */
    protected static MenuItem createMenu(ScilabGraph graph, final Class <? extends DefaultAction > klass) {
        DefaultAction action = GraphActionManager.getInstance(graph, klass);
        MenuItem item = ScilabMenuItem.createMenuItem();

        SwingScilabMenuItem swingItem = (SwingScilabMenuItem) item.getAsSimpleMenuItem();
        swingItem.setAction(action);

        return item;
    }

    /**
     * Create a menu item associated with the graph
     *
     * @param graph
     *            the graph to work on
     * @param klass
     *            the associated klass
     * @return the push button
     */
    protected static JButton createButton(ScilabGraph graph, final Class <? extends DefaultAction > klass) {
        DefaultAction action = GraphActionManager.getInstance(graph, klass);
        JButton item = new JButton();

        item.setAction(action);

        // Hide the text on buttons
        item.setHideActionText(true);

        return item;
    }

    /**
     * Create a menu item associated with the graph
     *
     * @param graph
     *            the graph to work on
     * @param klass
     *            the associated klass
     * @return the checkbox item
     */
    protected static CheckBoxMenuItem createCheckBoxMenu(ScilabGraph graph, Class <? extends DefaultAction > klass) {
        DefaultAction action = GraphActionManager.getInstance(graph, klass);
        CheckBoxMenuItem item = ScilabCheckBoxMenuItem.createCheckBoxMenuItem();

        SwingScilabCheckBoxMenuItem swingItem = (SwingScilabCheckBoxMenuItem) item.getAsSimpleCheckBoxMenuItem();
        swingItem.setAction(action);

        return item;
    }

    /**
     * Constructor
     *
     * @param label
     *            action descriptor
     * @param scilabGraph
     *            associated Scilab Graph
     */
    @Deprecated
    protected DefaultAction(String label, ScilabGraph scilabGraph) {
        super(label);
        this.scilabGraph = new WeakReference<ScilabGraph>(scilabGraph);
    }

    /**
     * Get associated graph
     *
     * @param e
     *            event
     * @return Returns the graph for the given action event.
     */
    protected final ScilabGraph getGraph(ActionEvent e) {
        final ScilabGraph graph = scilabGraph.get();
        if (graph != null) {
            return graph;
        }

        if (e == null) {
            return null;
        }

        if (e.getSource() instanceof Component) {
            Component component = (Component) e.getSource();

            while (component != null && !(component instanceof mxGraphComponent)) {
                component = component.getParent();
            }

            return (ScilabGraph) ((mxGraphComponent) component).getGraph();
        }

        return null;
    }

    /**
     * Create a button for a graph toolbar
     *
     * @param title
     *            label of the menu
     * @param icon
     *            the path the an icon file
     * @param listener
     *            action listener associated
     * @param keyStroke
     *            menu shortcut
     * @return the button
     */
    @Deprecated
    protected static MenuItem createMenu(String title, String icon, DefaultAction listener, KeyStroke keyStroke) {
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(listener);
        menu.setText(title);

        if (keyStroke != null) {
            ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(keyStroke);
        }

        return menu;
    }

    /**
     * Action
     *
     * @param e
     *            parameters
     * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public abstract void actionPerformed(ActionEvent e);

    /**
     * Not used
     *
     * @see org.scilab.modules.gui.events.callback.CallBack#callBack()
     */
    @Override
    public void callBack() {
        assert "Must never be called as we bypass Callback.java".equals("");
    }
}
