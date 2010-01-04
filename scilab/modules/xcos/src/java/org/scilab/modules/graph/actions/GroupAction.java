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

package org.scilab.modules.graph.actions;

import java.awt.event.ActionEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;

import com.mxgraph.swing.util.mxGraphActions;

public class GroupAction extends DefaultAction {

	public GroupAction(ScilabGraph scilabGraph) {
		super(ScilabGraphMessages.GROUP, scilabGraph);
	}

	public static PushButton groupButton(ScilabGraph scilabGraph) {
		return createButton(ScilabGraphMessages.GROUP, null, new GroupAction(scilabGraph));
	}

	public static MenuItem groupMenu(ScilabGraph scilabGraph) {
		return createMenu(ScilabGraphMessages.GROUP, null, new GroupAction(scilabGraph), null);
	}

	public void actionPerformed(ActionEvent e) {
		mxGraphActions.getGroupAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(),
				e.getID(), e.getActionCommand()));
	}

}
