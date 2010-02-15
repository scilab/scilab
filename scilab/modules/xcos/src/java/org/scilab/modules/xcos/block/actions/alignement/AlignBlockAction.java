/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.actions.alignement;

import java.awt.event.ActionEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.MultiSelectionDependantAction;

/**
 * Align selected blocks on axes (left-right-centered-top-bottom-middle)
 */
public abstract class AlignBlockAction extends MultiSelectionDependantAction {
    private String align;

    /**
     * @param scilabGraph graph
     * @param align value
     */
    protected AlignBlockAction(ScilabGraph scilabGraph, String align) {
	super(scilabGraph);
	this.align = align;
    }
    
	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
	getGraph(e).alignCells(align);
    }
}
