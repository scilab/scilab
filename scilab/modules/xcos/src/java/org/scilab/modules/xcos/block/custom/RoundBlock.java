/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2010-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.block.custom;

import com.mxgraph.model.mxGeometry;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.input.InputPort;

import com.mxgraph.model.mxICell;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;

/**
 * Implement a round block with inputs spread around the block.
 */
@SuppressWarnings(value = { "serial" })
public class RoundBlock extends BasicBlock {
    /**
     * Default constructor
     */
    public RoundBlock(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super(controller, uid, kind, value, geometry, style, id);
    }

    /**
     * Insert a port into this block.
     *
     * @param child
     *            the port to add
     * @param index
     *            the index
     * @return the inserted cell
     */
    @Override
    public mxICell insert(mxICell child, int index) {
        /*
         * Any input are placed around the block.
         */
        if (child instanceof InputPort) {
            final InputPort port = (InputPort) child;
            port.setOrientation(getInputPortOrientation(index));

        }

        return super.insert(child, index);
    }

    /**
     * Get the Orientation from the order
     *
     * @param order
     *            the port ordering
     * @return the selected orientation
     */
    private Orientation getInputPortOrientation(int order) {
        final Orientation ret;

        switch (order) {
            case 0:
                ret = Orientation.SOUTH;
                break;
            case 1:
                ret = Orientation.WEST;
                break;
            case 2:
                ret = Orientation.NORTH;
                break;
            default:
                ret = Orientation.WEST;
                break;
        }
        return ret;
    }
}
