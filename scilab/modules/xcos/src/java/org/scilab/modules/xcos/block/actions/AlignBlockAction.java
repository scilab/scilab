/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.actions;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.utils.XcosMessages;

public class AlignBlockAction extends DefaultAction {

    private static final long serialVersionUID = 6004051237556006262L;
    private String align;
    
    private AlignBlockAction(ScilabGraph scilabGraph) {
	super(XcosMessages.ALIGN_BLOCKS, scilabGraph);
    }

    private AlignBlockAction(ScilabGraph scilabGraph, String align) {
	this(scilabGraph);
	this.align = align;
    }

    public static MenuItem createMenu(ScilabGraph scilabGraph, String alignMessage, String alignValue) {
	return createMenu(alignMessage, null, new AlignBlockAction(scilabGraph, alignValue), null);
    }	
    
    public void doAction() {
	getGraph(null).alignCells(align);
    }
    
}
