/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.utils.ScilabAboutBox;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.utils.XcosMessages;


/**
 * Display about box for Xcos
 * @author Allan SIMON
 * @author Vincent COUVERT
 */
public class AboutXcosAction extends DefaultAction {

	private static final long serialVersionUID = -7307118101829056965L;

	/**
	 * Constructor
	 * @param scilabGraph associated Scilab graph
	 */
	public AboutXcosAction(ScilabGraph scilabGraph) {
		super(XcosMessages.ABOUT_XCOS, scilabGraph);
	}

	/**
	 * Menu created for "Help menu"
	 * @param scilabGraph associated Scilab graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.ABOUT_XCOS, null, new AboutXcosAction(scilabGraph), null);
	}
	
	/**
	 * Action !!
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		String[] contents = {
				Xcos.TRADENAME + " - " + Xcos.VERSION,
				"",
				XcosMessages.DIGITEO,
				XcosMessages.COPYRIGHT_INRIA,
				"",
				XcosMessages.SCICOS_BASED};
		
		ScilabAboutBox.createAboutBox(XcosMessages.ABOUT_XCOS, contents, null, null);
    	
    }
}
