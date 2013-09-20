/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.block.positionning;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

import com.mxgraph.model.mxICell;

/**
 * The Ground block has only one specificity : it's port position.
 */
@SuppressWarnings(value = { "serial" })
public class GroundBlock extends BasicBlock {

    /** Default constructor */
    public GroundBlock() {
        super();
    }

    /**
     * Set the orientation before calling parent method.
     *
     * @param child
     *            the port to add.
     * @param index
     *            the index of the add
     * @return the inserted cell
     */
    @Override
    public mxICell insert(mxICell child, int index) {
        if (child instanceof BasicPort) {
            ((BasicPort) child).setOrientation(Orientation.NORTH);
        }
        return super.insert(child, index);
    }
}
