/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

import java.util.logging.Logger;

import org.scilab.modules.xcos.port.BasicPort;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;

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
    public SplitBlock(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super(controller, uid, kind, value, new mxGeometry(geometry == null ? DEFAULT_POSITION_X : geometry.getX(), geometry == null ? DEFAULT_POSITION_Y : geometry.getY(), DEFAULT_SIZE, DEFAULT_SIZE), style, id);
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
