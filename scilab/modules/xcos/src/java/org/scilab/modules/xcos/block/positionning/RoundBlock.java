/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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
import org.scilab.modules.xcos.block.listener.ProdPortLabelingListener;
import org.scilab.modules.xcos.block.listener.SumPortLabelingListener;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.input.InputPort;

import com.mxgraph.model.mxICell;

/**
 * Implement a round block with inputs spread around the block.
 */
@SuppressWarnings(value = { "serial" })
public class RoundBlock extends BasicBlock {
    /**
     * Default constructor
     */
    public RoundBlock() {
        /*
         * Default value of the round block, for more information refers to
         * BlockFactory instantiation order.
         */
        this("CLKSOMV_f");
    }

    /**
     * Set default values
     *
     * @param interFunction
     *            the interfunction (label) string
     */
    public RoundBlock(String interFunction) {
        super();
        setInterfaceFunctionName(interFunction);
    }

    /**
     * Reinstall the property change listener when the interfunction change.
     *
     * @param interfaceFunctionName
     *            the new name
     * @see org.scilab.modules.xcos.block.BasicBlock#setInterfaceFunctionName(java.lang.String)
     */
    @Override
    public void setInterfaceFunctionName(String interfaceFunctionName) {
        getParametersPCS().removePropertyChangeListener(
            SumPortLabelingListener.getInstance());
        getParametersPCS().removePropertyChangeListener(
            ProdPortLabelingListener.getInstance());

        super.setInterfaceFunctionName(interfaceFunctionName);

        if (interfaceFunctionName.equals("SUM_f")) {
            getParametersPCS().addPropertyChangeListener("integerParameters",
                    SumPortLabelingListener.getInstance());
        } else if (interfaceFunctionName.equals("PROD_f")) {
            getParametersPCS().addPropertyChangeListener("realParameters",
                    ProdPortLabelingListener.getInstance());
        }
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
            port.setOrientation(getPortOrientation(port.getOrdering()));

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
    // CSOFF: MagicNumber
    private Orientation getPortOrientation(int order) {
        final Orientation ret;

        switch (order) {
            case 1:
                ret = Orientation.SOUTH;
                break;

            case 2:
                ret = Orientation.WEST;
                break;

            case 3:
                ret = Orientation.NORTH;
                break;

            default:
                ret = Orientation.WEST;
                break;
        }
        return ret;
    }
    // CSON: MagicNumber
}
