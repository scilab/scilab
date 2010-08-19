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

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;

import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Dump the graph into scilab.
 * 
 * This action is only used for debugging purpose but not on any release version.
 */
public class DumpAction extends DefaultAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.DUMP;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
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
	@Override
    public void actionPerformed(ActionEvent e) {
		try {
		    File temp = FileUtils.createTempFile();
		    temp.deleteOnExit();
		    ((XcosDiagram) getGraph(e)).dumpToHdf5File(temp);
		    try {
		    	String cmd = buildCall("import_from_hdf5", temp.getAbsolutePath());
		    	cmd += buildCall("deletefile", temp.getAbsolutePath());
		    	ScilabInterpreterManagement.synchronousScilabExec(cmd);
		    } catch (InterpreterException e1) {
				LogFactory.getLog(DumpAction.class).error(e1);
			}
		} catch (IOException e1) {
			LogFactory.getLog(DumpAction.class).error(e1);
		}
    }
}
