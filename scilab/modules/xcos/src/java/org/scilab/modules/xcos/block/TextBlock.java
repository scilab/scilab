/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.xcos.block;

import com.mxgraph.model.mxGeometry;
import java.util.Map;

import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.xcos.JavaController;
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
    public TextBlock(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super(controller, uid, kind, value, geometry, style, id);
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
