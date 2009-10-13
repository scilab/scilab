/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block;

import javax.swing.JOptionPane;

import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

public class SuperBlock extends BasicBlock {

    public SuperBlock(String label) {
	super(label);
    }

    public void openBlockSettings(String context) {
	this.setLocked(true);	
	XcosDiagram xcosDiagram = new SuperBlockDiagram(this);
	Xcos.showDiagram(xcosDiagram);
	xcosDiagram.loadDiagram(BlockReader.convertMListToDiagram((ScilabMList) getRealParameters()));
	String innerContext = xcosDiagram.getContext();
	if(!innerContext.contains(context)) {
	    innerContext = innerContext + ";" + context;
	    xcosDiagram.setContext(innerContext);
	}
    }

    public String getToolTipText() {
	StringBuffer result = new StringBuffer();
	result.append("<html>");
	result.append("SUPER BLOCK"+"<br>");
	result.append("Input ports : "+getAllInputPorts().size()+"<br>");
	result.append("Output ports : "+getAllOutputPorts().size()+"<br>");
	result.append("Control ports : "+getAllControlPorts().size()+"<br>");
	result.append("Command ports : "+getAllCommandPorts().size()+"<br>");
	result.append("isLocked : "+isLocked()+"<br>");
	result.append("</html>");
	return result.toString();
    }

    private class SuperBlockDiagram extends XcosDiagram {
	private SuperBlock container = null;
	
	public SuperBlockDiagram(SuperBlock superBlock) {
	    super();
	    this.container = superBlock;
	}
	
	public void closeDiagram() {
	    boolean wantToClose = true;

	    if (this.undoManager.canUndo()) {
		// The diagram has been modified
		// Ask the user want he want to do !
		int choice = JOptionPane.showConfirmDialog(getAsComponent(), XcosMessages.DIAGRAM_MODIFIED);
		if (choice == 0) {
		    // Save the diagram
		    container.setRealParameters(BlockWriter.convertDiagramToMList(this));
		} else if (choice == 1) {
		    // The user selects no !
		} else if (choice == 2) {
		    // The user cancels
		    wantToClose = false;
		}
	    }

	    if (wantToClose) {
		ScilabWindow xcosWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(getParentTab().getParentWindowId());
		xcosWindow.removeTab(getParentTab());
		//palette.getAsSimpleWindow().close();
		getViewPort().close();
		Xcos.closeDiagram(this);
		container.setLocked(false);
	    }
	}
    }


}
