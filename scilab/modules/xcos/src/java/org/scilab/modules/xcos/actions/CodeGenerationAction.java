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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.actions.SuperBlockSelectedAction;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.BlockReader;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosFileType;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement.InterpreterException;

import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;

/**
 * Generate code for the current graph.
 */
public class CodeGenerationAction extends SuperBlockSelectedAction {
	public static final String NAME = XcosMessages.CODE_GENERATION;
	public static final String SMALL_ICON = "";
	public static final int MNEMONIC_KEY = 0;
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
			final File tempOutput = File.createTempFile(XcosFileType.XCOS
					.getExtension(), XcosFileType.HDF5.getExtension(),
					new File(System.getenv(XcosConstants.TMPDIR)));
			final File tempInput = File.createTempFile(XcosFileType.XCOS
					.getExtension(), XcosFileType.HDF5.getExtension(),
					new File(System.getenv(XcosConstants.TMPDIR)));
	    tempOutput.deleteOnExit();
	    tempInput.deleteOnExit();
	    // Write scs_m
	    int fileId = H5Write.createFile(tempOutput.getAbsolutePath());
	    H5Write.writeInDataSet(fileId, "scs_m", block.getAsScilabObj());
	    H5Write.closeFile(fileId);
	    
			String command = "xcosCodeGeneration(\""
					+ tempOutput.getAbsolutePath() + "\"" + ", \""
					+ tempInput.getAbsolutePath() + "\");";
	    
			final ActionListener callback = new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					doAction(block, tempInput);
					((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
				}
			};
			
			XcosInterpreterManagement.asynchronousScilabExec(command, callback);
	} catch (IOException ex) {
		ex.printStackTrace();
	    ((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
	} catch (HDF5Exception ex) {
		ex.printStackTrace();
		((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
	} catch (InterpreterException ex) {
		ex.printStackTrace();
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
	    block.setStyle("blockWithLabel");
	    mxUtils.setCellStyles(block.getParentDiagram().getModel(),
		    new Object[] {block} , mxConstants.STYLE_SHAPE, mxConstants.SHAPE_RECTANGLE);
	    block.setValue(block.getSimulationFunctionName());
	    block.setChild(null);
	}
}
