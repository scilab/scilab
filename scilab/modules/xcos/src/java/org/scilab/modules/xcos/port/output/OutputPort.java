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

package org.scilab.modules.xcos.port.output;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.PortKind;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

/**
 * An output port acts as a protection barrier between the internal functions of
 * the block and a link.
 *
 * It's default orientation is on the block's EAST side and it <b>must</b> be
 * connected to an {@link org.scilab.modules.xcos.port.input.InputPort} .
 */
public abstract class OutputPort extends BasicPort {
    private static final long serialVersionUID = 0L;

    /**
     * Default constructor
     */
    public OutputPort(JavaController controller, long uid, Kind kind, Object value, String style, String id, boolean isImplicit) {
        super(controller, uid, kind, value, style, id, Orientation.EAST, isImplicit, PortKind.PORT_OUT);
    }

    @Override
    public PortKind getPortKind() {
        return PortKind.PORT_OUT;
    }
}
