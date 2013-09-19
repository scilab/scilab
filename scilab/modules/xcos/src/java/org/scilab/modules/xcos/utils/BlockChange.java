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

package org.scilab.modules.xcos.utils;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;

import com.mxgraph.util.mxUndoableEdit.mxUndoableChange;

/**
 * Specialize the mxGraph change to easily find it
 */
public class BlockChange implements mxUndoableChange {

    private final BasicBlock updated;
    private BasicBlock old;

    /**
     * Construct a new undoable edit with the given source
     *
     * @param updated
     *            The new state
     * @param old
     *            The old state
     */
    public BlockChange(BasicBlock updated, BasicBlock old) {
        this.updated = updated;
        this.old = old;
    }

    /**
     * Execute the current change.
     *
     * @see com.mxgraph.util.mxUndoableEdit.mxUndoableChange#execute()
     */
    @Override
    public void execute() {
        BasicBlock var;

        var = (BasicBlock) BlockFactory.createClone(updated);
        updated.updateBlockSettings(old);
        old = var;

    }
}
