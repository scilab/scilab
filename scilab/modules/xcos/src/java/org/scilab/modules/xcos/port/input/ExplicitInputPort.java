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

package org.scilab.modules.xcos.port.input;

/**
 * An explicit output port is a port where the transfer of information has a
 * 'IN' direction.
 */
public class ExplicitInputPort extends InputPort {

    private static final long serialVersionUID = 3389508566614965375L;

    /** Default constructor */
    public ExplicitInputPort() {
        super("ExplicitInputPort");
    }

    /** @return always Type.EXPLICIT */
    @Override
    public Type getType() {
        return Type.EXPLICIT;
    }

}
