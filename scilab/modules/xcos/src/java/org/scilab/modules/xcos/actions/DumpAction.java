/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
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
import java.io.File;
import java.io.IOException;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement.InterpreterException;

/**
 * Dump the graph into scilab.
 */
public class DumpAction extends DefaultAction {
	public static final String NAME = XcosMessages.DUMP;
	public static final String SMALL_ICON = "";
	public static final int MNEMONIC_KEY = 0;
	public static final int ACCELERATOR_KEY = 0;

    /**
     * @param scilabGraph graph
     */
    public DumpAction(ScilabGraph scilabGraph) {
    	super(scilabGraph);
    }

    /**
     * @param scilabGraph graph
     * @return push button
     */
    public static PushButton dumpButton(ScilabGraph scilabGraph) {
    	return createButton(scilabGraph, DumpAction.class);
    }

    /**
     * @param scilabGraph graph
     * @return menu item
     */
    public static MenuItem dumpMenu(ScilabGraph scilabGraph) {
    	return createMenu(scilabGraph, DumpAction.class);
    }

    /**
     * Do action !!!
     * @param e params
     * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
	try {
	    File temp = File.createTempFile("xcos", ".h5");
	    temp.deleteOnExit();
	    ((XcosDiagram) getGraph(e)).dumpToHdf5File(temp.getAbsolutePath());
	    try {
			XcosInterpreterManagement.synchronousScilabExec("import_from_hdf5(\"" + temp.getAbsolutePath() + "\");"
				+ "deletefile(\"" + temp.getAbsolutePath() + "\");");
		} catch (InterpreterException e1) {
			e1.printStackTrace();
		}
	} catch (IOException e1) {
	    e1.printStackTrace();
	}
    }
}
