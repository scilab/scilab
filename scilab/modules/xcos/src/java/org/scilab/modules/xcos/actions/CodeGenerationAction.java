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

import static org.scilab.modules.graph.utils.ScilabInterpreterManagement.asynchronousScilabExec;
import static org.scilab.modules.graph.utils.ScilabInterpreterManagement.buildCall;
import static org.scilab.modules.hdf5.write.H5Write.closeFile;
import static org.scilab.modules.hdf5.write.H5Write.createFile;
import static org.scilab.modules.hdf5.write.H5Write.writeInDataSet;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.utils.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.actions.SuperBlockSelectedAction;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.BlockReader;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;

/**
 * Generate code for the current graph.
 */
public class CodeGenerationAction extends SuperBlockSelectedAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.CODE_GENERATION;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = 0;
	
    /**
	 * Constructor
	 * @param scilabGraph associated Scilab graph
	 */
    public CodeGenerationAction(ScilabGraph scilabGraph) {
    	super(scilabGraph);
    }

    /**
	 * Menu for diagram menubar
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
    	return createMenu(scilabGraph, CodeGenerationAction.class);
    }

	/**
	 * Action !!!
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		Object selectedObj = getGraph(null).getSelectionCell();
			if (!(selectedObj instanceof SuperBlock)) {
				((XcosDiagram) getGraph(null)).error(XcosMessages.ERROR_GENERATING_C_CODE);
				return;
			}
	
		((XcosDiagram) getGraph(null)).info(XcosMessages.GENERATING_C_CODE);
		
		final SuperBlock block = (SuperBlock) selectedObj;
		try {
				/*
				 * Prepare data
				 */
				final File tempOutput = FileUtils.createTempFile();
				final File tempInput = FileUtils.createTempFile();
				
			    /*
			     * Export data
			     */
			    int fileId = createFile(tempOutput.getAbsolutePath());
			    writeInDataSet(fileId, "scs_m", block.getAsScilabObj());
			    closeFile(fileId);
			    
			    /*
			     * Prepare command and callback
			     */
				String cmd = buildCall("xcosCodeGeneration",
						tempOutput.getAbsolutePath(),
						tempInput.getAbsolutePath());
				
				final ActionListener callback = new ActionListener() {
					public void actionPerformed(ActionEvent arg0) {
						doAction(block, tempInput);
						
						tempOutput.delete();
						tempInput.delete();
						
						((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
					}
				};
				
				/*
				 * Execute
				 */
				asynchronousScilabExec(callback, cmd);
				
		} catch (IOException ex) {
			LogFactory.getLog(CodeGenerationAction.class).error(ex);
		    ((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
		} catch (HDF5Exception ex) {
			LogFactory.getLog(CodeGenerationAction.class).error(ex);
			((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
		} catch (InterpreterException ex) {
			LogFactory.getLog(CodeGenerationAction.class).error(ex);
			((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
		}
    }
    
    /**
     * Callback function
     * 
     * Read the block from the scilab 
     * 
     * @param block The block we are working on
     * @param tempInput Input file
     */
    private static void doAction(final SuperBlock block,
			final File tempInput) {
	    BasicBlock modifiedBlock = BlockReader.readBlockFromFile(tempInput.getAbsolutePath());
	    block.updateBlockSettings(modifiedBlock);
	    block.setInterfaceFunctionName(modifiedBlock.getInterfaceFunctionName());
	    block.setSimulationFunctionName(modifiedBlock.getSimulationFunctionName());
	    block.setSimulationFunctionType(modifiedBlock.getSimulationFunctionType());
	    mxUtils.setCellStyles(block.getParentDiagram().getModel(),
		    new Object[] {block} , mxConstants.STYLE_SHAPE, mxConstants.SHAPE_RECTANGLE);
	    block.setValue(block.getSimulationFunctionName());
	    block.setChild(null);
	}
}
