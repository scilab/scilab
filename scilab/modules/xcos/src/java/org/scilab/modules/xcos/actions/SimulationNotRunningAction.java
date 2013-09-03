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
