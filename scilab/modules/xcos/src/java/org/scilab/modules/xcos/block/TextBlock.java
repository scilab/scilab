/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.block;

import java.util.Map;

import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;

/**
 * A textblock is used to annotate diagrams.
 */
@SuppressWarnings(value = { "serial" })
public final class TextBlock extends BasicBlock {
    /**
     * Default constructor
     */
    public TextBlock(long uid) {
        super(uid, Kind.ANNOTATION);
    }

    /**
     * Customize menu
     *
     * @param menuList
     *            the menuList to work on
     */
    @Override
    protected void customizeMenu(Map < Class <? extends DefaultAction > , Menu > menuList) {
        menuList.get(BlockParametersAction.class).setEnabled(false);
        menuList.get(RegionToSuperblockAction.class).setEnabled(false);
    }
}
