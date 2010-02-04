/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.actions.alignement;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Align block on the left
 */
public final class AlignBlockActionLeft extends AlignBlockAction {
	public static final String NAME = XcosMessages.ALIGN_LEFT;
	public static final String SMALL_ICON = "";
	public static final int MNEMONIC_KEY = 0;
	public static final int ACCELERATOR_KEY = 0;
	
	/**
	 * Default constructor
	 * @param scilabGraph the graph to work on
	 */
	public AlignBlockActionLeft(ScilabGraph scilabGraph) {
		super(scilabGraph, XcosConstants.ALIGN_LEFT);
	}
	
	/**
	 * Create the menu associated with this action.
	 * @param scilabGraph the associated graph
	 * @return The associated menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, AlignBlockActionLeft.class);
	}
}
