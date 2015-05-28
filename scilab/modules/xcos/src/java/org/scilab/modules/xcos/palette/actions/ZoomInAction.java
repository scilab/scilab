/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.JButton;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;

/**
 * Zoom In Management
 * Increases the icon size
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
@SuppressWarnings(value = { "serial" })
public class ZoomInAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = ScilabGraphMessages.ZOOM_IN;
    /** Icon name of the action */
    public static final String SMALL_ICON = "zoom-in";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_ADD;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
    /** PaletteManagerView instance **/
    private static PaletteManagerView paletteManagerView;

    /**
     * Constructor
     *
     * @param scilabGraph corresponding Scilab Graph
     */
    public ZoomInAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create a button for a graph toolbar
     *
     * @param scilabGraph corresponding Scilab Graph
     * @param view PaletteManagerView instance
     * @return the button
     */
    public static JButton createButton(ScilabGraph scilabGraph, PaletteManagerView view) {
        paletteManagerView = view;
        return createButton(scilabGraph, ZoomInAction.class);
    }

    /**
     * Action associated
     *
     * @param e the event
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        try {
            paletteManagerView.getPanel().zoomIn();
        } catch (NullPointerException err) {
        }
    }
}
