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
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.Signal;
import org.scilab.modules.xcos.utils.XcosMessages;

public class StartAction  extends DefaultAction {

    private static final long serialVersionUID = -7548486977403506053L;
    private static String simulationEnd = "__simulationEnd__";

    public StartAction(ScilabGraph scilabGraph) {
	super(XcosMessages.START, scilabGraph);
    }

    public static PushButton createButton(ScilabGraph scilabGraph) {
	return createButton(XcosMessages.START, "media-playback-start.png", new StartAction(scilabGraph));
    }

    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.START, null, new StartAction(scilabGraph), null);
    }

    public void actionPerformed(ActionEvent e) {
	File temp;
	XcosTab.setStartEnabled(false);
	((XcosDiagram) getGraph(null)).info(XcosMessages.SIMULATION_IN_PROGRESS);
	((XcosDiagram) getGraph(null)).getParentTab().getInfoBar().draw();
	try {
	    temp = File.createTempFile("xcos",".h5");
	    temp.deleteOnExit();
	    ((XcosDiagram) getGraph(e)).getRootDiagram().dumpToHdf5File(temp.getAbsolutePath());
	    Thread launchMe = new Thread() {
		public void run() {
		    Signal.wait(simulationEnd);
		    ((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
		    XcosTab.setStartEnabled(true);
		}
	    };
	    launchMe.start();
	    InterpreterManagement.requestScilabExec("import_from_hdf5(\""+temp.getAbsolutePath()+"\");"
		    +"scicos_debug("+((XcosDiagram) getGraph(e)).getDebugLevel()+");"
		    +"xcos_simulate(scs_m);"
		    +"xcosNotify(\"" + simulationEnd + "\");"
		    +"deletefile(\"" + temp.getAbsolutePath()+"\");");
	} catch (IOException e1) {
	    e1.printStackTrace();
	    XcosTab.setStartEnabled(true);
	}
    }
}
