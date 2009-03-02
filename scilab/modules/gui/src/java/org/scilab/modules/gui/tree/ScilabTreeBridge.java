/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.tree;

import org.scilab.modules.gui.bridge.tree.SwingScilabTree;

/**
 * Bridge for Scilab tree in GUIs
 * @author Sylvestre KOUMAR
 */
public class ScilabTreeBridge {

	public static void showTree(Tree tree) {
		SwingScilabTree.showTree(tree);
	}

	public static SimpleTree createTree(Tree scilabTree) {
	    return new SwingScilabTree(scilabTree);
	}
}
