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
