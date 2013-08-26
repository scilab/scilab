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

package org.scilab.modules.xcos.block.actions;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.actions.base.SpecificCellSelectedConstraint;
import org.scilab.modules.xcos.block.SuperBlock;

/**
 * An action which is only active when a {@link SuperBlock} is selected on the
 * graph.
 */
public abstract class SuperBlockSelectedAction extends DefaultAction {

    /**
     * Default constructor
     *
     * @param scilabGraph
     *            The associated graph
     */
    public SuperBlockSelectedAction(ScilabGraph scilabGraph) {
        super(scilabGraph);

        SpecificCellSelectedConstraint c = new SpecificCellSelectedConstraint(
            SuperBlock.class);
        c.install(this, scilabGraph);
    }
}
