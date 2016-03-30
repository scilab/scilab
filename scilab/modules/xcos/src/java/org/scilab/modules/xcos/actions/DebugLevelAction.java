/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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

package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.actions.dialog.DebugLevelDialog;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Set the debug level
 */
@SuppressWarnings(value = { "serial" })
public final class DebugLevelAction extends SimulationNotRunningAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.SET_DEBUG;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * Get the enum level value
     */
    public static enum DebugLevel {
        /** No trace nor debug */
        ZERO(0, XcosMessages.DEBUGLEVEL_0), /** Discrete/Continuous switch */
        ONE(1, XcosMessages.DEBUGLEVEL_1), /** Per block trace */
        TWO(2, XcosMessages.DEBUGLEVEL_2), /** Debug blocks without trace */
        THREE(3, XcosMessages.DEBUGLEVEL_3);

        private int level;
        private String debugName;

        /**
         * @param realNumber
         *            level
         * @param name
         *            debug name
         */
        private DebugLevel(int realNumber, String name) {
            level = realNumber;
            debugName = name;
        }

        /**
         * @return value
         */
        public int getValue() {
            return level;
        }

        /**
         * @return the localized debug name
         * @see java.lang.Enum#toString()
         */
        @Override
        public String toString() {
            return debugName;
        }
    }

    /**
     * Constructor
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     */
    public DebugLevelAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Action !!!
     *
     * @param e
     *            action parameters
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        final DebugLevelDialog dialog = new DebugLevelDialog(graph.getAsComponent(), new ScicosParameters(graph.getUID(), graph.getKind()));

        dialog.pack();
        dialog.setVisible(true);
    }

    /**
     * @param scilabGraph
     *            graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, DebugLevelAction.class);
    }
}
