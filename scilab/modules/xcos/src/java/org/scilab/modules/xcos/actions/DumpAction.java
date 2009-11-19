/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
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
import java.io.File;
import java.io.IOException;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

public class DumpAction extends DefaultAction {

    public DumpAction(ScilabGraph scilabGraph) {
	super(XcosMessages.DUMP, scilabGraph);
    }

    public static PushButton dumpButton(ScilabGraph scilabGraph) {
	return createButton(XcosMessages.DUMP, null, new DumpAction(scilabGraph));
    }

    public static MenuItem dumpMenu(ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.DUMP, null, new DumpAction(scilabGraph), null);
    }

    public void actionPerformed(ActionEvent e) {
	try {
	    File temp = File.createTempFile("xcos",".hdf5");
	    temp.delete();
	    ((XcosDiagram) getGraph(e)).dumpToHdf5File(temp.getAbsolutePath());
	    InterpreterManagement.requestScilabExec("import_from_hdf5(\""+temp.getAbsolutePath()+"\");");
	} catch (IOException e1) {
	    // TODO Auto-generated catch block
	    e1.printStackTrace();
	}
    }
}
