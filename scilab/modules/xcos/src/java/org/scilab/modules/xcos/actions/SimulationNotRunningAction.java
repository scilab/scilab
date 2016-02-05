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

package org.scilab.modules.xcos.actions;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.actions.base.GraphActionManager;

/**
 * This action is enable when the simulation is not running, otherwise it is
 * disabled.
 */
@SuppressWarnings(value = { "serial" })
public abstract class SimulationNotRunningAction extends DefaultAction {
    /**
     * Default constructor
     *
     * @param scilabGraph
     *            the associated scilab graph
     */
    public SimulationNotRunningAction(ScilabGraph scilabGraph) {
        super(scilabGraph);

        GraphActionManager.get(scilabGraph, StartAction.class)
        .addPropertyChangeListener(new PropertyChangeListener() {
            @Override
            public void propertyChange(PropertyChangeEvent evt) {
                if (evt != null && evt.getNewValue() instanceof Boolean) {
                    setEnabled((Boolean) evt.getNewValue());
                }
            }
        });
    }

}
