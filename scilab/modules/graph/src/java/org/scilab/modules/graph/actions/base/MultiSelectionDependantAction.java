/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

import org.scilab.modules.graph.ScilabGraph;

/**
 * Common class for multi-selection dependent actions.
 *
 * Children of this class will be activated when there something selected. If
 * not, the action will be disabled.
 */
public abstract class MultiSelectionDependantAction extends DefaultAction {
    /**
     * Default constructor
     *
     * @param scilabGraph
     *            the associated graph
     */
    public MultiSelectionDependantAction(ScilabGraph scilabGraph) {
        super(scilabGraph);

        if (scilabGraph != null) {
            SelectedNumberOfCellsConstraint constraint = new SelectedNumberOfCellsConstraint(2);
            constraint.install(this, scilabGraph);
        }
    }
}
