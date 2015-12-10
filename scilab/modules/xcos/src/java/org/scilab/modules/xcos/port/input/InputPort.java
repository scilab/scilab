/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.port.input;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.PortKind;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

/**
 * An input port acts as a protection barrier between a link and the internal
 * functions of the block.
 *
 * It's default orientation is on the block's WEST side and it <b>must</b> be
 * connected to an {@link org.scilab.modules.xcos.port.output.OutputPort} .
 */
public abstract class InputPort extends BasicPort {
    private static final long serialVersionUID = 0L;

    /**
     * Default constructor
     */
    public InputPort(JavaController controller, long uid, Kind kind, Object value, String style, String id, boolean isImplicit) {
        super(controller, uid, kind, value, style, id, Orientation.WEST, isImplicit, PortKind.PORT_IN);
    }

    @Override
    public PortKind getPortKind() {
        return PortKind.PORT_IN;
    }
}
