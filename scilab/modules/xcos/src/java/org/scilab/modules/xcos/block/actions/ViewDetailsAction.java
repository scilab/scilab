/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.actions;

import java.io.File;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.BasicBlockInfo;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.utils.XcosMessages;


public class ViewDetailsAction extends DefaultAction {

    private ViewDetailsAction(ScilabGraph scilabGraph) {
	super(XcosMessages.DETAILS, scilabGraph);
    }

    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.DETAILS, null, new ViewDetailsAction(scilabGraph), null);
    }

    public void doAction() {
	XcosDiagram graph = (XcosDiagram) getGraph(null);
	Object[] selectedCells = graph.getSelectionCells();

	//if no cells are selected : Do nothing
	if (selectedCells.length == 0) { return; }

	for (int i = 0; i < selectedCells.length ; ++i) {
	    if ((selectedCells[i] instanceof BasicBlock) && !(selectedCells[i] instanceof SplitBlock)) {
		try {
		    File temp = File.createTempFile("xcos",".h5");
		    temp.deleteOnExit();
		    int file_id = H5Write.createFile(temp.getAbsolutePath());
		    H5Write.writeInDataSet(file_id, "scs_m", BasicBlockInfo.getAsScilabObj((BasicBlock) selectedCells[i]));
		    H5Write.closeFile(file_id);
		    InterpreterManagement.requestScilabExec("import_from_hdf5(\""+temp.getAbsolutePath()+"\");tree_show(scs_m);deletefile(\"" + temp.getAbsolutePath()+"\");");
		}
		catch (Exception e) {
		    // Do Nothing !!!
		}
	    }
	    if (selectedCells[i] instanceof BasicLink) {
		try {
		    File temp = File.createTempFile("xcos",".h5");
		    temp.deleteOnExit();
		    int file_id = H5Write.createFile(temp.getAbsolutePath());
		    H5Write.writeInDataSet(file_id, "scs_m", ((BasicLink) selectedCells[i]).getAsScilabObj());
		    H5Write.closeFile(file_id);
		    InterpreterManagement.requestScilabExec("import_from_hdf5(\""+temp.getAbsolutePath()+"\");tree_show(scs_m);deletefile(\"" + temp.getAbsolutePath()+"\");");;
		}
		catch (Exception e) {
		    // Do Nothing !!!
		}
	    }
	}

    }
}
