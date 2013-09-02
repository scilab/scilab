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

import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.BasicPort.Type;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;

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

    private static final long serialVersionUID = 5817243367840540106L;

    /**
     * Constructor
     */
    public SplitBlock() {
        super();
    }

    /**
     * Add connection port depending on the type of the source.
     *
     * @param source
     *            the type of the split
     */
    public void addConnection(BasicPort source) {
        if (source.getType() == Type.EXPLICIT) {
            addPort(new ExplicitInputPort());
            addPort(new ExplicitOutputPort());
            addPort(new ExplicitOutputPort());

            setInterfaceFunctionName("SPLIT_f");
        } else if (source.getType() == Type.IMPLICIT) {
            addPort(new ImplicitInputPort());
            addPort(new ImplicitOutputPort());
            addPort(new ImplicitOutputPort());

            setInterfaceFunctionName("IMPSPLIT_f");
        } else {
            addPort(new ControlPort());
            addPort(new CommandPort());
            addPort(new CommandPort());

            setInterfaceFunctionName("CLKSPLIT_f");
        }

        getChildAt(0).setVisible(false);
        getChildAt(1).setVisible(false);
        getChildAt(2).setVisible(false);
    }

    /**
     * Initialize the block with the default values
     */
    @Override
    protected void setDefaultValues() {
        super.setDefaultValues();
        setInterfaceFunctionName("SPLIT_f");
        setStyle(getInterfaceFunctionName());
        setSimulationFunctionName("lsplit");
        setRealParameters(new ScilabDouble());
        setIntegerParameters(new ScilabDouble());
        setObjectsParameters(new ScilabList());
        setExprs(new ScilabDouble());
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
        sortChildren();
        return (BasicPort) getChildAt(0);
    }

    /**
     * @return first output port
     */
    public BasicPort getOut1() {
        sortChildren();
        return (BasicPort) getChildAt(1);
    }

    /**
     * @return second output port
     */
    public BasicPort getOut2() {
        sortChildren();
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

            /*
             * Align the geometry on the grid
             */
            if (getParentDiagram() != null && getParentDiagram().isGridEnabled()) {
                final double cx = getParentDiagram().snap(geometry.getCenterX());
                final double cy = getParentDiagram().snap(geometry.getCenterY());

                geometry.setX(cx - (DEFAULT_SIZE / 2));
                geometry.setY(cy - (DEFAULT_SIZE / 2));
            }
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
