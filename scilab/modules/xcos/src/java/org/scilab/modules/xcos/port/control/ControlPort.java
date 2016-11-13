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

package org.scilab.modules.xcos.port.control;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.PortKind;
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
    public ControlPort(JavaController controller, long uid, Kind kind, Object value, String style, String id) {
        super(controller, uid, kind, value, style == null || style.isEmpty() ? "ControlPort" : style, id, Orientation.NORTH, false, PortKind.PORT_EIN);
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

    @Override
    public PortKind getPortKind() {
        return PortKind.PORT_EIN;
    }

}
