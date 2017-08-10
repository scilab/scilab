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

package org.scilab.modules.xcos.port.command;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.PortKind;
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
    private static final long serialVersionUID = 0L;

    /** Default constructor */
    public CommandPort(JavaController controller, long uid, Kind kind, Object value, String style, String id) {
        super(controller, uid, kind, value, style == null || style.isEmpty() ? "CommandPort" : style, id, Orientation.SOUTH, false, PortKind.PORT_EOUT);
    }

    @Override
    public Type getType() {
        return null;
    }

    @Override
    public PortKind getPortKind() {
        return PortKind.PORT_EOUT;
    }
}
