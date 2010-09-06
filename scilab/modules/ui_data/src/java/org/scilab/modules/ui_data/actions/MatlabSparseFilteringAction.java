/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.actions;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * Manage MATLAB SPARSE display
 * @author Vincent COUVERT
 *
 */
public class MatlabSparseFilteringAction extends FilteringAction {

	/** Name of the action */
	public static final String NAME = UiDataMessages.MATLAB_SPARSE;
	
	private static final long serialVersionUID = -7371889324166887650L;

	/**
	 * Constructor
	 */
	public MatlabSparseFilteringAction() {
		super();
	}

	/**
	 * Create checkbox menu for the graph menu bar
	 * @return the menu
	 */
	public static CheckBoxMenuItem createCheckBoxMenu() {
		CheckBoxMenuItem menu = createCheckBoxMenu(new MatlabSparseFilteringAction());
		return menu;
	}

}
