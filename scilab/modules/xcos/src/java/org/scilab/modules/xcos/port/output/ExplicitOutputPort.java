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

package org.scilab.modules.xcos.port.output;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;

/**
 * An explicit output port is a port where the transfer of information has a
 * 'OUT' direction.
 */
public class ExplicitOutputPort extends OutputPort {

    private static final long serialVersionUID = -8199491929829046860L;

    public ExplicitOutputPort(JavaController controller, long uid, Kind kind, Object value, String style, String id) {
        super(controller, uid, kind, value, style == null || style.isEmpty() ? "ExplicitOutputPort" : style, id, false);
    }

    /**
     * @return always Type.EXPLICIT
     */
    @Override
    public Type getType() {
        return Type.EXPLICIT;
    }
}
