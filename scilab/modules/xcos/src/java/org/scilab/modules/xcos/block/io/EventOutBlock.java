/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS <antoine.elias@scilab.org>
 * Copyright (C) 2009-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2010-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.block.io;

import com.mxgraph.model.mxGeometry;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;

/**
 * Implement an event exit point of a
 * {@link org.scilab.modules.xcos.block.SuperBlock}.
 */
public final class EventOutBlock extends ContextUpdate {

    private static final long serialVersionUID = 6585655980747217421L;

    /**
     * Constructor
     */
    public EventOutBlock(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super(controller, uid, kind, value, geometry, style, id);

        controller.setObjectProperty(uid, kind, ObjectProperties.SIM_FUNCTION_NAME, "output");
    }

    public EventOutBlock(JavaController controller) {
        super(controller, "CLKOUTV_f");

        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.SIM_FUNCTION_NAME, "output");
    }
}
