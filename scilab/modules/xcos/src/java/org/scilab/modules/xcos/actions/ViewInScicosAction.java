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

public class ViewInScicosAction  extends DefaultAction {
    public static PushButton viewInScicosButton(ScilabGraph scilabGraph) {
	return createButton("View In Scicos", null, new ViewInScicosAction(scilabGraph));
    }

    public static MenuItem viewInScicosMenu(ScilabGraph scilabGraph) {
	return createMenu("View In Scicos", null, new ViewInScicosAction(scilabGraph));
    }

    public ViewInScicosAction() {
	super();
    }

    public ViewInScicosAction(ScilabGraph scilabGraph) {
	super(scilabGraph);
    }

    public void actionPerformed(ActionEvent e) {
	File temp;
	try {
	    temp = File.createTempFile("xcos",".hdf5");
	    System.err.println("File = "+temp.getAbsolutePath());
	    ((XcosDiagram) getGraph(e)).dumpToHdf5File(temp.getAbsolutePath());
	    InterpreterManagement.requestScilabExec("import_from_hdf5(\""+temp.getAbsolutePath()+"\");scicos(scs_m);");
	    temp.deleteOnExit();
	} catch (IOException e1) {
	    e1.printStackTrace();
	}
    }
}
