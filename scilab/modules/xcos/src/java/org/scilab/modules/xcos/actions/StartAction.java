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
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement.InterpreterException;

/**
 * Start the simulation
 */
public class StartAction extends DefaultAction {
	public static final String NAME = XcosMessages.START;
	public static final String SMALL_ICON = "media-playback-start.png";
	public static final int MNEMONIC_KEY = 0;
	public static final int ACCELERATOR_KEY = 0;
	
    /**
     * @param scilabGraph graph
     */
    public StartAction(ScilabGraph scilabGraph) {
    	super(scilabGraph);
    	setEnabled(GraphActionManager.getEnable(StartAction.class));
    }

    /**
     * @param scilabGraph graph
     * @return push button
     */
    public static PushButton createButton(ScilabGraph scilabGraph) {
    	return createButton(scilabGraph, StartAction.class);
    }

    /**
     * @param scilabGraph graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
    	return createMenu(scilabGraph, StartAction.class);
    }

    /**
     * Action !!!
     * @param e the source event
     * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
	File temp;
	updateUI(true);
	
	try {
	    temp = File.createTempFile("xcos",".h5");
	    ((XcosDiagram) getGraph(e)).getRootDiagram().dumpToHdf5File(temp.getAbsolutePath());

	    String command = "import_from_hdf5(\"" + temp.getAbsolutePath() + "\");"
	    				+ "scicos_debug(" + ((XcosDiagram) getGraph(e)).getDebugLevel() + ");"
	    				+ "xcos_simulate(scs_m);"
	    				+ "deletefile(\"" + temp.getAbsolutePath() + "\");";
	    try {
			XcosInterpreterManagement.asynchronousScilabExec(command, new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					updateUI(false);
				}
			});
		} catch (InterpreterException e1) {
			e1.printStackTrace();
			updateUI(false);
		}
	} catch (IOException e1) {
	    e1.printStackTrace();
	    updateUI(false);
	}
    }

	/**
	 * Update the UI depending on the action selected or not
	 * @param started the started status
	 */
	public void updateUI(boolean started) {
		GraphActionManager.setEnable(StartAction.class, !started);
		GraphActionManager.setEnable(StopAction.class, started);
		((XcosDiagram) getGraph(null)).setReadOnly(started);
		
		if (started) {
			((XcosDiagram) getGraph(null)).info(XcosMessages.SIMULATION_IN_PROGRESS);
		} else {
			((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
		}
	}
}
