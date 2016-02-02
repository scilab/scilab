/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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
package org.scilab.modules.xcos.block.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.BlockPositioning;
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
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

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

            Object[] allCells = ((XcosDiagram) getGraph(null)).getSelectionCells();

            try {
                getGraph(null).getModel().beginUpdate();

                for (int i = 0; i < allCells.length; ++i) {
                    if (allCells[i] instanceof BasicBlock) {
                        BlockPositioning.toggleMirror((XcosDiagram) getGraph(null), (BasicBlock) allCells[i]);
                    }
                }
            } finally {
                getGraph(null).getModel().endUpdate();
            }
        }
    }

}
