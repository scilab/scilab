/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
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
public class CodeGenerationAction extends DefaultAction {

    private static final long serialVersionUID = -7756467773530338202L;

    /**
	 * Constructor
	 * @param scilabGraph associated Scilab graph
	 */
    public CodeGenerationAction(ScilabGraph scilabGraph) {
	super(XcosMessages.CODE_GENERATION, scilabGraph);
    }

    /**
	 * Create a button to add in Scilab Graph tool bar
	 * @param scilabGraph associated Scilab Graph
	 * @return the button
	 */
    public static PushButton createButton(ScilabGraph scilabGraph) {
	return createButton(XcosMessages.CODE_GENERATION, null, new CodeGenerationAction(scilabGraph));
    }

    /**
	 * Menu for diagram menubar
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.CODE_GENERATION, null, new CodeGenerationAction(scilabGraph), null);
    }

    /**
	 * Action !!
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
    public void doAction() {
	Object selectedObj = getGraph(null).getSelectionCell();
		if (!(selectedObj instanceof SuperBlock)) {
			((XcosDiagram) getGraph(null)).error(XcosMessages.ERROR_GENERATING_C_CODE);
			return;
		}

	((XcosDiagram) getGraph(null)).info(XcosMessages.GENERATING_C_CODE);
	
	final SuperBlock block = (SuperBlock) selectedObj;
	try {
	    final File tempOutput = File.createTempFile(XcosFileType.XCOS.getExtension(), XcosFileType.HDF5.getExtension(), new File(System.getenv(XcosConstants.TMPDIR)));
	    final File tempInput = File.createTempFile(XcosFileType.XCOS.getExtension(), XcosFileType.HDF5.getExtension(), new File(System.getenv(XcosConstants.TMPDIR)));
	    tempOutput.deleteOnExit();
	    tempInput.deleteOnExit();
	    // Write scs_m
	    int fileId = H5Write.createFile(tempOutput.getAbsolutePath());
	    H5Write.writeInDataSet(fileId, "scs_m", block.getAsScilabObj());
	    H5Write.closeFile(fileId);
	    
			String command = "xcosCodeGeneration(\""
					+ tempOutput.getAbsolutePath() + "\"" + ", \""
					+ tempInput.getAbsolutePath() + "\");";
	    
		try {
			XcosInterpreterManagement.asynchronousScilabExec(command, new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					// Now read new Block
				    BasicBlock modifiedBlock = BlockReader.readBlockFromFile(tempInput.getAbsolutePath());
				    block.updateBlockSettings(modifiedBlock);
				    block.setInterfaceFunctionName(modifiedBlock.getInterfaceFunctionName());
				    block.setSimulationFunctionName(modifiedBlock.getSimulationFunctionName());
				    block.setSimulationFunctionType(modifiedBlock.getSimulationFunctionType());
				    block.setStyle("blockWithLabel");
				    mxUtils.setCellStyles(block.getParentDiagram().getModel(),
					    new Object[] {block}, mxConstants.STYLE_SHAPE, mxConstants.SHAPE_RECTANGLE);
				    block.setValue(block.getSimulationFunctionName());
				    block.setChild(null);
				    ((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
				}
			});
		} catch (InterpreterException e) {
			e.printStackTrace();
		    ((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
		}
	} catch (IOException e) {
	    e.printStackTrace();
	    ((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
	} catch (HDF5Exception e) {
		e.printStackTrace();
		((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
	}
    }
}
