/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
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

package org.scilab.modules.xcos.block.actions;

import static org.scilab.modules.graph.utils.ScilabInterpreterManagement.buildCall;

import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.graph.utils.ScilabInterpreterManagement;
import org.scilab.modules.graph.utils.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.types.scilabTypes.ScilabMList;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.BasicBlockInfo;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosMessages;


/**
 * View the details of the action
 */
public final class ViewDetailsAction extends VertexSelectionDependantAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.DETAILS;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = 0;

    /**
     * Constructor
     * @param scilabGraph graph
     */
    public ViewDetailsAction(ScilabGraph scilabGraph) {
	super(scilabGraph);
    }

    /**
     * @param scilabGraph graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(scilabGraph, ViewDetailsAction.class);
    }

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		XcosDiagram graph = (XcosDiagram) getGraph(null);
		Object[] selectedCells = graph.getSelectionCells();
	
		//if no cells are selected : Do nothing
		if (selectedCells.length == 0) { return; }
	
		for (int i = 0; i < selectedCells.length; ++i) {
		    if ((selectedCells[i] instanceof BasicBlock) && !(selectedCells[i] instanceof SplitBlock)) {
		    	BasicBlock instance = (BasicBlock) selectedCells[i];
				viewDetails(BasicBlockInfo.getAsScilabObj(instance));
		    } else if (selectedCells[i] instanceof BasicLink) {
		    	BasicLink instance = (BasicLink) selectedCells[i];
				viewDetails(instance.getAsScilabObj());
		    }
		}
    }

	/**
	 * View the data details
	 * @param data the selected block or link data
	 */
	private void viewDetails(ScilabMList data) {
		/*
		 * Export data
		 */
		File temp;
		try {
			temp = FileUtils.createTempFile();
		} catch (IOException e1) {
			LogFactory.getLog(ViewDetailsAction.class).error(e1);
			return;
		}

		int fileId = H5Write.createFile(temp.getAbsolutePath());
		try {
			H5Write.writeInDataSet(fileId, "scs_m", data);
		} catch (HDF5Exception e1) {
			LogFactory.getLog(ViewDetailsAction.class).error(e1);
			return;
		}
		H5Write.closeFile(fileId);
		
		/*
		 * Build and execute the command
		 */
		String cmd = buildCall("import_from_hdf5", temp.getAbsolutePath());
		cmd += "tree_show(scs_m); ";
		cmd += buildCall("deletefile", temp.getAbsolutePath());
		try {
			ScilabInterpreterManagement.synchronousScilabExec(cmd);
		} catch (InterpreterException e1) {
			LogFactory.getLog(ViewDetailsAction.class).error(e1);
		}
	}
}
