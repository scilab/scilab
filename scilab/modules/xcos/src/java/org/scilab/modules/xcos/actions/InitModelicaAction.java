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
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabInterpreterManagement;
import org.scilab.modules.graph.utils.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Launch the modelica compiler configuration TCL UI
 */
public class InitModelicaAction extends DefaultAction {
    public static final String NAME = XcosMessages.INIT_MODELICA;
    public static final String SMALL_ICON = "";
    public static final int MNEMONIC_KEY = 0;
    public static final int ACCELERATOR_KEY = 0;
    
    /**
     * Constructor
     * @param scilabGraph corresponding Scilab Graph
     */
    public InitModelicaAction(ScilabGraph scilabGraph) {
	super(scilabGraph);
    }
    /**
     * Create a button for a graph toolbar
     * @param scilabGraph corresponding Scilab Graph
     * @return the button
     */
    public static PushButton createButton(ScilabGraph scilabGraph) {
	return createButton(scilabGraph, InitModelicaAction.class);
    }

    /**
     * Create a menu for a graph menubar
     * @param scilabGraph corresponding Scilab Graph
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(scilabGraph, InitModelicaAction.class);
    }

    /**
     * Action associated
     * @param e the event
     * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
	File temp;

	try {
	    ((XcosDiagram) getGraph(null)).info(XcosMessages.INITIALIZING_MODELICA_COMPILER);
	    temp = File.createTempFile("xcos", ".h5", XcosConstants.TMPDIR);
	    ((XcosDiagram) getGraph(e)).getRootDiagram().dumpToHdf5File(temp.getAbsolutePath());

	    String command = "import_from_hdf5(\"" + temp.getAbsolutePath() + "\"); "
	    + "xcosConfigureModelica();"
	    + "deletefile(\"" + temp.getAbsolutePath() + "\");";
	    try {
		ScilabInterpreterManagement.asynchronousScilabExec(command, new ActionListener() {
		    public void actionPerformed(ActionEvent arg0) {
			((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
		    }
		});
	    } catch (InterpreterException e1) {
		e1.printStackTrace();
	    }
	} catch (IOException e1) {
	    e1.printStackTrace();
	}  
    }
}
