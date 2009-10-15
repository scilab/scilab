/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

import java.util.Hashtable;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxCellState;

/**
 * Block shadow handling
 * @author Vincent COUVERT
 */
public class ShowHideShadowAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated diagram
	 */
	public ShowHideShadowAction(ScilabGraph scilabGraph) {
		super(XcosMessages.SHOWHIDE_SHADOW, scilabGraph);
	}

	/**
	 * Menu added to the menubar
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.SHOWHIDE_SHADOW, null, new ShowHideShadowAction(scilabGraph), null);
	}
	
	/**
	 * Action !!
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
	    if (((XcosDiagram) getGraph(null)).getSelectionCells().length != 0) {
		
		Object[] allCells = ((XcosDiagram) getGraph(null)).getSelectionCells();
		
		for (int i = 0 ; i < allCells.length ; ++i) {
		    if (allCells[i] instanceof BasicBlock) {
			//((BasicBlock) allCells[i])
				mxCellState state = getGraph(null).getView().getState(allCells[i]);
				Hashtable style = (state != null) ? state.getStyle() : getGraph(null).getCellStyle(allCells[i]);

				if (style != null)
				{
					String value = (mxUtils.isTrue(style, mxConstants.STYLE_SHADOW, false)) ? "0" : "1";
					getGraph(null).setCellStyles(mxConstants.STYLE_SHADOW, value, new Object[] { allCells[i] });
				}
		    }
		}
	    }
	}

}
