/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

package org.scilab.modules.xcos.block.actions.alignement;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Align block on the left
 */
@SuppressWarnings(value = { "serial" })
public final class AlignBlockActionLeft extends AlignBlockAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.ALIGN_LEFT;
    /** Icon name of the action */
    public static final String SMALL_ICON = "align-horizontal-left";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * Default constructor
     *
     * @param scilabGraph
     *            the graph to work on
     */
    public AlignBlockActionLeft(ScilabGraph scilabGraph) {
        super(scilabGraph, XcosConstants.ALIGN_LEFT);
    }

    /**
     * Create the menu associated with this action.
     *
     * @param scilabGraph
     *            the associated graph
     * @return The associated menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, AlignBlockActionLeft.class);
    }
}
