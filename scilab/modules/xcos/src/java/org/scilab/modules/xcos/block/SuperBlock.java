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

import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.xcos.XcosDiagram;

public class SuperBlock extends BasicBlock {

    public SuperBlock(String label) {
	super(label);
    }

    public void openBlockSettings(String context) {
	this.setLocked(true);	
	XcosDiagram xcosDiagram = new XcosDiagram() {
	    
	};
	xcosDiagram.loadDiagram(BlockReader.convertMListToDiagram((ScilabMList) getRealParameters()));
	

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
    
}
