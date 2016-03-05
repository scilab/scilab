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

package org.scilab.modules.xcos.link;

import com.mxgraph.model.mxGeometry;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;

/**
 * An command control link connect an
 * {@link org.scilab.modules.xcos.port.control.ControlPort} to an
 * {@link org.scilab.modules.xcos.port.command.CommandPort}.
 */
public class CommandControlLink extends BasicLink {

    private static final long serialVersionUID = 5092221307994387276L;
    // 5 means red color
    // -1 means activation link
    private static final double[][] COLOR_AND_TYPE = { { 5.0, -1.0 } };

    public CommandControlLink(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super(controller, uid, kind, value, geometry, style == null || style.isEmpty() ? "CommandControlLink" : style, id, -1);
    }

    /** @return A red colored activation link identifier */
    @Override
    public double[][] getColorAndType() {
        return COLOR_AND_TYPE;
    }

    @Override
    public int getLinkKind() {
        return -1;
    }



}
