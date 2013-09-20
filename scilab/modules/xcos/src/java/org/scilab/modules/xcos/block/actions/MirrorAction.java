/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.xcos.block.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Mirror a block
 */
@SuppressWarnings(value = { "serial" })
public class MirrorAction extends VertexSelectionDependantAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.MIRROR;
    /** Icon name of the action */
    public static final String SMALL_ICON = "transform-mirror";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_M;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit()
            .getMenuShortcutKeyMask();

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated diagram
     */
    public MirrorAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Menu added to the menubar
     *
     * @param scilabGraph
     *            associated diagram
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, MirrorAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        if (((XcosDiagram) getGraph(null)).getSelectionCells().length != 0) {

            Object[] allCells = ((XcosDiagram) getGraph(null))
                                .getSelectionCells();

            getGraph(null).getModel().beginUpdate();
            for (int i = 0; i < allCells.length; ++i) {
                if (allCells[i] instanceof BasicBlock) {
                    ((BasicBlock) allCells[i]).toggleMirror();
                }
            }
            getGraph(null).getModel().endUpdate();
        }
    }

}
