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

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Start the simulation
 */
public class StartAction extends DefaultAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.START;
	/** Icon name of the action */
	public static final String SMALL_ICON = "media-playback-start.png";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
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
	@Override
    public void actionPerformed(ActionEvent e) {
		final XcosDiagram diagram = ((XcosDiagram) getGraph(e)).getRootDiagram();
		String cmd;
		
		updateUI(true);
	
		try {
			cmd = createSimulationCommand(diagram);
		} catch (IOException ex) {
			LogFactory.getLog(StartAction.class).error(ex);
			updateUI(false);
			return;
		}
		
		final ActionListener action = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				updateUI(false);
				diagram.getEngine().setCompilationNeeded(false);
			}
		};
		
	    try {
			asynchronousScilabExec(action, cmd);
		} catch (InterpreterException e1) {
			e1.printStackTrace();
			updateUI(false);
		}
    }

	/**
	 * Create the command String
	 * 
	 * @param diagram
	 *            the working diagram
	 * @return the command string
	 * @throws IOException
	 *             when temporary files must not be created.
	 */
	private String createSimulationCommand(final XcosDiagram diagram)
			throws IOException {
		String cmd;
		final StringBuilder command = new StringBuilder();
		
		/*
		 * Log compilation info
		 */
		final Log log = LogFactory.getLog(StartAction.class);
		log.trace("start simulation");
		
		/*
		 * Import a valid scs_m structure into Scilab
		 */
		final File temp = FileUtils.createTempFile();
		diagram.dumpToHdf5File(temp);

		command.append(buildCall("import_from_hdf5", temp.getAbsolutePath()));
		command.append(buildCall("scicos_debug", diagram.getScicosParameters().getDebugLevel()));
		
		/*
		 * Simulate
		 */
		command.append("xcos_simulate(scs_m, 4); ");

		cmd = command.toString();
		return cmd;
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
