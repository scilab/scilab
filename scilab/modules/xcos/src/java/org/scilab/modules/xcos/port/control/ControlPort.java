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

package org.scilab.modules.xcos.port.control;

import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

/**
 * A control port acts as the execution tick for the block.
 *
 * The block is executed only when a rising edge occurs on this port. As we are
 * writing 'edge', this port can only be connected to a
 * {@link org.scilab.modules.xcos.port.command.CommandPort}. The port default
 * orientation is the NORTH.
 */
public class ControlPort extends BasicPort {

    private static final long serialVersionUID = -7919292589336989591L;

    /** Default constructor */
    public ControlPort() {
        super("ControlPort");
        setOrientation(Orientation.NORTH);

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

}
