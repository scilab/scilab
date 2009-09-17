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

package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;

import javax.swing.TransferHandler;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.XcosDiagram;

public class DumpAction extends DefaultAction {

    public static PushButton dumpButton(ScilabGraph scilabGraph) {
	return createButton("Dump", null, new DumpAction(scilabGraph));
    }
    
    public static MenuItem dumpMenu(ScilabGraph scilabGraph) {
	return createMenu("Dump", null, new DumpAction(scilabGraph));
    }
    
    public DumpAction() {
	super();
    }
    
    public DumpAction(ScilabGraph scilabGraph) {
	super(scilabGraph);
    }
 
    public void actionPerformed(ActionEvent e) {
	((XcosDiagram) getGraph(e)).dumpToHdf5File(null);
    }
}
