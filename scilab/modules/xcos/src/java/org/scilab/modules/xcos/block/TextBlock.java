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

package org.scilab.modules.xcos.block;

import java.util.Map;

import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.xcos.actions.BlockParametersAction;
import org.scilab.modules.xcos.actions.RegionToSuperblockAction;

public class TextBlock extends BasicBlock {

    public TextBlock() {
	super();
	setVertex(false);
	setVisible(false);
    }

    public TextBlock(String label) {
	super(label);
	setInterfaceFunctionName("TEXT_f");
    }

    /**
     * @return the text
     */
    public String getText() {
	return ((ScilabString) getExprs()).getData()[0][0];
    }
    
    /**
     * @return the fontNumber
     */
    public int getFontNumber() {
	return Integer.parseInt(((ScilabString) getExprs()).getData()[1][0]);
    }

    /**
     * @return the fontSize
     */
    public int getFontSize() {
	return Integer.parseInt(((ScilabString) getExprs()).getData()[2][0]);
    }
    
    /**
     * Disabling BlockSettings action
     */
    @Override
    public void openBlockSettings(String context[]) {
	// NOTHING TO BE DONE
    }
    
    /**
     * Disabling BlockSettings action
     */
    @Override
    public void updateBlockSettings(BasicBlock modifiedBlock) {
	// NOTHING TO BE DONE
    }
    
    /**
     * Customize menu
     */
    @Override
    protected void customizeMenu(
            Map<Class<? extends DefaultAction>, MenuItem> menuList) {
        menuList.get(BlockParametersAction.class).setEnabled(false);
        menuList.get(RegionToSuperblockAction.class).setEnabled(false);
    }
}
