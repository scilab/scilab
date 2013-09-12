/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
public abstract class OneSelectionDependantAction extends DefaultAction {
    /**
     * Default constructor
     *
     * @param scilabGraph
     *            the associated graph
     */
    public OneSelectionDependantAction(ScilabGraph scilabGraph) {
        super(scilabGraph);

        if (scilabGraph != null) {
            SelectedNumberOfCellsConstraint constraint = new SelectedNumberOfCellsConstraint(1);
            constraint.install(this, scilabGraph);
        }
    }
}
