/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.actions;

import static org.scilab.modules.commons.OS.MAC;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.KeyStroke;

import org.scilab.modules.commons.OS;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;

/**
 * Zoom management
 *
 * @author Bruno JOFFRET
 */
@SuppressWarnings(value = { "serial" })
public class ZoomOutAction extends DefaultAction implements ActionListener {
    /** Name of the action */
    public static final String NAME = ScilabGraphMessages.ZOOM_OUT;
    /** Icon name of the action */
    public static final String SMALL_ICON = "zoom-out";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_SUBTRACT;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /** key used on {@link InputMap} for this action */
    private static final String ZOOM_OUT = "zoomOut";

    /**
     * Implement custom mouse handling for the zoom
     */
    private static final class CustomMouseWheelListener implements MouseWheelListener {
        private final ScilabGraph scilabGraph;

        /**
         * Default constructor
         *
         * @param scilabGraph
         *            the current graph
         */
        public CustomMouseWheelListener(ScilabGraph scilabGraph) {
            this.scilabGraph = scilabGraph;
        }

        /**
         * When the wheel is used
         *
         * @param e
         *            the parameters
         * @see java.awt.event.MouseWheelListener#mouseWheelMoved(java.awt.event.MouseWheelEvent)
         */
        @Override
        public void mouseWheelMoved(MouseWheelEvent e) {
            if ((e.getModifiers() & ACCELERATOR_KEY) != 0) {
                if (e.getWheelRotation() > 0) {
                    scilabGraph.getAsComponent().zoomOut();
                }
            }
        }
    }

    /**
     * Constructor
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     */
    public ZoomOutAction(ScilabGraph scilabGraph) {
        super(scilabGraph);

        MouseWheelListener mouseListener = new CustomMouseWheelListener(scilabGraph);
        scilabGraph.getAsComponent().addMouseWheelListener(mouseListener);

        // Multi-shortcut action
        final ActionMap am = scilabGraph.getAsComponent().getActionMap();
        final InputMap map = scilabGraph.getAsComponent().getInputMap();

        // register the action to a unique action keyword
        am.put(ZOOM_OUT, this);

        // add custom key stroke for this action
        final KeyStroke[] keystrokes;
        if (OS.get() == MAC) {
            /*
             * AZERTY for Mac has a non-supported classic layout
             */
            keystrokes = new KeyStroke[] { KeyStroke.getKeyStroke('=', ACCELERATOR_KEY),
                                           KeyStroke.getKeyStroke('=', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK),
                                         };
        } else {
            keystrokes = new KeyStroke[] { KeyStroke.getKeyStroke('-', ACCELERATOR_KEY),
                                           KeyStroke.getKeyStroke('-', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK), KeyStroke.getKeyStroke('_', ACCELERATOR_KEY),
                                           KeyStroke.getKeyStroke('_', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK),
                                         };
        }

        for (KeyStroke k : keystrokes) {
            map.put(k, ZOOM_OUT);
        }
    }

    /**
     * Create a button for a graph toolbar
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     * @return the button
     */
    public static PushButton zoomoutButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, ZoomOutAction.class);
    }

    /**
     * Create a menu for a graph menubar
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     * @return the menu
     */
    public static MenuItem zoomoutMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, ZoomOutAction.class);
    }

    /**
     * Action associated
     *
     * @param e
     *            the event
     * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        getGraph(e).getAsComponent().zoomOut();
    }

}
