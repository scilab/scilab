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

package org.scilab.modules.xcos.port.command;

import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

/**
 * A command port acts generate execution ticks to another block.
 *
 * The connected block is executed only when a rising edge occurs on this port.
 * As we are writing 'edge', this port can only be connected to a
 * {@link org.scilab.modules.xcos.port.control.ControlPort}. To specify initial
 * conditions, this block offers a {@link #setInitialState(double)} method. The
 * port default orientation is the SOUTH.
 */
public class CommandPort extends BasicPort {

    private static final long serialVersionUID = 8098231554414576405L;
    private double initialState = -1.0;

    /** Default constructor */
    public CommandPort() {
        super("CommandPort");
        setOrientation(Orientation.SOUTH);

        setDefaultValues();
    }

    /**
     * This port is untyped
     *
     * @return always null
     */
    @Override
    public Type getType() {
        return null;
    }

    /**
     * @param initialState
     *            Initial state value
     */
    public void setInitialState(double initialState) {
        this.initialState = initialState;
    }

    /** @return Initial state value */
    public double getInitialState() {
        return initialState;
    }
}
