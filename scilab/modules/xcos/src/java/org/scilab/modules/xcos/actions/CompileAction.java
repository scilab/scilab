/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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
import static org.scilab.modules.xcos.utils.FileUtils.delete;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

import javax.swing.SwingWorker;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Diagram compilation management
 */
public class CompileAction extends SimulationNotRunningAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.COMPILE;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = 0;

	/**
	 * Constructor
	 * @param scilabGraph associated diagram
	 */
	public CompileAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create associated menu
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, CompileAction.class);
	}

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		((XcosDiagram) getGraph(null)).info(XcosMessages.EXPORT_IN_PROGRESS);
		
		final File temp;
		try {
			temp = FileUtils.createTempFile();
		} catch (IOException e1) {
			LogFactory.getLog(CompileAction.class).error(e1);
			return;
		}
		
		(new SwingWorker<Void, Void>() {

			@Override
			protected Void doInBackground() throws Exception {
				((XcosDiagram) getGraph(null)).dumpToHdf5File(temp);
				((XcosDiagram) getGraph(null)).setReadOnly(true);
				return null;
			}
			
			@Override
			protected void done() {
				((XcosDiagram) getGraph(null)).info(XcosMessages.COMPILATION_IN_PROGRESS);
				
				String cmd = buildCall("import_from_hdf5", temp.getAbsolutePath()) 
				                     + "xcos_compile(scs_m);";
				
				final ActionListener action = new ActionListener() {
					@Override
					public void actionPerformed(ActionEvent e) {
						delete(temp);
						((XcosDiagram) getGraph(null)).setReadOnly(false);
						((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
					}
				};
				
				try {
					ScilabInterpreterManagement.asynchronousScilabExec(action, cmd);
				} catch (InterpreterException e) {
					LogFactory.getLog(CompileAction.class).error(e);
				}
			}
			
		}).execute();
	}
}
