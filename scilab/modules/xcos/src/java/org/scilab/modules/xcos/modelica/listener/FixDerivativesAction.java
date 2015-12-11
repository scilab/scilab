/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
