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

import java.io.File;
import java.io.IOException;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Diagram compilation management
 * @author Allan SIMON
 */
public class CompileAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated diagram
	 */
	public CompileAction(ScilabGraph scilabGraph) {
		super(XcosMessages.COMPILE, scilabGraph);
	}

	/**
	 * Create associated menu
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.COMPILE, null, new CompileAction(scilabGraph), null);
	}

	/**
	 * Action !!
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		File temp;
		try {
			temp = File.createTempFile("xcos", ".hdf5");
			temp.delete();
			((XcosDiagram) getGraph(null)).dumpToHdf5File(temp.getAbsolutePath());
			InterpreterManagement.requestScilabExec("import_from_hdf5(\"" + temp.getAbsolutePath() + "\");xcos_compile(scs_m);");
			temp.deleteOnExit();
		} catch (IOException e1) {
			e1.printStackTrace();
		}
	}
}
