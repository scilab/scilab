/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.xcos.block;

import java.util.logging.Logger;

import org.scilab.modules.xcos.port.BasicPort;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;

/**
 * A SplitBlock is used on a junction between links.
 */
public final class SplitBlock extends BasicBlock {

    /** The default size */
    public static final int DEFAULT_SIZE = 7;
    /** The default color value */
    public static final int DEFAULT_COLOR = 7;

    /**
     * Constructor
     */
    public SplitBlock(long uid) {
        super(uid);
    }

    /**
     * Add a port on the block.
     *
     * @param child
     *            the port to add
     * @param index
     *            index where to put the child
     */
    @Override
    public mxICell insert(mxICell child, int index) {
        child.setVisible(false);
        return super.insert(child, index);
    }

    /**
     * @return input port
     */
    public BasicPort getIn() {
        return (BasicPort) getChildAt(0);
    }

    /**
     * @return first output port
     */
    public BasicPort getOut1() {
        return (BasicPort) getChildAt(1);
    }

    /**
     * @return second output port
     */
    public BasicPort getOut2() {
        return (BasicPort) getChildAt(2);
    }

    /**
     * Set the geometry of the block
     *
     * @param geometry
     *            change split block geometry
     */
    @Override
    public void setGeometry(mxGeometry geometry) {
        if (geometry != null) {
            geometry.setWidth(DEFAULT_SIZE);
            geometry.setHeight(DEFAULT_SIZE);
        }

        super.setGeometry(geometry);
    }

    /**
     * @return true if the split is connectable, false otherwise
     * @see org.scilab.modules.xcos.block.BasicBlock#isConnectable()
     */
    @Override
    public boolean isConnectable() {
        if (getChildCount() != 0) {
            boolean hasNoEdges = true;
            for (int i = 0; i < getChildCount() && hasNoEdges; i++) {
                final mxICell cell = getChildAt(i);
                hasNoEdges = cell.getEdgeCount() == 0;
            }

            return hasNoEdges;
        } else {
            return false;
        }
    }

    /**
     * Insert edges as children port edges
     *
     * @param edge
     *            the current edge
     * @param isOutgoing
     *            if it is an input port or output one
     * @return the inserted edges
     * @see com.mxgraph.model.mxCell#insertEdge(com.mxgraph.model.mxICell,
     *      boolean)
     */
    @Override
    public mxICell insertEdge(mxICell edge, boolean isOutgoing) {
        final mxICell ret;

        if (isOutgoing) {
            if (getOut1().getEdgeCount() == 0) {
                ret = getOut1().insertEdge(edge, isOutgoing);
            } else if (getOut2().getEdgeCount() == 0) {
                ret = getOut2().insertEdge(edge, isOutgoing);
            } else {
                ret = null;
            }
        } else {
            if (getIn().getEdgeCount() == 0) {
                ret = getIn().insertEdge(edge, isOutgoing);
            } else {
                ret = null;
            }
        }

        if (ret == null) {
            Logger.getLogger(SplitBlock.class.getName()).severe("Unable to connect : " + edge);
            return super.insertEdge(edge, isOutgoing);
        } else {
            return ret;
        }
    }
}
