/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2011 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.modelica.listener;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;

import org.scilab.modules.xcos.modelica.ModelicaController;
import org.scilab.modules.xcos.modelica.ModelicaMessages;

/**
 * Fix the derivative of the current model
 */
@SuppressWarnings(value = { "serial" })
public final class FixDerivativesAction extends AbstractAction {
    private final ModelicaController controller;

    /**
     * Default constructor
     *
     * @param controller
     *            the associated controller
     */
    public FixDerivativesAction(ModelicaController controller) {
        super();

        putValue(NAME, ModelicaMessages.FIX_DERIVATIVES);
        this.controller = controller;
    }

    /**
     * Update the not fixed derivatives variable to 1.0 and not fixed state
     * variables to 0.0
     *
     * @param e
     *            the event
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        controller.fixWeight(1.0, 0.0);
    }
}
